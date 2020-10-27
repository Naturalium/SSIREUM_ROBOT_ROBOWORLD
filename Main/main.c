#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_dma.h>
#include <stm32f10x_adc.h>
#include <stm32f10x_tim.h>
#include <stm32f10x_usart.h>
uint32_t ADCConvertedValue[6];
#define ADC1_DR_Address    ((uint32_t)0x4001244C)



void FF()
{
  GPIO_ResetBits(GPIOB, GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_10 | GPIO_Pin_11);
  GPIO_SetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_10);
}

void BB()
{
  GPIO_ResetBits(GPIOB, GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_10 | GPIO_Pin_11);
  GPIO_SetBits(GPIOB, GPIO_Pin_5 | GPIO_Pin_11);
}

void FL()
{
  GPIO_ResetBits(GPIOB, GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_10 | GPIO_Pin_11);
  GPIO_SetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_11);
}


void FR()
{
  GPIO_ResetBits(GPIOB, GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_10 | GPIO_Pin_11);
  GPIO_SetBits(GPIOB, GPIO_Pin_5 | GPIO_Pin_10);
}




////////////////////////////////////////////

int L_ST = 1;
int R_ST = 2;
int F_ST = 3;
int R_CL = 13;
int F_CL = 14;
int L_CL = 15;
int L_CL2 = 16;
int R_CL2 = 17;
int IR_L = 5;
int IR_R = 6;
int IR_F = 7;
int IR_R2 = 8;
int IR_L2 = 9;
int NONE = 10;
int NONE1 = 11;
int NONE2 = 12;
int ST = 13;

////////////////////////////////////////////

#define PSDL_ST 1000                            //먼거리 기준값
#define PSDS_ST 1000                            //근거리 기준값
#define PSDF_ST 1000
#define PSDFLR_ST 800                            //장거리 정면
#define IR_ST 1000                                      //수광 기준값


////////////////////////////////////////////

#define PSDL_N 2000                             //먼거리 확정값
#define PSDF_N 2000
#define PSDS_N 2000                             //근거리 확정값

TIM_OCInitTypeDef TIM_OCInitStructure;



///////////////////////////////////////////////
void RCC_Configuration()
{
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
   RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
   
}

void DMA_Configuration()
{

  DMA_InitTypeDef DMA_InitStructure;
   /* DMA1 channel1 configuration ----------------------------------------------*/
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADCConvertedValue;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 6;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word ;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  
  /* Enable DMA1 channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);

}

void ADC_Configuration()
{
  ADC_InitTypeDef ADC_InitStructure;
  /* ADC1 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 6;
  ADC_Init(ADC1, &ADC_InitStructure);

  ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_55Cycles5); // PSD1
  ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 2, ADC_SampleTime_55Cycles5); // PSD2
  ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 3, ADC_SampleTime_55Cycles5); // PSD3
  ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 4, ADC_SampleTime_55Cycles5); // PSD4
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 5, ADC_SampleTime_55Cycles5); // IR1
  ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 6, ADC_SampleTime_55Cycles5); // IR2
  //ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 10, ADC_SampleTime_55Cycles5);
  
  ADC_DMACmd(ADC1, ENABLE);
  
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Enable ADC1 reset calibration register */   
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));
     
  /* Start ADC1 Software Conversion */ 
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);

}


void USART_Configuration()
{
  USART_InitTypeDef USART_InitStructure;
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStructure);
  USART_Cmd(USART1, ENABLE);
}


void GPIO_Configuration()
{
  
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6  | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2  | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
 
  //GPIO_PinRemapConfig()
  
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2  | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}



void wheel1(unsigned int wheel1_num)
{
  TIM_OCInitStructure.TIM_Pulse = wheel1_num * 85 / 100;
  TIM_OC4Init(TIM4, &TIM_OCInitStructure);
  TIM_CtrlPWMOutputs(TIM4, ENABLE);
}

void wheel2(unsigned int wheel2_num)
{
  TIM_OCInitStructure.TIM_Pulse = wheel2_num * 85 / 100;
  TIM_OC3Init(TIM4, &TIM_OCInitStructure);
  TIM_CtrlPWMOutputs(TIM4, ENABLE);
}


int putchar(int c)               // 8bit, 1byte 출력 함수 ---- 1byte 는 글자 1개의 데이터량(한글의 경우 2byte, ASKII코드에는 존재하지 않아서 지원안됨)
{
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
  USART_SendData(USART1, c);
  return c;
}


void main()
{
  double a;
  a = 1000000;
  while(a)
  {
    a--;
  }
  RCC_Configuration();
  GPIO_Configuration();
  USART_Configuration();
  DMA_Configuration();
  ADC_Configuration();
  
  
  //////////////////////////////////////////// PWM Setting //////////////////////////////////////////////////
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
  TIM_TimeBaseInitStructure.TIM_Prescaler = 36;
  TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInitStructure.TIM_Period = 99;
  TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
  
  //TIM_OCInitTypeDef TIM_OCInitStructure;
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 99;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC4Init(TIM4, &TIM_OCInitStructure);
  TIM_OC3Init(TIM4, &TIM_OCInitStructure);
  
  TIM_ARRPreloadConfig(TIM4, ENABLE);
  TIM_Cmd(TIM4, ENABLE);
  TIM_CtrlPWMOutputs(TIM4, ENABLE);
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  /////////////////////////////////LED
  
  //GPIO_SetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);
  
  /////////////////////////////////
  
  ///////////////////////////////// ADC 보호
  
  GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);

  
  /////////////////////////////////
  ///////////////////////////////// 
  GPIO_SetBits(GPIOB, GPIO_Pin_5 | GPIO_Pin_10);
 
  
  TIM_OCInitStructure.TIM_Pulse = 85;
  TIM_OC3Init(TIM4, &TIM_OCInitStructure);
  
  TIM_OCInitStructure.TIM_Pulse = 85;
  TIM_OC4Init(TIM4, &TIM_OCInitStructure);
  
  FF();
  wheel1(0);
  wheel2(0);
  a = 5000;
  while(a)
  {
    a--;
  }
  
  
  GPIO_SetBits(GPIOB, GPIO_Pin_12 | GPIO_Pin_13);////////////////////////////////동작신호
  
  
  
  wheel1(0);
  wheel2(0);
  /*
  while(ADCConvertedValue[4] > 500 || ADCConvertedValue[5] > 500)
  {
    wheel1(100);
    wheel2(100);
  }
  */
  while (1)
  {


    FF();
    if(ADCConvertedValue[4] > 500)
      ST = IR_L;
    else if(ADCConvertedValue[5] > 500)
      ST = IR_R;
    
    else if(ADCConvertedValue[0] > 300 || ADCConvertedValue[3] > 300)///////
    {
      if(ADCConvertedValue[0] > 1200 || ADCConvertedValue[3] > 1200)/////
      ST = F_CL;
      else
      ST = F_ST;  
    }
    
      
    else if(ADCConvertedValue[1] < 1000)
    {
      ST = L_CL;
    }
      
    else if(ADCConvertedValue[2] < 1200)
    {
      if(ADCConvertedValue[2] > 1200)
      ST = R_CL;
      else if(ADCConvertedValue[2] > 800)
        ST = R_CL;
    } 
    else
      ST = NONE;
     //printf("%d\r\n", ST);
      
      
      
      
      
      
      
      if(ST == F_ST)
      {
        FF();
        if(ADCConvertedValue[0] > 300 && ADCConvertedValue[3] > 300)
        {
          wheel1(100);
          wheel2(100);
          a = 50000;
        while(a)
        {
          a--;
        }
        }
        else if(ADCConvertedValue[0] > 300 && ADCConvertedValue[3] < 300)
        {
          wheel1(80);
          wheel2(100);
          a = 50000;
        while(a)
        {
          a--;
        }
        }
        else if(ADCConvertedValue[0] < 300 && ADCConvertedValue[3] > 300)
        {
          wheel1(100);
          wheel2(80);
          a = 50000;
        while(a)
        {
          a--;
        }
        }
      }
      if(ST == F_CL)
      {
        FF();
        if(ADCConvertedValue[0] > 1200 && ADCConvertedValue[3] > 1200)
        {
          wheel1(100);
          wheel2(100);
          a = 50000;
        while(a)
        {
          a--;
        }
        }
        else if(ADCConvertedValue[0] > 1200 && ADCConvertedValue[3] < 1200)
        {
          FL();
          wheel1(100);
          wheel2(100);
           a = 50000;
        while(a)
        {
          a--;
        }
        }
        else if(ADCConvertedValue[0] < 1200 && ADCConvertedValue[3] > 1200)
        {
          FR();
          wheel1(100);
          wheel2(100);
          a = 50000;
        while(a)
        {
          a--;
        }
        }
      }
      if(ST == R_ST)
      {
        FR();
        wheel1(100);
        wheel2(0);
        a = 100000;
        while(a)
        {
          a--;
        }
      }
      if(ST == L_ST)
      {
        FL();
        wheel1(0);
        wheel2(100);
        a = 100000;
        while(a)
        {
          a--;
        }
      }
      if(ST == L_CL)
      {
        FL();
        while(ADCConvertedValue[3] < 1000)
        {
          wheel1(100);
          wheel2(100);
        }
      }
      if(ST == R_CL)
      {
        FR();
        while(ADCConvertedValue[0] < 1000)
        {
          wheel1(100);
          wheel2(100);
        }
      }
      if(ST == IR_R)
      {
        BB();
        while(1)
        {
          if(ADCConvertedValue[5] < 500 || ADCConvertedValue[4] < 500)
          {
            wheel1(0);
            wheel2(0);
            
            break;
          }
          else
          {
            wheel1(100);
            wheel2(100);
            a = 50000;
        while(a)
        {
          a--;
        }
          }
        }
        FR();
        wheel1(100);
        wheel2(100);
        a = 200000;
        while(a)
        {
          a--;
        }
       }
      
      if(ST == IR_L)
      {
        BB();
        while(1)
        {
        if(ADCConvertedValue[4] < 500 || ADCConvertedValue[5] < 500)
        {
          wheel1(0);
          wheel2(0);
          break;
        }
        else
        {
          wheel1(100);
          wheel2(100);
          a = 50000;
        while(a)
        {
          a--;
        }
        }
        }
        FL();
        wheel1(100);
        wheel2(100);
        a = 200000;
        while(a)
        {
          a--;
        }
      }
      
      if(ST = NONE)
      {
        FF();
        wheel1(70);
        wheel2(70);
        a = 50000;
        while(a)
        {
          a--;
        }
      }
      
          
      
     }
     
      

  
   }
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    

  
