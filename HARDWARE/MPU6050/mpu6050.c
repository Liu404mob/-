#include "mpu6050.h"

u8 Gyro_Off_started = 0;
int16_t lastAx, lastAy, lastAz,
        lastGx, lastGy, lastGz;
static int16_t Gx_offset = 0, Gy_offset = 0, Gz_offset = 0;

static void MPU6500_writeReg(u8 reg, u8 data)
{
    MPU6500_CSL();//�����ǵ�Ƭѡ������
    SPI3_ReadWrite_Byte(reg);
    SPI3_ReadWrite_Byte(data);
    MPU6500_CSH();
}

void MPU6500_initialize(void)
{
    MPU6500_writeReg(MPU6500_RA_PWR_MGMT_1, 0x80);          // ��λ
    delay_ms(20);//��Դ�������ĵ�7λresetλ��Ϊ1�ͻḴλ
    MPU6500_writeReg(MPU6500_RA_SIGNAL_PATH_RESET, 0x07);//���������������ź�·����
    delay_ms(20);
    MPU6500_writeReg(MPU6500_RA_PWR_MGMT_1,0x03);          // ʹ���ڲ�20Mʱ��
    delay_ms(20);//00000011��2��0��λ������໷׼�������Զ�ѡ����õ�ʱ�ӣ�����ʹ���ڲ�20Mʱ��
    //δ��ֹ�ж� �ж�����/��·ʹ�����üĴ��� ȫ������Ϊ0�Ϳɽ�ֹ�����ж�
    MPU6500_writeReg(MPU6500_RA_SMPLRT_DIV, 0x00);          // sample rate.  Fsample= 1Khz/(<this value>+1) = 1000Hz
    delay_ms(20);//��Ƶ��=0
    MPU6500_writeReg(MPU6500_RA_ACCEL_CONFIG, 0x10);        // +-8G accel
    delay_ms(20);//00010000 4:3λΪ10 ����+-8g 65536/16=4096LSB/g
    MPU6500_writeReg(MPU6500_RA_FF_THR, 0x04);              // �ӼƵ�ͨ�˲���
    delay_ms(20);//00000100����λΪ0 2:0Ϊ100 ������������˲�Ϊ20HZ
    MPU6500_writeReg(MPU6500_RA_GYRO_CONFIG, 0x18); //00011000 4:3Ϊ11����+-2000 gyro 0:1 Ϊ00 ����F_choiceΪ11
    delay_ms(20);//�����ǵ�16λAD�����Χ��-32768��+32768 ������Ϊ65536/4000=16.384LSB/��/S
    MPU6500_writeReg(MPU6500_RA_CONFIG, 0x03);              // �ڲ���ͨ������˹�˲���  ��ֹƵ�� 41Hz
    delay_ms(20);//00000011
    MPU6500_writeReg(MPU6500_RA_PWR_MGMT_2, 0x00);
    delay_ms(20);//������Դ����2�Ĵ�����5:0λ ȫ������Ϊ0�Ϳ���3�� �����Ǻͼ��ٶ�
//	  MPU6500_writeReg(MPU6500_RA_PWR_MGMT_1, 0x00);	//�������״̬
//	MPU6500_writeReg(MPU6500_RA_SMPLRT_DIV , 0x07);
//	MPU6500_writeReg(MPU6500_RA_CONFIG, 0x06);
//	MPU6500_writeReg(MPU6500_RA_GYRO_CONFIG, 0x18);
//	MPU6500_writeReg(MPU6500_RA_ACCEL_CONFIG , 0x10);
}

void MPU6500_readGyro_Acc(int16_t *gyro, int16_t *acc)
{
    static u8 buf[14];
    static int16_t gx, gy, gz;
    static int16_t ax, ay, az;
    MPU6500_CSL();//ÿ��������ݷָ�8��8�ֱ���� MPU6500_RA_ACCEL_XOUT_H   MPU6500_RA_ACCEL_XOUT_L �Ĵ�����
    SPI3_readRegs(MPU6500_RA_ACCEL_XOUT_H, 14, buf); //������3����6���Ĵ��� �¶ȼ���2���Ĵ��� ���ٶȼ���6���Ĵ���
    MPU6500_CSH();//һ��14�� ��������buf��������
    ax = (int16_t)(((int16_t)buf[0]) << 8 | buf[1]);
    ay = (int16_t)(((int16_t)buf[2]) << 8 | buf[3]);//���ٶȼƲ��������������ϵ�µ�
    az = (int16_t)(((int16_t)buf[4]) << 8 | buf[5]);
    gx = (int16_t)(((int16_t)buf[8]) << 8 | buf[9]);
    gy = (int16_t)(((int16_t)buf[10]) << 8 | buf[11]);
    gz = (int16_t)(((int16_t)buf[12]) << 8 | buf[13]);
    gyro[0] = gx - Gx_offset;	//gyro������
    gyro[1] = gy - Gy_offset;
    gyro[2] = gz - Gz_offset;
    acc[0] = ax; //acc���ٶȼ�
    acc[1] = ay;
    acc[2] = az;
}

void MPU6500_Init_Offset(void)//6500��ʼ��У׼ ���ƽ��ֵ ���˸���
{
    unsigned int i;
    int16_t temp[3], temp2[3]; //temp ���Ǵ�����ٶȼƵ�3������ temp2�Ǵ���������3����ٶȼ�����
    int32_t tempgx = 0, tempgy = 0, tempgz = 0;
    Gx_offset = 0;
    Gy_offset = 0;
    Gz_offset = 0;

    for(i = 0; i < 500; i++)
    {
        delay_us(200);
        MPU6500_readGyro_Acc(temp, temp2); //temp ���Ǵ��������ǵ�3������ temp2�Ǵ�����ٶȼ�3����ٶȼ�����
        tempgx += temp[0];//����5000���ۼ�
        tempgy += temp[1];
        tempgz += temp[2];
    }
    Gx_offset = tempgx / 500; //ֻ���������ǵ�ƽ��ֵ
    Gy_offset = tempgy / 500;
    Gz_offset = tempgz / 500;
}


//------------------End of File----------------------------

