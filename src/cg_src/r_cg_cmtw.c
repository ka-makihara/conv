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
* File Name    : r_cg_cmtw.c
* Version      : Code Generator for RZ/T1 V1.03.00.02 [25 Aug 2016]
* Device(s)    : R7S910018CBG
* Tool-Chain   : GCCARM
* Description  : This file implements device driver for CMTW module.
* Creation Date: 2016/12/16
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
#include "r_cg_cmtw.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_CMTW1_Create
* Description  : This function initializes the CMWT1 channel.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CMTW1_Create(void)
{
    /* Cancel CMTW stop state in LPC */
    MSTP(CMTW1) = 0U;

    /* Disable CMWI1 interrupt */
    VIC.IEC0.LONG = 0x40000000UL;

    /* Disable IC0I1 interrupt */
    VIC.IEC0.LONG = 0x80000000UL;

    /* Disable IC1I1 interrupt */
    VIC.IEC1.LONG = 0x00000001UL;

    /* Disable OC0I1 interrupt */
    VIC.IEC1.LONG = 0x00000002UL;

    /* Disable OC1I1 interrupt */
    VIC.IEC1.LONG = 0x00000004UL;

    /* Set CMWI1 edge detection type */
    VIC.PLS0.LONG |= 0x40000000UL;

    /* Set IC0I1 edge detection type */
    VIC.PLS0.LONG |= 0x80000000UL;

    /* Set IC1I1 edge detection type */
    VIC.PLS1.LONG |= 0x00000001UL;

    /* Set OC0I1 edge detection type */
    VIC.PLS1.LONG |= 0x00000002UL;

    /* Set OC1I1 edge detection type */
    VIC.PLS1.LONG |= 0x00000004UL;

    /* Stop CMTW1 count */
    CMTW1.CMWSTR.BIT.STR = 0U;

    /* Set timer I/O control register */
    CMTW1.CMWIOR.WORD = _CMTW_CMWIOR_IC0E_ENABLE | _CMTW_CMWIOR_IC0_RISE | _CMTW_CMWIOR_IC1E_ENABLE | 
                        _CMTW_CMWIOR_IC1_RISE | _CMTW_CMWIOR_OC0E_ENABLE | _CMTW_CMWIOR_OC0_RETAIN | 
                        _CMTW_CMWIOR_OC1E_ENABLE | _CMTW_CMWIOR_OC1_RETAIN | _CMTW_CMWIOR_CMWE_ENABLE;

    /* Set compare match register */
    CMTW1.CMWCOR = _CMTW1_CMWCOR_VALUE;

    /* Set output compare register 0 */
    CMTW1.CMWOCR0 = _CMTW1_CMWOCR0_VALUE;

    /* Set output compare register 1 */
    CMTW1.CMWOCR1 = _CMTW1_CMWOCR1_VALUE;

    /* Set digital noise filter control register */
    CMTW.NFCR1.LONG = _CMTW_NFCR1_NF2EN_DISABLE | _CMTW_NFCR1_NF3EN_DISABLE;

    /* Set ECM Dynamic Mode Error Output Select Register */
    CMTW.ECDMESLR.LONG = _CMTW_ECDMESLR_DMERSL_NOTUSED;

    /* Set control register */
    CMTW1.CMWCR.WORD = _CMTW_CMWCR_CLOCK_PCLKD8 | _CMTW_CMWCR_CMWIE_ENABLE | _CMTW_CMWCR_IC0IE_ENABLE | 
                       _CMTW_CMWCR_IC1IE_ENABLE | _CMTW_CMWCR_OC0IE_ENABLE | _CMTW_CMWCR_OC1IE_ENABLE | 
                       _CMTW_CMWCR_COUNTER_SIZE_32 | _CMTW_CMWCR_CCLR_ENABLE_CMWCOR;

    /* Set CMWI1 interrupt priority level */
    VIC.PRL30.LONG = _CMTW_PRIORITY_LEVEL0;

    /* Set CMWI1 interrupt address level */
    VIC.VAD30.LONG = (uint32_t)r_cmtw_cmwi1_interrupt;

    /* Set IC0I1 interrupt priority level */
    VIC.PRL31.LONG = _CMTW_PRIORITY_LEVEL0;

    /* Set IC0I1 interrupt address */
    VIC.VAD31.LONG = (uint32_t)r_cmtw_ic0i1_interrupt;

    /* Set IC1I1 interrupt priority level */
    VIC.PRL32.LONG = _CMTW_PRIORITY_LEVEL0;

    /* Set IC1I1 interrupt address */
    VIC.VAD32.LONG = (uint32_t)r_cmtw_ic1i1_interrupt;

    /* Set OC0I1 interrupt priority level */
    VIC.PRL33.LONG = _CMTW_PRIORITY_LEVEL0;

    /* Set OC0I1 interrupt address */
    VIC.VAD33.LONG = (uint32_t)r_cmtw_oc0i1_interrupt;

    /* Set OC1I1 interrupt priority level */
    VIC.PRL34.LONG = _CMTW_PRIORITY_LEVEL0;

    /* Set OC1I1 interrupt address */
    VIC.VAD34.LONG = (uint32_t)r_cmtw_oc1i1_interrupt;
}
/***********************************************************************************************************************
* Function Name: R_CMTW1_Start
* Description  : This function starts the CMTW1 channel counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CMTW1_Start(void)
{
    /* Enable CMWI1 interrupt*/
    VIC.IEN0.LONG |= 0x40000000UL;

    /* Enable IC0I1 interrupt*/
    VIC.IEN0.LONG |= 0x80000000UL;

    /* Enable IC1I1 interrupt*/
    VIC.IEN1.LONG |= 0x00000001UL;

    /* Enable OC0I1 interrupt*/
    VIC.IEN1.LONG |= 0x00000002UL;

    /* Enable OC1I1 interrupt*/
    VIC.IEN1.LONG |= 0x00000004UL;

    /* Start CMTW1 count */
    CMTW1.CMWSTR.BIT.STR = 1U;
}
/***********************************************************************************************************************
* Function Name: R_CMTW1_Stop
* Description  : This function stops the CMTW1 channel counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CMTW1_Stop(void)
{
    /* Stop CMTW1 count */
    CMTW1.CMWSTR.BIT.STR = 0U;

    /* Disable CMWI1 interrupt */
    VIC.IEC0.LONG = 0x40000000UL;

    /* Disable IC0I1 interrupt */
    VIC.IEC0.LONG = 0x80000000UL;

    /* Disable IC1I1 interrupt */
    VIC.IEC1.LONG = 0x00000001UL;

    /* Disable OC0I1 interrupt */
    VIC.IEC1.LONG = 0x00000002UL;

    /* Disable OC1I1 interrupt */
    VIC.IEC1.LONG = 0x00000004UL;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
