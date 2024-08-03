#include "stm32f10x.h"
#include "OLED.h"
#include "Delay.h"
#include "Serialaa.h"
#include "key.h"

uint8_t RxBuffer[32];
char AVolt[6];
char prevAVolt[6];
char AAmps[6];
char prevAAmps[6];
uint8_t status;

uint8_t hasChanged(char *prestr, char *str) {
    for(uint8_t i=0;i<6;i++) {
        if(str[i] != prestr[i]) {
            return 1;
        }
    }
    return 0;
}

void hextochar(uint8_t qwf, uint8_t arst, char *str) {
    uint16_t u16arg = (RxBuffer[qwf] << 8) | RxBuffer[arst];
    if (u16arg == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }

    // 记录字符位置
    char temp[6]; // 足够存储65535的5位数字和结束符
    int i = 0;

    // 将数字从末尾开始转换为字符
    while (u16arg > 0) {
        temp[i++] = (u16arg % 10) + '0'; // 将最后一位数字转换为字符
        u16arg /= 10; // 去掉最后一位数字
    }

    // 反转字符串
    for (int j = 0; j < i; j++) {
        str[j] = temp[i - j - 1];
    }
    str[i] = '\0'; // 添加字符串结束符
}

void GPIO_IN_FLOATING_Config(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void ButtonPressedHandler(int buttonIndex) {
    switch(buttonIndex) {
        case 0:
            status = 6;
            break;
        case 1:
            status = 5;
            break;
        case 2:
            status = 4;
            break;
        case 3:
            status = 3;
            break;
        case 4:
            status = 2;
            break;
        case 5:
            status = 1;
            break;
    }
}

void (*ButtonPressedHandlerPOINTER)(int) = ButtonPressedHandler;

int main(void) {
    GPIO_IN_FLOATING_Config();

    TIM2_Init();
    TIM2_IRQHandler();
    OLED_Init();
    GPIO_Init_Buttons();
    USART1_Init(9600);
    Delay_ms(25);
    OLED_Clear();
    status=0;
    for(uint8_t i; i<6;i++) {
        AVolt[i]='0';
        prevAVolt[i]='0';
        AAmps[i]='0';
        prevAAmps[i]='0';
    }
	while(1) {
        switch(status) {
            case 1:
                hextochar(0, 1, AVolt);
                hextochar(2, 3, AAmps);
                if (hasChanged(prevAVolt,AVolt)==1) {
                    OLED_ShowChar(3,4,AVolt[0]);
                    OLED_ShowChar(3,5,AVolt[1]);
                    OLED_ShowChar(3,6,AVolt[2]);
                    OLED_ShowChar(3,7,'.');
                    OLED_ShowChar(3,8,AVolt[3]);
                    OLED_ShowChar(3,9,AVolt[4]);

                    OLED_ShowChar(4,4,AAmps[0]);
                    OLED_ShowChar(4,5,'.');
                    OLED_ShowChar(4,6,AAmps[1]);
                    OLED_ShowChar(4,7,AAmps[2]);
                    OLED_ShowChar(4,8,AAmps[3]);
                    OLED_ShowChar(4,9,AAmps[4]);
                    OLED_Clear();
                    OLED_ShowString(2,1,"F:");
                    OLED_ShowString(2,3,"50.02 Hz");
                    OLED_ShowString(3,1,"LV:");
                    OLED_ShowString(4,1,"LC:");

                    for(int i=0;i<6;i++) {
                        prevAVolt[i] = AVolt[i];
                        prevAAmps[i] = AAmps[i];
                    }
                }

                break;
            case 2:
                break;
            case 3:
                break;
            default:
                OLED_ShowString(1,1,"press A to START");
                OLED_ShowString(2,1,"  groupMember:  ");
                OLED_ShowString(3,1,"  BZJ  DLW  HCZ ");
                OLED_ShowString(4,1,"   From  WXIT   ");
                Delay_ms(500);
        }
    }
}


