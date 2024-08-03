#include "key.h"

Button buttons[NUM_BUTTONS] = {
    {GPIOA, GPIO_Pin_0, 1, 1, 0},
    {GPIOA, GPIO_Pin_2, 1, 1, 0},
    {GPIOA, GPIO_Pin_4, 1, 1, 0},
    {GPIOA, GPIO_Pin_6, 1, 1, 0},
    {GPIOB, GPIO_Pin_0, 1, 1, 0},
    {GPIOB, GPIO_Pin_10, 1, 1, 0}
};

void GPIO_Init_Buttons(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;

    for (int i = 0; i < NUM_BUTTONS; i++) {
        GPIO_InitStructure.GPIO_Pin = buttons[i].pin;
        GPIO_Init(buttons[i].port, &GPIO_InitStructure);
    }
}

void TIM2_Init(void) {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = 2000 - 1; // 1ms
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1; // 72MHz / 72 = 1MHz
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM2, ENABLE);
}

void TIM2_IRQHandler(void) {
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

        for (int i = 0; i < NUM_BUTTONS; i++) {
            uint8_t newState = GPIO_ReadInputDataBit(buttons[i].port, buttons[i].pin);

            if (newState != buttons[i].currentState) {
                buttons[i].currentState = newState;
                buttons[i].debounceCounter = DEBOUNCE_DELAY;
            } else {
                if (buttons[i].debounceCounter > 0) {
                    buttons[i].debounceCounter--;

                    if (buttons[i].debounceCounter == 0 && buttons[i].stableState != newState) {
                        buttons[i].stableState = newState;
                        if (newState == 0) {
                            ButtonPressedHandlerPOINTER(i);
                        }
                    }
                }
            }
        }
    }
}

