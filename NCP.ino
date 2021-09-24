// Nold's Portable Coprocessor
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <FlashStorage.h>
#include "config.h"

volatile byte reg, cmd=0, request_data = 0;
bool update_leds = false, reset_leds = true;

/*
 * Setup
 */
void setup() {
  // Start I2C-Slave
  Wire.begin(I2C_ADDR);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

  // DAC for simulation button push on display controller
  analogWriteResolution(DAC_RES);
  analogWrite(DAC_PIN, 1023);

  flash_read();
  set_leds();
}

/*
 * Main Loop
 * 
 *  - Run "slow" commands
 *  - Update LEDs
 */
void loop() {
  // Handle slow commands
  if(cmd > 0) {
    switch(cmd) {
      case(CMD_UP): display_cmd(CMD_UP); break;
      case(CMD_DOWN): display_cmd(CMD_DOWN); break;
      case(CMD_MENU): display_cmd(CMD_MENU); break;
      case(CMD_ENTER): display_cmd(CMD_ENTER); break;
      case(CMD_BTAUDIO_ENABLE): toggle_btaudio_enable(); break;
      case(CMD_BTAUDIO_PAIR): toggle_btaudio_pair(); break;
    }
    cmd = 0;
  }

  // Update LEDs
  set_leds();
}

/*
 * I2C receiveEvent
 */
void receiveEvent(int bytesReceived) {
  uint8_t numBytes=0, buffer[16];
  // Get requested Register
  cmd = 0;
  reg = Wire.read();

  // Read data into buffer
  while(Wire.available()) {
    buffer[numBytes] = Wire.read();
    numBytes++;  
  }

  // Did we receive data?  == Write Command
  if(numBytes > 0) {
    // Set LED-Color, read whole buffer
    if(reg >= CMD_LED_COLOR && reg <= CMD_LED_COLOR_END) {
     for(int i=0; i<numBytes; i++)
       leds.color[(reg - CMD_LED_COLOR)][i] = buffer[i];
       update_leds = true;
    }
    // Set LED MODE
    else if(reg == CMD_LED_MODE) {
      leds.mode = buffer[0];
      update_leds = true;

    // Change LED Count
    } else if (reg == CMD_LED_NUM) {
      leds.count = buffer[0];
      reset_leds = true;

    // Change Animation speed
    } else if (reg == CMD_LED_SPEED) {
      leds.speed = buffer[0];

    // Change mode_index (animation type / color)
    } else if (reg == CMD_LED_INDEX) { 
      set_mode_index(buffer[0]);

    // if's unknown or "trigger" command we'll handle late
    } else {
       cmd = reg;
       return;
    }

  // No data? == Read Command  
  } else {
    // Write requested data into request_data & handle the request later
    switch(reg) {
      case(CMD_VERSION): request_data = NPC_VERSION; break;
      case(CMD_FEATURES): request_data = I2C_FEATURES; break;
      case(CMD_LED_NUM): request_data = leds.count; break;
      case(CMD_LED_MODE): request_data = leds.mode; break;
      case(CMD_LED_SPEED): request_data = leds.speed; break;
      case(CMD_LED_INDEX): request_data = leds.mode_index; break;
      case(CMD_LED_ANIMATIONS): request_data = ANIMATION_COUNT; break;
      case(CMD_LED_PREDEF_COLORS): request_data = PREDEF_COLORS; break;
      case(CMD_RESET): break; //TODO
      
      //If it's a command to execute something slow, we will do it later
      default: cmd = reg;
    }
  }
}

/*
 * I2C requestEvent
 * Send requested data to Master
 */
void requestEvent() {
  Wire.write((uint8_t *)&request_data, sizeof(request_data));
  request_data = 0;
}
