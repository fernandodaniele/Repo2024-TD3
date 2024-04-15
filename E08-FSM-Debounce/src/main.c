#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "../include/pulsador.h"

#define T		10/portTICK_PERIOD_MS
#define PULSADOR    GPIO_NUM_18

void app_main()
{
    inicializarBoton(PULSADOR);
    while(true){
        actualizarBoton(PULSADOR);
        vTaskDelay(T);
    }
}
