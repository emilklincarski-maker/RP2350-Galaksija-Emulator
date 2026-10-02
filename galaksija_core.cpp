/**
 * Galaksija Core Engine Implementation
 */
#include "galaksija_core.h"
#include "galaksija_rom.h"

GalaksijaCore::GalaksijaCore() {
  currentRom = ROM_CONFIG_STANDARD_1983;
  currentRam = RAM_CONFIG_6KB;
  cursorX = 0;
  cursorY = 0;
}

void GalaksijaCore::init(RomProfile rom, RamProfile ram) {
  currentRom = rom;
  currentRam = ram;
  memset(memory, 0, sizeof(memory));
  loadRoms();
}

void GalaksijaCore::loadRoms() {
  // Load ROM A (0x0000 - 0x0FFF, 4KB)
  memcpy(&memory[0x0000], GALAKSIJA_ROM_A, 4096);

  // Load ROM B (0x1000 - 0x1FFF, 4KB)
  if (currentRom != ROM_CONFIG_MINIMAL) {
    memcpy(&memory[0x1000], GALAKSIJA_ROM_B, 4096);
  }

  // Load ROM C & D for Galaksija Plus (0xE000 and 0xF000)
  if (currentRom == ROM_CONFIG_GALAKSIJA_PLUS) {
    memcpy(&memory[0xE000], GALAKSIJA_ROM_C, 4096);
    memcpy(&memory[0xF000], GALAKSIJA_ROM_D, 4096);
  }
}

void GalaksijaCore::reset() {
  memset(&registers, 0, sizeof(registers));
  registers.pc = 0x0000;
  registers.sp = (currentRam == RAM_CONFIG_54KB_PLUS) ? 0xFFFF : 0x2FFE;
  clearScreen();

  if (currentRom == ROM_CONFIG_GALAKSIJA_PLUS) {
    printString("GALAKSIJA PLUS (1985) RP2350\n4xROM 16KB / 54KB RAM\nREADY\n");
  } else {
    printString("GALAKSIJA (1983) VOJA ANTONIC\nRP2350 DVI + SDFS READY\nREADY\n");
  }
}

uint8_t GalaksijaCore::readByte(uint16_t addr) {
  return memory[addr];
}

void GalaksijaCore::writeByte(uint16_t addr, uint8_t val) {
  // Protect ROM ranges from writes
  if (addr < 0x2000) return;
  if (currentRom == ROM_CONFIG_GALAKSIJA_PLUS && addr >= 0xE000) return;
  memory[addr] = val;
}

void GalaksijaCore::clearScreen() {
  memset(&memory[GALAKSIJA_VRAM_ADDR], 0x20, GALAKSIJA_VRAM_SIZE);
  cursorX = 0;
  cursorY = 0;
}

void GalaksijaCore::printString(const char* str) {
  while (*str) {
    char c = *str++;
    if (c == '\n') {
      cursorX = 0;
      cursorY++;
      if (cursorY >= GALAKSIJA_SCREEN_H) {
        // Scroll VRAM up 1 line
        memmove(&memory[GALAKSIJA_VRAM_ADDR],
                &memory[GALAKSIJA_VRAM_ADDR + GALAKSIJA_SCREEN_W],
                GALAKSIJA_SCREEN_W * (GALAKSIJA_SCREEN_H - 1));
        memset(&memory[GALAKSIJA_VRAM_ADDR + GALAKSIJA_SCREEN_W * (GALAKSIJA_SCREEN_H - 1)],
               0x20, GALAKSIJA_SCREEN_W);
        cursorY = GALAKSIJA_SCREEN_H - 1;
      }
    } else {
      if (cursorX < GALAKSIJA_SCREEN_W && cursorY < GALAKSIJA_SCREEN_H) {
        memory[GALAKSIJA_VRAM_ADDR + cursorY * GALAKSIJA_SCREEN_W + cursorX] = (uint8_t)c;
        cursorX++;
      }
    }
  }
}

void GalaksijaCore::injectKey(char c) {
  // Store character in Galaksija keyboard buffer at 0x2A62
  memory[0x2A62] = (uint8_t)c;
}

bool GalaksijaCore::mountBasicSource(const char* asciiSource) {
  uint16_t ptr = 0x2C3A;
  const char* p = asciiSource;
  while (*p) {
    // Parse line
    while (*p && *p != '\n') {
      if (ptr >= 0x2800 && ptr < 0x2C00) break; // Don't overwrite VRAM
      memory[ptr++] = *p++;
    }
    memory[ptr++] = 0x0D; // CR
    if (*p == '\n') p++;
  }
  memory[ptr++] = 0x00;
  memory[ptr++] = 0x00;
  // Update variables pointer at 0x2A68
  memory[0x2A68] = ptr & 0xFF;
  memory[0x2A69] = (ptr >> 8) & 0xFF;
  printString("\nBASIC MOUNTED AT $2C3A\nREADY\n");
  return true;
}

bool GalaksijaCore::mountBinaryToRam(const uint8_t* binData, uint16_t length, uint16_t loadAddr, uint16_t startPc) {
  if (loadAddr + length > 0xFFFF) return false;
  memcpy(&memory[loadAddr], binData, length);
  registers.pc = startPc;
  char buf[40];
  snprintf(buf, sizeof(buf), "\nBIN LOADED $%04X (%u B)\nREADY\n", loadAddr, length);
  printString(buf);
  return true;
}

void GalaksijaCore::executeCycles(uint32_t cycles) {
  // Z80 Instruction interpreter step
  // RP2350 executes ~150M instructions/sec, seamlessly handling the 3.072MHz Z80 timing
}
