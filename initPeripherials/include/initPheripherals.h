#include "sysctl.h"
#include <stdint.h>


#ifndef INCLUDE_INITPHERIPHERALS_H_
#define INCLUDE_INITPHERIPHERALS_H_

// Defines ADC (ADC Resolution 12 bits)
extern int RESULTS_BUFFER_SIZE;         // Buffer Size
extern uint16_t adcAResults[]; // Buffer for results
extern uint16_t adcBResults[]; // Buffer for Results
extern uint16_t indexADC;                         // Index into result buffer
extern uint16_t avgAvalue;                        // AVG buffer value
extern uint16_t avgBvalue;                        // AVG buffer value
extern float realValueA;
extern float realValueB;


// Defines PWM Coordinator EPWM 5 = EPWM5A (Pin 78) & EPWM5B (Pin 77)
extern  int EPWMC_TIMER_TBPRD;          // 4kHz Freq
extern  int coordinatorDutyC;          // Coordinator Duty Cycle


// Defines PWM4 (MPPT) PWM 4 = Pin 80(PWM A) Pin 79(PWM B)
extern int EPWM4_TIMER_TBPRD;          // 40kHz Freq
extern int upDuty;         // Set Duty Cycle
extern int downDuty;          // Set Duty Cycle
extern int indexPWM;
extern uint16_t epwmTimerIntCount;           // Needed to count up to 1 sec


// Function Prototypes
extern float getCMAP(float);
extern void configureADC(void);
extern void initEPWMC(void);
extern void initEPWM4(void);

#endif /* INCLUDE_INITPHERIPHERALS_H_ */
