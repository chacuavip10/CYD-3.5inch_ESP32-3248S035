/*
RGB LED on CYD 3.5inch work with invert logic. 1 is off & 0 is on.
*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "driver/ledc.h"

#define PIN_NUM_RGB_RED 4
#define PIN_NUM_RGB_BLUE 16
#define PIN_NUM_RGB_GREEN 17

// LEDC TUIMER
#define LEDC_TIMER LEDC_TIMER_0
#define LEDC_MODE LEDC_LOW_SPEED_MODE
#define LEDC_DUTY_RES LEDC_TIMER_13_BIT
#define LEDC_FREQUENCY (5000)

// CHANNEL
#define LEDC_CHANNEL_RED LEDC_CHANNEL_0
#define LEDC_CHANNEL_GREEN LEDC_CHANNEL_1
#define LEDC_CHANNEL_BLUE LEDC_CHANNEL_2

static const char *TAG = "example";

static void init_rgb_led(void)
{
    ESP_LOGI(TAG, "Init RGB LED using GPIO only");
    gpio_config_t bk_gpio_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << PIN_NUM_RGB_RED) | (1ULL << PIN_NUM_RGB_GREEN) | (1ULL << PIN_NUM_RGB_BLUE)};
    ESP_ERROR_CHECK(gpio_config(&bk_gpio_config));
}

static void turn_on_rgb_led(void)
{
    ESP_LOGI(TAG, "Turn on RGB LED");
    gpio_set_level(PIN_NUM_RGB_RED, 0);
    gpio_set_level(PIN_NUM_RGB_BLUE, 0);
    gpio_set_level(PIN_NUM_RGB_GREEN, 0);
}

static void turn_off_rgb_led(void)
{
    ESP_LOGI(TAG, "Turn off RGB LED");
    gpio_set_level(PIN_NUM_RGB_RED, 1);
    gpio_set_level(PIN_NUM_RGB_BLUE, 1);
    gpio_set_level(PIN_NUM_RGB_GREEN, 1);
}

static void turn_on_red_led(void)
{
    ESP_LOGI(TAG, "Turn on RED LED");
    gpio_set_level(PIN_NUM_RGB_RED, 0);
    gpio_set_level(PIN_NUM_RGB_BLUE, 1);
    gpio_set_level(PIN_NUM_RGB_GREEN, 1);
}

static void turn_on_blue_led(void)
{
    ESP_LOGI(TAG, "Turn on BLUE LED");
    gpio_set_level(PIN_NUM_RGB_RED, 1);
    gpio_set_level(PIN_NUM_RGB_BLUE, 0);
    gpio_set_level(PIN_NUM_RGB_GREEN, 1);
}

static void turn_on_green_led(void)
{
    ESP_LOGI(TAG, "Turn on GREEN LED");
    gpio_set_level(PIN_NUM_RGB_RED, 1);
    gpio_set_level(PIN_NUM_RGB_BLUE, 1);
    gpio_set_level(PIN_NUM_RGB_GREEN, 0);
}

static void ledc_init(void)
{
    ESP_LOGI(TAG, "Init RGB LED using PWM only");

    // 1. Timer
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_MODE,
        .timer_num = LEDC_TIMER,
        .duty_resolution = LEDC_DUTY_RES,
        .freq_hz = LEDC_FREQUENCY, // Tần số PWM
        .clk_cfg = LEDC_AUTO_CLK};
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // 2. RED Channel
    ledc_channel_config_t ledc_channel_red = {
        .speed_mode = LEDC_MODE,
        .channel = LEDC_CHANNEL_RED,
        .timer_sel = LEDC_TIMER,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = PIN_NUM_RGB_RED,
        .duty = 8191, // Off when start
        .hpoint = 0};
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_red));

    // 3. GREEN Channel
    ledc_channel_config_t ledc_channel_green = {
        .speed_mode = LEDC_MODE,
        .channel = LEDC_CHANNEL_GREEN,
        .timer_sel = LEDC_TIMER,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = PIN_NUM_RGB_GREEN,
        .duty = 8191,
        .hpoint = 0};
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_green));

    // 4. BLUE Channel
    ledc_channel_config_t ledc_channel_blue = {
        .speed_mode = LEDC_MODE,
        .channel = LEDC_CHANNEL_BLUE,
        .timer_sel = LEDC_TIMER,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = PIN_NUM_RGB_BLUE,
        .duty = 8191,
        .hpoint = 0};
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_blue));

    // 5. Fade
    ESP_ERROR_CHECK(ledc_fade_func_install(0));
}

void set_rgb_color(uint32_t r, uint32_t g, uint32_t b, int fade_time_ms)
{
    // RED
    ledc_set_fade_with_time(LEDC_MODE, LEDC_CHANNEL_RED, 8191 - r, fade_time_ms);
    ledc_fade_start(LEDC_MODE, LEDC_CHANNEL_RED, LEDC_FADE_NO_WAIT);

    // GREEN
    ledc_set_fade_with_time(LEDC_MODE, LEDC_CHANNEL_GREEN, 8191 - g, fade_time_ms);
    ledc_fade_start(LEDC_MODE, LEDC_CHANNEL_GREEN, LEDC_FADE_NO_WAIT);

    // BLUE
    ledc_set_fade_with_time(LEDC_MODE, LEDC_CHANNEL_BLUE, 8191 - b, fade_time_ms);
    ledc_fade_start(LEDC_MODE, LEDC_CHANNEL_BLUE, LEDC_FADE_NO_WAIT);

    // FADE
    vTaskDelay(pdMS_TO_TICKS(fade_time_ms));
}

void app_main(void)
{

    /* Configure the peripheral according to the LED type */
    init_rgb_led();
    turn_on_rgb_led();
    vTaskDelay(pdMS_TO_TICKS(1000));
    turn_off_rgb_led();
    vTaskDelay(pdMS_TO_TICKS(1000));
    turn_on_red_led();
    vTaskDelay(pdMS_TO_TICKS(1000));
    turn_on_blue_led();
    vTaskDelay(pdMS_TO_TICKS(1000));
    turn_on_green_led();
    vTaskDelay(pdMS_TO_TICKS(1000));

    ledc_init();
    while (1)
    {
        printf("RED\n");
        set_rgb_color(8191, 0, 0, 1000);
        vTaskDelay(pdMS_TO_TICKS(500));

        printf("GREEN\n");
        set_rgb_color(0, 8191, 0, 2000);
        vTaskDelay(pdMS_TO_TICKS(500));

        printf("BLUE\n");
        set_rgb_color(0, 0, 8191, 2000);

        printf("PURPLE (RED + BLUE)\n");
        set_rgb_color(4000, 0, 4000, 3000);
        vTaskDelay(pdMS_TO_TICKS(500));

        printf("YELLOW (RED + GREEN)\n");
        set_rgb_color(4000, 4000, 0, 3000);
        vTaskDelay(pdMS_TO_TICKS(500));

        printf("RGB OFF\n");
        set_rgb_color(0, 0, 0, 3000);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
