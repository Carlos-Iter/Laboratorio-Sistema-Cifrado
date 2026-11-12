/**
 * @file    teclado.h
 * @brief   Teclado matricial 4x4 manejado por interrupción por cambio (IOC) en RB4..RB7.
 *
 * @details El main NO hace barrido: la ISR detecta el cambio en las columnas,
 *          aplica anti-rebote, escanea la fila y deja la tecla en un buffer.
 *          El main solo pregunta teclado_hay_tecla() y lee teclado_leer().
 * 
 * @author  Carlos Iter
 * @date    Noviembre 2026
 */
#ifndef TECLADO_H
#define TECLADO_H

#include <stdint.h>

/** @brief Configura PORTB: filas salida, columnas entrada, pull-ups, IOC y RBIE. */
void teclado_init(void);

/** @brief Rutina llamada desde la ISR cuando RBIF = 1. */
void teclado_isr(void);

/** @brief Retorna 1 si hay una tecla nueva sin leer. */
uint8_t teclado_hay_tecla(void);

/**
 * @brief Entrega la última tecla y limpia la bandera.
 * @return Carácter ASCII: '0'-'9', 'A'-'D', '*', '#'. 0 si no hay tecla.
 */
char teclado_leer(void);

#endif 
