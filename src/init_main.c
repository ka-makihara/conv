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
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* System Name  : RZ/T1 Init program
* File Name    : init_main.c
* Version      : 0.1
* Device       : R7S9100xx
* Abstract     : Main program for RZ/T1 Init program
* OS           : not use
* H/W Platform : Renesas Starter Kit for RZ/T1(Preliminary)
* Description  : Initialize the peripheral settings of RZ/T1
* Limitation   : none
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              :                     First Release
*******************************************************************************/

/*******************************************************************************
Includes <System Includes> , "Project Includes"
*******************************************************************************/
#include "typedefine.h"
#include "iodefine.h"
#include "r_system.h"
#include "r_icu_init.h"
#include "r_mpc.h"
#include "r_port.h"
#include "r_ecm.h"

/*******************************************************************************
Macro definitions
*******************************************************************************/

/*******************************************************************************
Typedef definitions
*******************************************************************************/

/*******************************************************************************
Imported global variables and functions (from other files)
*******************************************************************************/
extern void R_IRQ9_isr(void);

/*******************************************************************************
Exported global variables and functions (to be accessed by other files)
*******************************************************************************/

/*******************************************************************************
Private variables and functions
*******************************************************************************/
int main(void);
void port_init(void);
void ecm_init(void);
void icu_init(void);
void soft_wait(void);

/*******************************************************************************
* Outline      : main processing
* Function Name: main
* Description  : Initialize the peripheral settings of RZ/T1.
                 Main processing is toggle output of PF7 (LED0).
* Arguments    : none
* Return Value : none
*******************************************************************************/
int main (void)
{
    /* Initialize the port function */
    port_init();
    
    /* Initialize the ECM function */
    ecm_init();
    
    /* Initialize the ICU settings */
    icu_init();    

    while (1)
    {
        /* Toggle the PF7 output level(LED0) */
        PORTF.PODR.BIT.B7 ^= 1;
    
        soft_wait();  // Soft wait for blinking LED0
        
    }
   
}
/*******************************************************************************
 End of function main
*******************************************************************************/

/*******************************************************************************
* Function Name: port_init
* Description  : Initialize port setting as following.
*                 PF7 : Output and low level.
*                 P56 : Output and low level.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void port_init(void)
{
    /* Set PF7 to Output port pin (Using LED0) */
    PORTF.PDR.BIT.B7 = PORT_DIRECTION_HIZ;  // Initialize port settings
    PORTF.PMR.BIT.B7 = PORT_MODE_GENERAL;
  
    PORTF.PODR.BIT.B7 = PORT_OUTPUT_LOW;  // Set output level to Low
    PORTF.PDR.BIT.B7 = PORT_DIRECTION_OUTPUT;  // Set port direction to output
    
    
    /* Set P56 to Output port pin (Using LED1) */
    PORT5.PDR.BIT.B6 = PORT_DIRECTION_HIZ;  // Initialize port settings
    PORT5.PMR.BIT.B6 = PORT_MODE_GENERAL;
  
    PORT5.PODR.BIT.B6 = PORT_OUTPUT_LOW;  // Set output level to Low
    PORT5.PDR.BIT.B6 = PORT_DIRECTION_OUTPUT;  // Set port direction to output
}

/*******************************************************************************
 End of function port_init
*******************************************************************************/

/*******************************************************************************
* Function Name: ecm_init
* Description  : Initialize ECM setting as following.
*                  Error 35 : Permit to generate the ECM reset                                
* Arguments    : none
* Return Value : none
*******************************************************************************/
void ecm_init(void)
{
    volatile uint8_t result;
    
    /* Initialize ECM function  */
    R_ECM_Init();
    
}

/*******************************************************************************
 End of function ecm_init
*******************************************************************************/

/*******************************************************************************
* Function Name: icu_init
* Description  : Initialize Interrupt Controller Unit setting.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void icu_init(void)
{
    /*******************************************/
    /*      Set IRQ pin interrupt 5(IRQ9)      */
    /*******************************************/
    /* Initialize VIC (dummy writing to HVA0 register) */
    VIC.HVA0.LONG = 0x00000000; 
    asm("dmb");  
    
    R_ICU_Disable( ICU_VEC_NUM_9 );  // Mask IRQ9
    
    /* Set external interrupt pin: pin number, Falling edge, 
    degital noise filter(PCLKB/1) (if IRQ pin is used)  */
 
    R_ICU_ExtPinInit( ICU_EXT_PIN_5, ICU_DETECT_FALL, ICU_DNF_DIVISION_64 );
    
    /* Set multi-function pin controller setting (if IRQ pin is used) */    
    PORTN.PDR.BIT.B5 = PORT_DIRECTION_HIZ;  // Initialize port settings
    PORTN.PMR.BIT.B5 = PORT_MODE_GENERAL;
    
    PORTN.PDR.BIT.B5 = PORT_DIRECTION_INPUT; //Set PN5 to input
    
    R_MPC_WriteEnable();  // Enables writing to the PFS register 
    MPC.PN5PFS.BIT.ISEL = MPC_IRQ_ENABLE ;  // Set PN5PFS.ISEL bit to 1.
    R_MPC_WriteDisable();  // Disables writing to the PFS register
    
    /* Set VIC setting: vector number, detection type, priority level */
    /* Casting the ISR to a (uint32_t) is valid because this ISR will
    be located in 32 bit memory map address */
    R_ICU_Regist( ICU_VEC_NUM_9, ICU_TYPE_EDGE, ICU_PRIORITY_15, (uint32_t) R_IRQ9_isr );
    
    R_ICU_Enable( ICU_VEC_NUM_9 );  // Enable IRQ9
 
    /* Enable IRQ interrupt (Clear CPSR.I bit to 0) */
    asm("cpsie i");   // Clear CPSR.I bit to 0 
    asm("isb");       // Ensuring Context-changing
    
}

/*******************************************************************************
 End of function icu_init
*******************************************************************************/

/*******************************************************************************
* Function Name: soft_wait
* Description  : soft wait function using NOP 
* Arguments    : none
* Return Value : none
*******************************************************************************/
void soft_wait(void)
{
    uint32_t loop;
    
    for (loop = 0; loop < 10000000 ; loop++ )  
    {
        asm("nop");  
    }
    
}

/*******************************************************************************
 End of function soft_wait
*******************************************************************************/

/* End of File */


