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

uint32_t nTicks = 9999999;
uint32_t vTicks = 0;

void cfgGPIO(){
	LPC_PINCON->PINSEL1 &= ~(0x3 << 22);
	LPC_GPIO0->FIODIR |= (1 << 22);
	return;
}

void cfgSysTick(){
	SysTick->LOAD = nTicks;
	SysTick->VAL = 0;
	SysTick->CTRL = 0x07;

	return;
}

void Systick_Handler(){
	vTicks++;
	if(vTicks%2){
		LPC_GPIO0->FIOSET = (1 << 22);
	}else{
		LPC_GPIO0->FIOCLR = (1 << 22);
	}
	SysTick->CTRL &= SysTick->CTRL;
	return;
}
int main(void) {

	cfgGPIO();
	cfgSysTick();
	while(1) {
        }
    return 0 ;
}
