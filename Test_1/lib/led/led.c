#include "led.h"

// Внутрішній масив/змінна для відстеження поточного стану піна
static uint8_t s_led_state = 0;

void led_init(gpio_num_t gpio_num)
{
    gpio_reset_pin(gpio_num);
    gpio_set_direction(gpio_num, GPIO_MODE_OUTPUT);
    gpio_set_level(gpio_num, 0);
    s_led_state = 0;
}

void led_on(gpio_num_t gpio_num)
{
    gpio_set_level(gpio_num, 1);
    s_led_state = 1;
}

void led_off(gpio_num_t gpio_num)
{
    gpio_set_level(gpio_num, 0);
    s_led_state = 0;
}

void led_toggle(gpio_num_t gpio_num)
{
    s_led_state = !s_led_state;
    gpio_set_level(gpio_num, s_led_state);
}