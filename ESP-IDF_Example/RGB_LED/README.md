### NOTE
* RGB on CYD work with inverted logic, so 1 is off and 0 is on

### Hardware Required

* CYD 3.5inch
* Pinout: RGB (R: 4, G: 17, B: 16)

### Build and Flash

Run `idf.py -p PORT flash monitor` to build, flash and monitor the project.

(To exit the serial monitor, type ``Ctrl-]``.)

See the [Getting Started Guide](https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html) for full steps to configure and use ESP-IDF to build projects.

## Example Output

```text
I (265) main_task: Started on CPU0
I (275) main_task: Calling app_main()
I (275) example: Init RGB LED using GPIO only
I (275) example: Turn on RGB LED
I (1275) example: Turn off RGB LED
I (2275) example: Turn on RED LED
I (3275) example: Turn on BLUE LED
I (4275) example: Turn on GREEN LED
I (5275) example: Init RGB LED using PWM only
RED
GREEN
BLUE
PURPLE (RED + BLUE)
YELLOW (RED + GREEN)
RGB OFF
```

