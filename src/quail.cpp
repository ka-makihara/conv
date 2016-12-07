/*
 * quail.cpp
 *
 *  Created on: 2016/12/06
 *      Author: makihara
 */
#include	<stdlib.h>
#include	<string.h>

extern "C" {
#include "iodefine.h"
}

class QuailData_INI{
private:
	unsigned char start_code_;
	union{
		unsigned short size_;
		struct{
			unsigned char size_high_;
			unsigned char size_low_;
		};
	} DATA_SIZE;
	unsigned char txGrpID_;
	unsigned char txAxisID_;
	unsigned char rxGrpID_;
	unsigned char rxAxisID_;
	unsigned char rightGrpID_;
	unsigned char rightAxisID_;
	unsigned char msgID_;
	unsigned char cmdID_;
	unsigned char cmddata[2];
	unsigned char checkSum_;
	unsigned char endCode_;

public:
	QuailData_INI(){}

	int set_checksum()
	{
		unsigned char *p = (unsigned char*)this;
		int i ;
		int sum = p[0];

		for(i = 1; i < sizeof(QuailData_INI) - 2; i++){
			sum = sum ^ p[i];
		}
		checkSum_ = sum;
		return sum;
	}
	void set_data(unsigned char cmdNo, unsigned int no, unsigned char d1, unsigned char d2)
	{
		cmdID_ = cmdNo;
		cmddata[0] = d1;
		cmddata[1] = d2;
		msgID_ = no;

		set_checksum();
	}
	void init()
	{
		start_code_ = 0xAA;
		endCode_ = 0x55;
		rxAxisID_ = 0xfe;
		msgID_ = 0xd7;
		DATA_SIZE.size_low_ = (sizeof(QuailData_INI) & 0x00FF);
		DATA_SIZE.size_high_ = (sizeof(QuailData_INI) & 0xFF00) >> 8;
	}

}  __attribute__ ((packed));

QuailData_INI __quail;
QuailData_INI __recv;

extern "C" unsigned short R_SCIFA0_Serial_Send(const unsigned char * tx_buf, unsigned short tx_num);
extern "C" unsigned short R_SCIFA0_Serial_Receive(unsigned char * rx_buf, unsigned short rx_num);

extern "C" void sys_wait(int tm);

extern "C" unsigned char* init_quail(void)
{
	//	QuailData *dat = new QuailData();
	//unsigned char *pp = (unsigned char*)malloc(100);
	__quail.init();

	__quail.set_data(2,0xd7,0,0);	// ID Reset
	R_SCIFA0_Serial_Send((unsigned char*)&__quail,sizeof(QuailData_INI));

	PORT2.PODR.BIT.B1 = 1;
	sys_wait(10000);

	R_SCIFA0_Serial_Receive((unsigned char*)&__recv,sizeof(QuailData_INI));

	__quail.set_data(2,0xd8,0,1);	//ID Set
	PORT2.PODR.BIT.B1 = 1;
	R_SCIFA0_Serial_Send((unsigned char*)&__quail,sizeof(QuailData_INI));

	//R_SCIFA0_Serial_Receive((unsigned char*)&__recv,sizeof(QuailData_INI));
/*
	sys_wait(10000);
	__quail.set_data(1,0xd9,0,0);	//ACK
	PORT2.PODR.BIT.B1 = 1;
	R_SCIFA0_Serial_Send((unsigned char*)&__quail,sizeof(QuailData_INI));
*/
	return (unsigned char*)&__quail;
}

extern "C" void recv_quail()
{
	R_SCIFA0_Serial_Receive((unsigned char*)&__recv,sizeof(QuailData_INI));
}

