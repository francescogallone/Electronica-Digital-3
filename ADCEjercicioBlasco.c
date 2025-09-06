/*
mediciones cada 1 segundo con systick y cuando superemos cierto umbral prendemos led p0.22
refp y negativo del adc van a gnd y refp con la del adc es 3.3v
tension = (refpositiva-refnegativa/4096)*dato_convertido
dato_convertido va de 0 a 4096     4096 = 3.3v
 */
#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif
#include <stdint.h>
#include <cr_section_macros.h>
uint32_t nticks = 9999999;
uint32_t vticks = 0;
#define LED_ROJO (1 << 22 )
#define LED_VERDE (1 << 25)
#define LED_AZUL (1 << 26)


void cfgGPIO();
void cfgADC();
void cfgIntSysTick();
void Systick_Handler();
void ADC_IRQHandler();

int main(){
	cfgGPIO();
	cfgADC();
	cfgIntSysTick();

	while(1){
		__WFI();
	}
}

void cfgGPIO(){
	LPC_PINCON->PINSEL1 &= ~(1 << 12);
	LPC_PINCON->PINSEL6 &= ~(1 << 15);
	LPC_PINCON->PINSEL6 &= ~(1 << 16);
	LPC_GPIO0->FIODIR |= LED_ROJO;
	LPC_GPIO0->FIODIR |= LED_VERDE;
	LPC_GPIO0->FIODIR |= LED_AZUL;
}
void cfgADC(){
	LPC_SC->PCONP |= (1 << 12);  /* alimento el adc */
	LPC_ADC->ADCR |= (1 << 21) |(1 << 0) | (0 << 8); /* 1<<21 start adc, 1<<0 uso canal 0, 0<<8 limpio el clkdiv y 1<<16 para modo burst*/
	LPC_SC->PCLKSEL0 &= ~(3 << 24); /*limpio PCLKSEL0*/
	LPC_SC->PCLKSEL0 |= (3 << 24);    /* PONGO 11 o sea cclk/8 = 12.5mhz*/
	LPC_PINCON->PINSEL1 |= (1 << 14); /* selecciono la funcion alternativa del pin 23 como adc*/
	LPC_ADC->ADINTEN = (1 << 0); /*interrupcion en canal 0*/
	NVIC_EnableIRQ(ADC_IRQn);
}
void cfgIntSysTick(){
	SysTick -> LOAD = nticks;
	SysTick -> VAL = 0;
	SysTick -> CTRL = 0x7;
}
void Systick_Handler(){
	vticks++;
	if(vticks >= 10){
		vticks = 0;
		LPC_ADC->ADCR |= (1 << 24);
	}
}
void ADC_IRQHandler(){
	uint32_t resultado = (LPC_ADC->ADDR0 >> 4) & 0xFFF;
	float V = (3.3/4096)*resultado;

	LPC_GPIO0->FIOCLR |= LED_ROJO;
	LPC_GPIO3->FIOCLR |= (LED_VERDE)|(LED_AZUL);

	if (V < 1){
		LPC_GPIO0->FIOSET |= LED_ROJO;
	}else if(V>= 1 | V<2){
		LPC_GPIO3->FIOSET |= LED_AZUL;
	}else if(V>=2 | V<3 ){
		LPC_GPIO3->FIOSET |= LED_VERDE;
	}else if(V>=3){
		static int toggle = 0;
		toggle = !toggle;
		if (toggle){
			LPC_GPIO0->FIOCLR |= (LED_ROJO);
			LPC_GPIO3->FIOCLR |= (LED_AZUL);
			LPC_GPIO3->FIOCLR |= (LED_VERDE);
		}else{
			LPC_GPIO0->FIOSET |= (LED_ROJO);
			LPC_GPIO3->FIOSET |= (LED_AZUL);
			LPC_GPIO3->FIOSET |= (LED_VERDE);
		}
	}
}









