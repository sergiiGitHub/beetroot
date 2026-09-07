/*
 * Author: Oleksii Leznovskyi
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "adc.h"

static const char *TAG = "MAIN";

static TaskHandle_t adcTaskHandle = NULL;

void app_main(void)
{
    // Ініціалізуємо АЦП
    ADC_init();

    // Запускаємо таску для зчитування сирих даних
    BaseType_t res = xTaskCreate(
        ADC_task, 
        "ADC_Task", 
        4096, 
        NULL, 
        5, 
        &adcTaskHandle
    );

    if (res != pdPASS) {
        ESP_LOGE(TAG, "Failed to create ADC_Task!");
    } else {
        ESP_LOGI(TAG, "ADC_Task started successfully!");
    }
}