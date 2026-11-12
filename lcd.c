/**
 * @file    lcd.c
 * @brief   Implementación de la librería LCD HD44780 en modo 4 bits.
 *
 * @details Un byte se envía como dos nibbles (alto y luego bajo), cada uno
 *          validado con un pulso en EN (ver clase 04, diapositiva 9).
 * 
 * @author  Carlos Iter
 * @date    Noviembre 2026
 */
#include "config.h"
#include "pines.h"
#include "lcd.h"

/**
 * @brief Pone un nibble en RD4..RD7 sin alterar RD0..RD3 y da el pulso EN.
 * @param nibble valor en los 4 bits bajos
 */
static void lcd_nibble(uint8_t nibble)
{
    LCD_PUERTO = (uint8_t)((LCD_PUERTO & 0x0F) | ((nibble & 0x0F) << 4));
    LCD_EN = 1;
    __delay_us(2);
    LCD_EN = 0;
    __delay_us(50);
}

/** @brief Envía un byte completo en dos nibbles. */
static void lcd_byte(uint8_t dato, uint8_t rs)
{
    LCD_RS = rs;
    lcd_nibble(dato >> 4);
    lcd_nibble(dato & 0x0F);
}

void lcd_cmd(uint8_t cmd)
{
    lcd_byte(cmd, 0);
    if (cmd == LCD_CMD_CLEAR || cmd == LCD_CMD_HOME) {
        __delay_ms(2);              /* Estos comandos tardan ~1.6 ms */
    }
}

void lcd_putc(char c)
{
    lcd_byte((uint8_t)c, 1);
}

void lcd_puts(const char *s)
{
    while (*s) {
        lcd_putc(*s++);
    }
}

void lcd_clear(void)
{
    lcd_cmd(LCD_CMD_CLEAR);
}

void lcd_gotoxy(uint8_t fila, uint8_t col)
{
    uint8_t base = (fila == 2) ? LCD_LINEA2 : LCD_LINEA1;
    lcd_cmd((uint8_t)(base + (col - 1)));
}

void lcd_mostrar(const char *l1, const char *l2)
{
    lcd_clear();
    lcd_gotoxy(1, 1);
    lcd_puts(l1);
    lcd_gotoxy(2, 1);
    lcd_puts(l2);
}

void lcd_init(void)
{
    LCD_TRIS = 0x00;                /* RD0..RD7 como salidas (RS, EN, D4-D7) */
    LCD_PUERTO = 0x00;
    __delay_ms(20);                 /* Espera de encendido (>15 ms) */

    /* Secuencia de reinicio por software para entrar a 4 bits */
    LCD_RS = 0;
    lcd_nibble(0x03); __delay_ms(5);
    lcd_nibble(0x03); __delay_us(150);
    lcd_nibble(0x03);
    lcd_nibble(0x02);               /* Modo 4 bits */

    lcd_cmd(0x28);                  /* 4 bits, 2 líneas, 5x8      */
    lcd_cmd(0x0C);                  /* Display ON, cursor OFF     */
    lcd_cmd(0x06);                  /* Incremento, sin desplazar  */
    lcd_clear();
}
