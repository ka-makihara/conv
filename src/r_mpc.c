/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* System Name  : RZ/T1 Init program
* File Name    : r_mpc.c
* Version      : 0.1
* Device       : R7S9100xx
* Abstract     : API for MPC function
* OS           : not use
* H/W Platform : Renesas Starter Kit for RZ/T1(Preliminary)
* Description  : MPC setting API of RZ/T1
* Limitation   : none
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              :                     First Release
*******************************************************************************/

/*******************************************************************************
Includes <System Includes> , "Project Includes"
*******************************************************************************/
#include "typedefine.h"
#include "iodefine.h"
#include "r_system.h"
#include "r_mpc.h"

/*******************************************************************************
Macro definitions
*******************************************************************************/
#define MPC_PFSWE_WRITE_ENABLE (0x00)
#define MPC_PFS_WRITE_ENABLE   (0x40)
#define MPC_PFS_WRITE_DISABLE  (0x80)

/*******************************************************************************
Typedef definitions
*******************************************************************************/

/*******************************************************************************
Imported global variables and functions (from other files)
*******************************************************************************/

/*******************************************************************************
Exported global variables and functions (to be accessed by other files)
*******************************************************************************/

/*******************************************************************************
Private variables and functions
*******************************************************************************/

/*******************************************************************************
* Function Name : R_MPC_WriteEnable
* Description   : Enables writing to the PmnPFS register (m = 0-9, A-U, n = 0-7).
                  And dummy read the register in order to fix the register value.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void R_MPC_WriteEnable(void)
{
    volatile uint8_t dummy; 
  
    /* Enables writing to the PmnPFS register */
    MPC.PWPR.BYTE = MPC_PFSWE_WRITE_ENABLE;  // Enables writing to the PFSWE bit
    dummy = MPC.PWPR.BYTE;
    MPC.PWPR.BYTE = MPC_PFS_WRITE_ENABLE; // Enables writing to the PmnPFS register
    dummy = MPC.PWPR.BYTE;
    
}

/*******************************************************************************
 End of function R_MPC_WriteEnable
*******************************************************************************/

/*******************************************************************************
* Function Name : R_MPC_WriteDisable
* Description   : Disables writing to the PmnPFS register (m = 0-9, A-U, n = 0-7).
                  And dummy read the register in order to fix the register value.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void R_MPC_WriteDisable(void)
{
    volatile uint8_t dummy; 
  
    /* Disables writing to the PmnPFS register */
    MPC.PWPR.BYTE = MPC_PFS_WRITE_DISABLE;  // Disables writing to the PmnPFS register
    dummy = MPC.PWPR.BYTE;
    
}

/*******************************************************************************
 End of function R_MPC_WriteDisable
*******************************************************************************/

/* End of File */


