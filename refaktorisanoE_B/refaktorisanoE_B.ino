#include <Adafruit_BLE_UART.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h>
#include <bme680.h>
#include <bme680_defs.h>

Adafruit_BME680 senzor = Adafruit_BME680();
Adafruit_BLE_UART blutut = Adafruit_BLE_UART(46, 2, 48);
aci_evt_opcode_t stanje = ACI_EVT_INVALID;
String slacu = "";

void setup() {
  senzor.begin();
  blutut.setDeviceName("ARD2");
  blutut.begin();
}

void loop() {
  Serial.println("e");
  blutut.pollACI();
  stanje = blutut.getState();
  String t = String(senzor.readTemperature()), 
         p = String(senzor.readPressure()/1000),
         h = String(senzor.readHumidity());
  
  slacu = "{" + t + "," + p + "," + h + "}";
  uint8_t sendBuffer[20];
  slacu.getBytes(sendBuffer, 20);
  if(stanje == ACI_EVT_CONNECTED)
    blutut.write(sendBuffer, 20);
  delay(5000);
}
