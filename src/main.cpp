#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Ambient.h>
#include <MCP3XXX.h>
#include "secrets.h"

ADC_MODE(ADC_VCC);

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

const float getMoisture()
{
  const int POWER_PIN = 4; // IO4: 土壌水分量センサーと MCP3002 の電源に使うピン

  pinMode(POWER_PIN, OUTPUT);
  digitalWrite(POWER_PIN, HIGH);
  delay(1000); // センサーの状態が安定するまで待つ

  MCP3002 adc;
  adc.begin();

  const int vAir = 676;   // 空気中での測定値
  const int vWater = 308; // 水中での測定値
  const auto moisture = 100.0 * (vAir - adc.analogRead(0)) / (vAir - vWater);

  digitalWrite(POWER_PIN, LOW);

  return moisture;
}

void water()
{
  const int WATERING_PIN = 5;
  pinMode(WATERING_PIN, OUTPUT);

  // ポンプを 2 分間 ON にする
  digitalWrite(WATERING_PIN, HIGH);
  delay(2 * 60 * 1000);
  digitalWrite(WATERING_PIN, LOW);
}

void setup()
{
  Serial.begin(74880);
  setupWiFi();

  const auto moisture = getMoisture();
  Serial.printf("Moisture: %5.2f%%\n", moisture);
  const auto vcc = ESP.getVcc() / 1000.0;
  Serial.printf("Vcc: %5.3fV\n", vcc);

  Ambient ambient;
  WiFiClient client;
  ambient.begin(AMBIENT_CHANNEL_ID, AMBIENT_WRITE_KEY, &client);
  ambient.set(1, moisture);
  ambient.set(2, vcc);
  ambient.send();

  const int wateringThreshold = 65; // %
  if (moisture < wateringThreshold)
    water();

  deepSleep();
}

void loop() {}
