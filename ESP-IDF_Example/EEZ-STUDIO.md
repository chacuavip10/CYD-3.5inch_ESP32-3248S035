# Setting EEZ-STUDIO
- Build > Destination folder: `ui`
- lvgl include: `lvgl.h`
- Image must be in `png` format

# Setting ESP-IDF
- Copy `ui` folder from eez-studio project (build) to `main` src file
- Cmakes file in `main`
- Consider using `xTaskCreatePinnedToCore` to pin function contain `lv_timer_handler()` to core 1 (on multicore platform). Remember to increase stack size for complex UI to avoid stack overflow.
```text
file(GLOB_RECURSE MAIN_SRCS *.c *.cpp)
file(GLOB_RECURSE UI_SRCS ui/**/*.c ui/**/*.cpp)  # Changed to include subdirectories

set(srcs ${MAIN_SRCS} ${UI_SRCS})

set(include_dirs
    .
    ui
    )

idf_component_register( SRCS ${srcs}
                      INCLUDE_DIRS ${include_dirs}
                      )
```