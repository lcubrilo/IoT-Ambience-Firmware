#include <Adafruit_Sensor.h>

#include <Adafruit_BME680.h>
#include <bme680.h>
#include <bme680_defs.h>

Adafruit_BME680 *senzor;
bool doAgain = true;

void setup() {
  senzor = new Adafruit_BME680();
  senzor->begin();
  Serial.begin(9600);
  delay(1000);
  Serial.println("Serijalna veza otvorena");
}

void writeTemp(){
  Serial.print("Temperatura: ");
  Serial.println(senzor->readTemperature());
}
void writePres(){
  Serial.print("Pritisak: ");
  Serial.println(senzor->readPressure());
}
void writeHumi(){
  Serial.print("Vlaznost: ");
  Serial.println(senzor->readHumidity());
}
void writeAlti(){
  Serial.print("Nadm. visina: ");
  Serial.println(senzor->readAltitude(1013.25));
}

bool menu(){
  //ispis
  Serial.println("************************************************");
  Serial.println("Environment sensor - choose parameter to measure");
  Serial.println("\t1. Temperature");
  Serial.println("\t2. Pressure");
  Serial.println("\t3. Humidity");
  Serial.println("\t4. Altitude");
  Serial.println("\tq. Quit program");

  //unos
  while(Serial.available() == 0) 
    delay(500);
  char c = Serial.read();

  //Flush buffera
  while(Serial.available() > 0) 
    Serial.read();

  //rad
  switch(c){
    case '1': writeTemp(); break;
    case '2': writePres(); break;
    case '3': writeHumi(); break;
    case '4': writeAlti(); break; 
    case 'q': return false;
  }

  //kraj
  senzor->endReading();
  Serial.println("\n\n\n\n");
  return true;
}

void loop() {
  if(doAgain)
    doAgain = menu();
  else
    Serial.println("Kraj programa.");
  
  delay(1000);
}
