#include "uart_protocol.h"
#include "led.h"
#include <string.h>
#include "usart.h"
#include <stdio.h>

extern UART_HandleTypeDef huart1;

#pragma pack(push, 1)
typedef struct {
    uint8_t head;
    uint8_t run;
    uint8_t dir;
    uint8_t step_l;
    uint8_t step_h;
    uint8_t check;
} UartPacket_t;
#pragma pack(pop)

static uint8_t rx_buffer[6];
static uint8_t rx_index = 0;
static UartPacket_t rx_packet;
static volatile uint8_t new_packet_received = 0;
static volatile uint8_t is_sending = 0;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART2) {
        if (rx_index == 0 && rx_buffer[0] != 0xAA) {
            HAL_UART_Receive_IT(&huart2, &rx_buffer[0], 1);
            return;
        }

        rx_index++;

        if (rx_index >= 6) {
            uint8_t calc_check = (rx_buffer[0] + rx_buffer[1] + rx_buffer[2] + rx_buffer[3] + rx_buffer[4]) % 256;

            if (calc_check == rx_buffer[5]) {
                memcpy(&rx_packet, rx_buffer, 6);
                new_packet_received = 1;
            }
            rx_index = 0;
        }
        HAL_UART_Receive_IT(&huart2, &rx_buffer[rx_index], 1);
    }
}

void UART_Protocol_Init(void) {
    rx_index = 0;
    HAL_UART_Receive_IT(&huart2, &rx_buffer[0], 1);
}

void UART_Protocol_Task(void) {
    if (new_packet_received) {
        new_packet_received = 0;

        uint16_t step = (rx_packet.step_h << 8) | rx_packet.step_l;
        if (step < 100) step = 100;
        if (step > 2000) step = 2000;

        if (rx_packet.run <= 1 && rx_packet.dir <= 1) {
            LED_Set_State(rx_packet.run, rx_packet.dir, step);
            UART_Send_Feedback();
        }
    }
}

void UART_Send_Feedback(void) {
    static char tx_buf[64];

    if (is_sending) return;

    uint8_t run  = LED_Get_State();
    uint8_t dir  = LED_Get_Dir();
    uint16_t step= LED_Get_Step();
    uint8_t pos  = LED_Get_Pos();

    int len = sprintf(tx_buf, "STATE=%s,DIR=%s,STEP=%d,LED=%d\r\n",
                      run ? "RUN" : "STOP",
                      dir ? "REV" : "FWD",
                      step,
                      pos);

    if (HAL_UART_Transmit_IT(&huart2, (uint8_t*)tx_buf, len) == HAL_OK) {
        is_sending = 1;
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART2)
    {
        is_sending = 0;
    }
    }