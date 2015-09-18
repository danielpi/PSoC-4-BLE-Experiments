/*******************************************************************************
* File Name: Slider_SampleClk.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_Slider_SampleClk_H)
#define CY_CLOCK_Slider_SampleClk_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void Slider_SampleClk_StartEx(uint32 alignClkDiv);
#define Slider_SampleClk_Start() \
    Slider_SampleClk_StartEx(Slider_SampleClk__PA_DIV_ID)

#else

void Slider_SampleClk_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void Slider_SampleClk_Stop(void);

void Slider_SampleClk_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 Slider_SampleClk_GetDividerRegister(void);
uint8  Slider_SampleClk_GetFractionalDividerRegister(void);

#define Slider_SampleClk_Enable()                         Slider_SampleClk_Start()
#define Slider_SampleClk_Disable()                        Slider_SampleClk_Stop()
#define Slider_SampleClk_SetDividerRegister(clkDivider, reset)  \
    Slider_SampleClk_SetFractionalDividerRegister((clkDivider), 0u)
#define Slider_SampleClk_SetDivider(clkDivider)           Slider_SampleClk_SetDividerRegister((clkDivider), 1u)
#define Slider_SampleClk_SetDividerValue(clkDivider)      Slider_SampleClk_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define Slider_SampleClk_DIV_ID     Slider_SampleClk__DIV_ID

#define Slider_SampleClk_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define Slider_SampleClk_CTRL_REG   (*(reg32 *)Slider_SampleClk__CTRL_REGISTER)
#define Slider_SampleClk_DIV_REG    (*(reg32 *)Slider_SampleClk__DIV_REGISTER)

#define Slider_SampleClk_CMD_DIV_SHIFT          (0u)
#define Slider_SampleClk_CMD_PA_DIV_SHIFT       (8u)
#define Slider_SampleClk_CMD_DISABLE_SHIFT      (30u)
#define Slider_SampleClk_CMD_ENABLE_SHIFT       (31u)

#define Slider_SampleClk_CMD_DISABLE_MASK       ((uint32)((uint32)1u << Slider_SampleClk_CMD_DISABLE_SHIFT))
#define Slider_SampleClk_CMD_ENABLE_MASK        ((uint32)((uint32)1u << Slider_SampleClk_CMD_ENABLE_SHIFT))

#define Slider_SampleClk_DIV_FRAC_MASK  (0x000000F8u)
#define Slider_SampleClk_DIV_FRAC_SHIFT (3u)
#define Slider_SampleClk_DIV_INT_MASK   (0xFFFFFF00u)
#define Slider_SampleClk_DIV_INT_SHIFT  (8u)

#else 

#define Slider_SampleClk_DIV_REG        (*(reg32 *)Slider_SampleClk__REGISTER)
#define Slider_SampleClk_ENABLE_REG     Slider_SampleClk_DIV_REG
#define Slider_SampleClk_DIV_FRAC_MASK  Slider_SampleClk__FRAC_MASK
#define Slider_SampleClk_DIV_FRAC_SHIFT (16u)
#define Slider_SampleClk_DIV_INT_MASK   Slider_SampleClk__DIVIDER_MASK
#define Slider_SampleClk_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_Slider_SampleClk_H) */

/* [] END OF FILE */
