#include "sys.h"
#include "u8g2.h"


//�ڴ˶���ȫ�ֱ��� �ڲ���.h������ ��sys.h����extern��
PS2 Ps2_Number;							//�ֱ�����
SBUS Sbus_Number;
DroneRTInfo RT_Info;                   //������ʵʱ����
PIDOut OriginalPitch, OriginalRoll, OriginalYaw, OriginalPosX, OriginalPosY, OriginalPosZ,
       OriginalPitchRate, OriginalRollRate, OriginalYawRate, OriginalVelX, OriginalVelY, OriginalVelZ,
       OriginalFlowX, OriginalFlowY, OriginalFlowVelX, OriginalFlowVelY,
       OriginalAccZ;
PIDPara PID_ParaInfo;
KalmanFilter XAxis, YAxis, ZAxis, Barometer;
DroneTargetInfo Target_Info;           //������Ŀ���ȫ�ֱ���
Data_Combine DataCombineflag;      		 //����ģʽ0123
DroneFlightControl FlightControl;      //������״̬����
Thrust UAVThrust;											 //������Ť������
DroneErrangle Errangle_Info;           //������ƽ��У׼����
OffsetInfo OffsetData;                 //��У׼����ȫ�ֱ���
Throttle Throttle_Info;								 //������Űٷֱ�
float TOFHeight, RTbattery; //����߶�,��ص�ѹȫ�ֱ���

//�����־λ
u8 IMU_FLAG, KAERMAN_FLAG, PS2_FLAG, POSITION_FLAG, ATTITUDE_FLAG, SAFE_FLAG = 0, DAYIN_FLAG, LAND_FLAG, AADC_FLAG;

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����жϷ���2
	delay_init(168);
    uart_init(115200);//����1��ӡ���� 
    Adc_Init();
    Pid_Init();//PIDֵ��ʼ��
    TIM4_Init();//1ms�ж�һ��
    General_Gpioinit();
    PWM_Init();
    SPI3_Init();
	delay_ms(30);
    IMU_HardwareInit();// IMU�����������Ĵ�������
    Usart2_Init(115200); //����ģ��
	delay_ms(30);
	OLED_Init();				//��ʼ��OLED
	OLED_Clear();
    Usart5_Init(100000);//��ģң����
    KalmanFilter_Init(&XAxis, &YAxis, &ZAxis, &Barometer);	// KalmanFilter������ʼ��
    //	PS2_Init(); PS2_SetInit();
	LED1_ON;
    Beep_RingsOnce();	
	

	
    //�������ֲ�����
    unsigned int fly_Pretime = 0;
    float PreparationPitch = 0 ;
    float PreparationRoll = 0;
    u8 PreparationFlag = 1;
    FlightControl.ControlStart = false;
    while(1)
    {
		OLED_ShowCHinese(0,0,0);//
		OLED_ShowCHinese(18,0,1);//
		OLED_ShowCHinese(36,0,2);//
		OLED_ShowCHinese(54,0,3);//
		OLED_ShowCHinese(72,0,4);//
		OLED_ShowCHinese(90,0,5);//		
		OLED_ShowString(0,3,"1.3' OLED TEST");

        //��־λ�ǵ�����
//        if(DAYIN_FLAG == 1)
//        {
//            DAYIN_FLAG = 0;
////            printf("Height:%f\n", TOFHeight);
////            LED1 = ~LED1;
////            if(TOFHeight >= 0.70f)LED2_ON;
//        }
        //1111.PS2 50msһ��
        if(PS2_FLAG == 1)
        {
            PS2_FLAG = 0;
			Sbus_Data_Count();
			Sbus_Number.SBUS_RX=(CH[0]);//+-40  YAW  
			Sbus_Number.SBUS_RY=(CH[1]);//+-100  HEIGHT
			Sbus_Number.SBUS_LX=(CH[3]);//+-16  ROLL
			Sbus_Number.SBUS_LY=(CH[2]);//+-16  PITCH
			Sbus_Number.SBUS_V1=(float)CH[5];//V1
			Sbus_Number.SBUS_V2=(float)CH[6];//V2
			Sbus_Number.SBUS_SB=(float)CH[4];//SB
	
            if(Sbus_Number.SBUS_SB==1024)	//�⵽�м�
            {SAFE_FLAG = 1; ALLLED_ON;}                          
            else if(Sbus_Number.SBUS_SB==353)//�⵽�Ϸ�
            {ALLLED_OFF;SAFE_FLAG = 0;}   
			TIM2->CCR3=Sbus_Number.SBUS_RY;
			TIM2->CCR4=Sbus_Number.SBUS_RY;
//			if(Sbus_Number.SBUS_RY>=1100&&SAFE_FLAG==1)
//				{
//					for(int i=0;i<=Sbus_Number.SBUS_RY;i+=10)
//						{TIM1->CCR3=i;
//						TIM1->CCR4=i;
//						LED1_OFF;
//						}
//				}
//			else if(Sbus_Number.SBUS_RY<=1000&&SAFE_FLAG==1)
//				{
//				for(int i=0;i>=Sbus_Number.SBUS_RY;i-=10)
//						{TIM1->CCR3=i;
//						TIM1->CCR4=i;
//						LED1_OFF;
//						}
//				}

          
        }
//        //2222.�������˲� 5msһ��
//        if(KAERMAN_FLAG == 1)
//        {
//            KAERMAN_FLAG = 0;
//            POS_KalmanFilter(&ZAxis, TOFHeight, RT_Info.accZaxis);
//            RT_Info.Height = ZAxis.Axis_Pos;
//            RT_Info.Height_V = ZAxis.Axis_Vel;
//        }
//        //3333.λ�ÿ��� msһ��									//200*0.007
//        if(POSITION_FLAG == 1)
//        {
//            POSITION_FLAG = 0;
//            float Climbing = 0.007f;
//            float Declining = 0.002f;
//            Target_Info.Height = 1.0f;
//            if(FlightControl.DroneMode == Drone_Mode_4Axis && FlightControl.OnOff == Drone_On && FlightControl.ControlStart == true)
//            {
//                Position_control(0, Climbing, Declining);   //Data_Headmode = 0,	Data_Headfree = 1
//            }
//        }
//        //4444.��̬���� msһ��
//        if(ATTITUDE_FLAG == 1)
//        {
//            ATTITUDE_FLAG = 0;
//            //�ж��ǲ���ON����û�и߶ȵĻ������
//            if(FlightControl.OnOff == Drone_On  && RT_Info.Height >= 0.01f)
//            {
//                /*�ж��Ƿ�Ϊ�������ģʽ*/
//                if(FlightControl.DroneMode == Drone_Mode_4Axis)
//                {
//                    /*Ԥ�ɳ���*/
//                    if(fly_Pretime < 500) //ִֻ��һ��
//                    {
//                        fly_Pretime++;
//                        PreparationPitch += (RT_Info.Pitch - Errangle_Info.fixedErroPitch);
//                        PreparationRoll += (RT_Info.Roll - Errangle_Info.fixedErroRoll);
//                        PWM_OUTPUT(250, 250, 250, 250); //���Ե�������ת����4���ת��һ��
//                    }
//                    else
//                    {
//                        if(PreparationFlag) //ִֻ��һ��
//                        {
//                            PreparationPitch /= 500;//500��ƽ�����ٴ�����̬����
//                            PreparationRoll /= 500;
//                            PreparationFlag = false;
//                            Target_Info.Yaw = RT_Info.Yaw;//���ڵĽǶȾ���Ŀ��Ƕȣ���ֹ��ת*
//                            RT_Info.FlowX = 0 ;
//                            RT_Info.FlowY = 0 ;
//                            FlightControl.ControlStart = true;//��̬���Ƶ�����ı��־λ�� λ�ÿ��ƿ�ʼ
//                        }
//                        if(FlightControl.ControlStart)
//                        {
//                            Attitude_control(PreparationPitch, PreparationRoll);
//                            /*��������*/
//                            Safety_Protection();
//                        }
//                    }
//                }//�и߶ȵ���������ģʽ �Ͳ��ڿ���ƽ�����У׼
//                else
//                {
//                    Attitude_control(0, 0);
//                }
//            }
//            else //�ж��ǲ���ON����û�и߶� Ҳ����˵�ǽ���֮��Ϊ�ڶ��������׼��
//            {
//                PreparationFlag = true;
//                PreparationPitch = 0;
//                PreparationRoll = 0;
//                fly_Pretime = 0;
//                OriginalPitchRate.iOut = 0;
//                OriginalRollRate.iOut = 0;
//                OriginalYaw.iOut = 0;
//                OriginalVelZ.iOut = 0;
//                OriginalAccZ.iOut = 0;
//                OriginalVelX.iOut = 0;
//                OriginalVelY.iOut = 0;
//                OriginalFlowVelX.iOut = 0;
//                OriginalFlowVelY.iOut = 0;
//                Target_Info.Height = 1.0f; //�ָ���ʼ��Ĭ��Ŀ��߶�
//                Target_Info.Pitch = 0.0f; //�ָ���ʼ��Ĭ��Ŀ�긩��
//                Target_Info.Roll = 0.0f; //�ָ���ʼ��Ĭ��Ŀ�귭��
//                FlightControl.LaunchFlag = true; //��һ����ɱ�־λ λ�ÿ���������õ�
//                FlightControl.ControlStart = false;
//                PWM_OUTPUT(0, 0, 0, 0);
//            }
//        }

        //5555.��̬��� 2msһ��
        if(IMU_FLAG == 1)
        {
            IMU_FLAG = 0;
            IMU_getInfo();
        }
//        //6666.��ص�ѹ��� 1sһ��
//        if(AADC_FLAG == 1)
//        {
//            AADC_FLAG = 0;
//            RTbattery = Get_battery_volt() ;
//            if(RTbattery <= 10.5f&&RTbattery >= 9.5f)
//            {
//                TIM14 -> CCR1 = 2000;
//            }           
//        }


    }//WHILE
}//MAIN
