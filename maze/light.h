#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "main.h"
#include "core.h"

typedef struct SUNLIGHT {
	D3DLIGHT9		lightSun;
	D3DLIGHT9		lightMoon;
	BOOL			isDayLight;

	SUNLIGHT();
	~SUNLIGHT();

	void			Update();
	D3DXCOLOR		GetFogColor();
	D3DXCOLOR		GetSkyColor();
	D3DXCOLOR		GetSunColor();
} SUNLIGHT;

#endif // !__LIGHT_H__
