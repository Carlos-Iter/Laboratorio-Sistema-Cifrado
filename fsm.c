/**
 * @file    fsm.c
 * @brief   Implementación de la máquina de estados del sistema criptográfico.
 *
 * @details Regla general de filtrado: en cada estado solo se aceptan las
 *          teclas listadas; cualquier otra se ignora sin cambiar de estado.
 * 
 * @author  Carlos Iter
 * @date    Noviembre 2026
 */
#include "config.h"
#include "lcd.h"
#include "cifrado.h"
#include "fsm.h"

/* ---------------- Arreglos  ---------------- */
static char    mensaje[MSG_LEN + 1];      /**< 4 caracteres + '\0'          */
static char    hex_in[HEX_LEN + 1];       /**< 8 caracteres HEX + '\0'      */
static uint8_t bytes[MSG_LEN];            /**< 4 bytes matemáticos          */
static uint8_t indice;                    /**< Posición de captura actual   */
static char    clave;                     /**< Clave de 1 carácter          */
static estado_t estado;                   /**< Estado actual                */

/** @brief 1 si la tecla es válida como dato (todo menos '*'). */
static uint8_t es_dato(char t)
{
    return (t != '*' && t != 0);
}

/**
 * @brief Mapeo condicional del teclado según el estado.
 * @details SOLO en S4 y mientras faltan caracteres: '*' -> 'E', '#' -> 'F'.
 *          En los demás estados la tecla se entrega sin cambios.
 */
static char normalizar_tecla(char t)
{
    if (estado == S4_DES_HEX && indice < HEX_LEN) {
        if (t == '*') return 'E';
        if (t == '#') return 'F';
    }
    return t;
}

/** @brief Acciones de entrada a cada estado (lo que se muestra en la LCD). */
static void entrar(estado_t nuevo)
{
    estado = nuevo;
    indice = 0;
    switch (estado) {
    case S0_MENU:
        mensaje[0] = '\0';
        hex_in[0]  = '\0';
        clave = 0;
        lcd_mostrar("A: Cifrar", "B: Descifrar");
        break;
    case S1_CIF_MENSAJE:
        lcd_mostrar("Mensaje:", "");
        lcd_gotoxy(2, 1);
        break;
    case S2_CIF_CLAVE:
    case S5_DES_CLAVE:
        lcd_mostrar("Ingrese clave:", "");
        lcd_gotoxy(2, 1);
        break;
    case S4_DES_HEX:
        lcd_mostrar("Cripto (HEX):", "");
        lcd_gotoxy(2, 1);
        break;
    default:
        break;
    }
}

/** @brief S3: cifra y muestra "Cifrado (HEX):" + 4 bytes en HEX. */
static void mostrar_cifrado(void)
{
    char par[2];
    cifrar_mensaje(mensaje, clave, bytes);
    lcd_mostrar("Cifrado (HEX):", "");
    lcd_gotoxy(2, 1);
    for (uint8_t i = 0; i < MSG_LEN; i++) {
        byte_a_hex(bytes[i], par);
        lcd_putc(par[0]);
        lcd_putc(par[1]);
        if (i < MSG_LEN - 1) lcd_putc(' ');
    }
    estado = S3_CIF_RESULTADO;
}

/** @brief S6: empaqueta 8 HEX -> 4 bytes, descifra y muestra el texto. */
static void mostrar_descifrado(void)
{
    hex_a_bytes(hex_in, bytes);
    descifrar_mensaje(bytes, clave, mensaje);
    lcd_mostrar("Mensaje Original", "");   /* 16 caracteres máximo por línea */
    lcd_gotoxy(2, 1);
    for (uint8_t i = 0; i < MSG_LEN; i++) {
        char c = mensaje[i];
        lcd_putc((c >= 0x20 && c <= 0x7E) ? c : '?'); /* Clave errada -> '?' */
    }
    estado = S6_DES_RESULTADO;
}

void fsm_init(void)
{
    entrar(S0_MENU);
}

estado_t fsm_estado(void)
{
    return estado;
}

void fsm_procesar(char tecla)
{
    char t = normalizar_tecla(tecla);

    switch (estado) {
    case S0_MENU:
        /* Filtro del menú: solo 'A' o 'B'; todo lo demás se ignora */
        if (t == 'A')      entrar(S1_CIF_MENSAJE);
        else if (t == 'B') entrar(S4_DES_HEX);
        break;

    case S1_CIF_MENSAJE:
        if (indice < MSG_LEN && es_dato(t)) {
            mensaje[indice++] = t;
            mensaje[indice] = '\0';
            lcd_putc(t);
        } else if (indice == MSG_LEN && t == '*') {
            entrar(S2_CIF_CLAVE);
        }
        break;

    case S2_CIF_CLAVE:
        if (es_dato(t)) {
            clave = t;
            lcd_putc('*');          /* La clave nunca se muestra */
            __delay_ms(400);
            mostrar_cifrado();      /* -> S3 */
        }
        break;

    case S3_CIF_RESULTADO:
    case S6_DES_RESULTADO:
        if (t == '*') entrar(S0_MENU);
        break;

    case S4_DES_HEX:
        if (indice < HEX_LEN && ascii_a_nibble(t) != 0xFF) {
            hex_in[indice++] = t;   /* t ya viene con * -> E y # -> F */
            hex_in[indice] = '\0';
            lcd_putc(t);
        } else if (indice == HEX_LEN && t == '*') {
            entrar(S5_DES_CLAVE);   /* Con 8 HEX el mapeo se apaga: '*' = continuar */
        }
        break;

    case S5_DES_CLAVE:
        if (es_dato(t)) {
            clave = t;
            lcd_putc('*');
            __delay_ms(400);
            mostrar_descifrado();   /* -> S6 */
        }
        break;

    default:
        entrar(S0_MENU);
        break;
    }
}
