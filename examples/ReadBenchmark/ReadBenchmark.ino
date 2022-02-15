#include <SerialFlash.h>
#include <SPI.h>

#define VSPI_CS	5
#define VSPI_HD	21
#define VSPI_WP	22
SPIClass vspi(VSPI);
SerialFlashFile file;

void setup() {
 	pinMode(VSPI_HD,OUTPUT);
	pinMode(VSPI_WP,OUTPUT);
	digitalWrite(VSPI_HD,HIGH);
	digitalWrite(VSPI_WP,HIGH);

  Serial.begin(115200);

  if (!SerialFlash.begin(vspi,VSPI_CS)) {
    while (1) {
      Serial.println(F("Unable to access SPI Flash chip"));
      delay(2500);
    }
  }
	/*
	if(!SerialFlash.createErasable("tset.txt", 1024*1024)){
		Serial.println("file create error");
	}*/

  SerialFlash.opendir();
  int filecount = 0;
  while (1) {
    char filename[64];
    uint32_t filesize;

    if (SerialFlash.readdir(filename, sizeof(filename), filesize)) {
      Serial.print(F("  "));
      Serial.print(filename);
      Serial.print(F(", "));
      Serial.print(filesize);
      Serial.print(F(" bytes"));
      SerialFlashFile file = SerialFlash.open(filename);
      if (file) {
        unsigned long usbegin = micros();
        unsigned long n = filesize;
        char buffer[256];
        while (n > 0) {
          unsigned long rd = n;
          if (rd > sizeof(buffer)) rd = sizeof(buffer);
          file.read(buffer, rd);
          n = n - rd;
        }
        unsigned long usend = micros();
        Serial.print(F(", read in "));
        Serial.print(usend - usbegin);
        Serial.print(F(" us, speed = "));
        Serial.print((float)filesize * 1000.0 / (float)(usend - usbegin));
        Serial.println(F(" kbytes/sec"));
        file.close();
      } else {
        Serial.println(F(" error reading this file!"));
      }
      filecount = filecount + 1;
    } else {
      if (filecount == 0) {
        Serial.println(F("No files found in SerialFlash memory."));
      }
      break; // no more files
    }
  }
}

void spaces(int num) {
  for (int i=0; i < num; i++) {
    Serial.print(' ');
  }
}

void loop() {
	
}

void error(const char *message) {
  while (1) {
    Serial.println(message);
    delay(2500);
  }
}
