// Included Files
#include "initPheripherals.h"

#include "stdio.h"  // printf, puts, etc.
#include "stdlib.h" // rand?
#include "math.h"
#include "sysctl.h"
#include "driverlib.h"
#include "device.h"
#include "board.h"

// Defines ADC (ADC Resolution 12 bits)
int RESULTS_BUFFER_SIZE     = 5;          // Buffer Size
uint16_t adcAResults[5]; // Buffer for results
uint16_t adcBResults[5]; // Buffer for Results
uint16_t indexADC;                         // Index into result buffer
uint16_t avgAvalue;                        // AVG buffer value
uint16_t avgBvalue;                        // AVG buffer value
float realValueA;
float realValueB;


// Defines PWM Coordinator EPWM 5 = EPWM5A (Pin 78) & EPWM5B (Pin 77)
int EPWMC_TIMER_TBPRD  = 24999;          // 4kHz Freq
int coordinatorDutyC   = 50;          // Coordinator Duty Cycle


// Defines PWM4 (MPPT) PWM 4 = Pin 80(PWM A) Pin 79(PWM B)
int EPWM4_TIMER_TBPRD = 2499;          // 40kHz Freq
int upDuty   = 50;          // Set Duty Cycle
int downDuty = 70;          // Set Duty Cycle
int indexPWM = 0;
uint16_t epwmTimerIntCount = 0U;           // Needed to count up to 1 sec




// Write ADC configurations and power up the ADC
void configureADC()
{
    //The base ADC clock is provided directly by the system clock
    // Set ADCDLK divider to /1
    ADC_setPrescaler(ADCA_BASE, ADC_CLK_DIV_1_0);
    ADC_setMode(ADCA_BASE, ADC_RESOLUTION_12BIT, ADC_MODE_SINGLE_ENDED);

    // Configures the EOC to be generated at the end of the voltage conversion
    ADC_setInterruptPulseMode(ADCA_BASE, ADC_PULSE_END_OF_CONV);

    // Power up the ADC circuitry. Note: Allow at least a 500us delay before sampling after calling this API
    ADC_enableConverter(ADCA_BASE);

    // Delay for 1ms to allow ADC time to power up
    DEVICE_DELAY_US(1000);
}

void initEPWMC()
{
        SysCtl_setEPWMClockDivider(SYSCTL_EPWMCLK_DIV_1); //Force the clock to work at 100MHz
      // Set-up TBCLK
        EPWM_setTimeBasePeriod(EPWM5_BASE, EPWMC_TIMER_TBPRD);         // 4.0kHz (Period 0,00025 seg)
        EPWM_setPhaseShift(EPWM5_BASE, 0U);                            // Phase starts in zero
        EPWM_disablePhaseShiftLoad(EPWM5_BASE);                        // Disable Phase Shift
        EPWM_setTimeBaseCounter(EPWM5_BASE, 0U);                       // Counter starts in zero
        EPWM_setTimeBaseCounterMode(EPWM5_BASE, EPWM_COUNTER_MODE_UP); // Set COUNTER_MODE_UP
        EPWM_setClockPrescaler(EPWM5_BASE, EPWM_CLOCK_DIVIDER_1, EPWM_HSCLOCK_DIVIDER_1);

      // Set Compare values
        EPWM_setCounterCompareValue(EPWM5_BASE, EPWM_COUNTER_COMPARE_A, 12499U);
        EPWM_setCounterCompareValue(EPWM5_BASE, EPWM_COUNTER_COMPARE_B, 12499U);

      // Set shadowing load mode
        EPWM_setCounterCompareShadowLoadMode(EPWM5_BASE, EPWM_COUNTER_COMPARE_A, EPWM_COMP_LOAD_ON_CNTR_PERIOD);
        EPWM_setCounterCompareShadowLoadMode(EPWM5_BASE, EPWM_COUNTER_COMPARE_B, EPWM_COMP_LOAD_ON_CNTR_PERIOD);

      // Set actions
        EPWM_setActionQualifierAction(EPWM5_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_LOW, EPWM_AQ_OUTPUT_ON_TIMEBASE_PERIOD);
        EPWM_setActionQualifierAction(EPWM5_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_HIGH, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
        EPWM_setActionQualifierAction(EPWM5_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_LOW, EPWM_AQ_OUTPUT_ON_TIMEBASE_PERIOD);
        EPWM_setActionQualifierAction(EPWM5_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_HIGH, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);
}


// initEPWM4 - Configure ePWM4
void initEPWM4()
{
  // Set-up TBCLK
    EPWM_setTimeBasePeriod(EPWM4_BASE, EPWM4_TIMER_TBPRD); // 40.0kHz (Period 0,000025 seg)
    EPWM_setPhaseShift(EPWM4_BASE, 0U);                    // Phase starts in zero
    EPWM_setTimeBaseCounter(EPWM4_BASE, 0U);               // Counter starts in zero
    EPWM_setTimeBaseCounterMode(EPWM4_BASE, EPWM_COUNTER_MODE_UP);  // Set COUNTER_MODE_UP
    EPWM_disablePhaseShiftLoad(EPWM4_BASE);
    EPWM_setClockPrescaler(EPWM4_BASE, EPWM_CLOCK_DIVIDER_1, EPWM_HSCLOCK_DIVIDER_1);

  // Set Compare values
    EPWM_setCounterCompareValue(EPWM4_BASE, EPWM_COUNTER_COMPARE_A, 1249U);
    EPWM_setCounterCompareValue(EPWM4_BASE, EPWM_COUNTER_COMPARE_B, 1249U);

  // Set shadowing load mode
    EPWM_setCounterCompareShadowLoadMode(EPWM4_BASE, EPWM_COUNTER_COMPARE_A, EPWM_COMP_LOAD_ON_CNTR_PERIOD);
    EPWM_setCounterCompareShadowLoadMode(EPWM4_BASE, EPWM_COUNTER_COMPARE_B, EPWM_COMP_LOAD_ON_CNTR_PERIOD);

    // Set actions
      EPWM_setActionQualifierAction(EPWM4_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_LOW, EPWM_AQ_OUTPUT_ON_TIMEBASE_PERIOD);
      EPWM_setActionQualifierAction(EPWM4_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_HIGH, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
      EPWM_setActionQualifierAction(EPWM4_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_LOW, EPWM_AQ_OUTPUT_ON_TIMEBASE_PERIOD);
      EPWM_setActionQualifierAction(EPWM4_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_HIGH, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);
}



//Function to get CMPA Value for any DutyCycle
float getCMAP(float a)
{
    float af;
    af  = floor(((1-(a/100))*2500)-1);
    af++;
    return(af);
}

