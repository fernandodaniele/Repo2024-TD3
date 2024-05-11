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
//----------------- Inclusiones --------------
#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/FreeRTOSConfig.h"
#include "driver/gpio.h"
#include "pulsador.h"

//---------------- Definiciones -------------------
#define T_ESPERA_MS  40
#define T_ESPERA     pdMS_TO_TICKS(T_ESPERA_MS)
#define PROCESADORA 0
#define PROCESADORB 1

//-------------------- Variables globales ---------------
gpio_int_type_t led [N_PULSADOR] = {GPIO_NUM_25, GPIO_NUM_26, GPIO_NUM_27};

//-------------------- Prototipos ------------------------
void destello( void* taskParmPtr ); //Prototipo de la función de la tarea
void tarea_tecla( void* taskParmPtr );

//-------------------- Main ------------------------------
void app_main()
{
    // Crear tarea en freeRTOS
    // Devuelve pdPASS si la tarea fue creada y agregada a la lista ready
    // En caso contrario devuelve pdFAIL.
    inicializarPulsador();

    for(int i = 0; i< N_PULSADOR; i++)
    {
        BaseType_t res = xTaskCreatePinnedToCore(
                destello,                     	// Funcion de la tarea a ejecutar
                "Destello",   	                // Nombre de la tarea como String amigable para el usuario
                configMINIMAL_STACK_SIZE, 		// Cantidad de stack de la tarea
                i,                          	// Parametros de tarea
                tskIDLE_PRIORITY+i,         	// Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
                NULL,                          		// Puntero a la tarea creada en el sistema
                PROCESADORA
         );
        // Gestion de errores
        if(res == pdFAIL)
        {
            printf( "Error al crear la tarea.\r\n" );
            while(true);					// si no pudo crear la tarea queda en un bucle infinito
        }
    }
}

//------------------ Implementacion de funcion de la tarea
void destello( void* taskParmPtr )
{
    // ---------- Configuraciones ------------------------------
    uint8_t indice = ( uint8_t ) taskParmPtr; //Recibimos el indice led correspondiente a la tarea
    
    gpio_pad_select_gpio(led[indice]);
    gpio_set_direction(led[indice], GPIO_MODE_OUTPUT);

    TickType_t dif;

    // ---------- Bucle infinito --------------------------
    while( true )
    {
        dif = obtenerDiferencia(indice);

        if( dif > TIEMPO_NO_VALIDO )
        {
            gpio_set_level( led[indice], 1 );
            vTaskDelay( dif );
            gpio_set_level( led[indice], 0 );
            borrarDiferencia(indice);
        }
        else
        {
            vTaskDelay( T_ESPERA );
        }
    } 
}