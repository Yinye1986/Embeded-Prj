#include <wiringPi.h>
#include <stdint.h>
#include <stdio.h>
#include "MPU6050.h"

uint8_t ID;                     // 定义用于存放ID号的变量
int16_t AX, AY, AZ, GX, GY, GZ; // 定义用于存放各个数据的变量
                                //
int main() {
  if (wiringPiSetup() == -1) {
    printf("Setup wiringPi failed!\n");
    return 1;
  }

  MPU6050_iicInit(); // MPU6050初始化
  MPU6050_regInit(); // MPU6050初始化

  ID = MPU6050_getID();         // 获取MPU6050的ID号
  printf("DevID: %u\n",ID);

  while (1) {
    MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ); // 获取MPU6050的数据
    printf("%int16_t", AX);               // OLED显示数据
    printf("%int16_t", AY);
    printf("%int16_t", AZ);
    printf("%int16_t", GX);
    printf("%int16_t", GY);
    printf("%int16_t", GZ);
  }
}
