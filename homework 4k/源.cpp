#include <iostream>
#include <graphics.h>
#include <Windows.h> // 实现更复杂的游戏逻辑，比如持续检测按键、方向键控制等，可以结合GetAsyncKeyState()来做实时检测
#include <math.h>
#include <fstream> // 用于读取json文件

#include "json.hpp" // 用于解析json文件

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

	RemoveFontResourceEx(L"res\\Jersey10-Regular.ttf", FR_PRIVATE, NULL) ; // 释放临时字体资源
	//closegraph() ;

	return 0 ;
}