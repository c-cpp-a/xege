/**********************************************************************
 * �ļ�����egelines.cpp
 * 
 * ����Ŀ�ģ�ʵ�ֱ��������Ч��
 * 
 * ʹ�õ�ͼ�ο⣺EGE
 * 
 **********************************************************************/

#include <graphics.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "ege/fps.h"

int width = 640, height = 480;

/**
 * @brief ��ṹ�壬�������������ٶ�
 */
struct point
{
	double x;   /**< ��ĺ����� */
	double y;   /**< ��������� */
	double dx;  /**< ���ں����ϵ��ٶ� */
	double dy;  /**< ���������ϵ��ٶ� */
};

/**
 * @brief ����νṹ�壬������ĸ����͵������
 */
struct poly
{
	int n_point;        /**< ��ĸ��� */
	point p[20];        /**< ������� */
};

/**
 * @brief ����ζ�����ṹ��
 */
struct polys
{
	int n_poly;                 /**< ����ζ��еĳ��� */
	int color;                  /**< ��ǰ��ɫ */
	int nextcolor, prevcolor;   /**< ��һ�ε���ɫ��Ŀ����ɫ */
	int chtime, nowtime;        /**< ���ɱ仯ʱ��͵�ǰʱ�� */
	int time;                   /**< �����´θı���ɫ��ʱ�� */
	poly p[100];                /**< ��������� */
};

/**
 * @brief ����һ��λ�� [db, db+dv] ��Χ�ڵ����������
 * @param dv �������Χ�Ĵ�С
 * @param db �������Χ����ʼ��
 * @return ������ɵĸ�����
 */
double rand_float(double dv, double db)
{
	return randomf() * dv + db;
}

/**
 * @brief ���ݵ���ٶ������ƶ����λ�ã����Ƴ���Ļ����з�������
 * @param b ��Ҫ�ƶ��ĵ�
 */
void movepoint(struct point* b)
{
	double dv = 1.0, db = 0.5;
	double tw = width / 640.0, th = height / 480.0;
	
	if (b->x < 0)
		b->dx = rand_float(dv, db) * tw;
	if (b->y < 0)
		b->dy = rand_float(dv, db) * th;
	if (b->x > width)
		b->dx = -rand_float(dv, db) * tw;
	if (b->y > height)
		b->dy = -rand_float(dv, db) * th;
	
	b->x += b->dx;
	b->y += b->dy;
}

/**
 * @brief �ƶ���������Σ��ڲ����õ���ƶ�
 * @param p ��Ҫ�ƶ��Ķ����
 */
void movepoly(struct poly* p)
{
	int i;
	
	for (i = 0; i < p->n_point; ++i)
	{
		movepoint(&(p->p[i]));
	}
}

/**
 * @brief �ƶ�����ζ��У�����ʱ�������ɫ����
 * @param p ��Ҫ�ƶ��Ķ���ζ���
 */
void movepolys(struct polys* p)
{
	int i;
	
	for (i = p->n_poly - 1; i > 0; --i)
	{
		p->p[i] = p->p[i - 1];
	}
	
	movepoly(p->p);
	++(p->nowtime);
	
	if (--(p->time) <= 0)
	{
		p->prevcolor = p->color;
		p->nextcolor = HSVtoRGB((float)random(360), 1.0f, (float)rand_float(0.5, 0.5));
		p->time = random(1000);
		p->chtime = random(1000) + 60;
		p->nowtime = 0;
	}
	
	if (p->nowtime >= p->chtime)
	{
		p->color = p->nextcolor;
	}
	else
	{
		double dr = p->prevcolor & 0xFF, dg = (p->prevcolor >> 8) & 0xFF, db = (p->prevcolor >> 16) & 0xFF;
		double dt = 1 - p->nowtime / (double)(p->chtime);
		
		dr -= p->nextcolor & 0xFF, dg -= (p->nextcolor >> 8) & 0xFF, db -= (p->nextcolor >> 16) & 0xFF;
		dr *= dt, dg *= dt, db *= dt;
		dr += p->nextcolor & 0xFF, dg += (p->nextcolor >> 8) & 0xFF, db += (p->nextcolor >> 16) & 0xFF;
		
		p->color = ((int)dr) | ((int)dg << 8) | ((int)db << 16);
	}
}

/**
 * @brief ��ʼ������ζ�����
 * @param p ����ζ�����
 * @param npoly ����ζ��еĳ���
 * @param npoint ������е�ĸ���
 */
void initpolys(struct polys* p, int npoly, int npoint)
{
	int i, j;
	
	p->n_poly = npoly;
	p->color = 0;
	p->time = 1000;
	p->prevcolor = p->color;
	p->nextcolor = HSVtoRGB((float)random(360), 1.0f, 0.5f);
	p->chtime = 1000;
	p->nowtime = 0;
	
	j = 0;
	p->p[j].n_point = npoint;
	
	for (i = 0; i < npoint; ++i)
	{
		p->p[j].p[i].x = random(width);
		p->p[j].p[i].y = random(height);
		p->p[j].p[i].dx = (randomf() * 2 + 1);
		p->p[j].p[i].dy = (randomf() * 2 + 1);
	}
	
	for (j = 1; j < npoly; ++j)
	{
		p->p[i] = p->p[i - 1];
	}
}

/**
 * @brief
  
  ����һ�������
 * @param p ����ζ���
 * @param color ��ɫֵ
 */
void draw_poly(struct poly* p, int color)
{
	int points[100];
	int i;
	
	for (i = 0; i < p->n_point; ++i)
	{
		points[i * 2] = (int)(p->p[i].x + 0.5f);
		points[i * 2 + 1] = (int)(p->p[i].y + 0.5f);
	}
	
	points[i * 2] = (int)(p->p[0].x + 0.5f);
	points[i * 2 + 1] = (int)(p->p[0].y + 0.5f);
	
	setcolor(color);
	drawpoly(p->n_point + 1, points);
}

/**
 * @brief ���ƶ���ζ��еĶ���Σ�ֻ���Ƶ�һ�������һ�������һ�����ڲ�����
 * @param p ����ζ��ж���
 */
void draw_polys(struct polys* p)
{
	draw_poly(&(p->p[p->n_poly - 1]), 0);
	draw_poly(&(p->p[0]), p->color);
}

int main()
{
	static struct polys p[10] = {{0}};
	int n_points[10] = {4, 3, 5, 6, 7};
	int n_poly[10] = {80, 40, 10, 5, 1};
	int n_polys = 2, i;
	randomize();
	
	// ͼ�γ�ʼ��
{
	setinitmode(1, 0, 0);
	initgraph(-1, -1);
	width = getmaxx();
	height = getmaxy();
	setrendermode(RENDER_MANUAL);
}
	
	// ����ζ����ʼ��
	for (i = 0; i < n_polys; ++i)
	{
		initpolys(&p[i], n_poly[i], n_points[i]);
	}
	
	setfont(12, 6, "����");
	fps ui_fps;
	
	// ��ѭ��
	for (; is_run(); delay_fps(60))
	{
		if (kbhit() > 0) // �а������¾��˳�
		{
			break;
		}
		
		for (i = 0; i < n_polys; ++i)
		{
			movepolys(&(p[i]));
		}
		
		for (i = 0; i < n_polys; ++i)
		{
			draw_polys(&(p[i]));
		}
	}
	
	closegraph();
	return 0;
}

