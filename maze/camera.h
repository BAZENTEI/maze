/***********************************************
 * [camera.h]
 * カメラ処理
 * GP11A341 24 張馳騁
 ***********************************************/

#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "main.h"
#include "core.h"
#include "gamemain.h"
#include "input.h"
#include "player.h"

typedef struct CAMERA {
	PLAYER*					player;
	D3DVIEWPORT9			viewport;
	D3DXVECTOR3				v_tar;
	D3DXVECTOR3				v_pos;
	D3DXVECTOR3				v_up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMATRIX				mtxView;
	D3DXMATRIX				mtxProj;

	CAMERA(PLAYER* player, RECT* rect);

	D3DRECT					GetRect(void);
	D3DXMATRIX*				GetView(void);
	void					SetCamera(void);
	void					Update(void);
} CAMERA;

#endif // !__CAMERA_H__
