/**
 * @file    sistema.h
 * @brief   Inicialización común (oscilador, puertos digitales) e interrupciones.
 * 
 * @author  Carlos Iter
 * @date    Noviembre 2026
 */
#ifndef SISTEMA_H
#define SISTEMA_H

/** @brief Oscilador interno a 8 MHz y todos los pines digitales. */
void sistema_init(void);

/** @brief Habilita GIE (llamar después de inicializar los periféricos). */
void sistema_habilitar_interrupciones(void);

#endif 
