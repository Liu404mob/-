#include "MahonyAHRS.h"

#define sampleFreq	 500.0f			// Ƶ��Ϊ500HZ
#define twoKpDef	(2.0f * 0.42f)	// KP=1
#define twoKiDef	(2.0f * 0.2f)	// KI=0
//��Ԫ���Լ�PI�����Ķ���
volatile float twoKp = twoKpDef;																		
volatile float twoKi = twoKiDef;																		
volatile float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;	//��Ԫ���ĳ�ʼ��				
volatile float integralFBx = 0.0f,  integralFBy = 0.0f, integralFBz = 0.0f;	//���ֱ�����ʼ��
///////////////////////////////1
void MahonyAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz) 
	{
	float recipNorm;
    float q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;
	float hx, hy, bx, bz;
	float halfvx, halfvy, halfvz, halfwx, halfwy, halfwz;
	float halfex, halfey, halfez;
	float qa, qb, qc;
	//��������Ʋ�����Ч����ʹ��IMU�㷨�������ڴ����ƹ�һ����ʹ��NaN��
	if((mx == 0.0f) && (my == 0.0f) && (mz == 0.0f)) {
		MahonyAHRSupdateIMU(gx, gy, gz, ax, ay, az);
		return;
	}
	if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {
		recipNorm = invSqrt(ax * ax + ay * ay + az * az);
		ax *= recipNorm;
		ay *= recipNorm;
		az *= recipNorm;//ͨ����һ������� ax�� ay�� az ����ֵ��Χ���-1 ��+1 ֮�䡣
		recipNorm = invSqrt(mx * mx + my * my + mz * mz);
		mx *= recipNorm;
		my *= recipNorm;
		mz *= recipNorm;

		q0q0 = q0 * q0;
		q0q1 = q0 * q1;
		q0q2 = q0 * q2;
		q0q3 = q0 * q3;
		q1q1 = q1 * q1;
		q1q2 = q1 * q2;
		q1q3 = q1 * q3;
		q2q2 = q2 * q2;
		q2q3 = q2 * q3;
		q3q3 = q3 * q3;

		hx = 2.0f * (mx * (0.5f - q2q2 - q3q3) + my * (q1q2 - q0q3) + mz * (q1q3 + q0q2));
		hy = 2.0f * (mx * (q1q2 + q0q3) + my * (0.5f - q1q1 - q3q3) + mz * (q2q3 - q0q1));
		bx = sqrt(hx * hx + hy * hy);
		bz = 2.0f * (mx * (q1q3 - q0q2) + my * (q2q3 + q0q1) + mz * (0.5f - q1q1 - q2q2));
		//���ٶȼƵ������ת��
		halfvx = q1q3 - q0q2;
		halfvy = q0q1 + q2q3;
		halfvz = q0q0 - 0.5f + q3q3;
		//�����Ƶ�����ת��
		halfwx = bx * (0.5f - q2q2 - q3q3) + bz * (q1q3 - q0q2);
		halfwy = bx * (q1q2 - q0q3) + bz * (q0q1 + q2q3);
		halfwz = bx * (q0q2 + q1q3) + bz * (0.5f - q1q1 - q2q2);
		// �����ƺͼ��ٶȼƵ������
		halfex = (ay * halfvz - az * halfvy) + (my * halfwz - mz * halfwy);
		halfey = (az * halfvx - ax * halfvz) + (mz * halfwx - mx * halfwz);
		halfez = (ax * halfvy - ay * halfvx) + (mx * halfwy - my * halfwx);
		// KI=0���Բ�����
		if(twoKi > 0.0f) {
			integralFBx += twoKi * halfex * (1.0f / sampleFreq);	
			integralFBy += twoKi * halfey * (1.0f / sampleFreq);
			integralFBz += twoKi * halfez * (1.0f / sampleFreq);
			gx += integralFBx;	
			gy += integralFBy;
			gz += integralFBz;
		}
		else {
			integralFBx = 0.0f;	
			integralFBy = 0.0f;
			integralFBz = 0.0f;
			}
		// ��KP�ۼ��� ���ٶȼƺʹ������ܵ�
		gx += twoKp * halfex;
		gy += twoKp * halfey;
		gz += twoKp * halfez;
	}
	gx *= (0.5f * (1.0f / sampleFreq));	// �ۼ���500�� ���Գ���500
	gy *= (0.5f * (1.0f / sampleFreq));
	gz *= (0.5f * (1.0f / sampleFreq));
	qa = q0;
	qb = q1;
	qc = q2;
	q0 += (-qb * gx - qc * gy - q3 * gz);
	q1 += (qa * gx + qc * gz - q3 * gy);
	q2 += (qa * gy - qb * gz + q3 * gx);
	q3 += (qa * gz + qb * gy - qc * gx);//�õ����ٶȼƺʹ����ƹ�ͬ���������Ԫ��
	//��һ��
	recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q0 *= recipNorm;
	q1 *= recipNorm;
	q2 *= recipNorm;
	q3 *= recipNorm;
}
////////////////////////////////////2
void MahonyAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az) {
	float recipNorm;
	float halfvx, halfvy, halfvz;
	float halfex, halfey, halfez;
	float qa, qb, qc;
	/*������ټƸ���������� 0����ô���Ըü��ٶ����ݡ������ڼ��ټ����ݹ�һ������
��ʱ�򣬻ᵼ�³��� 0 �Ĵ���*/
	if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {
		recipNorm = invSqrt(ax * ax + ay * ay + az * az);
		ax *= recipNorm;
		ay *= recipNorm;
		az *= recipNorm;//ax�� ay�� az ����ֵ��Χ���-1 ��+1 ֮��
		//ax,ay,az��ʵ�ʵļ��ٶ�ֵ halfvx��vy,vz,�����۵ļ��ٶ�ֵ
		//������(0,0,1)ת�������������ϵ�������Ҳ�������۵ļ��ٶ�
		halfvx = q1 * q3 - q0 * q2;
		halfvy = q0 * q1 + q2 * q3;
		halfvz = q0 * q0 - 0.5f + q3 * q3;
		// �� a ������ b ����������Ĺ�ʽ���£��õ� e �����������������
		halfex = (ay * halfvz - az * halfvy);
		halfey = (az * halfvx - ax * halfvz);
		halfez = (ax * halfvy - ay * halfvx);
		// ����Ki����0 ������΢��
		if(twoKi > 0.0f) {
			integralFBx += twoKi * halfex * (1.0f / sampleFreq);
			integralFBy += twoKi * halfey * (1.0f / sampleFreq);
			integralFBz += twoKi * halfez * (1.0f / sampleFreq);
			gx += integralFBx;	
			gy += integralFBy;
			gz += integralFBz;
		}
		else {
			integralFBx = 0.0f;	
			integralFBy = 0.0f;
			integralFBz = 0.0f;
		}
		// ��������P���ڣ�Ȼ���ۼӵ������ǵ������ϣ�����������
		gx += twoKp * halfex;
		gy += twoKp * halfey;
		gz += twoKp * halfez;
	}

	// 0.5f����ΪTWOk *1/500��ΪƵ��Ϊ500HZ��һ���ۼӼ�����500��
	gx *= (0.5f * (1.0f / sampleFreq));		
	gy *= (0.5f * (1.0f / sampleFreq));
	gz *= (0.5f * (1.0f / sampleFreq));
	qa = q0;
	qb = q1;
	qc = q2;
	//һ������-������ ͨ��������������������Ԫ�� ������Ԥ��ļ��ٶȼ�
	q0 += (-qb * gx - qc * gy - q3 * gz);
	q1 += (qa * gx + qc * gz - q3 * gy);
	q2 += (qa * gy - qb * gz + q3 * gx);
	q3 += (qa * gz + qb * gy - qc * gx);
   //��ֵ��Χ���-1 ��+1 ֮��
	recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q0 *= recipNorm;
	q1 *= recipNorm;
	q2 *= recipNorm;
	q3 *= recipNorm;
}
/**************************ʵ�ֺ���*********************************************************************
��  ����static float invSqrt(float x) 
������: ���ټ��� 1/Sqrt(x) 	
��  ����Ҫ�����ֵ
����ֵ�����
��  ע������ͨSqrt()����Ҫ���ı�See: http://en.wikipedia.org/wiki/Fast_inverse_square_root
*********************************************************************************************************/
float invSqrt(float x) {
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}

//====================================================================================================
// END OF CODE
//====================================================================================================
