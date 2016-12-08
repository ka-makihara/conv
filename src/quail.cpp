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
#define	CMD_NOP			0
#define	CMD_ACK			1
#define	CMD_IDSET		2
#define	CMD_NEXT_ID		3
#define	CMD_SEND_BIN	4
#define	CMD_SEND_TEXT	5

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
		int i;

		cmdID_ = CMD_SEND_TEXT;
		msgID_ = msgId;
		for(i = 0; cmd[i]; i++){
			cmd_data_[i] = cmd[i];
		}
		cmd_data_[i++] = 0x0D;
		cmd_data_[i++] = 0x0A;
		cmd_data_[i++] = 0x00;
		set_checksum();
	}
} __attribute__ ((packed));

QuailData<2>	__recv;
QuailData<2>	__recv2;
QuailData<2>	__quail2;
QuailData<1>	__quail1;
QuailData<8>	__quail8;
QuailData<8>	__recv8;
QuailData<0>	__quail0;

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
extern void move_test();

extern "C" void init_quail(void)
{
	unsigned char cmd[2]={0,0};
	unsigned char	tmp[300];

	__quail2.init();

	// ID Reset {0,0}
	__quail2.set_data(CMD_IDSET,0xd7,2,cmd);
	send_quail_data((unsigned char*)&__quail2,__quail2.get_size());

	// ID Set {0,1}
	cmd[0] = 0;
	cmd[1] = 1;
	__quail2.set_data(CMD_IDSET,0xd8,2,cmd);

	R_SCIFA0_Serial_Receive((unsigned char*)&tmp,sizeof(tmp));
	send_quail_data((unsigned char*)&__quail2,__quail2.get_size());

	//���� {0}
	__quail1.init();
	__quail1.txAxisID_ = 1;
	__quail1.rightAxisID_ = 1;
	__quail1.set_data(CMD_ACK,0xd9,1,cmd);

	R_SCIFA0_Serial_Receive((unsigned char*)&tmp,sizeof(tmp));
	send_quail_data((unsigned char*)&__quail1,__quail1.get_size());

	//�����M��ID
	waitMs(100);
	cmd[0] = 0;
	cmd[1] = 0xFE;
	__quail2.init();
	__quail2.set_data(CMD_NEXT_ID,0xdA,2,cmd);
	R_SCIFA0_Serial_Receive((unsigned char*)&tmp,sizeof(tmp));
	send_quail_data((unsigned char*)&__quail2,__quail2.get_size());

	//INI.0 �����
	waitMs(100);
	__quail8.init();
	__quail8.rightAxisID_ = 0x1;
	__quail8.set_command(0x17,"INI.0");
	R_SCIFA0_Serial_Receive((unsigned char*)&tmp,sizeof(tmp));
	send_quail_data((unsigned char*)&__quail8,__quail8.get_size());

	//NOP
	waitMs(100);
	__quail0.init();
	__quail0.rightAxisID_ = 0x1;
	__quail0.set_data(CMD_NOP,0xDB,0,cmd);
	R_SCIFA0_Serial_Receive((unsigned char*)&tmp,sizeof(tmp));
	send_quail_data((unsigned char*)&__quail0,__quail0.get_size());

	move_test();
}


void move_test()
{
	QuailData<13> quail13;
	QuailData<9>  quail9;
	QuailData<14> quail14;
	QuailData<8>  quail8;
	QuailData<11> quail11;
	unsigned char tmp[64];

	waitMs(100);
	quail13.init();
	quail13.rightAxisID_ = 0x01;
	quail13.set_command(0x20,"ACC1.1=100");
	R_SCIFA0_Serial_Receive((unsigned char*)tmp,sizeof(tmp));
	send_quail_data((unsigned char*)&quail13,quail13.get_size());

	waitMs(100);
	quail9.init();
	quail9.rightAxisID_ = 0x01;
	quail9.set_command(0x21,"ACC1.1");
	R_SCIFA0_Serial_Receive((unsigned char*)tmp,sizeof(tmp));
	send_quail_data((unsigned char*)&quail9,quail9.get_size());

	waitMs(100);
	quail13.init();
	quail13.rightAxisID_ = 0x01;
	quail13.set_command(0x22,"DEC1.1=100");
	R_SCIFA0_Serial_Receive((unsigned char*)tmp,sizeof(tmp));
	send_quail_data((unsigned char*)&quail13,quail13.get_size());

	waitMs(100);
	quail9.init();
	quail9.rightAxisID_ = 0x01;
	quail9.set_command(0x23,"DEC1.1");
	R_SCIFA0_Serial_Receive((unsigned char*)tmp,sizeof(tmp));
	send_quail_data((unsigned char*)&quail9,quail9.get_size());

	waitMs(100);
	quail13.init();
	quail13.rightAxisID_ = 0x01;
	quail13.set_command(0x22,"TRQ1.1=100");
	R_SCIFA0_Serial_Receive((unsigned char*)tmp,sizeof(tmp));
	send_quail_data((unsigned char*)&quail13,quail13.get_size());

	waitMs(100);
	quail9.init();
	quail9.rightAxisID_ = 0x01;
	quail9.set_command(0x23,"TRQ1.1");
	R_SCIFA0_Serial_Receive((unsigned char*)tmp,sizeof(tmp));
	send_quail_data((unsigned char*)&quail9,quail9.get_size());

	waitMs(100);
	quail13.init();
	quail13.rightAxisID_ = 0x01;
	quail13.set_command(0x22,"VEL1.1=100");
	R_SCIFA0_Serial_Receive((unsigned char*)tmp,sizeof(tmp));
	send_quail_data((unsigned char*)&quail13,quail13.get_size());

	waitMs(100);
	quail9.init();
	quail9.rightAxisID_ = 0x01;
	quail9.set_command(0x23,"VEL1.1");
	R_SCIFA0_Serial_Receive((unsigned char*)tmp,sizeof(tmp));
	send_quail_data((unsigned char*)&quail9,quail9.get_size());

	waitMs(100);
	quail14.init();
	quail14.rightAxisID_ = 0x01;
	quail14.set_command(0x23,"POS1.1=8000");
	R_SCIFA0_Serial_Receive((unsigned char*)tmp,sizeof(tmp));
	send_quail_data((unsigned char*)&quail14,quail14.get_size());

	waitMs(100);
	quail11.init();
	quail11.rightAxisID_ = 0x01;
	quail11.set_command(0x23,"POS2.1=0");
	R_SCIFA0_Serial_Receive((unsigned char*)tmp,sizeof(tmp));
	send_quail_data((unsigned char*)&quail11,quail11.get_size());

	waitMs(100);
	quail8.init();
	quail8.rightAxisID_ = 0x1;
	quail8.set_command(0x17,"MON.0");
	R_SCIFA0_Serial_Receive((unsigned char*)tmp,sizeof(tmp));
	send_quail_data((unsigned char*)&quail8,quail8.get_size());

	waitMs(100);
	quail9.init();
	quail9.rightAxisID_ = 0x01;
	quail9.set_command(0x23,"MPA1.1");
	R_SCIFA0_Serial_Receive((unsigned char*)tmp,sizeof(tmp));
	send_quail_data((unsigned char*)&quail9,quail9.get_size());

	waitMs(100);
	quail9.init();
	quail9.rightAxisID_ = 0x01;
	quail9.set_command(0x23,"MPA2.1");
	R_SCIFA0_Serial_Receive((unsigned char*)tmp,sizeof(tmp));
	send_quail_data((unsigned char*)&quail9,quail9.get_size());
}

/*
ACC1.1=100
ACC1.1
DEC1.1=100
DEC1.1
TRQ1.1=100
TRQ1.1
VEL1.1=100
VEL1.1
POS1.1=8000
POS2.1=0

MON1.1

MPA1.1
MPA2.1

*/


