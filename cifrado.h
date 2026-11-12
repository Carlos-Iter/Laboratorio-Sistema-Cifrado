/**
 * @file    cifrado.h
 * @brief   Algoritmo de cifrado simétrico híbrido y conversiones ASCII <-> HEX.
 *
 * @details
 *  Cifrado:    C = (M XOR K) + 5        (módulo 256)
 *  Descifrado: M = (C - 5) XOR K        (primero se resta, luego XOR)
 *
 *  La precedencia importa: en el cifrado el XOR va primero y luego la suma,
 *  por lo tanto la inversa deshace en orden contrario: primero la resta y
 *  luego el XOR (el XOR es su propia inversa).
 *
 * @author  Carlos Iter
 * @date    Noviembre 2026
 */
#ifndef CIFRADO_H
#define CIFRADO_H

#include <stdint.h>

#define MSG_LEN     4             
#define HEX_LEN     (MSG_LEN * 2)  
#define DESPLAZAMIENTO 5           

/**Cifra un byte: (m ^ k) + 5. */
uint8_t cifrar_byte(uint8_t m, uint8_t k);

/**Descifra un byte: (c - 5) ^ k. */
uint8_t descifrar_byte(uint8_t c, uint8_t k);

void cifrar_mensaje(const char *msg, char clave, uint8_t *out);

void descifrar_mensaje(const uint8_t *in, char clave, char *msg);

char nibble_a_ascii(uint8_t n);

uint8_t ascii_a_nibble(char c);

void byte_a_hex(uint8_t b, char *out);

/**
 * @brief Empaqueta HEX_LEN caracteres HEX en MSG_LEN bytes.
 * @return 1 si todos los caracteres son válidos, 0 si no.
 */
uint8_t hex_a_bytes(const char *hex, uint8_t *out);

#endif 
