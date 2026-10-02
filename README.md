# Galaksija (1983) Емулатор за Waveshare RP2350-PiZero (Arduino IDE)

Овој комплетен проект ви овозможува да ја преведете и flash-увате автентичната **Galaksija** и **Galaksija Plus** на развојната плочка **Waveshare RP2350-PiZero** директно преку **Arduino IDE** (верзии 1.8.x или 2.x).

---

## 1. Потребни компоненти и софтвер

1. **Развојна плочка**: Waveshare RP2350-PiZero (со вграден Mini-HDMI порт и MicroSD слот)
2. **Arduino IDE 2.x**: [https://www.arduino.cc/en/software](https://www.arduino.cc/en/software)
3. **Arduino-Pico Core (Earle Philhower)**:
   - Отворете **Arduino IDE** -> **File** -> **Preferences**
   - Во полето **Additional Boards Manager URLs** залепете:
     ```
     https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
     ```
   - Отворете **Tools** -> **Board** -> **Boards Manager**, побарајте **Raspberry Pi Pico/RP2350** и инсталирајте ја најновата верзија.

---

## 2. Инсталација на библиотеките од овој ZIP

1. Распакувајте го овој ZIP фајл.
2. Содржината од фолдерот `libraries/` (`PicoDVI_RP2350`, `Z80_Emulator`) ископирајте ја во вашиот Arduino libraries директориум:
   - **Windows**: `C:\Users\<Корисник>\Documents\Arduino\libraries\`
   - **Linux**: `~/Arduino/libraries/`
   - **macOS**: `~/Documents/Arduino/libraries/`

---

## 3. Поставки во менито Tools на Arduino IDE

Отворете го фајлот `galaksija_rp2350_arduino.ino` и поставете ги следните опции во менито **Tools**:

- **Board**: `Raspberry Pi RP2350 (Generic)` или `Waveshare RP2350-PiZero`
- **CPU Speed**: `150 MHz` (или `252 MHz` за перфектен HDMI DVI 640x480 pixel clock)
- **USB Stack**: `Adafruit TinyUSB`
- **Flash Size**: `4MB (Sketch: 2MB, FS: 2MB)`
- **Optimize**: `Optimize for Speed (-O3)`

---

## 4. Шема на пинови (Waveshare RP2350-PiZero)

| Функција | Пин на RP2350 | Опис |
|----------|---------------|------|
| **DVI CLK+/-** | GP12 / GP13 | HSTX TMDS Такт (Mini-HDMI) |
| **DVI D0+/- (Blue)** | GP14 / GP15 | HSTX Сина боја + HSYNC/VSYNC |
| **DVI D1+/- (Green)** | GP16 / GP17 | HSTX Зелена боја |
| **DVI D2+/- (Red)** | GP18 / GP19 | HSTX Црвена боја |
| **MicroSD CS** | GP22 | SPI1 Chip Select |
| **MicroSD SCK** | GP21 | SPI1 Clock @ 12MHz |
| **MicroSD MISO** | GP20 | SPI1 Data RX |
| **MicroSD MOSI** | GP23 | SPI1 Data TX |
| **USB Keyboard** | USB Type-C | TinyUSB Host преку OTG кабел |
| **Звучник / Beeper** | GP26 | 1-bit PWM Galaksija бипер |

---

## 5. Подготовка на MicroSD картичката

Форматирајте ја вашата MicroSD картичка во **FAT32** формат и ископирајте ги фајловите од фолдерот `sd_card_sample_files/`:
- `AUTOEXEC.GAL`
- `DIJAMANT.GAL`
- `ZID_BREAKOUT.GAL`
- `ASTRONOMY.GAL`
- `INVADERS.BIN`
- `MONITOR_Z80.BIN`

Секоја програма може да се вчита со наредба во Galaksija или автоматски преку F11 менито!
