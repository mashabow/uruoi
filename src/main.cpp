#include <Arduino.h>

const int PUMP_PIN = 4; // IO4

void setup()
{
  pinMode(PUMP_PIN, OUTPUT);
  Serial.begin(74880);
}

void loop()
{
  const int value = analogRead(A0);
  Serial.println(value);
  delay(1000);

  // digitalWrite(PUMP_PIN, HIGH);
  // delay(5000);
  // digitalWrite(PUMP_PIN, LOW);
  // delay(5000);
}
