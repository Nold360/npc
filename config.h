#ifndef NPC_CONFIG
#define NPC_CONFIG

#include <Arduino.h>

#define NPC_VERSION 0x42

/*
 * Hardware Settings
 * LEDs:
 */
#define LED_PIN 3
#define LED_NUM 9 //count of leds

// Display Control / DAC Pin
#define DAC_PIN A0 // A0 aka 1 = DAC on Trinket M0
#define DAC_RES 10 // 10 Bit DAC

// Bluetooth Audio Pins
#define BTAUDIO_ENABLE_PIN 17
#define BTAUDIO_SYNC_PIN 18

/*
 * I2C Interface
 */
#define I2C_ADDR 0x42

//Feature Flags
#define I2C_FEAT_LED     0b00000001
#define I2C_FEAT_DISPLAY 0b00000010
#define I2C_FEAT_BTAUDIO 0x00000100

// Enabled Features
#define I2C_FEATURES (I2C_FEAT_LED | I2C_FEAT_DISPLAY | I2C_FEAT_BTAUDIO)
#define CMD_VERSION  0x00
#define CMD_FEATURES 0x01
#define CMD_RESET 0x02

// Write Ops / LED commands
// 15 LEDs possible:
#define CMD_LED_COLOR 0x10
#define CMD_LED_COLOR_END 0x1F

#define CMD_LED_SPEED 0x0A // for animation modes
#define CMD_LED_INDEX 0x0B // for mode = COLOR
#define CMD_LED_NUM 0x0C   // Number of LEDS
#define CMD_LED_MODE 0x0D  // Current Mode
#define CMD_LED_PREDEF_COLORS 0x0E // Count of predefined colors
#define CMD_LED_ANIMATIONS 0x0F // Number of predefined animations

// Display Commands
// Offset for Display commands
#define CMD_DISPLAY 0x30
// Display Actions to trigger
#define CMD_UP 0x30
#define CMD_DOWN 0x31
#define CMD_MENU 0x32
#define CMD_ENTER 0x33

//BTAUDIO Commands
#define CMD_BTAUDIO_ENABLE 0x41
#define CMD_BTAUDIO_PAIR 0x42

// LED Modes
#define LED_STATIC 1
#define LED_ANIMATION 2
#define LED_COLOR 3

// Number of predefined animations & colors
#define ANIMATION_COUNT 1
#define PREDEF_COLORS 13

// Depends on LEDs used:
int pixelFormat = NEO_GRB + NEO_KHZ800;

//Voltages for display actions
// ENTER == Power?
// MENU == Source?
// +, -, "DOWN", MENU
float display_cmd_voltage[] = {0.22, 2.62, 0.56, 1.32};

// -------- END CONFIG  -------------
// LED Colors in LED Struct
#define LED_R 0
#define LED_G 1
#define LED_B 2

// Whole config in a struct
typedef struct {
  // Metadata to force updates
  boolean valid;
  uint8_t version = 0;
  
  uint8_t count = LED_NUM;

  // Current LED Mode
  uint8_t mode = LED_STATIC;
  
  // Animation speed
  uint8_t speed = 100;

  // Current color in "color" mode or animation in animation mode
  uint8_t mode_index = 0;

  // Names of LEDs, 16 Chars max.
  char name[LED_NUM][16] = {"DPAD_0", "DPAD_1", "A_0", "A_1", "B", "Y_0", "Y_1", "X_0", "X_1"};

  // Default & current color in "static" mode
  uint8_t default_color[LED_NUM][3] = {{0, 128, 128}, {0, 128, 128}, {0, 255, 0}, {0, 255, 0}, {255, 0, 0}, {64, 64, 64}, {64, 64, 64}, {128, 128, 128}, {128, 128, 128}};
  uint8_t color[LED_NUM][3] = {{0, 128, 128}, {0, 128, 128}, {0, 255, 0}, {0, 255, 0}, {255, 0, 0}, {64, 64, 64}, {64, 64, 64}, {128, 128, 128}, {128, 128, 128}};

} config;

// Predefined colors for mode == "COLORS"
const uint8_t led_colors[PREDEF_COLORS][3] = {
    {0,0,255},
    {0,255,0},
    {255,0,0},
    {255.128,0},
    {255,255,0},
    {255,255,255},
    {0,128,255},
    {0,255,255},
    {255,0,255},
    {128,0,0},
    {0,128,0},
    {0,0,128},
    {0,0,0}
};

bool btaudio_enabled = false;

FlashStorage(config_store, config);

config leds;
Adafruit_NeoPixel *pixels;
#endif
