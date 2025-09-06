/*
En los pines P2.0 a P2.7 se encuentra conectado un display de 7 segmentos. Utilizando la variable numDisplay
[10] ={0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67} que codifica los números del 0 a 9 para ser mostrados en el display,
 realizar un programa que muestre indefinidamente la cuenta de 0 a 9 en dicho display.
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif
#define Segment (0x7F << 0)
#include <cr_section_macros.h>
#include <stdint.h>

const uint32_t numDisplay [10] ={0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67};
void delay(int tiempo){
	for(volatile int i = 0; i <tiempo;i++);
}
void configGPIO(){

	LPC_GPIO2->FIODIR |= Segment;
}

int main(void) {
	configGPIO();
	while(1){
		for (int i = 0; i < 10; i++ ){
			LPC_GPIO2->FIOCLR = Segment;
			LPC_GPIO2->FIOSET = (numDisplay[i] & Segment);
			delay(1000000);
		}
	}
    return 0 ;
}

