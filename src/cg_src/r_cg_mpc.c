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
* File Name    : r_cg_mpc.c
* Version      : Code Generator for RZ/T1 V1.03.00.02 [25 Aug 2016]
* Device(s)    : R7S910018CBG
* Tool-Chain   : GCCARM
* Description  : Setting of port and mpc registers.
* Creation Date: 2016/12/08
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
#include "r_cg_mpc.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_MPC_Create
* Description  : This function initializes the Port I/O.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MPC_Create(void)
{
    /* Set RSPCK0 pin */
    MPC.P77PFS.BYTE |= 0x0DU;
    PORT7.PMR.BYTE |= 0x80U;

    /* Set MOSI0 pin */
    MPC.PE5PFS.BYTE |= 0x0EU;
    PORTE.PMR.BYTE |= 0x20U;

    /* Set MISO0 pin */
    MPC.PA1PFS.BYTE |= 0x0DU;
    PORTA.PMR.BYTE |= 0x02U;

    /* Set SSL00 pin */
    MPC.P75PFS.BYTE |= 0x0DU;
    PORT7.PMR.BYTE |= 0x20U;

    /* Set SSL01 pin */
    MPC.P76PFS.BYTE |= 0x0DU;
    PORT7.PMR.BYTE |= 0x40U;

    /* Set SSL02 pin */
    MPC.PA2PFS.BYTE |= 0x0DU;
    PORTA.PMR.BYTE |= 0x04U;

    /* Set SSL03 pin */
    MPC.P74PFS.BYTE |= 0x0DU;
    PORT7.PMR.BYTE |= 0x10U;

    /* Set RSPCK1 pin */
    MPC.PN3PFS.BYTE |= 0x0EU;
    PORTN.PMR.BYTE |= 0x08U;

    /* Set MOSI1 pin */
    MPC.PN2PFS.BYTE |= 0x0EU;
    PORTN.PMR.BYTE |= 0x04U;

    /* Set MISO1 pin */
    MPC.PN1PFS.BYTE |= 0x0EU;
    PORTN.PMR.BYTE |= 0x02U;

    /* Set SSL10 pin */
    MPC.PN0PFS.BYTE |= 0x0EU;
    PORTN.PMR.BYTE |= 0x01U;

    /* Set SSL11 pin */
    MPC.PG6PFS.BYTE |= 0x0DU;
    PORTG.PMR.BYTE |= 0x40U;

    /* Set TXD0 pin */
    MPC.P23PFS.BYTE |= 0x0BU;
    PORT2.PMR.BYTE |= 0x08U;

    /* Set RXD0 pin */
    MPC.P24PFS.BYTE |= 0x0BU;
    PORT2.PMR.BYTE |= 0x10U;

    /* Set TOC0 pin */
    MPC.PD7PFS.BYTE |= 0x1DU;
    PORTD.PMR.BYTE |= 0x80U;

    /* Set TIC0 pin */
    MPC.PD5PFS.BYTE |= 0x1DU;
    PORTD.PMR.BYTE |= 0x20U;

    /* Set TOC1 pin */
    MPC.P86PFS.BYTE |= 0x1DU;
    PORT8.PMR.BYTE |= 0x40U;

    /* Set TIC1 pin */
    MPC.PD6PFS.BYTE |= 0x1DU;
    PORTD.PMR.BYTE |= 0x40U;

    /* Set TOC2 pin */
    MPC.PF6PFS.BYTE |= 0x1DU;
    PORTF.PMR.BYTE |= 0x40U;

    /* Set TIC2 pin */
    MPC.PF5PFS.BYTE |= 0x1DU;
    PORTF.PMR.BYTE |= 0x20U;

    /* Set TOC3 pin */
    MPC.PB7PFS.BYTE |= 0x1DU;
    PORTB.PMR.BYTE |= 0x80U;

    /* Set TIC3 pin */
    MPC.PB0PFS.BYTE |= 0x1DU;
    PORTB.PMR.BYTE |= 0x01U;

    /* Set TDO pin */
    MPC.P33PFS.BYTE |= 0x27U;
    PORT3.PMR.BYTE |= 0x08U;

    /* Set TDI pin */
    MPC.P34PFS.BYTE |= 0x27U;
    PORT3.PMR.BYTE |= 0x10U;

    R_MPC_Create_UserInit();
}
/***********************************************************************************************************************
* Function Name: R_MPC_Create_UserInit
* Description  : This function adds user code after initializing modules pin setting.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MPC_Create_UserInit(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
