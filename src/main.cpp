#include <Arduino.h>

const int PUMP_PIN = 4; // IO4

void setup()
{
  pinMode(PUMP_PIN, OUTPUT);
}

void loop()
{
  digitalWrite(PUMP_PIN, HIGH);
  delay(5000);
  digitalWrite(PUMP_PIN, LOW);
  delay(5000);
}
