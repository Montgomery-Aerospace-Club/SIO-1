#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "FS.h"

#include <xCore.h>
#include <xSI01.h>

xSI01 SI01;

#define I2C_SDA 26
#define I2C_SCL 27
#define PRINT_SPEED 250
#define SPI_MISO 12
#define SPI_MOSI 13
#define SPI_SCK 14
#define SD_CS 5

static unsigned long lastPrint = 0;
// ms, (m/s^2), (rad/s), idk what units is gforce in idk what is gforce im just forced to do this :skull:
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
      // Read and calculate data from SL01 sensor
    SI01.poll();
    String dataa = "";

    if ( (lastPrint + PRINT_SPEED) < millis()) {
      printGyro();  // Print "G: gx, gy, gz"
      printAccel(); // Print "A: ax, ay, az"
      printMag();   // Print "M: mx, my, mz"
      printAttitude(); // Print Roll, Pitch and G-Force
      Serial.println();
      lastPrint = millis(); // Update lastPrint time
    }
  }
}

void loop() {
  
  /*
      dataa += String(time);
    dataa.concat(",");
    dataa += String(eCO2);
    dataa.concat(",");
    dataa += String(TVOC);
    dataa.concat(",");
    dataa += String(tempC);
    dataa.concat(",");
    dataa += String(humidity);
    dataa.concat(",");
    dataa += String(tempC);
    dataa.concat(",");
    dataa += String(press);
    dataa.concat(",");
    dataa += String(pm1);
    dataa.concat(",");
    dataa += String(pm25);
    dataa.concat(",");
    dataa += String(pm10);
    dataa += "\n";
    
    if (pm1 != 69 && pm25 != 69 && pm10 != 69){
      // Serial.print(header);
      // Serial.println(dataa);
      appendFile(SD, "/data.txt", dataa.c_str());
    }
    
  */
}

void printGyro(void) {
  Serial.print("G: ");
  Serial.print(SI01.getGX(), 2);
  Serial.print(", ");
  Serial.print(SI01.getGY(), 2);
  Serial.print(", ");
  Serial.println(SI01.getGZ(), 2);

}

void printAccel(void) {
  Serial.print("A: ");
  Serial.print(SI01.getAX(), 2);
  Serial.print(", ");
  Serial.print(SI01.getAY(), 2);
  Serial.print(", ");
  Serial.println(SI01.getAZ(), 2);
}

void printMag(void) {
  Serial.print("M: ");
  Serial.print(SI01.getMX(), 2);
  Serial.print(", ");
  Serial.print(SI01.getMY(), 2);
  Serial.print(", ");
  Serial.println(SI01.getMZ(), 2);

}

void printAttitude(void) {
  Serial.print("Roll: ");
  Serial.println(SI01.getRoll(), 2);
  Serial.print("Pitch :");
  Serial.println(SI01.getPitch(), 2);
  Serial.print("GForce :");
  Serial.println(SI01.getGForce(), 2);
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


