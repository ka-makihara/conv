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
* File Name    : r_cg_scifa.c
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
const uint8_t * gp_scifa0_tx_address;   /* SCIFA0 transmit buffer address */
uint16_t        g_scifa0_tx_count;      /* SCIFA0 transmit data number */
uint8_t *       gp_scifa0_rx_address;   /* SCIFA0 receive buffer address */
uint16_t        g_scifa0_rx_count;      /* SCIFA0 receive data number */
uint16_t        g_scifa0_rx_length;     /* SCIFA0 receive data length */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_SCIFA0_Create
* Description  : This function initializes SCIFA0.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_SCIFA0_Create(void)
{
    volatile uint16_t dummy;
    uint32_t w_count;

    /* Cancel SCIFA0 module stop state */
    MSTP(SCIFA0) = 0U;

    /* Disable TXIF0 interrupt */
    VIC.IEC3.LONG = 0x00000004UL;

    /* Disable RXIF0 interrupt */
    VIC.IEC3.LONG = 0x00000002UL;

    /* Disable BRIF0 interrupt */
    VIC.IEC3.LONG = 0x00000001UL;

    /* Disable DRIF0 interrupt */
    VIC.IEC3.LONG = 0x00000008UL;

    /* Clear transmit/receive enable bits */
    SCIFA0.SCR.BIT.TE = 0U;
    SCIFA0.SCR.BIT.RE = 0U;

    /* Reset transmit/receive FIFO data register operation */
    SCIFA0.FCR.BIT.TFRST = 1U;
    SCIFA0.FCR.BIT.RFRST = 1U;

    /* Read and clear status flags */
    dummy = SCIFA0.FSR.WORD;
    SCIFA0.FSR.WORD = 0x00U;
    dummy = (uint16_t) SCIFA0.LSR.BIT.ORER;
    SCIFA0.LSR.BIT.ORER = 0U;

    /* Set clock enable bits */
    SCIFA0.SCR.WORD = _SCIF_INTERNAL_SCK_UNUSED;

    /* Set transmission/reception format */
    SCIFA0.SMR.WORD = _SCIF_CLOCK_SERICLK | _SCIF_STOP_1 | _SCIF_PARITY_DISABLE | _SCIF_DATA_LENGTH_8 | 
                      _SCIF_ASYNCHRONOUS_MODE;
    SCIFA0.SEMR.BYTE = _SCIF_8_BASE_CLOCK | _SCIF_NOISE_FILTER_DISABLE | _SCIF_DATA_TRANSFER_LSB_FIRST | 
                       _SCIF_BAUDRATE_SINGLE;

    /* Clear modulation duty register select */
    SCIFA0.SEMR.BIT.MDDRS = 0U;

    /* Set bit rate */
    SCIFA0.BRR_MDDR.BRR = 0x26U;

    /* Wait for at least 1-bit interval */
    for (w_count = 0U; w_count < _SCIF_1BIT_INTERVAL_0; w_count++)
    {
        nop();
    }

    /* Set FIFO trigger conditions */
    SCIFA0.FTCR.WORD = _SCIF_TX_FIFO_TRIGGER_NUM_0 | _SCIF_TX_TRIGGER_TFTC_VALID | _SCIF_RX_FIFO_TRIGGER_NUM_1 | 
                       _SCIF_RX_TRIGGER_RFTC_VALID;
    SCIFA0.FCR.WORD = _SCIF_LOOPBACK_DISABLE | _SCIF_MODEM_CONTROL_DISABLE;

    /* Disable transmit/receive FIFO data register reset operation */
    SCIFA0.FCR.BIT.TFRST = 0U;
    SCIFA0.FCR.BIT.RFRST = 0U;

    /* Set TXIF0 interrupt priority */
    VIC.PRL98.LONG = _SCIF_PRIORITY_LEVEL0;

    /* Set TXIF0 interrupt address */
    VIC.VAD98.LONG = (uint32_t)r_scifa0_txif0_interrupt;

    /* Set RXIF0 interrupt priority */
    VIC.PRL97.LONG = _SCIF_PRIORITY_LEVEL0;

    /* Set RXIF0 interrupt address */
    VIC.VAD97.LONG = (uint32_t)r_scifa0_rxif0_interrupt;

    /* Set BRIF0 interrupt priority */
    VIC.PRL96.LONG = _SCIF_PRIORITY_LEVEL0;

    /* Set BRIF0 interrupt address */
    VIC.VAD96.LONG = (uint32_t)r_scifa0_brif0_interrupt;

    /* Set DRIF0 interrupt priority */
    VIC.PRL99.LONG = _SCIF_PRIORITY_LEVEL0;

    /* Set DRIF0 interrupt address */
    VIC.VAD99.LONG = (uint32_t)r_scifa0_drif0_interrupt;
}
/***********************************************************************************************************************
* Function Name: R_SCIFA0_Start
* Description  : This function starts SCIFA0.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_SCIFA0_Start(void)
{
    /* Enable TXIF0 interrupt */
    VIC.IEN3.LONG |= 0x00000004UL;

    /* Enable RXIF0 interrupt */
    VIC.IEN3.LONG |= 0x00000002UL;

    /* Enable BRIF0 interrupt */
    VIC.IEN3.LONG |= 0x00000001UL;

    /* Enable DRIF0 interrupt */
    VIC.IEN3.LONG |= 0x00000008UL;
}
/***********************************************************************************************************************
* Function Name: R_SCIFA0_Stop
* Description  : This function stops SCIFA0.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_SCIFA0_Stop(void)
{
    /* Disable serial transmit */
    SCIFA0.SCR.BIT.TE = 0U;

    /* Disable serial receive */
    SCIFA0.SCR.BIT.RE = 0U;

    /* Disable TXI interrupt */
    SCIFA0.SCR.BIT.TIE = 0U;

    /* Disable RXI and ERI interrupt */
    SCIFA0.SCR.BIT.RIE = 0U;

    /* Disable TXIF0 interrupt */
    VIC.IEC3.LONG = 0x00000004UL;

    /* Disable RXIF0 interrupt */
    VIC.IEC3.LONG = 0x00000002UL;

    /* Disable BRIF0 interrupt */
    VIC.IEC3.LONG = 0x00000001UL;

    /* Disable DRIF0 interrupt */
    VIC.IEC3.LONG = 0x00000008UL;
}
/***********************************************************************************************************************
* Function Name: R_SCIFA0_Serial_Receive
* Description  : This function receives SCIFA0 data.
* Arguments    : rx_buf -
*                    receive buffer pointer (Not used when receive data handled by DMAC)
*                rx_num -
*                    buffer size (Not used when receive data handled by DMAC)
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/
MD_STATUS R_SCIFA0_Serial_Receive(uint8_t * rx_buf, uint16_t rx_num)
{
    MD_STATUS status = MD_OK;

    if (rx_num < 1U)
    {
        status = MD_ARGERROR;
    }
    else
    {
        g_scifa0_rx_count = 0U;
        g_scifa0_rx_length = rx_num;
        gp_scifa0_rx_address = rx_buf;

        SCIFA0.FTCR.BIT.RFTC = _SCIF_RX_TRIG_NUM_0;

        SCIFA0.SCR.BIT.RE = 1U;
        SCIFA0.SCR.BIT.RIE = 1U;
        SCIFA0.SCR.BIT.REIE = 1U;
    }

    return (status);
}
/***********************************************************************************************************************
* Function Name: R_SCIFA0_Serial_Send
* Description  : This function transmits SCIFA0 data.
* Arguments    : tx_buf -
*                    transfer buffer pointer (Not used when transmit data handled by DMAC)
*                tx_num -
*                    buffer size (Not used when transmit data handled by DMAC)
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/
MD_STATUS R_SCIFA0_Serial_Send(const uint8_t * tx_buf, uint16_t tx_num)
{
    MD_STATUS status = MD_OK;

    if (tx_num < 1U)
    {
        status = MD_ARGERROR;
    }
    else
    {
        gp_scifa0_tx_address = tx_buf;
        g_scifa0_tx_count = tx_num;
        SCIFA0.SCR.BIT.TE = 1U;
        SCIFA0.SCR.BIT.TIE = 1U;
    }

    return (status);
}

/* Start user code for adding. Do not edit comment generated here */

/*******************************************************************************
* Function Name: IoGetchar
* Description  : One character is received from SCIFA2, and it's data is returned.
*              : This function keeps waiting until it can obtain the receiving data.
* Arguments    : none
* Return Value : Character to receive (Byte).
*******************************************************************************/
uint8_t IoGetchar (void)
{
    int32_t    data;
    uint8_t    data_char;
    int32_t    ans;

    //ans = R_SCIFA2_Serial_Receive(&data_char,1);

    return data_char;
}

/*******************************************************************************
* Function Name: IoPutchar
* Description  : Character "buffer" is output to SCIFA2.
*              : This function keeps waiting until it becomes the transmission
*              : enabled state.
* Arguments    : int_t buffer : character to output
* Return Value : None
*******************************************************************************/
void IoPutchar (uint8_t *buffer)
{
	//R_SCIFA2_Serial_Send(buffer,1);
}


void scif_loop(void)
{
    uint8_t ans;

    ans = IoGetchar();
    if(ans >= 0){
        IoPutchar(ans);
    }
}
/* End user code. Do not edit comment generated here */
