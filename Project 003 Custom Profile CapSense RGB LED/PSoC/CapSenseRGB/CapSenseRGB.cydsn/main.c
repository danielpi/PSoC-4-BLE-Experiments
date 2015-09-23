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
#include <main.h>

#define TRUE 	(1)
#define FALSE 	(0)

uint8 deviceConnected = FALSE;

// Function Prototypes
void initializeSystem();
void bluetoothEventHandler(uint32 event, void * eventParam);


int main()
{
    initializeSystem();

    for(;;)
    {
        /* Place your application code here. */
        CyBle_ProcessEvents();
        
        
    }
}

void initializeSystem() {
	// Init Processor
	CyGlobalIntEnable; // What is this? Not a function call.
	
	
	// Init BLE component
	CyBle_Start(bluetoothEventHandler);
	
	
	// Init LED
	// Start the PrISM components
	RedGreenPrISM_Start();
	BluePrISM_Start();
	
	// The RGB LEDs are active low. Drive HIGH on pin to turn
	// the LED off.
	RedGreenPrISM_WritePulse0(255);
	RedGreenPrISM_WritePulse1(255);
	BluePrISM_WritePulse0(255);
	
	// Set the drive mode of the output pins from HiZ to Strong
	// Is this to stop the LED from blinking on power up???
	RED_SetDriveMode(RED_DM_STRONG);
	GREEN_SetDriveMode(GREEN_DM_STRONG);
	BLUE_SetDriveMode(BLUE_DM_STRONG);
	
	
	// Init CapSense
	Slider_Start();
	Slider_InitializeAllBaselines();
}

void bluetoothEventHandler(uint32 event, void * eventParam) {
    switch(event) {
    	case CYBLE_EVT_STACK_ON:
		case CYBLE_EVT_GAP_DEVICE_DISCONNECTED:
			// Start advertising and enter discoverable mode
			CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
			break;
			
		case CYBLE_EVT_GAPP_ADVERTISEMENT_START_STOP:
			// When advertising stops and we aren't connected to
			// to a computer we want to restart advertising
			if (CYBLE_STATE_DISCONNECTED == CyBle_GetState()) {
				CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
			}
			break;
			
		case CYBLE_EVT_GATT_CONNECT_IND:
			// This flag is used to keep track of the current state
			// of the connection.
			deviceConnected = TRUE;
			break;
			
		case CYBLE_EVT_GATT_DISCONNECT_IND:
			deviceConnected = FALSE;
			
			// Reset the CapSense notification
			// Reset the CCD value
			
			// Reset the color coordinates
			break;
			
    }
}
/* [] END OF FILE */
