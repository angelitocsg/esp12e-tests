#include <Arduino.h>
#include <ESP8266WiFi.h>

const long blink_interval = 500;
unsigned long blink_previousMillis = 0;
bool blink_status = false;

IPAddress staticIP(192, 168, 0, 65);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns1(1, 1, 1, 1);
IPAddress dns2(8, 8, 8, 8);

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    WiFi.begin("seu SSID", "sua senha");
    WiFi.config(staticIP, gateway, subnet, dns1, dns2);

    WiFi.softAP("ESP12E", "0123456789");
}

void loop()
{
    unsigned long currentMillis = millis();

    if (currentMillis - blink_previousMillis >= blink_interval)
    {
        blink_previousMillis = currentMillis;
        blink_status = !blink_status;
        digitalWrite(LED_BUILTIN, blink_status);
    }
}