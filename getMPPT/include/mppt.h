#include <stdint.h>

#ifndef INCLUDE_MPPT_H_
#define INCLUDE_MPPT_H_


//Define MPPT variables
extern int e;         // Threshold (Need Tuning)
extern int slope;     // Direction
extern int m;         // Constant (Need Tuning)
extern float step;    // StepSize
extern float vRef;    // Vref
extern float currentPower;
extern float previousPower;
extern float powerDifference;

// PI Variables
extern float controlOut;
extern float kp;
extern float ki;
extern float error;
extern float errorPrev;
extern float proportional;
extern float integral;
extern float ts;

// Function Prototypes
extern void getMPPT(float realValueA, float realValueB);
extern float piController(float vRef, float realValueA);

#endif
