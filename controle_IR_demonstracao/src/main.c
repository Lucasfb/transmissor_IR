#include <msp430.h>
#include "stdint.h"
#include <inc/IR_NEC.h>

// Mensagem a ser enviada
const uint8_t NEC_PWR_ADD = 0x04;
const uint8_t NEC_PWR_MSG = 0x08;


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

      // Loop infinito
      while(1){
          IR_send_NEC(NEC_PWR_ADD, NEC_PWR_MSG);
          _delay_cycles(10000);
      };

    return 0;
}
