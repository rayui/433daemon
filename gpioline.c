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
  struct gpiod_line_event event;
  
  int ret = gpiod_line_event_read(gpioLine->line, &event);
  if (ret < 0)
  {
    perror("Read last event notification failed\n");
    gpioLine->state = GPIO_ERR;
    return false;
  }

  if (ret > 0) printf("%d\n", ret);  
  gpioLine->tick = event.ts.tv_sec * 1000000000 + event.ts.tv_nsec;

  return true;
}

bool isEventReady(struct GPIOLine *gpioLine)
{
  struct timespec ts = {10, 0};
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
    return false;
  }

  gpioLine->state = GPIO_OK;
  return true;
}

void setLastTick(struct GPIOLine *gpioLine)
{
  gpioLine->last_tick = gpioLine->tick;
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

unsigned int getLastWidth(struct GPIOLine *gpioLine)
{
  return gpioLine->tick - gpioLine->last_tick;
}
