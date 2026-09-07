#ifndef ADC_H
#define ADC_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/**
 * @brief Ініціалізація АЦП на GPIO 4
 */
void ADC_init(QueueHandle_t xQueue);

/**
 * @brief Задача FreeRTOS для зчитування сирих даних з LDR
 */
void ADC_task(void *pvParameters);

#endif // ADC_H