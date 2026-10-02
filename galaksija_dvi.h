/**
 * RP2350 Hardware HSTX DVI Video Driver for Mini-HDMI Port
 * Waveshare RP2350-PiZero Pinout:
 * GP12/13: CLK+/CLK-
 * GP14/15: D0+/D0- (Blue)
 * GP16/17: D1+/D1- (Green)
 * GP18/19: D2+/D2- (Red)
 */
#ifndef GALAKSIJA_DVI_H
#define GALAKSIJA_DVI_H

#include <Arduino.h>

void galaksija_dvi_init(void);
void galaksija_dvi_render_frame(const uint8_t *vram_ptr);

#endif // GALAKSIJA_DVI_H
