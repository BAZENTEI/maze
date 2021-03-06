#ifndef __SKY_H__
#define __SKY_H__

#include "main.h"
#include "core.h"
#include "gamemain.h"
#include "light.h"
#include "particle.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

typedef enum SKYENUM {
	SKY_FR,
	SKY_BK,
	SKY_LF,
	SKY_RT,
	SKY_UP,
	SKY_DN,

	SKY_NUM
} SKYENUM;

struct SKY {
	LPDIRECT3DTEXTURE9		pTex[SKY_NUM] = { NULL };
	LPDIRECT3DTEXTURE9		pTexSun = NULL;
	LPDIRECT3DVERTEXBUFFER9 pVtx = NULL;
	D3DXMATRIX				mtx;
	D3DXVECTOR3				pos;
	SUNLIGHT*				light;
	EMITTER*				sun;

	SKY();
	~SKY();

	HRESULT				SetSky();
	void				Update();
	void				Draw(CAMERA* pCamera);
};

#endif
