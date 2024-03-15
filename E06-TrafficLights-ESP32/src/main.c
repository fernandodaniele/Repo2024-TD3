#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "../include/trafficLights.h"

void app_main()
{
    iniciarSemaforo();

    while(1)
    {
        actualizarSemaforo();
    }
}