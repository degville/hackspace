// Code for Hackspace magazine.
// Generates a tone from a speaker and modulates its pitch using an ADS envelope.
// Requirements: Arduino UNO, speaker, 10k ohm resistor and momentary switch.

const int interruptPin = 2;
const int piezoPin = 3;
const int pitchEnv[] = {1000, 250, 200}; // Attack time, decay time, sustain value
const int pitchMax = 255;

bool trigger = false;
unsigned long note_time;

void setup() {
  attachInterrupt(digitalPinToInterrupt(interruptPin), triggerSound, CHANGE);
  pinMode(piezoPin, OUTPUT);
  Serial.begin(9600);
}

// function hooked to the interrupt on digital pin 2
void triggerSound() {
  if (trigger = !trigger) {
    note_time = millis();
  }
}

void playSound(int pitch) {
  pitch += envMod();
  tone (piezoPin, pitch);
}

void stopSound(){
  noTone(piezoPin);
}

int envMod() {
  // Percent calculations: avoid using a float by first multiply by 100 and next divide.

  unsigned long current_dur = millis() - note_time;

  if (current_dur <= pitchEnv[0]) {  // Attack
    //Serial.println((pitchMax*(100*current_dur)/pitchEnv[0])/100);
    return ((pitchMax * (100 * current_dur) / pitchEnv[0]) / 100);
  } else if (current_dur <= (pitchEnv[0] + pitchEnv[1])) { //Decay
    // Serial.println(pitchMax - (pitchMax - pitchEnv[2]) * (100 * (current_dur - pitchEnv[0]) / pitchEnv[1]) / 100);
    return (pitchMax - (pitchMax - pitchEnv[2]) * (100 * (current_dur - pitchEnv[0]) / pitchEnv[1]) / 100);
  } else { // Sustain
    return (pitchEnv[2]);
  }
}

void loop()
{
  if (trigger) {
    playSound(261);
  } else {
    stopSound();
  }
}


