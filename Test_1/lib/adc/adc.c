#include "adc.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_adc/adc_oneshot.h"

static const char *TAG = "ADC_RAW";

static adc_oneshot_unit_handle_t s_adc_handle = NULL;

// GPIO 4 відповідає ADC1 Channel 3 на ESP32-S3
#define ADC_UNIT_ID      ADC_UNIT_1
#define ADC_CHANNEL_ID   ADC_CHANNEL_3 

void ADC_init(void)
{
    // 1. Конфігурація юніта ADC1
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT_ID,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config, &s_adc_handle));

    // 2. Конфігурація каналу (GPIO 4)
    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT, // 12 біт (0 - 4095)
        .atten = ADC_ATTEN_DB_12,         // Діапазон напруги 0..3.3V
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(s_adc_handle, ADC_CHANNEL_ID, &config));

    ESP_LOGI(TAG, "ADC initialized on GPIO 4 (ADC1 Channel 3)");
}

void ADC_task(void *pvParameters)
{
    int raw_val = 0;

    while (1) {
        if (s_adc_handle != NULL) {
            // Зчитуємо сире значення з фоторезистора
            esp_err_t ret = adc_oneshot_read(s_adc_handle, ADC_CHANNEL_ID, &raw_val);
            
            if (ret == ESP_OK) {
                ESP_LOGI(TAG, "LDR Raw Value: %d", raw_val);
            } else {
                ESP_LOGE(TAG, "Failed to read ADC, err: 0x%x", ret);
            }
        }

        // Затримка 500 мс між замірами
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}