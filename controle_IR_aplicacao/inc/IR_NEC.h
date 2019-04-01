/*
 * IR_NEC.h
 *
 *  Created on: 7 de jan de 2019
 *      Author: lucas
 */

#ifndef IR_NEC_H_
#define IR_NEC_H_

#include "stdint.h"
#include <msp430.h>

#define CCR0_PERIOD 25U
#define CCR1_DUTY_CYLE 9U

// Microssegundos de duracao para cada parte do sinal
#define NEC_DELAY_CORRECTION 20U

#define NEC_HEADER_MARK  9000U-NEC_DELAY_CORRECTION
#define NEC_HEADER_SPACE 4500U-NEC_DELAY_CORRECTION
#define NEC_BIT_MARK  560U-NEC_DELAY_CORRECTION
#define NEC_ONE_SPACE 1690U-NEC_DELAY_CORRECTION
#define NEC_ZERO_SPACE  560U-NEC_DELAY_CORRECTION

// Configura o PWM para 38kHz
void IR_PWM_config(void);
void IR_PWM_enable(void);
void IR_PWM_disable(void);

// Envia uma mensagem e para um endereco via IR usando o protocolo NEC
// Deve ser chamada apenas após o PWM ser configurado
void IR_send_NEC(uint8_t address, uint8_t data);



#endif /* IR_NEC_H_ */
