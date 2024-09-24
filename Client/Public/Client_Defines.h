#pragma once

#include <process.h>
#include <locale>
#include <codecvt>

namespace Client
{
	const unsigned int	g_iWinSizeX = { 1600 };
	const unsigned int	g_iWinSizeY = { 900 };	

	enum LEVELID { LEVEL_STATIC , LEVEL_BIN ,LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY , LEVEL_BOSS , LEVEL_END };
	enum SOUNDID { SOUND_BGM, SOUND_SFX };
}

extern HINSTANCE	g_hInst;
extern HWND			g_hWnd;

using namespace Client;
