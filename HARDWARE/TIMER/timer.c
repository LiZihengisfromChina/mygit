#include "timer.h"
#include "led.h"
#include "usart.h"
#include "rgb.h"
#include "key.h"
#include "string.h"
//////////////////////////////////////////////////////////////////////////////////	 
 
//��ʱ�� ��������	   
//STM32F4����-�⺯���汾
//�Ա����̣�http://mcudev.taobao.com									  
////////////////////////////////////////////////////////////////////////////////// 	 


//ͨ�ö�ʱ��3�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///ʹ��TIM3ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
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


//��ʱ��3�жϷ�����
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�
	{
		delay_ms(3);
		//printf("USART_RX_BUF����Before��%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", USART_RX_BUF[0],USART_RX_BUF[1],USART_RX_BUF[2],USART_RX_BUF[3],USART_RX_BUF[4],USART_RX_BUF[5],USART_RX_BUF[6],USART_RX_BUF[7],USART_RX_BUF[8],USART_RX_BUF[9],USART_RX_BUF[10]);

		r_usb = (USART_RX_BUF[0]-48) * 100 + (USART_RX_BUF[1]-48) * 10 + (USART_RX_BUF[2]-48);
//		printf("ASCLL->NUM1��%d\n", r_usb);
		g_usb = (USART_RX_BUF[4]-48) * 100 + (USART_RX_BUF[5]-48) * 10 + (USART_RX_BUF[6]-48);
//		printf("ASCLL->NUM2��%d\n", g_usb);
		b_usb = (USART_RX_BUF[8]-48) * 100 + (USART_RX_BUF[9]-48) * 10 + (USART_RX_BUF[10]-48);
//		printf("ASCLL->NUM3��%d\n", b_usb);	
		
		if(((USART_RX_BUF[0]-48) * 100 + (USART_RX_BUF[1]-48) * 10 + (USART_RX_BUF[2]-48)) < 0)
		{
			r_usb = 0; g_usb = 0; b_usb = 0;
		}
		else
		{	
			const RGBColor_TypeDef BlueViolet3 = {r_usb, g_usb, b_usb};
			if(run_label==0) RGB_BlueViolet_test(60,BlueViolet3);			// �Զ��������ɫ
			run_label=1;    //�ر��Զ�����鷢�ͺ���
			led_lab=0;    //�Զ��幦�ܺ�������
		}
		memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));    //�������USART_RX_BUF�е���������
		//printf("USART_RX_BUF����after��%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", USART_RX_BUF[0],USART_RX_BUF[1],USART_RX_BUF[2],USART_RX_BUF[3],USART_RX_BUF[4],USART_RX_BUF[5],USART_RX_BUF[6],USART_RX_BUF[7],USART_RX_BUF[8],USART_RX_BUF[9],USART_RX_BUF[10]);

		if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			printf("\r\n�����͵���ϢΪ:\r\n");
			fun_label=1;    //�ر��������еĵƹ⺯��
			run_label=0;    //�Զ��������ɫ�źſ���һ�η���
			delay_ms(1);    //�ȴ������߼�����ֹͣ
			for(t=0;t<len;t++)
			{
				USART_SendData(USART1, USART_RX_BUF[t]);         //�򴮿�1��������
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
			}
			// printf("\r\n\r\n");//���뻻��
			USART_RX_STA=0;
		}else
		{
			times++;
			if(times%100==0)
			{
				printf("\r\n mcudev STM32F407������ ����ʵ��\r\n");
				
				// printf("mcudev.taobao.com\r\n\r\n\r\n");
			}
			// if(times%200==0)printf("����������,�Իس�������\r\n");  
//			if(times%30==0)LED1=!LED1;//��˸LED,��ʾϵͳ��������.
		}
//		key = KEY_Scan(0);
//		if(KEY0 == 0)
//			labe_key =1;
//		if(KEY1 == 0)
//			labe_key =2;
//		if(KEY2 == 0)
//			labe_key =0;
//		printf("��ʱ�ж�\n");
//		printf("fun_label��%d\n", fun_label);
		LED2=!LED2;//D2��ת
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
}




























