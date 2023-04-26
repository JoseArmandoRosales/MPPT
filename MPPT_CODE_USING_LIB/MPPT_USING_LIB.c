// Included Files
 #include "stdio.h"  // printf, puts, etc.
#include "stdlib.h" // rand?
#include "math.h"
#include "sysctl.h"
#include "driverlib.h"
#include "device.h"
#include "board.h"
#include "initPheripherals.h"
#include "initInterrupts.h"
#include "mppt.h"


// Function Prototypes
void updateCompare(float);            // Update CMAP value
__interrupt void epwm5ISR(void);     // Calls updateCompare function
__interrupt void adcA1ISR(void);     // Saves the captured value in the ADC
float GetAVG(uint16_t resultbuff[]); // Returns the AVG buffer value



// Main
void main(void)
{
    Device_init();                            // Initialize device clock and peripherals
    Device_initGPIO();                        // Disable pin locks and enable internal pull ups.
    Board_init();                             // Configure GPIO0/1 as ePWM1A/1B pins
    Interrupt_initModule();                   // Initialize PIE and clear PIE registers. Disables CPU interrupts.
    Interrupt_initVectorTable();              // Initialize the PIE vector table with pointers to the shell Interrupt Service Routines (ISR).

    Interrupt_register(INT_ADCA1, &adcA1ISR); // Assign the interrupt service routines to ADC interrupts
    Interrupt_register(INT_EPWM5, &epwm5ISR); // Assign the interrupt service routines to ePWM interrupts

    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);  // Disable sync (Freeze clock to PWM as well)
    configureADC();                                         // Configure ADC A
    initEPWMC();                                            // InitPWM Coordinator
    initEPWM4();                                            // InitPWM
    initADCSOC();                                           // The channels to convert and set the interrupt source
    initEPWMCInterrupts();
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);   // Enable sync and clock to PWM


    Interrupt_enable(INT_EPWM5);                            // Enable ePWM interrupts
    Interrupt_enable(INT_ADCA1);                            // Enable ADC interrupts
    EINT;                                                   // Enable Global Interrupt (INTM)
    ERTM;                                                   // And realtime interrupt (DBGM)
    EPWM_enableADCTrigger(EPWM5_BASE, EPWM_SOC_A);          // Enable the ADC Trigger

    do
    {

    }
    while(1);
}




//Function to update  Duty Cycle
void updateCompare(float controlOut)
{
    EPWM_setCounterCompareValue(EPWM4_BASE, EPWM_COUNTER_COMPARE_A, getCMAP(controlOut));
}

// Function to get Average value
float GetAVG(uint16_t resultbuff[])
{
    float avg = 0;
    for(indexADC=0; indexADC < RESULTS_BUFFER_SIZE ;indexADC++)
    {
        avg= resultbuff[indexADC] + avg;
    }
    return avg / RESULTS_BUFFER_SIZE;
}

// ADC A Interrupt 1 ISR
__interrupt void adcA1ISR(void)
{
    // Store results
    adcAResults[indexADC] = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER1);
    adcBResults[indexADC] = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER0);
    indexADC++;
    if(RESULTS_BUFFER_SIZE <= indexADC)
        {
            avgAvalue= GetAVG(adcAResults);                      // Saves the AVG A value
            avgBvalue= GetAVG(adcBResults);                      // Saves the AVG B value
            indexADC = 0;
            realValueA = (((avgAvalue*3.3)/4095) - 0.002) / 0.0341;
            realValueB = (((avgBvalue*3.3)/4095) - 0.002) / 0.0341;
        }
    getMPPT(realValueA, realValueB);

    // Clear the interrupt flag
    ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);

    // Check if overflow has occurred
    if(true == ADC_getInterruptOverflowStatus(ADCA_BASE, ADC_INT_NUMBER1))
    {
        ADC_clearInterruptOverflowStatus(ADCA_BASE, ADC_INT_NUMBER1);
        ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);
    }

    // Acknowledge the interrupt
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
}

// epwm1ISR - ePWM 1 ISR
__interrupt void epwm5ISR(void)
{
    updateCompare(piController(vRef,realValueA));      // Update the CMPA values
    EPWM_clearEventTriggerInterruptFlag(EPWM5_BASE);   // Clear INT flag
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP3);     // Acknowledge interrupt group
}
