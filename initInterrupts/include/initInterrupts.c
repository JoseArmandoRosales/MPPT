// Included Files

#include "initInterrupts.h"

#include "math.h"
#include "sysctl.h"
#include "driverlib.h"
#include "device.h"
#include "board.h"
#include <stdint.h>


void initEPWMCInterrupts()
{
      // ADC
        // Disable SOC to Configure the SOC
        EPWM_disableADCTrigger(EPWM5_BASE, EPWM_SOC_A);
        // SOCA generated when Time-base counter equals CMPA (To synchronize when the PWMC waveform value is 1)
        EPWM_setADCTriggerSource(EPWM5_BASE, EPWM_SOC_A, EPWM_SOC_TBCTR_U_CMPA);
        // Sets the SOC event count that determines the number of Events that have to occur before an SOC is issued.
        EPWM_setADCTriggerEventPrescale(EPWM5_BASE, EPWM_SOC_A, 1);

      // PWM
        // Interrupt where we will change the Compare Values. Interrupt generated when Time-base counter equal to zero
        EPWM_setInterruptSource(EPWM5_BASE, EPWM_INT_TBCTR_PERIOD); // To synchronize when the PWMC waveform value is 1
        EPWM_enableInterrupt(EPWM5_BASE);                           // Select INT on Time base counter zero event,
        EPWM_setInterruptEventCount(EPWM5_BASE, 1U);               // Enable INT, generate INT every 1 events (0,00025 seg)
}


void initADCSOC(void)
{
        uint16_t acqps;

    // Determine minimum acquisition window (in SYSCLKS) based on resolution
    // Sample window must be at least 1 ADCCLK
    // ADCCLK derived from PERx.SYSCLK

        acqps = 14; // (1/200MHz)*14+1 = 75ns

    // SOCs need not use the same S+H window duration, but SOCs occurring in parallel should usually
    // use the same value to ensure simultaneous samples and synchronous operation.

    // Select the channels to convert and the configure the ePWM trigger
    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_EPWM5_SOCA, ADC_CH_ADCIN1, acqps);
    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER1, ADC_TRIGGER_EPWM5_SOCA, ADC_CH_ADCIN4, acqps);

    // Select SOC2 on ADCA as the interrupt source.  SOC2 on ADCD will end at
    // the same time, so either SOC2 would be an acceptable interrupt trigger.
    ADC_setInterruptSource(ADCA_BASE, ADC_INT_NUMBER1, ADC_SOC_NUMBER1);
    ADC_enableInterrupt(ADCA_BASE, ADC_INT_NUMBER1);
    ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);
}
