#include "rgb.h"
#include "usart.h"
#include "delay.h"

// Some Static Colors
const RGBColor_TypeDef RED      = {100,0,0};
const RGBColor_TypeDef GREEN    = {0,100,0};
const RGBColor_TypeDef BLUE     = {0,0,100};
const RGBColor_TypeDef SKY      = {0,255,255};
const RGBColor_TypeDef MAGENTA  = {255,0,255};
const RGBColor_TypeDef YELLOW   = {255,255,0};
const RGBColor_TypeDef ORANGE   = {127,106,0};
const RGBColor_TypeDef BLACK    = {0,0,0};
const RGBColor_TypeDef WHITE    = {255,255,255};
const RGBColor_TypeDef PURPLE   = {65,105,225};
//const RGBColor_TypeDef BlueViolet   = {50,53,138};
//const RGBColor_TypeDef BlueViolet   = {0,0,139};
const RGBColor_TypeDef BlueViolet   = {0,0,55};

static u8 pixelBuffer[Pixel_S1_NUM][24];                     //����


/***********************************************************************************************
**     name: WS2812b_Configuration
** function:  WS2812B SPI DMA���߳�ʼ��
**parameter: void
************************************************************************************************/
/*****************************************
 ˵����
 SPI2��
 ���ţ�ʹ�õ���PB15���ţ���TFTLCD�µ�LCD BL
 ʱ�ӣ���������ͼ��SPI2��APB1(42MHz)��Ƶ����
 ****************************************/
void WS2812b_Configuration(void){
	
  GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
	DMA_InitTypeDef  DMA_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //ʹ��GPIOBʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);  //ʹ��SPI2ʱ��
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);   //DMA1ʱ��ʹ�� 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;                    //PB15���ù������	
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                  //���ù���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;            //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                  //����
  GPIO_Init(GPIOB, &GPIO_InitStructure);                        //��ʼ��

	GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_SPI2);        //PB15����Ϊ SPI2

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;    //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		                      //����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		                  //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		                        //����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	                        //����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		                          //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;		//42M/8=5.25M
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	                  //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	                            //CRCֵ����Ķ���ʽ
	SPI_Init(SPI2, &SPI_InitStructure);                                   //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���

	SPI_Cmd(SPI2, ENABLE);                                                //ʹ��SPI����

	DMA_DeInit(DMA1_Stream4);

	while (DMA_GetCmdStatus(DMA1_Stream4) != DISABLE){}                       //�ȴ�DMA������ 

  DMA_InitStructure.DMA_Channel = DMA_Channel_0;                            //ͨ��ѡ��
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&SPI2->DR;                //DMA�����ַ
  DMA_InitStructure.DMA_Memory0BaseAddr = (u32)pixelBuffer;                 //DMA �洢��0��ַ
		
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;                   //�洢��������ģʽ
  DMA_InitStructure.DMA_BufferSize = Pixel_S1_NUM * 24;                     //���ݴ����� 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;          //���������ģʽ
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                   //�洢������ģʽ
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;   //�������ݳ���:8λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;           //�洢�����ݳ���:8λ
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                             // ʹ����ͨģʽ 
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                     //�е����ȼ�
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;               //�洢��ͻ�����δ���
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;       //����ͻ�����δ���
  DMA_Init(DMA1_Stream4, &DMA_InitStructure);                               //��ʼ��DMA Stream
  
	SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE);	         // ʹ��SPI2��DMA���� 	
	DMA_Cmd(DMA1_Stream4, DISABLE);                            //�ر�DMA���� 	
	while (DMA_GetCmdStatus(DMA1_Stream4) != DISABLE){}	       //ȷ��DMA���Ա�����  		
	DMA_SetCurrDataCounter(DMA1_Stream4,Pixel_S1_NUM * 24);    //���ݴ�����  
	DMA_Cmd(DMA1_Stream4, ENABLE); 
  
  delay_ms(1);
  RGB_BLACK(Pixel_S1_NUM);                                   //RGB RESET
  delay_ms(1);
}

/***********************************************************************************************
**     name: rgb_SetColor
** function: �趨ĳ��RGB LED����ɫ
**parameter: void
**   return: void
************************************************************************************************/
void rgb_SetColor(u16 LedId, RGBColor_TypeDef Color){
  if (LedId >= Pixel_S1_NUM){
    printf("Error: Out of Range!\r\n");
    return; // Avoid overflow
  }
  
  for (u8 i = 0; i < 8; i++){
    pixelBuffer[LedId][i] = (Color.G & (1 << (7 - i))) ? CODE1 : CODE0;
    pixelBuffer[LedId][i + 8] = (Color.R & (1 << (7 - i))) ? CODE1 : CODE0;
    pixelBuffer[LedId][i + 16] = (Color.B & (1 << (7 - i))) ? CODE1 : CODE0;
  }
}

/***********************************************************************************************
** ���ƣ� rgb_SendArray
** ���ܣ� Ϊ RGB ����ϵ��������ɫ��
             RGBColor_TypeDef��ָ�� RGBColor_TypeDef �ṹ��ָ�룬�ýṹ���� RGB ���ص���ɫ������Ϣ��
**������void
** ���أ�void
************************************************************************************************/
// ��rgb_SendArray�����У����Կ�����Ӷ�DMA״̬�ĳ�ʼ��飺
void rgb_SendArray(void) {
  if (DMA_GetCmdStatus(DMA1_Stream4) != DISABLE) {
    DMA_Cmd(DMA1_Stream4, DISABLE);
    while (DMA_GetCmdStatus(DMA1_Stream4) != DISABLE) {}
  }

  DMA_SetCurrDataCounter(DMA1_Stream4, Pixel_S1_NUM * 24);
  DMA_Cmd(DMA1_Stream4, ENABLE);    // �ر�DMA����
  while (DMA_GetFlagStatus(DMA1_Stream4, DMA_FLAG_TCIF4) == RESET) {}    // �ȴ������ֹ
  DMA_ClearFlag(DMA1_Stream4, DMA_FLAG_TCIF4);    // ����жϱ�־λ
	  
}

/***********************************************************************************************
**     name: RGB_BlueViolet
** function: �趨��ɫΪBlueViolet
**parameter: Pixel_LEN ������
**   return: void
************************************************************************************************/
void RGB_BlueViolet(u16 Pixel_LEN){
	
  u16 i;
	
  for(i = 0; i < Pixel_LEN; i++){  
    rgb_SetColor(i,BlueViolet);
	}
  
  rgb_SendArray();

}

void RGB_BlueViolet_test(u16 Pixel_LEN, RGBColor_TypeDef a){
	
  u16 i;
	
  for(i = 0; i < Pixel_LEN; i++){  
    rgb_SetColor(i,a);
	}
  
  rgb_SendArray();

}

/***********************************************************************************************
**     name: RGB_RED
** function: �趨��ɫΪRED
**parameter: Pixel_LEN ������
**   return: void
************************************************************************************************/
void RGB_RED(u16 Pixel_LEN){
	
  u16 i;
	
  for(i = 0; i < Pixel_LEN; i++){  
    rgb_SetColor(i,RED);
	}
  
  rgb_SendArray();

}

/***********************************************************************************************
**     name: RGB_PURPLE
** function: �趨��ɫΪPURPLE
**parameter: Pixel_LEN ������
**   return: void
************************************************************************************************/
void RGB_PURPLE(u16 Pixel_LEN){
	
  u16 i;
	
  for(i = 0; i < Pixel_LEN; i++){  
    rgb_SetColor(i,PURPLE);
	}
  
  rgb_SendArray();
}
/***********************************************************************************************
**     name: RGB_SKY
** function: �趨��ɫΪSKY
**parameter: Pixel_LEN ������
**   return: void
************************************************************************************************/
void RGB_SKY(u16 Pixel_LEN){
	
  u16 i;
	
  for(i = 0; i < Pixel_LEN; i++){  
    rgb_SetColor(i,SKY);
	}
  
  rgb_SendArray();
}

/***********************************************************************************************
**     name: RGB_MAGENTA
** function: �趨��ɫΪMAGENTA
**parameter: Pixel_LEN ������
**   return: void
************************************************************************************************/
void RGB_MAGENTA(u16 Pixel_LEN){
	
  u16 i;
	
  for(i = 0; i < Pixel_LEN; i++){  
    rgb_SetColor(i,MAGENTA);
	}
  
  rgb_SendArray();
}

/***********************************************************************************************
**     name: RGB_ORANGE
** function: �趨��ɫΪORANGE
**parameter: Pixel_LEN ������
**   return: void
************************************************************************************************/
void RGB_ORANGE(u16 Pixel_LEN){
	
  u16 i;
	
  for(i = 0; i < Pixel_LEN; i++){  
    rgb_SetColor(i,ORANGE);
	} 
	
  rgb_SendArray();
}

/***********************************************************************************************
**     name: RGB_GREEN
** function: �趨��ɫΪGREEN
**parameter: Pixel_LEN ������
**   return: void
************************************************************************************************/
void RGB_GREEN(u16 Pixel_LEN){
	
  u16 i;
	
  for(i = 0; i < Pixel_LEN; i++){
    rgb_SetColor(i,GREEN);
	}

  rgb_SendArray();
}

/***********************************************************************************************
**     name: RGB_BLUE
** function: �趨��ɫΪBLUE
**parameter: Pixel_LEN ������
**   return: void
************************************************************************************************/
void RGB_BLUE(u16 Pixel_LEN){
	
  u16 i;
	
  for(i = 0; i < Pixel_LEN; i++){
    rgb_SetColor(i,BLUE);
	}
      for (u8 i = 0; i < Pixel_S1_NUM; i++) {
  printf("LED %d: ", i);
  for (u8 j = 0; j < 24; j++) {
    printf("%02X ", pixelBuffer[i][j]);
  }
  printf("\n");
}
  rgb_SendArray();
}

/***********************************************************************************************
**     name: RGB_YELLOW
** function: �趨��ɫΪYELLOW
**parameter: Pixel_LEN ������
**   return: void
************************************************************************************************/
void RGB_YELLOW(u16 Pixel_LEN){
	
  u16 i;
	
  for(i = 0; i < Pixel_LEN; i++){
    rgb_SetColor(i,YELLOW);
	}
        for (u8 i = 0; i < Pixel_S1_NUM; i++) {
  printf("LED %d: ", i);
  for (u8 j = 0; j < 24; j++) {
    printf("%02X ", pixelBuffer[i][j]);
  }
  printf("\n");
}
  rgb_SendArray();
}

/***********************************************************************************************
**     name: RGB_BLACK
** function: �ر�ָ����������
**parameter: Pixel_LEN ������
**   return: void
************************************************************************************************/
void RGB_BLACK(u16 Pixel_LEN){
	
  u16 i;
	
  for(i = 0; i < Pixel_LEN; i++){
  
    rgb_SetColor(i,BLACK);
	}
  
  rgb_SendArray();
}

/***********************************************************************************************
**     name: RGB_WHITE
** function: �趨��ɫΪWHITE
**parameter: Pixel_LEN ������
**   return: void
************************************************************************************************/
void RGB_WHITE(u16 Pixel_LEN){
	
  u16 i;
	
  for(i = 0; i < Pixel_LEN; i++){
    rgb_SetColor(i,WHITE);
    
	}
  rgb_SendArray();
}

/***********************************************************************************************
**     name: Colourful_Wheel
** function: ����ɫת��ΪGRB
**parameter: WheelPos ��ɫ��ֵ
**   return: RGBColor_TypeDef ��ɫGRB
************************************************************************************************/
RGBColor_TypeDef Colourful_Wheel(u8 WheelPos){
	
	RGBColor_TypeDef color;
  WheelPos = 255 - WheelPos;
  
  if(WheelPos < 85){
    color.R = 255 - WheelPos * 3;
    color.G = 0;
    color.B = WheelPos * 3;
		return color;
  }
  if(WheelPos < 170){
    WheelPos -= 85;
    color.R = 0;
    color.G = WheelPos * 3;
    color.B = 255 - WheelPos * 3;
		return color;
  }
  
  WheelPos -= 170;
  color.R = WheelPos * 3; 
  color.G = 255 - WheelPos * 3;
  color.B = 0;
  
  return color;  
}

/***********************************************************************************************
**     name: rainbowCycle
** function: �����ƹ���
**parameter: Pixel_LEN ������
**   return: void
************************************************************************************************/
void rainbowCycle(u16 Pixel_LEN){
	
  u16 i, j = 0;

  for(j = 0; j < 1023; j++){                                                   // 1 cycles of all colors on wheel
    for(i = 0; i < Pixel_LEN; i++){  
      rgb_SetColor(i,Colourful_Wheel(((i * 256 / Pixel_LEN) + j)&255));
		} 
    rgb_SendArray();
    delay_ms(100);
  }
}

/***********************************************************************************************
**     name: breathingEffect
** function: ʵ��ָ����ɫ�ĺ�����Ч��
**parameter: Pixel_LEN ������, color ָ����ɫ
**   return: void
************************************************************************************************/
void breathingEffect(u16 Pixel_LEN, RGBColor_TypeDef color){
    u16 i, j;
    RGBColor_TypeDef breathColor;

    for(j = 0; j < 256; j++){       // �Ӱ�����
        for(i = 0; i < Pixel_LEN; i++){
            breathColor.R = (color.R * j) / 255;
            breathColor.G = (color.G * j) / 255;
            breathColor.B = (color.B * j) / 255;
            rgb_SetColor(i, breathColor);
        }
        rgb_SendArray();
        delay_ms(10);               // ���ƺ����ٶ�
    }

    for(j = 255; j > 0; j--){       // ��������
        for(i = 0; i < Pixel_LEN; i++){
            breathColor.R = (color.R * j) / 255;
            breathColor.G = (color.G * j) / 255;
            breathColor.B = (color.B * j) / 255;
            rgb_SetColor(i, breathColor);
        }
        rgb_SendArray();
        delay_ms(10);               // ���ƺ����ٶ�
    }
}




