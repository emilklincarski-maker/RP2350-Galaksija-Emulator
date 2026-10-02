/**
 * Galaksija MicroSD Storage Driver Implementation
 */
#include "galaksija_sd.h"
#include "galaksija_core.h"

bool galaksija_sd_init(void) {
  // Setup SPI1 on RP2350-PiZero
  SPI1.setRX(PIN_SD_MISO);
  SPI1.setSCK(PIN_SD_SCK);
  SPI1.setTX(PIN_SD_MOSI);
  SPI1.setCS(PIN_SD_CS);

  // Mount SDFS FAT32 Filesystem
  return SDFS.begin(SDFSConfig(PIN_SD_CS, 12000000, SPI1));
}

bool galaksija_sd_exists(const char* filename) {
  return SDFS.exists(filename);
}

bool galaksija_sd_load_basic(const char* filename, GalaksijaCore* core) {
  File f = SDFS.open(filename, "r");
  if (!f) return false;

  uint16_t basicStart = 0x2C3A;
  uint8_t* mem = core->getMemory();
  size_t bytesRead = f.read(&mem[basicStart], 32768);
  f.close();

  if (bytesRead > 0) {
    uint16_t varsStart = basicStart + bytesRead;
    mem[0x2A68] = varsStart & 0xFF;
    mem[0x2A69] = (varsStart >> 8) & 0xFF;
    Serial.printf("[SD] Loaded BASIC '%s' (%u bytes) into RAM at $2C3A\n", filename, (unsigned)bytesRead);
    return true;
  }
  return false;
}

bool galaksija_sd_save_basic(const char* filename, GalaksijaCore* core) {
  File f = SDFS.open(filename, "w");
  if (!f) return false;

  uint8_t* mem = core->getMemory();
  uint16_t basicStart = 0x2C3A;
  uint16_t varsStart = mem[0x2A68] | (mem[0x2A69] << 8);
  uint16_t len = varsStart - basicStart;

  size_t written = f.write(&mem[basicStart], len);
  f.close();
  Serial.printf("[SD] Saved BASIC '%s' (%u bytes)\n", filename, (unsigned)written);
  return written == len;
}

bool galaksija_sd_load_binary(const char* filename, uint16_t loadAddr, GalaksijaCore* core) {
  File f = SDFS.open(filename, "r");
  if (!f) return false;

  uint8_t* mem = core->getMemory();
  size_t bytesRead = f.read(&mem[loadAddr], 65536 - loadAddr);
  f.close();

  if (bytesRead > 0) {
    core->registers.pc = loadAddr;
    Serial.printf("[SD] Loaded Machine Code '%s' (%u bytes) at $%04X\n", filename, (unsigned)bytesRead, loadAddr);
    return true;
  }
  return false;
}

void galaksija_sd_task(GalaksijaCore* core) {
  // Periodically check for hot-plugged cards or hot-key load
}
