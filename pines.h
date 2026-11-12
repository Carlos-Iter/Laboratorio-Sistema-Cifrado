/**
 * @file    pines.h
 * @brief   Mapeo de hardware: qué pin del PIC16F887 va a cada periférico.
 *
 *  Teclado 4x4 (PORTB):
 *  - Filas  X0..X3 -> RB0..RB3 (salidas, en 0 lógico en reposo)
 *  - Columnas Y0..Y3 -> RB4..RB7 (entradas con pull-up interno + IOC)
 *  - Conector del teclado (8 pines): 1-4 filas, 5-8 columnas.
 * @author  Carlos Iter
 * @date    Noviembre 2026
 */
#ifndef PINES_H
#define PINES_H

#include "config.h"

/* ---------------- LCD ---------------- */
#define LCD_RS          PORTDbits.RD0   
#define LCD_EN          PORTDbits.RD1  
#define LCD_PUERTO      PORTD           
#define LCD_TRIS        TRISD           

/* ---------------- Teclado ---------------- */
#define TEC_PUERTO      PORTB
#define TEC_TRIS        TRISB
#define TEC_MASK_FILAS  0x0F            
#define TEC_MASK_COLS   0xF0            

#endif 
