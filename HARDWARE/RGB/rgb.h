#ifndef __RGB_H
#define __RGB_H

/* Includes ------------------------------------------------------------------*/
#include "sys.h"
#include "delay.h"



typedef struct
{
  u8 R;
  u8 G;
  u8 B;
} RGBColor_TypeDef;

//extern u8 pixelBuffer[][24];
extern const RGBColor_TypeDef RED ;
extern const RGBColor_TypeDef GREEN;
extern const RGBColor_TypeDef BLUE;
extern const RGBColor_TypeDef SKY;
extern const RGBColor_TypeDef MAGENTA ;
extern const RGBColor_TypeDef YELLOW ;
extern const RGBColor_TypeDef ORANGE;
extern const RGBColor_TypeDef BLACK;
extern const RGBColor_TypeDef WHITE;
extern const RGBColor_TypeDef PURPLE;
extern const RGBColor_TypeDef BlueViolet;
/* Exported constants --------------------------------------------------------*/
#define Pixel_S1_NUM 60		//���� RGB����

/**************************************
 ˵����

 WS2812B����Э��(��λ��ns)��
       min     typ     max
bit 0
 T0H:  220      -      420
 T0L:  750      -      1600
 
bit 1 
 T1H:  750      -      1600
 T1L:  220      -      420
 
 
 RESET: time > 300us


8��ƵAPB1��42MHz/8 = 5.25MHz
ʱ������Ϊ��1/5.25/1e6 = 1.90e-7=190ns
**************************************/
#define CODE0 0xC0 // 0��, ���͵�ʱ�� 1100 0000  ���ݲ�ͬ��SCK�ʵ�����
#define CODE1 0xFC // 1��, ���͵�ʱ�� 1111 1100

void WS2812b_Configuration(void);
void rgb_SetColor(u16 LedId, RGBColor_TypeDef Color);
/* Basic Color Effects -------------------------------------------------------*/
void RGB_BlueViolet(u16 Pixel_LEN);
void RGB_BlueViolet_test(u16 Pixel_LEN, RGBColor_TypeDef a);

void RGB_RED(u16 Pixel_LEN);
void RGB_GREEN(u16 Pixel_LEN);
void RGB_BLUE(u16 Pixel_LEN);
void RGB_YELLOW(u16 Pixel_LEN);
void RGB_MAGENTA(u16 Pixel_LEN);
void RGB_BLACK(u16 Pixel_LEN);
void RGB_WHITE(u16 Pixel_LEN);
void RGB_SKY(u16 Pixel_LEN);
void RGB_ORANGE(u16 Pixel_LEN);
void RGB_PURPLE(u16 Pixel_LEN);
/* Complicated Color Effects -------------------------------------------------*/
void rainbowCycle(u16 Pixel_LEN);
void rgb_SendArray(void);
void breathingEffect(u16 Pixel_LEN, RGBColor_TypeDef color);   // ������
#endif /* __WS2812_H */



