/**
 * ============================================================================
 * GALAKSIJA (1983) MICROCOMPUTER EMULATOR FOR ARDUINO IDE
 * Target Board: Waveshare RP2350-PiZero / Raspberry Pi Pico 2 (RP2350)
 * Core: Earle Philhower RP2040/RP2350 Arduino Core
 * ============================================================================
 * Features:
 * - Dual-Core Execution: Core 0 (Z80 CPU & OS) + Core 1 (HSTX DVI 640x480)
 * - Video: Digital DVI-D output on onboard Mini-HDMI connector (GP12..GP19)
 * - Storage: Onboard TF/MicroSD card slot via SPI1 (GP20..GP23) with SDFS/FAT32
 * - Audio: Authentic 1-bit Galaksija Beeper PWM on GP26/GP20
 * - Keyboard: USB Type-C Host via TinyUSB
 * ============================================================================
 */

#include <Arduino.h>
#include <SPI.h>
#include <SDFS.h>
#include "galaksija_core.h"
#include "galaksija_dvi.h"
#include "galaksija_sd.h"
#include "galaksija_sound.h"
#include "galaksija_keyboard.h"

// Galaksija Global Instance
GalaksijaCore galaksija;

/**
 * CORE 1: DEDICATED REAL-TIME VIDEO GENERATION
 * Outputs 640x480 @ 60Hz DVI-D signal through the RP2350 HSTX hardware block.
 */
void setup1() {
  delay(100);
  galaksija_dvi_init();
  Serial.println(F("[Core 1] HSTX DVI Video Controller Started (640x480@60Hz)"));
}

void loop1() {
  // Continuously render 16 rows of 32 characters directly from Galaksija VRAM (0x2800)
  galaksija_dvi_render_frame(galaksija.getVramPointer());
}

/**
 * CORE 0: Z80 CPU, BASIC INTERPRETER, KEYBOARD & SD CARD
 */
void setup() {
  // 1. Initialize USB Serial for debugging
  Serial.begin(115200);
  delay(500);
  Serial.println(F("=================================================="));
  Serial.println(F(" GALAKSIJA RP2350-PIZERO (ARDUINO CORE 4.x)"));
  Serial.println(F(" (C) 1983 Voja Antonic / 2026 RP2350 Port"));
  Serial.println(F("=================================================="));

  // 2. Initialize Sound Beeper
  galaksija_sound_init(PIN_BEEPER_PWM);

  // 3. Initialize USB Host for Keyboard
  galaksija_keyboard_init();

  // 4. Initialize MicroSD Card on SPI1
  if (galaksija_sd_init()) {
    Serial.println(F("[SD] MicroSD Card mounted successfully!"));
    // Try to auto-load AUTOEXEC.GAL if present on card
    if (galaksija_sd_exists("AUTOEXEC.GAL")) {
      Serial.println(F("[SD] Found AUTOEXEC.GAL - Auto-booting..."));
      galaksija_sd_load_basic("AUTOEXEC.GAL", &galaksija);
    }
  } else {
    Serial.println(F("[SD] Warning: MicroSD Card not detected or not formatted FAT32."));
  }

  // 5. Initialize Galaksija Hardware & Reset Z80
  galaksija.init(ROM_CONFIG_GALAKSIJA_PLUS, RAM_CONFIG_6KB);
  galaksija.reset();

  Serial.println(F("[System] Galaksija Ready. READY prompt displayed."));
}

void loop() {
  // 1. Handle USB Keyboard inputs
  galaksija_keyboard_task(&galaksija);

  // 2. Handle SD Card Hot-keys (F11 = SD Menu, F12 = Dump RAM)
  galaksija_sd_task(&galaksija);

  // 3. Execute Z80 Machine Cycles (Equivalent to 3.072 MHz Z80A)
  galaksija.executeCycles(3072);
  
  yield();
}
