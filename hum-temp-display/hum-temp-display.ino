// Code for HackSpace magazine.
// Displays temperature and humidity values on a quad seven segment display
// REQUIRES a DHT11 sensor and the following library:
// https://github.com/RobTillaart/Arduino/tree/master/libraries/DHTlib

#include <dht.h>

dht ourDHT;

// Pin that communicated with the DHT11
#define DHT11_PIN 2


// true = common anode; false = common cathode
const bool ANODE = true;

// DISPLAY ORDER: a,b,c,d,e,f,g,decimal point, digit 1, digit 2, digit 3, digit 4
const byte segPin[8] = {9, 3, 4, 5, 6, 7, 8};
const byte digPin[4]  = {13, 12, 11, 10};

//// Characters 0,1,2,3,4,5,6,7,8,9,a,b,c,d,e,f
bool segNum[16][7] = {
  {1, 1, 1, 1, 1, 1, 0}, {0, 1, 1, 0, 0, 0, 0},
  {1, 1, 0, 1, 1, 0, 1}, {1, 1, 1, 1, 0, 0, 1},
  {0, 1, 1, 0, 0, 1, 1}, {1, 0, 1, 1, 0, 1, 1},
  {1, 0, 1, 1, 1, 1, 1}, {1, 1, 1, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 1, 1}, {1, 1, 1, 1, 0, 1, 1},
  {1, 1, 1, 0, 1, 1, 1}, {0, 0, 1, 1, 1, 1, 1},
  {1, 0, 0, 1, 1, 1, 0}, {0, 1, 1, 1, 1, 0, 1},
  {1, 0, 0, 1, 1, 1, 1}, {1, 0, 0, 0, 1, 1, 1},
};

// run automatically
void setup() {

  Serial.begin(115200);
  
  for (int i = 0; i < 8; i++) {
    pinMode(segPin[i], OUTPUT);
  }
  for (int i = 0; i < 4; i++) {
    pinMode(digPin[i], OUTPUT);
  }
}

// Function to display a character
void displayNum (int number) {

  int tens = 0;

  while (tens < 4) {
    displayDigit(tens++, number % 10);
    number /= 10;
  }
}

void displayDigit(int digit, int number) {

  digitalWrite(digPin[digit], HIGH);

  for (int i = 0; i < 8; i++) {
    setSegment(segPin[i], segNum[number][i]);
  }

  delay(2);
  digitalWrite(digPin[digit], LOW);

}

void setSegment(int pin, bool state) {

  if (state ^ ANODE) {
    digitalWrite(pin, HIGH);
  } else {
    digitalWrite(pin, LOW);
  }

}

void loop() {

  int chk = ourDHT.read11(DHT11_PIN);
  displayNum((ourDHT.temperature*100)+ourDHT.humidity);

}

