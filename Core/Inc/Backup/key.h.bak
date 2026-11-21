

#ifndef TEST04_KEY_H
#define TEST04_KEY_H

#include "main.h"
#include <stdint.h>

#define KEY_DEBOUNCE_MS   20

typedef enum {
    KEY_EVENT_NONE = 0,
    KEY_EVENT_PRESS,
} KeyEvent_t;

typedef struct {
    GPIO_TypeDef *port;
    uint16_t      pin;
    uint8_t       idle_level;
    uint8_t       last_level;
    uint8_t       stable_level;
    uint32_t      last_change;
} Key_t;

void Key_Init(Key_t *k, GPIO_TypeDef *port, uint16_t pin, uint8_t idle_level);
KeyEvent_t Key_Scan(Key_t *k, uint32_t now);
#endif //TEST04_KEY_H
