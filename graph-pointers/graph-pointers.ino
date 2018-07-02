// Code for HackSpace magazine
// Displays sin() function on a I2C OLED Display 128X64 0.96"
// Uses pointers and a linked list
// REQUIRES a SSD1306  display plus the following libraries:cd 
// https://github.com/adafruit/Adafruit_SSD1306
// https://github.com/adafruit/Adafruit-GFX-Library


#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

// Size of stack
#define MAXSTACK 114

// I2C address for the screen. Update this for your screen.
#define SCREENADR 0x3C


struct stackNode {
  byte value = 0;
  stackNode *next;
};

class stackList {

  protected:
    byte stacksize;
    stackNode *top;
    stackNode *tail;
  public:
    stackList();
    void push (int);
    int peek (int);

};


stackList::stackList() {
  stacksize = 0;
  top = NULL;
  tail = NULL;
}

void stackList::push(int item) {

  stackNode *ptrtmp = new stackNode;

  ptrtmp->value = item;
  ptrtmp->next = NULL;

  if (tail == NULL)
    tail = ptrtmp;
  else
    top->next = ptrtmp;

  top = ptrtmp;

  if (++stacksize > MAXSTACK) {
    ptrtmp = tail;

    tail = ptrtmp->next;
    delete ptrtmp;

    stacksize--;
  }
}

int stackList::peek(int x) {
  byte pos = 0;
  stackNode* current = tail;

  while ((pos < x) && (current != top)) {
    current = current->next;
    pos++;
  }

  if (x > pos)
    return -1;
  else
    return current->value;
}


Adafruit_SSD1306 display(4);
stackList temp_stack;

int counter = 0;

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, SCREENADR);
  display.clearDisplay();
}

// Function to display the chart
void displayChart() {

  char x = 0;
  int value = 0;

  value = temp_stack.peek(x);

  while (value != -1) {
    display.drawLine(x, display.height(), x, 0, BLACK);
    display.drawLine(x, display.height(), x, display.height() - value, WHITE);

    value = temp_stack.peek(++x);
  }

}

void loop() {

  if (counter > 180)
    counter = -180;

  temp_stack.push((sin(counter * 3.14 / 180) + 1.1) * 29);
  counter = counter + 2;

  displayChart();
  display.display();
  delay(1);

}
