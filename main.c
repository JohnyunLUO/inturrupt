#include "stm32f10x.h"
#include "GPIO_STM32F10x.h"
#include "LED color.h"

GPIO_InitTypeDef GPIO_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
EXTI_InitTypeDef EXTI_InitStructure;

configure_LED(){
	
	// 設置PA0引腳為輸入模式
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
 // 設置中斷觸發引腳
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);

}


enum color ledcolor;

void Configure_Button_Interrupt(void);
int color_=6,i=7;
int main(void)
{	
	
	GPIO_PinConfigure(GPIOB,0,GPIO_OUT_PUSH_PULL,GPIO_MODE_OUT10MHZ);	//初始化 GPIOB,0號號腳位，上拉輸出，輸出10MHZ(LED green)
	GPIO_PinConfigure(GPIOB,1,GPIO_OUT_PUSH_PULL,GPIO_MODE_OUT10MHZ);	//初始化 GPIOB,1號號腳位，上拉輸出，輸出10MHZ(LED bule)
	GPIO_PinConfigure(GPIOB,5,GPIO_OUT_PUSH_PULL,GPIO_MODE_OUT10MHZ); //初始化 GPIOB,5號號腳位，上拉輸出，輸出10MHZ(LED red)
	
	//GPIO_PortClock (GPIOA, 1);	//設定GPIOA的時鐘(內建按鈕)
	//GPIO_PinConfigure(GPIOA,0,GPIO_IN_FLOATING,GPIO_MODE_INPUT);	//初始化 GPIOA,0號號腳位，浮空
	GPIO_PortWrite (GPIOB,0x23,0x23);	//控制portB，ODR,輸入1為高電位LED off，
																		//輸入0為低電位LED on,0x23=00100011(b),所以是0、1、5腳位，
																		//中間0x23選擇腳位並歸0，後面0x23所選腳位輸入1。
	// 啟用GPIOA外設時鐘
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	configure_LED();
	
  /*// 設置PA0引腳為輸入模式
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOA, &GPIO_InitStructure);*/

  // 設置中斷觸發
  Configure_Button_Interrupt();
	
	//ledcolor=WHITE;

	
	while(1){
		ledcolor=color_;
		if(ledcolor==DARK){
			LED_RED;		//LED燈亮紅色
			
		}
		else if(ledcolor==RED){
			
				LED_GREEN;			
		}
		else if(ledcolor==GREEN){			
			
				LED_BLUE;
		}
		else if(ledcolor==BLUE){			
			
				LED_YELLOW;
		}
		else if(ledcolor==YELLOW){			
			
				LED_PURPLE;
		}
		else if(ledcolor==PURPLE){			
			
				LED_CYAN;			
		}
		else if(ledcolor==CYAN){			
			
				LED_WHITE;
		}
		else if(ledcolor==WHITE){			
			
				LED_RGBOFF;
		}
	}
}


void Configure_Button_Interrupt(void)
{
  // 啟用EXTI外設時鐘AFIO
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

 

  // 設置中斷觸發
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; // 設置為上升沿觸發
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  // 設置中斷控制器NVIC
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); //把上面四行結構設定值帶入函式NVIC_Init
}


void EXTI0_IRQHandler(void) /*觸發中斷後會先跳到這段程式跑,這個function的選擇依據觸發來源而不同，
														例如此例是GPIO A0輸入高低電位觸發，所以用EXTI0_IRQHandler，若是觸發來源為USART，
														則會選USART1_IRQHandler,選擇請參startup_stm32f10x_hd.s*/
{
  // 確認是PA0引腳的中斷
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
    
		//中斷處理程式
    // 在這裡添加你的程式碼以處理按鈕中斷
		
		color_=i%8;
		i++;
		
		// 清除中斷標誌
    EXTI_ClearITPendingBit(EXTI_Line0);
  }
}

void SystemInit(void)
{	
}
