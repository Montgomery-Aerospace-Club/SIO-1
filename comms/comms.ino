#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "FS.h"

#include <xCore.h>
#include <xSI01.h>

xSI01 SI01;

#define I2C_SDA 26
#define I2C_SCL 27
#define PRINT_SPEED 250 // ms?
#define SPI_MISO 12
#define SPI_MOSI 13
#define SPI_SCK 14
#define SD_CS 5

static unsigned long lastPrint = 0;
// ms, (m/s^2), (rad/s), idk what units is gforce in idk what is gforce im just forced to do this 💀
String header = "time(ms),ax,ay,ax,mx,my,mz,roll,pitch,gforce";

void setup() {
  Serial.begin(115200);
  Wire.setPins(I2C_SDA, I2C_SCL);
  Wire.setClock(400000);
  Wire.begin();

  if (!SI01.begin()) {
    Serial.println("Failed to communicate with SI01.");
    Serial.println("Check the Connector");
    while (1){

    }
  } else {
    Serial.println("start successful");
  }
  millis();


  SPIClass spi = SPIClass(HSPI);
  spi.begin(SPI_SCK, SPI_MISO, SPI_MOSI, SD_CS);
  if (!SD.begin(SD_CS, spi,80000000)) {
    Serial.println(F("Card Mount Failed"));
    return;
  }
  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
      Serial.println(F("No SD card attached"));
      return;
  }

  if (SD.exists("/data.txt")) {
    SD.remove(F("/data.txt"));
  }
  writeFile(SD, "/data.txt", header.c_str());
  
  while (true){
    SI01.poll();
  
    if ( (lastPrint + PRINT_SPEED) < millis()) {
      lastPrint = millis(); // Update lastPrint time
      float gyrox = SI01.getGX();
      float gyroy = SI01.getGY();
      float gyroz = SI01.getGZ();
      float ax = SI01.getAX();
      float ay = SI01.getAY();
      float az = SI01.getAZ();
      float mx = SI01.getMX();
      float my = SI01.getMY();
      float mz = SI01.getMZ();
      float roll = SI01.getRoll();
      float pitch = SI01.getPitch();
      float gforce = SI01.getGForce()
      SDcardSTUFF(lastPrint, gyrox, gyroy, gyroz, float ax, float ay, float az, float mx, float my, float mz, float roll, float pitch, float gforce)
    }
  }
}

void loop() {

}

void SDcardSTUFF( uint32_t long time, float gyrox, float gyroy, float gyroz, float ax, float ay, float az, float mx, float my, float mz, float roll, float pitch, float gforce){
  String data = "";
  dataa += String(time);
  dataa.concat(",");
  dataa += String(gyrox, 3);
  dataa.concat(",");
  dataa += String(gyroy, 3);
  dataa.concat(",");
  dataa += String(gyroz, 3);
  dataa.concat(",");
  dataa += String(ax, 3);
  dataa.concat(",");
  dataa += String(ay, 3);
  dataa.concat(",");
  dataa += String(az, 3);
  dataa.concat(",");
  dataa += String(mx, 3);
  dataa.concat(",");
  dataa += String(my, 3);
  dataa.concat(",");
  dataa += String(mz, 3);
  dataa.concat(",");
  dataa += String(roll,2);
  dataa.concat(",");
  dataa += String(pitch, 2);
  dataa.concat(",");
  dataa += String(gforce,2);
  dataa.concat(",");
  dataa += "\n";
  Serial.print(dataa);
  appendFile(SD, "/data.txt", dataa.c_str());
}

//SD CARD
void writeFile(fs::FS &fs, const char * path, const char * message) {


  File file = fs.open(path, FILE_WRITE);
  if(!file) {
    return;
  }
  if(file.print(message)) {
  } else {
  }
  file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message) {

  File file = fs.open(path, FILE_APPEND);
  if(!file) {
    return;
  }
  if(file.print(message)) {
  } else {
  }
  file.close();
}


