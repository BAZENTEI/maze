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
private:
	static LPDIRECT3DTEXTURE9	pTexStamina;
	static LPDIRECT3DTEXTURE9	pTexStar;

	MAINGAME*					pGame;

	IMAGE*						fadeOut;
	FLOAT						fadeVal;

	IMAGE*						staminaBar;
	IMAGE*						stamina;

	IMAGE*						stars[MAZE_STAR_NUM];
public:
	MAINGAMEUI(MAINGAME* game);
	~MAINGAMEUI();

	void						Update();
	HRESULT						Draw();

} MAINGAMEUI;

#endif // !__UI_GAME_H__
