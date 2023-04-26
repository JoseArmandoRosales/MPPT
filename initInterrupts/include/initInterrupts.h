#include "sysctl.h"
#include <stdint.h>

#ifndef INCLUDE_INITINTERRUPTS_H_
#define INCLUDE_INITINTERRUPTS_H_


// Function Prototypes
extern void initADCSOC(void);               // Function to configure SOCs on ADCA
extern void initEPWMCInterrupts(void);      // Configure ADC & PWM Interrupts Triggered by PWMC



#endif /* INCLUDE_INITINTERRUPTS_H_ */
