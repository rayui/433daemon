#include <gpiod.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define GPIO_OK 0
#define GPIO_ERR 1
#define GPIO_WAIT 2

struct GPIOLine
{
  int num;
  int state;
  uint64_t last_tick;
  uint64_t tick;
  struct gpiod_line *line;
};

struct GPIOLine *createGPIOLine(int num, struct gpiod_chip *chip);
struct GPIOLine *destroyGPIOLine(struct GPIOLine *gpioLine);
bool updateTick(struct GPIOLine *gpioLine);
void setLastTick(struct GPIOLine *gpioLine);
void requestEvents(struct GPIOLine *gpioLine);
void getTick(struct GPIOLine *gpioLine);
bool isGPIOErr(struct GPIOLine *gpioLine);
bool isGPIOWaiting(struct GPIOLine *gpioLine);
unsigned int getLastWidth(struct GPIOLine *gpioLine);
