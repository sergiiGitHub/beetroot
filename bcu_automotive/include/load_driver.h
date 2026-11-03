#pragma once
#include "driver/gpio.h"
#ifdef __cplusplus

extern "C" {
#endif
#define LOAD_BUZZER_PIN GPIO_NUM_11// база BC547B через 220 Ом
#define LOAD_RELAY_PIN GPIO_NUM_12// IN релейного модуля (драйвер уже на платі)
void load_driver_init(void);
void load_relay_set(bool on);
void load_buzzer_set(bool on);
void load_buzzer_chirp(uint32_t ms);
#ifdef __cplusplus
// короткий "бііп" (замок, підтвердження)

#endif