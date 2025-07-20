#pragma once

#include <iostream>
#include <graphics.h>
#include <Windows.h>
#include <math.h>

#include "json.hpp"

using namespace std ;

class Start
{
public:
	static int select_number ;
	static int select_id ;

	// 定义LOGFONT结构体变量
	static LOGFONT L_welcome_title ;
	static LOGFONT L_welcome_press ;
	static LOGFONT L_select_song_name ;
	static LOGFONT L_select_song_composer ;
	static LOGFONT L_select_song_difficulty ;
	static LOGFONT L_select_song_grade ;
	static LOGFONT L_select_gardename ;

	static void StartGame() ;
	static void FortSetting() ;
	static void Welcome() ;
	static void DrawSelect() ;
	static void Select() ;
} ;