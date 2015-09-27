/*******************************************************************************
* File Name: RedGreenPrISM.h
* Version 2.20
*
* Description:
*  This file provides constants and parameter values for the PrISM
*  Component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/


#if !defined(CY_PrISM_RedGreenPrISM_H)    /* CY_PrISM_RedGreenPrISM_H */
#define CY_PrISM_RedGreenPrISM_H

#include "CyLib.h"
#include "cytypes.h"
#include "cyfitter.h"


/***************************************
* Conditional Compilation Parameters
***************************************/

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component PrISM_v2_20 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */


/***************************************
*    Initial Parameter Constants
***************************************/
                                     
#define RedGreenPrISM_RESOLUTION             (8u)
#define RedGreenPrISM_PULSE_TYPE_HARDCODED   (0u)


/***************************************
*       Type defines
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 enableState;
    #if(!RedGreenPrISM_PULSE_TYPE_HARDCODED)
        uint8 cr;
    #endif /* End RedGreenPrISM_PULSE_TYPE_HARDCODED */
    uint8 seed;
    uint8 seed_copy;
    uint8 polynom;
    #if(CY_UDB_V0)
        uint8 density0;
        uint8 density1;
    #endif /* End CY_UDB_V0 */
} RedGreenPrISM_BACKUP_STRUCT;


/***************************************
*       Function Prototypes
***************************************/

void RedGreenPrISM_Start(void) ;
void RedGreenPrISM_Stop(void) ;
void RedGreenPrISM_SetPulse0Mode(uint8 pulse0Type) ;
void RedGreenPrISM_SetPulse1Mode(uint8 pulse1Type) ;
uint8 RedGreenPrISM_ReadSeed(void) ;
void RedGreenPrISM_WriteSeed(uint8 seed) ;
uint8 RedGreenPrISM_ReadPolynomial(void) ;
void RedGreenPrISM_WritePolynomial(uint8 polynomial) \
                                                                ;
uint8 RedGreenPrISM_ReadPulse0(void) ;
void RedGreenPrISM_WritePulse0(uint8 pulseDensity0) \
                                                                ;
uint8 RedGreenPrISM_ReadPulse1(void) ;
void RedGreenPrISM_WritePulse1(uint8 pulseDensity1) \
                                                                ;
void RedGreenPrISM_Sleep(void) ;
void RedGreenPrISM_Wakeup(void) ;
void RedGreenPrISM_SaveConfig(void) ;
void RedGreenPrISM_RestoreConfig(void) ;
void RedGreenPrISM_Enable(void) ;
void RedGreenPrISM_Init(void) ;


/***************************************
*          API Constants
***************************************/

/* Constants for SetPulse0Mode(), SetPulse1Mode(), pulse type */
#define RedGreenPrISM_LESSTHAN_OR_EQUAL      (0x00u)
#define RedGreenPrISM_GREATERTHAN_OR_EQUAL   (0x01u)


/***************************************
* Global variables external identifier
***************************************/

extern uint8 RedGreenPrISM_initVar;


/***************************************
*    Initial Parameter Constants
***************************************/

#define RedGreenPrISM_POLYNOM                (0xB8u)
#define RedGreenPrISM_SEED                   (0xFFu)
#define RedGreenPrISM_DENSITY0               (0x1u)
#define RedGreenPrISM_DENSITY1               (0x1u)
#define RedGreenPrISM_COMPARE0               (0u)
#define RedGreenPrISM_COMPARE1               (0u)


/***************************************
*              Registers
***************************************/

#if (RedGreenPrISM_RESOLUTION  <= 8u) /* 8bit - PrISM */
    #define RedGreenPrISM_DENSITY0_REG       (* (reg8 *) RedGreenPrISM_sC8_PrISMdp_u0__D0_REG)
    #define RedGreenPrISM_DENSITY0_PTR       (  (reg8 *) RedGreenPrISM_sC8_PrISMdp_u0__D0_REG)
    #define RedGreenPrISM_DENSITY1_REG       (* (reg8 *) RedGreenPrISM_sC8_PrISMdp_u0__D1_REG)
    #define RedGreenPrISM_DENSITY1_PTR       (  (reg8 *) RedGreenPrISM_sC8_PrISMdp_u0__D1_REG)
    #define RedGreenPrISM_POLYNOM_REG        (* (reg8 *) RedGreenPrISM_sC8_PrISMdp_u0__A1_REG)
    #define RedGreenPrISM_POLYNOM_PTR        (  (reg8 *) RedGreenPrISM_sC8_PrISMdp_u0__A1_REG)
    #define RedGreenPrISM_SEED_REG           (* (reg8 *) RedGreenPrISM_sC8_PrISMdp_u0__A0_REG)
    #define RedGreenPrISM_SEED_PTR           (  (reg8 *) RedGreenPrISM_sC8_PrISMdp_u0__A0_REG)
    #define RedGreenPrISM_SEED_COPY_REG      (* (reg8 *) RedGreenPrISM_sC8_PrISMdp_u0__F0_REG)
    #define RedGreenPrISM_SEED_COPY_PTR      (  (reg8 *) RedGreenPrISM_sC8_PrISMdp_u0__F0_REG)
    #define RedGreenPrISM_AUX_CONTROL_REG    (* (reg8 *) RedGreenPrISM_sC8_PrISMdp_u0__DP_AUX_CTL_REG)
    #define RedGreenPrISM_AUX_CONTROL_PTR    (  (reg8 *) RedGreenPrISM_sC8_PrISMdp_u0__DP_AUX_CTL_REG)
#elif (RedGreenPrISM_RESOLUTION <= 16u) /* 16bit - PrISM */
    #if(CY_PSOC3) /* 8-bit address space */
        #define RedGreenPrISM_DENSITY0_PTR       ((reg16 *) RedGreenPrISM_sC16_PrISMdp_u0__D0_REG)
        #define RedGreenPrISM_DENSITY1_PTR       ((reg16 *) RedGreenPrISM_sC16_PrISMdp_u0__D1_REG)
        #define RedGreenPrISM_POLYNOM_PTR        ((reg16 *) RedGreenPrISM_sC16_PrISMdp_u0__A1_REG)
        #define RedGreenPrISM_SEED_PTR           ((reg16 *) RedGreenPrISM_sC16_PrISMdp_u0__A0_REG)
        #define RedGreenPrISM_SEED_COPY_PTR      ((reg16 *) RedGreenPrISM_sC16_PrISMdp_u0__F0_REG)
        #define RedGreenPrISM_AUX_CONTROL_PTR    ((reg16 *) RedGreenPrISM_sC16_PrISMdp_u0__DP_AUX_CTL_REG)
    #else /* 16-bit address space */
        #define RedGreenPrISM_DENSITY0_PTR       ((reg16 *) RedGreenPrISM_sC16_PrISMdp_u0__16BIT_D0_REG)
        #define RedGreenPrISM_DENSITY1_PTR       ((reg16 *) RedGreenPrISM_sC16_PrISMdp_u0__16BIT_D1_REG)
        #define RedGreenPrISM_POLYNOM_PTR        ((reg16 *) RedGreenPrISM_sC16_PrISMdp_u0__16BIT_A1_REG)
        #define RedGreenPrISM_SEED_PTR           ((reg16 *) RedGreenPrISM_sC16_PrISMdp_u0__16BIT_A0_REG)
        #define RedGreenPrISM_SEED_COPY_PTR      ((reg16 *) RedGreenPrISM_sC16_PrISMdp_u0__16BIT_F0_REG)
        #define RedGreenPrISM_AUX_CONTROL_PTR    ((reg16 *) RedGreenPrISM_sC16_PrISMdp_u0__16BIT_DP_AUX_CTL_REG)
    #endif /* CY_PSOC3 */
#elif (RedGreenPrISM_RESOLUTION <= 24u) /* 24bit - PrISM */
    #define RedGreenPrISM_DENSITY0_PTR       ((reg32 *) RedGreenPrISM_sC24_PrISMdp_u0__D0_REG)
    #define RedGreenPrISM_DENSITY1_PTR       ((reg32 *) RedGreenPrISM_sC24_PrISMdp_u0__D1_REG)
    #define RedGreenPrISM_POLYNOM_PTR        ((reg32 *) RedGreenPrISM_sC24_PrISMdp_u0__A1_REG)
    #define RedGreenPrISM_SEED_PTR           ((reg32 *) RedGreenPrISM_sC24_PrISMdp_u0__A0_REG)
    #define RedGreenPrISM_SEED_COPY_PTR      ((reg32 *) RedGreenPrISM_sC24_PrISMdp_u0__F0_REG)
    #define RedGreenPrISM_AUX_CONTROL_PTR    ((reg32 *) RedGreenPrISM_sC24_PrISMdp_u0__DP_AUX_CTL_REG)
#else /* 32bit - PrISM */
    #if(CY_PSOC3 || CY_PSOC5) /* 8-bit address space */
        #define RedGreenPrISM_DENSITY0_PTR       ((reg32 *) RedGreenPrISM_sC32_PrISMdp_u0__D0_REG)
        #define RedGreenPrISM_DENSITY1_PTR       ((reg32 *) RedGreenPrISM_sC32_PrISMdp_u0__D1_REG)
        #define RedGreenPrISM_POLYNOM_PTR        ((reg32 *) RedGreenPrISM_sC32_PrISMdp_u0__A1_REG)
        #define RedGreenPrISM_SEED_PTR           ((reg32 *) RedGreenPrISM_sC32_PrISMdp_u0__A0_REG)
        #define RedGreenPrISM_SEED_COPY_PTR      ((reg32 *) RedGreenPrISM_sC32_PrISMdp_u0__F0_REG)
        #define RedGreenPrISM_AUX_CONTROL_PTR    ((reg32 *) RedGreenPrISM_sC32_PrISMdp_u0__DP_AUX_CTL_REG)
    #else /* 32-bit address space */    
        #define RedGreenPrISM_DENSITY0_PTR       ((reg32 *) RedGreenPrISM_sC32_PrISMdp_u0__32BIT_D0_REG)
        #define RedGreenPrISM_DENSITY1_PTR       ((reg32 *) RedGreenPrISM_sC32_PrISMdp_u0__32BIT_D1_REG)
        #define RedGreenPrISM_POLYNOM_PTR        ((reg32 *) RedGreenPrISM_sC32_PrISMdp_u0__32BIT_A1_REG)
        #define RedGreenPrISM_SEED_PTR           ((reg32 *) RedGreenPrISM_sC32_PrISMdp_u0__32BIT_A0_REG)
        #define RedGreenPrISM_SEED_COPY_PTR      ((reg32 *) RedGreenPrISM_sC32_PrISMdp_u0__32BIT_F0_REG)
        #define RedGreenPrISM_AUX_CONTROL_PTR    ((reg32 *) RedGreenPrISM_sC32_PrISMdp_u0__32BIT_DP_AUX_CTL_REG)
    #endif
#endif /* End RedGreenPrISM_RESOLUTION */

#define RedGreenPrISM_CONTROL_REG   (* (reg8 *) RedGreenPrISM_SyncCtl_ControlReg__CONTROL_REG)
#define RedGreenPrISM_CONTROL_PTR   (  (reg8 *) RedGreenPrISM_SyncCtl_ControlReg__CONTROL_REG)


/***************************************
*       Register Constants
***************************************/

#define RedGreenPrISM_CTRL_ENABLE                                (0x01u)
#define RedGreenPrISM_CTRL_COMPARE_TYPE0_GREATER_THAN_OR_EQUAL   (0x02u)
#define RedGreenPrISM_CTRL_COMPARE_TYPE1_GREATER_THAN_OR_EQUAL   (0x04u)

#if (RedGreenPrISM_RESOLUTION  <= 8u) /* 8bit - PrISM */
    #define RedGreenPrISM_FIFO0_CLR                              (0x01u)
#elif (RedGreenPrISM_RESOLUTION <= 16u) /* 16bit - PrISM */
    #define RedGreenPrISM_FIFO0_CLR                              (0x0101u)
#elif (RedGreenPrISM_RESOLUTION <= 24u) /* 24bit - PrISM */
    #define RedGreenPrISM_FIFO0_CLR                              (0x010101Lu)
#else /* 32bit - PrISM */
    #define RedGreenPrISM_FIFO0_CLR                              (0x01010101Lu)
#endif /* End RedGreenPrISM_RESOLUTION */


/***************************************
* Renamed global variables or defines 
* for backward compatible
***************************************/
#define RedGreenPrISM_ReadPusle0     RedGreenPrISM_ReadPulse0
#define RedGreenPrISM_ReadPusle1     RedGreenPrISM_ReadPulse1


#endif  /* End CY_PrISM_RedGreenPrISM_H */


/* [] END OF FILE */
