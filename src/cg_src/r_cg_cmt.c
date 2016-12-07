/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2015, 2016 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_cmt.c
* Version      : Code Generator for RZ/T1 V1.03.00.02 [25 Aug 2016]
* Device(s)    : R7S910018CBG
* Tool-Chain   : GCCARM
* Description  : This file implements device driver for CMT module.
* Creation Date: 2016/12/07
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cmt.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_CMT0_Create
* Description  : This function initializes the CMT0 channel.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CMT0_Create(void)
{
    /* Disable CMI0 interrupt */
    VIC.IEC0.LONG = 0x00200000UL;

    /* Cancel CMT stop state in LPC */
    MSTP(CMT0) = 0U;

    /* Set control registers */
    CMT0.CMCR.WORD = _CMT_CMCR_CKS_PCLK512 | _CMT_CMCR_CMIE_ENABLE;
    CMT0.CMCOR = _CMT0_CMCOR_VALUE;

    /* Set CMI0 edge detection type */
    VIC.PLS0.LONG |= 0x00200000UL;

    /* Set CMI0 priority level */
    VIC.PRL21.LONG = _CMT_PRIORITY_LEVEL0;

    /* Set CMI0 interrupt address */
    VIC.VAD21.LONG = (uint32_t)r_cmt_cmi0_interrupt;
}
/***********************************************************************************************************************
* Function Name: R_CMT0_Start
* Description  : This function starts the CMT0 channel counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CMT0_Start(void)
{
    /* Enable CMI0 interrupt in ICU */
    VIC.IEN0.LONG |= 0x00200000UL;

    /* Start CMT0 count */
    CMT.CMSTR0.BIT.STR0 = 1U;
}
/***********************************************************************************************************************
* Function Name: R_CMT0_Stop
* Description  : This function stops the CMT0 channel counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CMT0_Stop(void)
{
    /* Disable CMI0 interrupt in ICU */
    VIC.IEC0.LONG = 0x00200000UL;

    /* Stop CMT0 count */
    CMT.CMSTR0.BIT.STR0 = 0U;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
