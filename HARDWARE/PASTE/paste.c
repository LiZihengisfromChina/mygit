#include "paste.h"


uint32_t combine_16(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
    return ((uint32_t)a << 24) | ((uint32_t)b << 16) | ((uint32_t)c << 8) | d;
}



//int main(void) {
//  uint16_t hex_num = 0x1A2B;  // 16进制值
//  uint32_t dec_num = hex_to_dec(hex_num);
//  
//  // 在这里使用转换后的十进制值，如打印输出等
//}

int hex_to_decimal(char hex[]) {
    int decimal, place;
	int i = 3;
    decimal = 0;
    place = 1;
    for ( ; i >= 0; i--) {
        if (hex[i] >= '0' && hex[i] <= '9') {
            decimal += (hex[i] - '0') * place;
            place *= 16;
        }
        else if (hex[i] >= 'A' && hex[i] <= 'F') {
            decimal += (hex[i] - 'A' + 10) * place;
            place *= 16;
        }
        else if (hex[i] >= 'a' && hex[i] <= 'f') {
            decimal += (hex[i] - 'a' + 10) * place;
            place *= 16;
        }
    }

    return decimal;
}
