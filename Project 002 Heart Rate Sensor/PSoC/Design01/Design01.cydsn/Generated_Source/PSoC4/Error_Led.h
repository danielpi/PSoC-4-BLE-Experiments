/*******************************************************************************
* File Name: Error_Led.h  
* Version 2.10
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Error_Led_H) /* Pins Error_Led_H */
#define CY_PINS_Error_Led_H

#include "cytypes.h"
#include "cyfitter.h"
#include "Error_Led_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    Error_Led_Write(uint8 value) ;
void    Error_Led_SetDriveMode(uint8 mode) ;
uint8   Error_Led_ReadDataReg(void) ;
uint8   Error_Led_Read(void) ;
uint8   Error_Led_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Error_Led_DRIVE_MODE_BITS        (3)
#define Error_Led_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - Error_Led_DRIVE_MODE_BITS))

#define Error_Led_DM_ALG_HIZ         (0x00u)
#define Error_Led_DM_DIG_HIZ         (0x01u)
#define Error_Led_DM_RES_UP          (0x02u)
#define Error_Led_DM_RES_DWN         (0x03u)
#define Error_Led_DM_OD_LO           (0x04u)
#define Error_Led_DM_OD_HI           (0x05u)
#define Error_Led_DM_STRONG          (0x06u)
#define Error_Led_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define Error_Led_MASK               Error_Led__MASK
#define Error_Led_SHIFT              Error_Led__SHIFT
#define Error_Led_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Error_Led_PS                     (* (reg32 *) Error_Led__PS)
/* Port Configuration */
#define Error_Led_PC                     (* (reg32 *) Error_Led__PC)
/* Data Register */
#define Error_Led_DR                     (* (reg32 *) Error_Led__DR)
/* Input Buffer Disable Override */
#define Error_Led_INP_DIS                (* (reg32 *) Error_Led__PC2)


#if defined(Error_Led__INTSTAT)  /* Interrupt Registers */

    #define Error_Led_INTSTAT                (* (reg32 *) Error_Led__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define Error_Led_DRIVE_MODE_SHIFT       (0x00u)
#define Error_Led_DRIVE_MODE_MASK        (0x07u << Error_Led_DRIVE_MODE_SHIFT)


#endif /* End Pins Error_Led_H */


/* [] END OF FILE */
