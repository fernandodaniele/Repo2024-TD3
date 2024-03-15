#include "../include/led.h"
#include "../include/trafficLights.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED_VERDE GPIO_NUM_25
#define LED_AMARILLO GPIO_NUM_26
#define LED_ROJO GPIO_NUM_27

#define T_VERDE		300
#define T_AMARILLO	100
#define T_ROJO		500

void iniciarSemaforo()
{
    configurarLed(LED_ROJO, LED_AMARILLO, LED_VERDE);
}

void actualizarSemaforo()
{
    prenderLed('R');
    vTaskDelay(T_ROJO);
    prenderLed('A');
    vTaskDelay(T_AMARILLO);
    apagarLed('R');
    apagarLed('A');
    prenderLed('V');
    vTaskDelay(T_VERDE);
    apagarLed('V');
    prenderLed('A');
    vTaskDelay(T_AMARILLO);
    apagarLed('A');
}