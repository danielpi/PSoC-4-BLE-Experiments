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
#include <main.h>

#define TRUE 	(1)
#define FALSE 	(0)

uint8 deviceConnected = FALSE;

// Function Prototypes
void initializeSystem();
void bluetoothEventHandler(uint32 event, void * eventParam);

#define RGB_LED_MAX_VAL	(255)

#define RED_INDEX 		(0)
#define GREEN_INDEX 	(1)
#define BLUE_INDEX 		(2)
#define INTENSITY_INDEX (3)
uint8 rgbLedData[4]


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

void UpdateRGBled(void) {
	// Local variables to calculate the color components from RGB received data
	uint8 debug_red;
	uint8 debug_green;
	uint8 debug_blue;
	uint8 intensity_divide_value = rgbLedData[INTENSITY_INDEX];

	debug_red = (uint8)(((uint16)rgbLedData[RED_INDEX] * intensity_divide_value) / 255);
	debug_green = (uint8)(((uint16)rgbLedData[GREEN_INDEX] * intensity_divide_value) / 255);
	debug_blue = (uint8)(((uint16)rgbLedData[BLUE_INDEX] * intensity_divide_value) / 255);

	// Update the density value of the PrISM module for color control
	RedGreenPrISM_WritePulse0(RGB_LED_MAX_VAL - debug_red);
    RedGreenPrISM_WritePulse1(RGB_LED_MAX_VAL - debug_green);
    BluePrISM_WritePulse0(RGB_LED_MAX_VAL - debug_blue);

	// Update RGB control handle with new values
	rgbHandle.attrHandle = RGB_LED_CHAR_HANDLE;
	rgbHandle.value.val = RGBledData;
	rgbHandle.value.len = RGB_CHAR_DATA_LEN;
	rgbHandle.value.actualLen = RGB_CHAR_DATA_LEN;

	// Send updated RGB control handle as attribute for read by central device
	CyBle_GattsWriteAttributeValue(&rgbHandle, FALSE, &cyBle_connHandle, FALSE);
}

/* [] END OF FILE */
