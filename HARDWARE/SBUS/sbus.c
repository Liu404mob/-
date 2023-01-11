#include "sbus.h"

void Usart5_Init(u32 bound)// RX PD2
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);

    GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����

    GPIO_Init(GPIOD, &GPIO_InitStructure);
    //USART5 ��ʼ������
    USART_InitStructure.USART_BaudRate = bound;//����������
    USART_InitStructure.USART_WordLength = USART_WordLength_9b;//���ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_2;//2��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_Even;//żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx| USART_Mode_Tx ;	//�շ�ģʽ
    USART_Init(UART5, &USART_InitStructure); //��ʼ������5

    //Usart5 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;//����5�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ�4
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

    USART_Cmd(UART5, ENABLE);  //ʹ�ܴ���5

    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//���յ�8λ���ݾ��ж�һ��
	USART_ITConfig(UART5, USART_IT_IDLE, ENABLE);
}

uint8_t buf[26];
void UART5_IRQHandler(void)
{
	uint8_t res;
	uint8_t clear = 0;
	static uint8_t Rx_Sta = 1;

if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
{
	res =UART5->DR;
	buf[Rx_Sta++] = res;
}
	//һ�����ݷ������ ׼������һ�º�ȥ������һ������ �м���4MS/14MS�ӳٹ��������
	else if(USART_GetITStatus(UART5, USART_IT_IDLE) != RESET)
	{//��⵽������·ʱ����λ��Ӳ���� 1����λ������������㣨���� USART_SR �Ĵ�����Ȼ����� USART_DR �Ĵ�������
		clear = UART5->SR;
		clear = UART5->DR;
		buf[0] = Rx_Sta - 1;//��Xms�ڽ��յ�����������
		Rx_Sta = 1;
	}

}

uint16_t CH[18]; // ͨ��ֵ
uint8_t rc_flag = 0;
void Sbus_Data_Count(void)
{
	CH[0] = ((int16_t)buf[ 2] >> 0 | ((int16_t)buf[ 3] << 8 )) & 0x07FF;//�ұ�ң�ˣ����ң� ��353-1024-1695��
	CH[1] = ((int16_t)buf[ 3] >> 3 | ((int16_t)buf[ 4] << 5 )) & 0x07FF;//�ұ�ҡ�ˣ����£� ��353-1024-1695��
	CH[2] = ((int16_t)buf[ 4] >> 6 | ((int16_t)buf[ 5] << 2 ) | (int16_t)buf[ 6] << 10 ) & 0x07FF;//���ҡ�� �����£�
	CH[3] = ((int16_t)buf[ 6] >> 1 | ((int16_t)buf[ 7] << 7 )) & 0x07FF;//���ҡ�� �����ң�
	CH[4] = ((int16_t)buf[ 7] >> 4 | ((int16_t)buf[ 8] << 4 )) & 0x07FF;//SB ��353����1024 ��1695
	CH[5] = ((int16_t)buf[ 8] >> 7 | ((int16_t)buf[ 9] << 1 ) | (int16_t)buf[10] << 9 ) & 0x07FF;//V1�м���ť ��-�ң�353-1695
	CH[6] = ((int16_t)buf[10] >> 2 | ((int16_t)buf[11] << 6 )) & 0x07FF;//V2����ת�� ���ϵ�353 �м�1024 ����1695

//û���õ���ͨ��
//CH[ 7] = ((int16_t)buf[11] >> 5 | ((int16_t)buf[12] << 3 )) & 0x07FF;
//CH[ 8] = ((int16_t)buf[13] << 0 | ((int16_t)buf[14] << 8 )) & 0x07FF;
//CH[ 9] = ((int16_t)buf[14] >> 3 | ((int16_t)buf[15] << 5 )) & 0x07FF;
//CH[10] = ((int16_t)buf[15] >> 6 | ((int16_t)buf[16] << 2 ) | (int16_t)buf[17] << 10 ) & 0x07FF;
//CH[11] = ((int16_t)buf[17] >> 1 | ((int16_t)buf[18] << 7 )) & 0x07FF;
//CH[12] = ((int16_t)buf[18] >> 4 | ((int16_t)buf[19] << 4 )) & 0x07FF;
//CH[13] = ((int16_t)buf[19] >> 7 | ((int16_t)buf[20] << 1 ) | (int16_t)buf[21] << 9 ) & 0x07FF;
//CH[14] = ((int16_t)buf[21] >> 2 | ((int16_t)buf[22] << 6 )) & 0x07FF;
//CH[15] = ((int16_t)buf[22] >> 5 | ((int16_t)buf[23] << 3 )) & 0x07FF;
}
