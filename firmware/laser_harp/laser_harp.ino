#include "USB.h"
#include "USBMIDI.h"
#include <Adafruit_NeoPixel.h>

USBMIDI MIDI;

const uint8_t PIN_LASER_EN = 1;
const uint8_t PIN_NEO      = 2;
const uint8_t PIN_BTN_OCT  = 3;
const uint8_t PIN_BTN_SCL  = 4;
const uint8_t CMP_PIN[8] = {13, 12, 11, 10, 9, 8, 7, 6};

const uint8_t NUM_STRINGS = 8;
const uint8_t VELOCITY = 100;
const uint8_t CHANNEL = 1;
const uint16_t DEBOUNCE_MS = 20;

const uint8_t NUM_SCALES = 4;
const uint8_t SCALES[NUM_SCALES][8] = {
  {0, 2, 4, 5, 7, 9, 11, 12},
  {0, 2, 3, 5, 7, 8, 10, 12},
  {0, 3, 5, 7, 10, 12, 15, 17},
  {0, 2, 4, 7, 9, 12, 14, 16}
};

const uint32_t COLORS[8] = {
  0xFF2DB4, 0xFFB02E, 0xF4FF3A, 0x3AFFA0, 0x3ACBFF, 0x6A5BFF, 0xC04BFF, 0xFF5A7A
};

Adafruit_NeoPixel pixels(NUM_STRINGS, PIN_NEO, NEO_GRB + NEO_KHZ800);

uint8_t octave = 4;
uint8_t scaleIdx = 0;
bool playing[8] = {false};
bool lastRaw[8] = {false};
uint32_t lastChange[8] = {0};
bool lastOct = true, lastScl = true;

uint8_t noteFor(uint8_t s) {
  return 12 * (octave + 1) + SCALES[scaleIdx][s];
}

void showStrings() {
  for (uint8_t i = 0; i < NUM_STRINGS; i++) {
    pixels.setPixelColor(i, playing[i] ? COLORS[i] : pixels.Color(2, 2, 2));
  }
  pixels.show();
}

void allNotesOff() {
  for (uint8_t i = 0; i < NUM_STRINGS; i++) {
    if (playing[i]) MIDI.noteOff(noteFor(i), 0, CHANNEL);
    playing[i] = false;
  }
}

void setup() {
  pinMode(PIN_LASER_EN, OUTPUT);
  digitalWrite(PIN_LASER_EN, HIGH);
  for (uint8_t i = 0; i < NUM_STRINGS; i++) pinMode(CMP_PIN[i], INPUT);
  pinMode(PIN_BTN_OCT, INPUT_PULLUP);
  pinMode(PIN_BTN_SCL, INPUT_PULLUP);
  pixels.begin();
  pixels.setBrightness(80);
  showStrings();
  MIDI.begin();
  USB.begin();
}

void loop() {
  uint32_t now = millis();

  bool oct = digitalRead(PIN_BTN_OCT);
  if (!oct && lastOct) {
    allNotesOff();
    octave = (octave >= 6) ? 2 : octave + 1;
  }
  lastOct = oct;

  bool scl = digitalRead(PIN_BTN_SCL);
  if (!scl && lastScl) {
    allNotesOff();
    scaleIdx = (scaleIdx + 1) % NUM_SCALES;
  }
  lastScl = scl;

  for (uint8_t i = 0; i < NUM_STRINGS; i++) {
    bool raw = digitalRead(CMP_PIN[i]);
    if (raw != lastRaw[i]) {
      lastRaw[i] = raw;
      lastChange[i] = now;
    }
    if ((now - lastChange[i]) > DEBOUNCE_MS && raw != playing[i]) {
      playing[i] = raw;
      if (raw) MIDI.noteOn(noteFor(i), VELOCITY, CHANNEL);
      else     MIDI.noteOff(noteFor(i), 0, CHANNEL);
      showStrings();
    }
  }
  delay(1);
}
