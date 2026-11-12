/**
 * @file    cifrado.c
 * @brief   Implementación del cifrado XOR + desplazamiento y de las conversiones.
 * 
 * @author  Carlos Iter
 * @date    Noviembre 2026
 */
#include "cifrado.h"

uint8_t cifrar_byte(uint8_t m, uint8_t k)
{
    return (uint8_t)((m ^ k) + DESPLAZAMIENTO);   /* XOR primero, luego +5 */
}

uint8_t descifrar_byte(uint8_t c, uint8_t k)
{
    return (uint8_t)((uint8_t)(c - DESPLAZAMIENTO) ^ k); /* -5 primero, luego XOR */
}

void cifrar_mensaje(const char *msg, char clave, uint8_t *out)
{
    for (uint8_t i = 0; i < MSG_LEN; i++) {
        out[i] = cifrar_byte((uint8_t)msg[i], (uint8_t)clave);
    }
}

void descifrar_mensaje(const uint8_t *in, char clave, char *msg)
{
    for (uint8_t i = 0; i < MSG_LEN; i++) {
        msg[i] = (char)descifrar_byte(in[i], (uint8_t)clave);
    }
}

char nibble_a_ascii(uint8_t n)
{
    n &= 0x0F;
    return (char)((n < 10) ? ('0' + n) : ('A' + n - 10));
}

uint8_t ascii_a_nibble(char c)
{
    if (c >= '0' && c <= '9') return (uint8_t)(c - '0');
    if (c >= 'A' && c <= 'F') return (uint8_t)(c - 'A' + 10);
    return 0xFF;
}

void byte_a_hex(uint8_t b, char *out)
{
    out[0] = nibble_a_ascii(b >> 4);    /* Nibble alto */
    out[1] = nibble_a_ascii(b & 0x0F);  /* Nibble bajo */
}

uint8_t hex_a_bytes(const char *hex, uint8_t *out)
{
    for (uint8_t i = 0; i < MSG_LEN; i++) {
        uint8_t alto = ascii_a_nibble(hex[2 * i]);
        uint8_t bajo = ascii_a_nibble(hex[2 * i + 1]);
        if (alto == 0xFF || bajo == 0xFF) return 0;
        out[i] = (uint8_t)((alto << 4) | bajo);
    }
    return 1;
}
