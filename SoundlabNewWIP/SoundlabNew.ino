const int amount = 8;
float smoothingFactor = 0.09;

const int sensorPin[amount] = {16, 17, 18, 19, 20, 21, 22, 23};
int sensorValue[amount] = {0, 0, 0, 0, 0, 0, 0, 0};

int sensorMax[amount] = {0, 0, 0, 0, 0, 0, 0, 0};
int sensorMin[amount] = {0, 0, 0, 0, 0, 0, 0, 0}; 

int menuHeader = 0;
int menuSettings[8] = {0, 0, 0, 0, 0, 0, 0, 0};

int state = 0;

//buttonstarts here
const int BUTTON1_PIN = 2;       // the number of the pushbutton pin

// Variables will change:
int lastState = LOW;  // the previous state from the input pin

int buttonState;   
int lastButtonState = LOW;  // the previous reading from the input pin

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50; 

bool clicked = false;
String onOff;

int reading;

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON1_PIN, INPUT_PULLUP);

  //calibrate()


}

void loop() {

  for (int i = 0; i < amount; i++) {
    //read sensordata
    sensorValue[i] = analogRead(sensorPin[i]);
  };

  //  print sensor data
  for (int i = 0; i < amount; i++) {
    Serial.print(" ");
    Serial.print(sensorValue[i]);
  };
  Serial.println();

  Serial.println(knopCheck(BUTTON1_PIN));
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

bool knopCheck(int pin) {
  bool result = false;
  reading = digitalRead(pin);
  
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW && !clicked){
        result = true;
        clicked = true;
      } 
      if(buttonState == HIGH && clicked){
        result = false;
        clicked = false;
      }
    }
  }
  lastButtonState = reading;

  return result;
  //if knopaan return true else return false
}