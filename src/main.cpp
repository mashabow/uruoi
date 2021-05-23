#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "Ambient.h"
#include "secrets.h"

const int PUMP_PIN = 4; // IO4

WiFiClient client;
Ambient ambient;

void setup()
{
  Serial.begin(74880);
  pinMode(PUMP_PIN, OUTPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }
  Serial.println("Wi-Fi connected.");

  ambient.begin(AMBIENT_CHANNEL_ID, AMBIENT_WRITE_KEY, &client);
}

void loop()
{
  const int vAir = 394;   // 空気中での測定値
  const int vWater = 228; // 水中での測定値
  const float moisture = 100.0 * (vAir - analogRead(A0)) / (vAir - vWater);
  Serial.println(moisture);

  ambient.set(1, moisture);
  ambient.send();

  delay(30 * 1000);

  // digitalWrite(PUMP_PIN, HIGH);
  // delay(5000);
  // digitalWrite(PUMP_PIN, LOW);
  // delay(5000);
}
