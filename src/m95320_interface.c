/**
 * @file m95320_interface.c
 * @author jozochen (jozocyz@hotmail.com)
 * @brief 
 * @date 2020-02-10
 * @copyright Apache License 2.0
 *            jozochen (jozocyz@hotmail.com) Copyright (c) 2020
 */

#include "m95320_interface.h"
#include "spi_init.h"
#include "port_init.h"

int m95320_spi_exchange(char* tx_buffer, char* rx_buffer, unsigned short len)
{
    int ret = 0;
    int wait_cnt = 1000;

    if( (tx_buffer == NULL_PTR) || (rx_buffer == NULL_PTR) || (len == 0)){
        ret = -1;
    }else{
        
        wait_cnt = 1000;
        while ( (IfxQspi_SpiMaster_getStatus(&spi_objs[SPI_CHANNEL_M95320].channel)
            == SpiIf_Status_busy) && (wait_cnt--) ) {}
            
        if(wait_cnt == 0){
            ret = -1;
        }else{
            if(IfxQspi_SpiMaster_exchange(&spi_objs[SPI_CHANNEL_M95320].channel, tx_buffer,
                rx_buffer, len) != SpiIf_Status_ok){
                    ret = -1;
                }
        }   

        wait_cnt = 1000;
        while ( (IfxQspi_SpiMaster_getStatus(&spi_objs[SPI_CHANNEL_M95320].channel)
            == SpiIf_Status_busy) && (wait_cnt--) ) {}     
    }
    
    return ret;
}

int m95320_set_pin_wp(int status)
{
    return (int)Port_set(M95320_WP, (PortStatus)status);
}