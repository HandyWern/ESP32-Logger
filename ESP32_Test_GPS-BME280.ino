#include "HardwareSerial.h"
#include "TinyGPSPlus.h"
#include "Wire.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_BME280.h"
#include "SD.h"
#include "SPI.h"

#define SEALEVELPRESSURE_HPA (1025.4)  // Check value for calibration 

Adafruit_BME280 bme;  // Assign BME280 Sensor
TinyGPSPlus gps;  // Assign GPS module
File logFile;
float temperature, humidity, pressure, alt;  // Float values for BME output

HardwareSerial SerialGPS(1);  // ???



void setup() {
  
 Serial.begin(115200);
 delay(100);

 if(!SD.begin(5)){
  Serial.println("Card Mount Failed");
 } else {
  Serial.println("SD card initialized");
 }


  bme.begin(0x76);  // Initialize BME280 Sensor
  SerialGPS.begin(9600, SERIAL_8N1, 16,17);   // tx,rx  // Initialize GPS module

}



void loop() {

temperature = bme.readTemperature();
humidity = bme.readHumidity();
pressure = bme.readPressure() / 100.0F;
alt = bme.readAltitude(SEALEVELPRESSURE_HPA);

while (SerialGPS.available() >0) {
       gps.encode(SerialGPS.read());
    }

  Serial.println("Environmetal Sensor Data Output Check");
  Serial.println("=====================================");

  Serial.print("Date:  ");
    Serial.print(gps.date.day());
    Serial.print("/");
    Serial.print(gps.date.month());
    Serial.print("/");
    Serial.println(gps.date.year());    


  Serial.print("Time:  ");  
    Serial.print(gps.time.hour());
    Serial.print(":");
    Serial.print(gps.time.minute());
    Serial.print(":");
    Serial.println(gps.time.second());
    Serial.println(" ");


    Serial.println("BME280 Sensor Data");
    Serial.println("------------------");
    Serial.println(" ");
    Serial.print("Temperature: ");
    Serial.println(temperature);
    Serial.print("Humidity:    ");
    Serial.println(humidity);
    Serial.print("Pressure:    ");
    Serial.println(pressure);
    Serial.print("Altitude:    ");
    Serial.println(alt);
    Serial.println(" ");


    Serial.println("GPS Sensor Data");
    Serial.println("------------------");
    Serial.println(" ");
 

    Serial.print("Latitude: ");  
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitude: "); 
    Serial.println(gps.location.lng(), 6);
    Serial.print("Altitude:  ");  
    Serial.println(gps.altitude.meters());
    Serial.println(" ");

    logFile = SD.open("LogFile.csv", FILE_WRITE);

    if (logFile){
      logFile.print(gps.date.day());
      logFile.print(F("/"));
      logFile.print(gps.date.month());
      logFile.print(F("/"));
      logFile.print(gps.date.year());
      logFile.print(F(","));

      if (gps.time.hour() < 10) logFile.print(F("0"));
      logFile.print(gps.time.hour());
      logFile.print(F(":"));
      if (gps.time.minute() < 10) logFile.print(F("0"));
      logFile.print(gps.time.minute());
      logFile.print(F(":"));
      if (gps.time.second() < 10) logFile.print(F("0"));
      logFile.print(gps.time.second());
      logFile.print(F(","));

      logFile.print(temperature);
      logFile.print(F(","));
    
      logFile.print(humidity);
      logFile.print(F(","));

      logFile.print(gps.location.lat(), 6);
      logFile.print(F(","));
      
      logFile.print(gps.location.lng(), 6);
      logFile.print(F(","));
      
      logFile.print(gps.altitude.meters());
      

    logFile.close();  
    } else {
      Serial.println("Error opening LogFile:");
    }

        
    delay(1000);
}
