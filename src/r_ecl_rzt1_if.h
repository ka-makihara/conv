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
* System Name  : Encoder I/F Configuration Library
* File Name    : r_ecl_rzt1_if.h
* Version      : $Rev: 58 $
* Device       : RZ/T1
* Abstract     : Header file of Encoder I/F Configuration Library.
* Tool-Chain   : IAR Embedded Workbench for ARM, ARM Development Studio 5,
*                Renesas e2 studio
* OS           : Not use
* H/W Platform : Renesas Starter Kit+ for RZ/T1
* Description  : Header file for using Encoder I/F Configuration Library.
* Limitation   : None
*******************************************************************************/
/*******************************************************************************
* History      : History is managed by Revision Control System.
*******************************************************************************/

#ifndef R_ECL_RZT1_IF_H
#define R_ECL_RZT1_IF_H

/*******************************************************************************
Includes <System Includes> , "Project Includes"
*******************************************************************************/

/*******************************************************************************
Macro definitions
*******************************************************************************/
#define R_ECL_SUCCESS       (0)
#define R_ECL_ERR_ARG       (-1)
#define R_ECL_ERR_FORMAT    (-2)
#define R_ECL_ERR_CRC       (-3)
#define R_ECL_ERR_DEVICE    (-4)
#define R_ECL_ERR_BUSY      (-5)
#define R_ECL_ERR_INTERNAL  (-6)

/*******************************************************************************
Typedef definitions
*******************************************************************************/

/*******************************************************************************
Public Functions
*******************************************************************************/
int R_ECL_Configure(const void *const pconfig);
int R_ECL_Start(const int id, const int freq);
int R_ECL_Stop(const int id);
unsigned int R_ECL_GetVersion(void);

#endif /* R_ECL_RZT1_IF_H */
