/**
 * Galaksija USB Host Keyboard Driver
 */
#ifndef GALAKSIJA_KEYBOARD_H
#define GALAKSIJA_KEYBOARD_H

#include <Arduino.h>
#include "galaksija_core.h"

inline void galaksija_keyboard_init(void) {
  // Initialize TinyUSB Host stack for HID Keyboard
}

inline void galaksija_keyboard_task(GalaksijaCore* core) {
  // Read key events and forward to core->injectKey(c);
  if (Serial.available()) {
    char c = Serial.read();
    core->injectKey(c);
  }
}

#endif // GALAKSIJA_KEYBOARD_H
