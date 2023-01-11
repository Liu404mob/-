#include "gpio.h"


void General_Gpioinit()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOB, ENABLE);
    //LED
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    //BEEP
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
	GPIO_SetBits(GPIOB, GPIO_Pin_2); 
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//��ͨ���ģʽ
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO
//    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM14);
//    //�ṹ��
//    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//    TIM_OCInitTypeDef  TIM_OCInitStructure;
//    //ʱ��
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
//    //��ʱ����������
//    TIM_TimeBaseStructure.TIM_Period = 3000;      							//PWM����
//    TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1;    						//����Ԥ��Ƶ
//    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;			//����ʱ�ӷ�Ƶϵ��������Ƶ
//    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
//    TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);
//    //ͨ��һ����
//    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;						//����ΪPWMģʽ2
//    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		//����ֵС�ڸ�ֵʱΪ�͵�ƽ
//    TIM_OCInitStructure.TIM_Pulse = 0;
//    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//    TIM_OC1Init(TIM14, &TIM_OCInitStructure);	 //ʹ��ͨ��1
//    TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);
//    //ʹ��
//    TIM_ARRPreloadConfig(TIM14, ENABLE);			 // ʹ��TIM14���ؼĴ���ARR
//    TIM_Cmd(TIM14, ENABLE);                   //ʹ�ܶ�ʱ��14

    //KEY
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//SW3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ���ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//SW2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ���ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIO

    GPIO_SetBits(GPIOB, GPIO_Pin_8 | GPIO_Pin_9);
    GPIO_SetBits(GPIOC, GPIO_Pin_2 | GPIO_Pin_3);
    GPIO_SetBits(GPIOC, GPIO_Pin_1);
}


void Beep_RingsOnce(void)
{
//    TIM14 -> CCR1 = 1500;
//    delay_ms(500);
//    TIM14 -> CCR1 = 0;
	GPIO_ResetBits(GPIOB,GPIO_Pin_2);  //��������Ӧ����GPIOF8���ͣ�
	delay_ms(500);
	GPIO_SetBits(GPIOB, GPIO_Pin_2); 
}

void SelfChecking_By(void)
{
    TIM14 -> CCR1 = 1000;
    delay_ms(500);
    TIM14 -> CCR1 = 0;
    delay_ms(100);
    TIM14 -> CCR1 = 2000;
    delay_ms(500);
    TIM14 -> CCR1 = 0;
    delay_ms(500);
}

