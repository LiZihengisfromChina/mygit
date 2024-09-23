#include "timer.h"
#include "led.h"
#include "usart.h"
#include "rgb.h"
#include "key.h"
#include "string.h"
//////////////////////////////////////////////////////////////////////////////////	 
 
//定时器 驱动代码	   
//STM32F4工程-库函数版本
//淘宝店铺：http://mcudev.taobao.com									  
////////////////////////////////////////////////////////////////////////////////// 	 


//通用定时器3中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM3,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

u8 t;
u8 i;
u8 len;
u8 buffer[200];
u16 times=0;
extern u16 fun_label;
extern u8 led_lab;
u32 r_usb=0;
u32 g_usb=0;
u32 b_usb=0;
u32 run_label=0;
u16 key;


//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
		delay_ms(3);
		//printf("USART_RX_BUF——Before：%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", USART_RX_BUF[0],USART_RX_BUF[1],USART_RX_BUF[2],USART_RX_BUF[3],USART_RX_BUF[4],USART_RX_BUF[5],USART_RX_BUF[6],USART_RX_BUF[7],USART_RX_BUF[8],USART_RX_BUF[9],USART_RX_BUF[10]);

		r_usb = (USART_RX_BUF[0]-48) * 100 + (USART_RX_BUF[1]-48) * 10 + (USART_RX_BUF[2]-48);
//		printf("ASCLL->NUM1：%d\n", r_usb);
		g_usb = (USART_RX_BUF[4]-48) * 100 + (USART_RX_BUF[5]-48) * 10 + (USART_RX_BUF[6]-48);
//		printf("ASCLL->NUM2：%d\n", g_usb);
		b_usb = (USART_RX_BUF[8]-48) * 100 + (USART_RX_BUF[9]-48) * 10 + (USART_RX_BUF[10]-48);
//		printf("ASCLL->NUM3：%d\n", b_usb);	
		
		if(((USART_RX_BUF[0]-48) * 100 + (USART_RX_BUF[1]-48) * 10 + (USART_RX_BUF[2]-48)) < 0)
		{
			r_usb = 0; g_usb = 0; b_usb = 0;
		}
		else
		{	
			const RGBColor_TypeDef BlueViolet3 = {r_usb, g_usb, b_usb};
			if(run_label==0) RGB_BlueViolet_test(60,BlueViolet3);			// 自定义灯珠颜色
			run_label=1;    //关闭自定义灯珠发送函数
			led_lab=0;    //自定义功能函数归零
		}
		memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));    //清除数组USART_RX_BUF中的所有数据
		//printf("USART_RX_BUF——after：%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", USART_RX_BUF[0],USART_RX_BUF[1],USART_RX_BUF[2],USART_RX_BUF[3],USART_RX_BUF[4],USART_RX_BUF[5],USART_RX_BUF[6],USART_RX_BUF[7],USART_RX_BUF[8],USART_RX_BUF[9],USART_RX_BUF[10]);

		if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			printf("\r\n您发送的消息为:\r\n");
			fun_label=1;    //关闭主函数中的灯光函数
			run_label=0;    //自定义灯珠颜色信号开启一次发送
			delay_ms(1);    //等待灯珠逻辑函数停止
			for(t=0;t<len;t++)
			{
				USART_SendData(USART1, USART_RX_BUF[t]);         //向串口1发送数据
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
			}
			// printf("\r\n\r\n");//插入换行
			USART_RX_STA=0;
		}else
		{
			times++;
			if(times%100==0)
			{
				printf("\r\n mcudev STM32F407开发板 串口实验\r\n");
				
				// printf("mcudev.taobao.com\r\n\r\n\r\n");
			}
			// if(times%200==0)printf("请输入数据,以回车键结束\r\n");  
//			if(times%30==0)LED1=!LED1;//闪烁LED,提示系统正在运行.
		}
//		key = KEY_Scan(0);
//		if(KEY0 == 0)
//			labe_key =1;
//		if(KEY1 == 0)
//			labe_key =2;
//		if(KEY2 == 0)
//			labe_key =0;
//		printf("定时中断\n");
//		printf("fun_label：%d\n", fun_label);
		LED2=!LED2;//D2翻转
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
}




























