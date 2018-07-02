// Code for HackSpace magazine
// Displays temperature and humidity values on a I2C OLED Display 128X64 0.96"
// Also shows a scrolling realtime temperature graph
// REQUIRES a DHT11 sensor and a SSD1306  display plus the following libraries:
// https://github.com/RobTillaart/Arduino/tree/master/libraries/DHTlib
// https://github.com/adafruit/Adafruit_SSD1306
// https://github.com/adafruit/Adafruit-GFX-Library


#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <dht.h>


// Pin that communicated with the DHT11
#define DHT11_PIN 2

// Width of our display - used as the maximum stack value
#define MAXSTACK 128

// I2C address for the screen. Update this for your screen.
#define SCREENADR 0x3C

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

dht ourDHT;
Stack temperature_stack;
Adafruit_SSD1306 display(4);

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, SCREENADR);
  display.clearDisplay();
}

// Function to display a character
void displayNum() {
  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(0, 0);
  display.println("Temp:" + String(ourDHT.temperature) + " Hum:" + String(ourDHT.humidity));
}

// Function to display the chart
void displayChart() {
  for (int x = 0; x < MAXSTACK; x++) {
    display.drawLine(x, display.height(), x, 0, BLACK);
    display.drawLine(x, display.height(), x, display.height() - temperature_stack.peek(x), WHITE);
  }
}

void loop() {
  int chk = ourDHT.read11(DHT11_PIN);
  temperature_stack.push(ourDHT.temperature);
  displayChart();
  displayNum();
  display.display();
  delay(100);

}
