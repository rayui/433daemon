#include <gpiod.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#include "include/parser.h"
#include "include/gpioline.h"

int main(int argc, char **argv)
{
	char *chipname = "gpiochip0";
	unsigned int line_num = 27;	// GPIO Pin #25
	struct gpiod_chip *chip;
  struct GPIOLine *gpioLine;
  struct Parser *parser;
  
  parser = createParser(0, 24, SHORT_PULSE_MIN, LONG_PULSE_MIN, LONG_PULSE_MAX);
	
  chip = gpiod_chip_open_by_name(chipname);
	if (!chip) {
		perror("Open chip failed\n");
		goto end;
	}

  gpioLine = createGPIOLine(line_num, chip);
  if (isGPIOErr(gpioLine)) {
    destroyGPIOLine(gpioLine);
    goto close_chip;
  }

  requestEvents(gpioLine); 
  if (isGPIOErr(gpioLine)) {
    destroyGPIOLine(gpioLine);
    goto close_chip;
  }

	while (true) {
    updateTick(gpioLine);

    if (isGPIOErr(gpioLine)) {
      destroyGPIOLine(gpioLine);
      goto close_chip;
    } else if (isGPIOWaiting(gpioLine)) {
      continue;
    } else {
      decodePulse(parser, getLastWidth(gpioLine));
    }
	}

close_chip:
	gpiod_chip_close(chip);
end:
  destroyParser(parser);
	return -1;
}
