/*
 Enciende y apaga un led conectado al pin 0.22 con un periodo de intermitencia dado por el pulsador ubicado en el pin 0.15
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#include <stdint.h>
uint32_t inte = 0;
void cfgGPIO(void){
	LPC_PINCON->PINSEL0 &= ~(0x3 << 30);
	LPC_PINCON->PINSEL1 &= ~(0x3 << 12);
	LPC_GPIO0->FIODIR &= ~(1 << 15);
	LPC_GPIO0->FIODIR |= (1 << 22);
}

void cfgIntGPIO(){
	LPC_GPIOINT->IO0IntEnR |= (1 << 15);
	LPC_GPIOINT->IO0IntClr |= (1 << 15);
	NVIC_SetPriority(EINT2_IRQn, 0);
	NVIC_EnableIRQ(EINT2_IRQn);
}

void EINT3_IRQHandler(){
	if(LPC_GPIOINT->IntStatus & (1 << 0)){    /* hay interrupcion en P0? */
		if(LPC_GPIOINT->IO0IntEnR & (1 << 15)){ /* hay interrupcion en P15?*/
			inte++;
		}

	}
	return;
}

void delay(uint32_t time){
	volatile int i;
	for (i = 0; i<time;i++){}
}

int main(void)
{
	uint32_t time = 1000000;
	cfgGPIO();
	cfgIntGPIO();
	while(1){
	if(inte%2){
		time = 1000000;
	}else{
		time = 4000000;
	}
	LPC_GPIO->FIOSET |= (1 << 22);
	delay(time);
	LPC_GPIO->FIOCLR |= (1 << 22);
	delay(time);
	}
	return;
}
