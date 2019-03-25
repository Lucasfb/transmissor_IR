/*
 * IR_NEC.c
 *
 *  Created on: 7 de jan de 2019
 *      Author: lucas
 */

#include <inc/IR_NEC.h>



void IR_send_NEC(uint8_t address, uint8_t data){

    //header

    IR_PWM_enable();
    _delay_cycles(NEC_HEADER_MARK);

    IR_PWM_disable();
    _delay_cycles(NEC_HEADER_SPACE);

    // Corpo da mensagem
    // Endereço
    uint8_t mask;
    for  (mask = 1; mask; mask <<= 1){
        if (address & mask){
            IR_PWM_enable();
            _delay_cycles(NEC_BIT_MARK);
            IR_PWM_disable();
            _delay_cycles(NEC_ONE_SPACE);
        }
        else{
            IR_PWM_enable();
            _delay_cycles(NEC_BIT_MARK);
            IR_PWM_disable();
            _delay_cycles(NEC_ZERO_SPACE);
        };
    };

    // Complemento do endereço
    for  (mask = 1; mask; mask <<= 1){
        if (~address & mask){
            IR_PWM_enable();
            _delay_cycles(NEC_BIT_MARK);
            IR_PWM_disable();
            _delay_cycles(NEC_ONE_SPACE);

        }
        else{
            IR_PWM_enable();
            _delay_cycles(NEC_BIT_MARK);
            IR_PWM_disable();
            _delay_cycles(NEC_ZERO_SPACE);
        };
    };
    // Data
    for  (mask = 1; mask; mask <<= 1){
        if (data & mask){
            IR_PWM_enable();
            _delay_cycles(NEC_BIT_MARK);
            IR_PWM_disable();
            _delay_cycles(NEC_ONE_SPACE);
        }
        else{
            IR_PWM_enable();
            _delay_cycles(NEC_BIT_MARK);
            IR_PWM_disable();
            _delay_cycles(NEC_ZERO_SPACE);
        };
    };
    // Complemento de Data
    for  (mask = 1; mask; mask <<= 1){
        if (~data & mask){
            IR_PWM_enable();
            _delay_cycles(NEC_BIT_MARK);
            IR_PWM_disable();
            _delay_cycles(NEC_ONE_SPACE);
        }
        else{
            IR_PWM_enable();
            _delay_cycles(NEC_BIT_MARK);
            IR_PWM_disable();
            _delay_cycles(NEC_ZERO_SPACE);
        };
    };

    // footer
    IR_PWM_enable();
    _delay_cycles(NEC_BIT_MARK);
    IR_PWM_disable();
    _delay_cycles(0);
}

void IR_PWM_config(void){
    // Configura P1.6 como Timer A PWM output
    // Para PDIP20 esse é o pin 14
    P1DIR |= BIT6;
    P1SEL |= BIT6;

    // Configura o Timer
    //TASSEL_2 -> CLOCK SOURCE: MCLK   MC_1 -> UP MODE
    TA0CTL = TASSEL_2 + MC_1;
    //MODO DE SAÍDA DO TIMER0_A: OUTPUT ONLY
    TACCTL1 = OUTMOD_0;
    // ESTADO QUANDO O PWM ESTA DESATIVADO E NIVEL BAIXO
    TACCTL1 &= ~OUT;
    // PERIODO E DUTY CYCLE DO PWM
    TACCR0 = CCR0_PERIOD; //PERÍODO DO PWM
    TACCR1 = CCR1_DUTY_CYLE; //DUTY CYCLE DO PWM EM 33%
}

void IR_PWM_enable(void){
    TACCTL1 = OUTMOD_7;
}

void IR_PWM_disable(void){
    // Para o Timer e reseta o valor
    TACCTL1 &= OUTMOD_0;

}

