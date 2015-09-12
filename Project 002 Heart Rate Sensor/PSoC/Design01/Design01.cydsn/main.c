/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>

#define LED_ON                      (0u)
#define LED_OFF                     (1u)

CYBLE_API_RESULT_T apiResult;

/*
On power up we need to start off the BLE service and start advertising the 
heart rate service. A LED blink whenever advertising packets go out would
be good (at the right rate at anyrate).

*/


void StartAdvertisment() {
    apiResult = CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
    if (apiResult != CYBLE_ERROR_OK) {
        printf("CyBle_Start API error: %x \r\n", apiResult);
        Error_Led_Write(LED_ON);
    } else {
        Advertising_Led_Write(LED_ON);
    }
}

void bleCallBack(uint32 event, void* eventParam) {
    switch(event) {
        case CYBLE_EVT_STACK_ON:
        case CYBLE_EVT_GAP_DEVICE_DISCONNECTED:
            StartAdvertisment();
            break;
        case CYBLE_EVT_GAP_DEVICE_CONNECTED:
            Advertising_Led_Write(LED_OFF);
            break;
        default:
            break;
    }
}


int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    Advertising_Led_Write(LED_OFF);
    Error_Led_Write(LED_OFF);
    apiResult = CyBle_Start(bleCallBack);
    if (apiResult != CYBLE_ERROR_OK) {
        printf("CyBle_Start API error: %x \r\n", apiResult);
        Error_Led_Write(LED_ON);
    }
    for(;;)
    {
        /* Place your application code here. */
        CyBle_ProcessEvents();
    }
}

/* [] END OF FILE */
