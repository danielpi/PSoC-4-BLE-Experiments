/*******************************************************************************
* File Name: Slider_INT.c
* Version 2.20
*
* Description:
*  This file provides the source code of the  Interrupt Service Routine (ISR)
*  for the CapSense CSD component.
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

/*******************************************************************************
*  Place your includes, defines and code here 
********************************************************************************/
/* `#START Slider_ISR_INTC` */

/* `#END` */


/*******************************************************************************
* Function Name: Slider_ISR
********************************************************************************
*
* Summary:
*  This ISR is executed when the measure window is closed. The measure window 
*  depends on the sensor scan resolution parameter. 
*  The ISR has two modes: 
*   1. Scans a single sensor and stores the measure result.
*   2. Stores the result of the current measure and starts the next scan till all 
*      the enabled sensors are scanned.
*  This interrupt handler is only used in one channel designs.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  Slider_csv - used to provide the status and mode of the scanning process.
*   Mode - a single scan or scan of all the enabled sensors.
*   Status - scan is in progress or ready for new scan.
*  Slider_sensorIndex - used to store a sensor scanning sensor number.
*
* Reentrant:
*  No
*
*******************************************************************************/
CY_ISR(Slider_ISR)
{
    static uint8 Slider_snsIndexTmp;
	/*  Place your Interrupt code here. */
    /* `#START Slider_ISR_ENTER` */

    /* `#END` */

	CyIntDisable(Slider_ISR_NUMBER);
    
    Slider_CSD_INTR_REG = 1u;

    Slider_PostScan((uint32)Slider_sensorIndex);
            
    if ((Slider_csdStatusVar & Slider_SW_CTRL_SINGLE_SCAN) != 0u)
    {
        Slider_csdStatusVar &= (uint8)~Slider_SW_STS_BUSY;
    }
    else
    {
        /* Proceed scanning */
        Slider_sensorIndex = Slider_FindNextSensor(Slider_sensorIndex);
        
        /* Check end of scan */
        if(Slider_sensorIndex < Slider_TOTAL_SENSOR_COUNT)
        {
            Slider_PreScan((uint32)Slider_sensorIndex);
        }
        else
        {
            Slider_csdStatusVar &= (uint8)~Slider_SW_STS_BUSY;

			 /* Check if widget has been scanned */
			if((Slider_csdStatusVar & Slider_SW_CTRL_WIDGET_SCAN) != 0u)
			{
			    /* Restore sensorEnableMask array */
				for(Slider_snsIndexTmp = 0u; 
				    Slider_snsIndexTmp < Slider_TOTAL_SENSOR_MASK; 
					Slider_snsIndexTmp++)
				{
					/* Backup sensorEnableMask array */
					Slider_sensorEnableMask[Slider_snsIndexTmp] = Slider_sensorEnableMaskBackup[Slider_snsIndexTmp];
				}
			}
        } 
    }
    CyIntEnable(Slider_ISR_NUMBER);
	
	/*  Place your Interrupt code here. */
    /* `#START Slider_ISR_EXIT` */

    /* `#END` */
}

/* [] END OF FILE */
