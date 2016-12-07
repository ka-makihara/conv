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
* File Name    : r_cg_interrupthandlers.h
* Version      : Code Generator for RZ/T1 V1.03.00.02 [25 Aug 2016]
* Device(s)    : R7S910018CBG
* Tool-Chain   : GCCARM
* Description  : This file declares interrupt handlers.
* Creation Date: 2016/12/07
***********************************************************************************************************************/
#ifndef INTERRUPT_HANDLERS_H
#define INTERRUPT_HANDLERS_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
/* FIQ exception handler */
void r_fiq_handler(void) __attribute__((interrupt ("FIQ")));

/* CMT CMI0 */
void r_cmt_cmi0_interrupt(void) __attribute__((interrupt ("IRQ")));

/* CMTW CMWI0 */
void r_cmtw_cmwi0_interrupt(void) __attribute__((interrupt ("IRQ")));

/* CMTW CMWI1 */
void r_cmtw_cmwi1_interrupt(void) __attribute__((interrupt ("IRQ")));

/* CMTW IC0I1 */
void r_cmtw_ic0i1_interrupt(void) __attribute__((interrupt ("IRQ")));

/* CMTW IC1I1 */
void r_cmtw_ic1i1_interrupt(void) __attribute__((interrupt ("IRQ")));

/* CMTW OC0I1 */
void r_cmtw_oc0i1_interrupt(void) __attribute__((interrupt ("IRQ")));

/* CMTW OC1I1 */
void r_cmtw_oc1i1_interrupt(void) __attribute__((interrupt ("IRQ")));

/* RSPI0 SPTI0 */
void r_rspi0_transmit_interrupt(void) __attribute__((interrupt ("IRQ")));

/* RSPI0 SPRI0 */
void r_rspi0_receive_interrupt(void) __attribute__((interrupt ("IRQ")));

/* RSPI0 SPEI0 */
void r_rspi0_error_interrupt(void) __attribute__((interrupt ("IRQ")));

/* RSPI0 SPII0 */
void r_rspi0_idle_interrupt(void) __attribute__((interrupt ("IRQ")));

/* RSPI1 SPTI1 */
void r_rspi1_transmit_interrupt(void) __attribute__((interrupt ("IRQ")));

/* RSPI1 SPRI1 */
void r_rspi1_receive_interrupt(void) __attribute__((interrupt ("IRQ")));

/* RSPI1 SPEI1 */
void r_rspi1_error_interrupt(void) __attribute__((interrupt ("IRQ")));

/* RSPI1 SPII1 */
void r_rspi1_idle_interrupt(void) __attribute__((interrupt ("IRQ")));

/* SCIFA TXIF0 */
void r_scifa0_txif0_interrupt(void) __attribute__((interrupt ("IRQ")));

/* SCIFA DRIF0 */
void r_scifa0_drif0_interrupt(void) __attribute__((interrupt ("IRQ")));

/* SCIFA RXIF0 */
void r_scifa0_rxif0_interrupt(void) __attribute__((interrupt ("IRQ")));

/* SCIFA BRIF0 */
void r_scifa0_brif0_interrupt(void) __attribute__((interrupt ("IRQ")));


#endif