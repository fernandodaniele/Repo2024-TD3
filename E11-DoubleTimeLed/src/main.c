/**
 * @file main.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-05-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */
//------- Inclusiones --------------
#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/FreeRTOSConfig.h"
#include "driver/gpio.h"
#include <math.h>

//-------- Constantes ---------------
#define N_LED       3
#define TON_LED     pdMS_TO_TICKS(500) // ( 500 / portTICK_RATE_MS );
#define PERIODO     pdMS_TO_TICKS(1000)
#define PROCESADORA 0
#define PROCESADORB 1

//-------- Tipos de datos ----------------
typedef struct
{
    gpio_int_type_t pin;
    TickType_t tiempoOn;		//tiempo Duty o ciclo de trabajo
    TickType_t periodo;
} led_t;

//--------- Variables globales ------------
gpio_int_type_t pines[N_LED]  = {GPIO_NUM_25, GPIO_NUM_26, GPIO_NUM_27};
led_t leds [N_LED];
uint32_t tiempoLeds [N_LED];

//--------- Prototipos de funciones -----------
void tareaDestello( void* taskParmPtr ); //Prototipo de la función de la tarea

//--------------- Main ---------------------------
void app_main()
{
    // Crear tarea en freeRTOS
    // Devuelve pdPASS si la tarea fue creada y agregada a la lista ready
    // En caso contrario devuelve pdFAIL.
    for(int i=0; i<N_LED; i++)
    {
        leds[i].pin = pines[i];
        leds[i].tiempoOn = TON_LED * pow(2,i); //i=0 -> 1; i=1 -> 2; i=4;
        leds[i].periodo = PERIODO * pow(2,i);

        BaseType_t res = xTaskCreate(
            tareaDestello,                     	// Funcion de la tarea a ejecutar
            "tareaDestello",   	                // Nombre de la tarea como String amigable para el usuario
            configMINIMAL_STACK_SIZE, 		// Cantidad de stack de la tarea
            &leds[i],                          	// Parametros de tarea
            tskIDLE_PRIORITY+i,         	// Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
            NULL                          		// Puntero a la tarea creada en el sistema
        );
        // Gestion de errores
        if(res == pdFAIL)
        {
            printf( "Error al crear la tarea.\r\n" );
            while(true);					// si no pudo crear la tarea queda en un bucle infinito
        }
    }
}

//------------- Tareas ----------------------

// Implementacion de funcion de la tarea
void tareaDestello( void* taskParmPtr )
{
    // ---------- Configuraciones ------------------------------
    led_t* led = (led_t*) taskParmPtr;
    gpio_set_direction(led->pin, GPIO_MODE_OUTPUT);

    TickType_t xLastWakeTime = xTaskGetTickCount();

    // ---------- Bucle infinito --------------------------
    while( true )
    {
        gpio_set_level(led->pin, 1);
        vTaskDelay( led->tiempoOn);
        gpio_set_level(led->pin, 0);
        // Envia la tarea al estado bloqueado durante xPeriodicity (delay periodico)
        vTaskDelayUntil( &xLastWakeTime , led->periodo );
    }
}