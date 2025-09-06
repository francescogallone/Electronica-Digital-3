/*Modificar el ejercicio ADC_1 configurando una interrupción de GPIO en el pin P0.15 y P0.16 (use el flanco que prefiera).
Si se produce una interrupción de GPIO por el pin P0.15 se realizará una conversión y se deshabilitará la conversión periódica del ejercicio ADC_1.
Si se produce una interrupción de GPIO en el pin P0.16, se habilita nuevamente las conversiones cada 1 segundo.
La lógica de los leds debe conservarse independientemente de si se está convirtiendo de forma periódica o
si se realiza una conversión disparada por interrupción de GPIO en el pin P0.15*/
#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif
#include <stdint.h>
#include <cr_section_macros.h>
uint32_t nTicks = 9999999;
uint32_t vTicks = 0;
int chirolitasdesactivadoras = 0;
#define LED_ROJO (1 << 22)
#define LED_VERDE (1 << 25)
#define LED_AZUL (1 << 26)

void cfgGPIO();
void cfgADC();
void cfgIntSysTick();
void ADC_IRQHandler();
void SysTick_Handler();
void EINT3_IRQHandler();
void cfgIntGPIO();

int main(void) {
	cfgGPIO();
	cfgADC();
	cfgIntGPIO();
	cfgIntSysTick();
	while(1) {

	}
    return 0 ;
}

void cfgGPIO(){
	LPC_PINCON->PINSEL0 &= ~(0x3 << 30);
	LPC_PINCON->PINSEL1 &= ~(0x3 << 12) | (0x3 << 0);
	LPC_GPIO0->FIODIR &= ~(1 << 15) | (1 << 16);
	LPC_GPIO0->FIODIR |= LED_ROJO;
	LPC_GPIO3->FIODIR |= LED_VERDE | LED_AZUL;
}

void cfgIntGPIO(){
	LPC_GPIOINT->IO0IntEnR |= (1 << 15);
	LPC_GPIOINT->IO0IntClr |= (1 << 15);
	LPC_GPIOINT->IO0IntEnF |= (1 << 16);
	LPC_GPIOINT->IO0IntClr |= (1 << 16);
	NVIC_SetPriority(EINT3_IRQn, 1);
	NVIC_EnableIRQ(EINT3_IRQn);
}

void cfgIntSysTick(){
	SysTick-> LOAD = nTicks;
	SysTick-> VAL = 0;
	SysTick-> CTRL = 0x7;
	NVIC_SetPriority(SysTick_IRQn,3);
}

void cfgADC(){
	LPC_SC->PCONP |= (1 << 12);
	LPC_ADC->ADCR |= (1 << 21) | (0 << 8) << (1 << 0);
	LPC_SC->PCLKSEL0 |= (0x3 << 24);
	LPC_PINCON->PINSEL1 |= (1 << 14);
	LPC_ADC->ADINTEN |= (1 << 0);
	NVIC_SetPriority(ADC_IRQn,2);
	NVIC_EnableIRQ(ADC_IRQn);

}

void Systick_Handler(){
	vTicks++;
	if(chirolitasdesactivadoras == 1){
		LPC_ADC->ADCR |= (1 << 24);
	}
	if(chirolitasdesactivadoras == 0){
		if(vTicks >= 10){
		vTicks = 0;
		LPC_ADC->ADCR |= (1 << 24);
		}
	}
}

void EINT3_IRQHandler(){
	if(LPC_GPIOINT->IntStatus & (1 << 0)){
		if(LPC_GPIOINT->IO0IntStatR & (1 << 15)){
			LPC_GPIOINT->IO0IntClr;
			chirolitasdesactivadoras = 1;
		}
		if(LPC_GPIOINT->IO0IntStatF & (1 << 16)){
			LPC_GPIOINT->IO0IntClr;
			chirolitasdesactivadoras = 0;
		}
	}
}

void ADC_IRQHandler(){
	uint32_t resultado = (LPC_ADC->ADDR0 << 4) & (0xFFF);
	float V = (3.3/4096)*resultado;
	if(V<1){
		LPC_GPIO0->FIOSET |= LED_ROJO;
	}else if (V >= 1 | V<2){
		LPC_GPIO3->FIOSET |= LED_AZUL;
	}else if (V>=2 | V<3){
		LPC_GPIO3->FIOSET |= LED_VERDE;
	}else{
		static int tortillita;
		tortillita = !tortillita;
		if(tortillita){
			LPC_GPIO0->FIOSET |= LED_ROJO;
			LPC_GPIO3->FIOSET |= LED_AZUL | LED_VERDE;
		}else{
			LPC_GPIO0->FIOCLR |= LED_ROJO;
			LPC_GPIO3->FIOCLR |= LED_AZUL | LED_VERDE;
		}
	}

}
