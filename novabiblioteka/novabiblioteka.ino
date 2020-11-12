#include <BLEPeripheral.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h>
#include <bme680.h>
#include <bme680_defs.h>

BLEPeripheral blutut = BLEPeripheral(46, 2, 48);
  BLEService ts = BLEService("CCC0");
  BLEIntCharacteristic tc = BLEIntCharacteristic("CCC1", BLERead | BLENotify);
  BLEDescriptor td = BLEDescriptor("2901", "Atmosphere: Temp, Humi, Pres");
    
  BLEService hs = BLEService("DDD0");
  BLEIntCharacteristic hc = BLEIntCharacteristic("DDD1", BLERead | BLENotify);
  BLEDescriptor hd = BLEDescriptor("2901", "Humidity Percent");
  
  BLEService ps = BLEService("EEE0");
  BLEIntCharacteristic pc = BLEIntCharacteristic("EEE1", BLERead | BLENotify);
  BLEDescriptor pd = BLEDescriptor("2901", "Pressure mbar");

Adafruit_BME680 senzor = Adafruit_BME680();
int previousMilis = 0;

void blututSetup(){

  blutut.setLocalName("ARDuino3");

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

  blutut.setEventHandler(BLEConnected, connectToBLE);
  blutut.setEventHandler(BLEDisconnected, disconnectFromBLE);

  blutut.begin();
  Serial.println("setapovao");
}

void sendValues(int t, int p, int h){  
  tc.setValue(t);
  pc.setValue(p);
  hc.setValue(h);
  Serial.println("Poslao na BLE.");
}

bool BLEconnected = false;
void connectToBLE(){
  BLEconnected=true;
  Serial.println("\nPovezan.");
}
void disconnectFromBLE(){
  BLEconnected=false;
  Serial.println("Odvezan.");
}

void setup() {
  Serial.begin(9600);
  Serial.println("EEEE");
  blututSetup();
  senzor.begin();
}

void loop() {
  blutut.poll();
  int currentMilis = millis(); //How much time has passed
  if(currentMilis - previousMilis < 10000 || !BLEconnected) //Not enough time since last time? skip.
    return;
  previousMilis = currentMilis;    
  
  Serial.println("loop");

  //Read off of sensor
  int t = int(senzor.readTemperature()), 
      p = int(senzor.readPressure()/100),
      h = int(senzor.readHumidity());
  String ispis = "Senzor: " + (String)t + " C, " + (String)p + " mbar, " + (String)h + "% ";
  Serial.println(ispis);  
  sendValues(t,p,h);
}
