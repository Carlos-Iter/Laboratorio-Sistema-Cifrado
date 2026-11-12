/**
 * @file    teclado.c
 * @brief   Implementación del teclado 4x4 con interrupción por cambio (IOC).
 *
 * @details
 *  Reposo: filas RB0..RB3 en 0 y columnas RB4..RB7 en 1 (pull-up).
 *  Al pulsar una tecla, una columna cae a 0 -> se genera RBIF.
 *  En la ISR: barrido inmediato fila por fila para saber cuál tecla fue,
 *  anti-rebote de 20 ms después de capturar, se restauran las filas a 0 y se lee PORTB para
 *  "desbloquear" el puerto (terminar la condición de mismatch) antes de
 *  limpiar RBIF. Si no se lee PORTB, RBIF se vuelve a activar sin fin.
 * 
 * @author  Carlos Iter
 * @date    Noviembre 2026
 */
#include "config.h"
#include "pines.h"
#include "teclado.h"

/** @brief Mapa físico: [fila][columna] */
static const char MAPA[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

static volatile char    tecla_buffer = 0;  /**< Última tecla detectada          */
static volatile uint8_t tecla_lista  = 0;  /**< 1 = hay tecla sin leer         */
static volatile uint8_t tecla_presionada = 0; /**< Evita repetir mientras se mantiene */

/**
 * @brief Barrido: activa una fila a la vez en 0 y lee columnas.
 * @return Carácter de la tecla o 0 si no hay ninguna.
 */
static char teclado_scan(void)
{
    for (uint8_t fila = 0; fila < 4; fila++) {
        /* Solo la fila actual en 0, las demás en 1 */
        TEC_PUERTO = (uint8_t)(TEC_MASK_FILAS & ~(1u << fila));
        __delay_us(30);
        uint8_t cols = TEC_PUERTO & TEC_MASK_COLS;
        for (uint8_t col = 0; col < 4; col++) {
            if (!(cols & (0x10u << col))) {
                return MAPA[fila][col];
            }
        }
    }
    return 0;
}

void teclado_init(void)
{
    ANSEL  = 0x00;                  /* Todo digital (PORTA/E) */
    ANSELH = 0x00;                  /* PORTB digital: requisito para leer columnas */
    TEC_TRIS = 0xF0;                /* RB0-RB3 salidas (filas), RB4-RB7 entradas */
    TEC_PUERTO = 0x00;              /* Filas en 0 en reposo */

    OPTION_REGbits.nRBPU = 0;       /* Habilita pull-ups de PORTB */
    WPUB = 0xF0;                    /* Pull-up solo en columnas */
    IOCB = 0xF0;                    /* IOC en RB4..RB7 */

    (void)PORTB;                    /* Lectura inicial: fija el valor de referencia */
    INTCONbits.RBIF = 0;
    INTCONbits.RBIE = 1;            /* Interrupción por cambio en PORTB */
}

void teclado_isr(void)
{
    /*    Captura inmediata: se barre apenas llega el flanco, así una
     *    pulsación corta no se pierde (antes se esperaba 20 ms y, si la
     *    tecla ya se había soltado, el barrido no encontraba nada). */
    if ((TEC_PUERTO & TEC_MASK_COLS) != TEC_MASK_COLS && !tecla_presionada) {
        char t = teclado_scan();
        if (t != 0) {
            tecla_buffer = t;
            tecla_lista = 1;
            tecla_presionada = 1;   /* Bloquea repeticiones hasta soltar */
        }
    }

    TEC_PUERTO = 0x00;              /* Todas las filas de nuevo en 0 */
    __delay_us(30);

    /*    Anti-rebote DESPUÉS de capturar: los rebotes que lleguen en este
     *    tiempo se descartan al limpiar RBIF. */
    __delay_ms(20);

    /*    Si las columnas están estables en 1, la tecla ya se soltó. */
    if ((TEC_PUERTO & TEC_MASK_COLS) == TEC_MASK_COLS) {
        tecla_presionada = 0;
    }

    (void)PORTB;                    /* Desbloqueo: fin de la condición de mismatch */
    INTCONbits.RBIF = 0;
}

uint8_t teclado_hay_tecla(void)
{
    return tecla_lista;
}

char teclado_leer(void)
{
    char t;
    INTCONbits.RBIE = 0;            /* Sección crítica corta */
    t = tecla_buffer;
    tecla_lista = 0;
    INTCONbits.RBIE = 1;
    return t;
}
