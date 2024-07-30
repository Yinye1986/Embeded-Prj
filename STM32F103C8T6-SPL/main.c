#include "stm32f10x.h"


#define LED0_PORT 			GPIOC  
#define LED0_PIN 			GPIO_Pin_13
#define LED0_PORT_RCC		RCC_APB2Periph_GPIOC	

void LED_Init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(LED0_PORT_RCC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=LED0_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(LED0_PORT,&GPIO_InitStructure);
	GPIO_SetBits(LED0_PORT,LED0_PIN);
}

int main() {
	LED_Init();
	GPIO_ResetBits(LED0_PORT,LED0_PIN);
	while(1) {

	}
}

