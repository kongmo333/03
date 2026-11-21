#include "key.h"

void Key_Init(Key_t *k, GPIO_TypeDef *port, uint16_t pin, uint8_t idle_level) {
    k->port         = port;
    k->pin          = pin;
    k->idle_level   = idle_level;
    k->last_level   = idle_level;
    k->stable_level = idle_level;
    k->last_change  = 0;
}

KeyEvent_t Key_Scan(Key_t *k, uint32_t now) {
    uint8_t raw = HAL_GPIO_ReadPin(k->port, k->pin);
    if (raw != k->last_level) {
        k->last_level  = raw;
        k->last_change = now;
        return KEY_EVENT_NONE;
    }

    if (now - k->last_change < KEY_DEBOUNCE_MS) {
        return KEY_EVENT_NONE;
    }

    if (k->stable_level != raw) {
        k->stable_level = raw;
        if (raw != k->idle_level) {
            return KEY_EVENT_PRESS;
        }
    }

    return KEY_EVENT_NONE;
}

