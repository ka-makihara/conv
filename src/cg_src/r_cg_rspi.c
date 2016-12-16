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
* File Name    : r_cg_rspi.c
* Version      : Code Generator for RZ/T1 V1.03.00.02 [25 Aug 2016]
* Device(s)    : R7S910018CBG
* Tool-Chain   : GCCARM
* Description  : This file implements device driver for RSPI module.
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
#include "r_cg_rspi.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
const uint32_t * gp_rspi0_tx_address;         /* RSPI0 transmit buffer address */
uint16_t         g_rspi0_tx_count;            /* RSPI0 transmit data number */
uint32_t * gp_rspi0_rx_address;         /* RSPI0 receive buffer address */
uint16_t   g_rspi0_rx_count;            /* RSPI0 receive data number */
uint16_t   g_rspi0_rx_length;           /* RSPI0 receive data length */
const uint32_t * gp_rspi1_tx_address;         /* RSPI1 transmit buffer address */
uint16_t         g_rspi1_tx_count;            /* RSPI1 transmit data number */
uint32_t * gp_rspi1_rx_address;         /* RSPI1 receive buffer address */
uint16_t   g_rspi1_rx_count;            /* RSPI1 receive data number */
uint16_t   g_rspi1_rx_length;           /* RSPI1 receive data length */
const uint32_t * gp_rspi2_tx_address;         /* RSPI2 transmit buffer address */
uint16_t         g_rspi2_tx_count;            /* RSPI2 transmit data number */
uint32_t * gp_rspi2_rx_address;         /* RSPI2 receive buffer address */
uint16_t   g_rspi2_rx_count;            /* RSPI2 receive data number */
uint16_t   g_rspi2_rx_length;           /* RSPI2 receive data length */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_RSPI0_Create
* Description  : This function initializes the RSPI0 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_RSPI0_Create(void)
{
    volatile uint8_t dummy;
    
    /* Disable RSPI interrupts */
    VIC.IEC2.LONG = 0x00020000UL; /* Disable SPTI0 interrupt */
    VIC.IEC2.LONG = 0x00010000UL; /* Disable SPRI0 interrupt */
    VIC.IEC2.LONG = 0x00040000UL; /* Disable SPEI0 interrupt */
    VIC.IEC2.LONG = 0x00080000UL; /* Disable SPII0 interrupt */

    /* Set interrupt detection type */
    VIC.PLS2.LONG |= 0x00020000UL; /* Set SPTI0 edge detection interrupt */
    VIC.PLS2.LONG |= 0x00010000UL; /* Set SPRI0 edge detection interrupt */

    /* Cancel RSPI module stop state */
    MSTP(RSPI0) = 0U;

    /* Disable RSPI function */
    RSPI0.SPCR.BIT.SPE = 0U;

    /* Set control registers */
    RSPI0.SSLP.BYTE = _RSPI_SSL0_POLARITY_LOW | _RSPI_SSL1_POLARITY_LOW | _RSPI_SSL2_POLARITY_LOW | 
                      _RSPI_SSL3_POLARITY_LOW;
    RSPI0.SPPCR.BYTE = _RSPI_MOSI_FIXING_PREV_TRANSFER | _RSPI_OUTPUT_PIN_CMOS | _RSPI_LOOPBACK_DISABLED | _RSPI_LOOPBACK2_DISABLED;
    RSPI0.SPBR = _RSPI0_DIVISOR;
    RSPI0.SPDCR.BYTE = _RSPI_ACCESS_LONGWORD | _RSPI_FRAMES_1;
    RSPI0.SPCKD.BYTE = _RSPI_RSPCK_DELAY_1;
    RSPI0.SSLND.BYTE = _RSPI_SSL_NEGATION_DELAY_1;
    RSPI0.SPND.BYTE = _RSPI_NEXT_ACCESS_DELAY_1;
    RSPI0.SPCR2.BYTE = _RSPI_PARITY_DISABLE;
    RSPI0.SPSCR.BYTE = _RSPI_SEQUENCE_LENGTH_1;
    RSPI0.SPCMD0.WORD = _RSPI_RSPCK_SAMPLING_EVEN | _RSPI_RSPCK_POLARITY_LOW | _RSPI_BASE_BITRATE_8 | 
                        _RSPI_SIGNAL_ASSERT_SSL0 | _RSPI_SSL_KEEP_DISABLE | _RSPI_DATA_LENGTH_BITS_8 | 
                        _RSPI_MSB_FIRST | _RSPI_NEXT_ACCESS_DELAY_DISABLE | _RSPI_NEGATION_DELAY_DISABLE | 
                        _RSPI_RSPCK_DELAY_DISABLE;

    /* Set SPTI0 priority level */
    VIC.PRL81.LONG = _RSPI_PRIORITY_LEVEL0;
    
    /* Set SPRI0 priority level */
    VIC.PRL80.LONG = _RSPI_PRIORITY_LEVEL0;
    
    /* Set SPEI0 priority level */
    VIC.PRL82.LONG = _RSPI_PRIORITY_LEVEL0;
    
    /* Set SPII0 priority level */
    VIC.PRL83.LONG = _RSPI_PRIORITY_LEVEL0;
    
    /* Set SPTI0 interrupt address */
    VIC.VAD81.LONG = (uint32_t)r_rspi0_transmit_interrupt;
    
    /* Set SPRI0 interrupt address */
    VIC.VAD80.LONG = (uint32_t)r_rspi0_receive_interrupt;
    
    /* Set SPEI0 interrupt address */
    VIC.VAD82.LONG = (uint32_t)r_rspi0_error_interrupt;
    
    /* Set SPII0 interrupt address */
    VIC.VAD83.LONG = (uint32_t)r_rspi0_idle_interrupt;
    
    RSPI0.SPCR.BYTE = _RSPI_MODE_SPI | _RSPI_FULL_DUPLEX_SYNCHRONOUS | _RSPI_MASTER_MODE;
    
    /* Read-back SPCR */
    dummy = RSPI0.SPCR.BYTE;
}
/***********************************************************************************************************************
* Function Name: R_RSPI0_Start
* Description  : This function starts the RSPI0 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_RSPI0_Start(void)
{
    volatile uint8_t dummy;

    /* Enable RSPI interrupts */
    VIC.IEN2.LONG |= 0x00020000UL; /* Enable SPTI0 interrupt */
    VIC.IEN2.LONG |= 0x00010000UL; /* Enable SPRI0 interrupt */
    VIC.IEN2.LONG |= 0x00040000UL; /* Enable SPEI0 interrupt */
    VIC.IEN2.LONG |= 0x00080000UL; /* Enable SPII0 interrupt */

    /* Clear error sources */
    dummy = RSPI0.SPSR.BYTE;
    RSPI0.SPSR.BYTE = 0x00U;

    /* Disable idle interrupt */
    RSPI0.SPCR2.BIT.SPIIE = 0U;
}
/***********************************************************************************************************************
* Function Name: R_RSPI0_Stop
* Description  : This function stops the RSPI0 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_RSPI0_Stop(void)
{
    /* Disable RSPI interrupts */
    VIC.IEC2.LONG = 0x00020000UL; /* Disable SPTI0 interrupt */
    VIC.IEC2.LONG = 0x00010000UL; /* Disable SPRI0 interrupt */
    VIC.IEC2.LONG = 0x00040000UL; /* Disable SPEI0 interrupt */
    VIC.IEC2.LONG = 0x00080000UL; /* Disable SPII0 interrupt */

    /* Disable RSPI function */
    RSPI0.SPCR.BIT.SPE = 0U;
}
/***********************************************************************************************************************
* Function Name: R_RSPI0_Send_Receive
* Description  : This function sends and receives RSPI0 data.
* Arguments    : tx_buf -
*                    transfer buffer pointer (not used when data is handled by DMAC)
*                tx_num -
*                    buffer size
*                rx_buf -
*                    receive buffer pointer (not used when data is handled by DMAC)
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/
MD_STATUS R_RSPI0_Send_Receive(const uint32_t * tx_buf, uint16_t tx_num, uint32_t * rx_buf)
{
    MD_STATUS status = MD_OK;

    if (tx_num < 1U)
    {
        status = MD_ARGERROR;
    }
    else
    {
        /* Initialize the global counters */
        gp_rspi0_tx_address = tx_buf;
        g_rspi0_tx_count = tx_num;
        gp_rspi0_rx_address = rx_buf;
        g_rspi0_rx_length = tx_num;
        g_rspi0_rx_count = 0U;

        /* Enable transmit interrupt */
        RSPI0.SPCR.BIT.SPTIE = 1U;

        /* Enable receive interrupt */
        RSPI0.SPCR.BIT.SPRIE = 1U;

        /* Enable error interrupt */
        RSPI0.SPCR.BIT.SPEIE = 1U;

        /* Enable RSPI function */
        RSPI0.SPCR.BIT.SPE = 1U;
    }

    return (status);
}
/***********************************************************************************************************************
* Function Name: R_RSPI1_Create
* Description  : This function initializes the RSPI1 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_RSPI1_Create(void)
{
    volatile uint8_t dummy;
    
    /* Disable RSPI interrupts */
    VIC.IEC2.LONG = 0x00200000UL; /* Disable SPTI1 interrupt */
    VIC.IEC2.LONG = 0x00100000UL; /* Disable SPRI1 interrupt */
    VIC.IEC2.LONG = 0x00400000UL; /* Disable SPEI1 interrupt */
    VIC.IEC2.LONG = 0x00800000UL; /* Disable SPII1 interrupt */

    /* Set interrupt detection type */
    VIC.PLS2.LONG |= 0x00200000UL; /* Set SPTI1 edge detection interrupt */
    VIC.PLS2.LONG |= 0x00100000UL; /* Set SPRI1 edge detection interrupt */

    /* Cancel RSPI module stop state */
    MSTP(RSPI1) = 0U;

    /* Disable RSPI function */
    RSPI1.SPCR.BIT.SPE = 0U;

    /* Set control registers */
    RSPI1.SSLP.BYTE = _RSPI_SSL0_POLARITY_LOW | _RSPI_SSL1_POLARITY_LOW;
    RSPI1.SPPCR.BYTE = _RSPI_MOSI_LEVEL_LOW | _RSPI_MOSI_FIXING_MOIFV_BIT | _RSPI_OUTPUT_PIN_CMOS | _RSPI_LOOPBACK_DISABLED | _RSPI_LOOPBACK2_DISABLED;
    RSPI1.SPBR = _RSPI1_DIVISOR;
    RSPI1.SPDCR.BYTE = _RSPI_ACCESS_LONGWORD | _RSPI_FRAMES_1;
    RSPI1.SPCKD.BYTE = _RSPI_RSPCK_DELAY_1;
    RSPI1.SSLND.BYTE = _RSPI_SSL_NEGATION_DELAY_1;
    RSPI1.SPND.BYTE = _RSPI_NEXT_ACCESS_DELAY_1;
    RSPI1.SPCR2.BYTE = _RSPI_PARITY_DISABLE;
    RSPI1.SPSCR.BYTE = _RSPI_SEQUENCE_LENGTH_1;
    RSPI1.SPCMD0.WORD = _RSPI_RSPCK_SAMPLING_ODD | _RSPI_RSPCK_POLARITY_HIGH | _RSPI_BASE_BITRATE_8 | 
                        _RSPI_SIGNAL_ASSERT_SSL0 | _RSPI_SSL_KEEP_DISABLE | _RSPI_DATA_LENGTH_BITS_8 | 
                        _RSPI_MSB_FIRST | _RSPI_NEXT_ACCESS_DELAY_DISABLE | _RSPI_NEGATION_DELAY_DISABLE | 
                        _RSPI_RSPCK_DELAY_DISABLE;

    /* Set SPTI1 priority level */
    VIC.PRL85.LONG = _RSPI_PRIORITY_LEVEL6;
    
    /* Set SPRI1 priority level */
    VIC.PRL84.LONG = _RSPI_PRIORITY_LEVEL6;
    
    /* Set SPEI1 priority level */
    VIC.PRL86.LONG = _RSPI_PRIORITY_LEVEL5;
    
    /* Set SPII1 priority level */
    VIC.PRL87.LONG = _RSPI_PRIORITY_LEVEL7;
    
    /* Set SPTI1 interrupt address */
    VIC.VAD85.LONG = (uint32_t)r_rspi1_transmit_interrupt;
    
    /* Set SPRI1 interrupt address */
    VIC.VAD84.LONG = (uint32_t)r_rspi1_receive_interrupt;
    
    /* Set SPEI1 interrupt address */
    VIC.VAD86.LONG = (uint32_t)r_rspi1_error_interrupt;
    
    /* Set SPII1 interrupt address */
    VIC.VAD87.LONG = (uint32_t)r_rspi1_idle_interrupt;
    
    RSPI1.SPCR.BYTE = _RSPI_MODE_SPI | _RSPI_FULL_DUPLEX_SYNCHRONOUS | _RSPI_MASTER_MODE;
    
    /* Read-back SPCR */
    dummy = RSPI1.SPCR.BYTE;
}
/***********************************************************************************************************************
* Function Name: R_RSPI1_Start
* Description  : This function starts the RSPI1 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_RSPI1_Start(void)
{
    volatile uint8_t dummy;

    /* Enable RSPI interrupts */
    VIC.IEN2.LONG |= 0x00200000UL; /* Enable SPTI1 interrupt */
    VIC.IEN2.LONG |= 0x00100000UL; /* Enable SPRI1 interrupt */
    VIC.IEN2.LONG |= 0x00400000UL; /* Enable SPEI1 interrupt */
    VIC.IEN2.LONG |= 0x00800000UL; /* Enable SPII1 interrupt */

    /* Clear error sources */
    dummy = RSPI1.SPSR.BYTE;
    RSPI1.SPSR.BYTE = 0x00U;

    /* Disable idle interrupt */
    RSPI1.SPCR2.BIT.SPIIE = 0U;
}
/***********************************************************************************************************************
* Function Name: R_RSPI1_Stop
* Description  : This function stops the RSPI1 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_RSPI1_Stop(void)
{
    /* Disable RSPI interrupts */
    VIC.IEC2.LONG = 0x00200000UL; /* Disable SPTI1 interrupt */
    VIC.IEC2.LONG = 0x00100000UL; /* Disable SPRI1 interrupt */
    VIC.IEC2.LONG = 0x00400000UL; /* Disable SPEI1 interrupt */
    VIC.IEC2.LONG = 0x00800000UL; /* Disable SPII1 interrupt */

    /* Disable RSPI function */
    RSPI1.SPCR.BIT.SPE = 0U;
}
/***********************************************************************************************************************
* Function Name: R_RSPI1_Send_Receive
* Description  : This function sends and receives RSPI1 data.
* Arguments    : tx_buf -
*                    transfer buffer pointer (not used when data is handled by DMAC)
*                tx_num -
*                    buffer size
*                rx_buf -
*                    receive buffer pointer (not used when data is handled by DMAC)
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/
MD_STATUS R_RSPI1_Send_Receive(const uint32_t * tx_buf, uint16_t tx_num, uint32_t * rx_buf)
{
    MD_STATUS status = MD_OK;

    if (tx_num < 1U)
    {
        status = MD_ARGERROR;
    }
    else
    {
        /* Initialize the global counters */
        gp_rspi1_tx_address = tx_buf;
        g_rspi1_tx_count = tx_num;
        gp_rspi1_rx_address = rx_buf;
        g_rspi1_rx_length = tx_num;
        g_rspi1_rx_count = 0U;

        /* Enable transmit interrupt */
        RSPI1.SPCR.BIT.SPTIE = 1U;

        /* Enable receive interrupt */
        RSPI1.SPCR.BIT.SPRIE = 1U;

        /* Enable error interrupt */
        RSPI1.SPCR.BIT.SPEIE = 1U;

        /* Enable RSPI function */
        RSPI1.SPCR.BIT.SPE = 1U;
    }

    return (status);
}
/***********************************************************************************************************************
* Function Name: R_RSPI2_Create
* Description  : This function initializes the RSPI2 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_RSPI2_Create(void)
{
    volatile uint8_t dummy;
    
    /* Disable RSPI interrupts */
    VIC.IEC2.LONG = 0x02000000UL; /* Disable SPTI2 interrupt */
    VIC.IEC2.LONG = 0x01000000UL; /* Disable SPRI2 interrupt */
    VIC.IEC2.LONG = 0x04000000UL; /* Disable SPEI2 interrupt */
    VIC.IEC2.LONG = 0x08000000UL; /* Disable SPII2 interrupt */

    /* Set interrupt detection type */
    VIC.PLS2.LONG |= 0x02000000UL; /* Set SPTI2 edge detection interrupt */
    VIC.PLS2.LONG |= 0x01000000UL; /* Set SPRI2 edge detection interrupt */

    /* Cancel RSPI module stop state */
    MSTP(RSPI2) = 0U;

    /* Disable RSPI function */
    RSPI2.SPCR.BIT.SPE = 0U;

    /* Set control registers */
    RSPI2.SSLP.BYTE = _RSPI_SSL0_POLARITY_LOW;
    RSPI2.SPPCR.BYTE = _RSPI_MOSI_LEVEL_HIGH | _RSPI_MOSI_FIXING_MOIFV_BIT | _RSPI_OUTPUT_PIN_CMOS | _RSPI_LOOPBACK_DISABLED | _RSPI_LOOPBACK2_DISABLED;
    RSPI2.SPBR = _RSPI2_DIVISOR;
    RSPI2.SPDCR.BYTE = _RSPI_ACCESS_LONGWORD | _RSPI_FRAMES_1;
    RSPI2.SPCKD.BYTE = _RSPI_RSPCK_DELAY_1;
    RSPI2.SSLND.BYTE = _RSPI_SSL_NEGATION_DELAY_1;
    RSPI2.SPND.BYTE = _RSPI_NEXT_ACCESS_DELAY_1;
    RSPI2.SPCR2.BYTE = _RSPI_PARITY_DISABLE;
    RSPI2.SPSCR.BYTE = _RSPI_SEQUENCE_LENGTH_1;
    RSPI2.SPCMD0.WORD = _RSPI_RSPCK_SAMPLING_ODD | _RSPI_RSPCK_POLARITY_HIGH | _RSPI_BASE_BITRATE_8 | 
                        _RSPI_SIGNAL_ASSERT_SSL0 | _RSPI_SSL_KEEP_DISABLE | _RSPI_DATA_LENGTH_BITS_8 | 
                        _RSPI_MSB_FIRST | _RSPI_NEXT_ACCESS_DELAY_DISABLE | _RSPI_NEGATION_DELAY_DISABLE | 
                        _RSPI_RSPCK_DELAY_DISABLE;

    /* Set SPTI2 priority level */
    VIC.PRL89.LONG = _RSPI_PRIORITY_LEVEL6;
    
    /* Set SPRI2 priority level */
    VIC.PRL88.LONG = _RSPI_PRIORITY_LEVEL6;
    
    /* Set SPEI2 priority level */
    VIC.PRL90.LONG = _RSPI_PRIORITY_LEVEL5;
    
    /* Set SPII2 priority level */
    VIC.PRL91.LONG = _RSPI_PRIORITY_LEVEL7;
    
    /* Set SPTI2 interrupt address */
    VIC.VAD89.LONG = (uint32_t)r_rspi2_transmit_interrupt;
    
    /* Set SPRI2 interrupt address */
    VIC.VAD88.LONG = (uint32_t)r_rspi2_receive_interrupt;
    
    /* Set SPEI2 interrupt address */
    VIC.VAD90.LONG = (uint32_t)r_rspi2_error_interrupt;
    
    /* Set SPII2 interrupt address */
    VIC.VAD91.LONG = (uint32_t)r_rspi2_idle_interrupt;
    
    RSPI2.SPCR.BYTE = _RSPI_MODE_SPI | _RSPI_FULL_DUPLEX_SYNCHRONOUS | _RSPI_MASTER_MODE;
    
    /* Read-back SPCR */
    dummy = RSPI2.SPCR.BYTE;
}
/***********************************************************************************************************************
* Function Name: R_RSPI2_Start
* Description  : This function starts the RSPI2 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_RSPI2_Start(void)
{
    volatile uint8_t dummy;

    /* Enable RSPI interrupts */
    VIC.IEN2.LONG |= 0x02000000UL; /* Enable SPTI2 interrupt */
    VIC.IEN2.LONG |= 0x01000000UL; /* Enable SPRI2 interrupt */
    VIC.IEN2.LONG |= 0x04000000UL; /* Enable SPEI2 interrupt */
    VIC.IEN2.LONG |= 0x08000000UL; /* Enable SPII2 interrupt */

    /* Clear error sources */
    dummy = RSPI2.SPSR.BYTE;
    RSPI2.SPSR.BYTE = 0x00U;

    /* Disable idle interrupt */
    RSPI2.SPCR2.BIT.SPIIE = 0U;
}
/***********************************************************************************************************************
* Function Name: R_RSPI2_Stop
* Description  : This function stops the RSPI2 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_RSPI2_Stop(void)
{
    /* Disable RSPI interrupts */
    VIC.IEC2.LONG = 0x02000000UL; /* Disable SPTI2 interrupt */
    VIC.IEC2.LONG = 0x01000000UL; /* Disable SPRI2 interrupt */
    VIC.IEC2.LONG = 0x04000000UL; /* Disable SPEI2 interrupt */
    VIC.IEC2.LONG = 0x08000000UL; /* Disable SPII2 interrupt */

    /* Disable RSPI function */
    RSPI2.SPCR.BIT.SPE = 0U;
}
/***********************************************************************************************************************
* Function Name: R_RSPI2_Send_Receive
* Description  : This function sends and receives RSPI2 data.
* Arguments    : tx_buf -
*                    transfer buffer pointer (not used when data is handled by DMAC)
*                tx_num -
*                    buffer size
*                rx_buf -
*                    receive buffer pointer (not used when data is handled by DMAC)
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/
MD_STATUS R_RSPI2_Send_Receive(const uint32_t * tx_buf, uint16_t tx_num, uint32_t * rx_buf)
{
    MD_STATUS status = MD_OK;

    if (tx_num < 1U)
    {
        status = MD_ARGERROR;
    }
    else
    {
        /* Initialize the global counters */
        gp_rspi2_tx_address = tx_buf;
        g_rspi2_tx_count = tx_num;
        gp_rspi2_rx_address = rx_buf;
        g_rspi2_rx_length = tx_num;
        g_rspi2_rx_count = 0U;

        /* Enable transmit interrupt */
        RSPI2.SPCR.BIT.SPTIE = 1U;

        /* Enable receive interrupt */
        RSPI2.SPCR.BIT.SPRIE = 1U;

        /* Enable error interrupt */
        RSPI2.SPCR.BIT.SPEIE = 1U;

        /* Enable RSPI function */
        RSPI2.SPCR.BIT.SPE = 1U;
    }

    return (status);
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
