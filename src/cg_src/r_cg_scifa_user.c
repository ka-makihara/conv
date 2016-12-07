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
* File Name    : r_cg_scifa_user.c
* Version      : Code Generator for RZ/T1 V1.03.00.02 [25 Aug 2016]
* Device(s)    : R7S910018CBG
* Tool-Chain   : GCCARM
* Description  : This file implements device driver for SCIF module.
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
#include "r_cg_scifa.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern const uint8_t * gp_scifa0_tx_address;   /* SCIFA0 send buffer address */
extern uint16_t        g_scifa0_tx_count;      /* SCIFA0 send data number */
extern uint8_t *       gp_scifa0_rx_address;   /* SCIFA0 receive buffer address */
extern uint16_t        g_scifa0_rx_count;      /* SCIFA0 receive data number */
extern uint16_t        g_scifa0_rx_length;     /* SCIFA0 receive data length */
/* Start user code for global. Do not edit comment generated here */

extern	int __txComp;
unsigned long trCnt = 0;
unsigned long txCnt = 0;
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_scifa0_txif0_interrupt
* Description  : This function is TXIF0 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void r_scifa0_txif0_interrupt(void)
{
    uint16_t count = 0;

    /* Get the amount of untransmitted data stored in the FRDR register */
    uint16_t dummy_fdr = SCIFA0.FDR.BIT.T;

    /* Write data to the transmit FIFO data register */
    while ((g_scifa0_tx_count > 0U) && (count < _SCIF_FIFO_MAX_SIZE - dummy_fdr))
    {
        SCIFA0.FTDR = *gp_scifa0_tx_address;
        gp_scifa0_tx_address++;
        g_scifa0_tx_count--;
        count++;
    }

    if (SCIFA0.FSR.BIT.TDFE == 1U)
    {
        SCIFA0.FSR.BIT.TDFE = 0U;
    }

    if (g_scifa0_tx_count <= 0U)
    {
        SCIFA0.SCR.BIT.TIE = 0U;
        SCIFA0.SCR.BIT.TEIE = 1U;
    }

    /* Wait the interrupt signal is disabled */
    while (0U != (VIC.IRQS3.LONG & 0x00000004UL))
    {
        VIC.IEC3.LONG = 0x00000004UL;
    }

    VIC.IEN3.LONG |= 0x00000004UL;

    /* Dummy write */
    VIC.HVA0.LONG = 0x00000000UL;
    asm("dmb");
}
/***********************************************************************************************************************
* Function Name: r_scifa0_rxif0_interrupt
* Description  : This function is RXIF0 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void r_scifa0_rxif0_interrupt(void)
{
    uint16_t count = 0;

    /* Get the amount of receive data stored in FRDR register */
    uint16_t dummy_fdr = SCIFA0.FDR.BIT.R;

    /* Read data from the receive FIFO data register */
    while ((g_scifa0_rx_length > g_scifa0_rx_count) && (count < dummy_fdr))
    {
        *gp_scifa0_rx_address = SCIFA0.FRDR;
        gp_scifa0_rx_address++;
        g_scifa0_rx_count++;
        count++;
    }

    /* If remaining data is less than the receive trigger number, receive interrupt will not occur.
       In this case, set trigger number to 1 to force receive interrupt for each one byte of data in FRDR */
    if ((g_scifa0_rx_length - g_scifa0_rx_count < _SCIF_RX_TRIG_NUM_0) && (SCIFA0.FTCR.BIT.RFTC != 1U))
    {
        SCIFA0.FTCR.BIT.RFTC = 1U;
    }

    /* Clear receive FIFO data full flag */
    if (SCIFA0.FSR.BIT.RDF == 1U)
    {
        SCIFA0.FSR.BIT.RDF = 0U;
    }

    if (g_scifa0_rx_length <= g_scifa0_rx_count)
    {
        /* All data received */
        SCIFA0.SCR.BIT.RE = 0U;
        r_scifa0_callback_receiveend();
    }

    /* Wait the interrupt signal is disabled */
    while (0U != (VIC.IRQS3.LONG & 0x00000002UL))
    {
        VIC.IEC3.LONG = 0x00000002UL;
    }

    VIC.IEN3.LONG |= 0x00000002UL;

    /* Dummy write */
    VIC.HVA0.LONG = 0x00000000UL;
    asm("dmb");
}
/***********************************************************************************************************************
* Function Name: r_scifa0_drif0_interrupt
* Description  : This function is TEIF 0 or DRIF0 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void r_scifa0_drif0_interrupt(void)
{
    if (1U == SCIFA0.FSR.BIT.TEND)
    {
        SCIFA0.SCR.BIT.TE = 0U;
        SCIFA0.SCR.BIT.TEIE = 0U;
    }
    r_scifa0_callback_transmitend();

    /* Clear data ready detect flag */
    if (1U == SCIFA0.FSR.BIT.DR)
    {
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
        SCIFA0.FSR.BIT.DR = 0U;  
    }

    /* Wait the interrupt signal is disabled */
    while (0U != (VIC.IRQS3.LONG & 0x00000008UL))
    {
        VIC.IEC3.LONG = 0x00000008UL;
    }

    VIC.IEN3.LONG |= 0x00000008UL;

    /* Dummy write */
    VIC.HVA0.LONG = 0x00000000UL;
    asm("dmb");
}
/***********************************************************************************************************************
* Function Name: r_scifa0_brif0_interrupt
* Description  : This function is BRIF0 or ERIF0 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void r_scifa0_brif0_interrupt(void)
{
    if (1U == SCIFA0.FSR.BIT.BRK)
    {
        r_scifa0_callback_error(BREAK_DETECT);
        /* Clear break detect flag */
        SCIFA0.FSR.BIT.BRK = 0U;
    }

    if (1U == SCIFA0.FSR.BIT.ER)
    {
        r_scifa0_callback_error(RECEIVE_ERROR);
        /* Clear receive error flag */
        SCIFA0.FSR.BIT.ER = 0U;
    }

    if (1U == SCIFA0.LSR.BIT.ORER)
    {
        r_scifa0_callback_error(OVERRUN_ERROR);
        /* Clear overrun error flag */
        SCIFA0.LSR.BIT.ORER = 0U;
    }

    /* Wait the interrupt signal is disabled */
    while (0U != (VIC.IRQS3.LONG & 0x00000001UL))
    {
        VIC.IEC3.LONG = 0x00000001UL;
    }

    VIC.IEN3.LONG |= 0x00000001UL;

    /* Dummy write */
    VIC.HVA0.LONG = 0x00000000UL;
    asm("dmb");
}
/***********************************************************************************************************************
* Function Name: r_scifa0_callback_transmitend
* Description  : This function is a callback function when SCIFA0 finishes transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void r_scifa0_callback_transmitend(void)
{
    /* Start user code. Do not edit comment generated here */
	PORT2.PODR.BIT.B1 = 0;
	trCnt++;
	__txComp = 1;
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_scifa0_callback_receiveend
* Description  : This function is a callback function when SCIFA0 finishes reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void r_scifa0_callback_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_scifa0_callback_error
* Description  : This function is a callback function when SCIFA0 reception encounters error.
* Arguments    : error_type -
*                    reception error type
* Return Value : None
***********************************************************************************************************************/
void r_scifa0_callback_error(scif_error_type_t error_type)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
