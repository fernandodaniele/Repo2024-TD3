#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/FreeRTOSConfig.h"
/*==================[definiciones y macros]==================================*/
#define EVIDENCIAR_PROBLEMA 0  //Cambiar esto para probar con o sin sección critica mutex

#if EVIDENCIAR_PROBLEMA
//================= Esto es cuando se imprime mal =================================
/*==================[declaraciones de funciones]====================*/

// Prototipo de funcion de la tarea
void tareaA( void* parametroTarea );
void tareaB( void* parametroTarea );

/*==================[funcion principal]======================================*/

void app_main()
{
    // ---------- CONFIGURACIONES ------------------------------
    BaseType_t res;
    
    res = xTaskCreatePinnedToCore (
              tareaA,                     // Funcion de la tarea a ejecutar
              ( const char * )"Tarea A",   	// Nombre de la tarea como String amigable para el usuario
              configMINIMAL_STACK_SIZE*3, 	// Cantidad de stack de la tarea
              NULL,                          	// Parametros de tarea
              tskIDLE_PRIORITY+1,         	// Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
              NULL,                          		// Puntero a la tarea creada en el sistema
              0
          );

    configASSERT( res == pdPASS ); // gestion de errores

    res = xTaskCreatePinnedToCore (
              tareaB,                     // Funcion de la tarea a ejecutar
              ( const char * )"Tarea B",   	// Nombre de la tarea como String amigable para el usuario
              configMINIMAL_STACK_SIZE*3, 	// Cantidad de stack de la tarea
              NULL,                          	// Parametros de tarea
              tskIDLE_PRIORITY+1,         	// Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
              NULL,                          		// Puntero a la tarea creada en el sistema
              1
          );

    configASSERT( res == pdPASS ); // gestion de errores

}

/*==================[definiciones de funciones]=====================*/

// Implementacion de funcion de la tarea
void tareaA( void* parametroTarea )
{
    while( true )
    {
        printf( "Hola, soy la tarea A " );
        printf( "y estoy ejecuntadome en el procesador numero 0\n" );
        vTaskDelay( 1 / portTICK_RATE_MS );
    }
}

void tareaB( void* parametroTarea )
{
    while( true )
    {
        printf( "Hola, soy la tarea B " );
        printf( "y estoy ejecuntadome en el procesador numero 1\n" );
        vTaskDelay( 1 / portTICK_RATE_MS );
    }
}


#else
#include "freertos/semphr.h"

/*==================[declaraciones de funciones]====================*/
SemaphoreHandle_t mutex;

// Prototipo de funcion de la tarea
void tareaA( void* parametroTarea );
void tareaB( void* parametroTarea );

/*==================[funcion principal]======================================*/

void app_main()
{
    BaseType_t res;

    printf( "Iniciando... " );
    // ---------- CONFIGURACIONES ------------------------------
    mutex = xSemaphoreCreateMutex();
    res = xTaskCreatePinnedToCore (
              tareaA,                     // Funcion de la tarea a ejecutar
              ( const char * )"Tarea A",   	// Nombre de la tarea como String amigable para el usuario
              configMINIMAL_STACK_SIZE*3, 	// Cantidad de stack de la tarea
              NULL,                          	// Parametros de tarea
              tskIDLE_PRIORITY+1,         	// Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
              NULL,                          		// Puntero a la tarea creada en el sistema
              0
          );

    configASSERT( res == pdPASS ); // gestion de errores

    res = xTaskCreatePinnedToCore (
              tareaB,                     // Funcion de la tarea a ejecutar
              ( const char * )"Tarea B",   	// Nombre de la tarea como String amigable para el usuario
              configMINIMAL_STACK_SIZE*3, 	// Cantidad de stack de la tarea
              NULL,                          	// Parametros de tarea
              tskIDLE_PRIORITY+1,         	// Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
              NULL,                          		// Puntero a la tarea creada en el sistema
              1
          );

    configASSERT( res == pdPASS ); // gestion de errores
}

/*==================[definiciones de funciones]=====================*/

// Implementacion de funcion de la tarea
void tareaA( void* parametroTarea )
{
    while( true )
    {
        xSemaphoreTake( mutex , portMAX_DELAY ); //abrir seccion critica
        printf( "Hola, soy la tarea A " );
        printf( "y estoy ejecuntadome en el procesador numero 0\n" );
        xSemaphoreGive( mutex );	//cerrar seccion critica

        vTaskDelay( 1 / portTICK_RATE_MS );
    }
}

void tareaB( void* parametroTarea )
{
    while( true )
    {
        xSemaphoreTake( mutex , portMAX_DELAY ); //abrir seccion critica
        printf( "Hola, soy la tarea B " );
        printf( "y estoy ejecuntadome en el procesador numero 1\n" );
        xSemaphoreGive( mutex );	//cerrar seccion critica

        vTaskDelay( 1 / portTICK_RATE_MS );
    }
}


#endif