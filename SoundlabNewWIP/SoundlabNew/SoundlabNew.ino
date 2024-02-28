#include <Encoder.h>
#include <EEPROM.h>

Encoder knobOne(7, 8);
Encoder knobTwo(5, 6);
long positionLeft = -999;
long positionRight = -999;
const int amount = 8;
const int sensorPin[amount] = { 16, 17, 18, 19, 20, 21, 22, 23 };
int sensorValue[amount] = { 0, 0, 0, 0, 0, 0, 0, 0 };
int scaledSensorValue[amount] = { 0, 0, 0, 0, 0, 0, 0, 0 };
int sensorMin[amount] = { 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023 };
int sensorMax[amount] = { 0, 0, 0, 0, 0, 0, 0, 0 };
//buttonstarts here
const int BUTTON1_PIN = 2;  // the number of the pushbutton pin
const int BUTTON2_PIN = 3;  // the number of the pushbutton pin
// Variables will change:
int lastState = HIGH;   // the previous state from the input pin
int lastState2 = HIGH;  // the previous state from the input pin
int buttonState = HIGH;
int buttonState2 = HIGH;
int lastButtonState = HIGH;   // the previous reading from the input pin
int lastButtonState2 = HIGH;  // the previous reading from the input pin
unsigned long lastDebounceTime = 0;   // the last time the output pin was toggled
unsigned long lastDebounceTime2 = 0;  // the last time the output pin was toggled
bool clicked = false;   //creates button toggle
bool clicked2 = false;  //creates button toggle
unsigned long debounceDelay = 50;
int readingButton;
int calibrationState = 0;
int menuState = 0;
float volume = 127;
long scaledLeft, scaledRight, menuScaledLeft;
int preset = 0;
bool calibrating = false;
bool menuSwitch = false;
bool pressed = false;
unsigned int addr = 0;


void setup() {
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);

  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  Serial.begin(115200);
  readCalibration();
  Serial.println("boot");
  blink(255, 255, 255, 50);
}

void loop() {
  //Read Data
  readSensors();
  readEncoderPos();
  readButton();

  if (buttonState == LOW && buttonState2 == LOW) {
    menuSwitch = true;  //activate menu
    pressed = true;
  }

  if (menuSwitch) {
    menu();
  } else {
    volume = scaledLeft;
    if (volume == 127) {
      colorLed(255, 100, 100);
    } else {
      int volumeScaled = volume;
      if (volumeScaled >= 200) {
        volumeScaled = 200;
      }
      colorLed(volumeScaled, volume * 0.5, volume * 0.5);
    }
  }

  midi();
  // delay(20);  //unnecessary
}

void menu() {
  while (pressed) {
    readButton();
    if (buttonState == HIGH && buttonState2 == HIGH) {
      pressed = false;  //activate menu
    }
  }

  if (buttonState == LOW && menuState == 0) {  //Calibration
    colorLed(50, 50, 50);                      // wave led
    calibrating = true;
    for (int i = 0; i < amount; i++) {
      sensorMin[i] = 1023;
      sensorMax[i] = 0;
    }
    Serial.println("Calibrate min values.");

    while (calibrating) {
      if (calibrationState == 0) {
        readButton();
        for (int i = 0; i < amount; i++) {  // recording min values
          sensorValue[i] = analogRead(sensorPin[i]);
          if (sensorValue[i] < sensorMin[i]) {
            sensorMin[i] = sensorValue[i];
            blink(255, 0, 0, 25);
          }
        }

        if (buttonState2 == LOW) {
          calibrationState = 1;
          Serial.println("Calibrate max values.");
        }
      }

      if (calibrationState == 1) {
        readButton();
        delay(50);
        for (int i = 0; i < amount; i++) {  // recording max values
          sensorValue[i] = analogRead(sensorPin[i]);
          if (sensorValue[i] > sensorMax[i]) {
            sensorMax[i] = sensorValue[i];
            blink(255, 0, 0, 25);
          }
        }

        if (buttonState == LOW) {
          writeCalibration(sensorMin, sensorMax);
          calibrating = false;
          calibrationState = 0;
          Serial.println("Calibration done.");
          menuSwitch = false;
        }
      }
    }

  } else if (buttonState == LOW && menuState == 1) {  //Change Preset
    if (preset == 0) {
      preset = 1;
    } else {
      preset = 0;
    }
    menuSwitch = false;
  } else if (buttonState == LOW && menuState == 2) {  //Cancel Menu
    menuSwitch = false;
  }

  if (menuSwitch) {
    if (menuState == 0) {
      colorLed(255, 0, 255);
    } else if (menuState == 1) {
      colorLed(0, 255, 0);
    } else if (menuState == 2) {
      colorLed(0, 0, 255);
    }
  }

}

void writeCalibration(int min[], int max[]) {
  for (int i = 0; i < amount; i++) {
    EEPROM.write(i, min[i] / 4);
  }
  for (int i = 0; i < amount; i++) {
    EEPROM.write(i + amount, max[i] / 4);
  }
  readCalibration();
}

void readCalibration() {
  for (int i = 0; i < amount; i++) {
    // Serial.print(i);
    // Serial.print(" , MinE = ");
    // Serial.print(EEPROM.read(i));
    sensorMin[i] = EEPROM.read(i);
    // Serial.print(" , MaxE = ");
    // Serial.print(EEPROM.read(i + amount));
    sensorMax[i] = EEPROM.read(i + amount);

    // Serial.print(", minV = ");
    // Serial.print(sensorMin[i]);
    // Serial.print(", maxV = ");
    // Serial.println(sensorMax[i]);
  }
}

void readSensors() {
  //read StretchSensors and Serial Send data
  for (int i = 0; i < amount; i++) {
    //read sensordata
    sensorValue[i] = analogRead(sensorPin[i]);
    scaledSensorValue[i] = map(analogRead(sensorPin[i]), sensorMin[i] * 4, sensorMax[i] * 4, 0, 127);
  };
}

bool knopCheck(int pin, int pinChoice) {
  //check for selected button if pressed
  bool result = false;
  readingButton = digitalRead(pin);

  if (pinChoice == 1) {

    if (readingButton != lastButtonState) {
      lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {
      if (readingButton != buttonState) {
        buttonState = readingButton;
        if (buttonState == LOW && !clicked) {
          result = true;
          clicked = true;
        }
        if (buttonState == HIGH && clicked) {
          result = false;
          clicked = false;
        }
      }
    }
    lastButtonState = readingButton;

  } else if (pinChoice == 2) {
    // reading = digitalRead(pin);

    if (readingButton != lastButtonState2) {
      lastDebounceTime2 = millis();
    }

    if ((millis() - lastDebounceTime2) > debounceDelay) {
      if (readingButton != buttonState2) {
        buttonState2 = readingButton;
        if (buttonState2 == LOW && !clicked) {
          result = true;
          clicked2 = true;
        }
        if (buttonState2 == HIGH && clicked) {
          result = false;
          clicked2 = false;
        }
      }
    }
    lastButtonState2 = readingButton;
  }

  return result;
}

void readEncoderPos() {
  //read encoderpositions and scale them to 0 - 127
  long newLeft, newRight;
  newLeft = knobOne.read();

  scaledLeft = newLeft / 4;
  if (scaledLeft < 0) {
    scaledLeft = 0;
  }
  setMenuState(scaledLeft % 3);

  newRight = knobTwo.read();
  scaledRight = newRight / 4;

  if (newLeft != positionLeft || newRight != positionRight) {
    positionLeft = newLeft;
    positionRight = newRight;
  }
  if (scaledRight >= 127) {
    scaledRight = 127;
  } else if (scaledRight <= 0) {
    scaledRight = 0;
  }

  if (scaledLeft >= 127) {
    scaledLeft = 127;
  } else if (scaledLeft <= 0) {
    scaledLeft = 0;
  }
}

void readButton() {
  //check both buttons for state
  knopCheck(BUTTON1_PIN, 1);
  knopCheck(BUTTON2_PIN, 2);
}

void midi() {  //send midi message: (Controller number, Control Value, Midi Channel) (CN 1-8 = sensors, CN 9 = Volume, CN 10 = preset)
  for (int i = 0; i < amount; i++) {
    usbMIDI.sendControlChange(i + 1, scaledSensorValue[i], 1);
  }
  usbMIDI.sendControlChange(9, volume, 1);
  usbMIDI.sendControlChange(10, preset, 1);
}

void colorLed(int r, int g, int b) {
  analogWrite(9, b);
  analogWrite(10, g);
  analogWrite(11, r);
}

void blink(int r, int g, int b, int dur) {
  colorLed(r, g, b);
  delay(dur);
  colorLed(50, 50, 50);
  delay(dur);
}

void setMenuState(int x) {
  menuState = x;
  // Serial.print("menuState: ");
  // Serial.println(menuState);
}
