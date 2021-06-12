#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "Ambient.h"
#include "secrets.h"

const int PUMP_PIN = 4; // IO4

WiFiClient client;
Ambient ambient;

/* deep sleep して終了。wake 時には setup から始まる */
void deepSleep()
{
  const int intervalMinutes = 20;
  ESP.deepSleep(intervalMinutes * 60 * 1000 * 1000, WAKE_RF_DEFAULT);
  delay(1000); // deep sleep が始まるまで待つ
}

void setupWiFi()
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("Connecting WiFi...");
  int retryCount = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    if (retryCount++ > 40)
      deepSleep();
    delay(500);
  }
  Serial.println("Connected.");
}

float getMoisture()
{
  const int vAir = 397;   // 空気中での測定値
  const int vWater = 225; // 水中での測定値
  return 100.0 * (vAir - analogRead(A0)) / (vAir - vWater);
}

void setup()
{
  Serial.begin(74880);
  pinMode(PUMP_PIN, OUTPUT);
  setupWiFi();

  const float moisture = getMoisture();
  Serial.println(moisture);

  ambient.begin(AMBIENT_CHANNEL_ID, AMBIENT_WRITE_KEY, &client);
  ambient.set(1, moisture);
  ambient.send();

  // digitalWrite(PUMP_PIN, HIGH);
  // delay(5000);
  // digitalWrite(PUMP_PIN, LOW);
  // delay(5000);

  deepSleep();
}

void loop() {}
