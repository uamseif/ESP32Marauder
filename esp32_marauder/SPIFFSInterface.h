#ifndef SPIFFSInterface_h
#define SPIFFSInterface_h

#include "configs.h"

#include "Buffer.h"
#ifdef HAS_SCREEN
#include "Display.h"
#endif

extern Buffer buffer_obj;
#ifdef HAS_SCREEN
extern Display display_obj;
#endif

class SPIFFSInterface {

private:

public:
    uint64_t cardSizeMB;
    bool do_save = true;
    String spiffs_sz;
    bool initSPIFFS();
    void addPacket(uint8_t* buf, uint32_t len);
    void openCapture(String file_name = "");
    void main();
    //void savePacket(uint8_t* buf, uint32_t len);
};

#endif
