#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "adc.h"
#include "indicator.h"

static const char *TAG = "MAIN";

#define QUEUE_LENGTH 10
typedef uint32_t Counter_size_t;

static TaskHandle_t adcTaskHandle = NULL;
static TaskHandle_t indicatorTaskHandle = NULL;
static QueueHandle_t xMessageQueue = NULL;

void app_main(void)
{
    // 1. Створюємо чергу FreeRTOS
    xMessageQueue = xQueueCreate(QUEUE_LENGTH, sizeof(Counter_size_t));
    if (xMessageQueue == NULL) {
        ESP_LOGE(TAG, "Failed to create Queue!");
        return;
    }
    // Ініціалізуємо АЦП
    ADC_init(xMessageQueue);
    Indicator_init(xMessageQueue);

    // Запускаємо таску для зчитування сирих даних
    BaseType_t res = xTaskCreate(
        ADC_task, 
        "ADC_Task", 
        4096, 
        NULL, 
        5, 
        &adcTaskHandle
    );
    xTaskCreate(Indicator_task, "Indicator_Task", 4096, NULL, 5, &indicatorTaskHandle);

    if (res != pdPASS) {
        ESP_LOGE(TAG, "Failed to create ADC_Task!");
    } else {
        ESP_LOGI(TAG, "ADC_Task started successfully!");
    }
}