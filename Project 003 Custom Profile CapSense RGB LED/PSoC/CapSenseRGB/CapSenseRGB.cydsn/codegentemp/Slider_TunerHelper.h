/*******************************************************************************
* File Name: Slider_TunerHelper.h
* Version 2.20
*
* Description:
*  This file provides constants and structure declarations for the tuner helper
*  APIs for the CapSense CSD component.
*
********************************************************************************
* Copyright 2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CAPSENSE_CSD_TUNERHELPER_Slider_H)
#define CY_CAPSENSE_CSD_TUNERHELPER_Slider_H

#include "Slider.h"

#if(0u != Slider_CSHL_API_GENERATE)
	#include "Slider_CSHL.h"
#endif /* (0u != Slider_CSHL_API_GENERATE) */

#if (Slider_TUNER_API_GENERATE)
    #include "Slider_MBX.h"
#endif /* (Slider_TUNER_API_GENERATE) */


/***************************************
*     Constants for mailboxes
***************************************/

#define Slider_DEFAULT_MAILBOXES_NUMBER   	(1u)

#define Slider_CENTROID_POSITION_BUFFER_SIZE	(2u)
#define Slider_MB_POSITION_BUFFER_SIZE		(2u)

#define Slider_DEFAULT_TRACKPAD_INDEX			(0u) 

#define Slider_FIRST_FINGER_INDEX   			(0u)
#define Slider_SECOND_FINGER_INDEX   			(1u)

#define Slider_FIRST_FINGER_X_INDEX			(0u)
#define Slider_FIRST_FINGER_Y_INDEX			(1u)

#define Slider_SECOND_FINGER_X_INDEX			(2u)
#define Slider_SECOND_FINGER_Y_INDEX			(3u)


/***************************************
*        Function Prototypes
***************************************/

void Slider_TunerStart(void);
void Slider_TunerComm(void);

#if (Slider_TUNER_API_GENERATE)
   Slider_NO_STRICT_VOLATILE void Slider_ProcessAllWidgets(volatile Slider_OUTBOX *outbox);
#endif /* (Slider_TUNER_API_GENERATE) */


/***************************************
*     Vars with External Linkage
***************************************/
#if (Slider_TUNER_API_GENERATE)
	extern volatile Slider_MAILBOXES Slider_mailboxesComm;
#endif /* (Slider_TUNER_API_GENERATE) */

#endif  /* (CY_CAPSENSE_CSD_TUNERHELPER_Slider_H)*/


/* [] END OF FILE */
