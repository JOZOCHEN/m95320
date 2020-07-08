/**
 * @file m95320_interface.h
 * @author jozochen (jozocyz@hotmail.com)
 * @brief 
 * @date 2020-02-10
 * @copyright Apache License 2.0
 *            jozochen (jozocyz@hotmail.com) Copyright (c) 2020
 */

#ifndef _M95320_INTERFACE_H
#define _M95320_INTERFACE_H

#ifndef NULL_PTR
#define NULL_PTR ((void*)0)
#endif

extern int m95320_spi_exchange(char* tx_buffer, char* rx_buffer, unsigned short len);
extern int m95320_set_pin_wp(int status);
#endif
