#include <RadioLib.h>
#include <Wire.h>
#include "SSD1306Wire.h"

//inicializAR lapantalla (direccion I2C,SDA, SCL)
SSD1306Wire display(0x3c, 4, 15);
//InicializaR el modulo de LoRa (NSS, DIOO,RST,DI01)
SX1276 radio = new Module(18, 26, 14, 35);



void setup() {
  Serial.begin(115200);
  delay(2000);

  //Inicializar pantalla OLED
  pinMode(16, OUTPUT);
  digitalWrite(16,LOW);
  delay(50);
  digitalWrite(16,HIGH);
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.clear();
  display.drawString(0, 0, "Iniciando Lora");
  display.display();

  //Inicializamos LoRa a 915 MHz
  int state=radio.begin(915.0);
  if(state==RADIOLIB_ERR_NONE){
    Serial.println("Lora Recibiendo");
    display.clear();
    display.drawString(0,0, "Lora Recibiendo");
    display.drawString(0,15, "Esperando Datos...");
    display.display();
  }
  else{
    Serial.println("Error al iniciar Lora --->");
    Serial.print(state);
    display.clear();
    display.drawString(0,0, "Error Lora " + String (state));
    display.display();
  }
  delay(1000);
}

void loop() {
String received= "";
// display.clear();
// display.display();
int state= radio.receive(received);
if (state==RADIOLIB_ERR_NONE){
  Serial.println("Datos Recibidos: ");
  Serial.print(received);
  Serial.println("RSST: ");
  Serial.print(radio.getRSSI());
  Serial.println(" dBm");

 display.clear();
  display.drawString(0,0, "<< LORA RECEPTOR >>");
  display.drawString(0,15, "Datos: " );
  display.drawString(0,28, received);
  display.drawString(0,45, "RSSI: " + String(radio.getRSSI())+ "DbM");
  display.display();
}

  else if(state==RADIOLIB_ERR_RX_TIMEOUT)
  {
    Serial.println("Esperando Dtos...");
    display.clear();
    display.drawString(0,0, "Esperando Dtos..");
    display.display();
  }
  else{
    Serial.println("Error al recibir datos");
    Serial.print(state);
    display.clear();
    display.drawString(0,0, "Error al recibir datos");
  }
}


