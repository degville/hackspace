// Code for HackSpace magazine
// Displays hexadecimal numbers on a seven segment display

// DISPLAY ORDER: a,b,c,d,e,f,g
const byte segPin[7] = {1, 7, 5, 4, 3, 2, 6};

// Characters 0,1,2,3,4,5,6,7,8,9,a,b,c,d,e,f
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


// useful for testing
//bool segNum[10][7]={
//{1,0,0,0,0,0,0}, {0,1,0,0,0,0,0},
//{0,0,1,0,0,0,0}, {0,0,0,1,0,0,0},
//{0,0,0,0,1,0,0}, {0,0,0,0,0,1,0},
//{0,0,0,0,0,0,1}, {0,0,0,0,0,0,0},
//{1,1,1,1,1,1,1}, {0,0,0,0,0,0,0},
//};

// run automatically
void setup() {
  for (int i = 0; i < 8; i++) {
    pinMode(segPin[i], OUTPUT);
  }
}


// Function to display a character
void displayNum (int number) {
  if (number <= 15) {
    for (int i = 0; i < 8; i++) {
      if (segNum[number][i]) {
        digitalWrite(segPin[i], HIGH);
      } else {
        digitalWrite(segPin[i], LOW);
      }
    }
  }
}



void loop() {
  for (int i = 0; i <= 15; i++) {
    displayNum(i);
    delay(500);
  }

}

