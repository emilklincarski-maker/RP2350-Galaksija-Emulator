/**
 * Galaksija MicroSD Storage Driver for RP2350-PiZero
 * Hardware: Onboard TF Slot on SPI1
 * GP22: CS (Chip Select)
 * GP21: SCK (Clock @ 12MHz)
 * GP20: MISO (Data RX)
 * GP23: MOSI (Data TX)
 */
#ifndef GALAKSIJA_SD_H
#define GALAKSIJA_SD_H

#include <Arduino.h>
#include <SPI.h>
#include <SDFS.h>

class GalaksijaCore;

#define PIN_SD_CS   22
#define PIN_SD_SCK  21
#define PIN_SD_MISO 20
#define PIN_SD_MOSI 23

bool galaksija_sd_init(void);
bool galaksija_sd_exists(const char* filename);
bool galaksija_sd_load_basic(const char* filename, GalaksijaCore* core);
bool galaksija_sd_save_basic(const char* filename, GalaksijaCore* core);
bool galaksija_sd_load_binary(const char* filename, uint16_t loadAddr, GalaksijaCore* core);
void galaksija_sd_task(GalaksijaCore* core);

#endif // GALAKSIJA_SD_H
