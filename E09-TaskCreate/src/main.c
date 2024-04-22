/**
 * @file main.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-05-22
 * 
 * @copyright Copyright (c) 2023
 * 
 */
//-------------------- Inclusiones -------------
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/FreeRTOSConfig.h"
#include "driver/gpio.h" //para driver de E/S digitales

//-------------------- Definiciones -------------
#define PULSADOR    GPIO_NUM_18
#define SALIDA1     GPIO_NUM_25
#define SALIDA2     GPIO_NUM_26
#define DELAY_US  1
#define DELAY_MS  10/portTICK_PERIOD_MS

//-------------------- Prototipos -------------
void TaskA( void * pvParameters ); //Prototipo de la tarea
void TaskB( void * pvParameters ); //Prototipo de la tarea
void TaskMonitor ( void * pvParameters ); //Prototipo de la tarea

//-------------------- Variables, constantes y punteros globales -------------
TaskHandle_t xHandleA = NULL;           //Puntero a la tarea
TaskHandle_t xHandleB = NULL;
TaskHandle_t xHandleMonitor = NULL;

//-------------------- main -------------
void app_main() 
{
    UBaseType_t prioridad = uxTaskPriorityGet (NULL);   //para obtener la prioridad de la tarea principal
    printf("%d/n",prioridad);   //debería ser 1

    xTaskCreatePinnedToCore( TaskA,             //Función que implementa la tarea. La misma no debe retornar.
                 "Tarea A",          //Nombre que reprenta la tarea, para facilitar la depuración.
                 configMINIMAL_STACK_SIZE,  //Tamaño del stack en bytes
                 NULL,                      //Puntero que se utilizará como parámetro para la tarea que se está creando. Como no lo usamos ponemos NULL
                 tskIDLE_PRIORITY+3,          //Prioridad de la tarea
                 &xHandleA,                  //Puntero a la tarea
                 0                          //Procesador donde se ejecuta
                );
    configASSERT( xHandleA );                //entra si no se pudieron crear las tareas, para debug

    xTaskCreatePinnedToCore( TaskB,             //Función que implementa la tarea. La misma no debe retornar.
                 "Tarea B",          //Nombre que reprenta la tarea, para facilitar la depuración.
                 configMINIMAL_STACK_SIZE,  //Tamaño del stack en bytes
                 NULL,                      //Puntero que se utilizará como parámetro para la tarea que se está creando. Como no lo usamos ponemos NULL
                 tskIDLE_PRIORITY+2,          //Prioridad de la tarea
                 &xHandleB ,                 //Puntero a la tarea
                 0
                );
    configASSERT( xHandleB );                //entra si no se pudieron crear las tareas, para debug

    xTaskCreatePinnedToCore( TaskMonitor,             //Función que implementa la tarea. La misma no debe retornar.
                 "Tarea monitoreo",          //Nombre que reprenta la tarea, para facilitar la depuración.
                 configMINIMAL_STACK_SIZE,  //Tamaño del stack en bytes
                 NULL,                      //Puntero que se utilizará como parámetro para la tarea que se está creando. Como no lo usamos ponemos NULL
                 tskIDLE_PRIORITY+1,          //Prioridad de la tarea
                 &xHandleMonitor ,                 //Puntero a la tarea
                 1
                );
    configASSERT( xHandleMonitor );                //entra si no se pudieron crear las tareas, para debug
}

//-------------------- Tareas -------------
void TaskA( void * pvParameters )
{
    gpio_set_direction(PULSADOR, GPIO_MODE_INPUT);
	gpio_set_pull_mode(PULSADOR, GPIO_PULLDOWN_ONLY);
    gpio_set_direction(SALIDA1, GPIO_MODE_OUTPUT);

    while (1)
    {
        if (gpio_get_level(PULSADOR))
        {
            //Borra la tarea usando el handle
            if( xHandleB != NULL )
            {
            vTaskDelete( xHandleB );
            }
        }
        gpio_set_level(SALIDA1, 1);
        //ets_delay_us(DELAY_US);    //delay en microsegundos que bloquea al procesador
        vTaskDelay(DELAY_MS);
        gpio_set_level(SALIDA1, 0);
        //ets_delay_us(DELAY_US);
        vTaskDelay(DELAY_MS);
    }
}

void TaskB( void * pvParameters )
{
    gpio_set_direction(SALIDA2, GPIO_MODE_OUTPUT);

    while (1)
    {
        gpio_set_level(SALIDA2, 1);
        //ets_delay_us(DELAY_US);    //delay en microsegundos que bloquea al procesador
        vTaskDelay(DELAY_MS);
        gpio_set_level(SALIDA2, 0);
        //ets_delay_us(DELAY_US);
        vTaskDelay(DELAY_MS);
    }
}

void TaskMonitor( void * pvParameters )
{
    while(true)
    {
        //Muestra el espacio mínimo libre de stack que ha habido desde que comenzó la tarea.
        //Cuanto menor sea el número devuelto, más cerca está la tarea de desbordar su stack.
        printf( "Task %u min %u bytes\r\n", 2,  uxTaskGetStackHighWaterMark( xHandleB));
        printf( "Task %u min %u bytes\r\n", 1,  uxTaskGetStackHighWaterMark( xHandleA));
        vTaskDelay(DELAY_MS);
    }
}