#include "adc.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_adc/adc_oneshot.h"

static const char *TAG = "ADC_MODULE";

static adc_oneshot_unit_handle_t s_adc_handle = NULL;
static QueueHandle_t s_adc_queue = NULL;

#define ADC_UNIT_ID      ADC_UNIT_1
#define ADC_CHANNEL_ID   ADC_CHANNEL_3 // GPIO 4

void ADC_init(QueueHandle_t xQueue)
{
    s_adc_queue = xQueue;

    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT_ID,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config, &s_adc_handle));

    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = ADC_ATTEN_DB_12,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(s_adc_handle, ADC_CHANNEL_ID, &config));

    ESP_LOGI(TAG, "ADC initialized on GPIO 4");
}

void ADC_task(void *pvParameters)
{
    int raw_val = 0;

    while (1) {
        if (s_adc_handle != NULL) {
            if (adc_oneshot_read(s_adc_handle, ADC_CHANNEL_ID, &raw_val) == ESP_OK) {
                
                // Надсилаємо значення у чергу
                if (s_adc_queue != NULL) {
                    uint32_t msg_data = (uint32_t)raw_val;
                    xQueueSend(s_adc_queue, &msg_data, pdMS_TO_TICKS(100));
                }
            }
        }

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}