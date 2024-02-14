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

int sensorMax[amount] = { 0, 0, 0, 0, 0, 0, 0, 0 };
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

bool clicked = false;
bool clicked2 = false;

unsigned long debounceDelay = 50;

String onOff;

int readingButton;
int calibrationState = 1;



void setup() {
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);

  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  Serial.begin(115200);

}

void loop() {
  //Read Data
  readSensors();
  readEncoderPos();
  readButton();
  calibrate();


  delay(20);
}

//


int scaler(int val, int min, int max){
  int scaled_value = map(val, 0, 127, min, max);
  return scaled_value;
}



void calibrate() {

  // Serial.print(calibrationState);
  // Serial.println(" Callibri");

  if (buttonState == LOW && buttonState2 == LOW) {
    calibrationState = 1;

  } else if (buttonState == LOW && calibrationState == 1) {
    calibrationState = 2;
    Serial.print("min");
    for(int i = 0; i < amount; i++){
      sensorMin[i] = sensorValue[i];
      Serial.println(sensorMin[i]);
    }


  } else if (buttonState2 == LOW && calibrationState == 2) {
        Serial.print("max");

    for(int i = 0; i < amount; i++){
      sensorMax[i] = sensorValue[i];
      Serial.println(sensorMax[i]);
    }    calibrationState = 0;
  }


  if (calibrationState == 1) {
    analogWrite(9, 0);
    analogWrite(10, 0);
    analogWrite(11, 255);
  } else if (calibrationState == 2) {
    analogWrite(9, 255);
    analogWrite(10, 0);
    analogWrite(11, 255);
  } else if (calibrationState == 0) {
    analogWrite(9, 255);
    analogWrite(10, 255);
    analogWrite(11, 255);
  }
}

void readSensors() {
  //read StretchSensors and Serial Send data
  for (int i = 0; i < amount; i++) {
    //read sensordata
    sensorValue[i] = analogRead(sensorPin[i]);
    scaledSensorValue[i] = map(analogRead(sensorPin[i]),0, 127, sensorMin[i], sensorMax[i]);
  };
  midi();
  // Serial.print("sensors ");

  // for (int i = 0; i < amount; i++) {

  //   Serial.print(sensorValue[i]);
  //   Serial.print(" ");
  // };
  // Serial.println();


  // usbMIDI.sendControlChange(10, volume_param, 1);
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
  /*if(buttoncheck){
    led op rood
    header = 0
    waitforselect(knopcheck(1)){
      header = readknop1;
    }

    waitforselect(knopcheck(2)){
      menuSettings[header] = knop2read;
    }

    if (knopCheck(2)) 
      header op 0
      buttoncheck = 0
      led uit
  }
  if(knopCheck(1)){
    buttoncheck = true
  }
     */
}

void readEncoderPos() {
  //read encodepositions and scale them to 0 - 127
  long newLeft, newRight;
  long scaledLeft, scaledRight;
  newLeft = knobOne.read();
  scaledLeft = newLeft / 4;

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

void midi() {
  usbMIDI.sendControlChange(1, scaledSensorValue[0], 1);
  usbMIDI.sendControlChange(2, scaledSensorValue[1], 1);
  usbMIDI.sendControlChange(3, scaledSensorValue[2], 1);
  usbMIDI.sendControlChange(4, scaledSensorValue[3], 1);
  usbMIDI.sendControlChange(5, scaledSensorValue[4], 1);
  usbMIDI.sendControlChange(6, scaledSensorValue[5], 1);
  usbMIDI.sendControlChange(7, scaledSensorValue[6], 1);
  usbMIDI.sendControlChange(8, scaledSensorValue[7], 1);
}
