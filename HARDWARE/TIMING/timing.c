#include "timing.h"

void TIM4_Init(void)//1MS�ж�һ��
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    //TIM4����APB1  42*2=84M �����ֲ���˵��Ƶ��*2
    TIM_TimeBaseStructure.TIM_Period = 999; //�Զ���װ��ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = 83; //Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //һ���Ӽ���1M�Σ�������999+1����Ҫ0.001s
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

    //����ʱ��TIM4��ʱ���ж����ȼ�
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //�������ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //����ѡ��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM4, ENABLE);
}
void TIM4_IRQHandler(void)
{

    static uint16_t  KAERMAN = 0, PS2 = 0, POSITION = 0, ATTITUDE = 0, IMU = 0, DAYIN = 0, LAND = 0, AADC = 0; //��Ƶϵ��
    if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
        KAERMAN++;
        PS2++, POSITION++, ATTITUDE++, IMU++, DAYIN++, AADC++;
        if(SAFE_FLAG == 1)LAND++;
        if(PS2 >= 50)
        {
            PS2_FLAG = 1;
            PS2 = 0;
        }
        if(KAERMAN >= 5)
        {
            KAERMAN_FLAG = 1;
            KAERMAN = 0;
        }
        if(POSITION >= 5)
        {
            POSITION_FLAG = 1;
            POSITION = 0;
        }
        if(ATTITUDE >= 2)
        {
            ATTITUDE_FLAG = 1;
            ATTITUDE = 0;
        }
        if(IMU >= 2)
        {
            IMU_FLAG = 1;
            IMU = 0;
        }
        if(DAYIN >= 1000)
        {
            DAYIN_FLAG = 1;
            DAYIN = 0;
        }
        if(AADC >= 500)
        {
            AADC_FLAG = 1;
            AADC = 0;
        }
        //		if(LAND>=10000)	{LAND_FLAG=1;LAND=0;SAFE_FLAG=0;}
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    }
}
