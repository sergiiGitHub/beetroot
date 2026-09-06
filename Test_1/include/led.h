#ifndef LED_H
#define LED_H

#include "driver/gpio.h"

#define DEFAULT_LED_PIN GPIO_NUM_46

/**
 * @brief Ініціалізує GPIO пін для роботи зі світлодіодом.
 * 
 * @param gpio_num Номер GPIO піна (наприклад, GPIO_NUM_46)
 */
void led_init(gpio_num_t gpio_num);

/**
 * @brief Вмикає світлодіод.
 * 
 * @param gpio_num Номер GPIO піна
 */
void led_on(gpio_num_t gpio_num);

/**
 * @brief Вимикає світлодіод.
 * 
 * @param gpio_num Номер GPIO піна
 */
void led_off(gpio_num_t gpio_num);

/**
 * @brief Перемикає стан світлодіода (з On на Off і навпаки).
 * 
 * @param gpio_num Номер GPIO піна
 */
void led_toggle(gpio_num_t gpio_num);

#endif // LED_H