#include "stm32f10x.h"

// 硬件定义
#define BEEP_PIN    GPIO_Pin_12   // 蜂鸣器PB12
#define BEEP_PORT   GPIOB
#define KEY_PIN     GPIO_Pin_0    // 按键PA0
#define KEY_PORT    GPIOA

// 全局变量
volatile uint32_t timer_counter = 0;
volatile uint8_t beep_active = 0;

// 函数声明
void GPIO_Configuration(void);
void TIM2_Configuration(void);

int main(void)
{
    // 系统初始化
    SystemInit();
    
    // 硬件初始化
    GPIO_Configuration();
    TIM2_Configuration();

    while(1)
    {
        // 非阻塞按键检测
        if(GPIO_ReadInputDataBit(KEY_PORT, KEY_PIN) == Bit_RESET) 
        {
            // 简单消抖（约5ms）
            for(volatile uint32_t i=0; i<2000; i++);
            
            if((GPIO_ReadInputDataBit(KEY_PORT, KEY_PIN) == Bit_RESET) && !beep_active)
            {
                // 启动蜂鸣器和定时器
                GPIO_SetBits(BEEP_PORT, BEEP_PIN);
                TIM_Cmd(TIM2, ENABLE);
                beep_active = 1;
            }
        }
    }
}

// GPIO配置函数
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    // 使能GPIO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

    // 配置蜂鸣器（PB12）
    GPIO_InitStruct.GPIO_Pin = BEEP_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BEEP_PORT, &GPIO_InitStruct);
    GPIO_ResetBits(BEEP_PORT, BEEP_PIN);

    // 配置按键（PA0）
    GPIO_InitStruct.GPIO_Pin = KEY_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(KEY_PORT, &GPIO_InitStruct);
}

// TIM2配置函数
void TIM2_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    // 使能TIM2时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // 定时器基础配置
    TIM_InitStruct.TIM_Prescaler = 7200 - 1;     // 72MHz/7200=10KHz
    TIM_InitStruct.TIM_Period = 100 - 1;         // 100分频=1ms
    TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_InitStruct);

    // 中断配置
    NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM2, DISABLE);
}

// TIM2中断服务函数
void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        timer_counter++;
        
        if(timer_counter >= 5000)  // 5000ms=5秒
        {
            GPIO_ResetBits(BEEP_PORT, BEEP_PIN);
            TIM_Cmd(TIM2, DISABLE);
            timer_counter = 0;
            beep_active = 0;
        }
    }
}
