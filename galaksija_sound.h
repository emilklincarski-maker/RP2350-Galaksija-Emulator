/**
 * Galaksija 1-bit PWM Beeper Sound Driver
 */
#ifndef GALAKSIJA_SOUND_H
#define GALAKSIJA_SOUND_H

#include <Arduino.h>

#define PIN_BEEPER_PWM 26

inline void galaksija_sound_init(uint8_t pin = PIN_BEEPER_PWM) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

inline void galaksija_beep(uint16_t freqHz, uint16_t durationMs, uint8_t pin = PIN_BEEPER_PWM) {
  if (freqHz > 0) {
    tone(pin, freqHz, durationMs);
  } else {
    noTone(pin);
  }
}

#endif // GALAKSIJA_SOUND_H
