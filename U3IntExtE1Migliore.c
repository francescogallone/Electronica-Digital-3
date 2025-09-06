/*
 * Copyright 2022 NXP
 * NXP confidential.
 * This software is owned or controlled by NXP and may only be used strictly
 * in accordance with the applicable license terms.  By expressly accepting
 * such terms or by downloading, installing, activating and/or otherwise using
 * the software, you are agreeing that you have read, and that you agree to
 * comply with and are bound by, such license terms.  If you do not agree to
 * be bound by the applicable license terms, then you may not retain, install,
 * activate or otherwise use the software.
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#include <stdint.h>

uint8_t secuencia1[] = {0,1,0,0,1,1,0,1,0};
uint8_t secuencia2[] = {0,1,1,1,0,0,1,1,0};

void delay(uint32_t tiempo){
	for (int i = 0;i<tiempo;i++){}
}

void cfgGPIO(){
	LPC_PINCON->PINSEL4 &= ~(0x3 << 0);
	LPC_PINCON->PINSEL4 &= ~(0x3 << 2);
	LPC_PINCON->PINSEL0 &= ~(0x3 << 0);
	LPC_PINCON->PINSEL0 &= ~(0x3 << 2);
	LPC_GPIO0->FIODIR |= (1 << 0);
	LPC_GPIO0->FIODIR |= (1 << 1);
	LPC_GPIO2->FIODIR &= ~(1 << 0);
	LPC_GPIO2->FIODIR &= ~(1 << 1);
}
void enviarsecuencia(uint8_t *secuencia, uint8_t longitud, uint8_t pin){
	for (int i = 0;i<longitud;i++){
		if(secuencia[i]){
			LPC_GPIO0->FIOSET = (1 << pin);
		}else{
			LPC_GPIO0->FIOCLR = (1 << pin);
		}
		delay(1000000);
	}
}

void cfgIntGPIO(){
	LPC_GPIOINT->IO2IntEnR |= (1 << 0);
	LPC_GPIOINT->IO2IntClr |= (1 << 0);
	LPC_GPIOINT->IO2IntEnF |= (1 << 1);
	LPC_GPIOINT->IO2IntClr |= (1 << 1);
	NVIC_SetPriority(EINT3_IRQn, 1);
	NVIC_EnableIRQ(EINT3_IRQn);
}

void EINT3_IRQHandler(){
	if(LPC_GPIOINT->IntStatus & (1 << 2)){
		if(LPC_GPIOINT->IO2IntStatR & (1 << 0)){
			LPC_GPIOINT->IO2IntClr = (1 << 0);
			enviarsecuencia(secuencia1,9,0);
	}
		if(LPC_GPIOINT->IO2IntStatF & (1 << 1)){
			LPC_GPIOINT->IO2IntClr = (1 << 1);
			enviarsecuencia(secuencia2,9,1);
		}
}
}
int main(void) {
	cfgGPIO();
	cfgIntGPIO();
	while(1){
		LPC_GPIO0->FIOSET = (1 << 0) | (1 << 1);
	}

}
