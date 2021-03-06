#ifndef __UI_TITLE_H__
#define __UI_TITLE_H__

#include "main.h"
#include "core.h"
#include "d3d.h"
#include "easing.h"
#include "image.h"
#include "title.h"

#define TITLE_TEX_MAIN		_T("data/TEXTURE/ui/title.png")
#define TITLE_TEX_MENU		_T("data/TEXTURE/ui/menu.png")

typedef struct TITLEUI {
	TITLEUI(TITLE* title);
	~TITLEUI();

	void						SetTitleColor();
	void						Update();
	HRESULT						Draw();

	static LPDIRECT3DTEXTURE9	pTexMenu;
	static LPDIRECT3DTEXTURE9	pTexTitle;

	TITLE*						pGameTitle;

	IMAGE*						menu[TITLEMENU_COUNT];
	IMAGE*						title;
	FLOAT						fTitleGradientTimer;
} TITLEUI;

#endif // !__UI_TITLE_H__
