#include <gpiod.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "include/output.h"

void printBinary(char *data, unsigned int len)
{
  unsigned int i = 0;
  unsigned int pos = 0;
  unsigned int res = 0;

  while (pos < len)
  {
    i = 0;
    res = 0;
    while (i < 8)
    {
      res += (data[pos] << i);
      i++;
      pos++;
    }
    printf("%02x", res);
  }
  printf("\n");
  fflush(stdout);
}

void printHex(char *data, unsigned int len_bits)
{
  unsigned int pos = 0;
  unsigned int len = (len_bits + 7) >> 3;

  while (pos < len)
  {
    printf("%02x", data[pos]);
    pos++;
  }
  printf("\n");
  fflush(stdout);
}

void printWaitError(unsigned int num)
{
  printf("Wait event notification on line #%u timeout\n", num);
  fflush(stdout);
}
