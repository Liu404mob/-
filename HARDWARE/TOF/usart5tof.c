#include "usart5tof.h"//��ʵ�Ǵ���2

//#define USART5_REC_LEN 200
//#define Data_Head      0x59
//#define Data_Length     9

u8 USART2_RX_BUF[200];
u8 ccc=0;
void Usart2_Init(u32 bound)//TX PD5 RX PD6
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    //USART2 ��ʼ������

    USART_InitStructure.USART_BaudRate = bound;//����������
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
    USART_Init(USART2, &USART_InitStructure); //��ʼ������5

    //Usart2 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//����5�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4; //��ռ���ȼ�4
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

    USART_Cmd(USART2, ENABLE);  //ʹ�ܴ���2

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//���յ�8λ���ݾ��ж�һ��


}

void USART2_IRQHandler(void)                	//����2�жϷ������
{
    //printf("b\r\n");
    static u8 flag_data = 0;
    static u8 index = 0;
    u16 CheckSum = 0;
    u8 i;
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�
    {
        //printf("a\r\n");
        if(USART_ReceiveData(USART2) == 0x59) //��ȡ���յ�������
        {
            flag_data = 1;
        }
        if(flag_data == 1)
        {
            USART2_RX_BUF[index++] = USART_ReceiveData(USART2);
            if(index == 9)
            {
                if((USART2_RX_BUF[0] == 0x59) && (USART2_RX_BUF[1] == 0x59))
                {
                    for(i = 0; i < 8; i++)
                    {
                        CheckSum += USART2_RX_BUF[i];
                    }
                    if((CheckSum & 0x00ff) == USART2_RX_BUF[8])
                    {
                        uint16_t Distance = (((uint16_t)USART2_RX_BUF[3]) << 8) + USART2_RX_BUF[2];
                        uint16_t Strength = (((uint16_t)USART2_RX_BUF[5]) << 8) + USART2_RX_BUF[4];
                        //���������ֲ�ǿ���������Χ����Ч
                        if(Strength > 500 && Strength < 60000)
                        {
                            TOFHeight = ((float)Distance) * 0.01f;   // ���Ƶ�λ
                        }
                        index = 0;
                        flag_data = 0;
                    }
                }
            }
        }
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }

}
