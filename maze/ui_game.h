#ifndef __UI_GAME_H__
#define __UI_GAME_H__

#include "main.h"
#include "core.h"
#include "image.h"
#include "easing.h"
#include "gamemain.h"
#include "ui_digits.h"

#define UI_MAX_DIGITS			6
#define UI_TIMER_DIGITS			3

typedef struct MAINGAMEUI {
	static LPDIRECT3DTEXTURE9	pTexStatus;
	static LPDIRECT3DTEXTURE9	pTexResult;

	MAINGAME*					pGame;

	DIGITIMAGE*					score[GM_MAX_PLAYER][UI_MAX_DIGITS];
	DWORD						dwScoreAnimate[GM_MAX_PLAYER];
	EASING						mScoreEasing[GM_MAX_PLAYER];
	
	IMAGE*						result[GM_MAX_PLAYER];
	DIGITIMAGE*					timer[UI_TIMER_DIGITS];

	MAINGAMEUI(MAINGAME* game);
	~MAINGAMEUI();

	void						Update();
	HRESULT						Draw();

} MAINGAMEUI;

#endif // !__UI_GAME_H__
