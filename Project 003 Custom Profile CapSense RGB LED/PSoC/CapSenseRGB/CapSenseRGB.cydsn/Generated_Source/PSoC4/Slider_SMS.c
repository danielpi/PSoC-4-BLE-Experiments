/*******************************************************************************
* File Name: Slider_SMS.c
* Version 2.20
*
* Description:
*  This file provides the source code of wrapper between CapSense CSD component 
*  and Auto Tuning library.
*
* Note:
*
********************************************************************************
* Copyright 2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Slider_SMS.h"
#include "Slider_PVT.h"

#include "cytypes.h"

#if (Slider_TUNING_METHOD == Slider__TUNING_AUTO)


/*  Real Charge Divider values */
#if (CY_PSOC4_4100BL || CY_PSOC4_4200BL || CY_PSOC4_4100M || CY_PSOC4_4200M)
    /*  Dividers are not chained */
	#if (CYDEV_BCLK__HFCLK__MHZ > 24u)
		const uint8 prescalerTable[Slider_PRESCALERS_TBL_SIZE] = {
			4u,     4u,     4u,    8u,    8u,    8u,    8u,    16u,    16u,    16u,    16u,    16u,    16u,    16u,    16u,    16u
		};
	#elif (CYDEV_BCLK__HFCLK__MHZ >12u)
		const uint8 prescalerTable[Slider_PRESCALERS_TBL_SIZE] = {
			2u,     2u,     2u,    4u,    4u,    4u,    4u,    8u,    8u,    8u,    8u,    8u,    8u,    8u,    8u,    8u
		};
	#else   /* (CYDEV_BCLK__HFCLK__MHZ > 12u) */
		const uint8 prescalerTable[Slider_PRESCALERS_TBL_SIZE] = {
			1u,     1u,     1u,    2u,    2u,    2u,    2u,    4u,    4u,    4u,    4u,    4u,    4u,    4u,    4u,    4u
		};
	#endif /* (CYDEV_BCLK__HFCLK__MHZ > 24u) */
	
#elif (0u != Slider_IS_M0S8PERI_BLOCK)
    /*  Dividers are not chained (PSoC 4000) */
	#if (CYDEV_BCLK__HFCLK__MHZ >= 12u)
		const uint8 prescalerTable[Slider_PRESCALERS_TBL_SIZE] = {
			1u,     1u,     1u,    2u,    2u,    2u,    2u,    4u,    4u,    4u,    4u,    4u,    4u,    4u,    4u,    4u
		};

	#elif (CYDEV_BCLK__HFCLK__MHZ >= 6u)
		const uint8 prescalerTable[Slider_PRESCALERS_TBL_SIZE] = {
			1u,     1u,     1u,    1u,    1u,    1u,    1u,    2u,    2u,    2u,    2u,    2u,    2u,    2u,    2u,    2u
		};
	#else
		const uint8 prescalerTable[Slider_PRESCALERS_TBL_SIZE] = {
			1u,     1u,     1u,    1u,    1u,    1u,    1u,    1u,    1u,    1u,    1u,    1u,    1u,    1u,    1u,    1u
		};
	#endif /* (CYDEV_BCLK__HFCLK__MHZ > 12u) */

#else
    /*  Dividers are chained (PSoC 4100, PSoC 4200) */ 
	#if (CYDEV_BCLK__HFCLK__MHZ > 24u)
		const uint8 prescalerTable[Slider_PRESCALERS_TBL_SIZE] = {
			2u,     2u,     2u,    4u,    4u,    4u,    4u,    8u,    8u,    8u,    8u,    8u,    8u,    8u,    8u,    8u
		};

	#elif (CYDEV_BCLK__HFCLK__MHZ >12u)
		const uint8 prescalerTable[Slider_PRESCALERS_TBL_SIZE] = {
			1u,     1u,     1u,    2u,    2u,    2u,    2u,    4u,    4u,    4u,    4u,    4u,    4u,    4u,    4u,    4u
		};
	#else   /* (CYDEV_BCLK__HFCLK__MHZ > 12u) */
		const uint8 prescalerTable[Slider_PRESCALERS_TBL_SIZE] = {
			1u,     1u,     1u,    1u,    1u,    1u,    1u,    2u,    2u,    2u,    2u,    2u,    2u,    2u,    2u,    2u
		};
	#endif /* (CYDEV_BCLK__HFCLK__MHZ > 24u) */
#endif /* (CY_PSOC4_4100BL || CY_PSOC4_4200BL || CY_PSOC4_4100M || CY_PSOC4_4200M) */


uint8 noiseEnvelopeTbl[Slider_TOTAL_SENSOR_COUNT];
uint8 runningDifferenceTbl[Slider_TOTAL_SENSOR_COUNT];
uint8 signRegisterTbl[Slider_TOTAL_SENSOR_COUNT];
uint16 sampleMinTbl[Slider_TOTAL_SENSOR_COUNT];
uint16 sampleMaxTbl[Slider_TOTAL_SENSOR_COUNT];
uint16 previousSampleTbl[Slider_TOTAL_SENSOR_COUNT];
uint8 kValueTbl[Slider_TOTAL_SENSOR_COUNT];

uint8 SMS_LIB_SensorSensitivity[] = {
    2u, 2u, 2u, 2u, 2u, 
};

Slider_CONFIG_TYPE_P4_v2_20 Slider_config;
Slider_CONFIG_TYPE_POINTERS_P4_v2_20 const Slider_configPointers = {
    Slider_modulationIDAC,
    Slider_compensationIDAC,
     SMS_LIB_SensorSensitivity,
    Slider_senseClkDividerVal,
    Slider_sampleClkDividerVal,
    Slider_widgetNumber,
    Slider_widgetResolution,
	(const uint8  *)Slider_rawDataIndex,
	(const uint8  *)Slider_numberOfSensors,
    &Slider_GetSensorRaw,
    &Slider_PreScan,
    &Slider_ReadSensorRaw,
    &Slider_GetBitValue,
	&Slider_SetBitValue
};


/*******************************************************************************
* Function Name: Slider_AutoTune
********************************************************************************
*
* Summary:
*  Provides the tuning procedure for all sensors.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*   Slider_config: the structure which contains configuration parameters
*   Slider_senseClkDividerVal[]: used to store the Analog Switch 
*   dividers for each sensor.
*   Slider_senseClkDividerVal[]: used to store the 
*    Analog Switch divider for all sensors.
*   Slider_prescalersTuningDone - used to notify the Tuner GUI that 
*   the pre-scalers tuning  is done.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Slider_AutoTune(void)
{
	uint32 curSensor;
    uint32 widget;

	Slider_config.totalScanslotsNum = Slider_TOTAL_SCANSLOT_COUNT;
	Slider_config.totalWidgetsNum = Slider_END_OF_WIDGETS_INDEX;
	Slider_config.totalSensorsNum = Slider_TOTAL_SENSOR_COUNT;
	
	Slider_config.hfclkFreq_MHz = CYDEV_BCLK__HFCLK__MHZ;
	Slider_config.sensetivitySeed = Slider_SENSETIVITY_FACTOR;
	
    Slider_config.pointers = &Slider_configPointers;       
    
	Slider_SetAnalogSwitchesSrcDirect();
    
    for(widget = 0u; widget < Slider_config.totalWidgetsNum; widget++)
    {
        Slider_widgetResolution[widget] = Slider_CALIBRATION_RESOLUTION;
    }
	

	for(curSensor = 0u; curSensor < Slider_TOTAL_SCANSLOT_COUNT; curSensor++)
    {
		Slider_senseClkDividerVal[curSensor] = Slider_CALIBRATION_ASD;
		Slider_sampleClkDividerVal[curSensor] = Slider_CALIBRATION_MD;
	}
    
    Slider_DisableBaselineIDAC();

    CalibrateSensors_P4_v2_20(&Slider_config, Slider_CAL_RAW_COUNT);

	#if(0u != Slider_TOTAL_CENTROIDS_COUNT) 
		Slider_NormalizeWidgets(Slider_END_OF_WIDGETS_INDEX, Slider_modulationIDAC);
	#endif /* (0u != Slider_TOTAL_CENTROIDS_COUNT)  */
	
    TunePrescalers_P4_v2_20(&Slider_config);
	
	#if(Slider_PRS_OPTIONS != Slider__PRS_NONE)
		Slider_prescalersTuningDone = 1u;
	#endif /* (Slider_PRS_OPTIONS == Slider__PRS_NONE) */
       
    CalibrateSensors_P4_v2_20(&Slider_config, Slider_CAL_RAW_COUNT);
	#if(0u != Slider_TOTAL_CENTROIDS_COUNT)
		Slider_NormalizeWidgets(Slider_END_OF_WIDGETS_INDEX, Slider_modulationIDAC);
	#endif /* (0u != Slider_TOTAL_CENTROIDS_COUNT)  */
    TuneSensitivity_P4_v2_20(&Slider_config);
	
	Slider_EnableBaselineIDAC(&Slider_config);
    
    for(curSensor = 0u; curSensor < Slider_TOTAL_SCANSLOT_COUNT; curSensor++)
    {
        Slider_UpdateThresholds(curSensor);
    }
}


/*******************************************************************************
* Function Name: Slider_UpdateThresholds
********************************************************************************
*
* Summary:
*  The API updates the Finger Threshold, Hysteresis, Noise Threshold, and 
*  Negative Noise Threshold in the AutoTuning (Smartsense) Mode.
*
* Parameters:
*  sensor: sensor number.
*
* Return:
*  None
*
* Global Variables:
*  Slider_widgetNumber[] - contains widget Number for given sensor.
*  0 through 7 (sensor 0 is bit 0, sensor 1 is bit 1).
*  For other sensors the array element equals to 255. 
*  Slider_numberOfSensors[widget] - Number of sensors in the widget.
*   Slider_fingerThreshold[] - contains the level of signal for each sensor
*   that determines if a finger present on the sensor.
*   Slider_negativeNoiseThreshold[] - negative Noise Threshold
*   Slider_hysteresis[] - the array with hysteresis values.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Slider_UpdateThresholds(uint32 sensor)
{
	uint8 fingerThreshold;
	uint32 widget;
	
	widget = Slider_widgetNumber[sensor];

	#if(0u != Slider_FLEXIBLE_THRESHOLDS_EN)
	    fingerThreshold = Slider_fingerThreshold[widget];
        /* Update Threshold manually (flexible) */   
		Slider_noiseThreshold[widget] = (uint8)(fingerThreshold >> 1u);
		Slider_negativeNoiseThreshold[widget] = (uint8)(fingerThreshold >> 1u);
		Slider_hysteresis[widget] = (uint8)(fingerThreshold >> 3u);
	#else
		/* Calculate Finger Threshold and Noise Threshold with Smartsense (automatic) */ 	
		CalculateThresholds_P4_v2_20(&Slider_config, (uint8)sensor, Slider_sensorRaw, (uint8)widget, Slider_fingerThreshold, Slider_noiseThreshold);
		
	    fingerThreshold = Slider_fingerThreshold[widget];
		
	    /* Update Threshold based on calculated with Smartsense (automatic) */
		Slider_negativeNoiseThreshold[widget] = Slider_noiseThreshold[widget];
        
		if(fingerThreshold < Slider_THRESHOLD_1)
		{
			Slider_hysteresis[widget] = (uint8)(fingerThreshold >> 3u);
		}
		else if(fingerThreshold <  Slider_THRESHOLD_2)
		{
			Slider_hysteresis[widget] = (uint8)(fingerThreshold >> 4u);
		}
		else if(fingerThreshold <  Slider_THRESHOLD_3)
		{
			Slider_hysteresis[widget] = (uint8)(fingerThreshold >> 5u);
		}
		else if(fingerThreshold <  Slider_THRESHOLD_4)
		{
			Slider_hysteresis[widget] = (uint8)(fingerThreshold >> 6u);
		}
		else
		{
			Slider_hysteresis[widget] = 0u;
		}
	#endif /* (0u != Slider_FLEXIBLE_THRESHOLDS_EN)  */
}

/*******************************************************************************
* Function Name: Slider_SetAnalogSwitchesSrcDirect
********************************************************************************
*
* Summary:
*  This API switches the charge clock source to prescaler. 
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
void Slider_SetAnalogSwitchesSrcDirect(void)
{
	Slider_CSD_CFG_REG &= ~(Slider_CSD_CFG_PRS_SELECT);
}


/*******************************************************************************
* Function Name: Slider_DisableBaselineIDAC
********************************************************************************
*
* Summary:
*  The API disables the Compensation IDAC. 
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
void Slider_DisableBaselineIDAC(void)
{
	uint32 newRegValue;
    
    newRegValue = Slider_CSD_IDAC_REG;
    
    newRegValue &= ~(Slider_CSD_IDAC1_MODE_MASK | Slider_CSD_IDAC2_MODE_MASK);
    newRegValue |= Slider_CSD_IDAC1_MODE_VARIABLE;
    
	Slider_CSD_IDAC_REG = newRegValue;
}


/*******************************************************************************
* Function Name: Slider_EnableBaselineIDAC
********************************************************************************
*
* Summary:
*  The API enables the Compensation IDAC. 
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  Slider_compensationIDAC[] - used to store a 7-bit idac value for all the sensors.
*
* Side Effects:
*  None
*
*******************************************************************************/
void  Slider_EnableBaselineIDAC(const Slider_CONFIG_TYPE_P4_v2_20 *config)
{
    uint32 curSensor;
    uint32 newRegValue;

    for(curSensor = 0u; curSensor < config->totalScanslotsNum; curSensor++)
    {
		Slider_compensationIDAC[curSensor] = Slider_modulationIDAC[curSensor] / 2u;
	    Slider_modulationIDAC[curSensor] = (Slider_modulationIDAC[curSensor] + 1u) / 2u;
    }
	
	CyIntDisable(Slider_ISR_NUMBER);
	
	newRegValue = Slider_CSD_IDAC_REG;
    newRegValue &= ~(Slider_CSD_IDAC1_MODE_MASK | Slider_CSD_IDAC2_MODE_MASK);
    newRegValue |= (Slider_CSD_IDAC1_MODE_VARIABLE | Slider_CSD_IDAC2_MODE_FIXED);
    Slider_CSD_IDAC_REG = newRegValue;
	
	CyIntEnable(Slider_ISR_NUMBER);
}


/*******************************************************************************
* Function Name: Slider_SetSensitivity
********************************************************************************
*
* Summary:
*  This API sets the sensitivity value for the sensor. The sensitivity 
*  value is used during the auto-tuning algorithm executed as part of the CapSense_Start API.
*  This API is called by the application layer prior to calling the CapSense_Start API. 
*  Calling this API after execution of CapSense_Start API has no effect. 
*
* Parameters: 
*  sensor  Sensor index 
*  data    Sensitivity of the sensor. Possible values are below
*  1 - 0.1pf sensitivity
*  2 - 0.2pf sensitivity
*  3 - 0.3pf sensitivity
*  4 - 0.4pf sensitivity
*  5 - 0.5pf sensitivity
*  6 - 0.6pf sensitivity
*  7 - 0.7pf sensitivity
*  8 - 0.8pf sensitivity
*  9 - 0.9pf sensitivity
*  10 - 1.0pf sensitivity
*  All other values, set sensitivity to 1.0pf
*
*  Return Value:
*   None
*
* Global Variables:
* SMS_LIB_SensorSensitivity[] - This 8-bits array contains Sensitivity in the LSB nibble
*                                and Noise Reset Counter value in the MSB nibble.      
*
* Side Effects: 
*  None
*
*******************************************************************************/
void Slider_SetSensitivity(uint32 sensor, uint32 value)
{
	if(value > 10u)
	{
		value = 10u;
	}
	
    /* Clear SensorSensitivity value in LSB nibble */
	SMS_LIB_SensorSensitivity[sensor] &= (uint8)~Slider_SENSITIVITY_MASK;
	/* Set SensorSensitivity value in LSB nibble */
	SMS_LIB_SensorSensitivity[sensor] |= (uint8)value;
}


/*******************************************************************************
* Function Name: Slider_GetSensitivityCoefficient
********************************************************************************
*
* Summary:
*  This API returns the K coefficient for the appropriate sensor. 
*
* Parameters: 
*  sensor:  Sensor index 
*
*  Return Value:
*   K - value for the appropriate sensor.
*
* Global Variables:
*  kValueTbl[] - This 8-bits array contains the K value for each sensor.
*
* Side Effects: 
*  None
*
*******************************************************************************/
uint8 Slider_GetSensitivityCoefficient(uint32 sensor)
{
	return (kValueTbl[sensor]);
}


/*******************************************************************************
* Function Name: Slider_GetNoiseEnvelope
********************************************************************************
*
* Summary:
*  This function returns the noise value of a
*  component.
*
* Parameters:
*  sensor - Sensor number.
*
* Return:
*  The noise envelope value of the sensor
*  indicated by argument.
*
* Global Variables:
*  baNoiseEnvelope[] - array with Noise Envelope.
*
* Side Effects:
*  None
*
*******************************************************************************/
uint16 Slider_GetNoiseEnvelope(uint32 sensor)
{
    return((uint16)((uint16)noiseEnvelopeTbl[sensor] << 1u) + 1u);
}


/*******************************************************************************
* Function Name: Slider_GetNormalizedDiffCountData
********************************************************************************
*
* Summary:
*  This API returns normalized difference count data.
*
* Parameters:
*  sensor:  Sensor number.
*
* Return:
*  This API returns a normalized count value of the sensor indicated by the 
*  argument. 
*
* Global Variables:
*  Slider_sensorSignal[] - array with difference counts for sensors
* 
* Side Effects:
*  None
*
*******************************************************************************/
uint16 Slider_GetNormalizedDiffCountData(uint32 sensor)
{
	return (uint16)(((uint32)Slider_sensorSignal[sensor] << 7u) / kValueTbl[sensor]);
}

#endif /* (Slider_TUNING_METHOD == Slider__TUNING_AUTO) */

/* [] END OF FILE */
