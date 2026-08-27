#include <RadioLib.h>
#include <Wire.h>
#include "SSD1306Wire.h"

const int echoPin = 12;
const int trigPin = 13;

//define sound speed in cm/uS
#define SOUND_SPEED 0.034

long duration;
float distanceCm;

// Display OLED (dirección, SDA, SCL)
SSD1306Wire display(0x3c, 4, 15);

//Inicializar el modulo de LoRa (NSS, DIOO, RST, DI01)
SX1276 radio = new Module(18, 26, 14, 35);

float measureDistance() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculate the distance
  distanceCm = duration * SOUND_SPEED / 2;

  // Prints the distance in the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);

  return distanceCm;
}

void setup() {
  Serial.begin(115200);
  delay(2000);

  pinMode(trigPin, OUTPUT);  // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);   // Sets the echoPin as an Input

  // Iniciar pantalla
  pinMode(16, OUTPUT);    // Pin reset del OLED
  digitalWrite(16, LOW);  // Reset
  delay(50);
  digitalWrite(16, HIGH);  // Encender

  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.clear();
  display.drawString(0, 0, "Iniciando LoRa...");
  display.display();

  // Iniciar LoRa
  Serial.println("Iniciando LoRa...");
  int state = radio.begin();
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

  if (radio.setFrequency(915.0) == RADIOLIB_ERR_INVALID_FREQUENCY) {
    Serial.println(F("Selected frequency is invalid for this module!"));
    while (true) { delay(10); }
  }

  // set bandwidth to 250 kHz
  if (radio.setBandwidth(250.0) == RADIOLIB_ERR_INVALID_BANDWIDTH) {
    Serial.println(F("Selected bandwidth is invalid for this module!"));
    while (true) { delay(10); }
  }

  // set spreading factor to 10
  if (radio.setSpreadingFactor(11) == RADIOLIB_ERR_INVALID_SPREADING_FACTOR) {
    Serial.println(F("Selected spreading factor is invalid for this module!"));
    while (true) { delay(10); }
  }

  // set coding rate to 6
  if (radio.setCodingRate(6) == RADIOLIB_ERR_INVALID_CODING_RATE) {
    Serial.println(F("Selected coding rate is invalid for this module!"));
    while (true) { delay(10); }
  }

  if (radio.setSyncWord(0x14) != RADIOLIB_ERR_NONE) {
    Serial.println(F("Unable to set sync word!"));
    while (true) { delay(10); }
  }

  // set output power to 10 dBm (accepted range is -3 - 17 dBm)
  // NOTE: 20 dBm value allows high power operation, but transmission
  //       duty cycle MUST NOT exceed 1%
  if (radio.setOutputPower(10) == RADIOLIB_ERR_INVALID_OUTPUT_POWER) {
    Serial.println(F("Selected output power is invalid for this module!"));
    while (true) { delay(10); }
  }

  // set over current protection limit to 80 mA (accepted range is 45 - 240 mA)
  // NOTE: set value to 0 to disable overcurrent protection
  if (radio.setCurrentLimit(120) == RADIOLIB_ERR_INVALID_CURRENT_LIMIT) {
    Serial.println(F("Selected current limit is invalid for this module!"));
    while (true) { delay(10); }
  }

  // set LoRa preamble length to 15 symbols (accepted range is 6 - 65535)
  if (radio.setPreambleLength(15) == RADIOLIB_ERR_INVALID_PREAMBLE_LENGTH) {
    Serial.println(F("Selected preamble length is invalid for this module!"));
    while (true) { delay(10); }
  }

  // set amplifier gain to 1 (accepted range is 1 - 6, where 1 is maximum gain)
  // NOTE: set value to 0 to enable automatic gain control
  //       leave at 0 unless you know what you're doing
  if (radio.setGain(0) == RADIOLIB_ERR_INVALID_GAIN) {
    Serial.println(F("Selected gain is invalid for this module!"));
    while (true) { delay(10); }
  }
  delay(1000);
}

int count = 0;

void loop() {
  float distance = measureDistance();

  String msg = String(distance, 2);

  Serial.print("Enviando: ");
  Serial.println(msg);


  // Mostrar en pantalla
  display.clear();
  display.drawString(0, 0, "HydroAlert");
  display.drawString(0, 15, msg);
  // display.display();}

  count++;

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