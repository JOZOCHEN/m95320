/**
 * @file m95320.c
 * @author jozochen (jozocyz@hotmail.com)
 * @brief 
 * @date 2020-02-10
 * @copyright Apache License 2.0
 *            jozochen (jozocyz@hotmail.com) Copyright (c) 2020
 */

#include "m95320_interface.h"
#include "m95320.h"

/**instructions**/
#define M95320_INSTRUCTION_WREN         (0x06)
#define M95320_INSTRUCTION_WRDI         (0x04)
#define M95320_INSTRUCTION_RDSR         (0x05)
#define M95320_INSTRUCTION_WRSR         (0x01)
#define M95320_INSTRUCTION_READ         (0x03)
#define M95320_INSTRUCTION_WRITE        (0x02)

#define M95320_DUMMY                    (0x00)
#define M95320_BUFFER_HEADER_LEN        (0X03)

typedef struct{
    char tx_buffer[M95320_BUFFER_MAX+M95320_BUFFER_HEADER_LEN];
    char rx_buffer[M95320_BUFFER_MAX+M95320_BUFFER_HEADER_LEN];
}m95320_obj;

static m95320_obj m95320;

int m95320_wren(void)
{
    char cmd = M95320_INSTRUCTION_WREN;
    char read_buffer = M95320_DUMMY;

    return m95320_spi_exchange(&cmd, &read_buffer, sizeof(cmd));
}

int m95320_wrdi(void)
{
    char cmd = M95320_INSTRUCTION_WRDI;
    char read_buffer = M95320_DUMMY;

    return m95320_spi_exchange(&cmd, &read_buffer, sizeof(cmd));
}

int m95320_rdsr(m95320_status_reg* reg)
{
    char cmd[2] = {M95320_INSTRUCTION_RDSR, M95320_DUMMY};
    char read_buffer[2] = {M95320_DUMMY, M95320_DUMMY};
    int ret = -1;

    if(reg == NULL_PTR){
        ret = -1;
    }else{
        ret = m95320_spi_exchange(&cmd[0], &read_buffer[0], sizeof(cmd));
        reg->byte = read_buffer[1];
    }

    return ret;
}

int m95320_wrsr(m95320_status_reg* reg)
{
    char cmd[2] = {M95320_INSTRUCTION_WRSR, M95320_DUMMY};
    char read_buffer[2] = {M95320_DUMMY, M95320_DUMMY};
    int ret = -1;

    if(reg == NULL_PTR){
        ret = -1;
    }else{
        m95320_set_pin_wp(1);
        ret = m95320_wren();
        if(ret == 0){
            cmd[1] = reg->byte;
            ret = m95320_spi_exchange(&cmd[0], &read_buffer[0], sizeof(cmd));
        }else{
            (void)m95320_wrdi();
        }   
        m95320_set_pin_wp(0);             
    }

    return ret;
}

int m95320_read(unsigned short addr, unsigned short len, char* rx_buffer)
{
    int ret = -1;
    unsigned short i = 0;

    if( (rx_buffer == NULL_PTR) || (len > M95320_BUFFER_MAX) ){
        ret = -1;
    }else{
        m95320.tx_buffer[0] = M95320_INSTRUCTION_READ;
        m95320.tx_buffer[1] = (char)(addr >> 8);
        m95320.tx_buffer[2] = (char)(addr & 0x00ff);
        ret = m95320_spi_exchange(m95320.tx_buffer, m95320.rx_buffer, len + M95320_BUFFER_HEADER_LEN);     
        if(ret == 0){
            for(i = 0; i < len; i++){
                rx_buffer[i] = m95320.rx_buffer[M95320_BUFFER_HEADER_LEN + i];
            }
        }     
    }

    return ret;
}

int m95320_write(unsigned short addr, unsigned short len, char* tx_buffer)
{
    int ret = -1;
    unsigned short i = 0;

    if( (tx_buffer == NULL_PTR) || (len > M95320_BUFFER_MAX) ){
        ret = -1;
    }else{
        m95320_set_pin_wp(1);
        ret = m95320_wren();
        if(ret == 0){
            m95320.tx_buffer[0] = M95320_INSTRUCTION_WRITE;
            m95320.tx_buffer[1] = (char)(addr >> 8);
            m95320.tx_buffer[2] = (char)(addr & 0x00ff);
            for(i = 0; i < len; i++){
                m95320.tx_buffer[M95320_BUFFER_HEADER_LEN + i] = tx_buffer[i];
            }
            ret = m95320_spi_exchange(m95320.tx_buffer, m95320.rx_buffer, len + M95320_BUFFER_HEADER_LEN);        
        }else{
            (void)m95320_wrdi();
        }
        m95320_set_pin_wp(0);
    }

    return ret;
}

int m95320_init(void)
{
    int ret = -1;
    m95320_status_reg config = {{0}};

    if(m95320_rdsr(&config) == 0){
        if( (config.bits.BP0 != 0) || (config.bits.BP1 != 0)){
            config.bits.BP0 = 0;
            config.bits.BP1 = 0;
            ret = m95320_wrsr(&config);
        }else{
            ret = 0;
        }
    }

    return ret;
}

void m96320_test(void)
{
    unsigned short addr_1 = 0x0f00, addr_2 = 0x0100; 
    static unsigned int data_1 = 0, data_2 = 0;

    m95320_read(addr_1, sizeof(data_1), (char *)&data_1);
    m95320_read(addr_2, sizeof(data_2), (char *)&data_2);

    if(data_1 > data_2){
        data_2 = data_1+1;
        m95320_write(addr_2, sizeof(data_2), (char *)&data_2);
    }else{
        data_1 = data_2+1;
        m95320_write(addr_1, sizeof(data_1), (char *)&data_1);        
    }    

    // m95320_read(addr_1, sizeof(data_1), (char *)&data_1);
    // data_1++;
    // m95320_write(addr_1, sizeof(data_1), (char *)&data_1);   

}
