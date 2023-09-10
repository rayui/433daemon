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
  printf("%02x%02x%02x\n", data & 0xFF, (data >> 8) & 0xFF, (data >> 16) & 0xFF);
  fflush(stdout);
}
