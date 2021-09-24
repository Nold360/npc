#include "config.h"

void flash_write() {
  config tmp = config_store.read();
  
  if(leds.version > tmp.version)
  config_store.write(leds);
}

void flash_init() {
  leds.valid = true;
  flash_write();
}


// Read & Write Flash to save/load config
void flash_read() {
  config tmp = config_store.read();
  
  if(tmp.valid == false) {
    flash_init();
  } else {
    leds = tmp;
  }
}
