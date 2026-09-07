#ifndef INDICATOR_H
#define INDICATOR_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

// Поріг темряви (0..4095)
#define LIGHT_THRESHOLD_DARK   2047
#define LIGHT_THRESHOLD_DELTA   100

/**
 * @brief Ініціалізація індикатора (LED) та збереження хендла черги.
 */
void Indicator_init(QueueHandle_t xQueue);

/**
 * @brief Задача-споживач (Consumer Task) для керування світлодіодом.
 */
void Indicator_task(void *pvParameters);

#endif // INDICATOR_H