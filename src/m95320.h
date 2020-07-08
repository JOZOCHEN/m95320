/**
 * @file m95320.h
 * @author jozochen (jozocyz@hotmail.com)
 * @brief 
 * @date 2020-02-10
 * @copyright Apache License 2.0
 *            jozochen (jozocyz@hotmail.com) Copyright (c) 2020
 */

#ifndef _M95320_H
#define _M95320_H

#define M95320_BUFFER_MAX (256)

typedef union{
    struct{
        char WIP    :1;
        char WEL    :1;
        char BP0    :1;
        char BP1    :1;
        char RSVD   :3;
        char SRWD   :1;
    }bits;
    char byte;
}m95320_status_reg;

extern int m95320_wren(void);
extern int m95320_wrdi(void);
extern int m95320_rdsr(m95320_status_reg* reg);
extern int m95320_wrsr(m95320_status_reg* reg);
extern int m95320_read(unsigned short addr, unsigned short len, char* rx_buffer);
extern int m95320_write(unsigned short addr, unsigned short len, char* tx_buffer);
extern int m95320_init(void);
extern void m96320_test(void);

#endif
