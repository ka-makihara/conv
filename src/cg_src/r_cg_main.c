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
* File Name    : r_cg_main.c
* Version      : Code Generator for RZ/T1 V1.03.00.02 [25 Aug 2016]
* Device(s)    : R7S910018CBG
* Tool-Chain   : GCCARM
* Description  : This file implements main function.
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
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_cmt.h"
#include "r_cg_cmtw.h"
#include "r_cg_scifa.h"
#include "r_cg_rspi.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
extern void wm_rp_04s_init(void);

void rsi_delayMs (uint16_t delay)
{

}

void sys_wait(int tm)
{
	int i;

	for(i = 0; i < tm; i++){
		asm("nop");
	}
}
extern void init_quail(void);
extern void recv_quail();
extern void waitMs(int ms);

extern int32_t endat_main(void);

uint16_t	__spi_recv[2]={0};
uint16_t	__spi_data[2];
/* End user code. Do not edit comment generated here */

extern void R_Systeminit(void);
void R_MAIN_UserInit(void);
/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
    R_Systeminit();

    R_MAIN_UserInit();

    /* Start user code. Do not edit comment generated here */
    while (1U)
    {
    	waitMs(1000);
    	PORTN.PODR.BIT.B6 ^= 1;
/*
    	 int32_t ans;

    	 ans = IoGetchar();
    	 if(ans >= 0){
    	     IoPutchar(ans);
    	 }
*/
        ;
    }
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: R_MAIN_UserInit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MAIN_UserInit(void)
{
    /* Start user code. Do not edit comment generated here */
	int i;

	R_CMTW0_Start();
	R_CMTW1_Start();

	R_SCIFA0_Start();

	__spi_data[0] = 0x15;
	__spi_data[1] = 0;
	__spi_recv[0] = 0xff;
	__spi_recv[1] = 0xaa;
	R_RSPI1_Start();
	R_RSPI1_Send_Receive((uint32_t*)__spi_data,2,(uint32_t*)&__spi_recv);

	//wm_rp_04s_init();					/* WM-RP-04S�ｿｽ�ｿｽ�ｿｽ�ｿｽ �ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ */

	//wm_rp_04s_connect();		/* �ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽﾚ托ｿｽ */
	waitMs(1);
	init_quail();
	endat_main();

    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
