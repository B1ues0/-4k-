#include <iostream>
#include <graphics.h>
#include <Windows.h>
#include <math.h>
#include <fstream>

#include "json.hpp"

#include "start.h"

using namespace std ;

int Start::select_number ;
int Start::select_id ;

LOGFONT Start::L_welcome_title ;
LOGFONT Start::L_welcome_press ;
LOGFONT Start::L_select_song_name ;
LOGFONT Start::L_select_song_composer ;
LOGFONT Start::L_select_song_difficulty ;
LOGFONT Start::L_select_song_grade ;
LOGFONT Start::L_select_gardename ;

void Start::StartGame()
{
	initgraph(1280, 720) ; // ��������
	setorigin(640, 720) ; // ����ԭ�㶨���м�����·�
	//circle(0, 0, 100); // ����
}

void Start::FortSetting()
{
	AddFontResourceEx(L"resourse\\font\\Jersey10-Regular.ttf", FR_PRIVATE, NULL) ; // ��ʱ����������Դ������ʹ�����·�������·����

	// ����L_welcome_title
	gettextstyle(&Start::L_welcome_title) ; // ��ȡĬ����������
	Start::L_welcome_title.lfHeight = 60 ; // �����ı��߶�
	Start::L_welcome_title.lfWidth = 30 ; // �����ı����
	_tcscpy_s(Start::L_welcome_title.lfFaceName, _T("Jersey 10")) ; // ��������ΪJersey 10
	Start::L_welcome_title.lfItalic = false ; // ����������б��
	Start::L_welcome_title.lfUnderline = false ; // �����������»���
	Start::L_welcome_title.lfQuality = ANTIALIASED_QUALITY ; // �������Ч��Ϊ�����

	// ����L_welcome_press
	gettextstyle(&Start::L_welcome_press) ;
	Start::L_welcome_press.lfHeight = 30 ;
	Start::L_welcome_press.lfWidth = 15 ;
	_tcscpy_s(Start::L_welcome_press.lfFaceName, _T("Jersey 10")) ;
	Start::L_welcome_press.lfItalic = false ;
	Start::L_welcome_press.lfUnderline = false ;
	Start::L_welcome_press.lfQuality = ANTIALIASED_QUALITY ;

	// ����L_select_song_name
	gettextstyle(&Start::L_select_song_name) ;
	Start::L_select_song_name.lfHeight = 37 ;
	Start::L_select_song_name.lfWidth = 23 ;
	_tcscpy_s(Start::L_select_song_name.lfFaceName, _T("Jersey 10")) ;
	Start::L_select_song_name.lfItalic = false ;
	Start::L_select_song_name.lfUnderline = false ;
	Start::L_select_song_name.lfQuality = ANTIALIASED_QUALITY ;

	// ����L_select_song_composer
	gettextstyle(&Start::L_select_song_composer) ;
	Start::L_select_song_composer.lfHeight = 14 ;
	Start::L_select_song_composer.lfWidth = 12 ;
	_tcscpy_s(Start::L_select_song_composer.lfFaceName, _T("Jersey 10")) ;
	Start::L_select_song_composer.lfItalic = false ;
	Start::L_select_song_composer.lfUnderline = false ;
	Start::L_select_song_composer.lfQuality = ANTIALIASED_QUALITY ;

	// ����L_select_song_difficulty
	gettextstyle(&Start::L_select_song_difficulty) ;
	Start::L_select_song_difficulty.lfHeight = 12 ;
	Start::L_select_song_difficulty.lfWidth = 12 ;
	_tcscpy_s(Start::L_select_song_difficulty.lfFaceName, _T("Jersey 10")) ;
	Start::L_select_song_difficulty.lfItalic = false ;
	Start::L_select_song_difficulty.lfUnderline = false ;
	Start::L_select_song_difficulty.lfQuality = ANTIALIASED_QUALITY ;
}

bool isAnyKeyPressed() {
	for (int i = 0x01; i <= 0xFE; i++) // �������п��ܵ�������� 
	{   
		if (GetAsyncKeyState(i) & 0x8000) // ����ü������£��� GetAsyncKeyState(i) �ķ���ֵ�� 0x8000 ������λ�롱��������������Ϊ 0��˵���ü����ڱ����£�
		{
			cout << "��⵽���������" << endl ;
			return true ;
		}
	}
	return false;
}

void Start::Welcome()
{
	// 1. ��ʾ��Ϸ����͡�Press any key to start��
	TCHAR text_welcome_title[20];
	_tcscpy_s(text_welcome_title, 20, _T("4kGame"));
	settextstyle(&Start::L_welcome_title) ; // ����������ʽ
	settextcolor(WHITE);
	outtextxy(0 - textwidth(text_welcome_title) / 2, -360 - textheight(text_welcome_title) / 2, text_welcome_title) ; // ����Ļ���Ļ�����Ϸ����

	TCHAR text_welcome_press[30];
	_tcscpy_s(text_welcome_press, 30, _T("Press any key to start"));
	settextstyle(&Start::L_welcome_press) ; // ����������ʽ
	settextcolor(WHITE);
	outtextxy(0 - textwidth(text_welcome_press) / 2, -180 - textheight(text_welcome_press) / 2, text_welcome_press) ; // ����Ļˮƽ���У���ֱ���·�����Press any key to start


	// 2. �ȴ������������Ȼ������
	FlushBatchDraw() ; // �ֶ�ˢ�»�ͼ����������������ʾ����

	while (!isAnyKeyPressed()) // �ȴ������������
	{
		Sleep(10) ; // ���� CPU ռ�ù���
	}
	cleardevice() ;
	//circle(0, 0, 100); // ����
}

void drawRotatedSquare(int centerX, int centerY, int size, double angle) // ������ת��������
{
	const double PI = 3.1415926535 ; // Բ����

	double rad = angle * PI / 180.0 ; // ���Ƕ�ת��Ϊ����

	double halfDiagonal = sqrt(2) * (size / 2.0) ; // ���������εİ�Խ��߳���

	POINT pts[4] ; // �����ε��ĸ����㣨��������ģ�

	for (int i = 0; i < 4; ++i)
	{
		double theta = i * PI / 2 + rad; // ÿ������� 90 �ȣ���������ת��
		pts[i].x = centerX + cos(theta) * halfDiagonal ;
		pts[i].y = centerY + sin(theta) * halfDiagonal ;
	}

	// ʹ�ö���κ�������������
	polygon(pts, 4);
}

void Start::DrawSelect()
{
	setorigin(0, 360) ; // ����ԭ�㶨���������м�

	BeginBatchDraw() ;

	setlinecolor(WHITE) ;
	rectangle(0, 77, 780, -77);

	int cover_length = 324 ;
	rectangle(1035 - ( cover_length / 2 ), cover_length / 2, 1035 + ( cover_length / 2 ), - ( cover_length / 2 ) ) ; // ���Ҳ������ˮƽ����
	drawRotatedSquare(1035, 0, cover_length, 12) ; // ���Ҳ��������б����

	FlushBatchDraw() ;
	EndBatchDraw() ;
}

void Start::Select()
{
	std::ifstream i("resource\\savefile\\savefile.json") ; // ��savefile.json

	if (!i.is_open())
	{
		std::cerr << "�޷���savefile.json" << std::endl ;
	} 


}