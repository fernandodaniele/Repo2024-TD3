#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/FreeRTOSConfig.h"
#include "driver/gpio.h"
#include "pulsador.h"

#define PERIODO_MS  5000
#define PERIODO     pdMS_TO_TICKS(PERIODO_MS)
#define PROCESADORA 0
#define PROCESADORB 1

gpio_int_type_t led [N_PULSADOR] = {GPIO_NUM_16, GPIO_NUM_17, GPIO_NUM_18};

extern pulsadorInfo pulsador [N_PULSADOR];

void destello( void* taskParmPtr ); //Prototipo de la función de la tarea
void tarea_tecla( void* taskParmPtr );

void app_main()
{
    // Crear tarea en freeRTOS
    // Devuelve pdPASS si la tarea fue creada y agregada a la lista ready
    // En caso contrario devuelve pdFAIL.
    inicializarPulsador();

    for(uint8_t i = 0; i< N_PULSADOR; i++)
    {
        BaseType_t res = xTaskCreatePinnedToCore(
                destello,                     	// Funcion de la tarea a ejecutar
                "Destello",   	                // Nombre de la tarea como String amigable para el usuario
                configMINIMAL_STACK_SIZE*3, 		// Cantidad de stack de la tarea
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

// Implementacion de funcion de la tarea
void destello( void* taskParmPtr )
{
    // ---------- Configuraciones ------------------------------
    uint8_t indice = ( uint8_t ) taskParmPtr;
    
    TickType_t xPeriodicity = PERIODO; // Tarea periodica cada 1000 ms
    TickType_t xLastWakeTime = xTaskGetTickCount();

    gpio_set_direction(led[indice], GPIO_MODE_OUTPUT);

    uint8_t contador;
    // ---------- Bucle infinito --------------------------
    while( true )
    {
        contador = ( uint8_t )uxSemaphoreGetCount( pulsador[indice].semaforo );
        printf( "Tarea %d. Quedan %d semaforos\n",indice, contador );
        xSemaphoreTake( pulsador[indice].semaforo, portMAX_DELAY );

        xLastWakeTime = xTaskGetTickCount();
        gpio_set_level( led[indice], 1 );
        vTaskDelay( xPeriodicity/2 );
        gpio_set_level( led[indice], 0 );

        vTaskDelayUntil( &xLastWakeTime, xPeriodicity );
    }
}