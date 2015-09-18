/*******************************************************************************
* File Name: Slider_SMS.h
* Version 2.20
*
* Description:
*  This file provides the declarations of the wrapper between the CapSense CSD component 
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
#if !defined(CY_CAPSENSE_CSD_SMS_Slider_H)
#define CY_CAPSENSE_CSD_SMS_Slider_H

#include "Slider.h"
#include "Slider_PVT.h"

#if(0u != Slider_CSHL_API_GENERATE)
	#include "Slider_CSHL.h"
#endif /* (0u != Slider_CSHL_API_GENERATE) */

#define Slider_PRESCALERS_TBL_SIZE		(16u)

#define Slider_CALIBRATION_FREQ_KHZ		(3000u)
#define Slider_CALIBRATION_MD  		    (4u)

#define Slider_MAX_RESOLUTION         (65535u)
#define Slider_CALIBRATION_RESOLUTION	(0x0FFF0000u)

#define Slider_PRS_CORRECTION_DIVIDER (4u)
#define Slider_SENSITIVITY_DIVIDER    (10u)

#define Slider_SENSITIVITY_MASK        	(0x0Fu)
#define Slider_FLEXIBLE_THRESHOLDS_EN  	(0u)
#define Slider_CAL_RAW_COUNT           	(3482u)

#define Slider_THRESHOLD_1                (220u)
#define Slider_THRESHOLD_2                (237u)
#define Slider_THRESHOLD_3                (245u)
#define Slider_THRESHOLD_4                (250u)


#if (0u != Slider_IS_M0S8PERI_BLOCK)
	#define Slider_CALIBRATION_ASD		(CYDEV_BCLK__HFCLK__KHZ /\
													 Slider_CALIBRATION_FREQ_KHZ)
#else
	#define Slider_MIN_IMO_FREQ_KHZ		(6000u)
	#if(CYDEV_BCLK__HFCLK__KHZ < Slider_MIN_IMO_FREQ_KHZ)
		#define Slider_CALIBRATION_ASD			(1u)
	#else
		#define Slider_CALIBRATION_ASD	(CYDEV_BCLK__HFCLK__KHZ / Slider_CALIBRATION_MD /\
												 	 Slider_CALIBRATION_FREQ_KHZ)
	#endif /* (CYDEV_BCLK__HFCLK__KHZ < Slider_MIN_IMO_FREQ_KHZ) */
#endif /* (0u != Slider_IS_M0S8PERI_BLOCK) */

#if (0u != Slider_IS_M0S8PERI_BLOCK)
    /*  Dividers are not chained */ 
	#if(Slider_PRS_OPTIONS != Slider__PRS_NONE)
		#define Slider_SENSETIVITY_FACTOR (1228u)
	#else
		#define Slider_SENSETIVITY_FACTOR (2456u)
	#endif /* (Slider_PRS_OPTIONS == Slider__PRS_NONE) */
#else
    /*  Dividers are chained */
	#if(Slider_PRS_OPTIONS != Slider__PRS_NONE)
		#define Slider_SENSETIVITY_FACTOR (1228u/Slider_CALIBRATION_MD)
	#else
		#define Slider_SENSETIVITY_FACTOR (2456u/Slider_CALIBRATION_MD)
	#endif /* (Slider_PRS_OPTIONS == Slider__PRS_NONE) */
#endif /* (0u != Slider_IS_M0S8PERI_BLOCK) */


/***************************************
*        Function Prototypes
***************************************/
void Slider_AutoTune(void);
void Slider_UpdateThresholds(uint32 sensor);
void Slider_SetSensitivity(uint32 sensor, uint32 value);
void Slider_SetAnalogSwitchesSrcDirect(void);
uint16 Slider_GetNoiseEnvelope(uint32 sensor);
uint8 Slider_GetSensitivityCoefficient(uint32 sensor);
uint16 Slider_GetNormalizedDiffCountData(uint32 sensor);


/***************************************
*     Vars with External Linkage
***************************************/

/*  Real Charge Divider values */
#if (0u != Slider_IS_M0S8PERI_BLOCK)
    /*  Dividers are not chained */
	#if (CYDEV_BCLK__HFCLK__MHZ >= 12u)
		extern const uint8 prescalerTable[Slider_PRESCALERS_TBL_SIZE];
	#elif (CYDEV_BCLK__HFCLK__MHZ >= 6u)
		extern const uint8 prescalerTable[Slider_PRESCALERS_TBL_SIZE];
	#else
		extern const uint8 prescalerTable[Slider_PRESCALERS_TBL_SIZE];
	#endif /* (CYDEV_BCLK__HFCLK__MHZ > 12u) */
#else
    /*  Dividers are chained */ 
	#if (CYDEV_BCLK__HFCLK__MHZ > 24u)
		extern const uint8 prescalerTable[Slider_PRESCALERS_TBL_SIZE];
	#elif (CYDEV_BCLK__HFCLK__MHZ >12u)
		extern const uint8 prescalerTable[Slider_PRESCALERS_TBL_SIZE];
	#else   /* (CYDEV_BCLK__HFCLK__MHZ > 12u) */
		extern const uint8 prescalerTable[Slider_PRESCALERS_TBL_SIZE];
	#endif /* (CYDEV_BCLK__HFCLK__MHZ > 24u) */
#endif /* (0u != Slider_IS_M0S8PERI_BLOCK) */


extern uint8 noiseEnvelopeTbl[Slider_TOTAL_SENSOR_COUNT];
extern uint8 runningDifferenceTbl[Slider_TOTAL_SENSOR_COUNT];
extern uint8 signRegisterTbl[Slider_TOTAL_SENSOR_COUNT];
extern uint16 sampleMinTbl[Slider_TOTAL_SENSOR_COUNT];
extern uint16 sampleMaxTbl[Slider_TOTAL_SENSOR_COUNT];
extern uint16 previousSampleTbl[Slider_TOTAL_SENSOR_COUNT];
extern uint8 kValueTbl[Slider_TOTAL_SENSOR_COUNT];


#endif  /* (CY_CAPSENSE_CSD_SMS_Slider_H) */

/* [] END OF FILE */
