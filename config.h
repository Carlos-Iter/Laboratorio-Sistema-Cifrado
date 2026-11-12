/**
 * @file    config.h
 * @brief   Bits de configuración del PIC16F887, frecuencia del reloj y
 *          selección del "main" que se compila (principal o pruebas por bloque).
 *
 * @author  Carlos Iter
 * @date    Noviembre 2026
 */
#ifndef CONFIG_H
#define CONFIG_H
#define MODO_PRINCIPAL      0   
#define MODO_TEST_LCD       1   
#define MODO_TEST_TECLADO   2   
#define MODO_TEST_CIFRADO   3   


#define APP_MODO            MODO_PRINCIPAL

/* 
/*  Frecuencia del oscilador (necesaria para __delay_ms / __delay_us)  */

#define _XTAL_FREQ          8000000UL   /**< Oscilador interno a 8 MHz */

#ifdef CONFIG_BITS_AQUI
#pragma config FOSC  = INTRC_NOCLKOUT /**< Oscilador interno, RA6/RA7 como I/O   */
#pragma config WDTE  = OFF            /**< Watchdog deshabilitado                 */
#pragma config PWRTE = ON             /**< Power-up timer habilitado              */
#pragma config MCLRE = ON             /**< RE3 como MCLR (botón de reset)         */
#pragma config CP    = OFF            /**< Sin protección de código               */
#pragma config CPD   = OFF            /**< Sin protección de EEPROM               */
#pragma config BOREN = ON             /**< Brown-out reset habilitado             */
#pragma config IESO  = OFF
#pragma config FCMEN = OFF
#pragma config LVP   = OFF            /**< RB3 libre (programación en bajo voltaje OFF) */
#pragma config BOR4V = BOR40V
#pragma config WRT   = OFF
#endif

#include <xc.h>
#include <stdint.h>

#endif 
