/*******************************************************************************
* File Name: RedGreenPrISM.c
* Version 2.20
*
* Description:
*  This file provides the source code of the API for the PrISM Component.
*
* Note:
*  The PRiSM Component consists of a 8, 16, 24, 32 - bit PrISM, it
*  depends on length Polynomial value and user selected Seed Value. 
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

uint8 RedGreenPrISM_initVar = 0u;


/*******************************************************************************
* FUNCTION NAME:   RedGreenPrISM_Start
********************************************************************************
*
* Summary:
*  The start function sets Polynomial, Seed and Pulse Density registers provided
*  by customizer. PrISM computation starts on rising edge of input clock.
*
* Parameters:  
*  None.
*
* Return:  
*  None.
*
* Global variables:
*  The RedGreenPrISM_initVar variable is used to indicate initial
*  configuration of this component.  The variable is initialized to zero and
*  set to 1 the first time RedGreenPrISM_Start() is called. This allows for
*  component initialization without re-initialization in all subsequent calls
*  to the RedGreenPrISM_Start() routine. 

* Reentrant:
*  No.
*
*******************************************************************************/
void RedGreenPrISM_Start(void) 
{
    /* If not Initialized then initialize all required hardware and software */
    if(RedGreenPrISM_initVar == 0u)
    {
        RedGreenPrISM_Init();
        RedGreenPrISM_initVar = 1u;
    }
    RedGreenPrISM_Enable();
}


/*******************************************************************************
* Function Name: RedGreenPrISM_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the 
*  customizer of the component placed onto schematic. Usually called in 
*  RedGreenPrISM_Start().
*
* Parameters:  
*  None.
*
* Return: 
*  None.
*
*******************************************************************************/
void RedGreenPrISM_Init(void) 
{
    uint8 enableInterrupts;
    
    /* Writes Seed value, polynom value and density provided by customizer */
    RedGreenPrISM_WriteSeed(RedGreenPrISM_SEED);
    RedGreenPrISM_WritePolynomial(RedGreenPrISM_POLYNOM);
    RedGreenPrISM_WritePulse0(RedGreenPrISM_DENSITY0);
    RedGreenPrISM_WritePulse1(RedGreenPrISM_DENSITY1);
    
    enableInterrupts = CyEnterCriticalSection();
    /* Set FIFO0_CLR bit to use FIFO0 as a simple one-byte buffer*/
    CY_SET_REG8(RedGreenPrISM_AUX_CONTROL_PTR, 
                        CY_GET_REG8(RedGreenPrISM_AUX_CONTROL_PTR) | RedGreenPrISM_FIFO0_CLR);
    CyExitCriticalSection(enableInterrupts);
    
    #if(!RedGreenPrISM_PULSE_TYPE_HARDCODED)
        /* Writes density type provided by customizer */
        #if(RedGreenPrISM_GREATERTHAN_OR_EQUAL == RedGreenPrISM_COMPARE0)
            RedGreenPrISM_CONTROL_REG |= RedGreenPrISM_CTRL_COMPARE_TYPE0_GREATER_THAN_OR_EQUAL;
        #else
            RedGreenPrISM_CONTROL_REG &= (uint8)~RedGreenPrISM_CTRL_COMPARE_TYPE0_GREATER_THAN_OR_EQUAL;
        #endif /* End RedGreenPrISM_COMPARE0 */    
    
        #if(RedGreenPrISM_GREATERTHAN_OR_EQUAL == RedGreenPrISM_COMPARE1)
            RedGreenPrISM_CONTROL_REG |= RedGreenPrISM_CTRL_COMPARE_TYPE1_GREATER_THAN_OR_EQUAL;
        #else
            RedGreenPrISM_CONTROL_REG &= (uint8)~RedGreenPrISM_CTRL_COMPARE_TYPE1_GREATER_THAN_OR_EQUAL;
        #endif /* End RedGreenPrISM_COMPARE1 */    
    #endif /* End RedGreenPrISM_PULSE_TYPE_HARDCODED */
}


/*******************************************************************************
* Function Name: RedGreenPrISM_Enable
********************************************************************************
*  
* Summary: 
*  Enables the PrISM block operation
*
* Parameters:  
*  None.
*
* Return: 
*  None.
*
*******************************************************************************/
void RedGreenPrISM_Enable(void) 
{
    #if(!RedGreenPrISM_PULSE_TYPE_HARDCODED)
        /* Enable the PrISM computation */
        RedGreenPrISM_CONTROL_REG |= RedGreenPrISM_CTRL_ENABLE;
    #endif /* End RedGreenPrISM_PULSE_TYPE_HARDCODED */
}


/*******************************************************************************
* FUNCTION NAME:   RedGreenPrISM_Stop
********************************************************************************
*
* Summary:
*  Stops PrISM computation. Outputs remain constant.
*
* Parameters:  
*  None.
*
* Return: 
*  None.
*
*******************************************************************************/
void RedGreenPrISM_Stop(void) 
{
    #if(!RedGreenPrISM_PULSE_TYPE_HARDCODED)
        RedGreenPrISM_CONTROL_REG &= (uint8)~RedGreenPrISM_CTRL_ENABLE;
    #else
        /* PulseTypeHardoded option enabled - to stop PrISM use enable input */
    #endif /* End $INSTANCE_NAME`_PULSE_TYPE_HARDCODED */
}

#if(!RedGreenPrISM_PULSE_TYPE_HARDCODED)


    /***************************************************************************
    * FUNCTION NAME:     RedGreenPrISM_SetPulse0Mode
    ****************************************************************************
    *
    * Summary:
    *  Sets the pulse density type for Density0. Less than or Equal(<=) or 
    *  Greater than or Equal(>=).
    *
    * Parameters:
    *  pulse0Type: Selected pulse density type.
    *
    * Return:
    *  None.
    *
    ***************************************************************************/
    void RedGreenPrISM_SetPulse0Mode(uint8 pulse0Type) 
    {
        if(pulse0Type == RedGreenPrISM_GREATERTHAN_OR_EQUAL)
        {
            RedGreenPrISM_CONTROL_REG |= RedGreenPrISM_CTRL_COMPARE_TYPE0_GREATER_THAN_OR_EQUAL;
        }
        else
        {
            RedGreenPrISM_CONTROL_REG &= (uint8)~RedGreenPrISM_CTRL_COMPARE_TYPE0_GREATER_THAN_OR_EQUAL;
        }
    }
    
    
    /***************************************************************************
    * FUNCTION NAME:   RedGreenPrISM_SetPulse1Mode
    ****************************************************************************
    *
    * Summary:
    *  Sets the pulse density type for Density1. Less than or Equal(<=) or 
    *  Greater than or Equal(>=).
    *
    * Parameters:  
    *  pulse1Type: Selected pulse density type.
    *
    * Return:
    *  None.
    *
    ***************************************************************************/
    void RedGreenPrISM_SetPulse1Mode(uint8 pulse1Type) 
    {
        if(pulse1Type == RedGreenPrISM_GREATERTHAN_OR_EQUAL)
        {
            RedGreenPrISM_CONTROL_REG |= RedGreenPrISM_CTRL_COMPARE_TYPE1_GREATER_THAN_OR_EQUAL;
        }
        else
        {
            RedGreenPrISM_CONTROL_REG &= (uint8)~RedGreenPrISM_CTRL_COMPARE_TYPE1_GREATER_THAN_OR_EQUAL;
        }
    }

#endif /* End RedGreenPrISM_PULSE_TYPE_HARDCODED == 0 */


/*******************************************************************************
* FUNCTION NAME:   RedGreenPrISM_ReadSeed
********************************************************************************
*
* Summary:
*  Reads the PrISM Seed register.
*
* Parameters:
*  None.
*
* Return:
*  Current Period register value.
*
*******************************************************************************/
uint8 RedGreenPrISM_ReadSeed(void) 
{
    return( CY_GET_REG8(RedGreenPrISM_SEED_PTR) );
}


/*******************************************************************************
* FUNCTION NAME:   RedGreenPrISM_WriteSeed
********************************************************************************
*
* Summary:
*  Writes the PrISM Seed register with the start value.
*
* Parameters:
*  seed: Seed register value.
*
* Return:
*  None.
*
*******************************************************************************/
void RedGreenPrISM_WriteSeed(uint8 seed) 
{
    if(seed != 0u)
    {
        CY_SET_REG8(RedGreenPrISM_SEED_COPY_PTR, seed);
        CY_SET_REG8(RedGreenPrISM_SEED_PTR, seed);
    }
}


/*******************************************************************************
* FUNCTION NAME:   RedGreenPrISM_ReadPolynomial
********************************************************************************
*
* Summary:
*  Reads the PrISM polynomial.
*
* Parameters:
*  None.
*
* Return:
*  PrISM polynomial.
*
*******************************************************************************/
uint8 RedGreenPrISM_ReadPolynomial(void) 
{
    return( CY_GET_REG8(RedGreenPrISM_POLYNOM_PTR) );
}


/*******************************************************************************
* FUNCTION NAME:   RedGreenPrISM_WritePolynomial
********************************************************************************
*
* Summary:
*  Writes the PrISM polynomial.
*
* Parameters:
*  polynomial: PrISM polynomial.
*
* Return:
*  None.
*
*******************************************************************************/
void RedGreenPrISM_WritePolynomial(uint8 polynomial) \
                                                                
{
    CY_SET_REG8(RedGreenPrISM_POLYNOM_PTR, polynomial);
    
}


/*******************************************************************************
* FUNCTION NAME:   RedGreenPrISM_ReadPulse0
********************************************************************************
*
* Summary:
*  Reads the PrISM Pulse Density0 register.
*
* Parameters:
*  None.
*
* Return:
*  Pulse Density0 register value.
*
*******************************************************************************/
uint8 RedGreenPrISM_ReadPulse0(void) 
{
    return( CY_GET_REG8(RedGreenPrISM_DENSITY0_PTR) );
}


/*******************************************************************************
* FUNCTION NAME:   RedGreenPrISM_WritePulse0
********************************************************************************
*
* Summary:
*  Writes the PrISM Pulse Density0 register with the Pulse Density value.
*
* Parameters:
*  pulseDensity0: Pulse Density value.
*
* Return:
*  None.
*
*******************************************************************************/
void RedGreenPrISM_WritePulse0(uint8 pulseDensity0) \
                                                                
{
    CY_SET_REG8(RedGreenPrISM_DENSITY0_PTR, pulseDensity0);
}


/*******************************************************************************
* FUNCTION NAME:   RedGreenPrISM_ReadPulse1
********************************************************************************
*
* Summary:
*  Reads the PrISM Pulse Density1 register.
*
* Parameters:
*  None.
*
* Return:
*  Pulse Density1 register value.
*
*******************************************************************************/
uint8 RedGreenPrISM_ReadPulse1(void) 
{
    return( CY_GET_REG8(RedGreenPrISM_DENSITY1_PTR) );
}


/*******************************************************************************
* FUNCTION NAME:   RedGreenPrISM_WritePulse1
********************************************************************************
*
* Summary:
*  Writes the PrISM Pulse Density1 register with the Pulse Density value.
*
* Parameters:
*  pulseDensity1: Pulse Density value.
*
* Return:
*  None.
*
*******************************************************************************/
void RedGreenPrISM_WritePulse1(uint8 pulseDensity1) \
                                                                    
{
    CY_SET_REG8(RedGreenPrISM_DENSITY1_PTR, pulseDensity1);
}


/* [] END OF FILE */
