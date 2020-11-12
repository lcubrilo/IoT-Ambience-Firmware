#include <BLEPeripheral.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h>
#include <bme680.h>
#include <bme680_defs.h>

BLEPeripheral blutut = BLEPeripheral(46, 2, 48);
BLEIntCharacteristic tc = BLEIntCharacteristic("CCC1", BLERead | BLENotify);
BLEIntCharacteristic pc = BLEIntCharacteristic("EEE1", BLERead | BLENotify);
BLEIntCharacteristic hc = BLEIntCharacteristic("DDD1", BLERead | BLENotify);

Adafruit_BME680 senzor = Adafruit_BME680();

void blututConnectHandler(BLECentral& central) {
  Serial.print(F("Connected event, central: "));
  Serial.println(central.address());
}
void blututDisconnectHandler(BLECentral& central) {
  Serial.print(F("Disconnected event, central: "));
  Serial.println(central.address());
}
void blututSetup(){
  BLEService ts = BLEService("CCC0");
  BLEDescriptor td = BLEDescriptor("2901", "Temp Celsius");
  
  BLEService hs = BLEService("DDD0");
  BLEDescriptor hd = BLEDescriptor("2901", "Humidity Percent");
  
  BLEService ps = BLEService("EEE0");
  BLEDescriptor pd = BLEDescriptor("2901", "Pressure mbar");
  blutut.setLocalName("xdD2");

  blutut.setAdvertisedServiceUuid(ts.uuid());
  blutut.addAttribute(ts);
  blutut.addAttribute(tc);
  blutut.addAttribute(td);

  blutut.setAdvertisedServiceUuid(hs.uuid());
  blutut.addAttribute(hs);
  blutut.addAttribute(hc);
  blutut.addAttribute(hd);

  blutut.setAdvertisedServiceUuid(ps.uuid());
  blutut.addAttribute(ps);
  blutut.addAttribute(pc);
  blutut.addAttribute(pd);

  blutut.setEventHandler(BLEConnected, blututConnectHandler);
  blutut.setEventHandler(BLEDisconnected, blututDisconnectHandler);

  blutut.begin();
  Serial.println("setapovao");
}

void sendValues(int t, int p, int h){  
  tc.setValue(t);
  pc.setValue(p);
  hc.setValue(h);
  
  Serial.println("poslao na ble");
}

bool BLEconnected = false;
void connectToBLE(){
  BLEconnected=true;
}
void disconnectFromBLE(){
  BLEconnected=false;
}

void setup() {
  Serial.begin(9600);
  Serial.println("EEEE");
  blututSetup();
  senzor.begin();
  blutut.setEventHandler(BLEConnected, connectToBLE);
  blutut.setEventHandler(BLEDisconnected, disconnectFromBLE);
}

int previousMilis = 0;
void loop() {
    
  int currentMilis = millis(); //How much time has passed
  if(currentMilis - previousMilis < 1000) //Not enough time since last time? skip.
    return
  previousMilis = currentMilis;
  
  Serial.println("e");
  blutut.poll();
  
  //Read off of sensor
  int t = int(senzor.readTemperature()), 
      p = int(senzor.readPressure()/1000),
      h = int(senzor.readHumidity());
  Serial.println("ucitao sa senzora");  
  
  if(BLEconnected)
    sendValues(t,p,h);
}
