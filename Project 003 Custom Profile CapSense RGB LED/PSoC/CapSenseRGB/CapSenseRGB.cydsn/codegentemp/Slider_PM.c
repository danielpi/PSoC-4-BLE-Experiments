/*******************************************************************************
* File Name: Slider_PM.c
* Version 2.20
*
* Description:
*  This file provides Sleep APIs for CapSense CSD Component.
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

static Slider_BACKUP_STRUCT Slider_backup =
{   
    0x00u, /* enableState; */
};


/*******************************************************************************
* Function Name: Slider_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the customer configuration of CapSense.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  Slider_backup - used to save the component state before entering the  sleep 
*  mode and none-retention registers.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Slider_SaveConfig(void)
{
	if ((Slider_CSD_CFG_REG & Slider_CSD_CFG_ENABLE) != 0u)
	{
		Slider_backup.enableState = 1u;
	}
}


/*******************************************************************************
* Function Name: Slider_Sleep
********************************************************************************
*
* Summary:
*  Disables the Active mode power.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  Slider_backup - used to save the component state before entering the sleep 
*  mode.
*
*******************************************************************************/
void Slider_Sleep(void)
{
	Slider_SaveConfig();
		
	/* Disable interrupt */
	CyIntDisable(Slider_ISR_NUMBER);
	
	Slider_CSD_CFG_REG &= ~(Slider_CSD_CFG_SENSE_COMP_EN | Slider_CSD_CFG_SENSE_EN);
	
	#if(Slider_IDAC_CNT == 2u)
		Slider_CSD_CFG_REG &= ~(Slider_CSD_CFG_ENABLE);
	#endif /* (Slider_IDAC_CNT == 2u) */
	
	/* Disable Clocks */
    Slider_SenseClk_Stop();
    Slider_SampleClk_Stop();
}


/*******************************************************************************
* Function Name: Slider_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the CapSense configuration.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  Must be called only after Slider_SaveConfig() routine. Otherwise 
*  the component configuration will be overwritten with its initial setting.  
*
* Global Variables:
*  Slider_backup - used to save the component state before entering the sleep 
*  mode and none-retention registers.
*
*******************************************************************************/
void Slider_RestoreConfig(void)
{

}
 
 
/*******************************************************************************
* Function Name: Slider_Wakeup
********************************************************************************
*
* Summary:
*  Restores the CapSense configuration and non-retention register values. 
*  Restores the enabled state of the component by setting the Active mode power template 
*  bits for a number of components used within CapSense.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  Slider_backup - used to save the component state before entering the sleep 
*  mode and none-retention registers.
*
*******************************************************************************/
void Slider_Wakeup(void)
{
	/* Enable the Clocks */
    Slider_SenseClk_Start();
    Slider_SampleClk_Start();
    
	/* Restore CapSense Enable state */
    if (Slider_backup.enableState != 0u)
    {
        Slider_Enable();
    }
}


/* [] END OF FILE */
