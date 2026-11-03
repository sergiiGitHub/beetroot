#include "freertos/FreeRTOS.h"
#include <button_fcm.h>
#include <load_driver.h>


void app_main() {

    load_driver_init();

    while (true)
    {
        load_buzzer_chirp(100);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }


}