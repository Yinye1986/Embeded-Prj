#include <wiringPi.h>
#include <stdint.h>
#include <stdio.h>
#include "SSD1315.h"

int main() {
  if (wiringPiSetup() == -1) {
    printf("Setup wiringPi failed!\n");
    return 1;
  }

  iicInit();
  oledInit();
  while (1) {
      oledShow16x8(0,0,7);

  }
}

