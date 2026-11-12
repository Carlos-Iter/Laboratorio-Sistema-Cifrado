/**
 * @file    sistema.c
 * @brief   Inicialización común del microcontrolador y rutina de interrupción.
 * 
 * @author  Carlos Iter
 * @date    Noviembre 2026
 */
#include "config.h"
#include "teclado.h"
#include "sistema.h"

void sistema_init(void)
{
    OSCCON = 0x71;          /* IRCF = 111 -> 8 MHz, SCS = 1 (oscilador interno) */
    ANSEL  = 0x00;          /* Sin canales analógicos en PORTA/PORTE */
    ANSELH = 0x00;          /* Sin canales analógicos en PORTB       */
    TRISA = 0xFF;
    TRISC = 0xFF;
}

void sistema_habilitar_interrupciones(void)
{
    INTCONbits.GIE = 1;
}

/**
 * @brief ISR única del programa. Solo atiende el cambio en PORTB (teclado).
 */
void __interrupt() isr(void)
{
    if (INTCONbits.RBIE && INTCONbits.RBIF) {
        teclado_isr();
    }
}
