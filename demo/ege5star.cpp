////////////////////////////////////////
/// @file ege5star.cpp
/// @brief ege�������� demo��ʵ���������ת����
///
/// 1. ʵ�ֶ�����ܡ�
/// 2. ���ݽǶȻ�������ǡ�
///
/// @date 2023-07-07 (��������: 2020-10-25)
///
////////////////////////////////////////

#include <graphics.h>
#include <time.h>
#include <math.h>

const double rotatingSpeed = -0.03; ///< ��ת�ٶȣ���λ������/�룩
const double fullCircleRatation = PI * 2; ///< ����Բ�ܽǶȣ���λ�����ȣ�
const double starAngle = PI * 4 / 5; ///< ����ǽǶ�

/**
  
  @brief ���������
  @param x ���ĵ� x ����
  @param y ���ĵ� y ����
  @param r �뾶
  @param a ��ת�Ƕ�
 */
void paintstar(double x, double y, double r, double a)
{
	int pt[10];
	for (int n = 0; n < 5; ++n)
	{
		pt[n * 2] = (int)(-cos(starAngle * n + a) * r + x);
		pt[n * 2 + 1] = (int)(sin(starAngle * n + a) * r + y);
	}
	fillpoly(5, pt);
}
int main()
{
	initgraph(640, 480); // ��ʼ����ͼ����
	setrendermode(RENDER_MANUAL);
	double r = 0;
	for (; is_run(); delay_fps(60))
	{
		r += rotatingSpeed;
		if (r > fullCircleRatation)
			r -= fullCircleRatation;
		
		cleardevice(); // �����Ļ
		setcolor(EGERGB(0xff, 0xff, 0xff)); // ���û�����ɫ
		setfillcolor(EGERGB(0, 0, 0xff));
		paintstar(300, 200, 100, r); // ���������
	}
	return 0;
}
