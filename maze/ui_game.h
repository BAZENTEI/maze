#ifndef __UI_GAME_H__
#define __UI_GAME_H__

#include "main.h"
#include "core.h"
#include "d3d.h"
#include "gamemain.h"
#include "image.h"
#include "easing.h"
#include "ui_digits.h"

#define UI_MAX_DIGITS			6
#define UI_TIMER_DIGITS			3

typedef struct MAINGAMEUI {
	static LPDIRECT3DTEXTURE9	pTexStatus;
	static LPDIRECT3DTEXTURE9	pTexResult;

	MAINGAME*					pGame;

	IMAGE*						result[GM_MAX_PLAYER];
	DIGITIMAGE*					timer[UI_TIMER_DIGITS];
	IMAGE*						fadeOut;
	FLOAT						fadeVal;

	MAINGAMEUI(MAINGAME* game);
	~MAINGAMEUI();

	void						Update();
	HRESULT						Draw();

} MAINGAMEUI;

#endif // !__UI_GAME_H__
