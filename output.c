#include <gpiod.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "include/output.h"

/*void printBinary(unsigned long long data, int len)
{
  printf(BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN "\n", BYTE_TO_BINARY(data & 0xFF), BYTE_TO_BINARY((data >> 8) & 0xFF), BYTE_TO_BINARY((data >> 16) & 0xFF));
  fflush(stdout);
}
*/

void printBinary(char *data, unsigned int len)
{
  unsigned int i = 0;
  unsigned int pos = 0;
  unsigned int res = 0;

  while (pos < len)
  {
    i = 0;
    res = 0;
    while (i < 8) {
      res += (data[pos] << i);
      i++;
      pos++;
    }
    printf("%02x", res);
  }
  printf("\n");
  fflush(stdout);
}

void printHex(char *data, unsigned int len)
{
  unsigned int pos = 0;

  while (pos < len)
  {
    printf("%02x", data[pos]);
    pos++;
  }
  printf("\n");
  fflush(stdout);
}
