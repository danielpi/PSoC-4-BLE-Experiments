/* ========================================
 *
 * Copyright Electronic Innovations, 2015
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>
//#include "ias.h"
//#include "common.h"

#define NO_ALERT           (0u)
#define MILD_ALERT         (1u)
#define HIGH_ALERT         (2u)

#define NO_ALERT_COMPARE   (0u)
#define MILD_ALERT_COMPARE (250u)
#define HIGH_ALERT_COMPARE (500u)

// Function Prototypes
void StackEventHandler(uint32 eventCode, void *eventParam);
void IasEventHandler(uint32 eventCode, void *eventParam);
void HandleAlertLEDs(uint8 status);


int main()
{
    CyGlobalIntEnable;   /* Enable global interrupts */
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    

    CyBle_Start( StackEventHandler );
    PWM_1_Start();
    PWM_2_Start();
    PWM_3_Start();
    CyBle_IasRegisterAttrCallback(IasEventHandler);
    
    while(1)
    {
        /* Place your application code here */
        CyBle_ProcessEvents();
    }
}

void StackEventHandler( uint32 eventCode, void *eventParam )
{
    switch( eventCode )
    {
        case CYBLE_EVT_STACK_ON:
        case CYBLE_EVT_GAP_DEVICE_DISCONNECTED:
            CyBle_GappStartAdvertisement( CYBLE_ADVERTISING_FAST );
            break;

        default:
            break;
    }
}

void IasEventHandler(uint32 eventCode, void *eventParam)
{
    uint8 alertLevel;
    
    switch(eventCode) {
        case CYBLE_EVT_IASS_WRITE_CHAR_CMD:
            // Extract the alert level
            CyBle_IassGetCharacteristicValue(CYBLE_IAS_ALERT_LEVEL, sizeof(alertLevel), &alertLevel);
            HandleAlertLEDs(alertLevel);
            break;
        default:
            break;
    }
}

void HandleAlertLEDs(uint8 status) {
    switch(status) 
    {
        case NO_ALERT:
            PWM_1_WriteCompare(NO_ALERT_COMPARE);
            PWM_2_WriteCompare(NO_ALERT_COMPARE);
            PWM_3_WriteCompare(NO_ALERT_COMPARE);
            break;
            
        case MILD_ALERT:
            PWM_1_WriteCompare(203);
            PWM_2_WriteCompare(192u);
            PWM_3_WriteCompare(255u);
            break;
            
        case HIGH_ALERT:
            PWM_1_WriteCompare(60u);
            PWM_2_WriteCompare(20u);
            PWM_3_WriteCompare(220u);
            break;                
    }
        
}