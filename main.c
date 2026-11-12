/**
 * @file    main.c
 * @brief   MAIN PRINCIPAL: Sistema de Cifrado Simétrico
 *
 * @details El main está libre de carga: no barre el teclado ni hace polling
 *          de pines. Toda la detección ocurre en la ISR (IOC en RB4..RB7); el
 *          main solo revisa la bandera que deja la ISR y, si hay tecla nueva,
 *          se la pasa a la máquina de estados.
 *
 * @author  Carlos Iter
 * @date    Noviembre 2026
 */
#include "config.h"

#if (APP_MODO == MODO_PRINCIPAL)

#include "sistema.h"
#include "lcd.h"
#include "teclado.h"
#include "fsm.h"

/** @brief Punto de entrada del sistema completo. */
void main(void)
{
    sistema_init();
    lcd_init();
    teclado_init();
    fsm_init();                         /* S0: "A: Cifrar" / "B: Descifrar" */
    sistema_habilitar_interrupciones();

    while (1) {
        if (teclado_hay_tecla()) {
            fsm_procesar(teclado_leer());
        }
    }
}

#endif
