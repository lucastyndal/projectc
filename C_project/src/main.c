#include <stdio.h>
#include <math.h>
//#include <unistd.h>
#include "sys/cm4.h"
#include "sys/devices.h"
#include "sys/init.h"
#include "sys/clock.h"


/* Prototype des fonctions utilisees */
void led_init();
void led_tri_set(uint32_t status);
void enable_GPIOA();
void enable_GPIOB();
void tempo_250ms();
uint32_t button_poll();
void button_init();
void led_pa5(uint32_t status);

void exo1_1(){
	led_pa5(0);
	while (1) {
		if(button_poll()){
	   		led_pa5(1);
		}
		else{
			led_pa5(0);
		}
	}
}

void exo1_2(){
	int8_t index = 0;	
	while(1){
		led_pa5(index % 2);
		tempo_250ms();		
		tempo_250ms();
		index++;	
	}
}

void exo1_3(){
	int8_t enclenche = 0;
	while(1){
		if(button_poll()){
			enclenche = 1;
			led_pa5(1);
		}
		
		else if(!button_poll() && enclenche){
			for(int8_t i = 0; i < 8; i++){
				led_pa5(i % 2);
				tempo_250ms();
			}
			led_pa5(0);
			enclenche = 0;
		}
		
		else{
			led_pa5(0);
		}
	}
}

void exo2(){
	char c = 0x0A;
	for(int8_t i = 0; i <  50; i++){
		printf("%c", c);
	}
	

	//scanf("%c", &c);
	//printf("%c", c);

	char s[2];
	scanf("%s", s);
	puts(s);
}

int main() {
	enable_GPIOA();
	led_init();
	button_init();
	SysTick_init(100);

	exo2();
	
	return 0;
}



void led_init() {
  /* Configure PA4-PA10 as outputs, push-pull, high speed*/
        GPIOA.MODER = (GPIOA.MODER & ~(0x3FFF<<8)) | (0x1555<<8);
        GPIOA.OTYPER  &= ~(0x7F<<4);
        GPIOA.OSPEEDR |= (0x3FFF<<10);
}

void SysTick_init(uint32_t freq){
	uint32_t p=get_SYSCLK()/freq;
	SysTick.LOAD=(p-1)&0x00FFFFFF;
	SysTick.VAL=0;
	SysTick.CTRL |=7;
}


void led_tri_set(uint32_t status) {
        GPIOA.ODR = (GPIOA.ODR & ~(7<<8) ) | (1<<(8+status));
}

void led_pa5(uint32_t status) {
        GPIOA.ODR = (GPIOA.ODR & ~(1<<5) ) | ((status & 0x01) << 5);
}

void tempo_250ms()
{
	volatile uint32_t duree;
	for (duree = 0; duree < 2800000; duree++);  
}	

//interuption
void __attribute__((interrupt))SysTick_Hander(){
	led_pa5(1);
	tempo_250ms();
	led_pa5(0);
	tempo_250ms();
	led_pa5(1);
	tempo_250ms();
	led_pa5(0);
}

void button_init() {
	/* Configure PB3-PB6, PB8 as inputs, no pull */
	GPIOB.MODER = GPIOB.MODER & ~(0xFF<<6 & 3<<16);
	GPIOB.PUPDR = GPIOB.PUPDR & ~(0xFF<<6 & 3<<16);
	
	GPIOC.MODER = GPIOC.MODER & ~(3<<26);
}

uint32_t button_poll() {
  return (~(GPIOC.IDR>>13) & 1); /* 1 if pressed */
}
