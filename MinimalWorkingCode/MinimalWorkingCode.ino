const int amount = 8;

const int sensorPin[amount] = {16, 17, 18, 19, 20, 21, 22, 23};
int sensorValue[amount] = {0, 0, 0, 0, 0, 0, 0, 0};

void setup() {
  Serial.begin(9600);
}

void loop() {
  //  print sensor data
  for (int i = 0; i < amount; i++) {
    sensorValue[i] = analogRead(sensorPin[i]);
    Serial.print(" ");
    Serial.print(sensorValue[i]);
  };
  Serial.println();

  delay(20);
}
