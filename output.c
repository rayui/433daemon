#include <gpiod.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "include/output.h"

void printBinary(unsigned long long data, int len) {
  printf(BYTE_TO_BINARY_PATTERN" "BYTE_TO_BINARY_PATTERN" "BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(data & 0xFF), BYTE_TO_BINARY((data >> 8) &0xFF), BYTE_TO_BINARY((data >> 16) & 0xFF));
  fflush(stdout);
}

void printHex(unsigned long long data, int len) {
  int i = 0;
  while (i < len - 7) {
    printf("%02x", (data >> i) & 0xFF);
    i += 8;
  }
  printf("\n");
  fflush(stdout);
}
