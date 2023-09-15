#include <gpiod.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>

#include "include/gpioline.h"
#include "include/output.h"

#ifndef CONSUMER
#define CONSUMER "Consumer"
#endif

bool updateTick(struct GPIOLine *gpioLine)
{
  struct timespec ts = {1, 0};
  struct gpiod_line_event event;
  int ret = gpiod_line_event_wait(gpioLine->line, &ts);

  if (ret < 0)
  {
    perror("Wait event notification failed\n");
    gpioLine->state = GPIO_ERR;
    return false;
  }
  else if (ret == 0)
  {
    printWaitError(gpioLine->num);
    gpioLine->state = GPIO_WAIT;
    return true;
  }

  ret = gpiod_line_event_read(gpioLine->line, &event);
  if (ret < 0)
  {
    perror("Read last event notification failed\n");
    gpioLine->state = GPIO_ERR;
    return false;
  }

  gpioLine->state = GPIO_OK;
  gpioLine->last_tick = gpioLine->tick;
  gpioLine->tick = event.ts.tv_sec * 1000000 + event.ts.tv_nsec / 1000;

  return true;
}

struct GPIOLine *createGPIOLine(int num, struct gpiod_chip *chip)
{
  struct GPIOLine *gpioLine = malloc(sizeof(struct GPIOLine));
  gpioLine->num = num;
  gpioLine->last_tick = 0;
  gpioLine->tick = 0;
  gpioLine->state = GPIO_OK;

  gpioLine->line = gpiod_chip_get_line(chip, gpioLine->num);
  if (!gpioLine->line)
  {
    gpioLine->state = GPIO_ERR;
    perror("Get line failed\n");
  }
  return gpioLine;
}

struct GPIOLine *destroyGPIOLine(struct GPIOLine *gpioLine)
{
  if (gpioLine->line)
    gpiod_line_release(gpioLine->line);
  free(gpioLine);
  return gpioLine;
}

void requestEvents(struct GPIOLine *gpioLine)
{
  if (gpiod_line_request_both_edges_events(gpioLine->line, CONSUMER) < 0)
  {
    gpioLine->state = GPIO_ERR;
    perror("Request event notification failed\n");
  }
}

bool isGPIOErr(struct GPIOLine *gpioLine)
{
  return gpioLine->state == GPIO_ERR;
}

bool isGPIOWaiting(struct GPIOLine *gpioLine)
{
  return gpioLine->state == GPIO_WAIT;
}

int getLastWidth(struct GPIOLine *gpioLine)
{
  return gpioLine->tick - gpioLine->last_tick;
}
