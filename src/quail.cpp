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
#include "string.h"
}
#define	CMD_NOP		0
#define	CMD_ACK		1
#define	CMD_IDSET	2
#define	CMD_NEXT_ID	3

template <unsigned Size> struct QuailData{
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
	unsigned char cmd_data_[Size];
	unsigned char checkSum_;
	unsigned char endCode_;

	int get_size(void){ return( sizeof(QuailData<Size>)); }
	int set_checksum()
	{
		unsigned char *p = (unsigned char*)this;
		int i ;
		int sum = p[0];
		for(i = 1; i < get_size() - 2; i++){
			sum = sum ^ p[i];
		}
		checkSum_ = sum;
		return sum;
	}
	void init()
	{
		start_code_ = 0xAA;
		endCode_ = 0x55;
		txGrpID_ = 0;
		rxGrpID_ = 0;
		rightGrpID_ = 0;
		rxAxisID_ = 0xfe;
		msgID_ = 0xd7;
		DATA_SIZE.size_low_ = (get_size() & 0x00FF);
		DATA_SIZE.size_high_ = (get_size() & 0xFF00) >> 8;
	}
	void set_data(unsigned char cmdNo, unsigned int msgId, int cmdCnt, unsigned char* data)
	{
		cmdID_ = cmdNo;
		msgID_ = msgId;

		for(int i = 0; i < cmdCnt; i++){
			cmd_data_[i] = data[i];
		}
		set_checksum();
	}
	void set_command(int msgId, char *cmd)
	{
		cmdID_ = 5;
		msgID_ = msgId;
		for(int i = 0; cmd[i]; i++){
			cmd_data_[i] = cmd[i];
		}
	}
} __attribute__ ((packed));

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

QuailData<2>	__recv;
QuailData<2>	__recv2;
QuailData<2>	__quail;
QuailData<1>	__quail1;
QuailData<5>	__quail5;
QuailData<8>	__recv8;

extern "C" unsigned short R_SCIFA0_Serial_Send(const unsigned char * tx_buf, unsigned short tx_num);
extern "C" unsigned short R_SCIFA0_Serial_Receive(unsigned char * rx_buf, unsigned short rx_num);

extern "C" void sys_wait(int tm);
extern "C" void waitMs(int ms);

extern int	__txComp;

extern "C" void send_quail_data(unsigned char* data, int sz)
{
	PORT2.PODR.BIT.B1 = 1;

	__txComp = 0;
	sys_wait(10000);
	R_SCIFA0_Serial_Send(data,sz);
	while( __txComp == 0);
}

extern "C" unsigned char* init_quail(void)
{
	unsigned char cmd[2]={0,0};

	__quail.init();

	// ID Reset {0,0}
	__quail.set_data(CMD_IDSET,0xd7,2,cmd);
	send_quail_data((unsigned char*)&__quail,__quail.get_size());

	// ID Set {0,1}
	cmd[0] = 0;
	cmd[1] = 1;
	__quail.set_data(CMD_IDSET,0xd8,2,cmd);

	R_SCIFA0_Serial_Receive((unsigned char*)&__recv,__recv.get_size());
	send_quail_data((unsigned char*)&__quail,__quail.get_size());

	//‰ž“š {0}
	__quail1.init();
	__quail1.txAxisID_ = 1;
	__quail1.rxAxisID_ = 0xFE;
	__quail1.rightAxisID_ = 1;
	__quail1.set_data(CMD_ACK,0xd9,1,cmd);

	R_SCIFA0_Serial_Receive((unsigned char*)&__recv2,__recv2.get_size());
	send_quail_data((unsigned char*)&__quail1,__quail1.get_size());

	waitMs(10);
	cmd[0] = 0;
	cmd[1] = 0xFE;
	__quail.init();
	__quail.set_data(CMD_NEXT_ID,0xdA,2,cmd);
	R_SCIFA0_Serial_Receive((unsigned char*)&__recv2,__recv2.get_size());
	send_quail_data((unsigned char*)&__quail,__quail.get_size());

	waitMs(10);
	__quail5.init();
	__quail5.set_command(0xDB,"INI.0");
	R_SCIFA0_Serial_Receive((unsigned char*)&__recv8,__recv8.get_size());
	send_quail_data((unsigned char*)&__quail5,__quail5.get_size());

	return (unsigned char*)&__quail;
}


