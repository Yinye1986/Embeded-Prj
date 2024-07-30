#include "ST7796U.h"
#include <stdint.h>
#include <stdio.h>
#include <wiringPi.h>

int main() {
  if (wiringPiSetup() == -1) {
    printf("Setup wiringPi failed!\n");
    return 1;
  }
  LCD_Init();
  while (1) {
  }
}
