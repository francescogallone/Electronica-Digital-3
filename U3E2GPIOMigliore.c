
#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#include <stdint.h>

uint32_t auxiliar;
uint16_t secuencia0 = 0b100111;
uint16_t secuencia2 = 0b111001011010110;

void cgfGPIO(){
	LPC_PINCON->PINSEL0 &= ~(0x3 << 0);
	LPC_PINCON->PINSEL4 &= ~(0x3 << 0);
	LPC_GPIO0->FIODIR &= ~(1 << 0);
	LPC_GPIO2->FIODIR &= ~(1 << 0);
}

void cfgIntGPIO(){
	LPC_GPIOINT->IO0IntEnR |= (1 << 0);
	LPC_GPIOINT->IO2IntClr |= (1 << 0);
	LPC_GPIOINT->IO2IntEnR |= (1 << 0);
	LPC_GPIOINT->IO2IntClr |= (1 << 0);
}

void delay(uint32_t time){
	for (int i = 0; i<time; i++){}
}

void EINT3_IRQHandler(){
	if(LPC_GPIOINT->IntStatus & (1 << 0)){
		if(LPC_GPIOINT->IO0IntStatR & (1 << 0)){
			LPC_GPIOINT->IO0IntClr |= (1 << 0);
			auxiliar = secuencia0;
			delay(1000000);
		}
	}
	if(LPC_GPIOINT->IntStatus & (1 << 2)){
		if(LPC_GPIOINT->IO2IntStatR & (1 << 0)){
			LPC_GPIOINT->IO2IntClr |= (1 << 0);
			auxiliar = secuencia2;
			delay(1000000);
		}
	}
}

int main(void) {

    while(1) {
    }
    return 0 ;
}
