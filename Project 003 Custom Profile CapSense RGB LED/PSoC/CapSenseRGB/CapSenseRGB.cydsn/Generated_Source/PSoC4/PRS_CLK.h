/*******************************************************************************
* File Name: PRS_CLK.h
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

#if !defined(CY_CLOCK_PRS_CLK_H)
#define CY_CLOCK_PRS_CLK_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void PRS_CLK_StartEx(uint32 alignClkDiv);
#define PRS_CLK_Start() \
    PRS_CLK_StartEx(PRS_CLK__PA_DIV_ID)

#else

void PRS_CLK_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void PRS_CLK_Stop(void);

void PRS_CLK_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 PRS_CLK_GetDividerRegister(void);
uint8  PRS_CLK_GetFractionalDividerRegister(void);

#define PRS_CLK_Enable()                         PRS_CLK_Start()
#define PRS_CLK_Disable()                        PRS_CLK_Stop()
#define PRS_CLK_SetDividerRegister(clkDivider, reset)  \
    PRS_CLK_SetFractionalDividerRegister((clkDivider), 0u)
#define PRS_CLK_SetDivider(clkDivider)           PRS_CLK_SetDividerRegister((clkDivider), 1u)
#define PRS_CLK_SetDividerValue(clkDivider)      PRS_CLK_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define PRS_CLK_DIV_ID     PRS_CLK__DIV_ID

#define PRS_CLK_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define PRS_CLK_CTRL_REG   (*(reg32 *)PRS_CLK__CTRL_REGISTER)
#define PRS_CLK_DIV_REG    (*(reg32 *)PRS_CLK__DIV_REGISTER)

#define PRS_CLK_CMD_DIV_SHIFT          (0u)
#define PRS_CLK_CMD_PA_DIV_SHIFT       (8u)
#define PRS_CLK_CMD_DISABLE_SHIFT      (30u)
#define PRS_CLK_CMD_ENABLE_SHIFT       (31u)

#define PRS_CLK_CMD_DISABLE_MASK       ((uint32)((uint32)1u << PRS_CLK_CMD_DISABLE_SHIFT))
#define PRS_CLK_CMD_ENABLE_MASK        ((uint32)((uint32)1u << PRS_CLK_CMD_ENABLE_SHIFT))

#define PRS_CLK_DIV_FRAC_MASK  (0x000000F8u)
#define PRS_CLK_DIV_FRAC_SHIFT (3u)
#define PRS_CLK_DIV_INT_MASK   (0xFFFFFF00u)
#define PRS_CLK_DIV_INT_SHIFT  (8u)

#else 

#define PRS_CLK_DIV_REG        (*(reg32 *)PRS_CLK__REGISTER)
#define PRS_CLK_ENABLE_REG     PRS_CLK_DIV_REG
#define PRS_CLK_DIV_FRAC_MASK  PRS_CLK__FRAC_MASK
#define PRS_CLK_DIV_FRAC_SHIFT (16u)
#define PRS_CLK_DIV_INT_MASK   PRS_CLK__DIVIDER_MASK
#define PRS_CLK_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_PRS_CLK_H) */

/* [] END OF FILE */
