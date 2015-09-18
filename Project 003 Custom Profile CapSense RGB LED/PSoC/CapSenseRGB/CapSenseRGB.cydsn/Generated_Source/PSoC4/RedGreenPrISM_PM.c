/*******************************************************************************
* File Name: RedGreenPrISM_PM.c
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

#include "RedGreenPrISM.h"


/***************************************
* Local data allocation
***************************************/
static RedGreenPrISM_BACKUP_STRUCT  RedGreenPrISM_backup = 
{
    /* enableState */
    0u,
    /* cr */
    #if(!RedGreenPrISM_PULSE_TYPE_HARDCODED)
       (((RedGreenPrISM_GREATERTHAN_OR_EQUAL == RedGreenPrISM_COMPARE0) ? \
                                                RedGreenPrISM_CTRL_COMPARE_TYPE0_GREATER_THAN_OR_EQUAL : 0u) |
        ((RedGreenPrISM_GREATERTHAN_OR_EQUAL == RedGreenPrISM_COMPARE1) ? \
                                                RedGreenPrISM_CTRL_COMPARE_TYPE1_GREATER_THAN_OR_EQUAL : 0u)),
    #endif /* End RedGreenPrISM_PULSE_TYPE_HARDCODED */
    /* seed */    
    RedGreenPrISM_SEED,
    /* seed_copy */    
    RedGreenPrISM_SEED,
    /* polynom */
    RedGreenPrISM_POLYNOM,
    #if(CY_UDB_V0)
        /* density0 */
        RedGreenPrISM_DENSITY0,
        /* density1 */
        RedGreenPrISM_DENSITY1,
    #endif /*End CY_UDB_V0*/
};


/*******************************************************************************
* Function Name: RedGreenPrISM_SaveConfig
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
*  RedGreenPrISM_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void RedGreenPrISM_SaveConfig(void) 
{
    #if (CY_UDB_V0)
        #if(!RedGreenPrISM_PULSE_TYPE_HARDCODED)
            RedGreenPrISM_backup.cr = RedGreenPrISM_CONTROL_REG;
        #endif /* End RedGreenPrISM_PULSE_TYPE_HARDCODED */
        RedGreenPrISM_backup.seed = RedGreenPrISM_ReadSeed();
        RedGreenPrISM_backup.seed_copy = CY_GET_REG8(RedGreenPrISM_SEED_COPY_PTR);
        RedGreenPrISM_backup.polynom = RedGreenPrISM_ReadPolynomial();
        RedGreenPrISM_backup.density0 = RedGreenPrISM_ReadPulse0();
        RedGreenPrISM_backup.density1 = RedGreenPrISM_ReadPulse1();
    #else /* CY_UDB_V1 */
        #if(!RedGreenPrISM_PULSE_TYPE_HARDCODED)
            RedGreenPrISM_backup.cr = RedGreenPrISM_CONTROL_REG;
        #endif /* End RedGreenPrISM_PULSE_TYPE_HARDCODED */
        RedGreenPrISM_backup.seed = RedGreenPrISM_ReadSeed();
        RedGreenPrISM_backup.seed_copy = CY_GET_REG8(RedGreenPrISM_SEED_COPY_PTR);
        RedGreenPrISM_backup.polynom = RedGreenPrISM_ReadPolynomial();
    #endif  /* CY_UDB_V0 */
}


/*******************************************************************************
* Function Name: RedGreenPrISM_RestoreConfig
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
*  RedGreenPrISM_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void RedGreenPrISM_RestoreConfig(void) 
{
    #if (CY_UDB_V0)
    
        uint8 enableInterrupts;
        
        #if(!RedGreenPrISM_PULSE_TYPE_HARDCODED)
            RedGreenPrISM_CONTROL_REG = RedGreenPrISM_backup.cr;
        #endif /* End RedGreenPrISM_PULSE_TYPE_HARDCODED */

        CY_SET_REG8(RedGreenPrISM_SEED_COPY_PTR, RedGreenPrISM_backup.seed_copy);
        CY_SET_REG8(RedGreenPrISM_SEED_PTR, RedGreenPrISM_backup.seed);
        RedGreenPrISM_WritePolynomial(RedGreenPrISM_backup.polynom);
        RedGreenPrISM_WritePulse0(RedGreenPrISM_backup.density0);
        RedGreenPrISM_WritePulse1(RedGreenPrISM_backup.density1);
        
        enableInterrupts = CyEnterCriticalSection();
        /* Set FIFO0_CLR bit to use FIFO0 as a simple one-byte buffer*/
        CY_SET_REG8(RedGreenPrISM_AUX_CONTROL_PTR, 
                        CY_GET_REG8(RedGreenPrISM_AUX_CONTROL_PTR) | RedGreenPrISM_FIFO0_CLR);
        CyExitCriticalSection(enableInterrupts);

    #else   /* CY_UDB_V1 */

        #if(!RedGreenPrISM_PULSE_TYPE_HARDCODED)
            RedGreenPrISM_CONTROL_REG = RedGreenPrISM_backup.cr;
        #endif /* End RedGreenPrISM_PULSE_TYPE_HARDCODED */

        CY_SET_REG8(RedGreenPrISM_SEED_COPY_PTR, RedGreenPrISM_backup.seed_copy);
        CY_SET_REG8(RedGreenPrISM_SEED_PTR, RedGreenPrISM_backup.seed);
        RedGreenPrISM_WritePolynomial(RedGreenPrISM_backup.polynom);
    
    #endif  /* End CY_UDB_V0 */
}


/*******************************************************************************
* Function Name: RedGreenPrISM_Sleep
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
*  RedGreenPrISM_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void RedGreenPrISM_Sleep(void) 
{
    #if(!RedGreenPrISM_PULSE_TYPE_HARDCODED)
        if((RedGreenPrISM_CONTROL_REG & RedGreenPrISM_CTRL_ENABLE) != 0u) 
        {
            RedGreenPrISM_backup.enableState = 1u;
        }
        else
        {
            RedGreenPrISM_backup.enableState = 0u;
        }
    #endif /* End RedGreenPrISM_PULSE_TYPE_HARDCODED */
    RedGreenPrISM_Stop();
    RedGreenPrISM_SaveConfig();
}


/*******************************************************************************
* Function Name: RedGreenPrISM_Wakeup
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
*  RedGreenPrISM_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void RedGreenPrISM_Wakeup(void) 
{
    RedGreenPrISM_RestoreConfig();
    if(RedGreenPrISM_backup.enableState != 0u)
    {
        RedGreenPrISM_Enable();
    } 
}


/* [] END OF FILE */
