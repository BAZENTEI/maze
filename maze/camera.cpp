#include "camera.h"
#include "d3d.h"

#define CM_PITCH			D3DXToRadian(0.0f)
#define CM_DIST_FPS			100.0f
#define CM_DIST_TPS			50.0f

CAMERA::CAMERA(PLAYER* player = NULL, RECT* rect = NULL)
{
	DWORD height = CL_HEIGHT;
	DWORD width = CL_WIDTH;
	DWORD x = 0, y = 0;
	if (player && rect) {
		x = rect->left;
		y = rect->top;
		width = rect->right - rect->left;
		height = rect->bottom - rect->top;
		this->player = player;
	}
	this->viewport = { x, y, width, height, 0.0f, 1.0f };
}

D3DRECT CAMERA::GetRect(void)
{

	return { (LONG)viewport.X, (LONG)viewport.Y, (LONG)(viewport.X + viewport.Width), (LONG)(viewport.Y + viewport.Height) };
}

D3DXMATRIX * CAMERA::GetView(void)
{
	return &mtxView;
}

void CAMERA::SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();

	D3DXMatrixLookAtLH(&mtxView, &v_pos, &v_tar, &v_up);

	D3DXMatrixPerspectiveFovLH(&mtxProj,
		D3DXToRadian((FLOAT)GR_VIEWANGLE),
		(FLOAT)viewport.Width / (FLOAT)viewport.Height,
		GR_NEAR_Z, GR_FAR_Z);									

	pDevice->SetViewport(&viewport);
	pDevice->SetTransform(D3DTS_VIEW, &mtxView);
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProj);
}

void CAMERA::Update(void)
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();

	SRT srt = player->srt;
	v_pos = srt.pos;

	D3DXVECTOR4 up = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	D3DXVECTOR4 vec = D3DXVECTOR4(CM_DIST_FPS, 0.0f, 0.0f, 1.0f);
	D3DXMATRIX mtxRot;

	D3DXMatrixRotationYawPitchRoll(&mtxRot, srt.rot.y , srt.rot.z, srt.rot.x);
	D3DXVec4Transform(&up, &up, &mtxRot);

	v_up = D3DXVECTOR3(up.x, up.y, up.z);

	// カメラの座標変換
	D3DXMatrixRotationYawPitchRoll(&mtxRot, srt.rot.y, srt.rot.z, srt.rot.x);
	D3DXVec4Transform(&vec, &vec, &mtxRot);

	D3DXVECTOR3 dir = D3DXVECTOR3(vec.x, vec.y, vec.z);
	v_tar = v_pos + dir;
}
