#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/FreeRTOSConfig.h"
#include "driver/gpio.h"
#include "pulsador.h"

#define SALIDA1     GPIO_NUM_25
#define T_ESPERA_MS  40
#define T_ESPERA     pdMS_TO_TICKS(T_ESPERA_MS)
#define PROCESADORA 0
#define PROCESADORB 1

gpio_int_type_t led [N_PULSADOR] = {GPIO_NUM_25, GPIO_NUM_26, GPIO_NUM_27};

extern pulsadorInfo pulsador [N_PULSADOR];

void tareaDestello( void* taskParmPtr ); //Prototipo de la función de la tarea

void app_main()
{
    // Crear tarea en freeRTOS
    // Devuelve pdPASS si la tarea fue creada y agregada a la lista ready
    // En caso contrario devuelve pdFAIL.
    inicializarPulsador();

    for(uint8_t i = 0; i< N_PULSADOR; i++)
    {
        BaseType_t res = xTaskCreatePinnedToCore(
                tareaDestello,                     	// Funcion de la tarea a ejecutar
                "tareaDestello",   	                // Nombre de la tarea como String amigable para el usuario
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

// Implementacion de funcion de la tarea
void tareaDestello( void* taskParmPtr )
{
    // ---------- Configuraciones ------------------------------
    uint8_t indice = ( uint8_t ) taskParmPtr;
    
    gpio_pad_select_gpio(led[indice]);
    gpio_set_direction(led[indice], GPIO_MODE_OUTPUT);

    TickType_t dif;

    // ---------- Bucle infinito --------------------------
    while( true )
    {
        xSemaphoreTake( pulsador[indice].semaforo, portMAX_DELAY );

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