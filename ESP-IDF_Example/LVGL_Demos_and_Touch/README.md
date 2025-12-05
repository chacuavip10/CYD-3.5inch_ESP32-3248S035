# SPI LCD and Touch Panel Example

[esp_lcd](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/lcd.html) provides several panel drivers out-of box, e.g. ST7789, SSD1306, NT35510. However, there're a lot of other panels on the market, it's beyond `esp_lcd` component's responsibility to include them all.

`esp_lcd` allows user to add their own panel drivers in the project scope (i.e. panel driver can live outside of esp-idf), so that the upper layer code like LVGL porting code can be reused without any modifications, as long as user-implemented panel driver follows the interface defined in the `esp_lcd` component.

This example shows how to use CYD 3.5inch ST7789 display driver from Component manager in esp-idf project. These components are using API provided by `esp_lcd` component. This example will use lv_demo_benchmark() to benchmark the system.

This example uses the [esp_timer](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/esp_timer.html) to generate the ticks needed by LVGL and uses a dedicated task to run the `lv_timer_handler()`. Since the LVGL APIs are not thread-safe, this example uses a mutex which be invoked before the call of `lv_timer_handler()` and released after it. The same mutex needs to be used in other tasks and threads around every LVGL (lv_...) related function call and code. For more porting guides, please refer to [LVGL porting doc](https://docs.lvgl.io/master/porting/index.html).

## Touch controller XPT2046

In this example you can enable touch controller XPT2046 connected via SPI. The SPI connection is shared with LCD screen.

## How to use the example

### Hardware Required

* An ESP development board
* An CYD 3.5inch, with SPI interface (with/without XPT2046 SPI touch)
* An USB cable for power supply and programming
* **On my CYD, i can set the SPI CLK Speed to maximum 80Mhz to get max performance. If you got corrupted display, lower the clk spd (80 > 60 > 40 > 20 > 10 Mhz)**

### Hardware Connection

The connection between ESP Board and the LCD is as follows:

```
       ESP Board                       CYD 3.5inch Panel + TOUCH
┌──────────────────────┐              ┌────────────────────┐
│             GND      ├─────────────►│ GND                │
│                      │              │                    │
│             3V3      ├─────────────►│ VCC                │
│                      │              │                    │
│             PCLK     ├─────────────►│ SCL                │
│                      │              │                    │
│             MOSI     ├─────────────►│ MOSI               │
│                      │              │                    │
│             MISO     |◄─────────────┤ MISO               │
│                      │              │                    │
│             RST      ├─────────────►│ RES                │
│                      │              │                    │
│             DC       ├─────────────►│ DC                 │
│                      │              │                    │
│             LCD CS   ├─────────────►│ LCD CS             │
│                      │              │                    │
│             TP CS    ├─────────────►│ TOUCH CS           │
│                      │              │                    │
│             TP INT   ├─────────────►│ TOUCH INTERUPT     │
│                      │              │                    │
│             BK_LIGHT ├─────────────►│ BLK                │
└──────────────────────┘              └────────────────────┘
```

The GPIO number used by this example can be changed in [main.c](main/_main.c).
Especially, please pay attention to the level used to turn on the LCD backlight, some LCD module needs a low level to turn it on, while others take a high level. You can change the backlight level macro `EXAMPLE_LCD_BK_LIGHT_ON_LEVEL` in [main.c](main/main.c).

### Build and Flash

Run `idf.py -p PORT build flash monitor` to build, flash and monitor the project. A fancy animation will show up on the LCD as expected.

The first time you run `idf.py` for the example will cost extra time as the build system needs to address the component dependencies and downloads the missing components from the ESP Component Registry into `managed_components` folder.

(To exit the serial monitor, type ``Ctrl-]``.)

See the [Getting Started Guide](https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html) for full steps to configure and use ESP-IDF to build projects.

### Example Output

```bash
...
I (29) boot: ESP-IDF v5.5.1 2nd stage bootloader
I (29) boot: compile time Dec  2 2025 14:38:02
I (29) boot: Multicore bootloader
I (31) boot: chip revision: v3.1
I (33) boot.esp32: SPI Speed      : 80MHz
I (37) boot.esp32: SPI Mode       : DIO
I (41) boot.esp32: SPI Flash Size : 4MB
I (44) boot: Enabling RNG early entropy source...
I (49) boot: Partition Table:
I (51) boot: ## Label            Usage          Type ST Offset   Length
I (58) boot:  0 nvs              WiFi data        01 02 00009000 00005000
I (64) boot:  1 otadata          OTA data         01 00 0000e000 00002000
I (71) boot:  2 app0             OTA app          00 10 00010000 00300000
I (77) boot:  3 spiffs           Unknown data     01 82 00310000 000f0000
I (84) boot: End of partition table
I (87) esp_image: segment 0: paddr=00010020 vaddr=3f400020 size=5b1b8h (373176) map
I (200) esp_image: segment 1: paddr=0006b1e0 vaddr=3ffb0000 size=02844h ( 10308) load
I (203) esp_image: segment 2: paddr=0006da2c vaddr=40080000 size=025ech (  9708) load
I (207) esp_image: segment 3: paddr=00070020 vaddr=400d0020 size=492e8h (299752) map
I (296) esp_image: segment 4: paddr=000b9310 vaddr=400825ec size=0da4ch ( 55884) load
I (314) esp_image: segment 5: paddr=000c6d64 vaddr=50000000 size=00020h (    32) load
I (322) boot: Loaded app from partition at offset 0x10000
I (322) boot: Disabling RNG early entropy source...
I (335) cpu_start: Multicore app
I (343) cpu_start: Pro cpu start user code
I (343) cpu_start: cpu freq: 240000000 Hz
I (343) app_init: Application information:
I (343) app_init: Project name:     CYD_3.5inch_ST7796_320x240
I (349) app_init: App version:      1
I (352) app_init: Compile time:     Dec  2 2025 14:45:07
I (357) app_init: ELF file SHA256:  6426abe7b...
I (361) app_init: ESP-IDF:          v5.5.1
I (365) efuse_init: Min chip rev:     v0.0
I (369) efuse_init: Max chip rev:     v3.99
I (373) efuse_init: Chip rev:         v3.1
I (377) heap_init: Initializing. RAM available for dynamic allocation:
I (383) heap_init: At 3FFAE6E0 len 00001920 (6 KiB): DRAM
I (388) heap_init: At 3FFCA420 len 00015BE0 (86 KiB): DRAM
I (393) heap_init: At 3FFE0440 len 00003AE0 (14 KiB): D/IRAM
I (399) heap_init: At 3FFE4350 len 0001BCB0 (111 KiB): D/IRAM
I (404) heap_init: At 40090038 len 0000FFC8 (63 KiB): IRAM
I (410) spi_flash: detected chip: generic
I (413) spi_flash: flash io: dio
I (0) main_task: Started on CPU1
I (10) main_task: Calling app_main()
I (10) example: Turn off LCD backlight
I (10) example: Initialize SPI bus
I (10) example: Install panel IO
I (10) example: Install ST7796 panel driver
I (10) st7796: version: 1.3.6
I (20) st7796_general: LCD panel create success, version: 1.3.6
I (240) example: Turn on LCD backlight
I (240) example: Initialize LVGL library
I (240) example: Install LVGL tick timer
I (240) example: Register io panel event callback for LVGL flush ready notification
I (240) example: Initialize touch controller XPT2046
I (250) example: Create LVGL task
I (250) example: Starting LVGL task
I (290) example: Display LVGL Meter Widget
I (290) main_task: Returned from app_main()
...
```


## Troubleshooting

* Why the LCD doesn't light up?
  * Check the backlight's turn-on level, and update it in `LCD_BK_LIGHT_ON_LEVEL`

For any technical queries, please open an [issue] (https://github.com/espressif/esp-idf/issues) on GitHub. We will get back to you soon.
