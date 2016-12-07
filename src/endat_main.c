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
* System Name  : EnDat sample program
* File Name    : main.c
* Version      : $Rev: 254 $
* Device       : RZ
* Abstract     : Main processing source file for EnDat.
* Tool-Chain   : IAR Embedded Workbench for ARM
*                ARM DS-5 Development Studio
*                Renesas e2studio
* OS           : Not use
* H/W Platform : RZ/T1
* Description  : EnDat sample program on RZ devices.
* Limitation   : None
*******************************************************************************/
/*******************************************************************************
* History      : History is managed by Revision Control System.
*******************************************************************************/
/*******************************************************************************
Includes <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __ICCARM__
#include <stdint.h>
#include <stdbool.h>
#include <intrinsics.h>
#endif
#ifdef __CC_ARM
#include <stdint.h>
#include <stdbool.h>
#else
#ifdef __GNUC__
#include "platform.h"
#endif
#endif
#include "iodefine.h"
#include "iodefine_endat.h"
#include "r_mpc.h"
#include "r_reset.h"
#include "r_mpc.h"
#include "r_port.h"
#include "r_system.h"
#include "r_icu_init.h"
#include "r_ecl_rzt1_if.h"
#include "r_endat_rzt1_dat.h"
#include "r_endat_rzt1_if.h"

/*******************************************************************************
Macro definitions
*******************************************************************************/
//#define DEBUG_PRINT(...)            printf(__VA_ARGS__)
#define	DEBUG_PRINT(...)
#define DEBUG_PRINT_END             "\n"        /* Line feed code */

#define ARG_MAX                     (4)         /* maximum number of comannd arguments */
#define CMD_BUF_SIZE                (256)       /* command buffer size */
#define CMD_NUM                     (5)         /* number of commands */
#define EXIT_CMD_NUM                (4)         /* exit command number */
#define CMD_DELIMITER               (" \t\r\n") /* command line delimiter */
#define CMD_EXIT_ARG_NUM            (1)         /* "exit" command paramter */
#define PSEL_VALUE                  (0x2B)      /* Value of MPC.PxxPFS.PSEL */

#define ENDAT_ENC_TSAT_WAIT         (1300u)     /* Waiting time after the power on reset */
#define ENDAT_ENC_1MS_WAIT          (1000u)     /* Wait 1ms */
#define ENDAT_ENC_INIT_RESET_WAIT   (60u)       /* Wait 51ms */
#define ENDAT_ENC_INIT_MEM_WAIT     (743u)      /* Wait 743us */
#define ENDAT_ENC_INIT_PRAM_WAIT    (13u)       /* Wait 13ms */
#define ENDAT_ENC_INIT_CABLE_WAIT   (588u)      /* Wait 588us */
#define ENDAT_WDG_MAX               (127u)      /* Watchdog timer settings 25.4ms */ 
#define ENDAT_POS_NUM               (255u)      /* Store the number of position value */
#define ENDAT_CMT1_CKS              (0u)        /* Set the clock input to CMT1 to PCLKD / 8 */
#define ENDAT_TIME_PLACE            (10u)       /* Adjust the unit of time */
#define ENDAT_TIME_RESOLUTION       (1u)        /* The smallest unit of count time by CMT1 */
#define ENDAT_TIME_START            (1u)        /* Starts the timer */
#define ENDAT_TEMP_SCA_FAC          (0.1)       /* Scaling function for temperature */
#define ENDAT_TEMP_ABS_ZERO         (273.2)     /* Absolute zero degrees Celsius */
#define ENDAT_SHIFT_32              (32)
#define ENDAT_MASK_LOW_32           (0xFFFFFFFFu)

/*******************************************************************************
Typedef definitions
*******************************************************************************/
typedef char my_char_t;
typedef void (*cmd_func_t)(void);

/*******************************************************************************
Imported global variables and functions (from other files)
*******************************************************************************/
extern uint8_t g_endat_config[];
extern void IoInitScifa2(void);

/*******************************************************************************
Exported global variables and functions (to be accessed by other files)
*******************************************************************************/

/*******************************************************************************
Private variables and functions
*******************************************************************************/
/* EnDat sample program */
static void endat_cmd_control(void);
static void endat_power_on_wait(void);
static void enc_init_reset_wait_callback(void);
static void enc_init_mem_wait_callback(void);
static void enc_init_pram_wait_callback(void);
static void enc_init_cable_wait_callback(void);
static void endat_pos(void);
static void endat_poscon(void);
static void endat_stop(void);
static void endat_temp(void);
static void endat_callback(r_endat_result_t * presult, r_endat_protocol_err_t *perr); 
static void endat_poscon_callback(r_endat_result_t * presult, r_endat_protocol_err_t *perr);
static void endat_rdst_callback(void); 
static void result_display(r_endat_result_t *presult, r_endat_protocol_err_t *perr);
static void init_pin_hiz_conf_endat0(void);
static void init_pin_conf_endat0(void);

/* Set the timer */
static void timer_init(void);
static void timer_wait(uint32_t time);

/* Terminal program */
static uint32_t get_cmd(my_char_t *parg[], const uint32_t arg_max);
static void cmd_exit(void);

static const my_char_t *pcmd_tbl[CMD_NUM] =
{
    "pos", "poscon", "stop", "temp", "exit"
};
static const cmd_func_t cmd_func_tbl[CMD_NUM] =
{
    &endat_pos, &endat_poscon, &endat_stop, &endat_temp, &cmd_exit,
};

volatile static bool endat_flg;
static r_endat_result_t *pendat_result;
static r_endat_protocol_err_t *pendat_err;
static r_endat_req_err_t poscon_err[ENDAT_POS_NUM];
static uint64_t poscon[ENDAT_POS_NUM];
static uint8_t poscon_valid;
static uint8_t poscon_num;
static bool poscon_empty;

/*******************************************************************************
* Function Name: main
* Description  : Main processing of EnDat sample program
* Arguments    : None
* Return Value : Error code of Encoder IF
*******************************************************************************/
int32_t endat_main(void)
{ 
    int32_t ret_code = 0;
    int32_t err_ecl_code;
    
    /* Initialize SCIFA 2 */
    //IoInitScifa2();
    
    DEBUG_PRINT("EnDat sample program start"DEBUG_PRINT_END);
    
    /* Initialization of CMT1 */
    timer_init();
    
    /* Waiting time after power-on of the encoder */
    endat_power_on_wait();
    
    /* Set pin port mode(Hi-Z) */
    init_pin_hiz_conf_endat0();
    
    /* Configuration of Encoder I/F EnDat */
    err_ecl_code = R_ECL_Configure(g_endat_config);
    if (R_ECL_SUCCESS != err_ecl_code)
    {
        DEBUG_PRINT("R_ECL_Configure: error(%d)" DEBUG_PRINT_END, (int)err_ecl_code);
        ret_code = (int32_t)err_ecl_code;
        goto end;
    }
    
    /* Set pin peripheral mode */
    init_pin_conf_endat0();
    
    /* Start of Encoder I/F EnDat */
    err_ecl_code = R_ECL_Start(R_ENDAT0_ID, R_ENDAT_FREQ);
    if (R_ECL_SUCCESS != err_ecl_code)
    {
        DEBUG_PRINT("R_ECL_Start: error(%d)" DEBUG_PRINT_END, (int)err_ecl_code);
        ret_code = (int32_t)err_ecl_code;
        goto end;
    }

    /* Enable IRQ interrupt */
#ifdef __ICCARM__
    __enable_irq();
    __ISB();
#endif
#ifdef __CC_ARM
    __asm("cpsie i");
    __asm("isb");
#else
#ifdef __GNUC__
    asm("cpsie i");
    asm("isb");
#endif
#endif

    /* Control of EnDat */
    endat_cmd_control();
/*
    err_ecl_code = R_ECL_Stop(R_ENDAT0_ID);
    if (R_ECL_SUCCESS != err_ecl_code)
    {
        DEBUG_PRINT("R_ECL_Stop: error(%d)" DEBUG_PRINT_END, (int)err_ecl_code);
        ret_code = (int32_t)err_ecl_code;
        goto end;
    }
*/
end:
    DEBUG_PRINT("EnDat sample program end"DEBUG_PRINT_END);
    return ret_code;
}
/*******************************************************************************
End of function main
*******************************************************************************/

/*******************************************************************************
* Function Name: endat_cmd_control
* Description  : Control function of EnDat.
* Arguments    : None
* Return Value : Error code of Encoder IF
*******************************************************************************/
#if 1
static void endat_cmd_control(void)
{ 
    r_endat_err_t err_endat_code;
    r_endat_info_t endat_info;
    uint32_t arg_num;
    int32_t cmd_index = 0;
    my_char_t *parg[ARG_MAX];
    
    endat_info.ftclk                = R_ENDAT_FTCLK_8330;
    endat_info.filter               = true;
    endat_info.delay_comp           = true;
    endat_info.tst                  = R_ENDAT_TST_10US;
    endat_info.penc_init_reset_wait = &enc_init_reset_wait_callback;
    endat_info.penc_init_mem_wait   = &enc_init_mem_wait_callback;
    endat_info.penc_init_pram_wait  = &enc_init_pram_wait_callback;
    endat_info.penc_init_cable_wait = &enc_init_cable_wait_callback;
    err_endat_code = R_ENDAT_Open(R_ENDAT0_ID, &endat_info);
    if (ENDAT_SUCCESS != err_endat_code)
    {
        DEBUG_PRINT("R_ENDAT_Open: error(%d)" DEBUG_PRINT_END, (int)err_endat_code); 
        goto endat_stop;
    }
    endat_pos();
    /*
    do
    {
        // Get command token from the console I/O.
        DEBUG_PRINT("endat >");
        arg_num = get_cmd(&parg[0], ARG_MAX);
        if (0 < arg_num)
        {
            // Search for the command name.
            for (cmd_index = 0; cmd_index <= CMD_NUM; cmd_index++)
            {
                if (strncmp(parg[0], pcmd_tbl[cmd_index], strlen(pcmd_tbl[cmd_index]) + CMD_EXIT_ARG_NUM) == 0)
                {
                    break;
                }
            }
            
            if (CMD_NUM > cmd_index)
            {
                // Call the function corresponding to the command.
                cmd_func_tbl[cmd_index]();
            }
            else
            {
                DEBUG_PRINT("unknown command" DEBUG_PRINT_END);
            }
        }
    } while ((EXIT_CMD_NUM != cmd_index) || (CMD_EXIT_ARG_NUM != arg_num)); 

    err_endat_code = R_ENDAT_Close(R_ENDAT0_ID);
    if (R_ECL_SUCCESS != err_endat_code)
    {
        DEBUG_PRINT("R_ENDAT_Close: error(%d)" DEBUG_PRINT_END, (int)err_endat_code);
    }
    
*/
endat_stop:
    return;
}
#endif
/*******************************************************************************
End of function endat_cmd_control
*******************************************************************************/

/*******************************************************************************
* Function Name: endat_power_on_wait
* Description  : Generate wait time after encoder power is turned on.
* Arguments    : None.
* Return Value : None.
*******************************************************************************/
static void endat_power_on_wait(void)
{
    uint32_t loop;
    
    /* 1.3s = 1ms * 1300 */
    for(loop = 0u; loop < ENDAT_ENC_TSAT_WAIT; loop++)
    {
        timer_wait(ENDAT_ENC_1MS_WAIT);
    }
    
    return;
}
/*******************************************************************************
End of function endat_power_on_wait
*******************************************************************************/

/*******************************************************************************
* Function Name: enc_init_reset_wait_callback
* Description  : Encoder reset wait time.
* Arguments    : None.
* Return Value : None.
*******************************************************************************/
static void enc_init_reset_wait_callback(void)
{
    uint8_t loop;
    
    /* 60ms = 1ms * 60 */
    for (loop = 0u; loop < ENDAT_ENC_INIT_RESET_WAIT; loop++)
    {
        timer_wait(ENDAT_ENC_1MS_WAIT); 
    }
    
    return;
}
/*******************************************************************************
End of function enc_init_reset_wait_callback
*******************************************************************************/

/*******************************************************************************
* Function Name: enc_init_mem_wait_callback
* Description  : Memory selection wait time of encoder.
* Arguments    : None.
* Return Value : None.
*******************************************************************************/
static void enc_init_mem_wait_callback(void)
{
    timer_wait(ENDAT_ENC_INIT_MEM_WAIT); /* 743us */
    
    return;
}
/*******************************************************************************
End of function enc_init_mem_wait_callback
*******************************************************************************/

/*******************************************************************************
* Function Name: enc_init_pram_wait_callback
* Description  : Parameters receive wait time of encoder.
* Arguments    : None.
* Return Value : None.
*******************************************************************************/
static void enc_init_pram_wait_callback(void)
{
    uint8_t loop;
    
    /* 13ms = 1ms * 13 */
    for (loop = 0u; loop < ENDAT_ENC_INIT_PRAM_WAIT; loop++)
    {
        timer_wait(ENDAT_ENC_1MS_WAIT); 
    }
    
    return;
}
/*******************************************************************************
End of function enc_init_pram_wait_callback
*******************************************************************************/

/*******************************************************************************
* Function Name: enc_init_cable_wait_callback
* Description  : Waiting time of cable propagation delay measurement.
* Arguments    : None.
* Return Value : None.
*******************************************************************************/
static void enc_init_cable_wait_callback(void)
{
    timer_wait(ENDAT_ENC_INIT_CABLE_WAIT); /* 588us */
    
    return;
}
/*******************************************************************************
End of function enc_init_cable_wait_callback
*******************************************************************************/

/*******************************************************************************
* Function Name: endat_pos
* Description  : "pos" command processing.
* Arguments    : None.
* Return Value : None.
*******************************************************************************/
static void endat_pos(void)
{
    r_endat_req_t endat_req;
    r_endat_err_t err_code;
    
    endat_flg = false;
    
    DEBUG_PRINT("pos command" DEBUG_PRINT_END);

    endat_req.mode_cmd          = R_ENDAT_POS;
    endat_req.dt                = false;
    endat_req.watchdog.range    = R_ENDAT_WD_RANGE_US;
    endat_req.watchdog.time     = ENDAT_WDG_MAX;
    endat_req.pisr_result       = &endat_callback;
    endat_req.pisr_rdst         = &endat_rdst_callback;
    err_code = R_ENDAT_Control(R_ENDAT0_ID, ENDAT_CMD_REQ, &endat_req);
    
    if (ENDAT_SUCCESS == err_code)
    {
        while (false == endat_flg)
        {
            ; /* DO NOTHING */
        }
        
        result_display(pendat_result, pendat_err);
    }
    else
    {
        DEBUG_PRINT("  R_ENDAT_Control(ENDAT_CMD_REQ) error: %d"DEBUG_PRINT_END, (int)err_code);
    }
    
    return;
}
/*******************************************************************************
End of function endat_pos
*******************************************************************************/

/*******************************************************************************
* Function Name: endat_poscon
* Description  : "poscon" command processing.
* Arguments    : None.
* Return Value : None.
*******************************************************************************/
static void endat_poscon(void)
{
    r_endat_req_t endat_req;
    r_endat_err_t err_code;
    uint32_t loop;
    
    endat_flg = false;
    poscon_num = 0u;
    poscon_valid = 0u;
    poscon_empty = true;

    for (loop = 0; loop < ENDAT_POS_NUM; loop++)
    {
        poscon_err[poscon_num] = ENDAT_REQ_SUCCESS;
        poscon[poscon_num] =  0u;
    }
    
    DEBUG_PRINT("poscon command" DEBUG_PRINT_END);

    endat_req.mode_cmd          = R_ENDAT_POS;
    endat_req.dt                = true;
    endat_req.watchdog.range    = R_ENDAT_WD_RANGE_US;
    endat_req.watchdog.time     = 0u;
    endat_req.pisr_result       = &endat_poscon_callback;
    endat_req.pisr_rdst         = &endat_rdst_callback;
    err_code = R_ENDAT_Control(R_ENDAT0_ID, ENDAT_CMD_REQ, &endat_req);
    
    if (ENDAT_SUCCESS != err_code)
    {
        DEBUG_PRINT("  R_ENDAT_Control(ENDAT_CMD_REQ) error: %d"DEBUG_PRINT_END, (int)err_code);
    }

    return;
}
/*******************************************************************************
End of function endat_poscon
*******************************************************************************/

/*******************************************************************************
* Function Name: endat_stop
* Description  : "stop" command processing.
* Arguments    : None.
* Return Value : None.
*******************************************************************************/
static void endat_stop(void)
{
    r_endat_err_t err_code;
    uint32_t pos_upper;
    uint32_t pos_lower;
    uint16_t loop;
    
    endat_flg = false;
    
    DEBUG_PRINT("stop command" DEBUG_PRINT_END);

    err_code = R_ENDAT_Control(R_ENDAT0_ID, ENDAT_CMD_POS_STOP, NULL);
    
    if (ENDAT_SUCCESS == err_code)
    {
      
        while (false == endat_flg)
        {
            ; /* DO NOTHING */
        }
        
        DEBUG_PRINT("  poscon_valid : %d"DEBUG_PRINT_END, poscon_valid);
       
        for (loop = poscon_num; loop < ENDAT_POS_NUM; loop++)
        {
            DEBUG_PRINT("  %d ------------------------------ "DEBUG_PRINT_END, loop);
            
            if (ENDAT_REQ_SUCCESS == poscon_err[loop])
            {
                pos_upper = poscon[loop] >> ENDAT_SHIFT_32;
                pos_lower = poscon[loop] & ENDAT_MASK_LOW_32;
                DEBUG_PRINT("      result    : ENDAT_SUCCESS"DEBUG_PRINT_END);
                DEBUG_PRINT("      pos_upper : 0x%08X"DEBUG_PRINT_END, (int)pos_upper);
                DEBUG_PRINT("      pos_lower : 0x%08X"DEBUG_PRINT_END, (int)pos_lower);
            }
            else
            {
                DEBUG_PRINT("      result    : ENDAT_REQ_ERR"DEBUG_PRINT_END);
            }
        }
        
        for (loop = 0; loop < poscon_num; loop++)
        {
            DEBUG_PRINT("  %d ------------------------------ "DEBUG_PRINT_END, loop);
            
            if (ENDAT_REQ_SUCCESS == poscon_err[loop])
            {
                pos_upper = poscon[loop] >> ENDAT_SHIFT_32;
                pos_lower = poscon[loop] & ENDAT_MASK_LOW_32;
                DEBUG_PRINT("      result    : ENDAT_SUCCESS"DEBUG_PRINT_END);
                DEBUG_PRINT("      pos_upper : 0x%08X"DEBUG_PRINT_END, (int)pos_upper);
                DEBUG_PRINT("      pos_lower : 0x%08X"DEBUG_PRINT_END, (int)pos_lower);
            }
            else
            {
                DEBUG_PRINT("      result    : ENDAT_REQ_ERR"DEBUG_PRINT_END);
            }
        }
    }
    else
    {
        DEBUG_PRINT("  R_ENDAT_Control(ENDAT_CMD_POS_STOP) error: %d"DEBUG_PRINT_END, (int)err_code);
    }
    
    return;
}
/*******************************************************************************
End of function endat_stop
*******************************************************************************/

/*******************************************************************************
* Function Name: endat_temp
* Description  : "temp" command processing.
* Arguments    : None.
* Return Value : None.
*******************************************************************************/
static void endat_temp(void)
{
  
    r_endat_req_t endat_req;
    r_endat_err_t err_code = ENDAT_SUCCESS;
    float temp_celsius;
    
    endat_flg = false;

    DEBUG_PRINT("temp command" DEBUG_PRINT_END);
 
    endat_req.mode_cmd          = R_ENDAT_POS_MEM;
    endat_req.dt                = false;
    endat_req.mrs               = R_ENDAT_MRS_TEMP2;
    endat_req.watchdog.range    = R_ENDAT_WD_RANGE_US;
    endat_req.watchdog.time     = ENDAT_WDG_MAX;
    endat_req.pisr_result       = &endat_callback;
    endat_req.pisr_rdst         = &endat_rdst_callback;
    err_code = R_ENDAT_Control(R_ENDAT0_ID, ENDAT_CMD_REQ, &endat_req);
    
    if (ENDAT_SUCCESS == err_code)
    {
        while (false == endat_flg)
        {
            ; /* DO NOTHING */
        }
    }
    else
    {
        goto temp_err;
    }
    
    endat_flg = false;
    
    endat_req.mode_cmd          = R_ENDAT_POS_ADD_DATA;
    endat_req.dt                = false;
    endat_req.watchdog.range    = R_ENDAT_WD_RANGE_US;
    endat_req.watchdog.time     = ENDAT_WDG_MAX;
    endat_req.pisr_result       = &endat_callback;
    endat_req.pisr_rdst         = &endat_rdst_callback;
    err_code = R_ENDAT_Control(R_ENDAT0_ID, ENDAT_CMD_REQ, &endat_req);
    
    if (ENDAT_SUCCESS == err_code)
    {
        while (false == endat_flg)
        {
            ; /* DO NOTHING */
        }
        
        result_display(pendat_result, pendat_err);
        temp_celsius = (pendat_result->data.add_datum1 * ENDAT_TEMP_SCA_FAC) - ENDAT_TEMP_ABS_ZERO;
        DEBUG_PRINT(" temperature : %d [Celsius]" DEBUG_PRINT_END, (int)temp_celsius);
    }
    else
    {
        goto temp_err;
    }
    
    goto end;
    
temp_err:
    DEBUG_PRINT("  R_ENDAT_Control(ENDAT_CMD_REQ) error: %d"DEBUG_PRINT_END, (int)err_code);
    
end:    
    return;
}
/*******************************************************************************
End of function endat_temp
*******************************************************************************/

/*******************************************************************************
* Function Name: endat_callback
* Description  : Call back function for Endat.
* Arguments    : presult - Result of request for EnDat
*                perr    - Error information.
* Return Value : None.
*******************************************************************************/
static void endat_callback(r_endat_result_t * presult, r_endat_protocol_err_t *perr)
{
    pendat_result = presult;
    pendat_err = perr;
    
    if (false != pendat_err->modeerr)
    {
        endat_flg = true;
    }
    
    return;
}
/*******************************************************************************
End of function endat_callback
*******************************************************************************/

/*******************************************************************************
* Function Name: endat_poscon_callback
* Description  : Call back function for Endat.
* Arguments    : presult - Result of request for EnDat
*                perr    - Error information.
* Return Value : None.
*******************************************************************************/
static void endat_poscon_callback(r_endat_result_t * presult, r_endat_protocol_err_t *perr)
{   
  
    poscon_err[poscon_num] = presult->result;
    poscon[poscon_num] =  presult->data.pos;
    
    poscon_num++;  
    
    if (false != poscon_empty)
    {
        poscon_valid++;
    }
      
    if (ENDAT_POS_NUM <= poscon_num)
    {
        poscon_num = 0u;
        poscon_empty = false;
    }
    
    return;
}
/*******************************************************************************
End of function endat_poscon_callback
*******************************************************************************/

/*******************************************************************************
* Function Name: endat_rdst_callback
* Description  : Call back function for Endat.
* Arguments    : None.
* Return Value : None.
*******************************************************************************/
static void endat_rdst_callback(void)
{
    endat_flg = true;
    
    return;
}
/*******************************************************************************
End of function endat_rdst_callback
*******************************************************************************/

/*******************************************************************************
* Function Name: get_cmd
* Description  : Get token of command line.
* Arguments    : parg    - Command arguments.
*                arg_max - Maximum number of command arguments.
* Return Value : Number of command arguments.
*******************************************************************************/
#if 0
static uint32_t get_cmd(my_char_t *parg[], const uint32_t arg_max)
{
    uint32_t count = 0u;
    uint32_t loop = 0u;
    static my_char_t cmd_buf[CMD_BUF_SIZE];
    my_char_t *ptok;

    /* Read one line from the console. */
    while(1)
    {
        cmd_buf[loop] = (char)getchar();
        if (('\n' == (cmd_buf[loop])) || (loop >= (CMD_BUF_SIZE - 1)))
        {
            cmd_buf[loop] = '\0';
            break;
        }
        loop++;
    }
 
    /* Token of first. */
    ptok = strtok(&cmd_buf[0], CMD_DELIMITER);
    while ((NULL != ptok) && (count < arg_max))
    {
        parg[count] = ptok;

        /* Token of second and later. */
        ptok = strtok(NULL, CMD_DELIMITER);
        count++;
    }

    return count;
}
#endif
/*******************************************************************************
End of function get_cmd
*******************************************************************************/

/*******************************************************************************
* Function Name: cmd_exit
* Description  : "exit" command processing.
* Arguments    : pinfo   - Information of encoder.
*                parg    - Command arguments.
*                arg_num - Number of command arguments.
* Return Value : None.
*******************************************************************************/
static void cmd_exit(void)
{
    DEBUG_PRINT("exit command" DEBUG_PRINT_END);
    
    return;
}
/*******************************************************************************
End of function cmd_exit
*******************************************************************************/

/*******************************************************************************
* Function Name: result_display
* Description  : Call back function for Endat.
* Arguments    : presult - Result of request for EnDat
*                perr    - Error information.
* Return Value : None.
*******************************************************************************/
static void result_display(r_endat_result_t *presult, r_endat_protocol_err_t *perr)
{
    uint32_t pos_upper;
    uint32_t pos_lower;
    
    if (ENDAT_REQ_SUCCESS == presult->result)
    {
        pos_upper = presult->data.pos >> ENDAT_SHIFT_32;
        pos_lower = presult->data.pos & ENDAT_MASK_LOW_32;
        DEBUG_PRINT("  result     : ENDAT_SUCCESS" DEBUG_PRINT_END);
        DEBUG_PRINT("  pos_upper  : 0x%08X" DEBUG_PRINT_END, (int)pos_upper);
        DEBUG_PRINT("  pos_lower  : 0x%08X" DEBUG_PRINT_END, (int)pos_lower);
        DEBUG_PRINT("  add_datum1 : 0x%08X" DEBUG_PRINT_END, (int)presult->data.add_datum1);
        DEBUG_PRINT("  add_datum2 : 0x%08X" DEBUG_PRINT_END, (int)presult->data.add_datum2);
        
        if (false != presult->status.busy)
        {
            DEBUG_PRINT("  busy       : true " DEBUG_PRINT_END);
        }
        
        if (false != presult->status.rm)
        {
            DEBUG_PRINT("  rm         : true " DEBUG_PRINT_END);
        }
        
        if (false != presult->status.wrn)
        {
            DEBUG_PRINT("  wrn        : true " DEBUG_PRINT_END);
        }
    }
    else
    {
        DEBUG_PRINT("  result   : ENDAT_REQ_ERR   " DEBUG_PRINT_END); 
        if (false != perr->err1)
        {
            DEBUG_PRINT("    err1   : true " DEBUG_PRINT_END);
        }
        
        if (false != perr->crc1)
        {
            DEBUG_PRINT("    crc1   : true " DEBUG_PRINT_END);
        }
        
        if (false != perr->ftype1)
        {
            DEBUG_PRINT("    ftype1 : true" DEBUG_PRINT_END);
        }
        
        if (false != perr->ftype2)
        {
            DEBUG_PRINT("    ftype2 : true" DEBUG_PRINT_END);
        }
        
        if (false != perr->err2)
        {
            DEBUG_PRINT("    err2   : true" DEBUG_PRINT_END);
        }
        
        if (false != perr->crc3)
        {
            DEBUG_PRINT("    crc3   : true" DEBUG_PRINT_END);
        }
        
        if (false != perr->crc2)
        {
            DEBUG_PRINT("    crc2   : true" DEBUG_PRINT_END);
        }
        
        if (false != perr->wdg)
        {
            DEBUG_PRINT("    wdg    : true" DEBUG_PRINT_END);
        }
        
        if (false != perr->ftype3)
        {
            DEBUG_PRINT("    ftype3 : true" DEBUG_PRINT_END);
        }
        
        if (false !=  perr->modeerr)
        {
            DEBUG_PRINT("    modeerr: true" DEBUG_PRINT_END);
        }
        
    }
    
    return;
}

/*******************************************************************************
End of function result_display
*******************************************************************************/

/*******************************************************************************
* Function Name: timer_init
* Description  : Initialization of the CMT1 of CMT unit 0.
* Arguments    : None.
* Return Value : None.
*******************************************************************************/
static void timer_init(void)
{
    /* Power on the CMT0 Unit of CMT1 */
    R_RST_WriteEnable();
    MSTP(CMT0) = 0u;
    R_RST_WriteDisable();

    CMT1.CMCR.BIT.CKS = ENDAT_CMT1_CKS;
    
    return;
}
/*******************************************************************************
End of function timer_init
*******************************************************************************/

/*******************************************************************************
* Function Name: timer_wait
* Description  : Generation of wait time.
* Arguments    : time - wait time.
* Return Value : None.
*******************************************************************************/
static void timer_wait(uint32_t time)
{
    uint16_t count;

    count = (uint16_t)((time * ENDAT_TIME_PLACE) / ENDAT_TIME_RESOLUTION);
    
    CMT1.CMCNT = 0u;
    CMT.CMSTR0.BIT.STR1 = ENDAT_TIME_START;
    
    while (CMT1.CMCNT <= count)
    {
        ; /* DO NOTHING */
    }
    
    CMT.CMSTR0.BIT.STR1 = 0u;
    
    return;
}
/*******************************************************************************
End of function timer_wait
*******************************************************************************/

/*******************************************************************************
* Function Name: init_pin_hiz_conf_endat0
* Description  : Setting pin Hi-Z for configuration EnDat0.
* Arguments    : None.
* Return Value : None.
*******************************************************************************/
static void init_pin_hiz_conf_endat0(void)
{
   /* Set pin port mode(Hi-Z). */
    PORT7.PDR.BIT.B0 = PORT_DIRECTION_HIZ; /* P70:DATA_RC */
    PORT7.PDR.BIT.B1 = PORT_DIRECTION_HIZ; /* P71:DATA_DV */
    PORT7.PDR.BIT.B2 = PORT_DIRECTION_HIZ; /* P72:DE      */
    PORT7.PDR.BIT.B3 = PORT_DIRECTION_HIZ; /* P73:TCLK    */
    
    PORT7.PMR.BIT.B0 = PORT_MODE_GENERAL;
    PORT7.PMR.BIT.B1 = PORT_MODE_GENERAL;
    PORT7.PMR.BIT.B2 = PORT_MODE_GENERAL;
    PORT7.PMR.BIT.B3 = PORT_MODE_GENERAL;
    
    return;
}
/*******************************************************************************
End of function init_pin_hiz_conf_endat0
*******************************************************************************/

/*******************************************************************************
* Function Name: init_pin_conf_endat0
* Description  : Initialization of pin for configuration EnDat0.
* Arguments    : None.
* Return Value : None.
*******************************************************************************/
static void init_pin_conf_endat0(void)
{

    R_MPC_WriteEnable();

    /* Set pin using EnDat */
    MPC.P70PFS.BIT.PSEL = PSEL_VALUE;
    MPC.P71PFS.BIT.PSEL = PSEL_VALUE;
    MPC.P72PFS.BIT.PSEL = PSEL_VALUE;
    MPC.P73PFS.BIT.PSEL = PSEL_VALUE;
    
    R_MPC_WriteDisable();

    /* Set pin peripheral mode */
    PORT7.PMR.BIT.B0 = PORT_MODE_PERIPHERAL;
    PORT7.PMR.BIT.B1 = PORT_MODE_PERIPHERAL;
    PORT7.PMR.BIT.B2 = PORT_MODE_PERIPHERAL;
    PORT7.PMR.BIT.B3 = PORT_MODE_PERIPHERAL;

    return;
}
/*******************************************************************************
End of function init_pin_conf_endat0
*******************************************************************************/
