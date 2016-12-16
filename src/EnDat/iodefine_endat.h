/********************************************************************************/
/*                                                                              */
/* Device     : RZ/T1                                                           */
/* File Name  : iodefine_endat.h                                                */
/* Abstract   : Definition of EnDat Register.                                   */
/* History    : V1.00  (2015-01-30)  [Hardware Manual Revision : 1.00]          */
/* Note       : This is a typical example.                                      */
/*                                                                              */
/*  Copyright(c) 2015 Renesas Electronics Corp.                                 */
/*                                                                              */
/********************************************************************************/
#ifndef IODEFINE_ENDAT_H
#define IODEFINE_ENDAT_H

#ifdef __ICCARM__
#pragma bitfields=disjoint_types
#pragma pack(1)
#endif /* __ICCARM__ */

struct st_endat0{
    union {
        unsigned long LONG;
        struct {
            unsigned long PI:16;
            unsigned long MAP:8;
            unsigned long MB:6;
            unsigned long :2;
        } BIT;
    } SEND;
    union {
        unsigned long LONG;
        struct {
            unsigned long :1;
            unsigned long DU:1;
            unsigned long DT:1;
            unsigned long :1;
            unsigned long FTCLK:4;
            unsigned long DWL:6;
            unsigned long RWIN:1;
            unsigned long ARST:1;
            unsigned long CPTU:8;
            unsigned long DLY:1;
            unsigned long :4;
            unsigned long RST:1;
            unsigned long :2;
        } BIT;
    } CFG1;
    union {
        unsigned long LONG;
        struct {
            unsigned long :8;
            unsigned long WDG:8;
            unsigned long TST:3;
            unsigned long :13;
        } BIT;
    } CFG2;
    union {
        unsigned long LONG;
        struct {
            unsigned long RX1:1;
            unsigned long ERR1:1;
            unsigned long CRC1:1;
            unsigned long FTYPE1:1;
            unsigned long FTYPE2:1;
            unsigned long MRSADR:1;
            unsigned long :2;
            unsigned long RX2:1;
            unsigned long RX3:1;
            unsigned long ERR2:1;
            unsigned long CRC3:1;
            unsigned long CRC2:1;
            unsigned long BUSY:1;
            unsigned long RM:1;
            unsigned long WRN:1;
            unsigned long :1;
            unsigned long WDG:1;
            unsigned long FTYPE3:1;
            unsigned long RXEND:1;
            unsigned long MBERR:1;
            unsigned long RDSTC:1;
            unsigned long :10;
        } BIT;
    } INTE;
    union {
        unsigned char BYTE;
        struct {
            unsigned char STRB:8;
        } BIT;
    } SWST;
    unsigned char dummyssai_w;
    unsigned short dummycptl_w;
    unsigned long dummystat_w;
    unsigned char dummy344[       8];
    unsigned char dummy000[   0x3e0];
    unsigned char dummy352[      17];
    unsigned char dummyssai_r;
    unsigned short dummycptl_rr;
    unsigned long dummystat_r;
    unsigned char dummy376[       8];
    unsigned char dummy001[   0x7e0];
    unsigned char dummy384[      17];
    unsigned char dummyssaiSSAI;
    union {
        unsigned short WORD;
        struct {
            unsigned short CPTL:16;
        } BIT;
    } CPTL;
    union {
        unsigned long LONG;
        struct {
            unsigned long RX1:1;
            unsigned long ERR1:1;
            unsigned long CRC1:1;
            unsigned long FTYPE1:1;
            unsigned long FTYPE2:1;
            unsigned long MRSADR:1;
            unsigned long :2;
            unsigned long RX2:1;
            unsigned long RX3:1;
            unsigned long ERR2:1;
            unsigned long CRC3:1;
            unsigned long CRC2:1;
            unsigned long BUSY:1;
            unsigned long RM:1;
            unsigned long WRN:1;
            unsigned long :1;
            unsigned long WDG:1;
            unsigned long FTYPE3:1;
            unsigned long RXEND:1;
            unsigned long MBERR:1;
            unsigned long RDSTC:1;
            unsigned long LZK:1;
            unsigned long LZM:1;
            unsigned long :6;
            unsigned long RDST:1;
            unsigned long :1;
        } BIT;
    } STAT;
    unsigned char  dummy408[       8];
    unsigned char  dummy002[0xbff5e0];
    unsigned char  dummy416[      32];
    unsigned char  dummy003[   0x3e0];
    union {
        unsigned long LONG;
        struct {
            unsigned long RXD1:32;
        } BIT;
    } RECV1L;
    union {
        unsigned long LONG;
        struct {
            unsigned long RXD1:16;
            unsigned long RXCRC1:5;
            unsigned long RXERR1:1;
            unsigned long RXERR2:1;
            unsigned long :9;
        } BIT;
    } RECV1U;
    union {
        unsigned long LONG;
        struct {
            unsigned long RXD2:24;
            unsigned long RXCRC2:5;
            unsigned long :3;
        } BIT;
    } RECV2;
    union {
        unsigned long LONG;
        struct {
            unsigned long RXD3:24;
            unsigned long RXCRC3:5;
            unsigned long :3;
        } BIT;
    } RECV3;
    union {
        unsigned long LONG;
        struct {
            unsigned long MINR:16;
            unsigned long MAJR:16;
        } BIT;
    } VER;
    unsigned char dummyinfo;
    unsigned char dummy469[      11];
    unsigned char dummy004[   0x7e0];
    unsigned char dummy480[      32];
    unsigned char dummynf[0xD329E];
    union {
        unsigned short WORD;
        struct {
            unsigned short :5;
            unsigned short INF:1;
            unsigned short :2;
            unsigned short NFINTV:4;
            unsigned short NFSCNT:4;
        } BIT;
    } NF;
};

struct st_endat1{
    union {
        unsigned long LONG;
        struct {
            unsigned long PI:16;
            unsigned long MAP:8;
            unsigned long MB:6;
            unsigned long :2;
        } BIT;
    } SEND;
    union {
        unsigned long LONG;
        struct {
            unsigned long :1;
            unsigned long DU:1;
            unsigned long DT:1;
            unsigned long :1;
            unsigned long FTCLK:4;
            unsigned long DWL:6;
            unsigned long RWIN:1;
            unsigned long ARST:1;
            unsigned long CPTU:8;
            unsigned long DLY:1;
            unsigned long :4;
            unsigned long RST:1;
            unsigned long :2;
        } BIT;
    } CFG1;
    union {
        unsigned long LONG;
        struct {
            unsigned long :8;
            unsigned long WDG:8;
            unsigned long TST:3;
            unsigned long :13;
        } BIT;
    } CFG2;
    union {
        unsigned long LONG;
        struct {
            unsigned long RX1:1;
            unsigned long ERR1:1;
            unsigned long CRC1:1;
            unsigned long FTYPE1:1;
            unsigned long FTYPE2:1;
            unsigned long MRSADR:1;
            unsigned long :2;
            unsigned long RX2:1;
            unsigned long RX3:1;
            unsigned long ERR2:1;
            unsigned long CRC3:1;
            unsigned long CRC2:1;
            unsigned long BUSY:1;
            unsigned long RM:1;
            unsigned long WRN:1;
            unsigned long :1;
            unsigned long WDG:1;
            unsigned long FTYPE3:1;
            unsigned long RXEND:1;
            unsigned long MBERR:1;
            unsigned long RDSTC:1;
            unsigned long :10;
        } BIT;
    } INTE;
    union {
        unsigned char BYTE;
        struct {
            unsigned char STRB:8;
        } BIT;
    } SWST;
    unsigned char dummyssai_w;
    unsigned short dummycptl_w;
    unsigned long dummystat_w;
    unsigned char dummy344[       8];
    unsigned char dummy000[   0x3e0];
    unsigned char dummy352[      17];
    unsigned char dummyssai_r;
    unsigned short dummycptl_rr;
    unsigned long dummystat_r;
    unsigned char dummy376[       8];
    unsigned char dummy001[   0x7e0];
    unsigned char dummy384[      17];
    unsigned char dummyssaiSSAI;
    union {
        unsigned short WORD;
        struct {
            unsigned short CPTL:16;
        } BIT;
    } CPTL;
    union {
        unsigned long LONG;
        struct {
            unsigned long RX1:1;
            unsigned long ERR1:1;
            unsigned long CRC1:1;
            unsigned long FTYPE1:1;
            unsigned long FTYPE2:1;
            unsigned long MRSADR:1;
            unsigned long :2;
            unsigned long RX2:1;
            unsigned long RX3:1;
            unsigned long ERR2:1;
            unsigned long CRC3:1;
            unsigned long CRC2:1;
            unsigned long BUSY:1;
            unsigned long RM:1;
            unsigned long WRN:1;
            unsigned long :1;
            unsigned long WDG:1;
            unsigned long FTYPE3:1;
            unsigned long RXEND:1;
            unsigned long MBERR:1;
            unsigned long RDSTC:1;
            unsigned long LZK:1;
            unsigned long LZM:1;
            unsigned long :6;
            unsigned long RDST:1;
            unsigned long :1;
        } BIT;
    } STAT;
    unsigned char  dummy408[       8];
    unsigned char  dummy002[0xbff5e0];
    unsigned char  dummy416[      32];
    unsigned char  dummy003[   0x3e0];
    union {
        unsigned long LONG;
        struct {
            unsigned long RXD1:32;
        } BIT;
    } RECV1L;
    union {
        unsigned long LONG;
        struct {
            unsigned long RXD1:16;
            unsigned long RXCRC1:5;
            unsigned long RXERR1:1;
            unsigned long RXERR2:1;
            unsigned long :9;
        } BIT;
    } RECV1U;
    union {
        unsigned long LONG;
        struct {
            unsigned long RXD2:24;
            unsigned long RXCRC2:5;
            unsigned long :3;
        } BIT;
    } RECV2;
    union {
        unsigned long LONG;
        struct {
            unsigned long RXD3:24;
            unsigned long RXCRC3:5;
            unsigned long :3;
        } BIT;
    } RECV3;
    union {
        unsigned long LONG;
        struct {
            unsigned long MINR:16;
            unsigned long MAJR:16;
        } BIT;
    } VER;
    unsigned char dummyinfo;
    unsigned char dummy469[      11];
    unsigned char dummy004[   0x7e0];
    unsigned char dummy480[      32];
    unsigned char dummynf[0x2D3260];
    union {
        unsigned short WORD;
        struct {
            unsigned short :5;
            unsigned short INF:1;
            unsigned short :2;
            unsigned short NFINTV:4;
            unsigned short NFSCNT:4;
        } BIT;
    } NF;
};

#define ENDAT0 (*(volatile struct st_endat0 *)0xB031C100)
#define ENDAT1 (*(volatile struct st_endat1 *)0xB011C100)

#ifdef __ICCARM__
#pragma bitfields=default
#pragma pack()
#endif /* __ICCARM__ */

#endif /* IODEFINE_ENDAT_H */
