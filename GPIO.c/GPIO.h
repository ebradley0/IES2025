#include <msp430.h>
#include <stdbool.h>
void init_GPIO();
void setPilot(bool open);
int checkCallForHeat();
void Igniter(bool on);