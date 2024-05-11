/**
 * @file main.c
 * @author your name (you@domain.com)
 * @brief Implementar una tarea que encienda un LED durante 500 ms cada T = 1 seg
 * @version 0.2
 * @date 2024-05-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */
//------------------- Inclusiones -----------------
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/FreeRTOSConfig.h"
#include "driver/gpio.h"

//------------------- Defines -----------------
#define SALIDA1     GPIO_NUM_25
#define SALIDA2     GPIO_NUM_26
#define SALIDA3     GPIO_NUM_27
#define TON_LED     pdMS_TO_TICKS(500) // ( 500 / portTICK_RATE_MS );
#define PERIODO     pdMS_TO_TICKS(1000)
#define PROCESADORA 0 //Para identificar los procesadores, por ejemplo para dividir tareas
#define PROCESADORB 1

void tareaDestelloA( void* taskParmPtr ); //Prototipo de la función de la tarea
void tareaDestelloB( void* taskParmPtr ); //Prototipo de la función de la tarea
void tareaDestelloC( void* taskParmPtr ); //Prototipo de la función de la tarea

void app_main()
{
    // Crear tarea en freeRTOS
    // Devuelve pdPASS si la tarea fue creada y agregada a la lista ready
    // En caso contrario devuelve pdFAIL.
    BaseType_t res = xTaskCreatePinnedToCore(
    	tareaDestelloA,                     	// Funcion de la tarea a ejecutar
        "tareaDestelloA",   	                // Nombre de la tarea como String amigable para el usuario
        configMINIMAL_STACK_SIZE, 		// Cantidad de stack de la tarea
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
    	tareaDestelloB,                     	// Funcion de la tarea a ejecutar
        "tareaDestelloB",   	                // Nombre de la tarea como String amigable para el usuario
        configMINIMAL_STACK_SIZE, 		// Cantidad de stack de la tarea
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
    	tareaDestelloC,                     	// Funcion de la tarea a ejecutar
        "tareaDestelloC",   	                // Nombre de la tarea como String amigable para el usuario
        configMINIMAL_STACK_SIZE, 		// Cantidad de stack de la tarea
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

// Implementacion de funcion de la tarea, que no cumple las especificaciones
void tareaDestelloA( void* taskParmPtr )
{
    // ---------- Congiguraciones ------------------------------
    gpio_set_direction(SALIDA1, GPIO_MODE_OUTPUT);

    // ---------- Bucle infinito --------------------------
    while( true )
    {
        gpio_set_level(SALIDA1, 1);
        vTaskDelay( TON_LED);
        gpio_set_level(SALIDA1, 0);

        ets_delay_us(100000); //delay que representa cualquier otra instrucción que podría hacer esta tarea

        vTaskDelay( TON_LED); //¿Es exacto el período?
    }
}

// Implementacion de funcion de la tarea con la solución correcta a nuestro problema
void tareaDestelloB( void* taskParmPtr )
{
    // ---------- Congiguraciones ------------------------------
    gpio_set_direction(SALIDA2, GPIO_MODE_OUTPUT);

    TickType_t xPeriodicity =  PERIODO;			// Tarea periodica cada 1000 ms

    TickType_t xLastWakeTime = xTaskGetTickCount();

    // ---------- Bucle infinito --------------------------
    while( true )
    {
        gpio_set_level(SALIDA2, 1);
        vTaskDelay( TON_LED);
        gpio_set_level(SALIDA2, 0);

        ets_delay_us(100000); //delay que representa cualquier otra instrucción que podría hacer esta tarea

        // Envia la tarea al estado bloqueado durante xPeriodicity (delay periodico)
        vTaskDelayUntil( &xLastWakeTime , xPeriodicity );
    }
}

// Implementacion de funcion de la tarea, que muestra un caso donde no se cumpliría el período
void tareaDestelloC( void* taskParmPtr )
{
    // ---------- Congiguraciones ------------------------------
    gpio_set_direction(SALIDA3, GPIO_MODE_OUTPUT);

    TickType_t xPeriodicity =  PERIODO;			// Tarea periodica cada 1000 ms

    TickType_t xLastWakeTime = xTaskGetTickCount();

    // ---------- Bucle infinito --------------------------
    while( true )
    {
        gpio_set_level(SALIDA3, 1);
        vTaskDelay( TON_LED);
        gpio_set_level(SALIDA3, 0);

        ets_delay_us(1000000); //delay que representa cualquier otra instrucción que podría hacer esta tarea

        // Envia la tarea al estado bloqueado durante xPeriodicity (delay periodico)
        vTaskDelayUntil( &xLastWakeTime , xPeriodicity );
    }
}