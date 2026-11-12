/**
 * @file    lcd.h
 * @brief   Librería para LCD 16x2 (HD44780) en modo 4 bits.
 * 
 * @author  Carlos Iter
 * @date    Noviembre 2026
 */
#ifndef LCD_H
#define LCD_H

#include <stdint.h>

#define LCD_CMD_CLEAR   0x01  /**< Limpia pantalla                 */
#define LCD_CMD_HOME    0x02  /**< Cursor a inicio                 */
#define LCD_LINEA1      0x80  /**< Dirección DDRAM línea 1         */
#define LCD_LINEA2      0xC0  /**< Dirección DDRAM línea 2         */

/** @brief Configura PORTD y ejecuta la secuencia de inicio a 4 bits. */
void lcd_init(void);
/** @brief Envía un comando al controlador (RS = 0). */
void lcd_cmd(uint8_t cmd);
/** @brief Escribe un carácter ASCII en la posición actual (RS = 1). */
void lcd_putc(char c);
/** @brief Escribe una cadena terminada en '\0'. */
void lcd_puts(const char *s);
/** @brief Limpia la pantalla y regresa el cursor al inicio. */
void lcd_clear(void);
/**
 * @brief Ubica el cursor.
 * @param fila 1 o 2
 * @param col  1..16
 */
void lcd_gotoxy(uint8_t fila, uint8_t col);
/** @brief Limpia y escribe dos líneas completas (atajo para los estados). */
void lcd_mostrar(const char *l1, const char *l2);

#endif 
