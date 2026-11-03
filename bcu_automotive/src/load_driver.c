#include "load_driver.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void load_driver_init(void) {
    gpio_config_t cfg = {
        .pin_bit_mask = (1ULL << LOAD_BUZZER_PIN) | (1ULL << LOAD_RELAY_PIN),
        .mode= GPIO_MODE_OUTPUT,
        .pull_up_en= GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type
        = GPIO_INTR_DISABLE,
    };

    gpio_config(&cfg);

    gpio_set_level(LOAD_BUZZER_PIN, 0);

    gpio_set_level(LOAD_RELAY_PIN, 0);
}

void load_relay_set(bool on) {
    gpio_set_level(LOAD_RELAY_PIN, on ? 1 : 0);
    // HIGH-active модуль; інверсія — якщо ваш LOW-active
}

void load_buzzer_set(bool on) {
    gpio_set_level(LOAD_BUZZER_PIN, on ? 1 : 0);
}

void load_buzzer_chirp(uint32_t ms) {
    // Короткий блокуючий сигнал ДОПУСТИМИЙ лише для миттєвого "біп" (10-200 мс)
    // під час підтвердження дії користувача — НЕ використовуємо це у постійному циклі!
    load_buzzer_set(true);
    vTaskDelay(pdMS_TO_TICKS(ms));
    load_buzzer_set(false);
}