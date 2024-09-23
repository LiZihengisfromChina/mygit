#ifndef __W_24BYJ_H
#define __W_24BYJ_H
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
 
//LED驱动代码	   
//STM32F4工程-库函数版本
//淘宝店铺：http://mcudev.taobao.com									  
////////////////////////////////////////////////////////////////////////////////// 	



// 定义GPIO引脚
#define IN1_PIN GPIO_Pin_4
#define IN2_PIN GPIO_Pin_5
#define IN3_PIN GPIO_Pin_6
#define IN4_PIN GPIO_Pin_7
#define IN_PORT GPIOG

//void Delay(uint32_t time);
void GPIO_Config(void);
void Delay(uint32_t time);
void Delay_us(uint32_t time);

#define w1 PGout(4)	  // D1
#define w2 PGout(5)	// D2
#define w3 PGout(6)	  // D1
#define w4 PGout(7)	// D2

//void W24BYJ_Init(void);
 				    
#endif
