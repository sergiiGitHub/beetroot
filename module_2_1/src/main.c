#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"

static const char *TAG = "LED_CONTROLLER";

// 1. Стани та режими роботи
typedef enum {
    BLINK,
    ALWAYS_ON,
    ALWAYS_OFF
} OperatingMode;

// 2. Змінюємо піни на зручні зовнішні піни (наприклад, 4 для LED, 5 для кнопки)
#define LED_PIN             GPIO_NUM_4
#define BUTTON_PIN          GPIO_NUM_5
#define BLINK_TICKS         pdMS_TO_TICKS(150)
#define DEBOUNCE_DELAY_MS   200

// Глобальна змінна для переривання
static volatile bool buttonPressedFlag = false;

// ISR обробник переривання для кнопки (в швидкій пам'яті IRAM)
static void IRAM_ATTR button_isr_handler(void* arg) {
    buttonPressedFlag = true;
}

// Ініціалізація LED
static void led_init(gpio_num_t pin) {
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << pin),
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE
    };
    gpio_config(&io_conf);
}

// Керування станом LED
static void led_set(gpio_num_t pin, bool state) {
    gpio_set_level(pin, state ? 1 : 0);
}

void app_main(void) {
    ESP_LOGI(TAG, "Initializing application with custom pins (LED: 4, Button: 5)...");

    // Ініціалізація світлодіода на GPIO 4
    led_init(LED_PIN);

    // Конфігурація піну кнопки на GPIO 5 з підтяжкою до живлення (PULLUP) та перериванням по спадаючому фронту
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_NEGEDGE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = (1ULL << BUTTON_PIN),
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_ENABLE
    };
    gpio_config(&io_conf);

    // Встановлення служби обробки переривань та реєстрація обробника для BUTTON_PIN
    gpio_install_isr_service(0);
    gpio_isr_handler_add(BUTTON_PIN, button_isr_handler, NULL);

    OperatingMode currentMode = BLINK;
    TickType_t lastDebounceTick = 0;
    bool blinkState = false;
    TickType_t previousBlinkTick = xTaskGetTickCount();

    ESP_LOGI(TAG, "System Ready. Current Mode: Fast Blink (150ms)");

    while (true) {
        TickType_t currentTick = xTaskGetTickCount();

        // Обробка натискання кнопки з урахуванням антидребезгу
        if (buttonPressedFlag) {
            if ((currentTick - lastDebounceTick) > pdMS_TO_TICKS(DEBOUNCE_DELAY_MS)) {
                lastDebounceTick = currentTick;
                buttonPressedFlag = false;

                // Циклічна зміна режимів
                switch (currentMode) {
                    case BLINK:
                        currentMode = ALWAYS_ON;
                        ESP_LOGI(TAG, "Mode changed: Always ON");
                        break;
                    case ALWAYS_ON:
                        currentMode = ALWAYS_OFF;
                        ESP_LOGI(TAG, "Mode changed: Always OFF");
                        break;
                    case ALWAYS_OFF:
                        currentMode = BLINK;
                        ESP_LOGI(TAG, "Mode changed: Fast Blink (150ms)");
                        break;
                }
            } else {
                buttonPressedFlag = false; 
            }
        }

        switch (currentMode) {
            case BLINK:
                if ((currentTick - previousBlinkTick) >= BLINK_TICKS) {
                    previousBlinkTick = currentTick;
                    blinkState = !blinkState;
                    led_set(LED_PIN, blinkState);
                }
                break;

            case ALWAYS_ON:
                led_set(LED_PIN, true);
                break;

            case ALWAYS_OFF:
                led_set(LED_PIN, false);
                break;
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}