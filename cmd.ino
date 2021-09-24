/*
 * LED Commands
 */
// Set LEDS static colors
void set_leds() {
  // Reset LEDs if requested
  if(reset_leds) {
    pixels = new Adafruit_NeoPixel(leds.count, LED_PIN, pixelFormat);
    pixels->begin();
    reset_leds = false;
  }

  // Update LED colors if requested
  if(update_leds) {
    if(leds.mode == LED_COLOR)
      set_colors();
      
    pixels->clear();
    for(int i=0; i<leds.count; i++) { 
      pixels->setPixelColor(i, pixels->Color(leds.color[i][LED_R], leds.color[i][LED_G], leds.color[i][LED_B]));
    }
    
    pixels->show();
    update_leds = false;
  }

  // Play Animations
  if (leds.mode == LED_ANIMATION) {
    switch(leds.mode_index) {
      default: rainbow(leds.speed);
    }
  }
}

// Set color of all LEDs
void set_colors() {
  for(uint8_t i = 0; i < pixels->numPixels(); i++) {
    for(uint8_t c = 0; c < 3 ; c++) {
      leds.color[i][c] = led_colors[i][c];
    }
  }
}

// Set LED mode & reset index
void set_mode(uint8_t mode) {
  leds.mode = mode;
  leds.mode_index = 0;
  update_leds = true;
}

// Set mode_index of leds
void set_mode_index(uint8_t index) {
  uint8_t max = 255, value;
    
  // Set max index value according to mode
  switch(leds.mode) {
    case(LED_ANIMATION): max = ANIMATION_COUNT; break;
    case(LED_COLOR): max = PREDEF_COLORS; break;
  }
  
  if(index <= max && index > 0) {
    leds.mode_index = index;
    update_leds = true;
  }
}

//rainbow  animation
void rainbow(uint8_t wait) {
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i < pixels->numPixels(); i++) { // For each pixel in strip...
      int pixelHue = firstPixelHue + (i * 65536L / pixels->numPixels());
      pixels->setPixelColor(i, pixels->gamma32(pixels->ColorHSV(pixelHue)));
      if(update_leds)
        return;
    }
    pixels->show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

/*
 * Display Commands
 */
void display_cmd(uint8_t cmd) {
  if(cmd < CMD_DISPLAY || cmd > CMD_ENTER)
    return;
    
  float vout = display_cmd_voltage[cmd - CMD_DISPLAY];
  
  // Writing 78 equals 78 / 1024 * 3.3V = 0.25V output.
  // Val = VOUT / 3.3 * 1024
  // The DAC on the SAMD21 is a 10-bit output, from 0-3.3V. So in theory you will have a resolution of 0.0032 Volts per bit. 
  //
  // range 0 - 1023, = 10 Bit

  //make sure pin is high & trigger pin
  analogWrite(DAC_PIN, 1023);
  delay(500);
  analogWrite(DAC_PIN, ((vout * 1023)/3.3));
  delay(500);
  analogWrite(DAC_PIN, 1023);
}


/*
 * Bluetooth Audio Commands
 *  - Enable BT Audio Module
 *  - Enable BT Audio Pairing mode
 */
void toggle_btaudio_enable() {
  btaudio_enabled = !btaudio_enabled;
  digitalWrite(BTAUDIO_ENABLE_PIN, btaudio_enabled);
}

void toggle_btaudio_pair() {
   digitalWrite(BTAUDIO_SYNC_PIN, HIGH);
   delay(500);
   digitalWrite(BTAUDIO_SYNC_PIN, LOW);
}
