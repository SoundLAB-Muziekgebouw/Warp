#include <Encoder.h>

Encoder knobOne(7, 8);
Encoder knobTwo(5, 6);
long positionLeft = -999;
long positionRight = -999;

const int amount = 8;
float smoothingFactor = 0.09;

const int sensorPin[amount] = { 16, 17, 18, 19, 20, 21, 22, 23 };
int sensorValue[amount] = { 0, 0, 0, 0, 0, 0, 0, 0 };
int scaledSensorValue[amount] = { 0, 0, 0, 0, 0, 0, 0, 0 };

int sensorMax[amount] = { 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023 };
int sensorMin[amount] = { 0, 0, 0, 0, 0, 0, 0, 0 };

int menuHeader = 0;
int menuSettings[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

int state = 0;

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

String onOff;

int readingButton;
int calibrationState = 0;
int volume = 127;
long scaledLeft, scaledRight, menuScaledLeft;
int preset = 0;




void setup() {
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);

  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  Serial.begin(115200);
  Serial.println("calibrate min values:");
}

void loop() {
  //Read Data
  readSensors();
  readEncoderPos();
  readButton();
  calibrate();
  delay(20);
}

void calibrate() {
  if (calibrationState == 0) {
    volume = scaledLeft;
  }
  if (buttonState == LOW && buttonState2 == LOW) {
    calibrationState = 1;
    Serial.print(calibrationState);
    Serial.println(" :calibrationStat");

  } else if (buttonState == LOW && calibrationState == 1 && menuScaledLeft == 1) {
    calibrationState = 2;
    Serial.print(calibrationState);
    Serial.println(" :calibrationStat");

    Serial.println("calibrate min values:");
  } else if (buttonState2 == LOW && calibrationState == 2) {
    calibrationState = 3;
    for (int i = 0; i < amount; i++) {
      sensorMin[i] = sensorValue[i];
      Serial.println(sensorMin[i]);
    }
    Serial.print(calibrationState);
    Serial.println(" :calibrationStat");

    Serial.println("calibrate max values:");


  } else if (buttonState == LOW && calibrationState == 3) {

    for (int i = 0; i < amount; i++) {
      sensorMax[i] = sensorValue[i];
      Serial.println(sensorMax[i]);
    }
    calibrationState = 0;
    Serial.print(calibrationState);
    Serial.println(" :calibrationStat");

    Serial.println("Calibrating done");
  } else if (buttonState == LOW && calibrationState == 1 && menuScaledLeft == 0) {
    if (preset == 0) {
      preset = 1;
    } else {
      preset = 0;
    }
    calibrationState = 0;
  } else if (buttonState == LOW && calibrationState == 1 && menuScaledLeft == 2) {
    calibrationState = 0;
  }


  if (calibrationState == 1 && menuScaledLeft == 0) {
    colorLed(255, 255, 0);
  } else if (calibrationState == 1 && menuScaledLeft == 1) {
    colorLed(255, 0, 255);
  } else if (calibrationState == 1 && menuScaledLeft == 2) {
    colorLed(0, 0, 255);
  }else if (calibrationState == 2) {
    colorLed(255, 0, 0);
  } else if (calibrationState == 3) {
    colorLed(255, 127, 0);
  } else if (calibrationState == 0) {
    colorLed(volume * 2, volume * 2, volume * 2);
  }
}

void readSensors() {
  //read StretchSensors and Serial Send data
  for (int i = 0; i < amount; i++) {
    //read sensordata
    sensorValue[i] = analogRead(sensorPin[i]);
    scaledSensorValue[i] = map(analogRead(sensorPin[i]), sensorMin[i], sensorMax[i], 0, 127);
  };
  midi();
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
  // Serial.print(pin);
  // Serial.print(".  ");
  // Serial.println(result);

  return result;
}

void menu() {
}

void readEncoderPos() {
  //read encoderpositions and scale them to 0 - 127
  long newLeft, newRight;
  newLeft = knobOne.read();
  // Serial.print("newLeft: ");
  // Serial.println(newLeft);
  scaledLeft = newLeft / 4;

  if (scaledLeft < 0) {
    menuScaledLeft = (-1 * scaledLeft) % 3;

  } else {
    menuScaledLeft = scaledLeft % 3;

  }
    Serial.print("menuScaledLeft: ");
    Serial.println(menuScaledLeft);
  if (scaledLeft < 0) {
    scaledLeft = 0;
  }
  // Serial.print("ScaledLeft: ");
  // Serial.println(scaledLeft);


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

  // Serial.print("Left = ");
  // Serial.print(scaledLeft);
  // Serial.print(", Right = ");
  // Serial.print(scaledRight);
  // Serial.println();
}

void readButton() {
  //check both buttons for state
  // Serial.print("knops: ");

  knopCheck(BUTTON1_PIN, 1);
  // Serial.print("  ");
  knopCheck(BUTTON2_PIN, 2);
}

void midi() {  //send midi message: (Controller number, Control Value, Midi Channel) (CN 1-8 are sensors, CN 9 is Volume)
  usbMIDI.sendControlChange(1, scaledSensorValue[0], 1);
  usbMIDI.sendControlChange(2, scaledSensorValue[1], 1);
  usbMIDI.sendControlChange(3, scaledSensorValue[2], 1);
  usbMIDI.sendControlChange(4, scaledSensorValue[3], 1);
  usbMIDI.sendControlChange(5, scaledSensorValue[4], 1);
  usbMIDI.sendControlChange(6, scaledSensorValue[5], 1);
  usbMIDI.sendControlChange(7, scaledSensorValue[6], 1);
  usbMIDI.sendControlChange(8, scaledSensorValue[7], 1);
  usbMIDI.sendControlChange(9, volume, 1);
  usbMIDI.sendControlChange(10, preset, 1);
}

void colorLed(int r, int g, int b) {
  analogWrite(9, b);
  analogWrite(10, g);
  analogWrite(11, r);
}
