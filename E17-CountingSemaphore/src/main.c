#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/FreeRTOSConfig.h"
#include "driver/gpio.h"
#include "pulsador.h"

#define N_LED     3
#define T_LED_MS  1000
#define T_LED     pdMS_TO_TICKS(T_LED_MS)
#define T_ESPERA_MS  40
#define T_ESPERA     pdMS_TO_TICKS(T_ESPERA_MS)
#define PROCESADORA 0
#define PROCESADORB 1

gpio_int_type_t led [N_LED] = {GPIO_NUM_25, GPIO_NUM_26, GPIO_NUM_27};

extern pulsadorInfo pulsador [N_PULSADOR];

void destello( void* taskParmPtr ); //Prototipo de la función de la tarea
void tarea_tecla( void* taskParmPtr );

void app_main()
{
    // Crear tarea en freeRTOS
    // Devuelve pdPASS si la tarea fue creada y agregada a la lista ready
    // En caso contrario devuelve pdFAIL.
    inicializarPulsador();

    for(uint8_t i = 0; i< N_LED; i++)
    {
        BaseType_t res = xTaskCreatePinnedToCore(
                destello,                     	// Funcion de la tarea a ejecutar
                "Destello",   	                // Nombre de la tarea como String amigable para el usuario
                configMINIMAL_STACK_SIZE, 		// Cantidad de stack de la tarea
                i,                          	// Parametros de tarea
                tskIDLE_PRIORITY+1,         	// Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
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
    
    gpio_pad_select_gpio(led[indice]);
    gpio_set_direction(led[indice], GPIO_MODE_OUTPUT);


    // ---------- Bucle infinito --------------------------
    while( true )
    {
        xSemaphoreTake( pulsador[0].semaforo, portMAX_DELAY );

        gpio_set_level( led[indice], 1 );
        vTaskDelay( T_LED );
        gpio_set_level( led[indice], 0 );

    }
}