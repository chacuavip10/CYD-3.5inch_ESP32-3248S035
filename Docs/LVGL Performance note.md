# Setting to consider
## Flash: base on MCU
Example: ESP32-WROOM-32 (ESP32N16R8 have QIO flash/psram, flash size 16MB)
- Flash SPI mode: DIO --> `ESPTOOLPY_FLASHMODE`
- Flash Sampling Mode: STR Mode --> `ESPTOOLPY_FLASH_SAMPLE_MODE`
- Flash SPI speed: 80Mhz --> `ESPTOOLPY_FLASHFREQ`
- Flash size: 4MB --> `ESPTOOLPY_FLASHSIZE`
## Frequency
- Main XTAL frequency: Max --> `XTAL_FREQ`
- CPU Frequency: Max --> `ESP_DEFAULT_CPU_FREQ_MHZ`
## Comliper
- Optimization Level: -02 (optimize for performance) --> `COMPILER_OPTIMIZATION`
## LVGL
### Buffer
- The size of **LVGL buffer** and **double buffering** feature has big impact on performance.
- Frame buffer size >25% of the screen does not bring relevant performance boost
- Frame buffer size <10% will have severe negative effect on performance
- *Note:* The measurements are valid for frame buffer in internal SRAM. Placing the frame buffer into external PSRAM will yield worse results.
- Buffer should be in internal SRAM with DMA cap
### Fast LVGL Ram
This option puts the most frequently used LVGL functions into IRAM for execution speed up.
- Fast LVGL memory: `LV_ATTRIBUTE_FAST_MEM_USE_IRAM`
Increase LVGL Ram pool
-Size of the memory used by `lv_malloc()` in kilobytes (>= 2kB): Default 64kB, increase base on HEAP memory left (128kB): `LV_MEM_SIZE_KILOBYTES`
### Affinity main task to second core
The main LVGL task can be processed on the second core of the CPU. It can increase performance. (It is available only on dual-core chips) ==> Need check case by case
- Main task core affinity: CPU1 --> `ESP_MAIN_TASK_AFFINITY`
### String function source
String functions source: Standard C memcpy/memset ...
### Reduce default display refresh, input device read and animation step period.
This option increase max FPS with the cost of increase CPU
- Default refresh period (ms): 10ms --> `LV_DEF_REFR_PERIOD`
### Remove some color support
Uncheck: 
- Enable support for L8 color format: `LV_DRAW_SW_SUPPORT_L8`
- Enable support for AL88 color format: `LV_DRAW_SW_SUPPORT_AL88`
- Enable support for A8 color format: `LV_DRAW_SW_SUPPORT_A8`
- Enable support for I1 color format: `LV_DRAW_SW_SUPPORT_I1`
### FreeRTOS tickrate
Increase tickrate -> smoother animation but also increase context switching
- configTICK_RATE_HZ: 1000 (default 100) --> `FREERTOS_HZ`

# Benchmark result
Using `lv_demo_benchmark()`

---
### Base-line
- CPU Freq: `160Mhz`
- `COMPILER_OPTIMIZATION`: for size
- `ESP_MAIN_TASK_AFFINITY`: No affinity
- String functions source: LVGL buit'in
- `LV_DEF_REFR_PERIOD`: 30ms
- `FREERTOS_HZ`: 100
- All color enable
- ***LVGL Buffer: single, DMA, 5% size***

| Name                      | Avg. CPU | Avg. FPS | Avg. time (render+flush) |
| ------------------------- | -------- | -------- | ------------------------ |
| All scenes avg.           | 69%      | 15 FPS   | 71 ms (49 + 22)          |
| Empty screen              | 70%      | 13 FPS   | 65 ms (22 + 43)          |
| Moving wallpaper          | 83%      | 8 FPS    | 99 ms (49 + 50)          |
| Single rectangle          | 30%      | 27 FPS   | 6 ms (2 + 4)             |
| Multiple rectangles       | 59%      | 19 FPS   | 32 ms (11 + 21)          |
| Multiple RGB images       | 57%      | 26 FPS   | 16 ms (9 + 7)            |
| Multiple ARGB images      | 67%      | 21 FPS   | 29 ms (21 + 8)           |
| Rotated ARGB images       | 77%      | 16 FPS   | 41 ms (34 + 7)           |
| Multiple labels           | 73%      | 22 FPS   | 29 ms (19 + 10)          |
| Screen sized text         | 93%      | 2 FPS    | 291 ms (242 + 49)        |
| Multiple arcs             | 32%      | 23 FPS   | 7 ms (5 + 2)             |
| Containers                | 64%      | 18 FPS   | 39 ms (26 + 13)          |
| Containers with overlay   | 78%      | 10 FPS   | 73 ms (48 + 25)          |
| Containers with opa       | 73%      | 15 FPS   | 45 ms (33 + 12)          |
| Containers with opa_layer | 80%      | 12 FPS   | 58 ms (46 + 12)          |
| Containers with scrolling | 90%      | 6 FPS    | 139 ms (89 + 50)         |
| Widget demo               | 92%      | 4 FPS    | 179 ms (140 + 39)        | 

---
### Increase buffer size to 12.5%
- CPU Freq: `160Mhz`
- `COMPILER_OPTIMIZATION`: for size
- `ESP_MAIN_TASK_AFFINITY`: No affinity
- String functions source: LVGL buit'in
- `LV_DEF_REFR_PERIOD`: 30ms
- `FREERTOS_HZ`: 100
- All color enable
- ***LVGL Buffer: single, DMA, 12.5% size***

| Name                      | Avg. CPU | Avg. FPS | Avg. time (render+flush) |
| ------------------------- | -------- | -------- | ------------------------ |
| All scenes avg.           | 62%      | 17 FPS   | 43 ms (33 + 10)          |
| Empty screen              | 69%      | 15 FPS   | 47 ms (12 + 35)          |
| Moving wallpaper          | 80%      | 13 FPS   | 56 ms (32 + 22)          |
| Single rectangle          | 19%      | 24 FPS   | 3 ms (2 + 1)             |
| Multiple rectangles       | 60%      | 22 FPS   | 25 ms (10 + 15)          |
| Multiple RGB images       | 39%      | 29 FPS   | 10 ms (7 + 3)            |
| Multiple ARGB images      | 54%      | 24 FPS   | 35 ms (31 + 4)           |
| Rotated ARGB images       | 70%      | 18 FPS   | 35 ms (31 + 4)           |
| Multiple labels           | 63%      | 22 FPS   | 23 ms (18 + 5)           |
| Screen sized text         | 88%      | 5 FPS    | 142 ms (125 + 17)        |
| Multiple arcs             | 28%      | 25 FPS   | 5 ms (4 + 1)             |
| Containers                | 48%      | 24 FPS   | 21 ms (18 + 3)           |
| Containers with overlay   | 73%      | 13 FPS   | 55 ms (42 + 13)          |
| Containers with opa       | 61%      | 21 FPS   | 30 ms (25 + 5)           |
| Containers with opa_layer | 72%      | 13 FPS   | 51 ms (46 + 5)           |
| Containers with scrolling | 83%      | 10 FPS   | 78 ms (59 + 19)          |
| Widget demo               | 89%      | 6 FPS    | 97 ms (84 + 13)          | 

---
### Double buffer, 12.5% size
- CPU Freq: `160Mhz`
- `COMPILER_OPTIMIZATION`: for size
- `ESP_MAIN_TASK_AFFINITY`: No affinity
- String functions source: LVGL buit'in
- `LV_DEF_REFR_PERIOD`: 30ms
- `FREERTOS_HZ`: 100
- All color enable
- ***LVGL Buffer: Double, DMA, 12.5% size***

| Name                      | Avg. CPU | Avg. FPS | Avg. time (render+flush) |
| ------------------------- | -------- | -------- | ------------------------ |
| All scenes avg.           | 66%      | 16 FPS   | 50 ms (32 + 18)          |
| Empty screen              | 71%      | 15 FPS   | 50 ms (13 + 37)          |
| Moving wallpaper          | 83%      | 10 FPS   | 76 ms (32 + 44)          |
| Single rectangle          | 24%      | 24 FPS   | 4 ms (2 + 2)             |
| Multiple rectangles       | 56%      | 19 FPS   | 27 ms (9 + 18)           |
| Multiple RGB images       | 46%      | 29 FPS   | 12 ms (7 + 5)            |
| Multiple ARGB images      | 59%      | 23 FPS   | 23 ms (18 + 5)           |
| Rotated ARGB images       | 74%      | 18 FPS   | 38 ms (31 + 7)           |
| Multiple labels           | 71%      | 22 FPS   | 28 ms (18 + 10)          |
| Screen sized text         | 91%      | 5 FPS    | 167 ms (123 + 44)        |
| Multiple arcs             | 29%      | 25 FPS   | 5 ms (4 + 1)             |
| Containers                | 55%      | 22 FPS   | 24 ms (17 + 7)           |
| Containers with overlay   | 80%      | 12 FPS   | 64 ms (39 + 25)          |
| Containers with opa       | 65%      | 18 FPS   | 33 ms (24 + 9)           |
| Containers with opa_layer | 77%      | 12 FPS   | 56 ms (47 + 9)           |
| Containers with scrolling | 87%      | 8 FPS    | 102 ms (58 + 44)         |
| Widget demo               | 90%      | 5 FPS    | 118 ms (84 + 34)         | 

---
### Increase CPU Freq
- ***CPU Freq: `240Mhz`***
- `COMPILER_OPTIMIZATION`: for size
- `ESP_MAIN_TASK_AFFINITY`: No affinity
- String functions source: LVGL buit'in
- `LV_DEF_REFR_PERIOD`: 30ms
- `FREERTOS_HZ`: 100
- All color enable
- **LVGL Buffer: Double, DMA, 12.5% size**

| Name                      | Avg. CPU | Avg. FPS | Avg. time (render+flush) |
| ------------------------- | -------- | -------- | ------------------------ |
| All scenes avg.           | 58%      | 20 FPS   | 33 ms (25 + 8)           |
| Empty screen              | 71%      | 17 FPS   | 39 ms (10 + 29)          |
| Moving wallpaper          | 77%      | 15 FPS   | 47 ms (26 + 21)          |
| Single rectangle          | 17%      | 24 FPS   | 3 ms (2 + 1)             |
| Multiple rectangles       | 51%      | 23 FPS   | 21 ms (8 + 13)           |
| Multiple RGB images       | 33%      | 25 FPS   | 9 ms (7 + 2)             |
| Multiple ARGB images      | 53%      | 30 FPS   | 16 ms (14 + 2)           |
| Rotated ARGB images       | 66%      | 23 FPS   | 25 ms (22 + 3)           |
| Multiple labels           | 60%      | 29 FPS   | 18 ms (14 + 4)           |
| Screen sized text         | 85%      | 8 FPS    | 98 ms (87 + 1)           |
| Multiple arcs             | 26%      | 24 FPS   | 3 ms (3 + 0)             |
| Containers                | 39%      | 27 FPS   | 16 ms (14 + 2)           |
| Containers with overlay   | 73%      | 15 FPS   | 46 ms (34 + 12)          |
| Containers with opa       | 54%      | 23 FPS   | 21 ms (18 + 3)           |
| Containers with opa_layer | 64%      | 18 FPS   | 36 ms (33 + 3)           |
| Containers with scrolling | 79%      | 12 FPS   | 61 ms (47 + 14)          |
| Widget demo               | 86%      | 8 FPS    | 73 ms (64 + 9)           | 

---
### Compiler optimization & affinity CORE 0
- **CPU Freq: `240Mhz`**
- ***`COMPILER_OPTIMIZATION`: -02 performance***
- ***`ESP_MAIN_TASK_AFFINITY`: Core 0***
- String functions source: LVGL buit'in
- `LV_DEF_REFR_PERIOD`: 30ms
- `FREERTOS_HZ`: 100
- All color enable
- **LVGL Buffer: Double, DMA, 12.5% size**

| Name                      | Avg. CPU | Avg. FPS | Avg. time (render+flush) |
| ------------------------- | -------- | -------- | ------------------------ |
| All scenes avg.           | 54%      | 20 FPS   | 31 ms (27 + 4)           |
| Empty screen              | 73%      | 19 FPS   | 35 ms (11 + 24)          |
| Moving wallpaper          | 71%      | 16 FPS   | 40 ms (26 + 14)          |
| Single rectangle          | 15%      | 24 FPS   | 2 ms (2 + 0)             |
| Multiple rectangles       | 56%      | 30 FPS   | 17 ms (7 + 10)           |
| Multiple RGB images       | 31%      | 25 FPS   | 7 ms (6 + 1)             | 
| Multiple ARGB images      | 39%      | 29 FPS   | 15 ms (14 + 1)           |
| Rotated ARGB images       | 62%      | 24 FPS   | 23 ms (21 + 2)           |
| Multiple labels           | 55%      | 25 FPS   | 19 ms (17 + 2)           |
| Screen sized text         | 86%      | 7 FPS    | 107 ms (103 + 4)         |
| Multiple arcs             | 27%      | 24 FPS   | 4 ms (4 + 0)             |
| Containers                | 36%      | 27 FPS   | 16 ms (15 + 1)           |
| Containers with overlay   | 68%      | 15 FPS   | 42 ms (36 + 6)           |
| Containers with opa       | 47%      | 23 FPS   | 19 ms (19 + 0)           |
| Containers with opa_layer | 64%      | 20 FPS   | 32 ms (31 + 1)           |
| Containers with scrolling | 79%      | 12 FPS   | 57 ms (50 + 7)           |
| Widget demo               | 86%      | 8 FPS    | 74 ms (71 + 3)           |

---
### Compiler optimization & affinity CORE 1
- **CPU Freq: `240Mhz`**
- ***`COMPILER_OPTIMIZATION`: -02 performance***
- ***`ESP_MAIN_TASK_AFFINITY`: Core 1***
- String functions source: LVGL buit'in
- `LV_DEF_REFR_PERIOD`: 30ms
- `FREERTOS_HZ`: 100
- All color enable
- **LVGL Buffer: Double, DMA, 12.5% size**

| Name                      | Avg. CPU | Avg. FPS | Avg. time (render+flush) |
| ------------------------- | -------- | -------- | ------------------------ |
| All scenes avg.           | 54%      | 19 FPS   | 31 ms (27 + 4)           |
| Empty screen              | 75%      | 19 FPS   | 36 ms (12 + 24)          |
| Moving wallpaper          | 71%      | 16 FPS   | 41 ms (28 + 13)          |
| Single rectangle          | 16%      | 24 FPS   | 1 ms (1 + 0)             |
| Multiple rectangles       | 47%      | 25 FPS   | 18 ms (8 + 10)           |
| Multiple RGB images       | 28%      | 25 FPS   | 13 ms (12 + 1)           |
| Multiple ARGB images      | 39%      | 25 FPS   | 13 ms (12 + 1)           |
| Rotated ARGB images       | 60%      | 24 FPS   | 24 ms (21 + 3)           |
| Multiple labels           | 54%      | 25 FPS   | 18 ms (16 + 4)           |
| Screen sized text         | 85%      | 7 FPS    | 107 ms (103 + 4)         |
| Multiple arcs             | 26%      | 24 FPS   | 4 ms (4 + 0)             |
| Containers                | 35%      | 26 FPS   | 15 ms (15 + 0)           |
| Containers with overlay   | 66%      | 15 FPS   | 42 ms (35 + 7)           |
| Containers with opa       | 46%      | 24 FPS   | 19 ms (19 + 0)           |
| Containers with opa_layer | 61%      | 20 FPS   | 33 ms (33 + 0)           |
| Containers with scrolling | 79%      | 12 FPS   | 57 ms (50 + 7)           |
| Widget demo               | 85%      | 8 FPS    | 73 ms (70 + 3)           | 

---
### String functions source: Standard C
- **CPU Freq: `240Mhz`**
- **`COMPILER_OPTIMIZATION`: -02 performance**
- **`ESP_MAIN_TASK_AFFINITY`: Core 1**
- ***String functions source: LVGL buit'in***
- `LV_DEF_REFR_PERIOD`: 30ms
- `FREERTOS_HZ`: 100
- All color enable
- **LVGL Buffer: Double, DMA, 12.5% size**

| Name                      | Avg. CPU | Avg. FPS | Avg. time (render+flush) |
| ------------------------- | -------- | -------- | ------------------------ |
| All scenes avg.           | 55%      | 21 FPS   | 29 ms (25 + 4)           |
| Empty screen              | 75%      | 19 FPS   | 34 ms (11 + 23)          |
| Moving wallpaper          | 71%      | 16 FPS   | 38 ms (26 + 2)           |
| Single rectangle          | 18%      | 25 FPS   | 2 ms (2 + 0)             |
| Multiple rectangles       | 54%      | 30 FPS   | 17 ms (7 + 10)           |
| Multiple RGB images       | 32%      | 29 FPS   | 8 ms (7 + 1)             |
| Multiple ARGB images      | 48%      | 31 FPS   | 13 ms (13 + 0)           |
| Rotated ARGB images       | 63%      | 24 FPS   | 24 ms (21 + 3)           |
| Multiple labels           | 51%      | 25 FPS   | 18 ms (15 + 3)           |
| Screen sized text         | 82%      | 9 FPS    | 87 ms (83 + 4)           | 
| Multiple arcs             | 24%      | 25 FPS   | 4 ms (4 + 0)             |
| Containers                | 35%      | 26 FPS   | 15 ms (15 + 0)           |
| Containers with overlay   | 68%      | 15 FPS   | 41 ms (34 + 7)           |
| Containers with opa       | 46%      | 24 FPS   | 19 ms (18 + 1)           |
| Containers with opa_layer | 61%      | 19 FPS   | 32 ms (31 + 1)           |
| Containers with scrolling | 78%      | 13 FPS   | 54 ms (47 + 7)           |
| Widget demo               | 85%      | 8 FPS    | 72 ms (68 + 4)           |

---
### Reduce colors ==> Not much change

---
### Increase tick rate to 1000Hz & FAST LVGL Mem
- **CPU Freq: `240Mhz`**
- **`COMPILER_OPTIMIZATION`: -02 performance**
- **`ESP_MAIN_TASK_AFFINITY`: Core 1**
- ***String functions source: LVGL buit'in***
- `LV_DEF_REFR_PERIOD`: 30ms
- ***Fast LVGL memory: `LV_ATTRIBUTE_FAST_MEM_USE_IRAM`***
- ***`FREERTOS_HZ`: 1000***
- ***All color enable***
- **LVGL Buffer: Double, DMA, 12.5% size**

| Name                      | Avg. CPU | Avg. FPS | Avg. time (render+flush) |
| ------------------------- | -------- | -------- | ------------------------ |
| All scenes avg.           | 63%      | 26 FPS   | 26 ms (21 + 5)           |
| Empty screen              | 96%      | 25 FPS   | 34 ms (9 + 25)           |
| Moving wallpaper          | 96%      | 23 FPS   | 38 ms (23 + 15)          |
| Single rectangle          | 15%      | 32 FPS   | 1 ms (1 + 0)             |
| Multiple rectangles       | 63%      | 33 FPS   | 17 ms (7 + 10)           |
| Multiple RGB images       | 32%      | 32 FPS   | 13 ms (12 + 1)           |
| Multiple ARGB images      | 49%      | 32 FPS   | 13 ms (12 + 1)           |
| Rotated ARGB images       | 78%      | 31 FPS   | 22 ms (20 + 2)           |
| Multiple labels           | 58%      | 32 FPS   | 15 ms (13 + 2)           |
| Screen sized text         | 94%      | 11 FPS   | 82 ms (77 + 5)           |
| Multiple arcs             | 26%      | 32 FPS   | 3 ms (3 + 0)             |
| Containers                | 32%      | 32 FPS   | 11 ms (11 + 0)           |
| Containers with overlay   | 94%      | 19 FPS   | 44 ms (34 + 10)          |
| Containers with opa       | 39%      | 32 FPS   | 13 ms (13 + 0)           |
| Containers with opa_layer | 54%      | 31 FPS   | 24 ms (23 + 1)           |
| Containers with scrolling | 94%      | 18 FPS   | 49 ms (41 + 8)           |
| Widget demo               | 97%      | 11 FPS   | 60 ms (56 + 4)           | 

### All setting optimized
- **CPU Freq: `240Mhz`**
- **`COMPILER_OPTIMIZATION`: -02 performance**
- **`ESP_MAIN_TASK_AFFINITY`: Core 1**
- **String functions source: LVGL buit'in**
- **`LV_DEF_REFR_PERIOD`: 10ms**
- **Fast LVGL memory: `LV_ATTRIBUTE_FAST_MEM_USE_IRAM`**
- **`FREERTOS_HZ`: 1000**
- **All color enable**
- **LVGL Buffer: Double, DMA, 12.5% size**

| Name                      | Avg. CPU | Avg. FPS | Avg. time (render+flush) |
| ------------------------- | -------- | -------- | ------------------------ |
| All scenes avg.           | 70%      | 50 FPS   | 25 ms (20 + 5)           |
| Empty screen              | 94%      | 25 FPS   | 33 ms (8 + 25)           |
| Moving wallpaper          | 94%      | 23 FPS   | 37 ms (21 + 16)          |
| Single rectangle          | 20%      | 89 FPS   | 1 ms (1 + 0)             |
| Multiple rectangles       | 84%      | 48 FPS   | 17 ms (5 + 12)           |
| Multiple RGB images       | 45%      | 90 FPS   | 3 ms (3 + 0)             |
| Multiple ARGB images      | 64%      | 80 FPS   | 7 ms (7 + 0)             |
| Rotated ARGB images       | 87%      | 37 FPS   | 23 ms (20 + 3)           |
| Multiple labels           | 93%      | 55 FPS   | 15 ms (13 + 2)           |
| Screen sized text         | 97%      | 11 FPS   | 82 ms (77 + 5)           |
| Multiple arcs             | 33%      | 89 FPS   | 1 ms (1 + 0)             |
| Containers                | 32%      | 79 FPS   | 13 ms (12 + 1)           |
| Containers with overlay   | 87%      | 20 FPS   | 45 ms (35 + 10)          |
| Containers with opa       | 46%      | 74 FPS   | 15 ms (14 + 1)           |
| Containers with opa_layer | 55%      | 57 FPS   | 25 ms (24 + 1)           |
| Containers with scrolling | 94%      | 18 FPS   | 48 ms (39 + 9)           |
| Widget demo               | 97%      | 11 FPS   | 59 ms (55 + 4)           | 
