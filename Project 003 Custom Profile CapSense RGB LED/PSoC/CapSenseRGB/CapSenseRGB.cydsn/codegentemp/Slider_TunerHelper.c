/*******************************************************************************
* File Name: Slider_TunerHelper.c
* Version 2.20
*
* Description:
*  This file provides the source code of the Tuner helper APIs for the CapSense CSD 
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

#include "Slider_TunerHelper.h"

#if (Slider_TUNER_API_GENERATE)
    #include "SCB.h"
    #include "SCB_EZI2C.h"
    volatile Slider_MAILBOXES Slider_mailboxesComm;
#endif  /* (Slider_TUNER_API_GENERATE) */

/* `#START Slider_TunerHelper_HEADER` */

/* `#END` */

/*******************************************************************************
* Function Name: Slider_TunerStart
********************************************************************************
*
* Summary:
*  Initializes the CapSense CSD component and EzI2C communication component to use
*  a mailbox data structure for communication with the Tuner GUI.
*  Start the scanning, after initialization is complete.
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
*******************************************************************************/
void Slider_TunerStart(void)
{
    #if (0u != Slider_TUNER_API_GENERATE)
		
        /* Init mbx and quick check */
        Slider_InitMailbox(&Slider_mailboxesComm.csdMailbox);
        Slider_mailboxesComm.numMailBoxes = Slider_DEFAULT_MAILBOXES_NUMBER;
        
        /* Start CapSense and baselines */
        Slider_Start();
        
        /* Initialize baselines */
		#if(0u != Slider_CSHL_API_GENERATE)
	        Slider_InitializeEnabledBaselines();
		#endif /* (0u != Slider_CSHL_API_GENERATE) */
		
        /* Start EzI2C, clears buf pointers */
        SCB_Start();
		
        /* Setup EzI2C buffers */
        SCB_EzI2CSetBuffer1(sizeof(Slider_mailboxesComm), sizeof(Slider_mailboxesComm),
                                      			   (volatile uint8 *) &Slider_mailboxesComm);
        
        /* Starts scan all enabled sensors */
        Slider_ScanEnabledWidgets();
    
    #endif  /* (0u != Slider_TUNER_API_GENERATE) */
}


/*******************************************************************************
* Function Name: Slider_TunerComm
********************************************************************************
*
* Summary:
*  This function is blocking. It waits till scanning loop is completed and applies
*  the new parameters from the Tuner GUI if available (the manual tuning mode only). Updates
*  the enabled baselines and state of widgets. Waits while the Tuner GUI reports that 
*  the mailbox content  could be modified. Then loads the report data into the outbox 
*  and sets a busy flag. Starts a new scanning loop.
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
*******************************************************************************/
void Slider_TunerComm(void)
{
    #if (0u != Slider_TUNER_API_GENERATE)
        if (0u == Slider_IsBusy())
        {   
            /* Apply new settings */
            #if((Slider_TUNING_METHOD != Slider__TUNING_NONE) &&\
				(0u != Slider_TOTAL_TRACKPAD_GESTURES_COUNT) ||\
		        (Slider_TUNING_METHOD == Slider__TUNING_MANUAL)) 
     
				Slider_ReadMessage(&Slider_mailboxesComm.csdMailbox);
				
            #endif  /* ((Slider_TUNING_METHOD != Slider__TUNING_NONE) &&\
						(0u != Slider_TOTAL_TRACKPAD_GESTURES_COUNT) ||\
		                (Slider_TUNING_METHOD == Slider__TUNING_MANUAL)) */
			
			#if(0u != Slider_CSHL_API_GENERATE)
				/* Update all baselines and process all widgets */
            	Slider_UpdateEnabledBaselines();
            	Slider_ProcessAllWidgets(&Slider_mailboxesComm.csdMailbox.outbox);
			#endif /* (0u != Slider_CSHL_API_GENERATE) */
			
            Slider_PostMessage(&Slider_mailboxesComm.csdMailbox);
			
            /* Enable EZI2C interrupts, after scan complete */
            SCB_EnableInt();

            while((Slider_mailboxesComm.csdMailbox.type != Slider_TYPE_ID) ||
			      (0u != (SCB_EzI2CGetActivity() & SCB_EZI2C_STATUS_BUSY))){}
            
            /* Disable EZI2C interrupts, while scanning */
            SCB_DisableInt();
            
            /* Start scan all sensors */
            Slider_ScanEnabledWidgets();
        }
    #endif /* (0u != Slider_TUNER_API_GENERATE) */
}


#if ((0u != Slider_TUNER_API_GENERATE) && (0u != Slider_CSHL_API_GENERATE))
    /*******************************************************************************
    * Function Name: Slider_ProcessAllWidgets
    ********************************************************************************
    *
    * Summary:
    *  Calls the required functions to update all the widgets' state:
    *   - Slider_GetCentroidPos() - calls only if linear sliders are
    *     available.
    *   - Slider_GetRadialCentroidPos() - calls only if the  radial slider is
    *     available.
    *   - Slider_GetTouchCentroidPos() - calls only if the  touchpad slider 
    *     available.
    *   - Slider_CheckIsAnyWidgetActive();
    *  The results of operations are copied to OUTBOX.
    *   
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    * Global Variables:
    *  Slider_OUTBOX outbox - the structure which is used as an output 
    *  buffer to report data to the Tuner GUI.
    *  Update fields:
    *    - position[];
    *    - OnMask[];
	*
	* Global Variables:
	*  None
	*
    *******************************************************************************/
    void Slider_ProcessAllWidgets(volatile Slider_OUTBOX *outbox)
    {
        uint8  i = 0u;
		
		#if (0u != Slider_TOTAL_MATRIX_BUTTONS_COUNT)
        	uint8 mbPositionBuf[Slider_MB_POSITION_BUFFER_SIZE];
		#endif /* (0u != Slider_TOTAL_MATRIX_BUTTONS_COUNT) */

        #if (0u != Slider_TOTAL_TOUCH_PADS_BASIC_COUNT)
            uint16 centroidPosBuf[Slider_CENTROID_POSITION_BUFFER_SIZE];
        #endif  /* (0u != Slider_TOTAL_TOUCH_PADS_BASIC_COUNT) */
		
		#if(0u != Slider_TOTAL_TRACKPAD_GESTURES_COUNT)
            Slider_POSITION_STRUCT trackpadPosBuf[Slider_CENTROID_POSITION_BUFFER_SIZE];
        #endif  /* (0u != Slider_TOTAL_TRACKPAD_GESTURES_COUNT) */
        
        #if ( (0u != Slider_TOTAL_RADIAL_SLIDERS_COUNT) || (0u != Slider_TOTAL_TOUCH_PADS_COUNT) || \
              (0u != Slider_TOTAL_MATRIX_BUTTONS_COUNT))
            uint8 widgetCnt;
        #endif  /* ((0u != Slider_TOTAL_RADIAL_SLIDERS_COUNT) || (0u != Slider_TOTAL_TOUCH_PADS_COUNT)) || 
                *   (0u != Slider_TOTAL_MATRIX_BUTTONS_COUNT)
                */
        
        /* Calculate widget with centroids */
        #if (0u != Slider_TOTAL_LINEAR_SLIDERS_COUNT)
            for(; i < Slider_TOTAL_LINEAR_SLIDERS_COUNT; i++)
            {
                outbox->centroidPosition[i] = (uint16)Slider_GetCentroidPos((uint32)i);
            }
        #endif /* (0u != Slider_TOTAL_LINEAR_SLIDERS_COUNT) */
        
        #if (0u != Slider_TOTAL_RADIAL_SLIDERS_COUNT)
            widgetCnt = i;
            for(; i < (widgetCnt + Slider_TOTAL_RADIAL_SLIDERS_COUNT); i++)
            {
                outbox->centroidPosition[i] = (uint16)Slider_GetRadialCentroidPos((uint32)i);
            }
        #endif /* (0u != Slider_TOTAL_RADIAL_SLIDERS_COUNT) */
        
        #if (0u != Slider_TOTAL_TOUCH_PADS_BASIC_COUNT)
            widgetCnt = i;
            for(; i < (widgetCnt + (Slider_TOTAL_TOUCH_PADS_BASIC_COUNT * 2u)); i=i+2u)
            {
                if(Slider_GetTouchCentroidPos((uint32)i, centroidPosBuf) == 0u)
                {
                    outbox->centroidPosition[i + Slider_FIRST_FINGER_X_INDEX] = Slider_MAX_UINT_16;
                    outbox->centroidPosition[i + Slider_FIRST_FINGER_Y_INDEX] = Slider_MAX_UINT_16;
                }
                else
                {
                    outbox->centroidPosition[i + Slider_FIRST_FINGER_X_INDEX] =
					(uint16) centroidPosBuf[Slider_FIRST_FINGER_X_INDEX];
                    
					outbox->centroidPosition[i + Slider_FIRST_FINGER_Y_INDEX] =
					(uint16) centroidPosBuf[Slider_FIRST_FINGER_Y_INDEX];
                }
            }
        #endif /* (0u != Slider_TOTAL_TOUCH_PADS_BASIC_COUNT) */
		
		#if (0u != Slider_TOTAL_TRACKPAD_GESTURES_COUNT)
			widgetCnt = i;
			for(; i < (widgetCnt + (Slider_TOTAL_TRACKPAD_GESTURES_COUNT * 4u)); i=i+4u)
			{
				outbox->fingersNum[Slider_DEFAULT_TRACKPAD_INDEX] =
				(uint8)Slider_GetDoubleTouchCentroidPos(trackpadPosBuf);
				
				outbox->gestureID[Slider_DEFAULT_TRACKPAD_INDEX] =
				(uint8)Slider_DecodeAllGestures((uint32)outbox->fingersNum[Slider_DEFAULT_TRACKPAD_INDEX], trackpadPosBuf); 

				#if((0u != Slider_ST_SCROLL_GEST_ENABLED) || (0u != Slider_DT_SCROLL_GEST_ENABLED))
					/*
					Report Scroll count only for scroll and Inertial Scroll gestures.
					For all other gestures report scroll count as zero to tuner.
					 */
					if (((outbox->gestureID[Slider_DEFAULT_TRACKPAD_INDEX]&0xF0u) == 0xC0) ||
							((outbox->gestureID[Slider_DEFAULT_TRACKPAD_INDEX]&0xF0u) == 0xB0u))
					{
						outbox->scrollCnt = Slider_GetScrollCnt();
					}
					else
					{
						outbox->scrollCnt = 0u;
					}
				#endif /* ((0u != Slider_ST_SCROLL_GEST_ENABLED) || (0u != Slider_DT_SCROLL_GEST_ENABLED)) */

                outbox->centroidPosition[i + Slider_FIRST_FINGER_X_INDEX] = Slider_MAX_UINT_16;
                outbox->centroidPosition[i + Slider_FIRST_FINGER_Y_INDEX] = Slider_MAX_UINT_16;
				outbox->centroidPosition[i + Slider_SECOND_FINGER_X_INDEX] = Slider_MAX_UINT_16;
                outbox->centroidPosition[i + Slider_SECOND_FINGER_Y_INDEX] = Slider_MAX_UINT_16;

	            if(0u != outbox->fingersNum[Slider_DEFAULT_TRACKPAD_INDEX])
	            {
	                outbox->centroidPosition[i + Slider_FIRST_FINGER_X_INDEX] =
					(uint16) trackpadPosBuf[Slider_FIRST_FINGER_INDEX].x;
	                
					outbox->centroidPosition[i + Slider_FIRST_FINGER_Y_INDEX] =
					(uint16) trackpadPosBuf[Slider_FIRST_FINGER_INDEX].y;
					
					if(outbox->fingersNum[Slider_DEFAULT_TRACKPAD_INDEX] > 1u)
					{
						outbox->centroidPosition[i + Slider_SECOND_FINGER_X_INDEX] =
						(uint16) trackpadPosBuf[Slider_SECOND_FINGER_INDEX].x;
	                	
						outbox->centroidPosition[i + Slider_SECOND_FINGER_Y_INDEX] =
						(uint16) trackpadPosBuf[Slider_SECOND_FINGER_INDEX].y;
					}
	            }
			}
		#endif /* (0u != Slider_TOTAL_TRACKPAD_GESTURES_COUNT) */

        #if (0u != Slider_TOTAL_MATRIX_BUTTONS_COUNT)
            i += Slider_TOTAL_BUTTONS_COUNT;
            widgetCnt = 0u;
            for(; widgetCnt < (Slider_TOTAL_MATRIX_BUTTONS_COUNT * 2u); widgetCnt += 2u)
            {
                if(Slider_GetMatrixButtonPos((uint32)i, mbPositionBuf) == 0u)
                {
                    outbox->mbPosition[widgetCnt] = Slider_MAX_UINT_8;
                    outbox->mbPosition[widgetCnt+1u] = Slider_MAX_UINT_8;
                }
				else
				{
					outbox->mbPosition[widgetCnt] = mbPositionBuf[0u];
                    outbox->mbPosition[widgetCnt+1u] = mbPositionBuf[1u];
				}
                i += 2u;
            }
        #endif /* (0u != Slider_TOTAL_MATRIX_BUTTONS_COUNT) */

		#if(0u != Slider_CSHL_API_GENERATE)
	        /* Update On/Off State */
	        (void)Slider_CheckIsAnyWidgetActive();

	        /* Copy OnMask */
	        for(i=0u; i < Slider_TOTAL_SENSOR_MASK_COUNT; i++)
	        {
	            outbox->onMask[i]  = Slider_sensorOnMask[i];
	        }
		#endif /* (0u != Slider_CSHL_API_GENERATE) */
		
		/* `#START Slider_ProcessAllWidgets_Debug` */

		/* `#END` */
    }


#endif /* ((0u != Slider_TUNER_API_GENERATE) && (0u != Slider_CSHL_API_GENERATE)) */


/* [] END OF FILE */
