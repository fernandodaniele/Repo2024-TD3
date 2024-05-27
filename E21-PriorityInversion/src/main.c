/*==================[Inclusiones]====================================*/
#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/FreeRTOSConfig.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"

/*==================[Definiciones y macros]==========================*/

#define CUENTAS_1MS         (9251*2)
#define T_LED               10000 / portTICK_RATE_MS

// En 1 usa semaforos en 0 usa mutex
#define EVIDENCIAR_PROBLEMA     0

#if EVIDENCIAR_PROBLEMA==1
#define CRITICAL_CONFIG     mutex = xSemaphoreCreateBinary()
#else
#define CRITICAL_CONFIG     mutex = xSemaphoreCreateMutex()
#endif

#define CRITICAL_DECLARE    SemaphoreHandle_t mutex
#define CRITICAL_START      xSemaphoreTake( mutex , portMAX_DELAY )
#define CRITICAL_END        xSemaphoreGive( mutex )

/*==================[Definiciones de datos ]=========================*/

gpio_int_type_t led[2] = { GPIO_NUM_25, GPIO_NUM_26 };

CRITICAL_DECLARE;

/*==================[Declaraciones de funciones ]====================*/
TaskHandle_t task_handles_a;
TaskHandle_t task_handles_b;
TaskHandle_t task_handles_c;
TaskHandle_t task_handles_d;

void tareaA( void*  );
void tareaD( void*  );
void tareaBC( void*  );

/*=====================[Funcion principal]===========================*/

void app_main()
{
    CRITICAL_CONFIG;
#if EVIDENCIAR_PROBLEMA==1
    // En este caso, lo libero para poder user al semaforo binario como control de seccion critica.
    xSemaphoreGive( mutex );
#endif

    //Creación de tareas
    BaseType_t res;

    res = xTaskCreatePinnedToCore (
              tareaA,
              "tarea_a",
              configMINIMAL_STACK_SIZE*2,
              "tarea  a",                               /* Parametros de tarea */
              tskIDLE_PRIORITY+1,                              /* minima prioridad del sistema */
              &task_handles_a,                           /* handle de la tarea */
              1
          );
    
    configASSERT( res == pdPASS );

    res = xTaskCreatePinnedToCore (
              tareaBC,
              ( const char * )"tarea_b",
              configMINIMAL_STACK_SIZE,
              &led[0],                               // Parametros de tarea 
              tskIDLE_PRIORITY + 2,                  // prioridad media del sistema 
              &task_handles_b,                       // handle de la tarea 
              1
          );

    
    configASSERT( res == pdPASS );

    res = xTaskCreatePinnedToCore (
              tareaBC,
              ( const char * ) "tarea_c",
              configMINIMAL_STACK_SIZE,
              &led[1],                               // Parametros de tarea
              tskIDLE_PRIORITY + 3,                  // prioridad media del sistema
              &task_handles_c,                       // handle de la tarea 
              1
          );

    configASSERT( res == pdPASS );
    
    res = xTaskCreatePinnedToCore (
            tareaD,
            ( const char * )"tarea_d",
            configMINIMAL_STACK_SIZE*2,
            "tarea  d",                               // Parametros de tarea 
            tskIDLE_PRIORITY + 4,                              // prioridad alta del sistema 
            &task_handles_d,                           // handle de la tarea 
            1
          );

    configASSERT( res == pdPASS );

    while(true);
}

/*==================[Definiciones de funciones ]=====================*/

void tareaBC( void* parametroTarea )
{
    gpio_int_type_t * led = ( gpio_int_type_t* ) parametroTarea;

    gpio_pad_select_gpio(*led);
    gpio_set_direction(*led, GPIO_MODE_OUTPUT);

    // Bloqueo de la tarea, para que la tarea A tome el CPU
    vTaskDelay( 100 / portTICK_RATE_MS );

    ets_delay_us(100000);   //Delay bloqueante para que no ceda el CPU y asi representar una tarea "larga"
    
    while(true)
    {
        gpio_set_level(*led,1);
        vTaskDelay( T_LED );
        gpio_set_level(*led,0);
    }
}

void funcionTareaAD( void* parametroTarea )
{
    char* texto = ( char* ) parametroTarea;

    TickType_t tiempo = xTaskGetTickCount();

    CRITICAL_START;

    printf( "Hola ! Soy %s y tarde %u ms en acceder al recurso\n", texto, xTaskGetTickCount()-tiempo );

    ets_delay_us(100000); // delay bloqueante que simula un procesamiento de un cierto tiempo

    CRITICAL_END;

    printf( "Chau ! Soy %s y tarde en ejecutarme %u ms\n", texto, xTaskGetTickCount()-tiempo );

}

void tareaA( void* parametroTarea )
{
    funcionTareaAD( parametroTarea );

    while(true)
    {
        vTaskDelay( 10000 / portTICK_RATE_MS );
    }
}

void tareaD( void* parametroTarea )
{
    // Bloqueo de la tarea para que la tarea A tome el CPU
    vTaskDelay( 100 / portTICK_RATE_MS );

    funcionTareaAD( parametroTarea );

    while(true)
    {
        vTaskDelay( 10000 / portTICK_RATE_MS );
    }
}