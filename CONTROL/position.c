#include "position.h"

void Position_control(unsigned char Data_flag,float Climb,float Decline)
{
	static unsigned char hover = 0 ;
	static unsigned char controlCnt = 0;
	static float TgtHeight = 0 ;
//	static Vector3f_t DesiredAcceleration;
	UAVThrust.Gravity_Acceleration = 9.8f ;
	
	controlCnt++;
	if(FlightControl.landFlag==1){//����λ�ÿ���ǰ���ж��Ƿ�Ҫ����
        //��������
        TgtHeight = TgtHeight - Decline;
		//�������� �߶Ȼ�PID��������ٶȻ�������������ٶȻ�
		if(controlCnt ==2)
			{
		  //�⻷��P������
			float heightErro = TgtHeight - RT_Info.Height;
			OriginalPosZ.value = Limits_data( PID_ParaInfo.PosZ.Kp * heightErro , 1.0f ,-1.0f);
			}
        //�ٶȻ�PID������
        OriginalVelZ.value = Limits_data(  PID_Control(&PID_ParaInfo.VelZ,&OriginalVelZ,OriginalPosZ.value,RT_Info.Height_V,0.002,4) ,7,-7);
		//���ٶȻ�PID������
		OriginalAccZ.value =  Limits_data(PID_Control(&PID_ParaInfo.AccZ,&OriginalAccZ,OriginalVelZ.value,RT_Info.accZaxis,0.002,7),10,-10);				
		//���䵽6CM���µ�ʱ��
        if(RT_Info.Height<0.06f){
            FlightControl.OnOff = Drone_Land;//��½ģʽ
            FlightControl.landFlag = 0;		 //��������־λ
            TgtHeight = 0;
			FlightControl.ControlStart = false;
            Target_Info.Height = 1.0f; //�ָ���ʼ��Ĭ��Ŀ��߶�
		
        }
    }
	    else{//�������		
				/* �ֱ�ң�ظ߶ȿ����� */
        if(Sbus_Number.SBUS_RY==0)
        {
            /* ��һ�λص���ͣ״̬�������ڵĸ߶�����ΪĿ��߶� */
            if(hover ==1)
            {	//Target_Info.Height ��������0.9m
                Target_Info.Height = RT_Info.Height ;
                hover=0 ;
            }
            /************************�߶���ͣ���� ************************/
		/* ��һ����ɻ������� */
		if(TgtHeight < Target_Info.Height && FlightControl.LaunchFlag == true)
		{
			if(TgtHeight < 0.6f)
					 TgtHeight = TgtHeight + Climb ;
			 else
					 TgtHeight = TgtHeight + Climb/2 ;
		}
		else{
					TgtHeight = Target_Info.Height;
					FlightControl.LaunchFlag = false ;
			}
	if(controlCnt ==2)
		{//��������
//�⻷��P������
float heightErro = TgtHeight - RT_Info.Height;
OriginalPosZ.value = Limits_data( PID_ParaInfo.PosZ.Kp * heightErro ,1.0f ,-1.0f);  //��1m/s��Ŀ���ٶ�
		}
//�ٶȻ�PID������
OriginalVelZ.value = Limits_data(  PID_Control(&PID_ParaInfo.VelZ,&OriginalVelZ,OriginalPosZ.value,RT_Info.Height_V,0.002,4) ,7.0f,-7.0f);
//���ٶȻ�PID������
OriginalAccZ.value =  Limits_data(PID_Control(&PID_ParaInfo.AccZ,&OriginalAccZ,OriginalVelZ.value,RT_Info.accZaxis,0.002,7),10.0f,-10.0f);
						
        }//�����ң������LY
        else
        {																						
OriginalVelZ.value = Limits_data(  PID_Control(&PID_ParaInfo.VelZ,&OriginalVelZ,Sbus_Number.SBUS_RY/100,RT_Info.Height_V,0.002,4) ,7.0f,-7.0f);
//���ٶȻ�PID������
OriginalAccZ.value =  Limits_data(PID_Control(&PID_ParaInfo.AccZ,&OriginalAccZ,OriginalVelZ.value,RT_Info.accZaxis,0.002,7),10.f,-10.f);
          hover =1;
        }
    }
UAVThrust.HeightThrust =  OriginalAccZ.value + UAVThrust.Gravity_Acceleration;
	
/////////////************************ λ�û��ٶȿ�����  ************************/
    //ֻ�з������ĸ߶ȴ���10cm�ſ�ʼ����λ�ÿ���
    if(RT_Info.Height != 0.0f)
	{
        switch (Data_flag){
            /************************ ��ͷģʽ���޶�λ��  ************************/
            case 0://///////////
			Target_Info.Pitch = Sbus_Number.SBUS_LY;
			Target_Info.Roll =  Sbus_Number.SBUS_LX;
                break;
            /************************ ��ͷģʽ���޶�λ��  ************************/
            case 1:

                break;
           }

	}
	if(controlCnt == 2)
	controlCnt =0;
}


