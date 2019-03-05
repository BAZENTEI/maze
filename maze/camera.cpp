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

	ZeroMemory(&spot, sizeof(D3DLIGHT9));

	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();
	// ライトのタイプの設定
	spot.Type = D3DLIGHT_SPOT;
	// 拡散光
	spot.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	// 環境光
	spot.Ambient = D3DXCOLOR(0.25f,0.25f,0.25f,1.0f);

	spot.Phi = D3DXToRadian(90.0f);
	spot.Theta = D3DXToRadian(30.0f);
	// ライトの方向の設定
	//vecDir = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	// ライトをレンダリングパイプラインに設定
	pDevice->SetLight(2, &spot);
	pDevice->LightEnable(2, TRUE);
}

CAMERA::~CAMERA()
{
}

D3DXMATRIX * CAMERA::GetView(void)
{
	return &m_view;
}

void CAMERA::SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_view);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_view,
		&v_pos,		// カメラの視点
		&v_tar,		// カメラの注視点
		&v_up);		// カメラの上方向

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_proj);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&m_proj,
		D3DXToRadian((FLOAT)GR_VIEWANGLE),				// 視野角
		(FLOAT)viewport.Width / (FLOAT)viewport.Height,	// アスペクト比
		GR_NEAR_Z,										// ビュー平面のNearZ値
		GR_FAR_Z);										// ビュー平面のFarZ値

	pDevice->SetViewport(&viewport);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_view);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_proj);
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

	spot.Position = v_pos;
	D3DXVec3Normalize((D3DXVECTOR3*)&spot.Direction, &dir);

	pDevice->SetLight(2, &spot);
}
