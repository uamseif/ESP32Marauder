#include "SPIFFSInterface.h"
#include "lang_var.h"

bool SPIFFSInterface::initSPIFFS() {
  String display_string = "";


  delay(10);

  if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)){
    Serial.println("Settings SPIFFS Mount Failed");
    return false;
  }
  else {


    this->cardSizeMB = SPIFFS.totalBytes() / (1024 * 1024);

    const int NUM_DIGITS = log10(this->cardSizeMB) + 1;

    char sz[NUM_DIGITS + 1];

    sz[NUM_DIGITS] =  0;
    for ( size_t i = NUM_DIGITS; i--; this->cardSizeMB /= 10)
    {
      sz[i] = '0' + (this->cardSizeMB % 10);
      display_string.concat((String)sz[i]);
    }

    this->spiffs_sz = sz;


    buffer_obj = Buffer();

    return true;
  }
}

void SPIFFSInterface::addPacket(uint8_t* buf, uint32_t len) {
  if (this->do_save) {
    buffer_obj.addPacket(buf, len);
  }
}

void SPIFFSInterface::openCapture(String file_name) {
buffer_obj.open(&SPIFFS, file_name);
}


void SPIFFSInterface::main() {
  if (this->do_save) {
    //Serial.println("Saving packet...");
    buffer_obj.forceSave(&SPIFFS);
  }
  else {
      delay(100);
      this->initSPIFFS();
  }
}
