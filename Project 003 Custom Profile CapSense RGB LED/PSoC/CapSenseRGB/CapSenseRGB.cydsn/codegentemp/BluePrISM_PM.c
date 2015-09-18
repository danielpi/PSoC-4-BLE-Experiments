/*******************************************************************************
* File Name: BluePrISM_PM.c
* Version 2.20
*
* Description:
*  This file provides Sleep/WakeUp APIs functionality of the PrISM component
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "BluePrISM.h"


/***************************************
* Local data allocation
***************************************/
static BluePrISM_BACKUP_STRUCT  BluePrISM_backup = 
{
    /* enableState */
    0u,
    /* cr */
    #if(!BluePrISM_PULSE_TYPE_HARDCODED)
       (((BluePrISM_GREATERTHAN_OR_EQUAL == BluePrISM_COMPARE0) ? \
                                                BluePrISM_CTRL_COMPARE_TYPE0_GREATER_THAN_OR_EQUAL : 0u) |
        ((BluePrISM_GREATERTHAN_OR_EQUAL == BluePrISM_COMPARE1) ? \
                                                BluePrISM_CTRL_COMPARE_TYPE1_GREATER_THAN_OR_EQUAL : 0u)),
    #endif /* End BluePrISM_PULSE_TYPE_HARDCODED */
    /* seed */    
    BluePrISM_SEED,
    /* seed_copy */    
    BluePrISM_SEED,
    /* polynom */
    BluePrISM_POLYNOM,
    #if(CY_UDB_V0)
        /* density0 */
        BluePrISM_DENSITY0,
        /* density1 */
        BluePrISM_DENSITY1,
    #endif /*End CY_UDB_V0*/
};


/*******************************************************************************
* Function Name: BluePrISM_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration.
*  
* Parameters:  
*  None.
*
* Return: 
*  None.
*
* Global Variables:
*  BluePrISM_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void BluePrISM_SaveConfig(void) 
{
    #if (CY_UDB_V0)
        #if(!BluePrISM_PULSE_TYPE_HARDCODED)
            BluePrISM_backup.cr = BluePrISM_CONTROL_REG;
        #endif /* End BluePrISM_PULSE_TYPE_HARDCODED */
        BluePrISM_backup.seed = BluePrISM_ReadSeed();
        BluePrISM_backup.seed_copy = CY_GET_REG8(BluePrISM_SEED_COPY_PTR);
        BluePrISM_backup.polynom = BluePrISM_ReadPolynomial();
        BluePrISM_backup.density0 = BluePrISM_ReadPulse0();
        BluePrISM_backup.density1 = BluePrISM_ReadPulse1();
    #else /* CY_UDB_V1 */
        #if(!BluePrISM_PULSE_TYPE_HARDCODED)
            BluePrISM_backup.cr = BluePrISM_CONTROL_REG;
        #endif /* End BluePrISM_PULSE_TYPE_HARDCODED */
        BluePrISM_backup.seed = BluePrISM_ReadSeed();
        BluePrISM_backup.seed_copy = CY_GET_REG8(BluePrISM_SEED_COPY_PTR);
        BluePrISM_backup.polynom = BluePrISM_ReadPolynomial();
    #endif  /* CY_UDB_V0 */
}


/*******************************************************************************
* Function Name: BluePrISM_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:  
*  None.
*
* Return: 
*  None.
*
* Global Variables:
*  BluePrISM_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void BluePrISM_RestoreConfig(void) 
{
    #if (CY_UDB_V0)
    
        uint8 enableInterrupts;
        
        #if(!BluePrISM_PULSE_TYPE_HARDCODED)
            BluePrISM_CONTROL_REG = BluePrISM_backup.cr;
        #endif /* End BluePrISM_PULSE_TYPE_HARDCODED */

        CY_SET_REG8(BluePrISM_SEED_COPY_PTR, BluePrISM_backup.seed_copy);
        CY_SET_REG8(BluePrISM_SEED_PTR, BluePrISM_backup.seed);
        BluePrISM_WritePolynomial(BluePrISM_backup.polynom);
        BluePrISM_WritePulse0(BluePrISM_backup.density0);
        BluePrISM_WritePulse1(BluePrISM_backup.density1);
        
        enableInterrupts = CyEnterCriticalSection();
        /* Set FIFO0_CLR bit to use FIFO0 as a simple one-byte buffer*/
        CY_SET_REG8(BluePrISM_AUX_CONTROL_PTR, 
                        CY_GET_REG8(BluePrISM_AUX_CONTROL_PTR) | BluePrISM_FIFO0_CLR);
        CyExitCriticalSection(enableInterrupts);

    #else   /* CY_UDB_V1 */

        #if(!BluePrISM_PULSE_TYPE_HARDCODED)
            BluePrISM_CONTROL_REG = BluePrISM_backup.cr;
        #endif /* End BluePrISM_PULSE_TYPE_HARDCODED */

        CY_SET_REG8(BluePrISM_SEED_COPY_PTR, BluePrISM_backup.seed_copy);
        CY_SET_REG8(BluePrISM_SEED_PTR, BluePrISM_backup.seed);
        BluePrISM_WritePolynomial(BluePrISM_backup.polynom);
    
    #endif  /* End CY_UDB_V0 */
}


/*******************************************************************************
* Function Name: BluePrISM_Sleep
********************************************************************************
*
* Summary:
*  Stops and saves the user configuration
*  
* Parameters:  
*  None.
*
* Return: 
*  None.
*
* Global Variables:
*  BluePrISM_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void BluePrISM_Sleep(void) 
{
    #if(!BluePrISM_PULSE_TYPE_HARDCODED)
        if((BluePrISM_CONTROL_REG & BluePrISM_CTRL_ENABLE) != 0u) 
        {
            BluePrISM_backup.enableState = 1u;
        }
        else
        {
            BluePrISM_backup.enableState = 0u;
        }
    #endif /* End BluePrISM_PULSE_TYPE_HARDCODED */
    BluePrISM_Stop();
    BluePrISM_SaveConfig();
}


/*******************************************************************************
* Function Name: BluePrISM_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*  
* Parameters:  
*  None.
*
* Return: 
*  None.
*
* Global Variables:
*  BluePrISM_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void BluePrISM_Wakeup(void) 
{
    BluePrISM_RestoreConfig();
    if(BluePrISM_backup.enableState != 0u)
    {
        BluePrISM_Enable();
    } 
}


/* [] END OF FILE */
