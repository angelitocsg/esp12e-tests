#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>

#include "extra.h"

char device_name[] = "BROADCAST";
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};  // CLEAN

int counter = 0;
int received_count = 0;
int received_bytes = 0;

const long interval = 250;
unsigned long previousMillis = 0;

void updateDisplay() {
    Serial.println("====================================");
    Serial.println("MAC: " + WiFi.macAddress());
    Serial.println("Sended: " + String(counter));
    Serial.println("Received: " + String(received_count));
    Serial.println("====================================");
}

void onDataSent(uint8_t *mac_addr, uint8_t status) {
    if (status == ESP_NOW_SEND_SUCCESS) {
        Serial.println("Delivery success");
        counter++;
    } else {
        Serial.println("Delivery fail");
    }

    if (counter == 50000) {
        counter = 0;
    }
}

void onDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
    memcpy(&received_count, incomingData, len);

    // Serial.println("====================================");
    // Serial.print("Received: ");
    // Serial.print(received_count);
    // Serial.println("");
    // Serial.println("------------------------------------");

    received_bytes = len;
}

void init_esp_now() {
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
    esp_now_register_send_cb(onDataSent);

    esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
    esp_now_register_recv_cb(onDataRecv);
}

void setup() {
    Serial.begin(115200);
    Serial.println();
    Serial.print("ESP8266 Board MAC Address:  ");
    Serial.println(WiFi.macAddress());

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    init_esp_now();

    updateDisplay();

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        esp_now_send(broadcastAddress, (uint8_t *)&counter, sizeof(counter));

        // digitalWrite(LED_BUILTIN, HIGH);
        // delay(500);
        // digitalWrite(LED_BUILTIN, LOW);
        // delay(500);

        updateDisplay();
    }
}