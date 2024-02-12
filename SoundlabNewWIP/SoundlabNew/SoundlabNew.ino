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

//copy paste hihi
const int pinA = 12;           // Our first hardware interrupt pin is digital pin 12
const int pinB = 11;           // Our second hardware interrupt pin is digital pin 13
volatile byte aFlag = 0;       // let's us know when we're expecting a rising edge on pinA to signal that the encoder has arrived at a detent
volatile byte bFlag = 0;       // let's us know when we're expecting a rising edge on pinB to signal that the encoder has arrived at a detent (opposite direction to when aFlag is set)
volatile byte encoderPos = 0;  //this variable stores our current value of encoder position. Change to int or uin16_t instead of byte if you want to record a larger range than 0-255
volatile byte oldEncPos = 0;   //stores the last encoder position value so we can compare to the current reading and see if it has changed (so we know when to print to the serial monitor)
volatile byte reading = 0;     //somewhere to store the direct values we read from our interrupt pins before checking to see if we have moved a whole detent
//popy caste hihi

void setup() {
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  //copy paste hihi
  pinMode(pinA, INPUT_PULLUP);       // set pinA as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  pinMode(pinB, INPUT_PULLUP);       // set pinB as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  attachInterrupt(0, PinA, RISING);  // set an interrupt on PinA, looking for a rising edge signal and executing the "PinA" Interrupt Service Routine (below)
  attachInterrupt(1, PinB, RISING);
  //popy caste hihi
  Serial.begin(115200);

  //calibrate()
}

void PinA(){
  cli(); //stop interrupts happening before we read pin values
  reading = PIND & 0xC; // read all eight pin values then strip away all but pinA and pinB's values
  if(reading == B00001100 && aFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    encoderPos --; //decrement the encoder's position count
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
  }
  else if (reading == B00000100) bFlag = 1; //signal that we're expecting pinB to signal the transition to detent from free rotation
  sei(); //restart interrupts
}

void PinB(){
  cli(); //stop interrupts happening before we read pin values
  reading = PIND & 0xC; //read all eight pin values then strip away all but pinA and pinB's values
  if (reading == B00001100 && bFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    encoderPos ++; //increment the encoder's position count
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
  }
  else if (reading == B00001000) aFlag = 1; //signal that we're expecting pinA to signal the transition to detent from free rotation
  sei(); //restart interrupts
}


void loop() {

    Serial.print("encody ");
    Serial.println(encoderPos);
 if(oldEncPos != encoderPos) {

    oldEncPos = encoderPos;
  }


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
