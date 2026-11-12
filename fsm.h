/**
 * @file    fsm.h
 * @brief   Máquina de estados de la interfaz (menú, cifrado y descifrado).
 *
 * @author  Carlos Iter
 * @date    Noviembre 2026
 */
#ifndef FSM_H
#define FSM_H


typedef enum {
    S0_MENU = 0,        /**< Menú principal                         */
    S1_CIF_MENSAJE,     /**< Captura de 4 caracteres a cifrar       */
    S2_CIF_CLAVE,       /**< Captura de la clave (se muestra '*')   */
    S3_CIF_RESULTADO,   /**< Muestra el criptograma en HEX          */
    S4_DES_HEX,         /**< Captura 8 HEX, mapeo * = E y # = F     */
    S5_DES_CLAVE,       /**< Captura de la clave (se muestra '*')   */
    S6_DES_RESULTADO    /**< Muestra el mensaje original            */
} estado_t;

/** @brief Lleva el sistema a S0 y dibuja el menú. */
void fsm_init(void);

/**
 * @brief Procesa UNA tecla según el estado actual (llamada desde el main
 *        solo cuando la ISR reportó una tecla nueva).
 */
void fsm_procesar(char tecla);

/** @brief Estado actual (útil para depuración). */
estado_t fsm_estado(void);

#endif 
