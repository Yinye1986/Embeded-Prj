#ifndef __SERIALAA_H
#define __SERIALAA_H

#include "stm32f10x.h"


extern uint8_t RxBuffer[];
extern uint8_t RxCounter;

void USART1_IRQHandler(void);
void USART1_Init(uint32_t baudrate);

#endif
