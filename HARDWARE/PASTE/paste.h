#ifndef __PASTE_H
#define __PASTE_H

#include "sys.h"
		
uint32_t combine_16(uint8_t a, uint8_t b, uint8_t c, uint8_t d);    // 把4个16进制数合成1个16进制数
int hex_to_decimal(char hex[]);    // 将16进制数改成10进制数


#endif /* __paste_H */
