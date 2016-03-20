#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;

unsigned long microtime;
String payload;

void setup() {
    USE_SERIAL.begin(115200);
    // USE_SERIAL.setDebugOutput(true);

    WiFiMulti.addAP("AimtecHackathon24", "AimtecHackathon2016");

    // wait until wifi connects
    USE_SERIAL.print("[SETUP] wait");
    while (true) {
        if((WiFiMulti.run() == WL_CONNECTED)) {
            break;
        } else {
            delay(1000);
            USE_SERIAL.print(".");
        }
    }
    USE_SERIAL.println("done");
}

void loop() {
    if((WiFiMulti.run() == WL_CONNECTED)) {
        microtime = micros();

        HTTPClient http;

        USE_SERIAL.print("[HTTPS] begin...\n");
        // configure traged server and url
        http.begin("https://boiling-torch-5046.firebaseio.com/clients/node_mcu.json", "7A 54 06 9B DC 7A 25 B3 86 8D 66 53 48 2C 0B 96 42 C7 B3 0A"); //HTTPS

        payload = "{\"microtime\":"+String(microtime)+"}";

        USE_SERIAL.println("[HTTPS] payload: " + payload);

        int httpCode = http.sendRequest("PUT", payload);

        // httpCode will be negative on error
        if(httpCode != HTTP_CODE_OK) {
            USE_SERIAL.printf("[HTTPS] failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
    }
}
