/*
 * endat.c
 *
 *  Created on: 2016/12/15
 *      Author: makihara
 */
#include "rtos.h"

extern int endat_pos(uint32_t *posUpper, uint32_t *posLower);

Event	__enDatEv = NULL;
Queue	__enDatQu = NULL;

extern	Event		__servoEvent;
extern	uint64_t	__endat_pos;

void endat_task(void *pParams)
{
	uint32_t retVal;
	uint32_t pos_upper;
	uint32_t pos_lower;
	uint64_t pos64;

	while( 1 ){
		//現在値取得開始
		rtos_wait_queue(__enDatQu,&retVal,WAIT_FOREVER);
		int same = 0;
		while(1){
			if( endat_pos(&pos_upper,&pos_lower) < 0 ){
				break;
			}
			pos64 = pos_upper;	//32bitｼﾌﾄの為に一旦64bit変数へ代入
			pos64 = (pos64 << 32) + pos_lower;

			uint64_t sub = abs(__endat_pos - pos64);

			if( sub <= 2000 ){
				same++;
			}
			else{
				same = 0;
			}
			if( same > 1 ){
				//2000以下が10ms以上継続(delayが10msなので、2回以上連続したら)
				rtos_post_event_bit(__servoEvent,2);
				break;
			}

			rtos_delay(5);
		}
	}
}

void create_endat_task()
{
	__enDatEv = rtos_create_event();
	__enDatQu = rtos_create_queue(1,sizeof(uint32_t));

	xTaskCreate( endat_task, "EnDatPos", 300,0,tskIDLE_PRIORITY+1,NULL);
}