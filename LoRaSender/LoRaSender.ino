#include <RadioLib.h>
#include <Wire.h>
#include "SSD1306Wire.h"

// Display OLED (dirección, SDA, SCL)
SSD1306Wire display(0x3c, 4, 15);

// Pines del LoRa en WiFi LoRa 32 V2
SX1276 radio = new Module(18, 26, 14, 35);

void setup() {
    Serial.begin(115200);
    delay(2000);

    // Iniciar pantalla
    pinMode(16, OUTPUT);      // Pin reset del OLED
    digitalWrite(16, LOW);    // Reset
    delay(50);
    digitalWrite(16, HIGH);   // Encender

    display.init();
    display.flipScreenVertically();
    display.setFont(ArialMT_Plain_10);
    display.clear();
    display.drawString(0, 0, "Iniciando LoRa...");
    display.display();

    // Iniciar LoRa
    Serial.println("Iniciando LoRa...");
    int state = radio.begin(915.0);
    if (state == RADIOLIB_ERR_NONE) {
        Serial.println("LoRa OK!");
        display.clear();
        display.drawString(0, 0, "LoRa OK!");
        display.display();
    } else {
        Serial.print("Error: ");
        Serial.println(state);
        display.clear();
        display.drawString(0, 0, "Error LoRa: " + String(state));
        display.display();
    }
    delay(2000);
}

int count = 0;

void loop() {
    String msg = "Hello world " + String(count++);

    Serial.print("Enviando: ");
    Serial.println(msg);

    // Mostrar en pantalla
    display.clear();
    display.drawString(0, 0,  "Enviando paquete:");
    display.drawString(0, 15, msg);
    display.display();

    int state = radio.transmit(msg);
    if (state == RADIOLIB_ERR_NONE) {
        Serial.println("Enviado OK!");
        display.drawString(0, 30, "Estado: OK");
        display.drawString(0, 45, "Paquete #" + String(count));
    } else {
        Serial.print("Error: ");
        Serial.println(state);
        display.drawString(0, 30, "Error: " + String(state));
    }
    display.display();
    delay(2000);
}