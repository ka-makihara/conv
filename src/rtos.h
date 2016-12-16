#ifndef __INC_RTOS_H__
#define __INC_RTOS_H__

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "event_groups.h"

/* Renesas includes. */
#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"

#include	<stdlib.h>
#include	<string.h>

#include "iodefine.h"
#include "string.h"

#define	WAIT_FOREVER	(uint32_t)0xFFFFFFFF
#define DELAY_MS(t)		( (t == WAIT_FOREVER) ? t : pdMS_TO_TICKS(t) )
#define	EV_MODE_AND		1
#define	EV_MODE_OR		0


typedef SemaphoreHandle_t	SEM_ID;
typedef TaskHandle_t		TASK_ID;
typedef EventGroupHandle_t	Event;
typedef QueueHandle_t		Queue;

extern	void	rtos_start(void);	//RTOS のｽｹｼﾞｭｰﾘﾝｸﾞを開始する
extern	uint32_t get_tick_count(void);

extern	Queue	rtos_create_queue(int cnt, uint32_t byteSize);
extern	long	rtos_send_queue(Queue id, void* val, long ms);
extern	long	rtos_wait_queue(Queue id, void* val, long ms);

extern	void	rtos_delay(int ms);

extern	int		rtos_create_semaphore(void);
extern	int		rtos_sem_take(SEM_ID hdl, unsigned long ms);
extern	int		rtos_sem_give(SEM_ID hdl);
extern	void	rtos_sem_delete(SEM_ID hdl);

extern	void	rtos_sem_give_ISR(SEM_ID hdl, long *task);	//割り込みﾊﾝﾄﾞﾗ内からｾﾏﾌｫをGiveする時に使用
extern	void	rtos_switch_task(long task);				//割り込み内で上記関数を呼び出した場合は最後に呼ぶ事

extern	void	rtos_task_suspend(TASK_ID task);
extern	void	rtos_task_resume(TASK_ID task);
extern	void	rtos_task_delete(TASK_ID task);

extern	Event		rtos_create_event(void);
extern	uint32_t	rtos_post_event_bit(Event ev, uint32_t bitData);
extern	uint32_t	rtos_wait_event_bit(Event ev, uint32_t bitData, int and_or, uint32_t ms);
extern	uint32_t	rtos_wait_event_bit_continue(Event ev, uint32_t bitData, int and_or, uint32_t ms);
extern	uint32_t	rtos_pos_event(Event ev);
extern	uint32_t	rtos_wait_event(Event ev, int32_t ms);

#endif /* __INC_RTOS_H__ */
