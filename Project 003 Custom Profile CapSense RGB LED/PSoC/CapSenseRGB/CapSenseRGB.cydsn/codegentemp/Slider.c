/*******************************************************************************
* File Name: Slider.c
* Version 2.20
*
* Description:
*  This file provides the source code for scanning APIs for the CapSense CSD 
*  component.
*
* Note:
*
********************************************************************************
* Copyright 2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Slider.h"
#include "Slider_PVT.h"
#include "cypins.h"

#if(0u != Slider_CSHL_API_GENERATE)
	#include "Slider_CSHL.h"
#endif /* (0u != Slider_CSHL_API_GENERATE) */

/* This definition is intended to prevent unexpected linker error. 
   For more details please see the IAR Technical Note 49981 */
#if defined(__ICCARM__)
	extern void Slider_EnableSensor(uint32 sensor);
	extern void Slider_DisableSensor(uint32 sensor);
#endif /* (__ICCARM__) */	

/* SmartSense functions */
#if (Slider_TUNING_METHOD == Slider__TUNING_AUTO)
    uint8 Slider_lowLevelTuningDone = 0u;
	uint8 scanSpeedTbl[((Slider_TOTAL_SENSOR_COUNT - 1u) / 8u) + 1u];
#endif /* (Slider_TUNING_METHOD == Slider__TUNING_AUTO) */

#if(Slider_PRS_OPTIONS != Slider__PRS_NONE)
	uint8 Slider_prescalersTuningDone = 0u;
#endif /* (Slider_PRS_OPTIONS == Slider__PRS_NONE) */
            
/* Global software variables */
volatile uint8 Slider_csdStatusVar = 0u;   /* CapSense CSD status, variable */
volatile uint8 Slider_sensorIndex = 0u;    /* Index of scannig sensor */

/* Global array of Raw Counts */
uint16 Slider_sensorRaw[Slider_TOTAL_SENSOR_COUNT] = {0u};

/* Backup variables for trim registers*/
#if (Slider_IDAC1_POLARITY == Slider__IDAC_SINK)
	uint8 Slider_csdTrim2;   
#else
    uint8 Slider_csdTrim1;	
#endif /* (Slider_IDAC1_POLARITY == Slider__IDAC_SINK) */

/* Global array of un-scanned sensors state */
uint8 Slider_unscannedSnsDriveMode[Slider_TOTAL_SENSOR_COUNT];

/* Backup array for Slider_sensorEnableMask */
uint8 Slider_sensorEnableMaskBackup[(((Slider_TOTAL_SENSOR_COUNT - 1u) / 8u) + 1u)];

/* Configured constants and arrays by Customizer */
uint8 Slider_sensorEnableMask[(((Slider_TOTAL_SENSOR_COUNT - 1u) / 8u) + 1u)] = {
0x1Fu, };

reg32 * Slider_pcTable[] = {
    (reg32 *)Slider_Sns__LinearSlider0_e0__LS__PC, 
    (reg32 *)Slider_Sns__LinearSlider0_e1__LS__PC, 
    (reg32 *)Slider_Sns__LinearSlider0_e2__LS__PC, 
    (reg32 *)Slider_Sns__LinearSlider0_e3__LS__PC, 
    (reg32 *)Slider_Sns__LinearSlider0_e4__LS__PC, 
};

const uint8 Slider_portTable[] = {
    Slider_Sns__LinearSlider0_e0__LS__PORT, 
    Slider_Sns__LinearSlider0_e1__LS__PORT, 
    Slider_Sns__LinearSlider0_e2__LS__PORT, 
    Slider_Sns__LinearSlider0_e3__LS__PORT, 
    Slider_Sns__LinearSlider0_e4__LS__PORT, 
};

const uint32 Slider_maskTable[] = {
    Slider_Sns__LinearSlider0_e0__LS__MASK, 
    Slider_Sns__LinearSlider0_e1__LS__MASK, 
    Slider_Sns__LinearSlider0_e2__LS__MASK, 
    Slider_Sns__LinearSlider0_e3__LS__MASK, 
    Slider_Sns__LinearSlider0_e4__LS__MASK, 
};

const uint8 Slider_pinShiftTbl[] = {
    (uint8) Slider_Sns__LinearSlider0_e0__LS__SHIFT, 
    (uint8) Slider_Sns__LinearSlider0_e1__LS__SHIFT, 
    (uint8) Slider_Sns__LinearSlider0_e2__LS__SHIFT, 
    (uint8) Slider_Sns__LinearSlider0_e3__LS__SHIFT, 
    (uint8) Slider_Sns__LinearSlider0_e4__LS__SHIFT, 
};

uint8 Slider_modulationIDAC[Slider_TOTAL_SENSOR_COUNT];
uint8 Slider_compensationIDAC[Slider_TOTAL_SENSOR_COUNT];

uint32 Slider_widgetResolution[Slider_RESOLUTIONS_TBL_SIZE] = {
    Slider_RESOLUTION_12_BITS,
};

uint8 Slider_senseClkDividerVal[Slider_TOTAL_SCANSLOT_COUNT];
uint8 Slider_sampleClkDividerVal[Slider_TOTAL_SCANSLOT_COUNT];

const uint8 Slider_widgetNumber[Slider_TOTAL_SENSOR_COUNT] = {
    0u, 0u, 0u, 0u, 0u, /* LinearSlider0__LS */
};



reg32* const Slider_prtSelTbl[] = {
/* CY_PSOC4_4000 is default*/
	((reg32 *) CYREG_HSIOM_PORT_SEL0),
	((reg32 *) CYREG_HSIOM_PORT_SEL1),
	((reg32 *) CYREG_HSIOM_PORT_SEL2),
	((reg32 *) CYREG_HSIOM_PORT_SEL3),
#if (Slider_CFG_REG_TBL_SIZE == 5u)
	((reg32 *) CYREG_HSIOM_PORT_SEL4)
#elif(Slider_CFG_REG_TBL_SIZE == 6u)
	((reg32 *) CYREG_HSIOM_PORT_SEL4),
	((reg32 *) CYREG_HSIOM_PORT_SEL5)
#elif(Slider_CFG_REG_TBL_SIZE == 8u)
	((reg32 *) CYREG_HSIOM_PORT_SEL4),
	((reg32 *) CYREG_HSIOM_PORT_SEL5),
	((reg32 *) CYREG_HSIOM_PORT_SEL6),
	((reg32 *) CYREG_HSIOM_PORT_SEL7)
#endif /* (CY_PSOC4_4200) */
};

reg32* const Slider_prtCfgTbl[] = {
/* CY_PSOC4_4000 is default*/
	((reg32 *) CYREG_GPIO_PRT0_PC),
	((reg32 *) CYREG_GPIO_PRT1_PC),
	((reg32 *) CYREG_GPIO_PRT2_PC),
	((reg32 *) CYREG_GPIO_PRT3_PC),
#if (Slider_CFG_REG_TBL_SIZE == 5u)
	((reg32 *) CYREG_GPIO_PRT4_PC)
#elif(Slider_CFG_REG_TBL_SIZE == 6u)
	((reg32 *) CYREG_GPIO_PRT4_PC),
	((reg32 *) CYREG_GPIO_PRT5_PC)
#elif(Slider_CFG_REG_TBL_SIZE == 8u)
	((reg32 *) CYREG_GPIO_PRT4_PC),
	((reg32 *) CYREG_GPIO_PRT5_PC),
	((reg32 *) CYREG_GPIO_PRT6_PC),
	((reg32 *) CYREG_GPIO_PRT7_PC)
#endif /* (CY_PSOC4_4200) */	
};

reg32* const Slider_prtDRTbl[] = {
/* CY_PSOC4_4000 is default*/
	((reg32 *) CYREG_GPIO_PRT0_DR),
	((reg32 *) CYREG_GPIO_PRT1_DR),
	((reg32 *) CYREG_GPIO_PRT2_DR),
	((reg32 *) CYREG_GPIO_PRT3_DR),
#if (Slider_CFG_REG_TBL_SIZE == 5u)
	((reg32 *) CYREG_GPIO_PRT4_DR)
#elif(Slider_CFG_REG_TBL_SIZE == 6u)
	((reg32 *) CYREG_GPIO_PRT4_DR),
	((reg32 *) CYREG_GPIO_PRT5_DR),
#elif(Slider_CFG_REG_TBL_SIZE == 8u)
	((reg32 *) CYREG_GPIO_PRT4_DR),
	((reg32 *) CYREG_GPIO_PRT5_DR),
	((reg32 *) CYREG_GPIO_PRT6_DR),
	((reg32 *) CYREG_GPIO_PRT7_DR),
#endif /* (CY_PSOC4_4200) */	
};
    

/*******************************************************************************
* Function Name: Slider_Init
********************************************************************************
*
* Summary:
*  API initializes default CapSense configuration provided by the customizer that defines 
*  the mode of component operations and resets all the sensors to an inactive state.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  Slider_immunityIndex - defines immunity level.
*
* Side Effects:
*  None
* 
*******************************************************************************/
void Slider_Init(void)
{   
	uint32 curSensor;
	
	#if(Slider_IS_SHIELD_ENABLE)
		
		#if((Slider_CSH_TANK_PREGARGE_OPTION == Slider__CAPPRIOBUF) || (0u != Slider_CSH_TANK_ENABLE))
			uint32 newRegValue;
			
			newRegValue = Slider_CTANK_CONNECTION_REG;
	        newRegValue &= ~(Slider_CSD_CTANK_CONNECTION_MASK);
			newRegValue |= Slider_CSD_CTANK_TO_AMUXBUS_B;
	        Slider_CTANK_CONNECTION_REG = newRegValue;
			
			#if(Slider_CSH_TANK_PREGARGE_OPTION == Slider__CAPPRIOBUF)
				newRegValue = Slider_CTANK_PORT_PC_REG;
		        newRegValue &= ~(Slider_CSD_CTANK_PC_MASK);
				newRegValue |= Slider_CTANK_PC_STRONG_MODE;
		        Slider_CTANK_PORT_PC_REG = newRegValue;
				
				newRegValue = Slider_CTANK_PORT_DR_REG;
		        newRegValue &= ~(Slider_CTANK_DR_MASK);
				newRegValue |= Slider_CTANK_DR_CONFIG;
		        Slider_CTANK_PORT_DR_REG = newRegValue;
			#endif /* (Slider_CSH_TANK_PREGARGE_OPTION == Slider__CAPPRIOBUF) */
	        
		#endif /* ((Slider_CSH_TANK_PREGARGE_OPTION == Slider__CAPPRIOBUF) || (Slider_CSH_TANK_ENABLE)) */
		
		Slider_EnableShieldElectrode((uint32)Slider_SHIELD_PIN_NUMBER, (uint32)Slider_SHIELD_PORT_NUMBER);
		
	#endif /* (Slider_IS_SHIELD_ENABLE) */
	
	for(curSensor = 0u; curSensor < Slider_TOTAL_SENSOR_COUNT; curSensor++)
	{
		Slider_unscannedSnsDriveMode[curSensor] = Slider_CONNECT_INACTIVE_SNS;
	}
    
	Slider_CsdHwBlockInit();
    Slider_SetShieldDelay(Slider_SHIELD_DELAY );
	
	/* Clear all sensors */
    Slider_ClearSensors();
}

/*******************************************************************************
* Function Name: Slider_CsdHwBlockInit
********************************************************************************
*
* Summary:
*  Initialises the hardware parameters of the CSD block 
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  Slider_csdTrim1 - Contains IDAC trim register value for Sourcing Mode.
*  Slider_csdTrim2 - Contains IDAC trim register value for Sinking Mode.
*
* Side Effects:
*  None
* 
*******************************************************************************/
void Slider_CsdHwBlockInit(void)
{
	uint32 newRegValue;
	
	    /* Set trim registers for CSD Mode */
    #if (Slider_IDAC1_POLARITY == Slider__IDAC_SINK)
		/* iDAC1 Sinking Mode */ 
		Slider_csdTrim2 = (uint8)Slider_CSD_TRIM2_REG;
		newRegValue = Slider_csdTrim2;
		newRegValue &= Slider_CSD_IDAC1_TRIM_MASK;
		newRegValue |= (uint32)((uint32)Slider_SFLASH_CSD_TRIM2_REG & (uint32)Slider_CSFLASH_TRIM_IDAC1_MASK); 
		
		#if (Slider_IDAC_CNT == 2u)
		    /* iDAC2 Sinking Mode */ 
			newRegValue &= Slider_CSD_IDAC2_TRIM_MASK;
			newRegValue |= (uint32)((uint32)Slider_SFLASH_CSD_TRIM2_REG & (uint32)Slider_CSFLASH_TRIM_IDAC2_MASK); 
		#endif /* (Slider_IDAC_CNT == 2u) */
		Slider_CSD_TRIM2_REG = newRegValue;
    #else   
		/* iDAC1 Sourcing Mode */ 
		Slider_csdTrim1 = (uint8)Slider_CSD_TRIM1_REG;
		newRegValue = Slider_csdTrim1;
		newRegValue &= Slider_CSD_IDAC1_TRIM_MASK;
		newRegValue |= (uint32)((uint32)Slider_SFLASH_CSD_TRIM1_REG & (uint32)Slider_CSFLASH_TRIM_IDAC1_MASK);
		#if (Slider_IDAC_CNT == 2u)
		     /* iDAC2 Sourcing Mode */ 
			newRegValue &= Slider_CSD_IDAC2_TRIM_MASK;
			newRegValue |= (uint32)((uint32)Slider_SFLASH_CSD_TRIM1_REG & (uint32)Slider_CSFLASH_TRIM_IDAC2_MASK);
		#endif		
		Slider_CSD_TRIM1_REG = newRegValue;
    #endif /* (Slider_IDAC1_POLARITY == Slider__IDAC_SINK) */
	
	/* Configure CSD and IDAC */
	#if (Slider_IDAC_CNT > 1u)
		Slider_CSD_IDAC_REG = Slider_DEFAULT_CSD_IDAC_CONFIG;
		Slider_CSD_CFG_REG = Slider_DEFAULT_CSD_CONFIG;
	#else
		Slider_CSD_IDAC_REG &= ~(Slider_CSD_IDAC1_MODE_MASK  | Slider_CSD_IDAC1_DATA_MASK);
		Slider_CSD_IDAC_REG |= Slider_DEFAULT_CSD_IDAC_CONFIG;
		
		Slider_CSD_CFG_REG &= ~(Slider_CSD_CONFIG_MASK);
		Slider_CSD_CFG_REG |= (Slider_DEFAULT_CSD_CONFIG);
	#endif /* (Slider_IDAC_CNT > 1u) */
	
	
	/* Connect Cmod to AMUX bus */ 
	newRegValue = Slider_CMOD_CONNECTION_REG; 
    newRegValue &= ~(Slider_CSD_CMOD_CONNECTION_MASK);
    newRegValue |= Slider_CSD_CMOD_TO_AMUXBUS_A;
    Slider_CMOD_CONNECTION_REG = newRegValue;
	
	/* Configure Dead Band PWM if it is enabled */
	#if(Slider_CSD_4B_PWM_MODE != Slider__PWM_OFF)
		Slider_CSD_4B_PWM_REG = Slider_DEFAULT_CSD_4B_PWM_CONFIG;
	#endif /* (Slider_CSD_4B_PWM_MODE != Slider__PWM_OFF) */

	/* Setup ISR */
	CyIntDisable(Slider_ISR_NUMBER);
	#if !defined(CY_EXTERNAL_INTERRUPT_CONFIG)		
	    (void)CyIntSetVector(Slider_ISR_NUMBER, &Slider_ISR);
	    CyIntSetPriority(Slider_ISR_NUMBER, Slider_ISR_PRIORITY);
	#endif /* (CY_EXTERNAL_INTERRUPT_CONFIG) */
}


/*******************************************************************************
* Function Name: Slider_Enable
********************************************************************************
*
* Summary:
*  Enables the CSD block and related resources to an active mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  None.
*
* Side Effects:
*  None
* 
*******************************************************************************/
void Slider_Enable(void)
{
    uint32 newRegValue;
    
    /* Enable Clocks */
	Slider_SenseClk_Stop();
	Slider_SampleClk_Stop();
	
	Slider_SampleClk_SetDividerValue((uint16)Slider_INITIAL_CLK_DIVIDER);
	Slider_SenseClk_SetDividerValue((uint16)Slider_INITIAL_CLK_DIVIDER);
	
	#if (0u == Slider_IS_M0S8PERI_BLOCK)
		Slider_SenseClk_Start();
		Slider_SampleClk_Start();
	#else
		Slider_SampleClk_Start();
		Slider_SenseClk_StartEx(Slider_SampleClk__DIV_ID);
	#endif /* (0u == Slider_IS_M0S8PERI_BLOCK) */
	
	/* Enable the CSD block */
    newRegValue = Slider_CSD_CFG_REG;
    newRegValue |= (Slider_CSD_CFG_ENABLE | Slider_CSD_CFG_SENSE_COMP_EN 
                                                   | Slider_CSD_CFG_SENSE_EN);    
    Slider_CSD_CFG_REG = newRegValue;

    /* Enable interrupt */
    CyIntEnable(Slider_ISR_NUMBER);
}


/*******************************************************************************
* Function Name: Slider_Start
********************************************************************************
*
* Summary:
*  This is the preferred method to begin the component operation. CapSense_Start() 
*  calls the CapSense_Init() function, and then calls the CapSense_Enable() 
*  function. Initializes the registers and starts the CSD method of the CapSense 
*  component. Resets all the sensors to an inactive state. Enables interrupts for 
*  sensors scanning. When the SmartSense tuning mode is selected, the tuning procedure
*  is applied for all the sensors. The CapSense_Start() routine must be called before 
*  any other API routines.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*   Slider_initVar - used to check the initial configuration, 
*   modified on the first function call.
*  Slider_lowLevelTuningDone - Used to notify the Tuner GUI that 
*   tuning of the scanning parameters is done. 
*
* Side Effects:
*  None
* 
*******************************************************************************/
void Slider_Start(void)
{

	static uint8 Slider_initVar = 0u;

	#if((0u != Slider_AUTOCALIBRATION_ENABLE) && (Slider_TUNING_METHOD != Slider__TUNING_AUTO))
		uint32 curSensor;
		uint32 rawLevel;
		uint32 widget;
    #endif /* ((0u != Slider_AUTOCALIBRATION_ENABLE) && (Slider_TUNING_METHOD != Slider__TUNING_AUTO)) */
	
    if (Slider_initVar == 0u)
    {
        Slider_Init();
        Slider_initVar = 1u;
    }
    Slider_Enable();
	
    /* AutoTunning start */
    #if(Slider_TUNING_METHOD == Slider__TUNING_AUTO)
		#if(0u != Slider_CSHL_API_GENERATE)
	        Slider_AutoTune();
	        Slider_lowLevelTuningDone = 1u;
		#endif /* (0u != Slider_CSHL_API_GENERATE) */
	#else
		#if(0u != Slider_AUTOCALIBRATION_ENABLE)
			
			#if(Slider_IDAC_CNT > 1u)
				Slider_CSD_IDAC_REG &= ~(Slider_CSD_IDAC2_MODE_MASK);	
			#endif /* (Slider_IDAC_CNT > 1u) */
			
			for(curSensor = 0u; curSensor < Slider_TOTAL_SCANSLOT_COUNT; curSensor++)
			{
				widget = Slider_widgetNumber[curSensor];
				rawLevel = Slider_widgetResolution[widget] >> Slider_RESOLUTION_OFFSET;
				
				/* Calibration level should be equal to 85% from scanning resolution */
				rawLevel = (rawLevel * 85u) / 100u;
				
				Slider_CalibrateSensor(curSensor, rawLevel, Slider_modulationIDAC);
			}

			#if(0u != Slider_TOTAL_CENTROIDS_COUNT) 
				Slider_NormalizeWidgets(Slider_END_OF_WIDGETS_INDEX, Slider_modulationIDAC);
			#endif /* (0u != Slider_TOTAL_CENTROIDS_COUNT)  */

			#if(Slider_IDAC_CNT > 1u)
				for(curSensor = 0u; curSensor < Slider_TOTAL_SCANSLOT_COUNT; curSensor++)
			    {
					Slider_compensationIDAC[curSensor] = Slider_modulationIDAC[curSensor] / 2u;
					Slider_modulationIDAC[curSensor] = (Slider_modulationIDAC[curSensor] + 1u) / 2u;
			    }
				Slider_CSD_IDAC_REG |= Slider_CSD_IDAC2_MODE_FIXED;
			#endif /* (Slider_IDAC_CNT > 1u) */
			
        #endif /* (0u != Slider_AUTOCALIBRATION_ENABLE) */ 
					
    #endif /* ((Slider_TUNING_METHOD == Slider__TUNING_AUTO) */	
	
	/* Initialize Advanced Centroid */
	#if(Slider_TOTAL_TRACKPAD_GESTURES_COUNT > 0u)
		Slider_AdvancedCentroidInit();
	#endif /* (Slider_TOTAL_TRACKPAD_GESTURES_COUNT > 0u) */
	
	/* Connect sense comparator input to AMUXA */
	Slider_CSD_CFG_REG |= Slider_CSD_CFG_SENSE_INSEL;
}


/*******************************************************************************
* Function Name: Slider_Stop
********************************************************************************
*
* Summary:
*  Stops the sensor scanning, disables component interrupts, and resets all the 
*  sensors to an inactive state. Disables the Active mode power template bits for
*  the subcomponents used within CapSense.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  Slider_csdTrim1 - Contains the IDAC trim register value for the Sourcing Mode.
*  Slider_csdTrim2 - Contains the IDAC trim register value for vSinking Mode.
*
* Side Effects:
*  This function should be called after scans are completed.
*
*******************************************************************************/
void Slider_Stop(void)
{
    /* Disable interrupt */
    CyIntDisable(Slider_ISR_NUMBER);
	
	Slider_CSD_CFG_REG &= ~(Slider_CSD_CFG_SENSE_COMP_EN | Slider_CSD_CFG_SENSE_EN);
	
	#if(Slider_IDAC_CNT == 2u)
		Slider_CSD_CFG_REG &= ~(Slider_CSD_CFG_ENABLE);
	#endif /* (Slider_IDAC_CNT == 2u) */

    /* Disable the Clocks */
    Slider_SenseClk_Stop();
    Slider_SampleClk_Stop();
}


/*******************************************************************************
* Function Name: Slider_FindNextSensor
********************************************************************************
*
* Summary:
*  Finds the next sensor to scan. 
*
* Parameters:
*  snsIndex:  Current index of sensor.
*
* Return:
*  Returns the next sensor index to scan.
*
* Global Variables:
*  Slider_sensorEnableMask[] - used to store bit masks of the enabled sensors.
*
* Side Effects:
*  This function affects the  current scanning and should not
*  be used outside the component.
*
*******************************************************************************/
uint8 Slider_FindNextSensor(uint8 snsIndex)
{
    uint32 enableFlag;
    
    /* Check if sensor enabled */
    do
    {
        /* Proceed with next sensor */
        snsIndex++;
        if(snsIndex == Slider_TOTAL_SENSOR_COUNT)
        {
            break;
        }
        enableFlag = Slider_GetBitValue(Slider_sensorEnableMask, (uint32)snsIndex);
    }    
    while(enableFlag == 0u);
    
    return ((uint8)snsIndex);
}


/*******************************************************************************
* Function Name: Slider_ScanSensor
********************************************************************************
*
* Summary:
*  Sets the scan settings and starts scanning a sensor. After scanning is complete,
*  the ISR copies the measured sensor raw data to the global raw sensor array. 
*  Use of the ISR ensures this function is non-blocking. 
*  Each sensor has a unique number within the sensor array. 
*  This number is assigned by the CapSense customizer in sequence.
*
* Parameters:
*  sensor:  Sensor number.
*
* Return:
*  None
*
* Global Variables:
*  Slider_csdStatusVar - used to provide the status and mode of the scanning process. 
*  Sets the busy status(scan in progress) and mode of scan as single scan.
*  Slider_sensorIndex - used to store a sensor scanning sensor number.
*  Sets to the provided sensor argument.
*
* Side Effects:
*  None
* 
*******************************************************************************/
void Slider_ScanSensor(uint32 sensor)
{
    /* Clears status/control variable and set sensorIndex */
    Slider_csdStatusVar = 0u;
    Slider_sensorIndex = (uint8)sensor;
    
    /* Start of sensor scan */
    Slider_csdStatusVar = (Slider_SW_STS_BUSY | Slider_SW_CTRL_SINGLE_SCAN);
    Slider_PreScan(sensor);
}


#if(0u != Slider_CSHL_API_GENERATE)
/*******************************************************************************
* Function Name: Slider_ScanWidget
********************************************************************************
*
* Summary:
*  Sets the scan settings and starts scanning a widget.
*
* Parameters:
*  uint32 widget: Widget number.
*
* Return:
*  None
*
* Global Variables:
*  Slider_csdStatusVar - used to provide the status and mode of the scanning process. 
*  Sets the busy status(scan in progress) and clears the single scan mode.
*  Slider_sensorIndex - used to store a sensor scanning sensor number.
*  Sets to 0xFF and provided to function Slider_FindNextSensor or
*  Slider_sensorEnableMask[] - used to store bit masks of the enabled sensors.
*  Slider_sensorEnableMaskBackup[] - used to backup bit masks of the enabled
*   sensors.
*
* Side Effects:
*  None
* 
*******************************************************************************/
void Slider_ScanWidget(uint32 widget)
{
    uint32 sensorsPerWidget;
    uint32 lastSensor;
    uint32 snsIndex;

	/* Get first sensor in widget */
	Slider_sensorIndex = Slider_rawDataIndex[widget];
	
	/* Get number of sensors in widget */
	sensorsPerWidget = Slider_numberOfSensors[widget];
	
	/* Check if generic Sensor */
	if(sensorsPerWidget == 0u)
	{
		sensorsPerWidget = 1u;
	}
	
	/* Get last sensor in widget */
	lastSensor = (Slider_rawDataIndex[widget] + sensorsPerWidget) - 1u;

	/* Backup sensorEnableMask array */
	for(snsIndex = 0u; snsIndex < Slider_TOTAL_SENSOR_MASK; snsIndex++)
	{
		/* Backup sensorEnableMask array */
		Slider_sensorEnableMaskBackup[snsIndex] = Slider_sensorEnableMask[snsIndex];
	}

	/* Update sensorEnableMask to scan the sensors that belong to widget */
	for(snsIndex = 0u; snsIndex < Slider_TOTAL_SENSOR_COUNT; snsIndex++)
	{
		/* Update sensorEnableMask array bits to scan the widget only */ 
	    if((snsIndex >= Slider_sensorIndex) && (snsIndex <= lastSensor))
		{
		    /* Set sensor bit to scan */
			Slider_SetBitValue(Slider_sensorEnableMask, snsIndex, 1u);
		}
		else
		{
		    /* Reset sensor bit to do not scan */
		    Slider_SetBitValue(Slider_sensorEnableMask, snsIndex, 0u);
		}
	}

    /* Check end of scan condition */
    if(Slider_sensorIndex < Slider_TOTAL_SENSOR_COUNT)
    {
		/* Set widget busy bit in status/control variable*/ 
		Slider_csdStatusVar = (Slider_SW_STS_BUSY | Slider_SW_CTRL_WIDGET_SCAN);
		/* Scan first sensor of widget*/ 
        Slider_PreScan((uint32)Slider_sensorIndex);
    }
}
#endif /* (0u != Slider_CSHL_API_GENERATE) */


/*******************************************************************************
* Function Name: Slider_ScanEnableWidgets
********************************************************************************
*
* Summary:
*  This is the preferred method to scan all of the enabled widgets. 
*  The API starts scanning a sensor within the enabled widgets. 
*  The ISR continues scanning the sensors until all the enabled widgets are scanned. 
*  Use of the ISR ensures this function is non-blocking.
*  All the widgets are enabled by default except proximity widgets. 
*  The proximity widgets must be manually enabled as their long scan time 
*  is incompatible with a fast response required of other widget types.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  Slider_csdStatusVar - used to provide the status and mode of the scanning process. 
*  Sets the busy status(scan in progress) and clears the single scan mode.
*  Slider_sensorIndex - used to store a sensor scanning sensor number.
*  Sets to 0xFF and provided to function Slider_FindNextSensor or
*
* Side Effects:
*  None
* 
*******************************************************************************/
void Slider_ScanEnabledWidgets(void)
{
    /* Clears status/control variable and set sensorIndex */
    Slider_csdStatusVar = 0u;
    Slider_sensorIndex = 0xFFu;
    
    /* Find next sensor */
    Slider_sensorIndex = (uint8)Slider_FindNextSensor(Slider_sensorIndex);

    /* Check end of scan condition */
    if(Slider_sensorIndex < Slider_TOTAL_SENSOR_COUNT)
    {
        Slider_csdStatusVar |= Slider_SW_STS_BUSY;
        Slider_PreScan((uint32)Slider_sensorIndex);
    }
}


/*******************************************************************************
* Function Name: Slider_IsBusy
********************************************************************************
*
* Summary:
*  Returns the state of the CapSense component. 1 means that scanning in 
*  progress and 0 means that scanning is complete.
*
* Parameters:
*  None
*
* Return:
*  Returns the state of scanning. 1 - scanning in progress, 0 - scanning 
*  completed.
*
* Global Variables:
*  Slider_csdStatusVar - used to provide the status and mode of the scanning process. 
*  Checks the busy status.
*
* Side Effects:
*  None
* 
*******************************************************************************/
uint32 Slider_IsBusy(void)
{
    return ((uint32)((0u != (Slider_csdStatusVar & Slider_SW_STS_BUSY)) ? 1u : 0u));
}


/*******************************************************************************
* Function Name: Slider_ReadSensorRaw
********************************************************************************
*
* Summary:
*  Returns sensor raw data from the global CapSense_sensorRaw[ ] array. 
*  Each scan sensor has a unique number within the sensor array. 
*  This number is assigned by the CapSense customizer in sequence. 
*  Raw data can be used to perform calculations outside of the CapSense 
*  provided framework.
*
* Parameters:
*  sensor:  Sensor number.
*
* Return:
*  Returns the current raw data value for a defined sensor number.
*
* Global Variables:
*  Slider_sensorRaw[] - used to store sensors raw data.
*
* Side Effects: 
*  None
*
*******************************************************************************/
uint16 Slider_ReadSensorRaw(uint32 sensor)
{
    return Slider_sensorRaw[sensor];
}


/*******************************************************************************
* Function Name: Slider_WriteSensorRaw
********************************************************************************
*
* Summary:
*  This API writes the raw count value passed as an argument to the sensor Raw count array.
*
* Parameters:
*  sensor:  Sensor number.
*  value: Raw count value.
*
* Global Variables:
*  Slider_sensorRaw[] - used to store sensors raw data.
*
* Return:
*  None
*
* Side Effects: 
*  None
*
*******************************************************************************/
void Slider_WriteSensorRaw(uint32 sensor, uint16 value)
{
	Slider_sensorRaw[sensor] = value;
}


#if (Slider_TUNING_METHOD != Slider__TUNING_NONE)
	/*******************************************************************************
	* Function Name: Slider_SetScanResolution
	********************************************************************************
	*
	* Summary:
	*  Sets a value of the sensor scan resolution for a widget. 
	*  The MIN resolution can be set 8-bit. The MAX Resolution can be set 16 bit.
	*  This function is not available for the tuning mode "None".
	*
	* Parameters:
	*  widget:     Widget index.
	*  resolution: Represents the resolution value. The following defines which are available in the
	*			   Slider.h file should be used:
	*			   Slider_RESOLUTION_6_BITS
	*			   Slider_RESOLUTION_7_BITS
	*			   Slider_RESOLUTION_8_BITS
	*			   Slider_RESOLUTION_9_BITS
	*			   Slider_RESOLUTION_10_BITS
	*			   Slider_RESOLUTION_11_BITS
	*			   Slider_RESOLUTION_12_BITS
	*			   Slider_RESOLUTION_13_BITS
	*			   Slider_RESOLUTION_14_BITS
	*			   Slider_RESOLUTION_15_BITS
	*			   Slider_RESOLUTION_16_BITS
	*
	* Return:
	*  None
	*
	* Global Variables:
	*  Slider_widgetResolution[] - used to store scan resolution of each widget.
	*
	* Side Effects: 
	*  None
	*
	*******************************************************************************/
	void Slider_SetScanResolution(uint32 widget, uint32 resolution)
	{ 
        Slider_widgetResolution[widget] = resolution;
	}
#endif /* (Slider_TUNING_METHOD != Slider__TUNING_NONE) */


/*******************************************************************************
* Function Name: Slider_GetScanResolution
********************************************************************************
*
* Summary:
*  Returns the resolution value for the appropriate sensor.
*  This function is not available for tuning mode "None".
*
* Parameters:
*  widget:     Widget index.
*
* Return:
*  resolution: Returns the resolution value for the appropriate sensor. 
*              The resolution values are defined in the Slider.h file 
*              The defines are encountered below:
*			   Slider_RESOLUTION_6_BITS
*			   Slider_RESOLUTION_7_BITS
*			   Slider_RESOLUTION_8_BITS
*			   Slider_RESOLUTION_9_BITS
*			   Slider_RESOLUTION_10_BITS
*			   Slider_RESOLUTION_11_BITS
*			   Slider_RESOLUTION_12_BITS
*			   Slider_RESOLUTION_13_BITS
*			   Slider_RESOLUTION_14_BITS
*			   Slider_RESOLUTION_15_BITS
*			   Slider_RESOLUTION_16_BITS
*
* Global Variables:
*  Slider_widgetResolution[] - used to store scan resolution of every widget.
*
* Side Effects: 
*  None
*
*******************************************************************************/
uint32 Slider_GetScanResolution(uint32 widget)
{
	return(Slider_widgetResolution[widget]);
}


/*******************************************************************************
* Function Name: Slider_ClearSensors
********************************************************************************
*
* Summary:
*  Resets all the sensors to the non-sampling state by sequentially disconnecting
*  all the sensors from Analog MUX Bus and putting them to an inactive state.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  None
*
* Side Effects: 
*  None
*
*******************************************************************************/
void Slider_ClearSensors(void)
{
	uint32 snsIndex;
	
    for (snsIndex = 0u; snsIndex < Slider_TOTAL_SENSOR_COUNT; snsIndex++)
    {
        Slider_DisableScanSlot(snsIndex);
    }
}


#if (Slider_IS_COMPLEX_SCANSLOTS)
    /*******************************************************************************
    * Function Name: Slider_EnableScanSlot
    ********************************************************************************
    *
    * Summary:
    *  Configures the selected slot to measure during the next measurement 
    *  cycle. The corresponding pin/pins are set to Analog High-Z mode and 
    *  connected to the Analog Mux Bus. This also enables the comparator function.
    *
    * Parameters:
    *  slot:  Slot number.
    *
    * Return:
    *  None
    *
    * Global Constants:
    *  Slider_portTable[]  - used to store the port number that pin 
    *  belongs to for each sensor.
    *  Slider_maskTable[]  - used to store the pin within the port for 
    *  each sensor.
    *  Slider_indexTable[] - used to store indexes of complex sensors.
    *  The offset and position in this array are stored in a port and mask table for 
    *  complex sensors.
    *  Bit 7 (msb) is used to define the sensor type: single or complex.
    *
	* Side Effects: 
	*  None
	*
    *******************************************************************************/
    void Slider_EnableScanSlot(uint32 slot)
    {
        uint8 j;
        uint8 snsNumber;
        const uint8 *index;
        /* Read sensor type: single or complex */
        uint8 snsType = Slider_portTable[slot];

        /* Check if sensor is complex */
        if ((snsType & Slider_COMPLEX_SS_FLAG) == 0u)
        {
            /* Enable sensor (single) */
            Slider_EnableSensor(slot);
        }
        else
        {
            /* Enable complex sensor */
            snsType &= ~Slider_COMPLEX_SS_FLAG;
            index = &Slider_indexTable[snsType];
            snsNumber = Slider_maskTable[slot];
                        
            for (j = 0u; j < snsNumber; j++)
            {
                Slider_EnableSensor(index[j]);
            }
        }
    }
    
    
    /*******************************************************************************
    * Function Name: Slider_DisableScanSlot
    ********************************************************************************
    *
    * Summary:
    *  Disables the selected slot. The corresponding pin/pins is/are disconnected 
    *  from the Analog Mux Bus and connected to GND, High_Z or Shield electrode.
    *
    * Parameters:
    *  slot:  Slot number.
    *
    * Return:
    *  None
    *
    * Global Variables:
    *  Slider_portTable[]  - used to store the port number that pin 
    *  belongs to for each sensor.
    *  Slider_maskTable[]  - used to store the pin within the port for 
    *  each sensor.
    *  Slider_indexTable[] - used to store indexes of complex sensors.
    *  The offset and position in this array are stored in a port and mask table for 
    *  complex sensors.
    *  7bit(msb) is used to define the sensor type: single or complex.
    *
	* Side Effects: 
	*  None
	*
    *******************************************************************************/
    void Slider_DisableScanSlot(uint32 slot)
    {
        uint8 j;
        uint8 snsNumber;
        const uint8 *index;

        /* Read sensor type: single or complex */
        uint8 snsType = Slider_portTable[slot];
        
        /* Check if sensor is complex */
        if ((snsType & Slider_COMPLEX_SS_FLAG) == 0u)
        {
            /* Disable sensor (single) */
            Slider_DisableSensor(slot);
        }
        else
        {
            /* Disable complex sensor */
            snsType &= ~Slider_COMPLEX_SS_FLAG;
            index = &Slider_indexTable[snsType];
            snsNumber = Slider_maskTable[slot];
                        
            for (j=0; j < snsNumber; j++)
            {
                Slider_DisableSensor(index[j]);
            }
        } 
    }
#endif  /* Slider_IS_COMPLEX_SCANSLOTS */


/*******************************************************************************
* Function Name: Slider_EnableSensor
********************************************************************************
*
* Summary:
*  Configures the selected sensor to measure during the next measurement cycle.
*  The corresponding pins are set to Analog High-Z mode and connected to the
*  Analog Mux Bus. This also enables the comparator function.
*
* Parameters:
*  sensor:  Sensor number.
*
* Return:
*  None
*
* Global Variables:
*  Slider_portTable[] - used to store the port number that pin 
*  belongs to for each sensor.
*  Slider_pinShiftTbl[] - used to store position of pin that 
*  configured as sensor in port. 
*  Slider_prtSelTbl[] - Contains pointers to the HSIOM 
*  registers for each port. 
*  Slider_PrtCfgTb[] - Contains pointers to the port config 
*  registers for each port.
*
* Side Effects: 
*  None
*
*******************************************************************************/
void Slider_EnableSensor(uint32 sensor)
{
	uint8  pinModeShift;
    uint8  pinHSIOMShift;
	uint8 interruptState;
	uint32 newRegisterValue;
    uint32 port;
	
	port = (uint32) Slider_portTable[sensor];
    pinModeShift = Slider_pinShiftTbl[sensor]  * Slider_PC_PIN_CFG_SIZE;
    pinHSIOMShift = Slider_pinShiftTbl[sensor] * Slider_HSIOM_PIN_CFG_SIZE;

	interruptState = CyEnterCriticalSection();
	
	newRegisterValue = *Slider_prtSelTbl[port];
	newRegisterValue &= ~(Slider_CSD_HSIOM_MASK << pinHSIOMShift);
	newRegisterValue |= (uint32)((uint32)Slider_CSD_SENSE_PORT_MODE << pinHSIOMShift);
   
    *Slider_prtCfgTbl[port] &= (uint32)~((uint32)Slider_CSD_PIN_MODE_MASK << pinModeShift);
    *Slider_prtSelTbl[port] = newRegisterValue;
	
	CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: Slider_DisableSensor
********************************************************************************
*
* Summary:
*  Disables the selected sensor. The corresponding pin is disconnected from the
*  Analog Mux Bus and connected to GND, High_Z or Shield electrode.
*
* Parameters:
*  sensor:  Sensor number
*
* Return:
*  None
*
* Global Variables:
*  Slider_portTable[] - used to store the port number that pin 
*  belongs to for each sensor.
*  Slider_pinShiftTbl[] - used to store position of pin that 
*  configured as a sensor in the port. 
*  Slider_prtSelTbl[] - Contains pointers to the HSIOM 
*  registers for each port. 
*  Slider_PrtCfgTb[] - Contains pointers to the port config 
*  registers for each port.
*
* Side Effects: 
*  None
*
*******************************************************************************/
void Slider_DisableSensor(uint32 sensor)
{
    uint8 interruptState;
	uint32 newRegisterValue;
	
    uint32 port = (uint32) Slider_portTable[sensor];
    uint8  pinHSIOMShift = Slider_pinShiftTbl[sensor] * Slider_HSIOM_PIN_CFG_SIZE;
    uint8  pinModeShift = Slider_pinShiftTbl[sensor]  * Slider_PC_PIN_CFG_SIZE;

	uint32 inactiveConnect = Slider_SNS_HIZANALOG_CONNECT;
	uint32 sensorState = Slider_unscannedSnsDriveMode[sensor];

    *Slider_prtSelTbl[port] &= ~(Slider_CSD_HSIOM_MASK << pinHSIOMShift);

	#if(Slider_IS_SHIELD_ENABLE != 0)
	if(sensorState != (uint32)Slider__SHIELD)
	{
	#else
	    /* Connected to Ground if shield is disabled */
	    if(sensorState == (uint32)Slider__SHIELD)
		{
			sensorState = (uint32)Slider__GROUND;
		}
	#endif /* (Slider_IS_SHIELD_ENABLE) */
	    if(sensorState != (uint32)Slider__HIZ_ANALOG)
		{   
		    /* Connected to Ground */
		    inactiveConnect = (uint32)Slider_SNS_GROUND_CONNECT;
		}
		
        interruptState = CyEnterCriticalSection();

        /* Set drive mode */
    	newRegisterValue = *Slider_prtCfgTbl[port];
    	newRegisterValue &= ~(Slider_CSD_PIN_MODE_MASK << pinModeShift);
    	newRegisterValue |=  (uint32)(inactiveConnect << pinModeShift);
        *Slider_prtCfgTbl[port] =  newRegisterValue;
        
        CyExitCriticalSection(interruptState);

    	*Slider_prtDRTbl[port]  &=  (uint32)~(uint32)((uint32)1u << Slider_pinShiftTbl[sensor]); 
	#if(Slider_IS_SHIELD_ENABLE != 0)
	}
	else
	{
		/* Connect to Shield */
		*Slider_prtSelTbl[port] |= (Slider_CSD_SHIELD_PORT_MODE << pinHSIOMShift);
	}
	#endif /* (Slider_IS_SHIELD_ENABLE) */
}


/*******************************************************************************
* Function Name: Slider_SetDriveModeAllPins
********************************************************************************
*
* Summary:
* This API sets the drive mode of port pins used by the CapSense
* component (sensors, guard, shield, shield tank and Cmod) to drive the 
* mode specified by the argument.
*
* Parameters:
*  driveMode:  Drive mode definition.
*
* Return:
*  None
*
* Global Variables:
*  Slider_portTable[] - used to store the port number that pin 
*  belongs to for each sensor.
*  Slider_pinShiftTbl[] - used to store position of pin that 
*  configured as a sensor in the port. 
*
* Side Effects: 
*  This API shall be called only after CapSense component is stopped.
*
*******************************************************************************/
void Slider_SetDriveModeAllPins(uint32 driveMode)
{
	uint8 curSensor;
	#if (Slider_IS_COMPLEX_SCANSLOTS)
		uint8 snsNumber;
		uint8 snsType;
	#endif  /* Slider_IS_COMPLEX_SCANSLOTS */
	uint32 prtNumberTmp;
	uint32 pinNumberTmp;

	for(curSensor = 0u; curSensor < Slider_TOTAL_SENSOR_COUNT; curSensor++)
	{	
		#if (Slider_IS_COMPLEX_SCANSLOTS)
			/* Read sensor type: single or complex */
			snsType = Slider_portTable[curSensor];
		
			/* Check if sensor is complex */
			if ((snsType & Slider_COMPLEX_SS_FLAG) == 0u)
			{
				/* The sensor is not complex */
				snsNumber = curSensor;
			}
			else
			{
				/* Get dedicated sensor ID of the complex sensor */
				snsType &= ~Slider_COMPLEX_SS_FLAG;
				snsNumber = Slider_indexTable[snsType];
			}

			prtNumberTmp = Slider_portTable[snsNumber];
			pinNumberTmp = Slider_pinShiftTbl[snsNumber];
		#else
			prtNumberTmp = Slider_portTable[curSensor];
			pinNumberTmp = Slider_pinShiftTbl[curSensor];
		#endif  /* Slider_IS_COMPLEX_SCANSLOTS */
	
		Slider_SetPinDriveMode(driveMode, pinNumberTmp, prtNumberTmp);
	}
	
	Slider_SetPinDriveMode(driveMode, (uint32)Slider_CMOD_PIN_NUMBER, (uint32)Slider_CMOD_PRT_NUMBER);
	
	#if(0u != Slider_CSH_TANK_ENABLE)
		Slider_SetPinDriveMode(driveMode, (uint32)Slider_CTANK_PIN_NUMBER, (uint32)Slider_CTANK_PRT_NUMBER);
	#endif /* (0u != Slider_CSH_TANK_ENABLE) */	
	
	#if(0u != Slider_IS_SHIELD_ENABLE)
		Slider_SetPinDriveMode(driveMode, (uint32)Slider_SHIELD_PIN_NUMBER, (uint32)Slider_SHIELD_PORT_NUMBER);
	#endif /* (0u != Slider_IS_SHIELD_ENABLE) */	
}


/*******************************************************************************
* Function Name: Slider_RestoreDriveModeAllPins
********************************************************************************
*
* Summary:
*  This API restores the drive for all the CapSense port pins to the original
*  state.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  Slider_prtSelTbl[] - Contains pointers to the HSIOM 
*  registers for each port. 
*
* Side Effects: 
*  This API is called only after the CapSense component is stopped.
*
*******************************************************************************/
void Slider_RestoreDriveModeAllPins(void)
{
	uint32 newRegisterValue;
	
	Slider_SetDriveModeAllPins(CY_SYS_PINS_DM_ALG_HIZ);
	
	newRegisterValue = Slider_CMOD_CONNECTION_REG; 
    newRegisterValue &= ~(Slider_CSD_CMOD_CONNECTION_MASK);
    newRegisterValue |= Slider_CSD_CMOD_TO_AMUXBUS_A;
    Slider_CMOD_CONNECTION_REG = newRegisterValue;
	
	#if(0u != Slider_CSH_TANK_ENABLE)
		newRegisterValue = Slider_CTANK_CONNECTION_REG;
        newRegisterValue &= ~(Slider_CSD_CTANK_CONNECTION_MASK);
		newRegisterValue |= Slider_CSD_CTANK_TO_AMUXBUS_B;
        Slider_CTANK_CONNECTION_REG = newRegisterValue;
	#endif/* (0u != Slider_CSH_TANK_ENABLE) */
	
	#if(0u != Slider_IS_SHIELD_ENABLE)
		newRegisterValue = *Slider_prtSelTbl[Slider_SHIELD_PORT_NUMBER];
        newRegisterValue &= ~(Slider_CSD_HSIOM_MASK << ((uint32)Slider_SHIELD_PIN_NUMBER * Slider_HSIOM_PIN_CFG_SIZE));
        newRegisterValue |= (Slider_CSD_SHIELD_PORT_MODE << ((uint32)Slider_SHIELD_PIN_NUMBER * Slider_HSIOM_PIN_CFG_SIZE));
        *Slider_prtSelTbl[Slider_SHIELD_PORT_NUMBER] = newRegisterValue;
	#endif /* (0u != Slider_IS_SHIELD_ENABLE) */	
}


/*******************************************************************************
* Function Name: Slider_SetPinDriveMode
********************************************************************************
*
* Summary:
* This API sets the drive mode for the appropriate port/pin.
*
* Parameters:
*  driveMode:  Drive mode definition.
*  portNumber: contains port number (0, 1, 2).
*  pinNumber: contains pin number (0, 1, 2, ... , 7).
*
* Return:
*  None
*
* Global Variables:
*  Slider_prtSelTbl[] - Contains pointers to the HSIOM 
*  registers for each port. 
*  Slider_prtCfgTb[] - Contains pointers to the port config 
*  registers for each port.
*
* Side Effects: 
*  This API is called only after CapSense component is stopped.
*
*******************************************************************************/
void Slider_SetPinDriveMode(uint32 driveMode, uint32 pinNumber, uint32 portNumber)
{	
	uint32  pinModeShift;
	uint32 newRegisterValue;
	
	pinModeShift  = pinNumber * Slider_PC_PIN_CFG_SIZE;
	
	newRegisterValue = *Slider_prtCfgTbl[portNumber];
	newRegisterValue &= ~(Slider_CSD_PIN_MODE_MASK << pinModeShift);
	newRegisterValue |=  (uint32)((uint32)driveMode << pinModeShift);
    *Slider_prtCfgTbl[portNumber] =  newRegisterValue;
}


/*******************************************************************************
* Function Name: Slider_PreScan
********************************************************************************
*
* Summary:
*  Sets required settings, enables a sensor, removes Vref from AMUX and starts the 
*  scanning process of the sensor.
*
* Parameters:
*  sensor:  Sensor number.
*
* Return:
*  None
*
* Global Variables:
* Slider_widgetNumber[] - This array contains numbers of widgets for each sensor.
* Slider_widgetResolution[] - Contains the widget resolution.
*
* Side Effects: 
*  None
*
*******************************************************************************/
void Slider_PreScan(uint32 sensor)
{
    uint8 widget;
    uint8 interruptState;
	uint32 newRegValue;
	uint32 counterResolution;
	
	#if(Slider_PRS_OPTIONS == Slider__PRS_AUTO)
		uint8 senseClkDivMath;
		uint8 sampleClkDivMath;
	#endif /* (Slider_PRS_OPTIONS == Slider__PRS_AUTO) */	
	
	#if ((Slider_TUNING_METHOD == Slider__TUNING_AUTO) &&\
		 (0 != Slider_IS_OVERSAMPLING_EN))
		uint32 oversamplingFactor;
	#endif /* ((Slider_TUNING_METHOD == Slider__TUNING_AUTO) &&\
	           (0 != Slider_IS_OVERSAMPLING_EN)) */

    /* Define widget sensor belongs to */
    widget = Slider_widgetNumber[sensor];

	/* Recalculate Counter Resolution to MSB 16 bits */
	counterResolution = Slider_widgetResolution[widget];
	
	#if ((Slider_TUNING_METHOD == Slider__TUNING_AUTO) &&\
		 (0 != Slider_IS_OVERSAMPLING_EN))
	    oversamplingFactor = Slider_GetBitValue(scanSpeedTbl, sensor);

		if(counterResolution < Slider_RESOLUTION_16_BITS)
		{
			counterResolution <<= oversamplingFactor;
			counterResolution |= (uint32)(Slider_RESOLUTION_8_BITS);
		}
	#endif /* ((Slider_TUNING_METHOD == Slider__TUNING_AUTO) &&\
	           (0 != Slider_IS_OVERSAMPLING_EN)) */
	
	#if (0u != Slider_IS_M0S8PERI_BLOCK)
		Slider_SenseClk_Stop();
		Slider_SampleClk_Stop();
	#endif /* (0u != Slider_IS_M0S8PERI_BLOCK) */

	#if (0u != Slider_MULTIPLE_FREQUENCY_SET)
		Slider_SampleClk_SetDividerValue((uint16)Slider_sampleClkDividerVal[sensor]);
		Slider_SenseClk_SetDividerValue((uint16)Slider_senseClkDividerVal[sensor]);
	#else
		Slider_SampleClk_SetDividerValue((uint16)Slider_sampleClkDividerVal);
		Slider_SenseClk_SetDividerValue((uint16)Slider_senseClkDividerVal);
	#endif /* (0u != Slider_MULTIPLE_FREQUENCY_SET) */

	#if (0u != Slider_IS_M0S8PERI_BLOCK)
		Slider_SampleClk_Start();
		Slider_SenseClk_StartEx(Slider_SampleClk__DIV_ID);
		
		#if(Slider_PRS_OPTIONS == Slider__PRS_NONE)
			CyIntDisable(Slider_ISR_NUMBER);
			Slider_CSD_CNT_REG = Slider_ONE_CYCLE;
			while(0u != (Slider_CSD_CNT_REG & Slider_RESOLUTION_16_BITS))
			{
			/* Wait until scanning is complete */ 
			}
			Slider_CSD_INTR_REG = 1u;
			CyIntClearPending(Slider_ISR_NUMBER);
			CyIntEnable(Slider_ISR_NUMBER); 
		#endif /* Slider_PRS_OPTIONS == Slider__PRS_NONE */
	#endif /* (0u != Slider_IS_M0S8PERI_BLOCK) */

#if(Slider_PRS_OPTIONS != Slider__PRS_NONE)

	#if(Slider_TUNING_METHOD == Slider__TUNING_AUTO)
		if(Slider_prescalersTuningDone != 0u)
		{
	#endif /* (Slider_TUNING_METHOD == Slider__TUNING_AUTO) */
			
			CyIntDisable(Slider_ISR_NUMBER);
			
			newRegValue = Slider_CSD_CFG_REG;
			newRegValue |= Slider_CSD_CFG_PRS_SELECT;
			
			#if(Slider_PRS_OPTIONS == Slider__PRS_AUTO)
				
				newRegValue &= ~(Slider_PRS_MODE_MASK);
				
				#if (0u != Slider_MULTIPLE_FREQUENCY_SET)
					senseClkDivMath = Slider_senseClkDividerVal[sensor];
					sampleClkDivMath = Slider_sampleClkDividerVal[sensor];
				#else
					senseClkDivMath = Slider_senseClkDividerVal;
					sampleClkDivMath = Slider_sampleClkDividerVal;
				#endif /* ( Slider_MULTIPLE_FREQUENCY_SET) */
				
				#if(0u == Slider_IS_M0S8PERI_BLOCK)
					senseClkDivMath *= sampleClkDivMath;
				#endif /* (0u == Slider_IS_M0S8PERI_BLOCK) */	
				
				if((senseClkDivMath * Slider_RESOLUTION_12_BITS) <
				   (sampleClkDivMath * Slider_widgetResolution[widget]))
				{
					newRegValue |= Slider_CSD_PRS_12_BIT;
				}
			#endif /* (Slider_PRS_OPTIONS == Slider__PRS_AUTO) */
			
			Slider_CSD_CFG_REG = newRegValue;
			
			CyIntEnable(Slider_ISR_NUMBER);
			
	#if(Slider_TUNING_METHOD == Slider__TUNING_AUTO)	
		}
	#endif /* (Slider_PRS_OPTIONS != Slider__PRS_NONE) */

#endif /* (Slider_PRS_OPTIONS == Slider__PRS_NONE) */
	
	/* Set Idac Value */
	CyIntDisable(Slider_ISR_NUMBER);
    newRegValue = Slider_CSD_IDAC_REG;
 
#if (Slider_IDAC_CNT == 1u)    
	newRegValue &= ~(Slider_CSD_IDAC1_DATA_MASK);
	newRegValue |= Slider_modulationIDAC[sensor];
#else
	newRegValue &= ~(Slider_CSD_IDAC1_DATA_MASK | Slider_CSD_IDAC2_DATA_MASK);
	newRegValue |= (Slider_modulationIDAC[sensor] | 
							(uint32)((uint32)Slider_compensationIDAC[sensor] <<
							Slider_CSD_IDAC2_DATA_OFFSET));                               
#endif /* (Slider_IDAC_CNT == 1u) */
	
	Slider_CSD_IDAC_REG = newRegValue;

#if(Slider_CMOD_PREGARGE_OPTION == Slider__CAPPRIOBUF)
	newRegValue = Slider_CMOD_CONNECTION_REG;
	newRegValue &= ~(Slider_CSD_CMOD_CONNECTION_MASK);
	newRegValue |= Slider_CSD_CMOD_TO_AMUXBUS_A;
	Slider_CMOD_CONNECTION_REG = newRegValue;
	
	newRegValue = Slider_CMOD_PORT_PC_REG;
	newRegValue &= ~(Slider_CSD_CMOD_PC_MASK);
	newRegValue |= Slider_CMOD_PC_HIGH_Z_MODE;
	Slider_CMOD_PORT_PC_REG = newRegValue;
#endif /* (Slider_CMOD_PREGARGE_OPTION == Slider__CAPPRIOBUF) */

    /* Disconnect Vref Buffer from AMUX */
	newRegValue = Slider_CSD_CFG_REG;
	newRegValue &= ~(Slider_PRECHARGE_CONFIG_MASK);
	newRegValue |= Slider_CTANK_PRECHARGE_CONFIG;
	
	CyIntEnable(Slider_ISR_NUMBER);
	
    /* Enable Sensor */
    Slider_EnableScanSlot(sensor);
    
	interruptState = CyEnterCriticalSection();
	Slider_CSD_CFG_REG = newRegValue;
	
	/* `#START Slider_PreSettlingDelay_Debug` */

	/* `#END` */
	
	CyDelayCycles(Slider_GLITCH_ELIMINATION_CYCLES);
	
	/* `#START Slider_PreScan_Debug` */

	/* `#END` */
	
    Slider_CSD_CNT_REG = counterResolution;
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: Slider_PostScan
********************************************************************************
*
* Summary:
*  Stores the results of measurement in the Slider_sensorRaw[] array,
*  sets the scanning sensor in the non-sampling state, turns off Idac(Current Source IDAC),
*  disconnects the IDAC(Sink mode), and applies Vref on AMUX.
*
* Parameters:
*  sensor:  Sensor number.
*
* Return:
*  None
*
* Global Variables:
*  Slider_sensorRaw[] - used to store sensors raw data.
*
* Side Effects: 
*  None
*
*******************************************************************************/
void Slider_PostScan(uint32 sensor)
{	
	uint32 newRegValue;
#if ((Slider_TUNING_METHOD == Slider__TUNING_AUTO) &&\
	 (0 != Slider_IS_OVERSAMPLING_EN))
    uint32 oversamplingFactor;
    uint32 widget;
#endif /* ((Slider_TUNING_METHOD == Slider__TUNING_AUTO) &&\
		   (0 != Slider_IS_OVERSAMPLING_EN)) */	

	/* `#START Slider_PostScan_Debug` */

	/* `#END` */


    /* Read SlotResult from Raw Counter */
    Slider_sensorRaw[sensor]  = (uint16)Slider_CSD_CNT_REG;
	
#if ((Slider_TUNING_METHOD == Slider__TUNING_AUTO) &&\
	 (0 != Slider_IS_OVERSAMPLING_EN))
	widget = Slider_widgetNumber[sensor];
	if(Slider_widgetResolution[widget] < Slider_RESOLUTION_16_BITS)
	{
		oversamplingFactor = Slider_GetBitValue(scanSpeedTbl, sensor);
		Slider_sensorRaw[sensor] >>= oversamplingFactor;
	}
#endif /* ((Slider_TUNING_METHOD == Slider__TUNING_AUTO) &&\
		   (0 != Slider_IS_OVERSAMPLING_EN)) */

    /* Disable Sensor */
    Slider_DisableScanSlot(sensor);

	CyIntDisable(Slider_ISR_NUMBER);
	
#if(Slider_CMOD_PREGARGE_OPTION == Slider__CAPPRIOBUF)
	newRegValue = Slider_CMOD_CONNECTION_REG;
	newRegValue &= ~(Slider_CSD_CMOD_CONNECTION_MASK);
	newRegValue |= Slider_CSD_CMOD_TO_AMUXBUS_B;
	Slider_CMOD_CONNECTION_REG = newRegValue;
	
	newRegValue = Slider_CMOD_PORT_PC_REG;
	newRegValue &= ~(Slider_CSD_CMOD_PC_MASK);
	newRegValue |= Slider_CMOD_PC_STRONG_MODE;
	Slider_CMOD_PORT_PC_REG = newRegValue;
		
	newRegValue = Slider_CMOD_PORT_DR_REG;
    newRegValue &= ~(Slider_CMOD_DR_MASK);
	newRegValue |= Slider_CMOD_DR_CONFIG;
    Slider_CMOD_PORT_DR_REG = newRegValue;    
#endif /* (Slider_CMOD_PREGARGE_OPTION == Slider__CAPPRIOBUF) */
	
	/* Connect Vref Buffer to AMUX bus  */
	newRegValue = Slider_CSD_CFG_REG;
	newRegValue &= ~(Slider_PRECHARGE_CONFIG_MASK);
	newRegValue |= Slider_CMOD_PRECHARGE_CONFIG;
	Slider_CSD_CFG_REG = newRegValue;

	/* Set Idac Value = 0 */
#if (Slider_IDAC_CNT == 1u)   
	Slider_CSD_IDAC_REG &= ~(Slider_CSD_IDAC1_DATA_MASK);
#else
	Slider_CSD_IDAC_REG &= ~(Slider_CSD_IDAC1_DATA_MASK | Slider_CSD_IDAC2_DATA_MASK);                               
#endif /* (Slider_IDAC_CNT == 1u) */

	CyIntEnable(Slider_ISR_NUMBER);
}


/*******************************************************************************
* Function Name: Slider_EnableShieldElectrode
********************************************************************************
*
* Summary:
*  This API enables or disables the shield electrode on a specified port pin.
*
* Parameters:
*  portNumber: contains the shield electrode port number (0, 1, 2).
*  pinNumber: contains the shield electrode pin number (0, 1, 2, ... , 7).
*
* Return:
*  None
*
* Global Variables:
*  Slider_prtCfgTbl[] - Contains pointers to the port config registers for each port.
*  Slider_prtSelTbl[] - Contains pointers to the HSIOM registers for each port.
*
* Side Effects: 
*  None
*
*******************************************************************************/
void Slider_EnableShieldElectrode(uint32 pinNumber, uint32 portNumber)
{
	uint32 newRegValue;
	
	*Slider_prtCfgTbl[portNumber] &= ~(Slider_CSD_PIN_MODE_MASK << (pinNumber * Slider_PC_PIN_CFG_SIZE));
	newRegValue = *Slider_prtSelTbl[portNumber];
    newRegValue &= ~(Slider_CSD_HSIOM_MASK << (pinNumber * Slider_HSIOM_PIN_CFG_SIZE));
    newRegValue |= (Slider_CSD_SHIELD_PORT_MODE << (pinNumber * Slider_HSIOM_PIN_CFG_SIZE));
    *Slider_prtSelTbl[portNumber] = newRegValue;
}


/*******************************************************************************
* Function Name: Slider_SetShieldDelay
********************************************************************************
*
* Summary:
*  This API sets a shield delay.
*
* Parameters:
*  delay:  shield delay value:
*                               0 - no delay
*                               1 - 1 cycle delay
*                               2 - 2 cycles delay
*
* Return:
*  None
*
* Global Variables:
*  None
*
* Side Effects: 
*  None
*
*******************************************************************************/
void Slider_SetShieldDelay(uint32 delay)
{
    uint32 newRegValue;
    
    delay &= 0x03u;
    
    newRegValue = Slider_CSD_CFG_REG;
    newRegValue &= (uint32)(~(uint32)Slider_SHIELD_DELAY);    
    newRegValue |= (delay << Slider_CSD_CFG_SHIELD_DELAY_POS);    
    Slider_CSD_CFG_REG = newRegValue;
}


/*******************************************************************************
* Function Name: Slider_ReadCurrentScanningSensor
********************************************************************************
*
* Summary:
*  This API returns scanning sensor number when sensor scan is in progress. 
*  When sensor scan is completed the API returns 
*  $INSTANCE_NAME`_NOT_SENSOR (0xFFFFFFFF) (when no sensor is scanned).
*
* Parameters:
*	None.
*
* Return:
*  Returns Sensor number if sensor is being scanned and 
*  $INSTANCE_NAME`_NOT_SENSOR (0xFFFFFFFF) if scanning is complete.
*
* Global Variables:
*  Slider_sensorIndex - the sensor number is being scanned.
*
* Side Effects: 
*  None
*
*******************************************************************************/
uint32 Slider_ReadCurrentScanningSensor(void)
{
    return ((uint32)(( 0u != (Slider_csdStatusVar & Slider_SW_STS_BUSY)) ? 
                     Slider_sensorIndex : Slider_NOT_SENSOR));
}


/*******************************************************************************
* Function Name: Slider_GetBitValue
********************************************************************************
*
* Summary:
*  The API returns a bit status of the bit in the table array which contains the masks.
* 
* Parameters:
*  table[] - array with bit masks.
*  position - position of bit in the table[] array.
*
* Return:
*  0 - bit is not set; 1 - bit is set.
*
* Global Variables:
*  None
*
* Side Effects:
*  None
*
*******************************************************************************/
uint32 Slider_GetBitValue(const uint8 table[], uint32 position)
{
    uint32 offset;
	uint8 enMask;
	
	/* position is divided by 8 to calculate the element of the 
	   table[] array that contains the enable bit 
	   for an appropriate sensor.
	*/
	offset = (position >> 3u);
	
	/* The enMask calculation for the appropriate sensor. Operation (position & 0x07u) 
	   intends to calculate the enable bit offset for the 8-bit element of the 
	   table[] array.
	*/
	enMask = 0x01u << (position & 0x07u);
    
    return ((0u !=(table[offset] & enMask)) ? 1Lu : 0Lu);
}


/*******************************************************************************
* Function Name: Slider_SetBitValue
********************************************************************************
*
* Summary:
*  The API sets a bit status of the bit in the table array which contains masks.
* 
* Parameters:
*  table[] - array with bit masks.
*  position - position of bit in the table[] array.
*  value: 0 - bit is not set; 1 - bit is set.
*
* Return:
*  None
*
* Global Variables:
*  None
*
* Side Effects:
*  None
*
*******************************************************************************/
void Slider_SetBitValue(uint8 table[], uint32 position, uint32 value)
{
    uint32 offset;
	uint8 enMask;
	
	/* position is divided by 8 to calculate the element of the 
	   table[] array that contains the enable bit 
	   for an appropriate sensor.
	*/
	offset = (position >> 3u);
	
	/* The enMask calculation for the appropriate sensor. Operation (position & 0x07u) 
	   intends to calculate the enable bit offset for the 8-bit element of the 
	   table[] array.
	*/
	enMask = 0x01u << (position & 0x07u);
	
	if(0u != value)
	{
		table[offset] |= enMask;
	}
	else
	{
		table[offset] &= (uint8)~(enMask);
	}
}



/*******************************************************************************
* Function Name: Slider_GetSenseClkDivider
********************************************************************************
*
* Summary:
*  This API returns a value of the sense clock divider for the  sensor.
*
* Parameters:
*  sensor: sensor index. The index value can be 
*  from 0 to (Slider_TOTAL_SENSOR_COUNT-1).
*
* Return:
*  This API returns the sense clock divider of the sensor. 
*
* Global Variables:
*  Slider_senseClkDividerVal[] - stores the sense clock divider values.
* 
* Side Effects:
*  None
*
*******************************************************************************/
uint32 Slider_GetSenseClkDivider(uint32 sensor)
{
	#if(0u != Slider_MULTIPLE_FREQUENCY_SET)
		return((uint32)Slider_senseClkDividerVal[sensor]);
	#else
		return((uint32)Slider_senseClkDividerVal);
	#endif /* (0u != Slider_MULTIPLE_FREQUENCY_SET) */
}

#if (Slider_TUNING_METHOD != Slider__TUNING_NONE)
	/*******************************************************************************
	* Function Name: Slider_SetSenseClkDivider
	********************************************************************************
	*
	* Summary:
	*  This API sets a value of the sense clock divider for the  sensor. 
	*
	* Parameters:
	*  sensor:  Sensor index.
	*  senseClk: represents the sense clock value.
	*
	* Return:
	*  None.
	*
	* Global Variables:
	*  Slider_senseClkDividerVal[] - stores the sense clock divider values.
	* 
	* Side Effects:
	*  None
	*
	*******************************************************************************/
	void Slider_SetSenseClkDivider(uint32 sensor, uint32 senseClk)
	{
		#if(0u != Slider_MULTIPLE_FREQUENCY_SET)
			Slider_senseClkDividerVal[sensor] = (uint8)senseClk;
		#else
			Slider_senseClkDividerVal = (uint8)senseClk;
		#endif /* (0u != Slider_MULTIPLE_FREQUENCY_SET) */
	}
#endif /* (Slider_TUNING_METHOD != Slider__TUNING_NONE) */


/*******************************************************************************
* Function Name: Slider_GetModulatorClkDivider
********************************************************************************
*
* Summary:
*  This API returns a value of the modulator sample clock divider for the  sensor.
*
* Parameters:
*  sensor: sensor index. The value of index can be 
*  from 0 to (Slider_TOTAL_SENSOR_COUNT-1).
*
* Return:
*  This API returns the modulator sample clock divider for the  sensor.
*
* Global Variables:
*  Slider_sampleClkDividerVal[] - stores the modulator sample divider values.
* 
* Side Effects:
*  None
*
*******************************************************************************/
uint32 Slider_GetModulatorClkDivider(uint32 sensor)
{
	#if(0u != Slider_MULTIPLE_FREQUENCY_SET)
		return((uint32)Slider_sampleClkDividerVal[sensor]);
	#else
		return((uint32)Slider_sampleClkDividerVal);
	#endif /* (0u != Slider_MULTIPLE_FREQUENCY_SET) */
}

#if (Slider_TUNING_METHOD != Slider__TUNING_NONE)
	/*******************************************************************************
	* Function Name: Slider_SetModulatorClkDivider
	********************************************************************************
	*
	* Summary:
	*  This API sets a value of the modulator sample clock divider for the  sensor.
	*
	* Parameters:
	*  sensor:  Sensor index.
	*  modulatorClk: represents the modulator sample clock value.
	*
	* Return:
	*  None.
	*
	* Global Variables:
	*  Slider_sampleClkDividerVal[] - stores the modulator sample divider values.
	* 
	* Side Effects:
	*  None
	*
	*******************************************************************************/
	void Slider_SetModulatorClkDivider(uint32 sensor, uint32 modulatorClk)
	{
		#if(0u != Slider_MULTIPLE_FREQUENCY_SET)
			Slider_sampleClkDividerVal[sensor] = (uint8)modulatorClk;
		#else
			Slider_sampleClkDividerVal = (uint8)modulatorClk;
		#endif /* (0u != Slider_MULTIPLE_FREQUENCY_SET) */
	}
#endif /* (Slider_TUNING_METHOD != Slider__TUNING_NONE) */

/*******************************************************************************
* Function Name: Slider_GetModulationIDAC
********************************************************************************
*
* Summary:
*  This API returns a value of the modulation IDAC for the  sensor.
*
* Parameters:
*  sensor: sensor index. The index value can be 
*  from 0 to (Slider_TOTAL_SENSOR_COUNT-1).
*
* Return:
*  This API returns the modulation IDAC of the sensor. 
*
* Global Variables:
*  Slider_modulationIDAC[] - stores modulation IDAC values.
* 
* Side Effects:
*  None
*
*******************************************************************************/
uint32 Slider_GetModulationIDAC(uint32 sensor)
{
		return((uint32)Slider_modulationIDAC[sensor]);
}

#if (Slider_TUNING_METHOD != Slider__TUNING_NONE)
	/*******************************************************************************
	* Function Name: Slider_SetModulationIDAC
	********************************************************************************
	*
	* Summary:
	*  This API sets a value of the modulation IDAC for the  sensor.
	*
	* Parameters:
	*  sensor:  Sensor index.
	*  compIdacValue: represents the modulation IDAC data register value.
	*
	* Return:
	*  None.
	*
	* Global Variables:
	*  Slider_modulationIDAC[] - array with modulation IDAC values
	* 
	* Side Effects:
	*  None
	*
	*******************************************************************************/
	void Slider_SetModulationIDAC(uint32 sensor, uint32 modIdacValue)
	{
		Slider_modulationIDAC[sensor] = (uint8)modIdacValue;
	}
#endif /* (Slider_TUNING_METHOD != Slider__TUNING_NONE) */


#if(Slider_IDAC_CNT > 1u)
	/*******************************************************************************
	* Function Name: Slider_GetCompensationIDAC
	********************************************************************************
	*
	* Summary:
	*  This API returns a value of the compensation IDAC for the  sensor.
	*
	* Parameters:
	*  sensor: sensor index. The index value can be 
	*  from 0 to (Slider_TOTAL_SENSOR_COUNT-1).
	*
	* Return:
	*  This API returns the compensation IDAC of the sensor. 
	*
	* Global Variables:
	*  Slider_compensationIDAC[] - stores the compensation IDAC values.
	* 
	* Side Effects:
	*  None
	*
	*******************************************************************************/
	uint32 Slider_GetCompensationIDAC(uint32 sensor)
	{
		return((uint32)Slider_compensationIDAC[sensor]);
	}
#endif /* (Slider_IDAC_CNT > 1u) */


#if((Slider_TUNING_METHOD != Slider__TUNING_NONE) && (Slider_IDAC_CNT > 1u))
	/*******************************************************************************
	* Function Name: Slider_SetCompensationIDAC
	********************************************************************************
	*
	* Summary:
	*  This API sets a value of compensation IDAC for the  sensor.
	*
	* Parameters:
	*  sensor:  Sensor index.
	*  compIdacValue: represents the compensation IDAC data register value.
	*
	* Return:
	*  None.
	*
	* Global Variables:
	*  Slider_compensationIDAC[] - an array with compensation IDAC values
	* 
	* Side Effects:
	*  None
	*
	*******************************************************************************/
	void Slider_SetCompensationIDAC(uint32 sensor, uint32 compIdacValue)
	{
		Slider_compensationIDAC[sensor] = (uint8)compIdacValue;
	}
#endif /* ((Slider_TUNING_METHOD != Slider__TUNING_NONE) && (Slider_IDAC_CNT > 1u)) */

/*******************************************************************************
* Function Name: Slider_GetIDACRange
********************************************************************************
*
* Summary:
*  This API returns a value that indicates the IDAC range used by the 
*  component to scan sensors. The IDAC range is common for all the sensors.
*
* Parameters:
*  None
*
* Return:
*  This API Returns a value that indicates the IDAC range:
*      0 - IDAC range set to 4x
*      1 - IDAC range set to 8x
*
* Global Variables:
*  None
* 
* Side Effects:
*  None
*
*******************************************************************************/
uint32 Slider_GetIDACRange(void)
{
	return((0u != (Slider_CSD_IDAC_REG & Slider_CSD_IDAC1_RANGE_8X)) ? 1uL : 0uL);
}

#if (Slider_TUNING_METHOD != Slider__TUNING_NONE)
	/*******************************************************************************
	* Function Name: Slider_SetIDACRange
	********************************************************************************
	*
	* Summary:
	*  This API sets the IDAC range to the 4x (1.2uA/bit) or 8x (2.4uA/bit) mode.
    *  The IDAC range is common for all the sensors and common for the modulation and 
	*  compensation IDACs.
	*
	* Parameters:
	*  iDacRange:  represents value for IDAC range 
	*  0 -  IDAC range set to 4x (1.2uA/bit)
	*  1 or >1 - IDAC range set to 8x (2.4uA/bit)
	*
	* Return:
	*  None.
	*
	* Global Variables:
	*  None
	* 
	* Side Effects:
	*  None
	*
	*******************************************************************************/
	void Slider_SetIDACRange(uint32 iDacRange)
	{
	    if(iDacRange != 0u)
		{
		    /*  IDAC range = 8x (2.4uA/bit) */ 
		    Slider_CSD_IDAC_REG |= (Slider_CSD_IDAC1_RANGE_8X |\
											  Slider_CSD_IDAC2_RANGE_8X);
		}
		else
		{
		    /*  IDAC range = 4x (1.2uA/bit) */ 
		    /*  IDAC range = 8x (2.4uA/bit) */ 
		    Slider_CSD_IDAC_REG &= ~(Slider_CSD_IDAC1_RANGE_8X |\
											   Slider_CSD_IDAC2_RANGE_8X);
		}
	}
#endif /* (Slider_TUNING_METHOD != Slider__TUNING_NONE) */


#if((0u != Slider_AUTOCALIBRATION_ENABLE) || (Slider_TUNING_METHOD == Slider__TUNING_AUTO))
	/*******************************************************************************
	* Function Name: Slider_GetSensorRaw
	********************************************************************************
	*
	* Summary:
	*  The API updates and gets uprated raw data from the sensor.
	*
	* Parameters:
	*  sensor - Sensor number.
	*
	* Return:
	*  Returns the current raw data value for a defined sensor number.
	*
	* Global Variables:
	*  None
	*
	* Side Effects:
	*  None
	*
	*******************************************************************************/
	uint16  Slider_GetSensorRaw(uint32 sensor)
	{
		uint32 curSample;
		uint32 avgVal = 0u;
		
		for(curSample = 0u; curSample < Slider_AVG_SAMPLES_NUM; curSample++)
		{
	    
			Slider_ScanSensor((uint32)sensor);
		    while(Slider_IsBusy() == 1u)
			{
				/* Wait while sensor is busy */
			}
			avgVal += Slider_ReadSensorRaw((uint32)sensor);
		}
	    
	    return((uint16)(avgVal / Slider_AVG_SAMPLES_NUM));
	}
#endif /* ((0u != Slider_AUTOCALIBRATION_ENABLE) && (Slider_TUNING_METHOD == Slider__TUNING_AUTO)) */


#if((0u != Slider_AUTOCALIBRATION_ENABLE) && (Slider_TUNING_METHOD != Slider__TUNING_AUTO))
	/*******************************************************************************
	* Function Name: Slider_CalibrateSensor
	********************************************************************************
	*
	* Summary:
	*  Computes the one sensor IDAC value, which provides the raw signal on
	*  a specified level, with a specified prescaler, speed, and resolution.
	*
	* Parameters:
	*  sensor - Sensor Number.
	*
	*  rawLevel -  Raw data level which should be reached during the calibration
	*              procedure.
	*
	*  idacLevelsTbl - Pointer to the modulatorIdac data register configuration table.            
	*
	* Return:       
	*  None
	*
	*******************************************************************************/
	void Slider_CalibrateSensor(uint32 sensor, uint32 rawLevel, uint8 idacLevelsTbl[])
	{
		uint8 mask;
		uint16 rawData;
		
		rawData = 0u;
		
		mask = 0x80u;
		/* Init IDAC for null Channel */
		idacLevelsTbl[sensor] = 0x00u;
		
		do
		{
		    /* Set bit for null Channel */
			idacLevelsTbl[sensor] |= mask;

	        /* Scan null Channel and get Rawdata */
			rawData = Slider_GetSensorRaw(sensor);
			
			/* Decrease IDAC until Rawdata reaches rawLevel */ 
			if(rawData < rawLevel)
			{
			    /* Reset bit for null Channel  */ 
				idacLevelsTbl[sensor] &= (uint8)~mask;
			}
			
			mask >>= 1u;
		}
		while(mask > 0u);
		
	}
#endif /* ((0u != Slider_AUTOCALIBRATION_ENABLE) && (Slider_TUNING_METHOD != Slider__TUNING_AUTO)) */


/*******************************************************************************
* Function Name: Slider_SetUnscannedSensorState
********************************************************************************
*
* Summary:
*  This API sets a state for un-scanned sensors. 
*  It is possible to set the state to Ground, High-Z, or the shield electrode. 
*  The un-scanned sensor can be connected to a shield electrode only if the shield is 
*  enabled. If the shield is disabled and this API is called with the parameter which
*  indicates the shield state, the un-scanned sensor will be connected to Ground.
*
* Parameters:
*  sensor - Sensor Number.
*  sensorState: This parameter indicates un-scanned sensor state:
*
*     Slider__GROUND 0
*     Slider__HIZ_ANALOG 1
*     Slider__SHIELD 2
*
* Return:
*  None.
*
* Global Variables:
*  Slider_unscannedSnsDriveMode[] - used to store the state for
*  un-scanned sensors.
* 
* Side Effects:
*  None
*
*******************************************************************************/
void Slider_SetUnscannedSensorState(uint32 sensor, uint32 sensorState)
{
    Slider_unscannedSnsDriveMode[sensor] = (uint8)sensorState;	
}

#if(0u != Slider_TOTAL_CENTROIDS_COUNT) 
/*******************************************************************************
* Function Name: Slider_NormalizeWidgets
********************************************************************************
*
* Summary:
*  This API aligns all the parameters of the widget to the maximum parameter.
*
* Parameters:
*  uint32 widgetsNum: Number of widgets.
*  uint8 *dataPrt: pointer to array with widget parameters.	
*
* Return:
*  None
*
* Global Variables:
*  Slider_numberOfSensors[] - Number of sensors in the widget.
*  Slider_rawDataIndex[currentWidget] - Contains the  1st sensor
*  position in the widget.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Slider_NormalizeWidgets(uint32 widgetsNum, uint8 *dataPtr)
{
	uint32 currentWidget;
	uint32 currentSensor;
	uint32 lastSensor;
	uint32 sensorsPerWidget;
	uint32 maxPerWidget;

	for(currentWidget = 0u; currentWidget < widgetsNum; currentWidget++)
	{
		maxPerWidget = 0u;
		
		sensorsPerWidget = Slider_numberOfSensors[currentWidget];
		currentSensor = Slider_rawDataIndex[currentWidget];
		lastSensor = currentSensor + sensorsPerWidget;
		
		while(currentSensor < lastSensor)
		{
			if(maxPerWidget < dataPtr[currentSensor])
			{
				maxPerWidget = dataPtr[currentSensor];
			}
			currentSensor++;
		}
		
		currentSensor = Slider_rawDataIndex[currentWidget];
		
		while(currentSensor < lastSensor)
		{
			dataPtr[currentSensor] = (uint8)maxPerWidget;
			currentSensor++;
		}
	}
}
#endif /* (0u != Slider_TOTAL_CENTROIDS_COUNT) */

/* [] END OF FILE */
