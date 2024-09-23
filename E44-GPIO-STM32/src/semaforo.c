#include "../include/led.h"
#include "../include/semaforo.h"

#define LED_VERDE 		LD4_Pin     //número de pin
#define LED_AMARILLO	LD3_Pin
#define LED_ROJO 		LD5_Pin

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
    HAL_Delay(T_ROJO);
    prenderLed('A');
    HAL_Delay(T_AMARILLO);
    apagarLed('R');
    apagarLed('A');
    prenderLed('V');
    HAL_Delay(T_VERDE);
    apagarLed('V');
    prenderLed('A');
    HAL_Delay(T_AMARILLO);
    apagarLed('A');
}
