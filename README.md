<h1 align="center">Comunicación LoRa con ESP32</h1>

<p align="center">
  Implementación de comunicación inalámbrica de largo alcance usando Heltec WiFi LoRa 32 V2
</p>

<p align="center">
  <img src="https://img.shields.io/badge/ESP32-LoRa-blue?style=for-the-badge">
  <img src="https://img.shields.io/badge/RadioLib-SX1276-green?style=for-the-badge">
  <img src="https://img.shields.io/badge/OLED-SSD1306-black?style=for-the-badge">
</p>

---
## Requisitos del Hadware
La placa utilizada es la Heltec WiFi LoRa 32 (V2), que integra en un solo módulo el microcontrolador ESP32, el transceptor LoRa SX1276 y una pantalla OLED de 128x64 píxeles.

> IMPORTANTE: 
Conecta siempre la antena antes de alimentar la placa.
Operar el transmisor LoRa sin antena puede dañar permanentemente el chip SX1276.

---

## 📖 Descripción

Este proyecto implementa un sistema de comunicación inalámbrica utilizando tecnología LoRa con la placa Heltec WiFi LoRa 32 V2 basada en ESP32. Permite la transmisión y recepción de datos entre dispositivos mostrando la información en una pantalla OLED y en el monitor serie.

---

## 🎯 Objetivo

Desarrollar e implementar un sistema funcional de comunicación LoRa entre dos dispositivos, demostrando su configuración, instalación y operación para transmisión de datos a larga distancia.

---
## ⚙️ Requisitos de Librerias e instalacion
<h3 align="center">Agregar el Repositorio de Heltec</h3>

Para que Arduino IDE reconozca la placa Heltec, sigue estos pasos:
1.	Abre Arduino IDE y ve a: Archivo > Preferencias
2.	En el campo "Gestor de URLs adicionales de tarjetas" pega la siguiente URL:
https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series/releases/download/0.0.7/package_heltec_esp32_index.json
3.  Haz clic en OK para guardar
4.	Ve a: Herramientas > Placa > Gestor de placas
5.	Busca "Heltec" e instala: Heltec ESP32 Series Dev-boards (versión 0.0.7)

<h3 align="center">Seleccionar la Placa Correcta</h3>
Una vez instalado el paquete, selecciona la placa en:

1. Herramientas > Placa > Heltec ESP32 Series Dev-boards > WiFi LoRa 32(V2)
2. Herramientas > Puerto > (selecciona el COM que aparece al conectar la placa)


<h3 align="center">Librerías Necesarias</h3>

Para este proyecto se utilizan dos librerías principales que debes instalar desde el Administrador de Bibliotecas de Arduino IDE (Herramientas > Administrar bibliotecas):

1. RadioLib
Librería para comunicación LoRa sin restricciones de licencia. Compatible con el chip SX1276 de la placa Heltec V2.

        Nombre en IDE : RadioLib

        Autor : Jan Gromes
2. ESP8266 and ESP32 OLED driver for SSD1306 Librería para controlar la pantalla OLED integrada en la placa.

        Nombre en IDE: ESP8266 and ESP32 OLED driver for SSD1306 displays
        Autor: ThingPulse, Fabrice Weinberg
        << Busca "SSD1306" > selecciona la de ThingPulse > Instalar>>
>[ ⚠️ IMPORTANTE]
No uses la librería LoRaWan_APP.h de Heltec en versión 0.0.7.
Requiere un sistema de licencia por Chip ID que bloquea el funcionamiento
en placas no registradas en la base de datos oficial de Heltec.
RadioLib no tiene esta restricción y funciona perfectamente.


<h3 align="center">Pines Importantes del Heltec WiFi LoRa 32 V2</h3>

Los siguientes pines son los que usa RadioLib para comunicarse con el chip SX1276. Estos valores están fijos en el hardware y no deben cambiarse:

        Función 	    Pin ESP32    Descripción

        NSS (CS)	    18	        Chip Select del SX1276
        DIO0	        26	        Interrupción de TX/RX completado
        RST	            14	        Reset del módulo LoRa
        BUSY / DIO1	    35	        Pin de estado ocupado
        SDA (OLED)	    4	        Datos I2C del display
        SCL (OLED)	    15	        Reloj I2C del display
        RST (OLED)	    16	        Reset del display OLED

---
👤 Autor

Proyecto realizado con fines educativos para monitorial el nivel del agua en tiempo real usando LoRa

---