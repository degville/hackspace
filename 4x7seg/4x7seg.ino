// Code for HackSpace magazine.
// Displays hexadecimal numbers on a quad seven segment display

// true = common anode; false = common cathode
const bool ANODE = true;

// DISPLAY ORDER: a,b,c,d,e,f,g,decimal point, digit 1, digit 2, digit 3, digit 4
const byte segPin[8] = {2, 3, 4, 5, 6, 7, 8};
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

  delay(5);
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
  for (int i = 0; i <= 9999; i++) {
    displayNum(i);
  }
}

