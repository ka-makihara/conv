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
* File Name    : r_cg_cmtw_user.c
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
unsigned long __ticks;

/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_cmtw_cmwi1_interrupt
* Description  : This function is CMWI1 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void r_cmtw_cmwi1_interrupt(void)
{
    /* Clear the interrupt source */
    VIC.PIC0.LONG = 0x40000000UL;

    /* Start user code. Do not edit comment generated here */
    __ticks++;
    /* End user code. Do not edit comment generated here */

    /* Dummy write */
    VIC.HVA0.LONG = 0x00000000UL;
    asm("dmb");
}
/***********************************************************************************************************************
* Function Name: r_cmtw_ic0i1_interrupt
* Description  : This function is IC0I1 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void r_cmtw_ic0i1_interrupt(void)
{
    /* Clear the interrupt source */
    VIC.PIC0.LONG = 0x80000000UL;

    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */

    /* Dummy write */
    VIC.HVA0.LONG = 0x00000000UL;
    asm("dmb");
}
/***********************************************************************************************************************
* Function Name: r_cmtw_ic1i1_interrupt
* Description  : This function is IC1I1 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void r_cmtw_ic1i1_interrupt(void)
{
    /* Clear the interrupt source */
    VIC.PIC1.LONG = 0x00000001UL;

    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */

    /* Dummy write */
    VIC.HVA0.LONG = 0x00000000UL;
    asm("dmb");
}
/***********************************************************************************************************************
* Function Name: r_cmtw_oc0i1_interrupt
* Description  : This function is OC0I1 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void r_cmtw_oc0i1_interrupt(void)
{
    /* Clear the interrupt source */
    VIC.PIC1.LONG = 0x00000002UL;

    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */

    /* Dummy write */
    VIC.HVA0.LONG = 0x00000000UL;
    asm("dmb");
}
/***********************************************************************************************************************
* Function Name: r_cmtw_oc1i1_interrupt
* Description  : This function is OC1I1 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void r_cmtw_oc1i1_interrupt(void)
{
    /* Clear the interrupt source */
    VIC.PIC1.LONG = 0x00000004UL;

    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */

    /* Dummy write */
    VIC.HVA0.LONG = 0x00000000UL;
    asm("dmb");
}

/* Start user code for adding. Do not edit comment generated here */
unsigned long get_tick(void)
{
	return __ticks;
}

void waitMs(int ms)
{
	unsigned long tc = get_tick();

	while(1){
		unsigned long tt = get_tick();
		if( tt - tc >= ms){
			break;
		}
	}
}
/* End user code. Do not edit comment generated here */
