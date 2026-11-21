//
// Created by 74943 on 2026/11/17.
//

#ifndef TEST04_LED_H
#define TEST04_LED_H

#include "gpio.h"
#include "main.h"
#include <stdint.h>

uint8_t LED_Get_State(void);
uint8_t LED_Get_Dir(void);
uint16_t LED_Get_Step(void);
uint8_t LED_Get_Pos(void);


void LED_Task(void);
void LED_Key_ToggleRun(void);
void LED_Key_ToggleDir(void);
void LED_Key_Reset(void);

void LED_Set_State(uint8_t run, uint8_t dir, uint16_t step_ms);
#endif //TEST04_LED_H
