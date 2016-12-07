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
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* System Name  : EnDat driver
* File Name    : r_endat_rzt1_config.h
* Version      : $Rev: 201 $
* Device       : RZ
* Abstract     : EnDat driver configuration header file.
* Tool-Chain   : IAR Embedded Workbench for ARM
*                ARM DS-5 Development Studio
*                Renesas e2studio
* OS           : Not use
* H/W Platform : RZ/T1
* Description  : Configuration header file of EnDat driver on RZ devices.
* Limitation   : None
*******************************************************************************/
/*******************************************************************************
* History      : History is managed by Revision Control System.
*******************************************************************************/

#ifndef R_RZT1_ENDAT_CONFIG_H
#define R_RZT1_ENDAT_CONFIG_H

/*******************************************************************************
Includes <System Includes> , "Project Includes"
*******************************************************************************/

/*******************************************************************************
Macro definitions
*******************************************************************************/
#define R_ENDAT_CABLE_DELAY   (5)             /* Number of measurements of delay correction */
#define R_ENDAT0_ISR_PRI      (0u)            /* Interrupt Priority of EnDat */
#define R_ENDAT1_ISR_PRI      (0u)

/*******************************************************************************
Typedef definitions
*******************************************************************************/

/*******************************************************************************
Public Functions
*******************************************************************************/

#endif /* R_RZT1_FAC_CONFIG_H */
