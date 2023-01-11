#ifndef _PARAMETER_H
#define _PARAMETER_H
#include <stdbool.h>
#include <stdint.h>
/* ѡ��ƥ��ĵ���ͺ� */
//#define Tmotor_2213
#define Tmotor_2306


/* ѡ�񶨸ߴ����� */
#define Tof
//#define Ultrasonic
//#define Barometer


//����������
//#define Model330
#ifdef Model330
    #define ARM_Length 0.165f
    #define Drone_Mass 1.02f
    #define Drag_Coeff 0.016f
    //ת����������  ת������ = M*R^2  �����ʵ���ԡ������޷����ת��������С����Ϊһ���ɵ���������
    #define Inertia_Wx    0.002f
    #define Inertia_Wy    0.002f
    #define Inertia_Wz    0.004f
#else
    #define ARM_Length 0.125f
    #define Drone_Mass 0.86f
    #define Drag_Coeff 0.016f
    //ת����������  ת������ = M*R^2  �����ʵ���ԡ������޷����ת��������С����Ϊһ���ɵ���������
    #define Inertia_Wx    0.001f
    #define Inertia_Wy    0.001f
    #define Inertia_Wz    0.002f
#endif

typedef struct{//PS2ң�����
	double PS2_LX;
	double PS2_LY;
	double PS2_RX;
	double PS2_RY;
	int PS2_KEY;
}PS2;

typedef struct{//PS2ң�����
	float SBUS_LX;
	float SBUS_LY;
	float SBUS_RX;
	float SBUS_RY;
	float SBUS_KEYL;//���ҡ��
	float SBUS_KEYR;//�ұ�ҡ��
	float SBUS_SB;
	float SBUS_V1;
	float SBUS_V2;
}SBUS;
/*���˻�ʵʱ��Ϣ*/
typedef struct
{
	float Pitch; 		  //ʵʱPitch�Ƕ�
	float Roll;			  //ʵʱRoll�Ƕ�
    float Yaw;			  //ʵʱYaw�Ƕ�
	float HomeYaw;		  //���ʱyaw�Ƕ�
	float HeadfreeYaw;    //��ͷģʽ��yaw�Ƕ�
	float Headfreezeta;   //��ͷģʽ��תZeta��
	float ratePitch;
	float rateRoll;
	float rateYaw;
	float accXaxis;
	float accYaxis;
	float accZaxis;
	float Height;					//US100�������߶�
	float Height_V;				//US100�������ٶ�
	float Barometer;      //��ѹ������
	float Barometer_V;    //��ѹ���ٶ�
	float FlowX;					//����λ��X
	float FlowY;          //����λ��Y
	float FlowX_V;				//�����ٶ�X
	float FlowY_V;				//�����ٶ�Y
	float PointX;					//������Xλ��
	float PointY;					//������Yλ��
	float PointX_V;				//������X���ٶ�
	float PointY_V;				//������Y���ٶ�
	float batteryVoltage; //��ص���
	float AccX;
	float AccY;
	float AccZ;
	float GyroX;
	float GyroY;
	float GyroZ;
	float MagX;
	float MagY;
	float MagZ;
	int lowPowerFlag;			//�͵�ѹ��־λ
	float CpuTick;        //ϵͳʱ���ʱ
	float LPFTest1;        //�˲�ǰ���ݲ���
	float LPFTest2;        //�˲�ǰ���ݲ���
	unsigned char AllowLanding;  // ң�������ƽ����־
//	Remote_Control_Status  controlStatus;  //רҵң��������״̬
//	Height_Data_Switching  heightDataSwitching;  //�߶�������Դ�л�
}DroneRTInfo;

typedef struct
{
	float error;
	float lasterror;
	float differential;
	float differentialFliter;
	float pOut;
    float iOut;
	float dOut;
	float value;//PID�������
	
}PIDOut;

typedef struct
{
	float Kp;
	float Ki;
	float Kd;
}PID;

typedef struct
{
    PID Pitch;
    PID Roll;
    PID Yaw;

    PID PitchRate;
    PID RollRate;
    PID YawRate;

    PID PosX;
    PID PosY;
    PID PosZ;

    PID VelX;
    PID VelY;
    PID VelZ;

    PID AccZ;

    PID FlowX;
    PID FlowY;
    PID FlowVelX;
    PID FlowVelY;
}PIDPara;



typedef struct{
    //�˲�ʱ��
    float Merge_t;

    //����
    float Q_Position;
    float Q_Velocity;
    float Q_Bias;
    float R_Position;

    //״̬
    float Axis_Pos;
    float Axis_Vel;
    float Axis_Bias;
    float Axis_Err;
    float AxisPCt_0;
    float AxisPCt_1;
    float AxisPCt_2;
    float AxisE;
    char  AxisC_0;
    float AxisK_0;
    float AxisK_1;
    float AxisK_2;
    float Axist_0;
    float Axist_1;
    float Axist_2;
    float AxisPdot[9];
    float AxisPP[3][3];
}KalmanFilter;


typedef struct
{
	float Pitch;
	float Roll;
	float Yaw;
	float RateRoll;
	float RatePitch;
	float RateYaw;
	float Height;
	float VelHeight;
	float AccHeight;
	float RemotePitch;
	float RemoteRoll;
	float BlackLineV;
    float BlackLineYaw;
	float DesiredAccelerationX;
	float DesiredAccelerationY;
	float DesiredAccelerationZ;
}DroneTargetInfo;

typedef enum
{
	Data_Headmode = 0,
	Data_Headfree = 1,
	Data_Point = 2,
	Data_Flow = 3,
	Data_Follow = 4,
}Data_Combine;

typedef enum
{  
	Drone_Off  = 0x00,//��ɻ��ߵ��Դ򿪵��
  Drone_On   = 0x01,//�رյ��
  Drone_Land = 0x02,//����	
}DroneFlightOnOff_TypeDef;

typedef enum{
  Drone_Mode_None=0,
  Drone_Mode_RatePitch, //��̬�ڻ�
  Drone_Mode_RateRoll,
  Drone_Mode_Pitch,     //��̬�⻷
  Drone_Mode_Roll,
  Drone_Mode_4Axis,     //�������
}DroneFlightMode_TypeDef;

typedef enum
{  
	Report_SET      = 0x01,
  Report_RESET    = 0x00, 		 	
}DroneReportSW_TypeDef;
typedef struct
{
	DroneFlightOnOff_TypeDef OnOff;
	DroneFlightMode_TypeDef DroneMode;
	DroneReportSW_TypeDef ReportSW;
	int landFlag;
	bool LaunchFlag;
	bool ControlStart;
}DroneFlightControl;

typedef struct{
    float PitchThrust;
    float RollThrust;
    float YawThrust;
    float HeightThrust;
    float Gravity_Acceleration;
	float f1;
	float f2;
	float f3;
	float f4;
	float t1;
	float t2;
	float t3;
	float t4;
	float PaddleEffect;
}Thrust;

/* ƽ��������� */
typedef struct
{
	float fixedErroPitch;
	float fixedErroRoll;
}DroneErrangle;

/*У׼����*/
typedef struct
{
	bool MagOffseting;
	int16_t MagX;
	int16_t MagY;
	int16_t MagZ;
	float GyroX;
	float GyroY;
	float GyroZ;
	float AccX;
	float AccY;
	float AccZ;
	float AccXScale;
	float AccYScale;
	float AccZScale;
}OffsetInfo;

typedef struct {
    float x;
    float y;
    float z;
} Vector3f_t;

typedef struct
{
	int M1;
	int M2;
	int M3;
	int M4;
}Throttle;

#endif
