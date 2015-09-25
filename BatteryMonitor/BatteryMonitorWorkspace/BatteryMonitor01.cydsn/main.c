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

// Function Prototypes
static void initializeSystem(void);


void initializeSystem(void) {
    CyGlobalIntEnable;
    
    PrISM_1_Start();
    PrISM_2_Start();
    
    // Start the Bluetooth Stack
    CyBle_Start(CustomEventHandler);	
    
    // Set up the LED. First set its output to be off so that the LED doesn't blink on.
    // Then set the drive mode to strong.
	PrISM_1_WritePulse0(255);
	PrISM_1_WritePulse1(255);
	PrISM_2_WritePulse0(255);
	
	RED_SetDriveMode(RED_DM_STRONG);
	GREEN_SetDriveMode(GREEN_DM_STRONG);
	BLUE_SetDriveMode(BLUE_DM_STRONG);
}

void CustomEventHandler(uint32 event, void * eventParam)
{
	CYBLE_GATTS_WRITE_REQ_PARAM_T *wrReqParam;
   
    switch(event)
    {
        case CYBLE_EVT_STACK_ON:
        case CYBLE_EVT_GAP_DEVICE_DISCONNECTED:
			/* Start Advertisement and enter Discoverable mode*/
			CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
			break;
			
		case CYBLE_EVT_GAPP_ADVERTISEMENT_START_STOP:
			/* Set the BLE state variable to control LED status */
            if(CYBLE_STATE_DISCONNECTED == CyBle_GetState())
            {
                /* Start Advertisement and enter Discoverable mode*/
                CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
            }
			break;
			
        case CYBLE_EVT_GATT_CONNECT_IND:
			/* This flag is used in application to check connection status */
			deviceConnected = TRUE;
			break;
        
        case CYBLE_EVT_GATT_DISCONNECT_IND:
			/* Update deviceConnected flag*/
			deviceConnected = FALSE;
			
			/* Reset CapSense notification flag to prevent further notifications
			 * being sent to Central device after next connection. */
			//sendCapSenseSliderNotifications = FALSE;
			
			/* Reset the CCCD value to disable notifications */
			//updateNotificationCCCAttribute = TRUE;
			
			/* Reset the color coordinates */
			RGBledData[RED_INDEX] = 0;
            RGBledData[GREEN_INDEX] = 0;
            RGBledData[BLUE_INDEX] = 0;
            RGBledData[INTENSITY_INDEX] = 0;
			//UpdateRGBled();

			break;
        
            
        case CYBLE_EVT_GATTS_WRITE_REQ: 							
            /* This event is received when Central device sends a Write command 
             * on an Attribute. 
             * We first get the attribute handle from the event parameter and 
             * then try to match that handle with an attribute in the database.
             */
            wrReqParam = (CYBLE_GATTS_WRITE_REQ_PARAM_T *) eventParam;
            

            /* This condition checks whether the RGB LED characteristic was
             * written to by matching the attribute handle.
             * If the attribute handle matches, then the value written to the 
             * attribute is extracted and used to drive RGB LED.
             */
            
            /* ADD_CODE to extract the attribute handle for the RGB LED 
             * characteristic from the custom service data structure.
             */
            if(wrReqParam->handleValPair.attrHandle == cyBle_customs[RGB_LED_SERVICE_INDEX].\
								customServiceInfo[RGB_LED_CHAR_INDEX].customServiceCharHandle)
            {
                /* ADD_CODE to extract the value of the attribute from 
                 * the handle-value pair database. */
                RGBledData[RED_INDEX] = wrReqParam->handleValPair.value.val[RED_INDEX];
                RGBledData[GREEN_INDEX] = wrReqParam->handleValPair.value.val[GREEN_INDEX];
                RGBledData[BLUE_INDEX] = wrReqParam->handleValPair.value.val[BLUE_INDEX];
                RGBledData[INTENSITY_INDEX] = wrReqParam->handleValPair.value.val[INTENSITY_INDEX];
                
                /* Update the PrISM components and the attribute for RGB LED read 
                 * characteristics */
                //UpdateRGBled();
            }

            
            /* This condition checks whether the CCCD descriptor for CapSense
             * slider characteristic has been written to. This tells us whether
             * the notifications for CapSense slider have been enabled/disabled.
             */
            if(wrReqParam->handleValPair.attrHandle == cyBle_customs[CAPSENSE_SERVICE_INDEX].\
				customServiceInfo[CAPSENSE_SLIDER_CHAR_INDEX].customServiceCharDescriptors[CAPSENSE_SLIDER_CCC_INDEX])
            {
                sendCapSenseSliderNotifications = wrReqParam->handleValPair.value.val[CCC_DATA_INDEX];
				
				/* Set flag to allow CCCD to be updated for next read operation */
				updateNotificationCCCAttribute = TRUE;
            }

			
			/* ADD_CODE to send the response to the write request received. */
			CyBle_GattsWriteRsp(cyBle_connHandle);
			
			break;

        default:

       	 	break;
    }
}


int main()
{
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    initializeSystem()
    
    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
