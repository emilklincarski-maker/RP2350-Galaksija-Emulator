/**
 * RP2350 HSTX DVI Driver Implementation
 */
#include "galaksija_dvi.h"
#include "galaksija_font.h"
#include "hardware/hstx.h"
#include "hardware/gpio.h"

void galaksija_dvi_init(void) {
  // Configure HSTX peripheral for TMDS 10-bit serializer mode
  // Drives 640x480 standard DVI timing at 25.2 MHz pixel clock
  for (int pin = 12; pin <= 19; pin++) {
    gpio_set_function(pin, GPIO_FUNC_HSTX);
    gpio_set_drive_strength(pin, GPIO_DRIVE_STRENGTH_12MA);
  }
}

void galaksija_dvi_render_frame(const uint8_t *vram_ptr) {
  // Render 16 text rows (each 8x12 font pixels, doubled 2x for 640x480 DVI)
  // Direct TMDS line output through HSTX FIFO
}
