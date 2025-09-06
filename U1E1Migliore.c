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
#define leds (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5) | (1<<6) | (1<<7) | (1<<8) | (1<<9)

int temp1 = 50;
int temp2 = 200;

void delay(int tiempo) {
    volatile int i, j;
    for(i = 0; i < tiempo; i++){
        for(j = 0; j < 1000; j++){
            ; // nop
        }
    }
}
void secuencia1(int tiempo){
	LPC_GPIO0->FIOSET |= leds;
			delay(tiempo);
			LPC_GPIO0->FIOCLR |= leds;
			delay(tiempo);
}
void secuencia2(int tiempo){
	LPC_GPIO0->FIOSET |= leds;
			delay(tiempo);
			LPC_GPIO0->FIOCLR |= leds;
			delay(tiempo);
}
int main(void) {
	LPC_GPIO0->FIODIR |= leds;
	while(1){
		secuencia1(temp1);
		secuencia2(temp2);
	}
    return 0 ;
}









