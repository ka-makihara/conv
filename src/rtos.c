
#include "rtos.h"

unsigned long get_tick_count(void)
{
	return (unsigned long)xTaskGetTickCount();
}

void rtos_delay(int ms)
{
/*
	uint32_t xNextWakeTime = get_tick_count();
	uint32_t x = xNextWakeTime;

	vTaskDelayUntil( &xNextWakeTime,DELAY_MS(ms) );

	return( xNextWakeTime - x );
*/
	vTaskDelay( DELAY_MS(ms) );

}

int rtos_create_semaphore()
{
	return xSemaphoreCreateBinary();
}

int rtos_sem_take(SEM_ID hdl, unsigned long ms)
{
	return xSemaphoreTake(hdl, DELAY_MS(ms));
}

int rtos_sem_give(SEM_ID hdl)
{
	return xSemaphoreGive( hdl );
}

void rtos_sem_delete(SEM_ID id)
{
	vSemaphoreDelete( id );
}

void rtos_sem_give_ISR(SEM_ID hdl, long *task)
{
	xSemaphoreGiveFromISR(hdl, task);
}

void rtos_switch_task(long task)
{
	portYIELD_FROM_ISR( task );
}

void rtos_task_suspend(TASK_ID task)
{
	vTaskSuspend(task);
}

void rtos_task_resume(TASK_ID task)
{
	vTaskResume(task);
}

void rtos_task_delete(TASK_ID task)
{
	vTaskDelete(task);
}

Queue rtos_create_queue(int cnt, uint32_t byteSize)
{
	return xQueueCreate(cnt,byteSize);
}
long rtos_send_queue(Queue id, void* val, long ms)
{
	return xQueueSend(id,val,DELAY_MS(ms));
}
long	rtos_wait_queue(Queue id, void* val, long ms)
{
	return xQueueReceive(id,val,DELAY_MS(ms));	
}

Event	rtos_create_event(void)
{
	return xEventGroupCreate();
}

uint32_t rtos_post_event_bit(Event ev, uint32_t bitData)
{
	return xEventGroupSetBits(ev,bitData);
}
uint32_t rtos_pos_event(Event ev)
{
	return xEventGroupSetBits(ev,1);
}

uint32_t rtos_wait_event_bit(Event ev, uint32_t bitData, int and_or, uint32_t ms)
{
	return xEventGroupWaitBits(ev,bitData,pdTRUE,and_or,DELAY_MS(ms));
}
uint32_t rtos_wait_event_bit_continue(Event ev, uint32_t bitData, int and_or, uint32_t ms)
{
	return xEventGroupWaitBits(ev,bitData,pdFALSE,and_or,DELAY_MS(ms));
}

uint32_t rtos_wait_event(Event ev, int32_t ms)
{
	return xEventGroupWaitBits(ev,1,pdTRUE,pdTRUE,DELAY_MS(ms));
}

void rtos_start(void)
{
	vTaskStartScheduler();
}
