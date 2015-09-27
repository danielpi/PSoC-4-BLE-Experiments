/*******************************************************************************
* File Name: Slider_PVT.h
* Version 2.20
*
* Description:
*  This file provides constants and structure declarations for the
*  CapSense CSD component.
*
* Note:
*
********************************************************************************
* Copyright 2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#if !defined(CY_CAPSENSE_CSD_PVT_Slider_H)
#define CY_CAPSENSE_CSD_PVT_Slider_H

#include "Slider.h"

#if(0u != Slider_CSHL_API_GENERATE)
	#include "Slider_CSHL.h"
#endif /* (0u != Slider_CSHL_API_GENERATE) */

#if(Slider_TOTAL_TRACKPAD_GESTURES_COUNT > 0u)
	#include "Slider_GESTURE.h"
#endif /* (Slider_TOTAL_TRACKPAD_GESTURES_COUNT > 0u) */
	
typedef struct
{
	#if (Slider_IS_COMPLEX_SCANSLOTS)
	uint8 sensor;
	#endif  /* Slider_IS_COMPLEX_SCANSLOTS */
	uint8 pinNumber;
	uint8 portNumber;
	uint8 idacValue;
	uint16 senseClkDiv;
	uint16 sampleClkDiv;
	uint8 avgSamplesNum;
	uint16 calibrationCount;
	uint32 scanResolution;
}Slider_CP_MEASURE_CONFIG;

typedef struct
{
    uint8 *idac1LevelsTbl;
    uint8 *idac2LevelsTbl;
    uint8 *sensetivityTbl;
    uint8 *chargeDivTbl;
    uint8 *modDivTbl;
    const uint8  *widgetNubmersTbl;
    uint32 *resolutionsTbl;
	uint8 const  *rawDataIndex;
	uint8 const *numberOfSensors;
    uint16 (*ptrGetRaw)(uint32 sensor);
	void (*ptrPreScan)(uint32 sensor);
	uint16 (*ptrReadSensorRaw)(uint32 sensor);
    uint32 (*ptrGetBitValue)(const uint8 table[], uint32 position);
	void (*ptrSetBitValue)(uint8 table[], uint32 position, uint32 value);
}Slider_CONFIG_TYPE_POINTERS_P4_v2_20;

typedef struct
{
    uint8 totalScanslotsNum;
	uint8 totalWidgetsNum;
    uint8 totalSensorsNum;
	uint16 sensetivitySeed;
	uint16 hfclkFreq_MHz;
    Slider_CONFIG_TYPE_POINTERS_P4_v2_20 const * pointers;
}Slider_CONFIG_TYPE_P4_v2_20;

#define Slider_AVG_SAMPLES_NUM		(1u)


/***************************************
*        Function Prototypes
***************************************/
void Slider_PreScan(uint32 sensor);
void Slider_PostScan(uint32 sensor);

void Slider_EnableSensor(uint32 sensor);
void Slider_DisableSensor(uint32 sensor);

#if (Slider_IS_COMPLEX_SCANSLOTS)
    void Slider_EnableScanSlot(uint32 slot);
    void Slider_DisableScanSlot(uint32 slot);
#else
    #define Slider_EnableScanSlot(slot)   Slider_EnableSensor(slot)
    #define Slider_DisableScanSlot(slot)  Slider_DisableSensor(slot)
#endif  /* End Slider_IS_COMPLEX_SCANSLOTS */

void Slider_EnableShieldElectrode(uint32 pinNumber, uint32 portNumber);

uint32 Slider_GetBitValue(const uint8 table[], uint32 position);
void Slider_SetBitValue(uint8 table[], uint32 position, uint32 value);

/* Find next sensor for One Channel design */
uint8 Slider_FindNextSensor(uint8 snsIndex);

void Slider_SetPinDriveMode(uint32 driveMode, uint32 pinNumber, uint32 portNumber);
void Slider_CalibrateSensor(uint32 sensor, uint32 rawLevel, uint8 idacLevelsTbl[]);
uint16  Slider_GetSensorRaw(uint32 sensor);

void Slider_DisableBaselineIDAC(void);
void Slider_EnableBaselineIDAC(const Slider_CONFIG_TYPE_P4_v2_20 *config);

uint32 Slider_GetLowCp(Slider_CP_MEASURE_CONFIG *config);
uint32 Slider_GetHighCp(const Slider_CP_MEASURE_CONFIG *config);
uint32 Slider_CalibrateIDAC(Slider_CP_MEASURE_CONFIG *config);
uint16 Slider_ScanSensorCp(const Slider_CP_MEASURE_CONFIG *config);

void Slider_ConnectElectrode(uint32 pinNumber, uint32 portNumber);
void Slider_DisconnectElectrode(uint32 pinNumber, uint32 portNumber);

void TunePrescalers_P4_v2_20(Slider_CONFIG_TYPE_P4_v2_20 *config);
void TuneSensitivity_P4_v2_20(Slider_CONFIG_TYPE_P4_v2_20 *config);
void MeasureNoiseEnvelope_P4_v2_20(Slider_CONFIG_TYPE_P4_v2_20 *config, const uint8 sensorNumber, const uint16 *rawData);
void CalculateThresholds_P4_v2_20(Slider_CONFIG_TYPE_P4_v2_20 *config, const uint8 sensorNumber, const uint16 *rawData, const uint8 widget, uint8 *fingerThres, uint8 *noiseThres);
void CalibrateSensors_P4_v2_20(Slider_CONFIG_TYPE_P4_v2_20 *config, uint16 rawLevel);
void CalibrateOneDAC_P4_v2_20(Slider_CONFIG_TYPE_P4_v2_20 *config, uint16 rawLevel, uint32 curSensor, uint8 *idacLevelsTbl);


#endif /* CY_CAPSENSE_CSD_PVT_Slider_H */
