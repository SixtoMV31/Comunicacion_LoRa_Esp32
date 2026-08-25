#include <RadioLib.h>
#include <Wire.h>
#include "SSD1306Wire.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Preferences.h>
#include "index.h"

//inicializAR lapantalla (direccion I2C,SDA, SCL)
SSD1306Wire display(0x3c, 4, 15);

//Inicializar el modulo de LoRa (NSS, DIOO, RST, DI01)
SX1276 radio = new Module(18, 26, 14, 35);

const char* SSID = "HydroAlert";
const char* PASS = "0123456789";

const char* PARAM_INPUT_HEIGHT = "height";

float height = 0.0;

AsyncWebServer server(80);
Preferences preferences;

String processor(const String& var) {
  if (var == "HEIGHT") {
    return String(preferences.getFloat("height", 0.0), 2);
  }

  return String();
}

void notFound(AsyncWebServerRequest* request) {
  request->send(404, "text/plain", "NOT FOUND");
}

void setup() {
  Serial.begin(115200);
  delay(50);

  Serial.println("Initing data storage");
  preferences.begin("data", false);
  Serial.println("Initing data storage done");

  Serial.println("Reading data from storage");
  height = preferences.getFloat("height", 0.0);
  Serial.println("Reading data from storage done");

  Serial.print("Initing AP: ");
  Serial.println(SSID);
  WiFi.softAP(SSID, PASS);
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  Serial.println("Initing server");
  server.onNotFound(notFound);
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "text/html", index_html, processor);
  });

  server.on("/update", HTTP_GET, [](AsyncWebServerRequest* request) {
    String inputMessage;

    if (request->hasParam(PARAM_INPUT_HEIGHT)) {
      inputMessage = request->getParam(PARAM_INPUT_HEIGHT)->value();
      height = inputMessage.toFloat();
      preferences.putFloat("height", height);
    }

    request->redirect("/");
  });

  server.begin();
  Serial.println("Webserver inting done");

  //Inicializar pantalla OLED
  pinMode(16, OUTPUT);
  digitalWrite(16, LOW);
  delay(50);
  digitalWrite(16, HIGH);
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.clear();
  display.drawString(0, 0, "Iniciando Lora");
  display.display();

  //Inicializamos LoRa a 915 MHz
  int state = radio.begin(915.0);

  if (state == RADIOLIB_ERR_NONE) {
    Serial.println("LoRa recibiendo");
    display.clear();
    display.drawString(0, 0, "LoRa recibiendo");
    display.drawString(0, 15, "Esperando datos...");
    display.display();
  } else {
    Serial.println("Error al iniciar LoRa --->");
    Serial.print(state);
    display.clear();
    display.drawString(0, 0, "Error LoRa " + String(state));
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

void loop() {
  String received = "";
  display.clear();

  int state = radio.receive(received);
  if (state == RADIOLIB_ERR_NONE) {
    // Serial.print("Datos Recibidos: ");
    // Serial.println(received);
    // Serial.print("RSST: ");
    // Serial.print(radio.getRSSI());
    // Serial.println(" dBm");

    // Serial.print("{altura:");
    Serial.println(height - received.toFloat());
    // Serial.println("}");

    display.drawString(0, 0, "HydroAlert");
    display.drawString(0, 15, "Datos: ");
    display.drawString(0, 28, received);
    display.drawString(0, 45, "RSSI: " + String(radio.getRSSI()) + "DbM");
    display.display();
  } else if (state == RADIOLIB_ERR_RX_TIMEOUT) {
    Serial.println("Esperando datos...");
    display.clear();
    display.drawString(0, 0, "HydroAlert");
    display.drawString(0, 15, "Esperando datos...");
    display.display();
  } else {
    Serial.println("Error al recibir datos");
    Serial.print(state);
    display.clear();
    display.drawString(0, 0, "HydroAlert");
    display.drawString(0, 15, "Error al recibir datos");
    display.display();
  }
}
