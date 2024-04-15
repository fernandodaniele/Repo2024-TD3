#include "../include/pulsador.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define T_ESPERA 4 //cantidad de veces que va a ingresar antes de cambiar de estado
estadoBoton_t estadoActual;

void inicializarBoton( gpio_int_type_t pulsador )
{
    estadoActual = BAJO;  // Se configura estado inicial
    gpio_set_direction(pulsador, GPIO_MODE_INPUT); //Configura el pin como entrada digital
    gpio_set_pull_mode(pulsador, GPIO_PULLDOWN_ONLY); //Habilita resistencia de PULLDOWN interna
    gpio_set_direction(GPIO_NUM_19, GPIO_MODE_OUTPUT);
}

void errorBoton( void )
{
   //Acá estaría bueno advertir que hubo una falla
   estadoActual = ALTO;
}

//Actualización de la MEF
void actualizarBoton( gpio_int_type_t pulsador )
{
   static uint8_t contDescendente = 0;
   static uint8_t contAscendente = 0;

   switch( estadoActual ){

      case BAJO: 
         if( gpio_get_level(pulsador) ){
            estadoActual = ASCENDENTE;
         }
      break;

      case ASCENDENTE:      
         if( contAscendente >= T_ESPERA ){
            if( gpio_get_level(pulsador) ){
               estadoActual = ALTO;
               botonPresionado();
            } else{
               estadoActual = BAJO;
            }
            contAscendente = 0;
         }
         contAscendente++;
      break;

      case ALTO:
         if( !gpio_get_level(pulsador) ){
            estadoActual = DESCENDENTE;
         }
      break;

      case DESCENDENTE:      
         if( contDescendente >= T_ESPERA ){
            if( !gpio_get_level(pulsador) ){
               estadoActual = BAJO;
               botonLiberado();
            } else{
               estadoActual = ALTO;
            }
            contDescendente = 0;
         }
         contDescendente++;
      break;

      default:
         errorBoton();
      break;
   }
}


void botonPresionado( void )
{
    printf("Boton presionado\n");
    gpio_set_level(GPIO_NUM_19,1);
}

void botonLiberado( void )
{
    printf("Boton liberado\n");
    gpio_set_level(GPIO_NUM_19,0);
}