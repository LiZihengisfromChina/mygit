#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "rgb.h"
#include "timer.h"
#include "exti.h"
#include "key.h"
#include "w_24byj.h"

//串口通信实验 -库函数版本
//STM32F4工程-库函数版本
//淘宝店铺：http://mcudev.taobao.com	

extern u8 led_lab;
extern u16 key;
extern int labe_key;
u16 fun_label = 0;

void w24byj(void);
void led_red_warning(int unm);    // 异常状态警示灯
void led_red_up(int num);    // 开机状态灯光
void led_red_down(int num);    // 关机状态灯光
void led_red_pragmatic(int num);    // 待机状态灯光
void led_red_door(int num);    // 舱门状态灯光
void led_red_cloture(int num);    // 关闭舱门状态灯光
void led_red_ing(int num);    // 检测中状态灯光

// 定义电机步进序列
#define STEPS 8
uint8_t step_seq[STEPS][4] = {
    {1, 0, 0, 0},
    {1, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 1, 0},
	{0, 0, 1, 0},
	{0, 0, 1, 1},
	{0, 0, 0, 1},
	{1, 0, 0, 1}
};
//uint8_t step_seq[STEPS][4] = {
//	{1, 0, 0, 1},
//    {1, 0, 0, 0},
//    {1, 1, 0, 0},
//    {0, 1, 0, 0},
//    {0, 1, 1, 0},
//	{0, 0, 1, 0},
//	{0, 0, 1, 1},
//	{0, 0, 0, 1}
//	
//};
uint32_t step = 0;
	
int main(void)
{

	delay_init(168);		//延时初始化 

	KEY_Init();             //按键初始化

	LED_Init();		  		//初始化与LED连接的硬件接口 	

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2

	uart_init(9600);	//串口初始化波特率为9600
	EXTIX_Init();       //外部中断初始化
	// 配置WS2812B
    WS2812b_Configuration();		//WS2812B的初始化要在定时中断之前，不然在定时中断中调用相关函数时会有冲突
	TIM3_Int_Init(5000-1,8400-1);	//定时器时钟84M，分频系数8400，所以84M/8400=10Khz的计数频率，计数5000次为500ms  
	GPIO_Config();

//	extern uint8_t step_seq[STEPS][4];
	while(1)
	{
		printf("USART_RX_BUF:%c\n", USART_RX_BUF[0]);
		printf("USART_RX_BUF:%c\n", USART_RX_BUF[1]);
		printf("USART_RX_BUF:%c\n", USART_RX_BUF[2]);
		printf("USART_RX_BUF:%c\n", USART_RX_BUF[3]);
		printf("USART_RX_BUF:%c\n\n", USART_RX_BUF[4]);
//		unsigned char str[] = "{\"cmd\":\"clean\",\n\"action\":\"ok\" }";
//		for(int i=0;i<=sizeof(str);i++)
//		{
//			printf("%c",str[i]);
//		}
		
		w24byj();
//        Delay_us(1000);
//		if(led_lab == 1) led_red_warning(60);
//		if(led_lab == 2) led_red_up(60);
//		if(led_lab == 3) led_red_down(60);
//		if(led_lab == 4) led_red_pragmatic(60);
//		if(led_lab == 5) led_red_door(60);
//		if(led_lab == 6) led_red_cloture(60);	
//		if(led_lab == 7) led_red_ing(60);	
//		if(led_lab == 8) breathingEffect(60, BlueViolet);   // 呼吸灯效果
			delay_ms(2000);   		
	}
}

void w24byj(void)
{
	// 选择一个步进序列		
	if(labe_key == 1)
	{
		delay_ms(3);
		printf("labe_key：%d\n", labe_key);
		for(int i=0;i<1024*8;i++)
		{
			w1 = step_seq[step][0];
			w2 = step_seq[step][1];
			w3 = step_seq[step][2];
			w4 = step_seq[step][3];		
	// 更新步进序列
	step = (step + 1) % STEPS;				
			delay_ms(3);
			if(labe_key!=1)
				break;
		}
	labe_key = 0;
	}
	
	if(labe_key == 2)
	{
		delay_ms(5);
		printf("labe_key：%d\n", labe_key);
		for(int i=0;i<1024*4;i++)
		{
			w4 = step_seq[step][0];
			w3 = step_seq[step][1];
			w2 = step_seq[step][2];
			w1 = step_seq[step][3];		
	// 更新步进序列
	step = (step + 1) % STEPS;				
			delay_ms(3);
			if(labe_key!=2)
				break;
		}
	labe_key = 0;
	}
	
	if(labe_key == 0)    
	{
		GPIO_ResetBits(GPIOG,GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);//引脚设置低
	}
}

void led_red_warning(int num)   // 异常状态警示灯
{
	while(1)
	{
		if(fun_label==1)
		{
			break;
		}
		if(led_lab!=1) break;
		RGB_RED(Pixel_S1_NUM);     // 全部灯珠设为红色
		delay_ms(150);
		RGB_BLACK(Pixel_S1_NUM);    // 关闭全部灯珠
		delay_ms(150);
	}

}

void led_red_up(int num)    // 开机状态灯光
{
	while(1)
	{
		if(fun_label==1)
		{
			break;
		}
		if(led_lab!=2) break;
		RGB_BlueViolet(Pixel_S1_NUM);     // 全部灯珠设为深紫罗兰的蓝色
		delay_ms(1000);
		RGB_BLACK(Pixel_S1_NUM);    // 关闭全部灯珠
		delay_ms(1000);
	}

}

void led_red_down(int num)    // 关机状态灯光
{
	while(1)
	{
		if(fun_label==1)
		{
			fun_label=0;
			break;
		}
		if(led_lab!=3) break;
		RGB_RED(Pixel_S1_NUM);     // 全部灯珠设为红色
		delay_ms(1000);
		RGB_BLACK(Pixel_S1_NUM);    // 关闭全部灯珠
		delay_ms(1000);
	}

}

void led_red_pragmatic(int num)    // 待机状态灯光
{
	
	RGB_BlueViolet(Pixel_S1_NUM);     // 全部灯珠设为深紫罗兰的蓝色

}

void led_red_door(int num)    // 舱门状态灯光
{
	while(1)
	{
		if(fun_label==1)
		{
			fun_label=0;
			break;
		}
		if(led_lab!=5) break;
		RGB_BlueViolet(Pixel_S1_NUM);     // 全部灯珠设为红色
		delay_ms(500);
		RGB_BLACK(Pixel_S1_NUM);    // 关闭全部灯珠
		delay_ms(500);
	}

}

void led_red_cloture(int num)    // 关闭舱门状态灯光
{

	RGB_WHITE(Pixel_S1_NUM);     // 全部灯珠设为深紫罗兰的蓝色

}

void led_red_ing(int num)    // 检测中状态灯光
{
	while(1)
	{
		if(fun_label==1)
		{
			fun_label=0;
			break;
		}
		if(led_lab!=7) break;
		RGB_WHITE(Pixel_S1_NUM);     // 全部灯珠设为白色
		delay_ms(500);
		RGB_BLACK(Pixel_S1_NUM);    // 关闭全部灯珠
		delay_ms(500);
	}

}



























