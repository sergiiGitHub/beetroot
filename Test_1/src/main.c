#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#include "led.h"

void app_main(void)
{
    // Ініціалізуємо світлодіод на GPIO 46
    led_init(DEFAULT_LED_PIN);

    while (1) {
        //led_toggle(DEFAULT_LED_PIN);
        led_on(DEFAULT_LED_PIN);
        vTaskDelay(pdMS_TO_TICKS(10)); // Затримка 1 секунда

        led_off(DEFAULT_LED_PIN);
        vTaskDelay(pdMS_TO_TICKS(20)); 
    }
}