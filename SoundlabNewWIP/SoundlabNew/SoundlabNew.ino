const int amount = 8;
float smoothingFactor = 0.09;

const int sensorPin[amount] = { 16, 17, 18, 19, 20, 21, 22, 23 };
int sensorValue[amount] = { 0, 0, 0, 0, 0, 0, 0, 0 };

int sensorMax[amount] = { 0, 0, 0, 0, 0, 0, 0, 0 };
int sensorMin[amount] = { 0, 0, 0, 0, 0, 0, 0, 0 };

int menuHeader = 0;
int menuSettings[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

int state = 0;

//buttonstarts here
const int BUTTON1_PIN = 2;  // the number of the pushbutton pin
const int BUTTON2_PIN = 3;  // the number of the pushbutton pin

// Variables will change:
int lastState = LOW;   // the previous state from the input pin
int lastState2 = LOW;  // the previous state from the input pin


int buttonState;
int buttonState2;

int lastButtonState = LOW;   // the previous reading from the input pin
int lastButtonState2 = LOW;  // the previous reading from the input pin


unsigned long lastDebounceTime = 0;   // the last time the output pin was toggled
unsigned long lastDebounceTime2 = 0;  // the last time the output pin was toggled

bool clicked = false;
bool clicked2 = false;

unsigned long debounceDelay = 50;

String onOff;

int readingButton;


void setup() {
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);

  Serial.begin(115200);

  //calibrate()
}

void loop() {
  for (int i = 0; i < amount; i++) {
    //read sensordata
    sensorValue[i] = analogRead(sensorPin[i]);
  };

  //  print sensor data
  Serial.print("sensors ");

  for (int i = 0; i < amount; i++) {

    Serial.print(sensorValue[i]);
     Serial.print(" ");
  };
  Serial.println();
  Serial.print("knops: ");

  Serial.print(knopCheck(BUTTON1_PIN, 1));
  Serial.print("  ");
  Serial.println(knopCheck(BUTTON2_PIN, 2));

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

  delay(20);
}

void calibrate() {
  /*
  led geel
   waitforselect(knopcheck(1)){
      forloop{
        vul minindex
      }
    }

    waitforselect(knopcheck(1)){
      forloop{
        vul plusindex
      }
    }
  if(knopCheck(1)){
    exit de boel
  }

  */
}

bool knopCheck(int pin, int pinChoice) {
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
  //if knopaan return true else return false
}
