#include <iostream>
#include <graphics.h>
#include <Windows.h> // ʵ�ָ����ӵ���Ϸ�߼������������ⰴ������������Ƶȣ����Խ��GetAsyncKeyState()����ʵʱ���
#include <math.h>
#include <fstream> // ���ڶ�ȡjson�ļ�

#include "json.hpp" // ���ڽ���json�ļ�

#include "start.h"
#include "play.h"

using namespace std ;

int main()
{
	Start::StartGame() ;
	Start::FortSetting() ;
	Start::Welcome() ;
	//Start::DrawSelect() ;

	play() ;

	/*system("pause") ;*/

	RemoveFontResourceEx(L"res\\Jersey10-Regular.ttf", FR_PRIVATE, NULL) ; // �ͷ���ʱ������Դ
	//closegraph() ;

	return 0 ;
}