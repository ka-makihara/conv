/*
 * quail.cpp
 *
 *  Created on: 2016/12/06
 *      Author: makihara
 */
/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Renesas includes. */
#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"

#include 	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include "iodefine.h"
#include "string.h"
#include "rtos.h"

#define	CMD_NOP		0
#define	CMD_ACK		1
#define	CMD_IDSET	2
#define	CMD_NEXT_ID	3
#define	CMD_BIN		4
#define	CMD_TEXT	5
#define	CMD_UPLOAD	6

#define	QUAIL_SIZE_HI_OFFSET	1
#define	QUAIL_SIZE_LO_OFFSET	2
#define	QUAIL_CMD_OFFSET		10	//ﾒｯｾｰｼﾞｲﾝﾃﾞｯｸｽ
#define	QUAIL_DATA_OFFSET		11

#define	QUAIL_STAT_MOVE			(3)			//移動ｲﾝﾃﾞｯｸｽ(2ﾋﾞｯﾄで0,1,2を表わす)
#define	QUAIL_STAT_MOTOR_ON		(1 << 2)	//ﾓｰﾀｰON
#define	QUAIL_STAT_INPOS		(1 << 3)	//ｲﾝﾎﾟｼﾞｼｮﾝ
#define	QUAIL_STAT_RELEASE		(1 << 4)	//払い出し中
#define	QUAIL_STAT_TRQ_CTRL		(1 << 5)	//押付ﾄﾙｸ制御中
#define	QUAIL_STAT_TRQ_COMP		(1 << 6)	//押付完了状態
#define	QUAIL_STAT_INIT_POS		(1 << 7)	//原点復帰完了
#define	QUAIL_STAT_MOVE_COMP	(1 << 8)	//位置決め完了
#define	QUAIL_STAT_DOVEL_WAIT	(1 << 9)	//ﾄﾞｳﾞｪﾙ待機中
#define	QUAIL_STAT_EMG_STOP		(1 << 13)	//緊急停止
#define	QUAIL_STAT_ERROR		(1 << 14)	//ｴﾗｰ中
#define	QUAIL_STAT_PASSWORD		(1 << 15)	//ﾊﾟｽﾜｰﾄﾞ管理中

#define	QUAIL_CMD_GET			1
#define	QUAIL_CMD_POS			2

#define	ARRAY_COUNT(d)	(sizeof(d) / sizeof(d[0]))
#define	MM_TO_PULSE(mm)	(int32_t)(8000.0 * mm / 17.952)				//mmをQuailのﾊﾟﾙｽ値に変換
#define	MM_TO_SCALE(mm)	(uint64_t)((double)mm * 1000000.0 / 50.0)	//mmをﾘﾆｱｽｹｰﾙの番地へ変換

#define	SCALE_TO_MM(mm)	 ((double)mm * 50.0 / 1000000.0)			//ﾘﾆｱｽｹｰﾙ値をmmへ
#define	SCALE_TO_PULSE(mm)	MM_TO_PULSE( SCALE_TO_MM(mm) )			//ﾘﾆｱｽｹｰﾙ値をﾊﾟﾙｽ値に

typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned long  u_long;

extern  unsigned short R_SCIFA0_Serial_Send(const unsigned char * tx_buf, unsigned short tx_num);
extern  unsigned short R_SCIFA0_Serial_Receive(unsigned char * rx_buf, unsigned short rx_num);

extern  void	sys_wait(int tm);
extern  void	waitMs(int ms);
extern	void	move_test();
extern	int		endat_pos(uint32_t *upper, uint32_t *lower);

extern	int	__txComp;
extern	int	__rxComp;
extern	int g_rx_cnt;

extern	Queue 	__enDatQu;


Queue	__quailQu = NULL;
uint64_t	__endat_pos;	//位置監視用

//////////////////////////////////////////////////////
//
// Quail との通信用ﾃﾞｰﾀ
//
#pragma pack(1)
typedef struct Quail{
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
	// 実際はここに実ﾃﾞｰﾀ (0～263 ﾊﾞｲﾄ)
	// ﾁｪｯｸｻﾑ 	(u_char)
	// 終了ｺｰﾄﾞ(0x55)
} Quail ;


void quail_init_cmd(Quail *pQuail, u_char tgtID, u_char srcID, u_char rID)
{
	pQuail->start_code_ = 0xAA;
	pQuail->srcGrpID_ = 0;
	pQuail->tgtGrpID_ = 0;
	pQuail->rightGrpID_ = 0;

	pQuail->tgtAxisID_ = tgtID;
	pQuail->srcAxisID_ = srcID;
	pQuail->rightAxisID_ = rID;
}

/////////////////////////////////////////////////////
//
// ﾁｪｯｸｻﾑの計算
//   ﾃﾞｰﾀの先頭から、実ﾃﾞｰﾀ部までの各ﾊﾞｲﾄのXOR値
//
int get_checksum(u_char* data, int sz)
{
	u_char *p = data;
	int sum = p[0];

	for(int i = 1; i < sz; i++){
		sum = sum ^ p[i];
	}
	return sum;
}

/////////////////////////////////////////////////////
//
// strlen() の代わり
//
int length(const char *str)
{
	int ll;

	for(ll = 0; str[ll]; ll++);

	return ll;
}

////////////////////////////////////////////////////
//
// Quail のﾃﾞｰﾀﾊﾟｹｯﾄを生成する
//    pQuail: Quailﾊﾟｹｯﾄの基本ﾃﾞｰﾀ(quail_init_cmd()で生成)
//      buff: 生成される送信用ﾊﾞｯﾌｧ
//        no: ﾒｯｾｰｼﾞID(必ずｲﾝｸﾘﾒﾝﾄすること)
//      type: ｺﾏﾝﾄﾞﾀｲﾌﾟ(通常はﾃｷｽﾄ=CMD_TEXT)
//    cmdLen: cmd の長さ
//       cmd: 実ﾃﾞｰﾀに書き込むﾃﾞｰﾀ
//
int setup_quail_cmd(Quail pQuail,u_char *buff, u_char no, u_char type, int cmdLen, u_char *cmd)
{
	u_char *pBase = (u_char*)&pQuail;
	int idx = 0;

	int sz = sizeof(Quail);

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
		//送信ﾃﾞｰﾀがﾃｷｽﾄの場合は <CR> + <LF> + NULL を追加する
		buff[idx++] = 0x0D;	//CR
		buff[idx++] = 0x0A;	//LF
		buff[idx++] = 0x00;	//NULL
		
		dataSize = sizeof(Quail) + cmdLen + 5;	//CR + LF + NULL + CheckSum + 終端(0x55)
	}
	else{
		dataSize = sizeof(Quail) + cmdLen + 2;	//CheckSum + 終端(0x55)
	}

	//ﾊﾟｹｯﾄｻｲｽﾞの設定
	((Quail*)buff)->DATA_SIZE.size_high_ = (dataSize & 0xFF00) >> 8;
	((Quail*)buff)->DATA_SIZE.size_low_  = (dataSize & 0x00FF);

	//ﾁｪｯｸｻﾑと終端ｺｰﾄﾞ
	buff[idx++] = get_checksum(buff,idx-1);
	buff[idx++] = 0x55;	//終端

	return( idx );
}

////////////////////////////////////////////////////////
//
// Quail に対してﾃﾞｰﾀを送信する
//	※Quailは半二重通信、I/Oで送信・受信を切り替える必要がある
//    送信完了割り込み内で、I/Oは OFFされています
// 
//
void send_quail_data(unsigned char* data, int sz)
{
	PORT2.PODR.BIT.B1 = 1;

	__txComp = 0;
	sys_wait(10000);
	R_SCIFA0_Serial_Send(data,sz);
	while( __txComp == 0);
}

//////////////////////////////////////////////////////////
//
// Quail からのﾃﾞｰﾀ受信
//  ※受信は「予約」のような意味となっているので、送信する前に受信「予約」をする事
//      R_SCIFA0_Serial_Receive() では受信割り込みを許可
//
void recv_quail_data(unsigned char* data, int sz)
{
	__rxComp = 0;
	g_rx_cnt = 0;
	R_SCIFA0_Serial_Receive(data,sz);
}

void quail_command(int cmdNo, int rID, int cmdType, char *cmdStr, u_char *recv_cmd, int recvSize)
{
	int		packetSz;
	u_char	send_cmd[64];
	Quail 	cmdData;

	quail_init_cmd(&cmdData,0,0xFE,rID);

	packetSz = setup_quail_cmd(cmdData,send_cmd,cmdNo,cmdType,length(cmdStr),(u_char*)cmdStr);
	recv_quail_data((u_char*)recv_cmd,recvSize);
	send_quail_data((u_char*)send_cmd,packetSz);

}

/////////////////////////////////////////////////////////////////////////
//
// Quail ｺﾝﾄﾛｰﾗｰの初期化
//
extern  void init_quail(void)
{
	u_char	cmd[2]={0,0};
	u_char	tmp[100];
	u_char	send_cmd[100];
	int		packetSz;
	Quail	cmdData;

	// ID Reset {0,0}
	quail_init_cmd(&cmdData,0,0xFE,0);
	packetSz = setup_quail_cmd(cmdData,send_cmd,0xD7,CMD_IDSET,2,cmd);
	send_quail_data((unsigned char*)send_cmd,packetSz);

	// ID Set {0,1}
	waitMs(100);
	cmd[0] = 0;
	cmd[1] = 1;
	quail_init_cmd(&cmdData,0,0xFE,0);
	packetSz = setup_quail_cmd(cmdData,send_cmd,0xD8,CMD_IDSET,2,cmd);
	recv_quail_data((unsigned char*)&tmp,sizeof(tmp));
	send_quail_data((unsigned char*)send_cmd,packetSz);

	//応答 {0}
	waitMs(100);
	quail_init_cmd(&cmdData,1,0xFE,1);
	packetSz = setup_quail_cmd(cmdData,send_cmd,0xD9,CMD_ACK,1,cmd);
	recv_quail_data((unsigned char*)&tmp,sizeof(tmp));
	send_quail_data((unsigned char*)send_cmd,packetSz);

	//次送信権ID
	waitMs(100);
	cmd[0] = 0;
	cmd[1] = 0xFE;
	quail_init_cmd(&cmdData,0,0xFE,0);
	packetSz = setup_quail_cmd(cmdData,send_cmd,0xDA,CMD_NEXT_ID,2,cmd);
	recv_quail_data((unsigned char*)&tmp,sizeof(tmp));
	send_quail_data((unsigned char*)send_cmd,packetSz);

	//INI.0 ｺﾏﾝﾄﾞ
	waitMs(100);
	quail_init_cmd(&cmdData,0,0xFE,1);
	packetSz = setup_quail_cmd(cmdData,send_cmd,0x17,CMD_TEXT,length("INI.0"),(u_char*)"INI.0");
	recv_quail_data((unsigned char*)&tmp,sizeof(tmp));
	send_quail_data((unsigned char*)send_cmd,packetSz);

	//NOP
	waitMs(100);
	quail_init_cmd(&cmdData,0,0xFE,1);
	packetSz = setup_quail_cmd(cmdData,send_cmd,0xDB,CMD_NOP,0,cmd);
	recv_quail_data((unsigned char*)&tmp,sizeof(tmp));
	send_quail_data((unsigned char*)send_cmd,packetSz);

	 //quail_command(0xDB, CMD_NOP, "", tmp, sizeof(tmp))

	move_test();
}

/////////////////////////////////////////////////////
//
// ﾓｰﾀｰの動作確認用のﾃｽﾄ
//
void move_test()
{
	u_char	tmp[64];
	int i;
	char*	cmd_data[] = {"ACC1.1=100", "ACC1.1", "DEC1.1=100", "DEC1.1", "TRQ1.1=100", "TRQ1.1",
						"VEL1.1=100", "VEL1.1"};//, "POS1.1=8000", "POS2.1=0","MON.0"};
						//, "MPA1.1", "MPA2.1"};

	for(i = 0; i < ARRAY_COUNT(cmd_data); i++){
		waitMs(100);
		quail_command(0x20+i,1,CMD_TEXT,cmd_data[i],tmp,sizeof(tmp));
	}
}

/***************************************************************
 *
 * 以下はｱﾌﾟﾘ動作用の定義
 *
 *
 */

////////////////////////////////////////////////////////////////
void led_task(void *params)
{
	while(1){
		rtos_delay(200);

		LED1 = !LED1;
	}
}

///////////////////////////////////////////
//
// 受信したﾊﾟｹｯﾄの確認
//   ﾃﾞｰﾀは受信割り込みでﾊﾞｯﾌｧに格納される
//
int check_recv(u_char *buff)
{
	unsigned short sz;
	uint8_t		endCode;
	int			i;
	int 		endFlag;
	int 		sum;
	uint8_t		xor;

	//先頭が0xAA(1msのｲﾝﾀｰﾊﾞﾙで0xAAを10回確認)
	endFlag = 0;
	for(i = 0; i < 10; i++){
		if( buff[0] == 0xAA ){
			endFlag = 1;
			break;
		}
		rtos_delay(1);
	}
	if( endFlag == 0 ){
		//先頭の0xAAが確認できなかった
		return -1;
	}

	rtos_delay(5);

	//受信ﾃﾞｰﾀのｻｲｽﾞ
	sz =  (buff[QUAIL_SIZE_HI_OFFSET] << 8) + buff[QUAIL_SIZE_LO_OFFSET];

	//終了ｺｰﾄﾞ(0x55)を確認する(50msのｲﾝﾀｰﾊﾞﾙで確認)
	endFlag = 0;
	for(i = 0; i < 10; i++){
		rtos_delay(10);
		endCode = buff[sz-1];
		if( endCode == 0x55 ){
			endFlag = 1;
			break;
		}
	}
	if( endFlag == 0 ){
		//終了ｺｰﾄﾞが確認できなかった
		return -2;
	}
	sum = get_checksum(buff,sz-2);
	xor = buff[sz-2];
	if( sum != xor ){
		//ﾁｪｯｸｻﾑｴﾗｰ
		return -3;
	}
	return 0;
}

/////////////////////////////////////////////////////
//
// Quailからの応答で ****=???? の ???? 部分を取り出す
//   終端が <CR> + <LF> + NULL なので <CR> までで区切る
//
char* get_cmd_data(char *retStr)
{
	int i;

	for(i = 0; retStr[i] && retStr[i] != '='; i++);

	if( retStr[i] == '\0' ){
		//'='が見つからなかったら先頭
	}	return retStr;

	return( &retStr[i+1] );
}

/////////////////////////////////////////////////////
//
// 文字で定義された16進の値を数値へ変換する
//   A03D --> 41021
//
u_short htous(char *s)
{
	u_short sum = 0;

	for(int i = 0; s[i] && s[i] != 0x0A && s[i] != 0x0D; i++){
		sum *= 16;
		if( s[i] >= '0' && s[i] <= '9' ){
			sum += (s[i] - '0');
		}
		else{
			switch(s[i]){
			case 'A': case 'a': sum += 10; break;
			case 'B': case 'b': sum += 11; break;
			case 'C': case 'c': sum += 12; break;
			case 'D': case 'd': sum += 13; break;
			case 'E': case 'e': sum += 14; break;
			case 'F': case 'f': sum += 15; break;
			}
		}
	}
	return( sum );
}

///
int stat_check(u_short stat, u_short mask)
{
	return( (stat & mask) ? 1 : 0 );
}

////////////////////////////////////////
/// Quail にｴﾗｰがあるか
int is_stat_error(u_short stat)
{
	return( stat_check(stat,QUAIL_STAT_ERROR) );
}
///////////////////////////////////////
// ﾓｰﾀｰがONしているか
int is_motor_pow(u_short stat)
{
	return( stat_check(stat,QUAIL_STAT_MOTOR_ON) );	
}
///////////////////////////////////////
// 原点復帰を完了しているか
int is_init_pos(u_short stat)
{
	return( stat_check(stat,QUAIL_STAT_INIT_POS) );	
}
//////////////////////////////////////
// 移動動作が完了しているか
int is_move_done(u_short stat)
{
	return( stat_check(stat,QUAIL_STAT_MOVE_COMP) );
}

/////////////////////////////////////////////////////////////////////
//
// Quail に ｺﾏﾝﾄﾞを送信して応答を得る
//
int quail_get_command(int cmdNo, char *cmdStr, u_char *recv_tmp, int recvSize)
{
	quail_command(cmdNo,1,CMD_TEXT,cmdStr,recv_tmp,recvSize);

	rtos_delay(50);

	if( check_recv(recv_tmp) < 0 ){
		//受信したﾃﾞｰﾀｴﾗｰ
		return -1;
	}
	else{
		uint8_t cmdNo = recv_tmp[QUAIL_CMD_OFFSET];
		if( cmdNo != CMD_TEXT && cmdNo != CMD_UPLOAD ){
			return -2;
		}
	}
	return 0;
}

Event	__servoEvent;
extern Event	__enDatEvent;

/////////////////////////////////////////////////////////
//
// ｽﾃｰﾀｽ値の取得 "GET12.1" ｺﾏﾝﾄﾞ
//   (GET12.<軸番号>) 戻り値は GET12.1=####
//   #### は16進の値(文字列)
//
u_short quail_status(int cmdNo)
{
	u_char	recv_tmp[64];
	int		ret;

	memset(recv_tmp,0,sizeof(recv_tmp));

	if( (ret = quail_get_command(cmdNo,"GET12.1",recv_tmp,sizeof(recv_tmp))) < 0 ){
		return 0;
	}

	char *pStr = get_cmd_data(&recv_tmp[QUAIL_DATA_OFFSET]);

	u_short v = htous(pStr);
	return v;
}

void quail_nop(int cmdNo)
{
	u_char tmp[64];

	quail_command(cmdNo,1,CMD_NOP,"",tmp,sizeof(tmp));

	rtos_delay(50);
	check_recv(tmp);
}

/////////////////////////////////////////////////////////
//
// 現在値の取得 "GET13.1"ｺﾏﾝﾄﾞ
//   戻り値は NowPos.1=####
//
int quail_pos(int cmdNo)
{
	u_char	recv_tmp[64];
	int		ret;

	memset(recv_tmp,0,sizeof(recv_tmp));

	if( (ret = quail_get_command(cmdNo,"GET13.1",recv_tmp,sizeof(recv_tmp))) < 0 ){
		return ret;
	}

	return 0;
}

//////////////////////////////////////////////////////////
//
// Quail ｽﾃｰﾀｽ監視ﾀｽｸ
//
void quail_stat_task(void *pParams)
{
	u_long	recvVal;
	u_short stat;
	int		cmdNo = 0;

	//ｺﾝﾊﾟｲﾙの警告回避用
	(void)pParams;

	while(1){
		rtos_wait_queue(__quailQu,&recvVal,WAIT_FOREVER);	
		int nn = 1;
		while(nn){
			stat = quail_status(cmdNo);
			if( (stat & 3) == recvVal ){
			}
			else{
				if( is_move_done(stat) ){
					//位置決め完了ﾋﾞｯﾄが ON
					nn = 0;
					rtos_post_event_bit(__servoEvent,1);
				}
				if( is_stat_error(stat) ){
					//ｴﾗｰﾋﾞｯﾄが ON
					nn = 0;
					quail_nop(cmdNo+1);
				}
			}
			cmdNo = (cmdNo+1 > 255) ? 0 : cmdNo+1;
		}
	}
}

uint64_t ltoll(uint32_t up, uint32_t lo)
{
	uint64_t m = up;

	return( (m << 32) + lo);	
}

/////////////////////////////////////////////////////////////////
//
// Quail に移動指示を行うﾀｽｸ
//
void	quail_cmd_task(void *pParams)
{
	u_long	sendValue = QUAIL_CMD_GET;
	char 	cmd_str[64];
	char 	tmp[64];
	uint32_t	endat_upper, endat_lower;
	uint64_t	e_pos, e_pos2;

	unsigned long pos[] = { 100.0, 0.0};

	__servoEvent = rtos_create_event();

	quail_command(100,1,CMD_TEXT,"MON.0",tmp,sizeof(tmp));

	//ｴﾝｺｰﾀﾞｰの現在値
	endat_pos(&endat_upper, &endat_lower);
	e_pos = ltoll(endat_upper,endat_lower);

	while(1){
		rtos_delay(1000);
		for(int i = 0; i < ARRAY_COUNT(pos); i++){
			sprintf(cmd_str,"POS1.1=%d,MPA1.1",MM_TO_PULSE(pos[i]));

			//現在の移動ｲﾝﾃﾞｯｸｽ値を取得
			u_short stat = quail_status(100+i);
			quail_command(i,0xFE,CMD_TEXT,cmd_str,tmp,sizeof(tmp));

			sendValue = (stat & 0x3);
			rtos_send_queue(__quailQu,&sendValue,0);

			//監視するﾘﾆｱの番地を設定
			__endat_pos = e_pos + MM_TO_SCALE(pos[i]);

			//Quailの位置決め完了を待つ
			rtos_wait_event_bit(__servoEvent,1,EV_MODE_AND,WAIT_FOREVER);

			//ﾘﾆｱの番地監視を開始
			rtos_send_queue(__enDatQu,&sendValue,0);

			for(int sub = 1; ; sub++){
				//ﾘﾆｱの一致待ち
				if( rtos_wait_event_bit_continue(__servoEvent,2,EV_MODE_AND,10) != 0 ){
					//一致ｲﾍﾞﾝﾄ
					break;
				}
				//ﾘﾆｱの番地を読み込む
				endat_pos(&endat_upper, &endat_lower);
				e_pos2 = ltoll(endat_upper,endat_lower);

				int32_t m = __endat_pos - e_pos2;

				//ﾘﾆｱズレをQuailのﾊﾟﾙｽ数へ
				sprintf(cmd_str,"POS2.1=%d,MPI2.1",SCALE_TO_PULSE(m));
				quail_command(i+sub,0xFE,CMD_TEXT,cmd_str,tmp,sizeof(tmp));
			}

			LED0 = !LED0;
		}
	}	
}

void endat_test_task(void *pParams)
{
	u_long	sendValue = QUAIL_CMD_GET;
	char 	cmd_str[64];
	char 	tmp[64];

	__servoEvent = rtos_create_event();
	uint64_t pos[] = {	19360000,
						20360000,
						21360000,
						22360000,
						23360000};

	for(int i = 0; i < ARRAY_COUNT(pos); i++){
		__endat_pos = pos[i];
		rtos_send_queue(__enDatQu,&sendValue,0);

		rtos_wait_event_bit(__servoEvent,1,EV_MODE_AND,WAIT_FOREVER);

		LED0 = !LED0;
	}
	while(1);
}

extern void create_endat_task(void);

void create_quail_task(void)
{
	//
	__quailQu = rtos_create_queue(1,sizeof(uint32_t));

	create_endat_task();

	if( __enDatQu == 0 ){
		__enDatQu = rtos_create_queue(1,sizeof(uint32_t));
	}

	if( __quailQu != NULL ){
		xTaskCreate( quail_stat_task,"QuailStat",300,0,tskIDLE_PRIORITY+1,NULL);
		xTaskCreate( quail_cmd_task, "QuailCmd", 300,0,tskIDLE_PRIORITY+2,NULL);
		//xTaskCreate( endat_test_task,"Quail_Send",300,0,tskIDLE_PRIORITY+2,NULL);

		xTaskCreate( led_task,"LED",100,0,tskIDLE_PRIORITY+2,NULL);
	}
}

//////////////////////////////////////////////////////////

SEM_ID __semId;

void sem_send(void *param)
{
	while(1) {
		rtos_sem_give(__semId);
		rtos_delay(1000);
	}
}
void sem_recv(void *param)
{
	while(1){
		rtos_sem_take(__semId,WAIT_FOREVER);
		LED0 = !LED0;	
	}
}
void sem_test()
{
	__semId = rtos_create_semaphore();

	xTaskCreate( sem_send, "Wait", 300,0,tskIDLE_PRIORITY+2,NULL);
	xTaskCreate( sem_recv, "Recv", 300,0,tskIDLE_PRIORITY+2,NULL);
}
