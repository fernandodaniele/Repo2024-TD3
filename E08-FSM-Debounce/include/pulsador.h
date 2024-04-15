#ifndef PULSADOR_H
#define PULSADOR_H

#include "driver/gpio.h"

typedef enum{
   ALTO,
   BAJO,
   DESCENDENTE,
   ASCENDENTE
} estadoBoton_t;

void errorBoton( void );
void inicializarBoton( gpio_int_type_t pulsador );
void actualizarBoton( gpio_int_type_t pulsador );
void botonPresionado( void );
void botonLiberado( void );

#endif