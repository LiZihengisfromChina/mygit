#include "w_24byj.h"
// 步进电机配置

#include "stm32f4xx.h"


void GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 启用GPIO时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
  
    // 配置GPIO引脚为输出模式
    GPIO_InitStructure.GPIO_Pin = IN1_PIN | IN2_PIN | IN3_PIN | IN4_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(IN_PORT, &GPIO_InitStructure);
}
void Delay(uint32_t time)
{
    while(time--);
}

void Delay_us(uint32_t time)
{
    // 简单的循环延时，time参数为微秒
    uint32_t count = time * (SystemCoreClock / 1000000 / 5);
    while(count--);
}






























