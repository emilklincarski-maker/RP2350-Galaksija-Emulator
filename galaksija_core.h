/**
 * Galaksija Core Engine Header for Arduino RP2350
 */
#ifndef GALAKSIJA_CORE_H
#define GALAKSIJA_CORE_H

#include <Arduino.h>

#define GALAKSIJA_VRAM_ADDR  0x2800
#define GALAKSIJA_VRAM_SIZE  512     // 32 cols x 16 rows
#define GALAKSIJA_SCREEN_W   32
#define GALAKSIJA_SCREEN_H   16

enum RomProfile {
  ROM_CONFIG_STANDARD_1983 = 0, // 2x ROM (8KB)
  ROM_CONFIG_GALAKSIJA_PLUS = 1, // 4x ROM (16KB)
  ROM_CONFIG_MINIMAL = 2         // 1x ROM (4KB)
};

enum RamProfile {
  RAM_CONFIG_2KB = 0,
  RAM_CONFIG_4KB = 1,
  RAM_CONFIG_6KB = 2,
  RAM_CONFIG_54KB_PLUS = 3
};

// Z80 CPU Registers Struct
struct Z80Regs {
  uint8_t a, f, b, c, d, e, h, l;
  uint16_t pc, sp, ix, iy;
  uint8_t i, r;
  bool iff1, iff2;
};

class GalaksijaCore {
public:
  GalaksijaCore();
  void init(RomProfile rom = ROM_CONFIG_STANDARD_1983, RamProfile ram = RAM_CONFIG_6KB);
  void reset();
  void executeCycles(uint32_t cycles);

  // Memory Access
  uint8_t readByte(uint16_t addr);
  void writeByte(uint16_t addr, uint8_t val);
  uint8_t* getMemory() { return memory; }
  const uint8_t* getVramPointer() const { return &memory[GALAKSIJA_VRAM_ADDR]; }

  // Virtual SD Card / Disk Mount
  bool mountBasicSource(const char* asciiSource);
  bool mountBinaryToRam(const uint8_t* binData, uint16_t length, uint16_t loadAddr, uint16_t startPc);

  // Keyboard Injection
  void injectKey(char c);

  // Screen Helpers
  void printString(const char* str);
  void clearScreen();

  Z80Regs registers;
  RomProfile currentRom;
  RamProfile currentRam;

private:
  uint8_t memory[65536]; // 64 KB unified RAM/ROM space in RP2350 SRAM
  uint8_t cursorX;
  uint8_t cursorY;
  void loadRoms();
};

#endif // GALAKSIJA_CORE_H
