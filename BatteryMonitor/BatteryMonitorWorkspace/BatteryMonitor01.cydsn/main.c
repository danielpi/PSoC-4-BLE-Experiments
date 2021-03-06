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
#include <iprintf.h>
/* ADC SAR sequencer component header to access Vref value */
#include <ADC_SAR_SEQ.h>
#include <BLE_custom.h>

// Macros and Definitions
#define RED_INDEX						(0)
#define GREEN_INDEX						(1)
#define BLUE_INDEX						(2)
#define INTENSITY_INDEX					(3)

#define TRUE							(1)
#define FALSE							(0)
#define ZERO							(0)

#define RGB_LED_MAX_VAL					(255)
#define RGB_LED_OFF						(255)
#define RGB_LED_ON						(0)

#define RGB_LED_SERVICE_INDEX           (0x02)
#define RGB_LED_CHAR_INDEX              (0x00)
#define RGB_LED_CHAR_HANDLE				(0x0018u)
#define RGB_CHAR_DATA_LEN				(4)

#define BLE_STATE_ADVERTISING			(0x01)
#define BLE_STATE_CONNECTED				(0x02)
#define BLE_STATE_DISCONNECTED			(0x00)

#define PASSIVE_LED_STATUS				(0xFF)

#define LED_ADV_BLINK_PERIOD			(40000)
#define LED_CONN_ON_PERIOD				(145000)

#define MTU_XCHANGE_DATA_LEN			(0x0020)

#define VOLTAGE_MEASUREMENT_CHAR_HANDLE (0x0013)
#define RAW_ADC_CHAR_HANDLE             (0x000E)

// ADC defines
#define CH0_N               (0x00u)
#define TEMP_CH             (0x01u)
#define DELAY_1SEC          (1000u)

/* Get actual Vref. value from ADC SAR sequencer */
#define ADC_VREF_VALUE_V    ((float)ADC_SAR_Seq_DEFAULT_VREF_MV_VALUE/1000.0)

volatile uint32 dataReady = 0u;
volatile uint16 rawADCValue;
volatile uint8 rawADCValuesIndex = 0;
volatile uint16 rawADCValues[16];
volatile float32 voltageReading;
volatile int16 elapsed;
volatile uint32 timer_delay = 0u;

// Function Prototypes
static void initializeSystem(void);
void CustomEventHandler(uint32 event, void * eventParam);
void UpdateRGBled(void);
void SendVoltageMeasurementNotification(float32 voltageData);
void SendRawADCNotification(uint16 rawADC);

// Global Variables
CYBLE_GATT_HANDLE_VALUE_PAIR_T rgbHandle;	
uint8 RGBledData[RGB_CHAR_DATA_LEN];
uint8 deviceConnected = FALSE;


CY_ISR(ADC_SAR_Seq_ISR_LOC) {
    uint32 intr_status;
    uint32 range_status;
    
    TP0_Write(1u);
    /* Read interrupt status registers */
    intr_status = ADC_SAR_Seq_SAR_INTR_MASKED_REG;
    /* Check for End of Scan interrupt */
    if((intr_status & ADC_SAR_Seq_EOS_MASK) != 0u)
    {
        /* Read range detect status */
        range_status = ADC_SAR_Seq_SAR_RANGE_INTR_MASKED_REG;
        /* Verify that the conversion result met the condition Low_Limit <= Result < High_Limit  */
        //if((range_status & (uint32)(1ul << CH0_N)) != 0u) {
            /* Read conversion result */
            rawADCValue = ADC_SAR_Seq_GetResult16(CH0_N);
            
            rawADCValuesIndex++;
            rawADCValues[rawADCValuesIndex & 15] = rawADCValue;
            //voltageReading = 28.4971707317073 * ((float)rawADCValue/0x7FF);
            //SendVoltageMeasurementNotification(ADC_SAR_Seq_GetResult16(CH0_N));
            /* Set PWM compare from channel0 */
            //PWM_WriteCompare(result[CH0_N]);
        //}    
        /* Clear range detect status */
        ADC_SAR_Seq_SAR_RANGE_INTR_REG = range_status;
        dataReady |= ADC_SAR_Seq_EOS_MASK;
    }    

    /* Clear handled interrupt */
    ADC_SAR_Seq_SAR_INTR_REG = intr_status;
    TP0_Write(0u);
}

// Once a second or so I'd like to change the data in the advertising packet so that it contains the 
// up to date voltage reading. CyBle_GapUpdateAdvData()


void initializeSystem(void) {
    CyGlobalIntEnable;
    
    UART_Start(); 
    
    PrISM_1_Start();
    PrISM_2_Start();
    
    Opamp_1_Start();
    
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
    
    // Start the ADC
    ADC_SAR_Seq_Start();
    ADC_SAR_Seq_StartConvert();
    ADC_SAR_Seq_IRQ_Enable();
    // Enable an interupt for when the ADC has data
    ADC_SAR_Seq_IRQ_StartEx(ADC_SAR_Seq_ISR_LOC);
    
    // Start the Timer for the ADC
    ADC_Timer_Start();
    
    elapsed = 0;
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
			UpdateRGBled();

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
            //if(wrReqParam->handleValPair.attrHandle == cyBle_customs[RGB_LED_SERVICE_INDEX].\
			//					customServiceInfo[RGB_LED_CHAR_INDEX].customServiceCharHandle)
            if(wrReqParam->handleValPair.attrHandle == cyBle_customs[CYBLE_RGB_LED_SERVICE_SERVICE_INDEX].\
								customServiceInfo[CYBLE_RGB_LED_SERVICE_RGB_LED_CHARACTERISTIC_CHAR_INDEX].customServiceCharHandle)
            {
                /* ADD_CODE to extract the value of the attribute from 
                 * the handle-value pair database. */
                RGBledData[RED_INDEX] = wrReqParam->handleValPair.value.val[RED_INDEX];
                RGBledData[GREEN_INDEX] = wrReqParam->handleValPair.value.val[GREEN_INDEX];
                RGBledData[BLUE_INDEX] = wrReqParam->handleValPair.value.val[BLUE_INDEX];
                RGBledData[INTENSITY_INDEX] = wrReqParam->handleValPair.value.val[INTENSITY_INDEX];
                
                /* Update the PrISM components and the attribute for RGB LED read 
                 * characteristics */
                UpdateRGBled();
                //SendVoltageMeasurementNotification(voltageReading);
            }
            if(wrReqParam->handleValPair.attrHandle == 0x2A00) {
                RGBledData[RED_INDEX] = 0;
                RGBledData[GREEN_INDEX] = 0;
                RGBledData[BLUE_INDEX] = 0;
                RGBledData[INTENSITY_INDEX] = 0;
                
                /* Update the PrISM components and the attribute for RGB LED read 
                 * characteristics */
                UpdateRGBled();
            }
            //printf("%d",wrReqParam->handleValPair.attrHandle);
			
			/* ADD_CODE to send the response to the write request received. */
			CyBle_GattsWriteRsp(cyBle_connHandle);
			break;
        case CYBLE_EVT_GATTS_READ_CHAR_VAL_ACCESS_REQ:
            // Is this in response to a read request?
            
            break;
            
        default:
       	 	break;
    }
}


// Handle Interrupt Service Routine. Source - ADC SAR Seq.


void UpdateRGBled(void)
{
	/* Local variables to calculate the color components from RGB received data*/
	uint8 debug_red;
	uint8 debug_green;
	uint8 debug_blue;
	uint8 intensity_divide_value = RGBledData[INTENSITY_INDEX];
	
	debug_red = (uint8)(((uint16)RGBledData[RED_INDEX] * intensity_divide_value) / 255);
	debug_green = (uint8)(((uint16)RGBledData[GREEN_INDEX] * intensity_divide_value) / 255);
	debug_blue = (uint8)(((uint16)RGBledData[BLUE_INDEX] * intensity_divide_value) / 255);
	
	/* Update the density value of the PrISM module for color control*/
	PrISM_1_WritePulse0(RGB_LED_MAX_VAL - debug_red);
    PrISM_1_WritePulse1(RGB_LED_MAX_VAL - debug_green);
    PrISM_2_WritePulse0(RGB_LED_MAX_VAL - debug_blue);
	
	/* Update RGB control handle with new values */
	rgbHandle.attrHandle = CYBLE_RGB_LED_SERVICE_RGB_LED_CHARACTERISTIC_CHAR_HANDLE;
	rgbHandle.value.val = RGBledData;
	rgbHandle.value.len = RGB_CHAR_DATA_LEN;
	rgbHandle.value.actualLen = RGB_CHAR_DATA_LEN;
	
	/* Send updated RGB control handle as attribute for read by central device */
	CyBle_GattsWriteAttributeValue(&rgbHandle, FALSE, &cyBle_connHandle, FALSE);  
}

void SendVoltageMeasurementNotification(float32 voltageData) {
	CYBLE_GATTS_HANDLE_VALUE_NTF_T		VoltageMeasurementNotificationHandle;	
	
	VoltageMeasurementNotificationHandle.attrHandle = CYBLE_VOLTAGE_MEASUREMENT_SERVICE_VOLTAGE_MEASUREMENT_CHARACTERISTIC_CHAR_HANDLE;				
	VoltageMeasurementNotificationHandle.value.val = &voltageData;
	VoltageMeasurementNotificationHandle.value.len = 4;
	
	/* Send notifications. */
	CyBle_GattsNotification(cyBle_connHandle, &VoltageMeasurementNotificationHandle);
}

void SendRawADCNotification(uint16 rawADC) {
    CYBLE_GATTS_HANDLE_VALUE_NTF_T  RawADCNotificationHandle;
    
    RawADCNotificationHandle.attrHandle = CYBLE_RAW_ADC_SERVICE_RAW_ADC_CHARACTERISTIC_CHAR_HANDLE;
    RawADCNotificationHandle.value.val = &rawADC;
    RawADCNotificationHandle.value.len = 2u;
    
    CyBle_GattsNotification(cyBle_connHandle, &RawADCNotificationHandle);
}

int main()
{
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    //printf("Restarted\n");
    float resistor_divider_multiplier;
    float adc_multiplier;
    float fudge_multiplier;
    uint16 offset;
    
    uint32 value;
    int i;
    
    offset = 402;
    resistor_divider_multiplier = ((220000 + 18000) / 18000);
    adc_multiplier = 2.048 / 65536;
    fudge_multiplier = 1.02534275031159;
    
    initializeSystem();
    
    for(;;) {
        elapsed = elapsed + 1;
        UART_UartPutChar(72u);//H
        UART_UartPutChar(101u);//e
        UART_UartPutChar(108u);//l
        UART_UartPutChar(108u);//l
        UART_UartPutChar(111u);//o
        iprintf(", World! %d", elapsed);
        UART_UartPutChar(13u);//CR
        UART_UartPutChar(10u);//LF
        
        
        TP1_Write(1u);
        /* Place your application code here. */
        CyBle_ProcessEvents();
        TP1_Write(0u);
        
        if (elapsed == 0) {
            //voltageReading = 29.6907207207207 * ((float)rawADCValue/0x7FFF);
            //voltageReading = 0.0009 * (float)rawADCValue + 0.4238;
            //voltageReading = 0.0005892 * (float)rawADCValue + 0.28026;
            
            value = 0;
            for (i = 0; i < 16; i++) {
                value += rawADCValues[i];
            }
            value = value / 16;
            voltageReading = fudge_multiplier * resistor_divider_multiplier * adc_multiplier * (float)(value - offset);
            SendVoltageMeasurementNotification(voltageReading);
            SendRawADCNotification(rawADCValue);
            //printf("Voltage: %f\n", voltageReading);
        }
    }
}

/* [] END OF FILE */
