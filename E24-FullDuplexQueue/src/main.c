//=================== INCLUSIONES =====================
#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/FreeRTOSConfig.h"
#include "driver/gpio.h"
#include "freertos/semphr.h"

//=================== DEFINICIONES =====================
#define PROCESADORA 0
#define PROCESADORB 1
#define LARGO_MENSAJE 20
#define N_COLA 	5
#define MAX_RATE_MS 5000
#define MAX_RATE pdMS_TO_TICKS(MAX_RATE_MS)

//=================== VARIABLES  =====================
QueueHandle_t colaAB;
QueueHandle_t colaBA;
SemaphoreHandle_t mutex;

//=================== PROTOTIPO DE FUNCIONES =====================
void tareaA( void* parametroTarea );
void tareaB( void* parametroTarea );

//=================== FUNCIÓN PRINCIPAL =====================
void app_main()
{
    colaAB = xQueueCreate( N_COLA , sizeof(char[LARGO_MENSAJE]) );
    // Gestion de errores de colas
	if( colaAB == NULL)
	{
		printf( "Error al crear la cola.\r\n" );
		while(true);					
	}
	colaBA = xQueueCreate( N_COLA , sizeof(char[LARGO_MENSAJE]) );
    // Gestion de errores de colas
	if( colaBA  == NULL)
	{
		printf( "Error al crear la cola.\r\n" );
		while(true);					
	}

	mutex =  xSemaphoreCreateMutex();
	// Gestion de errores de semaforos
	if( mutex == NULL)
	{
		printf( "Error al crear el mutex.\r\n" );
		while(true);						
	}

    // Crear tarea en freeRTOS
    // Devuelve pdPASS si la tarea fue creada y agregada a la lista ready
    // En caso contrario devuelve pdFAIL.
    BaseType_t res = xTaskCreatePinnedToCore(
            tareaA,                     	// Funcion de la tarea a ejecutar
            "tareaA",   	                // Nombre de la tarea como String amigable para el usuario
            configMINIMAL_STACK_SIZE*3, 		// Cantidad de stack de la tarea
            NULL,                          	// Parametros de tarea
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

    res = xTaskCreatePinnedToCore(
            tareaB,                     	// Funcion de la tarea a ejecutar
            "tareaB",   	                // Nombre de la tarea como String amigable para el usuario
            configMINIMAL_STACK_SIZE*3, 		// Cantidad de stack de la tarea
            NULL,                          	// Parametros de tarea
            tskIDLE_PRIORITY+1,         	// Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
            NULL,                          		// Puntero a la tarea creada en el sistema
            PROCESADORB
        );
    // Gestion de errores
    if(res == pdFAIL)
    {
        printf( "Error al crear la tarea.\r\n" );
        while(true);					// si no pudo crear la tarea queda en un bucle infinito
    }
}

//=================== IMPLEMENTACIONES DE FUNCIONES =====================

void tareaA( void* parametroTarea )
{
    // ---------- CONFIGURACIONES ------------------------------
	TickType_t xPeriodicity =  MAX_RATE;
	TickType_t xLastWakeTime = xTaskGetTickCount();

	char mensaje[LARGO_MENSAJE];
	xQueueSend( colaAB , "Comenzamos!" ,  portMAX_DELAY );
    
    // ---------- BUCLE INFINITO ------------------------------
	while( true )
	{
		xQueueReceive( colaBA , &mensaje,  portMAX_DELAY );

		xSemaphoreTake( mutex , portMAX_DELAY );			//abrir seccion critica
		printf("A] Tarea B dijo: %s\r\n",mensaje);
		xSemaphoreGive( mutex );							//cerrar seccion critica

		vTaskDelay( MAX_RATE / 2 );
		xQueueSend( colaAB , "Hola, soy A" ,  portMAX_DELAY );

		vTaskDelayUntil( &xLastWakeTime , xPeriodicity );
	}
}

void tareaB( void* parametroTarea )
{
    // ---------- CONFIGURACIONES ------------------------------
	TickType_t xPeriodicity =  MAX_RATE;
	TickType_t xLastWakeTime = xTaskGetTickCount();
	char mensaje[LARGO_MENSAJE];

    // ---------- BUCLE INFINITO ------------------------------
	while( true )
	{
		xQueueReceive( colaAB , &mensaje,  portMAX_DELAY );

		xSemaphoreTake( mutex , portMAX_DELAY );			//abrir seccion critica
		printf("B] Tarea A dijo: %s\r\n",mensaje);
		xSemaphoreGive( mutex );							//cerrar seccion critica

		vTaskDelay( MAX_RATE / 2 );
		xQueueSend( colaBA , "Hola, soy B" ,  portMAX_DELAY );

		vTaskDelayUntil( &xLastWakeTime , xPeriodicity );
	}
}
