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
* File Name    : r_cg_cmtw.h
* Version      : Code Generator for RZ/T1 V1.03.00.02 [25 Aug 2016]
* Device(s)    : R7S910018CBG
* Tool-Chain   : GCCARM
* Description  : This file implements device driver for CMTW module.
* Creation Date: 2016/12/16
***********************************************************************************************************************/
#ifndef CMTW_H
#define CMTW_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/
/*
    Timer Start Register (CMWSTR)
*/
/* Counter start (STR) */
#define _CMTW_CMWSTR_COUNTER_STOP            (0x0000U) /* Stop counter count */
#define _CMTW_CMWSTR_COUNTER_START           (0x0001U) /* Start counter count */

/*
    Timer Control Register (CMWCR)
*/
/* Clock select (CKS[1:0]) */
#define _CMTW_CMWCR_CLOCK_PCLKD8             (0x0000U) /* PCLKD/8 */
#define _CMTW_CMWCR_CLOCK_PCLKD32            (0x0001U) /* PCLKD/32 */
#define _CMTW_CMWCR_CLOCK_PCLKD128           (0x0002U) /* PCLKD/128 */
#define _CMTW_CMWCR_CLOCK_PCLKD512           (0x0003U) /* PCLKD/512 */
/* Compare Match Interrupt Enable (CMWIE) */
#define _CMTW_CMWCR_CMWIE_DISABLE            (0x0000U) /* Disable Compare Match Interrupt */
#define _CMTW_CMWCR_CMWIE_ENABLE             (0x0008U) /* Enable Compare Match Interrupt */
/* Input Capture 0 Interrupt Enable (IC0IE) */
#define _CMTW_CMWCR_IC0IE_DISABLE            (0x0000U) /* Disable Input Capture 0 Interrupt */
#define _CMTW_CMWCR_IC0IE_ENABLE             (0x0010U) /* Enable Input Capture 0 Interrupt */
/* Input Capture 1 Interrupt Enable (IC1IE) */
#define _CMTW_CMWCR_IC1IE_DISABLE            (0x0000U) /* Disable Input Capture 1 Interrupt */
#define _CMTW_CMWCR_IC1IE_ENABLE             (0x0020U) /* Enable Input Capture 1 Interrupt */
/* Output Compare 0 Interrupt Enable (OC0IE) */
#define _CMTW_CMWCR_OC0IE_DISABLE            (0x0000U) /* Disable Output Compare 0 Interrupt */
#define _CMTW_CMWCR_OC0IE_ENABLE             (0x0040U) /* Enable Output Compare 0 Interrupt */
/* Output Compare 1 Interrupt Enable (OC1IE) */
#define _CMTW_CMWCR_OC1IE_DISABLE            (0x0000U) /* Disable Output Compare 1 Interrupt */
#define _CMTW_CMWCR_OC1IE_ENABLE             (0x0080U) /* Enable Output Compare 1 Interrupt */
/* Timer Counter Size (CMS) */
#define _CMTW_CMWCR_COUNTER_SIZE_32          (0x0000U) /* 32 bits */
#define _CMTW_CMWCR_COUNTER_SIZE_16          (0x0200U) /* 16 bits */
/* Counter Clear (CCLR[2:0]) */
#define _CMTW_CMWCR_CCLR_ENABLE_CMWCOR       (0x0000U) /* CMWCNT counter cleared by CMWCOR */
#define _CMTW_CMWCR_CCLR_DISABLE             (0x2000U) /* Clearing of CMWCNT counter disabled */
#define _CMTW_CMWCR_CCLR_CMWICR0_ENABLE      (0x8000U) /* CMWCNT counter cleared by CMWICR0 */
#define _CMTW_CMWCR_CCLR_CMWICR1_ENABLE      (0xA000U) /* CMWCNT counter cleared by CMWICR1 */
#define _CMTW_CMWCR_CCLR_CMWOCR0_ENABLE      (0xC000U) /* CMWCNT counter cleared by CMWOCR0 */
#define _CMTW_CMWCR_CCLR_CMWOCR1_ENABLE      (0xE000U) /* CMWCNT counter cleared by CMWOCR1 */

/*
    Timer I/O Control Register (CMWIOR)
*/
/* Input Compare Control 0 (IC0[1:0]) */
#define _CMTW_CMWIOR_IC0_RISE                (0x0000U) /* Rising edge */
#define _CMTW_CMWIOR_IC0_FALL                (0x0001U) /* Falling edge */
#define _CMTW_CMWIOR_IC0_BOTH                (0x0002U) /* Both edges */
/* Input Capture Control 1 (IC1[1:0]) */
#define _CMTW_CMWIOR_IC1_RISE                (0x0000U) /* Rising edge */
#define _CMTW_CMWIOR_IC1_FALL                (0x0004U) /* Falling edge */
#define _CMTW_CMWIOR_IC1_BOTH                (0x0008U) /* Both edges */
/* Input Capture Enable 0 (IC0E) */
#define _CMTW_CMWIOR_IC0E_DISABLE            (0x0000U) /* Disable input capture of CMWICR0 */
#define _CMTW_CMWIOR_IC0E_ENABLE             (0x0010U) /* Enable input capture of CMWICR0 */
/* Input Capture Enable 1 (IC1E) */
#define _CMTW_CMWIOR_IC1E_DISABLE            (0x0000U) /* Disable input capture of CMWICR1 */
#define _CMTW_CMWIOR_IC1E_ENABLE             (0x0020U) /* Enable input capture of CMWICR1 */
/* Output Compare Control 0 (OC0[1:0]) */
#define _CMTW_CMWIOR_OC0_RETAIN              (0x0000U) /* Retains the output value */
#define _CMTW_CMWIOR_OC0_OUTPUT0             (0x0100U) /* Initially outputs 0 */
#define _CMTW_CMWIOR_OC0_OUTPUT1             (0x0200U) /* Initially outputs 1 */
/* Output Compare Control 1 (OC1[1:0]) */
#define _CMTW_CMWIOR_OC1_RETAIN              (0x0000U) /* Retains the output value */
#define _CMTW_CMWIOR_OC1_OUTPUT0             (0x0400U) /* Initially outputs 0 */
#define _CMTW_CMWIOR_OC1_OUTPUT1             (0x0800U) /* Initially outputs 1 */
/* Compare Match Enable 0 (OC0E) */
#define _CMTW_CMWIOR_OC0E_DISABLE            (0x0000U) /* Disable compare match using CMWOCR0 */
#define _CMTW_CMWIOR_OC0E_ENABLE             (0x1000U) /* Enable compare match using CMWOCR0 */
/* Compare Match Enable 1 (OC1E) */
#define _CMTW_CMWIOR_OC1E_DISABLE            (0x0000U) /* Disable compare match using CMWOCR1 */
#define _CMTW_CMWIOR_OC1E_ENABLE             (0x2000U) /* Enable compare match using CMWOCR1 */
/* Compare Match Enable (CMWE) */
#define _CMTW_CMWIOR_CMWE_DISABLE            (0x0000U) /* Disable compare match using CMWCOR */
#define _CMTW_CMWIOR_CMWE_ENABLE             (0x8000U) /* Enable compare match using CMWCOR */

/* 
    Digital Noise Filter Control Register 0 (NFCR0)
*/
/* Digital Noise Filter Enable 0 (NF0EN) */
#define _CMTW_NFCR0_NF0EN_DISABLE            (0x00000000UL) /* Enable digital noise filter for the TIC0 pin */
#define _CMTW_NFCR0_NF0EN_ENABLE             (0x00000001UL) /* Disable digital noise filter for the TIC0 pin */
/* Digital Noise Filter Enable 0 (NF1EN) */
#define _CMTW_NFCR0_NF1EN_DISABLE            (0x00000000UL) /* Enable digital noise filter for the TIC1 pin */
#define _CMTW_NFCR0_NF1EN_ENABLE             (0x00000002UL) /* Disable digital noise filter for the TIC1 pin */
/* Digital Noise Filter Clock Select 0 (NFCS0[1:0]) */
#define _CMTW_NFCR0_CLOCK_PCLKD              (0x00000000UL) /* PCLKD/1 */
#define _CMTW_NFCR0_CLOCK_PCLKD8             (0x00000004UL) /* PCLKD/8 */
#define _CMTW_NFCR0_CLOCK_PCLKD32            (0x00000008UL) /* PCLKD/32 */
#define _CMTW_NFCR0_CLOCK_PCLKD64            (0x0000000CUL) /* PCLKD/64 */

/* 
    Digital Noise Filter Control Register 1 (NFCR1)
*/
/* Digital Noise Filter Enable 0 (NF2EN) */
#define _CMTW_NFCR1_NF2EN_DISABLE            (0x00000000UL) /* Disable digital noise filter for the TIC2 pin */
#define _CMTW_NFCR1_NF2EN_ENABLE             (0x00000001UL) /* Enable digital noise filter for the TIC2 pin */
/* Digital Noise Filter Enable 0 (NF3EN) */
#define _CMTW_NFCR1_NF3EN_DISABLE            (0x00000000UL) /* Disable digital noise filter for the TIC3 pin */
#define _CMTW_NFCR1_NF3EN_ENABLE             (0x00000002UL) /* Enable digital noise filter for the TIC3 pin */
/* Digital Noise Filter Clock Select 1 (NFCS1[1:0]) */
#define _CMTW_NFCR1_CLOCK_PCLKD              (0x00000000UL) /* PCLKD/1 */
#define _CMTW_NFCR1_CLOCK_PCLKD8             (0x00000004UL) /* PCLKD/8 */
#define _CMTW_NFCR1_CLOCK_PCLKD32            (0x00000008UL) /* PCLKD/32 */
#define _CMTW_NFCR1_CLOCK_PCLKD64            (0x0000000CUL) /* PCLKD/64 */

/*
    ECM Dynamic Mode Error Output Select Register (ECDMESLR)
*/
/* ECM Dynamic Mode Error Output Select (DMERSL[2:0]) */
#define _CMTW_ECDMESLR_DMERSL_NOTUSED        (0x00000000UL) /* Output compare signal not used */
#define _CMTW_ECDMESLR_DMERSL_CMTW0_CMWCOR0  (0x00000001UL) /* Select CMTW0 output compare 0 signal */
#define _CMTW_ECDMESLR_DMERSL_CMTW0_CMWCOR1  (0x00000002UL) /* Select CMTW0 output compare 1 signal */
#define _CMTW_ECDMESLR_DMERSL_CMTW1_CMWCOR0  (0x00000003UL) /* Select CMTW1 output compare 0 signal */
#define _CMTW_ECDMESLR_DMERSL_CMTW1_CMWCOR1  (0x00000004UL) /* Select CMTW1 output compare 1 signal */

/*
    Interrupt Source Priority Register n (IPRn)
*/
/* Interrupt Priority Level Select (IPR[3:0]) */
#define _CMTW_PRIORITY_LEVEL0                (0x00000000UL) /* Level 0 (highest) */
#define _CMTW_PRIORITY_LEVEL1                (0x00000001UL) /* Level 1 */
#define _CMTW_PRIORITY_LEVEL2                (0x00000002UL) /* Level 2 */
#define _CMTW_PRIORITY_LEVEL3                (0x00000003UL) /* Level 3 */
#define _CMTW_PRIORITY_LEVEL4                (0x00000004UL) /* Level 4 */
#define _CMTW_PRIORITY_LEVEL5                (0x00000005UL) /* Level 5 */
#define _CMTW_PRIORITY_LEVEL6                (0x00000006UL) /* Level 6 */
#define _CMTW_PRIORITY_LEVEL7                (0x00000007UL) /* Level 7 */
#define _CMTW_PRIORITY_LEVEL8                (0x00000008UL) /* Level 8 */
#define _CMTW_PRIORITY_LEVEL9                (0x00000009UL) /* Level 9 */
#define _CMTW_PRIORITY_LEVEL10               (0x0000000AUL) /* Level 10 */
#define _CMTW_PRIORITY_LEVEL11               (0x0000000BUL) /* Level 11 */
#define _CMTW_PRIORITY_LEVEL12               (0x0000000CUL) /* Level 12 */
#define _CMTW_PRIORITY_LEVEL13               (0x0000000DUL) /* Level 13 */
#define _CMTW_PRIORITY_LEVEL14               (0x0000000EUL) /* Level 14 */
#define _CMTW_PRIORITY_LEVEL15               (0x0000000FUL) /* Level 15 */

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* Compare Match Constant Register (CMWCOR) */
#define _CMTW1_CMWCOR_VALUE                  (0x0000249EUL)
/* Output Compare Registers 0 (CMWOCR0) */
#define _CMTW1_CMWOCR0_VALUE                 (0x0000000AUL)
/* Output Compare Registers 1 (CMWOCR1) */
#define _CMTW1_CMWOCR1_VALUE                 (0x0000000AUL)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_CMTW1_Create(void);
void R_CMTW1_Start(void);
void R_CMTW1_Stop(void);

/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif