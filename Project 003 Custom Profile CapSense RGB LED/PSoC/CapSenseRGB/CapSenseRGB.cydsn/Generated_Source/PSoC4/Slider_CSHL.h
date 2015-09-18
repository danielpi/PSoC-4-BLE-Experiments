/*******************************************************************************
* File Name: Slider_CSHL.h
* Version 2.20
*
* Description:
*  This file provides constants and parameter values for the High Level APIs
*  for CapSense CSD component.
*
* Note:
*
********************************************************************************
* Copyright 2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CAPSENSE_CSD_CSHL_Slider_H)
#define CY_CAPSENSE_CSD_CSHL_Slider_H

#include "Slider.h"


/***************************************
*   Condition compilation parameters
***************************************/

#define Slider_SIGNAL_SIZE                    (8u)
#define Slider_AUTO_RESET                     (0u)
#define Slider_RAW_FILTER_MASK                (8u)

/* Signal size definition */
#define Slider_SIGNAL_SIZE_UINT8              (8u)
#define Slider_SIGNAL_SIZE_UINT16             (16u)

/* Auto reset definition */
#define Slider_AUTO_RESET_DISABLE             (0u)
#define Slider_AUTO_RESET_ENABLE              (1u)

/* Mask for RAW and POS filters */
#define Slider_MEDIAN_FILTER                  (0x01u)
#define Slider_AVERAGING_FILTER               (0x02u)
#define Slider_IIR2_FILTER                    (0x04u)
#define Slider_IIR4_FILTER                    (0x08u)
#define Slider_JITTER_FILTER                  (0x10u)
#define Slider_IIR8_FILTER                    (0x20u)
#define Slider_IIR16_FILTER                   (0x40u)
#define Slider_RAW_FILTERS_ENABLED            (0x01u)                                                   
#define Slider_RAW_FILTERS_DISABLED           (0x00u) 

/***************************************
*           API Constants
***************************************/

/* Widgets constants definition */
#define Slider_LINEARSLIDER0__LS        (0u)

#define Slider_TOTAL_DIPLEXED_SLIDERS_COUNT        (0u)
#define Slider_TOTAL_LINEAR_SLIDERS_COUNT          (1u)
#define Slider_TOTAL_RADIAL_SLIDERS_COUNT          (0u)
#define Slider_TOTAL_TOUCH_PADS_COUNT              (0u)
#define Slider_TOTAL_TOUCH_PADS_BASIC_COUNT        (0u)
#define Slider_TOTAL_TRACKPAD_GESTURES_COUNT       (0u)
#define Slider_TOTAL_BUTTONS_COUNT                 (0u)
#define Slider_TOTAL_MATRIX_BUTTONS_COUNT          (0u)
#define Slider_TOTAL_GENERICS_COUNT                (0u)

#define Slider_POS_FILTERS_MASK                    (0x8u)
#define Slider_LINEAR_SLIDERS_POS_FILTERS_MASK     (0x8u)
#define Slider_RADIAL_SLIDERS_POS_FILTERS_MASK     (0x0u)
#define Slider_TOUCH_PADS_POS_FILTERS_MASK         (0x0u)
#define Slider_TRACKPAD_GEST_POS_FILTERS_MASK      (0x0u)

#define Slider_UNUSED_DEBOUNCE_COUNTER_INDEX       (0u)

#define Slider_TOTAL_PROX_SENSORS_COUNT            (0u)

#define Slider_END_OF_SLIDERS_INDEX                (0u)
#define Slider_END_OF_TOUCH_PAD_INDEX              (0u)
#define Slider_END_OF_BUTTONS_INDEX                (0u)
#define Slider_END_OF_MATRIX_BUTTONS_INDEX         (0u)
#define Slider_END_OF_WIDGETS_INDEX                (1u)



#define Slider_TOTAL_SLIDERS_COUNT            ( Slider_TOTAL_LINEAR_SLIDERS_COUNT + \
                                                          Slider_TOTAL_RADIAL_SLIDERS_COUNT )
                                                          
#define Slider_TOTAL_CENTROIDS_COUNT          ( Slider_TOTAL_SLIDERS_COUNT + \
                                                         (Slider_TOTAL_TOUCH_PADS_BASIC_COUNT * 2u) +\
														 (Slider_TOTAL_TRACKPAD_GESTURES_COUNT * 4u))

#define Slider_TOTAL_CENTROIDS_BASIC_COUNT    ( Slider_TOTAL_SLIDERS_COUNT + \
                                                         (Slider_TOTAL_TOUCH_PADS_BASIC_COUNT * 2u))

#define Slider_TOTAL_CENTROID_AXES_COUNT      ( Slider_TOTAL_SLIDERS_COUNT + \
                                                         (Slider_TOTAL_TOUCH_PADS_BASIC_COUNT * 2u) +\
														 (Slider_TOTAL_TRACKPAD_GESTURES_COUNT * 2u))

#define Slider_TOTAL_WIDGET_COUNT             ( Slider_TOTAL_LINEAR_SLIDERS_COUNT + \
                                                          Slider_TOTAL_RADIAL_SLIDERS_COUNT + \
                                                          Slider_TOTAL_TOUCH_PADS_COUNT + \
                                                          Slider_TOTAL_BUTTONS_COUNT + \
                                                          Slider_TOTAL_MATRIX_BUTTONS_COUNT )
                                                           
#define Slider_ANY_POS_FILTER                 ( Slider_MEDIAN_FILTER | \
                                                          Slider_AVERAGING_FILTER | \
                                                          Slider_IIR2_FILTER | \
                                                          Slider_IIR4_FILTER | \
                                                          Slider_JITTER_FILTER )
                                                         
#define Slider_IS_DIPLEX_SLIDER               ( Slider_TOTAL_DIPLEXED_SLIDERS_COUNT > 0u)

#define Slider_IS_NON_DIPLEX_SLIDER           ( (Slider_TOTAL_LINEAR_SLIDERS_COUNT - \
                                                           Slider_TOTAL_DIPLEXED_SLIDERS_COUNT) > 0u)
#define Slider_ADD_SLIDER_TYPE                ((Slider_TOTAL_RADIAL_SLIDERS_COUNT > 0u) ? \
                                                        ((Slider_TOTAL_TOUCH_PADS_COUNT > 0u) || \
                                                         (Slider_TOTAL_LINEAR_SLIDERS_COUNT > 0u)) : 0u)

#define Slider_TOTAL_PROX_SENSOR_COUNT		(Slider_TOTAL_PROX_SENSORS_COUNT)

#define Slider_WIDGETS_TBL_SIZE               ( Slider_TOTAL_WIDGET_COUNT + \
                                                          Slider_TOTAL_GENERICS_COUNT)
                                                          
#define Slider_WIDGET_PARAM_TBL_SIZE		    (Slider_TOTAL_BUTTONS_COUNT + \
														 Slider_TOTAL_SLIDERS_COUNT +\
                                                         Slider_TOTAL_TOUCH_PADS_BASIC_COUNT * 2u + \
														 Slider_TOTAL_TRACKPAD_GESTURES_COUNT * 2u +\
                                                         Slider_TOTAL_MATRIX_BUTTONS_COUNT * 2u)

#define Slider_THRESHOLD_TBL_SIZE			(Slider_WIDGET_PARAM_TBL_SIZE)
#define Slider_DEBOUNCE_CNT_TBL_SIZE		(Slider_WIDGET_PARAM_TBL_SIZE)
#define Slider_RAW_DATA_INDEX_TBL_SIZE	(Slider_WIDGET_PARAM_TBL_SIZE +\
													 Slider_TOTAL_GENERICS_COUNT)

#define Slider_RES_MULT               	(256u)


#define Slider_NOT_WIDGET		                (0xFFFFFFFFu)
														  
/*Types of centroids */
#define Slider_TYPE_RADIAL_SLIDER             (0x01u)
#define Slider_TYPE_LINEAR_SLIDER             (0x02u)
#define Slider_TYPE_GENERIC                   (0xFFu)

/* Defines if sensors or widgets are active */
#define Slider_SENSOR_IS_ACTIVE               (0x01u)
#define Slider_SENSOR_1_IS_ACTIVE     		(0x01u)
#define Slider_SENSOR_2_IS_ACTIVE     		(0x02u)
#define Slider_WIDGET_IS_ACTIVE               (0x01u)

/* Defines diplex type of Slider */
#define Slider_IS_DIPLEX                      (0x80u)

/* Defines fingers positions on Slider  */
#define Slider_POS_PREV                       (0u)
#define Slider_POS                            (1u)
#define Slider_POS_NEXT                       (2u)
#define Slider_CENTROID_ROUND_VALUE           (0x7F00u)
#define Slider_MAXIMUM_CENTROID               (0xFFu)

#define Slider_NEGATIVE_NOISE_THRESHOLD       (20u)
#define Slider_LOW_BASELINE_RESET             (5u)


/***************************************
*        Function Prototypes
***************************************/

void Slider_InitializeSensorBaseline(uint32 sensor);
void Slider_InitializeAllBaselines(void);
void Slider_InitializeEnabledBaselines(void);
void Slider_UpdateSensorBaseline(uint32 sensor);
void Slider_UpdateBaselineNoThreshold(uint32 sensor);
void Slider_UpdateEnabledBaselines(void);
void Slider_UpdateWidgetBaseline(uint32 widget);
uint16 Slider_GetBaselineData(uint32 sensor);
void Slider_SetBaselineData(uint32 sensor, uint16 data);
void Slider_BaseInit(uint32 sensor);

#if (Slider_IS_DIPLEX_SLIDER)
	uint8 Slider_FindMaximum(uint8 offset, uint8 count, uint8 fingerThreshold, const uint8 *diplex);
#else 
	uint8 Slider_FindMaximum(uint8 offset, uint8 count, uint8 fingerThreshold);
#endif /* (Slider_IS_DIPLEX_SLIDER) */

#if (Slider_TOTAL_TRACKPAD_GESTURES_COUNT > 0u)
	uint8 Slider_CalcCentroid(uint8 maximum, uint8 offset, \
                                    uint8 count, uint32 resolution, uint8 noiseThreshold);
#else								
	uint8 Slider_CalcCentroid(uint8 maximum, uint8 offset, \
                                    uint8 count, uint16 resolution, uint8 noiseThreshold);										
#endif /* (Slider_TOTAL_TRACKPAD_GESTURES_COUNT > 0u) */


uint8 Slider_GetFingerThreshold(uint32 widget);
uint8 Slider_GetNoiseThreshold(uint32 widget);
uint8 Slider_GetFingerHysteresis(uint32 widget);
uint8 Slider_GetNegativeNoiseThreshold(uint32 widget);

#if(Slider_TUNING_METHOD != Slider__TUNING_NONE)
	void Slider_SetFingerThreshold(uint32 widget, uint8 value);
	void Slider_SetNoiseThreshold(uint32 widget, uint8 value);
	void Slider_SetFingerHysteresis(uint32 widget, uint8 value);
	void Slider_SetNegativeNoiseThreshold(uint32 widget, uint8 value);
	void Slider_SetDebounce(uint32 widget, uint8 value);
	void Slider_SetLowBaselineReset(uint32 sensor, uint8 value);
#endif /* (Slider_TUNING_METHOD != Slider__TUNING_NONE) */

uint8 Slider_GetDiffCountData(uint32 sensor);
void Slider_SetDiffCountData(uint32 sensor, uint8 value);								
							
uint32 Slider_CheckIsSensorActive(uint32 sensor);
uint32 Slider_CheckIsWidgetActive(uint32 widget);
uint32 Slider_CheckIsAnyWidgetActive(void);
void Slider_EnableWidget(uint32 widget);
void Slider_DisableWidget(uint32 widget);
void Slider_EnableRawDataFilters(void);
void Slider_DisableRawDataFilters(void);

#if (Slider_TOTAL_MATRIX_BUTTONS_COUNT)
    uint32 Slider_GetMatrixButtonPos(uint32 widget, uint8* pos);
#endif /* (Slider_TOTAL_MATRIX_BUTTONS_COUNT) */

#if((Slider_TOTAL_RADIAL_SLIDERS_COUNT > 0u) || (Slider_TOTAL_LINEAR_SLIDERS_COUNT > 0u))
    uint16 Slider_GetCentroidPos(uint32 widget);
#endif /* ((Slider_TOTAL_RADIAL_SLIDERS_COUNT > 0u) || (Slider_TOTAL_LINEAR_SLIDERS_COUNT > 0u)) */
#if((Slider_TOTAL_RADIAL_SLIDERS_COUNT > 0u) || (Slider_TOTAL_LINEAR_SLIDERS_COUNT > 0u))
    uint16 Slider_GetRadialCentroidPos(uint32 widget);
#endif /* #if((Slider_TOTAL_RADIAL_SLIDERS_COUNT > 0u) || (Slider_TOTAL_LINEAR_SLIDERS_COUNT > 0u)) */
#if (Slider_TOTAL_TOUCH_PADS_COUNT)
    uint32 Slider_GetTouchCentroidPos(uint32 widget, uint16* pos);
#endif /* (Slider_TOTAL_TOUCH_PADS_COUNT) */

uint32 Slider_GetWidgetNumber(uint32 sensor);
uint8 Slider_GetLowBaselineReset(uint32 sensor);
uint8 Slider_GetDebounce(uint32 widget);

/* Filter function prototypes for High level APIs */

/* Median filter function prototype */
#if ( (0u != (Slider_RAW_FILTER_MASK & Slider_MEDIAN_FILTER)) || \
      (0u != (Slider_POS_FILTERS_MASK & Slider_MEDIAN_FILTER)) || \
      ((Slider_TUNING_METHOD == Slider__TUNING_AUTO)) )
    uint16 Slider_MedianFilter(uint16 x1, uint16 x2, uint16 x3);
#endif /* ( (0u != (Slider_RAW_FILTER_MASK & Slider_MEDIAN_FILTER)) || \
      (0u != (Slider_POS_FILTERS_MASK & Slider_MEDIAN_FILTER)) || \
      ((Slider_TUNING_METHOD == Slider__TUNING_AUTO)) ) */

/* Averaging filter function prototype */
#if ( (0u != (Slider_RAW_FILTER_MASK & Slider_AVERAGING_FILTER)) || \
      (0u != (Slider_POS_FILTERS_MASK & Slider_AVERAGING_FILTER)) )
    uint16 Slider_AveragingFilter(uint16 x1, uint16 x2, uint16 x3);
#endif /* Slider_RAW_FILTER_MASK && Slider_POS_FILTERS_MASK */

/* IIR2Filter(1/2prev + 1/2cur) filter function prototype */
#if ( (0u != (Slider_RAW_FILTER_MASK & Slider_IIR2_FILTER)) || \
      (0u != (Slider_POS_FILTERS_MASK & Slider_IIR2_FILTER)) )
    uint16 Slider_IIR2Filter(uint16 x1, uint16 x2);
#endif /* Slider_RAW_FILTER_MASK && Slider_POS_FILTERS_MASK */

/* IIR4Filter(3/4prev + 1/4cur) filter function prototype */
#if ( (0u != (Slider_RAW_FILTER_MASK & Slider_IIR4_FILTER)) || \
      (0u != (Slider_POS_FILTERS_MASK & Slider_IIR4_FILTER)) )
    uint16 Slider_IIR4Filter(uint16 x1, uint16 x2);
#endif /* Slider_RAW_FILTER_MASK && Slider_POS_FILTERS_MASK */

/* IIR8Filter(7/8prev + 1/8cur) filter function prototype - RawCounts only */
#if (0u != (Slider_RAW_FILTER_MASK & Slider_IIR8_FILTER))
    uint16 Slider_IIR8Filter(uint16 x1, uint16 x2);
#endif /* Slider_RAW_FILTER_MASK */

/* IIR16Filter(15/16prev + 1/16cur) filter function prototype - RawCounts only */
#if (0u != (Slider_RAW_FILTER_MASK & Slider_IIR16_FILTER))
    uint16 Slider_IIR16Filter(uint16 x1, uint16 x2);
#endif /* Slider_RAW_FILTER_MASK */

/* JitterFilter filter function prototype */
#if ( (0u != (Slider_RAW_FILTER_MASK & Slider_JITTER_FILTER)) || \
      (0u != (Slider_POS_FILTERS_MASK & Slider_JITTER_FILTER)) || \
      (0u != (Slider_TRACKPAD_GEST_POS_FILTERS_MASK & Slider_JITTER_FILTER))) 
    uint16 Slider_JitterFilter(uint16 x1, uint16 x2);
#endif /* ( (0u != (Slider_RAW_FILTER_MASK & Slider_JITTER_FILTER)) || \
       *    (0u != (Slider_POS_FILTERS_MASK & Slider_JITTER_FILTER)) )
	   *    (0u != (Slider_TRACKPAD_GEST_POS_FILTERS_MASK & Slider_JITTER_FILTER)) )
       */


/***************************************
*     Vars with External Linkage
***************************************/
extern uint16 Slider_sensorBaseline[Slider_TOTAL_SENSOR_COUNT];
extern uint8  Slider_sensorBaselineLow[Slider_TOTAL_SENSOR_COUNT];
extern uint8 Slider_sensorSignal[Slider_TOTAL_SENSOR_COUNT];
extern uint8  Slider_sensorOnMask[Slider_TOTAL_SENSOR_MASK];

extern uint8 Slider_lowBaselineResetCnt[Slider_TOTAL_SENSOR_COUNT];
extern uint8 Slider_lowBaselineReset[Slider_TOTAL_SENSOR_COUNT];

/* Generated by Customizer */
#if (Slider_TUNING_METHOD != Slider__TUNING_NONE)
	extern uint8 Slider_fingerThreshold[Slider_WIDGET_PARAM_TBL_SIZE];
	extern uint8 Slider_noiseThreshold[Slider_WIDGET_PARAM_TBL_SIZE];
	extern uint8 Slider_negativeNoiseThreshold[Slider_WIDGET_PARAM_TBL_SIZE];
	extern uint8 Slider_hysteresis[Slider_WIDGET_PARAM_TBL_SIZE];
	extern uint8  Slider_debounce[Slider_WIDGET_PARAM_TBL_SIZE];
#else
	extern const uint8 Slider_fingerThreshold[Slider_WIDGET_PARAM_TBL_SIZE];
	extern const uint8 Slider_noiseThreshold[Slider_WIDGET_PARAM_TBL_SIZE];
	extern const uint8 Slider_hysteresis[Slider_WIDGET_PARAM_TBL_SIZE];
	extern const uint8  Slider_debounce[Slider_WIDGET_PARAM_TBL_SIZE];
#endif /* (Slider_TUNING_METHOD != Slider__TUNING_NONE) */

extern const uint8 Slider_rawDataIndex[Slider_RAW_DATA_INDEX_TBL_SIZE];
extern const uint8 Slider_numberOfSensors[Slider_RAW_DATA_INDEX_TBL_SIZE];

#if (0u != Slider_TOTAL_TRACKPAD_GESTURES_COUNT)
    #if (Slider_TUNING_METHOD != Slider__TUNING_NONE)
		extern uint8 Slider_posFiltersMask[Slider_TOTAL_CENTROID_AXES_COUNT];
	#else
		extern const uint8 Slider_posFiltersMask[Slider_TOTAL_CENTROID_AXES_COUNT];
	#endif /* (Slider_TUNING_METHOD != Slider__TUNING_NONE) */

	extern uint32 Slider_centroidMult[Slider_TOTAL_CENTROID_AXES_COUNT];
#endif /* (0u != Slider_TOTAL_TRACKPAD_GESTURES_COUNT) */	

/***************************************
*        Obsolete Names
***************************************/
#define Slider_SensorRaw				Slider_sensorRaw
#define Slider_SensorEnableMask		Slider_sensorEnableMask
#define Slider_SensorBaseline			Slider_sensorBaseline
#define Slider_SensorBaselineLow		Slider_sensorBaselineLow
#define Slider_SensorSignal			Slider_sensorSignal
#define Slider_SensorOnMask			Slider_sensorOnMask
#define Slider_LowBaselineResetCnt	Slider_lowBaselineResetCnt

#endif /* CY_CAPSENSE_CSD_CSHL_Slider_H */

/* [] END OF FILE */
