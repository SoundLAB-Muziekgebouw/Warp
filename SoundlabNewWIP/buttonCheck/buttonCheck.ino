/*
// constants won't change. They're used here to set pin numbers:
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
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // initialize the pushbutton pin as an pull-up input
  // the pull-up input pin will be HIGH when the switch is open and LOW when the switch is closed.
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
   reading = digitalRead(BUTTON_PIN);
  
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW && !clicked){
        Serial.println("biemwe");
        clicked = true;
      } 
      if(buttonState == HIGH && clicked){
        clicked = false;
      }
    }
  }

  lastButtonState = reading;

  Serial.println(lastButtonState);
  delay(25);
  // Serial.println(" ");
}

*/