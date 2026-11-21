#include "led.h"

#include "stm32f1xx_hal_gpio.h"
typedef enum {
    LED_PAUSED = 0,
    LED_RUNNING,
} LED_State_t;

static LED_State_t led_state = LED_PAUSED;
static uint8_t     led_pos   = 0;
static int8_t      led_dir   = 1;
static uint32_t    t0;
static uint16_t    led_step_ms = 1000;

static const uint16_t led_pins[3] = {
    GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_11
};
#define LED_PORT     GPIOA

static void LED_AllOff(void)
{
    HAL_GPIO_WritePin(LED_PORT,
        GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11, GPIO_PIN_RESET);
};
static void LED_On(uint8_t pos)
{
    LED_AllOff();
    HAL_GPIO_WritePin(LED_PORT, led_pins[pos], GPIO_PIN_SET);
};

void LED_Key_ToggleRun(void) {
    if (led_state == LED_RUNNING) {
        led_state = LED_PAUSED;
    } else {
        led_state = LED_RUNNING;
        t0 = HAL_GetTick();
        LED_On(led_pos);
    }
}
void LED_Key_ToggleDir(void) {
    led_dir = -led_dir;
}
void LED_Task(void) {
    switch (led_state) {
    case LED_PAUSED:
        break;
    case LED_RUNNING:
        if (HAL_GetTick() - t0 >= led_step_ms) {
            led_pos = (uint8_t)((led_pos + 3 + led_dir) % 3);
            LED_On(led_pos);
            t0 = HAL_GetTick();
        }
        break;
    }
}
void LED_Set_State(uint8_t run, uint8_t dir, uint16_t step_ms) {
    led_step_ms = step_ms;
    led_dir = (dir == 0) ? 1 : -1;

    if (run == 1) {
        if (led_state != LED_RUNNING) {
            t0 = HAL_GetTick();
            LED_On(led_pos);
        }
        led_state = LED_RUNNING;
    } else {
        led_state = LED_PAUSED;
        LED_AllOff();
    }
}

uint8_t LED_Get_State(void) { return (led_state == LED_RUNNING) ? 1 : 0; }
uint8_t LED_Get_Dir(void)   { return (led_dir == 1) ? 0 : 1; }
uint16_t LED_Get_Step(void) { return led_step_ms; }
uint8_t LED_Get_Pos(void)   { return led_pos + 1; }

//
// Created by 74943 on 2026/11/17.
//
