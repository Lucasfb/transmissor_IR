#include <msp430.h>
#include "stdint.h"
#include <inc/IR_NEC.h>

#define DEBOUNCE_TIME 1000U // Microssegundos.
#define DEBOUNCE_REPETITIONS (DEBOUNCE_TIME/CCR0_PERIOD)
#define BUTTON_PRESS_ACTIVE 0 // Botao ativo em nivel logico baixo

// Mensagem a ser enviada
const uint8_t NEC_PWR_ADD = 0x04;
const uint8_t NEC_PWR_MSG = 0x08;

// Auxiliar para debouce
#define CCR2_PERIOD CCR0_PERIOD // CCR2 possui o mesmo period de CCR1
volatile uint16_t debouncer_countdown;
volatile uint16_t debounce_end = 0;

/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Para watchdog timer

    // Configura clock
    // Clock interno de 1MHz
    // Fonte de clock: DCOCLK

    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    // Configura PWM no pin P1.6
    IR_PWM_config();

    // Configura botao no pin 2.1
    P2DIR = 0x00; // Todos inicializados como input
    P2OUT |= BIT1; // necessário para usar o pullup resistor
    P2REN |= BIT1; // Pullup resistor
    P2SEL &= ~BIT1;
    P2SEL2 &= ~BIT1; // Funcao de I/O
    P2IES |= BIT1; // Interrupção em High-to-Low
    P2IFG &= ~BIT1; // Inicialmente não há nenhuma interrupção pendente
    P2IE |= BIT1;


    // Configura Timer0 CCR2 para uso no debouncer do pin 2.1
    TA0CCTL2 &= ~CAP; // Modo Compare
    TA0CCTL2 &= ~CCIE; // Desabilita interrupcoes do CCR2 para o debouncer
    TA0CCR2 = CCR2_PERIOD; // Periodo da interrupcao
    debouncer_countdown = DEBOUNCE_REPETITIONS;

    _bis_SR_register(GIE); // Ativa interrupcoes globais

      // Loop infinito
      while(1){
          if(debounce_end){
              // Caso apos o debounce o pin esta em nivel baixo, a funcao do botao e executada
              if ((~P2IN & BIT1)){
                  IR_send_NEC(NEC_PWR_ADD, NEC_PWR_MSG);
              }
              // Reseta o debounce
              debounce_end = 0;
              debouncer_countdown = DEBOUNCE_REPETITIONS;
              P2IFG &= ~BIT1; // Limpa flag de interrupcao para P2.1
              P2IE |= BIT1; // Ativa novamente interrupcoes no pino 2.1
          }

      };
    return 0;
}

// Interrupcao do botao sendo pressionado
// Para evitar bouncing do botao, uma segunda interrupcao desse mesmo botao é dasativada
// Uma interrupçao de timer diz se ja passou tempo suficiente para outro botao ser apertado
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    if(BIT1 & P2IFG) { // Verifica se a interrupcao e referent ao pino 2.1
        P2IFG &= ~BIT1; // Limpa flag de interrupcao para P2.1
        P2IE &= ~BIT1; // Desativa interrupcoes no pino 2.1
        TA0CCTL2 |= CCIE; // Habilita interrupcoes do timer para debouncer
    }
}


#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer_A3(void)
{
    debouncer_countdown--;
    if (!debouncer_countdown){
        debounce_end = 1;  // Flag
        TA0CCTL2 &= ~CCIE; // Desabilita interrupcoes do debouncer
        TACCTL2 &= ~CCIFG; // Limpa flag de interrupcao do timer
        }
}

