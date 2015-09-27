/*******************************************************************************
* File Name: Slider_SenseClk.h
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

#if !defined(CY_CLOCK_Slider_SenseClk_H)
#define CY_CLOCK_Slider_SenseClk_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void Slider_SenseClk_StartEx(uint32 alignClkDiv);
#define Slider_SenseClk_Start() \
    Slider_SenseClk_StartEx(Slider_SenseClk__PA_DIV_ID)

#else

void Slider_SenseClk_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void Slider_SenseClk_Stop(void);

void Slider_SenseClk_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 Slider_SenseClk_GetDividerRegister(void);
uint8  Slider_SenseClk_GetFractionalDividerRegister(void);

#define Slider_SenseClk_Enable()                         Slider_SenseClk_Start()
#define Slider_SenseClk_Disable()                        Slider_SenseClk_Stop()
#define Slider_SenseClk_SetDividerRegister(clkDivider, reset)  \
    Slider_SenseClk_SetFractionalDividerRegister((clkDivider), 0u)
#define Slider_SenseClk_SetDivider(clkDivider)           Slider_SenseClk_SetDividerRegister((clkDivider), 1u)
#define Slider_SenseClk_SetDividerValue(clkDivider)      Slider_SenseClk_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define Slider_SenseClk_DIV_ID     Slider_SenseClk__DIV_ID

#define Slider_SenseClk_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define Slider_SenseClk_CTRL_REG   (*(reg32 *)Slider_SenseClk__CTRL_REGISTER)
#define Slider_SenseClk_DIV_REG    (*(reg32 *)Slider_SenseClk__DIV_REGISTER)

#define Slider_SenseClk_CMD_DIV_SHIFT          (0u)
#define Slider_SenseClk_CMD_PA_DIV_SHIFT       (8u)
#define Slider_SenseClk_CMD_DISABLE_SHIFT      (30u)
#define Slider_SenseClk_CMD_ENABLE_SHIFT       (31u)

#define Slider_SenseClk_CMD_DISABLE_MASK       ((uint32)((uint32)1u << Slider_SenseClk_CMD_DISABLE_SHIFT))
#define Slider_SenseClk_CMD_ENABLE_MASK        ((uint32)((uint32)1u << Slider_SenseClk_CMD_ENABLE_SHIFT))

#define Slider_SenseClk_DIV_FRAC_MASK  (0x000000F8u)
#define Slider_SenseClk_DIV_FRAC_SHIFT (3u)
#define Slider_SenseClk_DIV_INT_MASK   (0xFFFFFF00u)
#define Slider_SenseClk_DIV_INT_SHIFT  (8u)

#else 

#define Slider_SenseClk_DIV_REG        (*(reg32 *)Slider_SenseClk__REGISTER)
#define Slider_SenseClk_ENABLE_REG     Slider_SenseClk_DIV_REG
#define Slider_SenseClk_DIV_FRAC_MASK  Slider_SenseClk__FRAC_MASK
#define Slider_SenseClk_DIV_FRAC_SHIFT (16u)
#define Slider_SenseClk_DIV_INT_MASK   Slider_SenseClk__DIVIDER_MASK
#define Slider_SenseClk_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_Slider_SenseClk_H) */

/* [] END OF FILE */
