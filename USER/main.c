#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "rgb.h"
#include "timer.h"
#include "exti.h"
#include "key.h"
#include "w_24byj.h"

//����ͨ��ʵ�� -�⺯���汾
//STM32F4����-�⺯���汾
//�Ա����̣�http://mcudev.taobao.com	

extern u8 led_lab;
extern u16 key;
extern int labe_key;
u16 fun_label = 0;

void w24byj(void);
void led_red_warning(int unm);    // �쳣״̬��ʾ��
void led_red_up(int num);    // ����״̬�ƹ�
void led_red_down(int num);    // �ػ�״̬�ƹ�
void led_red_pragmatic(int num);    // ����״̬�ƹ�
void led_red_door(int num);    // ����״̬�ƹ�
void led_red_cloture(int num);    // �رղ���״̬�ƹ�
void led_red_ing(int num);    // �����״̬�ƹ�

// ��������������
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

	delay_init(168);		//��ʱ��ʼ�� 

	KEY_Init();             //������ʼ��

	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ� 	

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2

	uart_init(9600);	//���ڳ�ʼ��������Ϊ9600
	EXTIX_Init();       //�ⲿ�жϳ�ʼ��
	// ����WS2812B
    WS2812b_Configuration();		//WS2812B�ĳ�ʼ��Ҫ�ڶ�ʱ�ж�֮ǰ����Ȼ�ڶ�ʱ�ж��е�����غ���ʱ���г�ͻ
	TIM3_Int_Init(5000-1,8400-1);	//��ʱ��ʱ��84M����Ƶϵ��8400������84M/8400=10Khz�ļ���Ƶ�ʣ�����5000��Ϊ500ms  
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
//		if(led_lab == 8) breathingEffect(60, BlueViolet);   // ������Ч��
			delay_ms(2000);   		
	}
}

void w24byj(void)
{
	// ѡ��һ����������		
	if(labe_key == 1)
	{
		delay_ms(3);
		printf("labe_key��%d\n", labe_key);
		for(int i=0;i<1024*8;i++)
		{
			w1 = step_seq[step][0];
			w2 = step_seq[step][1];
			w3 = step_seq[step][2];
			w4 = step_seq[step][3];		
	// ���²�������
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
		printf("labe_key��%d\n", labe_key);
		for(int i=0;i<1024*4;i++)
		{
			w4 = step_seq[step][0];
			w3 = step_seq[step][1];
			w2 = step_seq[step][2];
			w1 = step_seq[step][3];		
	// ���²�������
	step = (step + 1) % STEPS;				
			delay_ms(3);
			if(labe_key!=2)
				break;
		}
	labe_key = 0;
	}
	
	if(labe_key == 0)    
	{
		GPIO_ResetBits(GPIOG,GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);//�������õ�
	}
}

void led_red_warning(int num)   // �쳣״̬��ʾ��
{
	while(1)
	{
		if(fun_label==1)
		{
			break;
		}
		if(led_lab!=1) break;
		RGB_RED(Pixel_S1_NUM);     // ȫ��������Ϊ��ɫ
		delay_ms(150);
		RGB_BLACK(Pixel_S1_NUM);    // �ر�ȫ������
		delay_ms(150);
	}

}

void led_red_up(int num)    // ����״̬�ƹ�
{
	while(1)
	{
		if(fun_label==1)
		{
			break;
		}
		if(led_lab!=2) break;
		RGB_BlueViolet(Pixel_S1_NUM);     // ȫ��������Ϊ������������ɫ
		delay_ms(1000);
		RGB_BLACK(Pixel_S1_NUM);    // �ر�ȫ������
		delay_ms(1000);
	}

}

void led_red_down(int num)    // �ػ�״̬�ƹ�
{
	while(1)
	{
		if(fun_label==1)
		{
			fun_label=0;
			break;
		}
		if(led_lab!=3) break;
		RGB_RED(Pixel_S1_NUM);     // ȫ��������Ϊ��ɫ
		delay_ms(1000);
		RGB_BLACK(Pixel_S1_NUM);    // �ر�ȫ������
		delay_ms(1000);
	}

}

void led_red_pragmatic(int num)    // ����״̬�ƹ�
{
	
	RGB_BlueViolet(Pixel_S1_NUM);     // ȫ��������Ϊ������������ɫ

}

void led_red_door(int num)    // ����״̬�ƹ�
{
	while(1)
	{
		if(fun_label==1)
		{
			fun_label=0;
			break;
		}
		if(led_lab!=5) break;
		RGB_BlueViolet(Pixel_S1_NUM);     // ȫ��������Ϊ��ɫ
		delay_ms(500);
		RGB_BLACK(Pixel_S1_NUM);    // �ر�ȫ������
		delay_ms(500);
	}

}

void led_red_cloture(int num)    // �رղ���״̬�ƹ�
{

	RGB_WHITE(Pixel_S1_NUM);     // ȫ��������Ϊ������������ɫ

}

void led_red_ing(int num)    // �����״̬�ƹ�
{
	while(1)
	{
		if(fun_label==1)
		{
			fun_label=0;
			break;
		}
		if(led_lab!=7) break;
		RGB_WHITE(Pixel_S1_NUM);     // ȫ��������Ϊ��ɫ
		delay_ms(500);
		RGB_BLACK(Pixel_S1_NUM);    // �ر�ȫ������
		delay_ms(500);
	}

}



























