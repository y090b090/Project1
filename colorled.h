

#ifndef _COLOR_LED_DRV_H_
#define _COLOR_LED_DRV_H_

int pwmActiveAll(void);
int pwmInactiveAll(void);
int pwmSetDuty(int dutyCycle, int pwmIndex);
int pwmSetPeriod(int Period, int pwmIndex);
int pwmSetPercent(int percent, int ledColor);
int pwmStartAll(void);
int pwmLedInit(void);
int pwmLedOn(int argc, char *argv[]);

#endif //COLOR_LED_DRV_H_
