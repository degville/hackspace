// Code for HackSpace magazine
// Scrolling space game for Arduino - Part 2
// See how far you can navigate your spaceship/submarine down a tunnel
// Requires n Arduino UNO, I2C OLED Display 128X64 0.96" and analogue joystick
//
// https://github.com/adafruit/Adafruit_SSD1306
// https://github.com/adafruit/Adafruit-GFX-Library


#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

// Width of our display - used as the maximum stack value
const int MAXSTACK = 128;

// Maximum number of stars to show
const int MAXSTARS = 10;

// I2C address for the screen. Update this for your screen.
const int SCREENADR = 0x3C;

// Analoguejoystick connections for X and Y
const int JOYY = A0;
const int JOYX = A1;

// Digital input for the Joystick switch
const int SWITCH_PIN = 7;

class Stack
{
  private:
    int ourList[MAXSTACK];
    int top;

  public:

    Stack() {
      top = 0;
      for (int i = 0; i <= MAXSTACK; i++)
        ourList[i] = 0;
    }

    void push(int item) {

      if (top == MAXSTACK)
        top = 0;

      ourList[top++] = item;
    }

    int peek(int x) {
      return ourList[(top + x) % MAXSTACK];
    }
};

struct stars {
  int x, y, speed, size;
};

const unsigned char shipBMP [] PROGMEM = {
  // 'ship, 16x16px
  0x00, 0x00, 0x70, 0x00, 0x38, 0x00, 0x1f, 0xe0, 0x18, 0x10, 0x1b, 0x08, 0x9b, 0x88, 0xd9, 0x2c,
  0xfb, 0xae, 0xff, 0xff, 0xdf, 0xff, 0x9f, 0xfe, 0x1f, 0xfc, 0x38, 0x00, 0x70, 0x00, 0x00, 0x00
};


Adafruit_SSD1306 display(4);
Stack land_stack;
int counter, second_counter;
int difficulty;
int shipspeed, shipx, shipy;
int switchstate;
int tunnel_size;
int current_score, high_score, score_counter;
bool playstate = false;

stars starfield[MAXSTARS];

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, SCREENADR);
  display.clearDisplay();

  pinMode(SWITCH_PIN, INPUT_PULLUP);

  initGame();
}

void initGame() {

  counter = 45;
  second_counter = 45;
  difficulty = 5;
  shipspeed = 10;
  shipx = 10;
  shipy = 10;
  switchstate = 0;
  tunnel_size = 80;
  current_score = 0;

  for (int i = 0; i < MAXSTARS; i++) {
    starfield[i].x = random(0, (display.width() * 10));
    starfield[i].y = random(0, display.height());
    starfield[i].speed = random(1, 50);
    starfield[i].size = random(1, difficulty - 2);
  }

  for (int x = 0; x < MAXSTACK; x++) {
    land_stack.push(0);
  }

}

// Function to display a character
void displayScore() {
  if (current_score > high_score)
    high_score = current_score;

  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(0, 0);
  display.println("Score:" + String(current_score) + "     High:" + String(high_score));
  display.setCursor(10, 28);
  display.setTextSize(2);
  display.println("Game Over");

}

// Function to display the tunnel, adapted from previous chart examples.
bool displayTunnel() {
  int height;
  for (int x = 0; x < MAXSTACK; x++) {
    if (land_stack.peek(x) != 0) {
      height = display.height() - ((land_stack.peek(x) + sin(counter * 3.1 / 180) * difficulty));

      display.drawLine(x, height - (tunnel_size / 2), x, -1, WHITE);
      display.drawLine(x, display.height(), x, height + (tunnel_size / 2) , WHITE);

      if (x == shipx) {
        if ( (shipy < (height - (tunnel_size / 2) )) || ((shipy + 12) > (height + (tunnel_size / 2) )) ) {
          playstate = false;
        }
      }
    }
  }
}

// Simple starfield for background and start/end screens
void displayStars() {
  for (int i = 0; i < MAXSTARS; i++) {
    display.fillCircle((starfield[i].x / 10), starfield[i].y, starfield[i].size, WHITE);
    starfield[i].x = (starfield[i].x - starfield[i].speed);
    if (starfield[i].x < 0) {
      starfield[i].x = (display.width() * 10);
      starfield[i].y = random(0, display.height());
      starfield[i].speed = random(1, 50);
      starfield[i].size = random(1, difficulty - 2);
    }
  }
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

  if (playstate) {

    if (counter > 180)
      counter = -180;

    if (second_counter > 120)
      second_counter = -90;

    land_stack.push((sin(counter * 3.1 / 180) + 1.1) * (4 * difficulty));
    counter = counter + sin((second_counter++ * 3.1 / 180) + 1.1) * difficulty;

    updateShip();
    displayTunnel();
    displayShip(shipx, shipy);
    displayStars();

    if (score_counter++ == 100) {
      tunnel_size--;
      score_counter = 0;
      current_score++;
    }

  } else {

    displayStars();
    displayScore();
    switchstate = digitalRead(SWITCH_PIN);

    if (switchstate == LOW) {
      initGame();
      playstate = true;
    }
  }

  display.display();
  delay(1);
  display.fillScreen(BLACK);

}
