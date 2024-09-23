#ifndef WS2812B_H
#define WS2812B_H

#include "sys.h"

#define LED_COUNT 60
#define LED_BUFFER_SIZE (LED_COUNT * 24 + 50)

void WS2812B_Init(void);
void WS2812B_SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t ledIndex);
void WS2812B_Send(void);

#endif // WS2812B_H
