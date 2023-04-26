#include "mppt.h"

// MPPT Variables
int e = 1;        // Threshold (Need Tuning)
int slope = 1;    // Direction
int m = 3;        // Constant (Need Tuning)
float step = 0;   // StepSize
float vRef = 50;  // Vref
float currentPower  = 0;
float previousPower = 0;
float powerDifference = 0;

// PI Variables
float controlOut = 0;
float kp = 0.01;
float ki = 0.69;
float error = 0;
float errorPrev = 0;
float proportional = 0;
float integral = 0;
float ts = 1/4000;
float maxOut = 0.8;
float minOut = 0.01;



void getMPPT(float realValueA, float realValueB)
{
    currentPower  = realValueA*realValueB;
    if(currentPower  != previousPower)
    {
        powerDifference = currentPower  - previousPower;

                if(powerDifference/step > e){
                    if(powerDifference>0){

                        slope = 1;

                    } else{
                        slope = -1;
                    }
                } else{
                    if(powerDifference<0){
                        slope = slope*-1;
                    }
                }
                step = m*(powerDifference/step);
                vRef = vRef+(step*slope);
                previousPower = currentPower;
    }
 }

float piController(float vRef, float realValueA)
{
    error = vRef - realValueA;
    proportional = kp*error;
    integral = integral + ((ki*.5*ts)*(error + errorPrev));
    controlOut = proportional+integral;

    errorPrev = error;

    if(controlOut > maxOut){
        controlOut = maxOut;
    }
    if(controlOut < minOut){
        controlOut = minOut;
    }
    return controlOut;
}

