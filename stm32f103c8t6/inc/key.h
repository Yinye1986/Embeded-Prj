#ifndef __KEY_H__
#define __KEY_H__

#include "stm32f10x.h"
#include "OLED.h"

#define DEBOUNCE_DELAY 10  // 20ms去抖动时间
#define NUM_BUTTONS 6

typedef struct {
    GPIO_TypeDef* port;
    uint16_t pin;
    uint8_t stableState;
    uint8_t currentState;
    uint8_t debounceCounter;
} Button;

void GPIO_Init_Buttons(void);
void TIM2_Init(void);
void TIM2_IRQHandler(void);

extern void (*ButtonPressedHandlerPOINTER)(int);

#endif

