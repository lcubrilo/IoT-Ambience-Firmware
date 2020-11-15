/****************************************
 * Title:
 * Firmware for the Arduino MEGA board.
 * 
 * Part 1: Input
 * Sensors measure air characteristics.
 * Using mikroE's "Environment click" with Bosch's BME680 environmental sensor
 * 
 * Part 2: Output
 * Sending the values over BLE 
 * Using mikroE's "BLE P click" with Nordic's nRF8001 chip 
 * 
 * Credits:
 * Made possible by Execom and their amazing IoT team, through an internship.
 * They provided the initial project idea, hardware, guidance and 
 * software support and bugfixes whenever necessary.
 * 
 * Coded by Luka Čubrilo.
 ****************************************/
 
#include <BLEPeripheral.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h>
#include <bme680.h>
#include <bme680_defs.h>

//In this example we have 3 characteristics:
//Temperature, pressure and humidity
#define NO_OF_CHARS 3 

//BLE setting up that couldn't have been tucked in the function
BLEPeripheral bluetoothLE = BLEPeripheral(46, 2, 48);
BLEService ts = BLEService("CCC0");
BLEIntCharacteristic tc = BLEIntCharacteristic("CCC1", BLERead | BLENotify);
BLEDescriptor td = BLEDescriptor("2901", "Temperature; Degrees Celsius");

BLEService hs = BLEService("DDD0");
BLEIntCharacteristic hc = BLEIntCharacteristic("DDD1", BLERead | BLENotify);
BLEDescriptor hd = BLEDescriptor("2901", "Humidity; Percent");

BLEService ps = BLEService("EEE0");
BLEIntCharacteristic pc = BLEIntCharacteristic("EEE1", BLERead | BLENotify);
BLEDescriptor pd = BLEDescriptor("2901", "Pressure; mbar/hPa");

Adafruit_BME680 atmosphereSensor = Adafruit_BME680();
int previousMillis = 0, characteristicsIterator = 1;

//Setting up tucked in the function
void bluetoothLESetup() { 
  bluetoothLE.setLocalName("ARDuino3");

  bluetoothLE.setAdvertisedServiceUuid(ts.uuid());
  bluetoothLE.addAttribute(ts);
  bluetoothLE.addAttribute(tc);
  bluetoothLE.addAttribute(td);

  bluetoothLE.setAdvertisedServiceUuid(hs.uuid());
  bluetoothLE.addAttribute(hs);
  bluetoothLE.addAttribute(hc);
  bluetoothLE.addAttribute(hd);

  bluetoothLE.setAdvertisedServiceUuid(ps.uuid());
  bluetoothLE.addAttribute(ps);
  bluetoothLE.addAttribute(pc);
  bluetoothLE.addAttribute(pd);

  bluetoothLE.setEventHandler(BLEConnected, connectToBLE);
  bluetoothLE.setEventHandler(BLEDisconnected, disconnectFromBLE);

  bluetoothLE.begin();
  Serial.println("Finished setting up the BLE.");
}

//BLE connectivity checks and callbacks
bool BLEconnected = false;
void connectToBLE() {
  BLEconnected = true;
  Serial.println("\nPovezan.");
}
void disconnectFromBLE() {
  BLEconnected = false;
  Serial.println("Odvezan.");
}

void setup() {
  Serial.begin(9600);
  Serial.println("Started the firmware.");
  bluetoothLESetup();
  atmosphereSensor.begin();
}

//Most of the heavy  work that transfers data from sensors to a nearby connected device
bool sensor2cloud()
{
  int currentMillis = millis();
  //Burst of all options within 5 seconds, with 0.5 second pauses between options
  if((currentMillis - previousMillis) < (5000 + (characteristicsIterator * 500)))
    return false;
 
  //If enough time has passed, get to work:
  int Value;
  
  //getting from the proper sensor
  switch(characteristicsIterator){ 
    case 1: Value = int(atmosphereSensor.readTemperature()); Serial.print("Read temp: "); break;
    case 2: Value = int(atmosphereSensor.readPressure()/100); Serial.print("Read pres: "); break;
    case 3: Value = int(atmosphereSensor.readHumidity()); Serial.print("Read humi: "); break;
  }
  //Printing the decimal value for humans, and the hex (since it is stored in hex)
  Serial.print(Value);
  Serial.print(" hex: ");
  Serial.println(Value, HEX);
  
  //sending to the proper characteristic
  switch(characteristicsIterator){
    case 1: tc.setValue(Value); break;
    case 2: pc.setValue(Value); break;
    case 3: hc.setValue(Value); break;
  }  
  
  //If we've went through all of them, it's time to start over
  if(characteristicsIterator == NO_OF_CHARS){
    previousMillis = currentMillis;
    characteristicsIterator = 0;
    Serial.println("---------");
  }
  
  characteristicsIterator++;
  return true;
}

bool ok;
void loop() {
  bluetoothLE.poll();
  if(BLEconnected){
    do
      ok = sensor2cloud();
    while(ok);
  }
}
