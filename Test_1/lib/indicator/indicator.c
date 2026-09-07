#include "indicator.h"
#include "led.h"
#include "esp_log.h"

static const char *TAG = "INDICATOR_MODULE";
static QueueHandle_t s_indicator_queue = NULL;

void Indicator_init(QueueHandle_t xQueue)
{
    // Передаємо QueueHandle_t напряму (це вже вказівник у FreeRTOS!)
    s_indicator_queue = xQueue;

    // Ініціалізуємо світлодіод
    led_init(DEFAULT_LED_PIN);

    ESP_LOGI(TAG, "Indicator initialized successfully");
}

void Indicator_task(void *pvParameters)
{
    uint32_t adc_val = 0;

    while (1) {
        if (s_indicator_queue != NULL) {
            // Блокуємо таску до моменту, поки в черзі не з'явиться нове значення від ADC
            if (xQueueReceive(s_indicator_queue, &adc_val, portMAX_DELAY) == pdTRUE) {
                
                ESP_LOGI(TAG, "Received LDR value: %lu", adc_val);

                // Якщо значення менше порогу -> темно -> вмикаємо LED
                if (adc_val < LIGHT_THRESHOLD_DARK - LIGHT_THRESHOLD_DELTA) {
                    led_on(DEFAULT_LED_PIN);
                    ESP_LOGI(TAG, "State: DARK -> LED ON");
                } else if (adc_val > LIGHT_THRESHOLD_DARK + LIGHT_THRESHOLD_DELTA){
                    led_off(DEFAULT_LED_PIN);
                    ESP_LOGI(TAG, "State: BRIGHT -> LED OFF");
                }
            }
        } else {
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
}