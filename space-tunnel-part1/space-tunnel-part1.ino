// Code for HackSpace magazine
// Scrolling space game for Arduino - Part 1
// See how far you can navigate your spaceship/submarine down a tunnel
// Requires n Arduino UNO, I2C OLED Display 128X64 0.96" and analogue joystick
//
// https://github.com/adafruit/Adafruit_SSD1306
// https://github.com/adafruit/Adafruit-GFX-Library


#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

// Width of our display - used as the maximum stack value
const int MAXSTACK = 128;

// I2C address for the screen. Update this for your screen.
const int SCREENADR = 0x3C;

// Analoguejoystick connections for X and Y
const int JOYY = A0;
const int JOYX = A1;

// Digital input for the Joystick switch
const int SWITCH_PIN = 7;

const unsigned char shipBMP [] PROGMEM = {
  // 'ship, 16x16px
  0x00, 0x00, 0x70, 0x00, 0x38, 0x00, 0x1f, 0xe0, 0x18, 0x10, 0x1b, 0x08, 0x9b, 0x88, 0xd9, 0x2c,
  0xfb, 0xae, 0xff, 0xff, 0xdf, 0xff, 0x9f, 0xfe, 0x1f, 0xfc, 0x38, 0x00, 0x70, 0x00, 0x00, 0x00
};

Adafruit_SSD1306 display(4);

int shipx, shipy;

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, SCREENADR);
  display.clearDisplay();
}


void displayShip(int x, int y) {
  display.drawBitmap(x, y, shipBMP, 16, 16, 1);
}

void updateShip() {
  int xValue, yValue;
  xValue = map(analogRead(JOYX), 0, 1024, 5, -8); // 5, -6 for no move backwards movement
  yValue = map(analogRead(JOYY), 0, 1024, -5, 5);

  shipx = shipx + xValue;
  shipy = shipy + yValue;

  if (shipx < 1)
    shipx = 1;
  if (shipy < 1)
    shipy = 1;
  if (shipx > display.width() - 12)
    shipx = display.width() - 12;
  if (shipy > display.height() - 12)
    shipy = display.height() - 12;

}

void loop() {

  updateShip();
  displayShip(shipx, shipy);


  display.display();
  delay(1);
  display.fillScreen(BLACK);

}
