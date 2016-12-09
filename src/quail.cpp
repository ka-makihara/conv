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
#define	CMD_BIN		4
#define	CMD_TEXT	5

typedef unsigned char u_char;
typedef unsigned short u_short;

extern "C" unsigned short R_SCIFA0_Serial_Send(const unsigned char * tx_buf, unsigned short tx_num);
extern "C" unsigned short R_SCIFA0_Serial_Receive(unsigned char * rx_buf, unsigned short rx_num);

extern "C" void sys_wait(int tm);
extern "C" void waitMs(int ms);

extern int	__txComp;

struct Quail{
	unsigned char start_code_;
	union{
		unsigned short size_;
		struct{
			unsigned char size_high_;
			unsigned char size_low_;
		};
	} DATA_SIZE;
	unsigned char tgtGrpID_;	//送信先ｸﾞﾙｰﾌﾟID
	unsigned char tgtAxisID_;	//送信先軸ID
	unsigned char srcGrpID_;	//送信元ｸﾞﾙｰﾌﾟID
	unsigned char srcAxisID_;	//送信元軸ID
	unsigned char rightGrpID_;
	unsigned char rightAxisID_;
	unsigned char msgID_;
	unsigned char cmdID_;

	void init(u_char tgtID=0, u_char srcID=0xFE, u_char rID=0)
	{
		start_code_ = 0xAA;
		srcGrpID_ = 0;
		tgtGrpID_ = 0;
		rightGrpID_ = 0;

		tgtAxisID_ = tgtID;
		srcAxisID_ = srcID;
		rightAxisID_ = rID;
	}
	Quail(u_char tgtID=0, u_char srcID=0xFE, u_char rID=0)
	{
		init(tgtID,srcID,rID);
	}
} __attribute__ ((packed));

template <unsigned Size> struct R_DATA {
	u_char	data[Size];
	void* operator & ()
	{
		return( (void*)this );
	}
	u_char &operator[](int idx)
	{
		return( data[idx] );
	}
	operator u_char* ()
	{
		return( (u_char*)this );
	}

} __attribute__ ((packed));

int get_checksum(u_char* data, int sz)
{
	u_char *p = data;
	int sum = p[0];

	for(int i = 1; i < sz; i++){
		sum = sum ^ p[i];
	}
	return sum;
}

int length(const char *str)
{
	int ll;

	for(ll = 0; str[ll]; ll++);

	return ll;
}

int setup_quail_cmd(Quail pQuail,u_char *buff, u_char no, u_char type, int cmdLen, u_char *cmd)
{
	u_char *pBase = (u_char*)&pQuail;
	int idx = 0;

	pQuail.msgID_ = no;
	pQuail.cmdID_ = type;
	//実ﾃﾞｰﾀ部までをｺﾋﾟｰ
	for(idx = 0; idx < sizeof(Quail); idx++){
		buff[idx] = pBase[idx]; 
	}
	//実ﾃﾞｰﾀ部
	for(int i = 0; i < cmdLen; i++){
		buff[idx++] = cmd[i];
	}

	u_short dataSize;

	if( type == CMD_TEXT ){
		buff[idx++] = 0x0D;	//CR
		buff[idx++] = 0x0A;	//LF
		buff[idx++] = 0x00;	//NULL
		
		dataSize = sizeof(Quail) + cmdLen + 5;	//CR + LF + NULL + CheckSum + 終端(0x55)
	}
	else{
		dataSize = sizeof(Quail) + cmdLen + 2;	//CheckSum + 終端(0x55)
	}

	((Quail*)buff)->DATA_SIZE.size_high_ = (dataSize & 0xFF00) >> 8;
	((Quail*)buff)->DATA_SIZE.size_low_  = (dataSize & 0x00FF);

	buff[idx++] = get_checksum(buff,idx-1);
	buff[idx++] = 0x55;	//終端

	return( idx );
}

/*
template <unsigned Size> int setup_quail_cmd(Quail pQuail,u_char *buff, u_char no, u_char type, int cmdLen, R_DATA<Size> cmd)
{
	u_char *pBase = (u_char*)&pQuail;
	int idx = 0;

	pQuail.msgID_ = no;
	pQuail.cmdID_ = type;
	//実ﾃﾞｰﾀ部までをｺﾋﾟｰ
	for(idx = 0; idx < sizeof(Quail); idx++){
		buff[idx] = pBase[idx]; 
	}
	//実ﾃﾞｰﾀ部
	for(int i = 0; i < cmdLen; i++){
		buff[idx++] = cmd[i];
	}

	u_short dataSize;

	if( type == CMD_TEXT ){
		buff[idx++] = 0x0D;	//CR
		buff[idx++] = 0x0A;	//LF
		buff[idx++] = 0x00;	//NULL
		
		dataSize = sizeof(Quail) + cmdLen + 5;	//CR + LF + NULL + CheckSum + 終端(0x55)
	}
	else{
		dataSize = sizeof(Quail) + cmdLen + 2;	//CheckSum + 終端(0x55)
	}

	((Quail*)buff)->DATA_SIZE.size_high_ = (dataSize & 0xFF00) >> 8;
	((Quail*)buff)->DATA_SIZE.size_low_  = (dataSize & 0x00FF);

	buff[idx++] = get_checksum(buff,idx-1);
	buff[idx++] = 0x55;	//終端

	return( idx );
}
*/

extern "C" void send_quail_data(unsigned char* data, int sz)
{
	PORT2.PODR.BIT.B1 = 1;

	__txComp = 0;
	sys_wait(10000);
	R_SCIFA0_Serial_Send(data,sz);
	while( __txComp == 0);
}
extern void move_test();

void t_func(u_char *p)
{
	return;
}
template<unsigned sz> int tt_func(R_DATA<sz> obj)
{
	u_char d1 = obj[0];
	u_char d2 = obj[1];

	return d1 + d2;
}

extern "C" void init_quail(void)
{
	u_char	cmd[2]={0,0};
	u_char	tmp[100];
	u_char	send_cmd[100];
	int		packetSz;

/*
	R_DATA<2> dd = {10,120};
	t_func( (u_char*)&dd );
	tt_func( R_DATA<2>{1,2} );
*/

	// ID Reset {0,0}
	packetSz = setup_quail_cmd(Quail(),send_cmd,0xD7,CMD_IDSET,2,cmd);
	//packetSz = setup_quail_cmd(Quail(),send_cmd,0xD7,CMD_IDSET,2,R_DATA<2>{0,0});
	send_quail_data((unsigned char*)send_cmd,packetSz);

	// ID Set {0,1}
	cmd[0] = 0;
	cmd[1] = 1;
	packetSz = setup_quail_cmd(Quail(0,0xFE,0),send_cmd,0xD8,CMD_IDSET,2,cmd);

	R_SCIFA0_Serial_Receive((unsigned char*)&tmp,sizeof(tmp));
	send_quail_data((unsigned char*)send_cmd,packetSz);

	//応答 {0}
	packetSz = setup_quail_cmd(Quail(1,0xFE,1),send_cmd,0xD9,CMD_ACK,1,cmd);

	R_SCIFA0_Serial_Receive((unsigned char*)&tmp,sizeof(tmp));
	send_quail_data((unsigned char*)send_cmd,packetSz);

	//次送信権ID
	waitMs(100);
	cmd[0] = 0;
	cmd[1] = 0xFE;
	packetSz = setup_quail_cmd(Quail(0,0xFE,0),send_cmd,0xDA,CMD_NEXT_ID,2,cmd);

	R_SCIFA0_Serial_Receive((unsigned char*)&tmp,sizeof(tmp));
	send_quail_data((unsigned char*)send_cmd,packetSz);

	//INI.0 ｺﾏﾝﾄﾞ
	waitMs(100);
	packetSz = setup_quail_cmd(Quail(0,0xFE,1),send_cmd,0x17,CMD_TEXT,length("INI.0"),(u_char*)"INI.0");

	R_SCIFA0_Serial_Receive((unsigned char*)&tmp,sizeof(tmp));
	send_quail_data((unsigned char*)send_cmd,packetSz);

	//NOP
	waitMs(100);
	packetSz = setup_quail_cmd(Quail(0,0xFE,1),send_cmd,0xDB,CMD_NOP,0,cmd);

	R_SCIFA0_Serial_Receive((unsigned char*)&tmp,sizeof(tmp));
	send_quail_data((unsigned char*)send_cmd,packetSz);

	move_test();
}


void move_test()
{
	u_char	tmp[64];
	u_char	send_cmd[64];
	int		packetSz;
	char*	cmd_data[] = {"ACC1.1=100", "ACC1.1",
						"DEC1.1=100", "DEC1.1",
						"TRC1.1=100", "TRQ1.1",
						"VEL1.1=100", "VEL1.1",
						"POS1.1=8000", "POS2.1=0",
						"MON.0", "MPA1.1", "MPA2.1"};

	for(int i = 0; i < 13; i++){
		char *cmdStr = cmd_data[i];

		waitMs(100);
		packetSz = setup_quail_cmd(Quail(0,0xFE,1),send_cmd,0x20+i,CMD_TEXT,length(cmdStr),(u_char*)cmdStr);
		R_SCIFA0_Serial_Receive((u_char*)tmp,sizeof(tmp));
		send_quail_data((u_char*)send_cmd,packetSz);
	}
}
