#include "title.h"
#include "stage.h"
#include "sound.h"
#include "maze.h"
#include "ui_title.h"

STAGENAME stageTarget[TITLEMENU_COUNT] = {
	STAGE_EASYMODE, STAGE_HARDMODE, STAGE_EXIT
};

TITLE::TITLE(GAMESTAGE* state)
{
	menuSelected = 0;

	pStage = state;
	maze = new MAZE(31, 31);
	sky = new SKY();

	camera = new CAMERA(NULL, NULL);
	camera->v_tar = D3DXVECTOR3(0.0f, MAZE_BLOCK_HEIGHT, 0.0f);
	camera->v_up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	FLOAT dist = MAZE_BLOCK_SIZE * 8;
	D3DXMATRIX mtxRot;
	D3DXVECTOR4 vec = D3DXVECTOR4(0.0f, 0.0f, dist, 1.0f);
	D3DXMatrixRotationX(&mtxRot, D3DXToRadian(45.0f));
	D3DXVec4Transform(&vec, &vec, &mtxRot);
	rot = D3DXVECTOR3(vec.x, vec.y, vec.z);

	titleUI = new TITLEUI(this);
}

TITLE::~TITLE()
{
	SAFE_DELETE(camera);
	SAFE_DELETE(sky);
	SAFE_DELETE(maze);
	SAFE_DELETE(titleUI);
}

void TITLE::Update() {

	sky->Update();
	UpdateCamera();

	switch (pStage->newStage) {
	case STAGE_TITLE:
	default:
		if (DINPUT::KeyTriggered(DIK_UP) || DINPUT::ButtonTriggered(BUTTON_UP)) {
			menuSelected = (menuSelected + TITLEMENU_COUNT - 1) % TITLEMENU_COUNT;
			DSOUND::Play(SOUND_CLICK);
		}
		if (DINPUT::KeyTriggered(DIK_DOWN) || DINPUT::ButtonTriggered(BUTTON_DOWN)) {
			menuSelected = (menuSelected + 1) % TITLEMENU_COUNT;
			DSOUND::Play(SOUND_CLICK);
		}
		if (DINPUT::KeyTriggered(DIK_RETURN) || DINPUT::ButtonTriggered(BUTTON_START)) {
			pStage->SetStage(stageTarget[menuSelected]);
			DSOUND::Play(SOUND_CLICK);
		}
		titleUI->Update();
		break;
	}

}

void TITLE::UpdateCamera()
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();
	D3DXVECTOR4 vec;
	D3DXMATRIX mtxRot;
	D3DXMatrixRotationY(&mtxRot, D3DXToRadian(2.0f) * D3D::fAnimationRate);
	D3DXVec3Transform(&vec, &rot, &mtxRot);
	rot = D3DXVECTOR3(vec.x, vec.y, vec.z);
	camera->v_pos = camera->v_tar - rot;

	// ビューマトリックスの作成
	D3DXMatrixIdentity(&camera->mtxView);
	D3DXMatrixLookAtLH(&camera->mtxView,
		&camera->v_pos,		// カメラの視点
		&camera->v_tar,		// カメラの注視点
		&camera->v_up);		// カメラの上方向

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &camera->mtxView);
}

HRESULT TITLE::Draw()
{
	camera->SetCamera();

	sky->Draw(camera);

	//D3D::EnableFog(sky->light->GetFogColor());
	maze->Draw(camera);
	//D3D::DisableFog();

#if _DEBUG
	WCHAR str[256];
	swprintf_s(str, _T("menuSelected %d"), menuSelected);
	D3D::ShowText(str, 0, 0);
#endif

	switch (pStage->newStage) {
	case STAGE_TITLE:
		titleUI->Draw();
		break;
	}

	return S_OK;
}
