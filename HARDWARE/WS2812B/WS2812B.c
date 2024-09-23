#include "WS2812B.h"

static uint16_t ledBuffer[LED_BUFFER_SIZE];

static void TIM3_Init(void);
static void DMA_Init(void);
static void GPIO_Init(void);

void WS2812B_Init(void)
{
    HAL_Init();
    SystemClock_Config();
    GPIO_Init();
    TIM3_Init();
    DMA_Init();
}

static void GPIO_Init(void)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();
    
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

static void TIM3_Init(void)
{
    __HAL_RCC_TIM3_CLK_ENABLE();
    
    TIM_HandleTypeDef htim3;
    TIM_OC_InitTypeDef sConfigOC = {0};
    
    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 0;
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = 89; // 800kHz PWM signal (84MHz/90)
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    HAL_TIM_PWM_Init(&htim3);
    
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1);
    
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
}

static void DMA_Init(void)
{
    __HAL_RCC_DMA1_CLK_ENABLE();
    
    DMA_HandleTypeDef hdma_tim3_ch1;
    
    hdma_tim3_ch1.Instance = DMA1_Stream4;
    hdma_tim3_ch1.Init.Channel = DMA_CHANNEL_5;
    hdma_tim3_ch1.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_tim3_ch1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_tim3_ch1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_tim3_ch1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_tim3_ch1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_tim3_ch1.Init.Mode = DMA_NORMAL;
    hdma_tim3_ch1.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_tim3_ch1.Init.F
