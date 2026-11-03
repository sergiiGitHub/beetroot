#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "driver/gpio.h"
#ifdef __cplusplus

extern "C" {
    #endif
    // Модуль 2.6: чотири стани FSM-дебаунсу — рівно як у теорії, жодних "магічних чисел"
    typedef enum {
        BTN_STATE_IDLE = 0,
        BTN_STATE_PRESSED,
        BTN_STATE_HELD,
        BTN_STATE_RELEASE,
    } button_state_t;
    // Подія, яку FSM віддає застосунку РІВНО ОДИН РАЗ за натискання
    typedef enum {
        BTN_EVENT_NONE = 0,
        BTN_EVENT_SHORT_PRESS,// відпущено раніше, ніж LONG_PRESS_MS
        BTN_EVENT_LONG_PRESS,// утримано довше LONG_PRESS_MS (подія одразу під час утримання)
    } button_event_t;
    // POD-структура (Plain Old Data) — без конструкторів, без вказівників на купу.
    // Кожен екземпляр важить кілька байтів і живе у статичному масиві, ніякого malloc.
    typedef struct {
        gpio_num_t pin;
        button_state_t state;
        uint32_t state_enter_ms; // мітка часу (millis) входу в поточний стан
        bool long_fired;
    // прапорець "довгий натиск уже віддано" (одноразовість)
    } button_fsm_t;
    // Константи часу — Модуль 2.6, exact values з теорії

    #define BTN_DEBOUNCE_MS50u// стабільність перед PRESSED->HELD і HELD->RELEASE->IDLE
    #define BTN_LONG_PRESS_MS2000u// поріг короткий/довгий натиск
    #define BTN_FSM_TICK_MS10u// період опитування (esp_timer), Крок 7

    void button_fsm_init(button_fsm_t *btn, gpio_num_t pin);

    button_event_t button_fsm_tick(button_fsm_t *btn, uint32_t now_ms);

    #ifdef __cplusplus
}
#endif
