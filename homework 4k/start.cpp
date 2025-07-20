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
	initgraph(1280, 720) ; // 创建窗口
	setorigin(640, 720) ; // 坐标原点定在中间的最下方
	//circle(0, 0, 100); // 测试
}

void Start::FortSetting()
{
	AddFontResourceEx(L"resourse\\font\\Jersey10-Regular.ttf", FR_PRIVATE, NULL) ; // 临时加载字体资源（可以使用相对路径或绝对路径）

	// 设置L_welcome_title
	gettextstyle(&Start::L_welcome_title) ; // 获取默认字体设置
	Start::L_welcome_title.lfHeight = 60 ; // 设置文本高度
	Start::L_welcome_title.lfWidth = 30 ; // 设置文本宽度
	_tcscpy_s(Start::L_welcome_title.lfFaceName, _T("Jersey 10")) ; // 设置字体为Jersey 10
	Start::L_welcome_title.lfItalic = false ; // 设置字体无斜体
	Start::L_welcome_title.lfUnderline = false ; // 设置字体无下划线
	Start::L_welcome_title.lfQuality = ANTIALIASED_QUALITY ; // 设置输出效果为抗锯齿

	// 设置L_welcome_press
	gettextstyle(&Start::L_welcome_press) ;
	Start::L_welcome_press.lfHeight = 30 ;
	Start::L_welcome_press.lfWidth = 15 ;
	_tcscpy_s(Start::L_welcome_press.lfFaceName, _T("Jersey 10")) ;
	Start::L_welcome_press.lfItalic = false ;
	Start::L_welcome_press.lfUnderline = false ;
	Start::L_welcome_press.lfQuality = ANTIALIASED_QUALITY ;

	// 设置L_select_song_name
	gettextstyle(&Start::L_select_song_name) ;
	Start::L_select_song_name.lfHeight = 37 ;
	Start::L_select_song_name.lfWidth = 23 ;
	_tcscpy_s(Start::L_select_song_name.lfFaceName, _T("Jersey 10")) ;
	Start::L_select_song_name.lfItalic = false ;
	Start::L_select_song_name.lfUnderline = false ;
	Start::L_select_song_name.lfQuality = ANTIALIASED_QUALITY ;

	// 设置L_select_song_composer
	gettextstyle(&Start::L_select_song_composer) ;
	Start::L_select_song_composer.lfHeight = 14 ;
	Start::L_select_song_composer.lfWidth = 12 ;
	_tcscpy_s(Start::L_select_song_composer.lfFaceName, _T("Jersey 10")) ;
	Start::L_select_song_composer.lfItalic = false ;
	Start::L_select_song_composer.lfUnderline = false ;
	Start::L_select_song_composer.lfQuality = ANTIALIASED_QUALITY ;

	// 设置L_select_song_difficulty
	gettextstyle(&Start::L_select_song_difficulty) ;
	Start::L_select_song_difficulty.lfHeight = 12 ;
	Start::L_select_song_difficulty.lfWidth = 12 ;
	_tcscpy_s(Start::L_select_song_difficulty.lfFaceName, _T("Jersey 10")) ;
	Start::L_select_song_difficulty.lfItalic = false ;
	Start::L_select_song_difficulty.lfUnderline = false ;
	Start::L_select_song_difficulty.lfQuality = ANTIALIASED_QUALITY ;
}

bool isAnyKeyPressed() {
	for (int i = 0x01; i <= 0xFE; i++) // 遍历所有可能的虚拟键码 
	{   
		if (GetAsyncKeyState(i) & 0x8000) // 如果该键被按下（将 GetAsyncKeyState(i) 的返回值与 0x8000 做“按位与”操作，如果结果不为 0，说明该键正在被按下）
		{
			cout << "检测到按下任意键" << endl ;
			return true ;
		}
	}
	return false;
}

void Start::Welcome()
{
	// 1. 显示游戏标题和“Press any key to start”
	TCHAR text_welcome_title[20];
	_tcscpy_s(text_welcome_title, 20, _T("4kGame"));
	settextstyle(&Start::L_welcome_title) ; // 设置字体样式
	settextcolor(WHITE);
	outtextxy(0 - textwidth(text_welcome_title) / 2, -360 - textheight(text_welcome_title) / 2, text_welcome_title) ; // 在屏幕中心绘制游戏标题

	TCHAR text_welcome_press[30];
	_tcscpy_s(text_welcome_press, 30, _T("Press any key to start"));
	settextstyle(&Start::L_welcome_press) ; // 设置字体样式
	settextcolor(WHITE);
	outtextxy(0 - textwidth(text_welcome_press) / 2, -180 - textheight(text_welcome_press) / 2, text_welcome_press) ; // 在屏幕水平居中，竖直靠下方绘制Press any key to start


	// 2. 等待按下任意键，然后清屏
	FlushBatchDraw() ; // 手动刷新绘图缓冲区，让文字显示出来

	while (!isAnyKeyPressed()) // 等待任意键被按下
	{
		Sleep(10) ; // 避免 CPU 占用过高
	}
	cleardevice() ;
	//circle(0, 0, 100); // 测试
}

void drawRotatedSquare(int centerX, int centerY, int size, double angle) // 绘制旋转的正方形
{
	const double PI = 3.1415926535 ; // 圆周率

	double rad = angle * PI / 180.0 ; // 将角度转换为弧度

	double halfDiagonal = sqrt(2) * (size / 2.0) ; // 计算正方形的半对角线长度

	POINT pts[4] ; // 正方形的四个顶点（相对于中心）

	for (int i = 0; i < 4; ++i)
	{
		double theta = i * PI / 2 + rad; // 每个点相差 90 度，并加上旋转角
		pts[i].x = centerX + cos(theta) * halfDiagonal ;
		pts[i].y = centerY + sin(theta) * halfDiagonal ;
	}

	// 使用多边形函数绘制正方形
	polygon(pts, 4);
}

void Start::DrawSelect()
{
	setorigin(0, 360) ; // 坐标原点定在最左侧的中间

	BeginBatchDraw() ;

	setlinecolor(WHITE) ;
	rectangle(0, 77, 780, -77);

	int cover_length = 324 ;
	rectangle(1035 - ( cover_length / 2 ), cover_length / 2, 1035 + ( cover_length / 2 ), - ( cover_length / 2 ) ) ; // 画右侧曲绘的水平方框
	drawRotatedSquare(1035, 0, cover_length, 12) ; // 画右侧曲绘的倾斜方框

	FlushBatchDraw() ;
	EndBatchDraw() ;
}

void Start::Select()
{
	std::ifstream i("resource\\savefile\\savefile.json") ; // 打开savefile.json

	if (!i.is_open())
	{
		std::cerr << "无法打开savefile.json" << std::endl ;
	} 


}