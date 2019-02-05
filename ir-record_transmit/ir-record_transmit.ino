// Code for HackSpace magazine
// Infrared receiver and transmitter with a button
// Hold button to record infrared signal
// Press button briefly to send dignal
// Requires an Arduino UNO, button, reciever and transmitter


#include <IRremote.h>

const int RECV_PIN = 10; // IR receiver input pin
const int BUTN_PIN = 7;

long buttonTimer = 0;
long longPressTime = 250;

bool buttonActive = false;
bool longPressActive = false;

int msglen = 0;
unsigned int receivedData[RAWBUF];
int khz = 38;

IRrecv irrecv(RECV_PIN);
IRsend irsend;
decode_results results; // decode_results class is defined in IRremote.h


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTN_PIN, INPUT);
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver

}

void loop() {

  if (digitalRead(BUTN_PIN) == HIGH) {

    if (buttonActive == false) {

      buttonActive = true;
      buttonTimer = millis();

    }

    if ((millis() - buttonTimer > longPressTime) && (longPressActive == false)) {

      longPressActive = true;

      digitalWrite(LED_BUILTIN, HIGH);

      Serial.println("long press ");
    }
  } else {

    if (buttonActive == true) {

      if (longPressActive == true) {

        longPressActive = false;
        digitalWrite(LED_BUILTIN, LOW);

      } else {

        Serial.println("short press");
        if (msglen) {
          Serial.println("Send IR length: ");
          Serial.println(msglen, DEC);
                  irsend.sendRaw(receivedData, msglen, khz);
        delay(50);
        irrecv.enableIRIn();
        }
      }
      buttonActive = false;
    }
  }

  if (irrecv.decode(&results)) {
    if (longPressActive) {
      Serial.println("GO GO GO");
      //storeCode(&results);
      msglen = results.rawlen - 1;

      for (int i = 1; i <= msglen; i++) {
        if (i % 2) {
          receivedData[i - 1] = results.rawbuf[i] * USECPERTICK - MARK_EXCESS;
        }
        else {
          // Space
          receivedData[i - 1] = results.rawbuf[i] * USECPERTICK + MARK_EXCESS;
        }
      }
    }
    irrecv.resume(); // resume receiver
  }

}



