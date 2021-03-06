#include "player.h"
#include "camera.h"
#include "game.h"
#include "input.h"
#include "item.h"
#include "maze.h"
#include "portal.h"
#include "sound.h"
#include "star.h"

#define PLAYER_LIGHT_ID		0x10

typedef enum ACTION {
	ACTION_TURNLEFT,
	ACTION_TURNRIGHT,
	ACTION_LOOKUP,
	ACTION_LOOKDOWN,
	ACTION_JUMP,
	ACTION_CONSUME,
	ACTION_MOVE,

	ACTION_COUNT
} ACTION;

BYTE KEYBOARDMAP[ACTION_COUNT] = { 
	DIK_A, DIK_D, DIK_UP, DIK_DOWN, DIK_SPACE, DIK_E, DIK_W
};
DWORD GAMEPADMAP[ACTION_COUNT] = {
	BUTTON_LEFT, BUTTON_RIGHT, BUTTON_DOWN, BUTTON_UP, BUTTON_LT, BUTTON_RT, BUTTON_A
};

PLAYER::PLAYER(MAINGAME* game, int nId, int nTotal)
{
	pGame = game;
	pMaze = game->maze;
	id = nId;
	bDoOnceFlag = TRUE;
	bJumping = FALSE;
	bMoveable = FALSE;
	fSize = PLAYER_SIZE;
	fJumpSpeed = 0.0f;
	fMoveSpeed = MOVE_MIN_SPEED;
	nStarGained = 0;
	fStamina = fStaminaMax = FLT_MAX;
	fTimeElapsed = 0.0f;

	RECT rect = { (LONG)(CL_WIDTH  * id / nTotal), 0, (LONG)(CL_WIDTH * (id + 1) / nTotal), CL_HEIGHT };
	camera = new CAMERA(this, &rect);


#if _DEBUG
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();
	pGuideLine = NULL;
	D3DXCreateLine(pDevice, &pGuideLine);
	pGuideLine->SetWidth(10.0f);
	pGuideLine->SetAntialias(TRUE);
#endif
	guideVtx.clear();

	guideStar = new STAR(pMaze, this);
}

PLAYER::~PLAYER()
{
	SAFE_DELETE(camera);
	SAFE_DELETE(guideStar);
#if _DEBUG
	SAFE_RELEASE(pGuideLine);
#endif // _DEBUG
}

HRESULT PLAYER::Draw(CAMERA* camera)
{
#if _DEBUG
	D3DXMATRIX mtx;
	D3DXMATRIX* mtxView = &camera->mtxView;
	D3DXMATRIX* mtxProj = &camera->mtxProj;

	D3DXMatrixMultiply(&mtx, mtxView, mtxProj);
	if (guideVtx.size() > 1) {
		pGuideLine->Begin();
		pGuideLine->DrawTransform((D3DXVECTOR3*)&guideVtx[0], guideVtx.size(), &mtx, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
		pGuideLine->End();
	}
#endif // _DEBUG
	guideStar->Draw();

	return S_OK;
}

void PLAYER::Update()
{
	if (bMoveable) {
		fTimeElapsed += D3D::fAnimationRate;
	}
	if (bMoveable && bDoOnceFlag) {
		bDoOnceFlag = FALSE;

		pGame->maze->SetItems(this);
		fStamina = fStaminaMax = (FLOAT)guideVtx.size();
	}
	_update_consume();
	_update_movement();
	_update_path();
	//
	guideStar->Update();
	camera->Update();
}

void PLAYER::_update_consume(void)
{
	if (DINPUT::KeyTriggered(KEYBOARDMAP[ACTION_CONSUME]) ||
		DINPUT::ButtonTriggered(GAMEPADMAP[ACTION_CONSUME]))
	{
		if (nStarGained > 0) {
			nStarGained -= 1;
			switch (pGame->gameLevel) {
			case LEVEL_HARD:
				fStamina += fStaminaMax * 0.25f;
				fStamina = min(fStamina, fStaminaMax);
				break;
			case LEVEL_EASY:
			default:
				fStamina = fStaminaMax;
				break;
			}
		}
	}

}

void PLAYER::_update_jumping(void)
{
	const FLOAT a = PLAYER_GRAVITY * D3D::fAnimationRate;

	if (bJumping) {
		fJumpSpeed -= a;
		srt.pos.y += fJumpSpeed;
		if (srt.pos.y < MAZE_BLOCK_HEIGHT * 0.5f) {
			srt.pos.y = MAZE_BLOCK_HEIGHT * 0.5f;
			bJumping = FALSE;
		}
	}
	else if (DINPUT::KeyTriggered(KEYBOARDMAP[ACTION_JUMP]) || DINPUT::ButtonTriggered(GAMEPADMAP[ACTION_JUMP])) {
		bJumping = TRUE;
		fJumpSpeed = PLAYER_JUMP_SPEED;
		DSOUND::Play(SOUND_MOVEMENT);
	}
}

inline void PLAYER::_update_line(vector<NODE*> path)
{
	int nGuideLength = path.size();
	bMoveable = TRUE;
	guideVtx.clear();
	for (int i = 0; i < nGuideLength; i++) {
		guideVtx.push_back(pMaze->GetCellPosition(path[i]->x, path[i]->y));
		guideVtx[i].y = 10.0f;
	}
}

inline void PLAYER::_update_movement()
{
	if (!bMoveable || pGame->bGameOver) {
		return;
	}

	FLOAT rate = D3D::fAnimationRate;
	_update_jumping();

	static float moveTime = 0.0f;
	// Move
	if (DINPUT::KeyPressed(KEYBOARDMAP[ACTION_MOVE]) ||
			DINPUT::ButtonPressed(GAMEPADMAP[ACTION_MOVE])) {
		fMoveSpeed += MOVE_STEP * rate;
		if (fMoveSpeed >= MOVE_MAX_SPEED)
			fMoveSpeed = MOVE_MAX_SPEED;

		moveTime += rate;
		if(moveTime > 1.0f){
			DSOUND::Play(SOUND_MOVEMENT);
			moveTime -= 1.0f;
		}
	}
	else {
		moveTime = 0.0f;
		fMoveSpeed = 0.0f;
	}

	// 垂直方向調整
	if (DINPUT::KeyPressed(KEYBOARDMAP[ACTION_LOOKUP]) && DINPUT::KeyPressed(KEYBOARDMAP[ACTION_LOOKDOWN]) ||
		DINPUT::ButtonPressed(GAMEPADMAP[ACTION_LOOKUP]) && DINPUT::ButtonPressed(GAMEPADMAP[ACTION_LOOKDOWN])) {
	}
	else if (DINPUT::KeyPressed(KEYBOARDMAP[ACTION_LOOKDOWN]) ||
		DINPUT::ButtonPressed(GAMEPADMAP[ACTION_LOOKDOWN])) {
			
		srt.rot.x -= PITCH_STEP * rate;
		if (srt.rot.x <= -PITCH_MAX_ANGLE)
			srt.rot.x = -PITCH_MAX_ANGLE;
	}
	else if (DINPUT::KeyPressed(KEYBOARDMAP[ACTION_LOOKUP]) ||
		DINPUT::ButtonPressed(GAMEPADMAP[ACTION_LOOKUP])) {
			
		srt.rot.x += PITCH_STEP * rate;
		if (srt.rot.x >= PITCH_MAX_ANGLE)
			srt.rot.x = PITCH_MAX_ANGLE;
	}

	// 水平方向調整
	if (DINPUT::KeyPressed(KEYBOARDMAP[ACTION_TURNLEFT]) && DINPUT::KeyPressed(KEYBOARDMAP[ACTION_TURNRIGHT]) ||
		DINPUT::ButtonPressed(GAMEPADMAP[ACTION_TURNLEFT]) && DINPUT::ButtonPressed(GAMEPADMAP[ACTION_TURNRIGHT])) {
	}
	else if (DINPUT::KeyPressed(KEYBOARDMAP[ACTION_TURNLEFT]) ||
		DINPUT::ButtonPressed(GAMEPADMAP[ACTION_TURNLEFT])) {
		srt.rot.y -= YAW_STEP * rate;
	}
	else if (DINPUT::KeyPressed(KEYBOARDMAP[ACTION_TURNRIGHT]) ||
		DINPUT::ButtonPressed(GAMEPADMAP[ACTION_TURNRIGHT])) {
		srt.rot.y += YAW_STEP * rate;
	}

	D3DXVECTOR3 dir = D3DXVECTOR3(fMoveSpeed * rate, 0.0f, 0.0f);
	D3DXVECTOR4 temp;
	D3DXMATRIX mtx;

	D3DXMatrixRotationY(&mtx, srt.rot.y);
	D3DXVec3Transform(&temp, &dir, &mtx);

	D3DXVECTOR3 l = srt.pos + D3DXVECTOR3(-fSize, 0.0f, 0.0f);
	D3DXVECTOR3 r = srt.pos + D3DXVECTOR3(fSize, 0.0f, 0.0f);
	D3DXVECTOR3 t = srt.pos + D3DXVECTOR3(0.0f, 0.0f, -fSize);
	D3DXVECTOR3 b = srt.pos + D3DXVECTOR3(0.0f, 0.0f, fSize);
	D3DXVECTOR3 lt = srt.pos + D3DXVECTOR3(-fSize, 0.0f, -fSize);
	D3DXVECTOR3 rt = srt.pos + D3DXVECTOR3(fSize, 0.0f, -fSize);
	D3DXVECTOR3 lb = srt.pos + D3DXVECTOR3(-fSize, 0.0f, fSize);
	D3DXVECTOR3 rb = srt.pos + D3DXVECTOR3(fSize, 0.0f, fSize);

	if (temp.x == 0.0f && temp.z != 0.0f) {
		if (pMaze->GetPositionCell(t)  ||
			pMaze->GetPositionCell(lt) ||
			pMaze->GetPositionCell(rt) ||
			pMaze->GetPositionCell(b)  ||
			pMaze->GetPositionCell(lb) ||
			pMaze->GetPositionCell(rb))
			temp.z = 0.0f;
	}
	else if (temp.x != 0.0f && temp.z == 0.0f) {
		if (pMaze->GetPositionCell(l)  ||
			pMaze->GetPositionCell(lt) ||
			pMaze->GetPositionCell(lb) ||
			pMaze->GetPositionCell(r)  ||
			pMaze->GetPositionCell(rt) ||
			pMaze->GetPositionCell(rb))
			temp.x = 0.0f;
	}

#define COMPARE(a, b) {\
	if (fabsf(a) > fabsf(b)) b = 0.0f; \
	else if (fabsf(a) < fabsf(b)) a = 0.0f; \
	else a = b = 0.0f; }

	if (temp.x < 0.0f && temp.z < 0.0f) {
		if (pMaze->GetPositionCell(l) || pMaze->GetPositionCell(lb)) temp.x = 0.0f;
		if (pMaze->GetPositionCell(t) || pMaze->GetPositionCell(rt)) temp.z = 0.0f;
		if (pMaze->GetPositionCell(lt)) {
			if (!(pMaze->GetPositionCell(l) || pMaze->GetPositionCell(t))) COMPARE(temp.x, temp.z);
		}
	}
	if (temp.x > 0.0f && temp.z > 0.0f) {
		if (pMaze->GetPositionCell(r) || pMaze->GetPositionCell(rt)) temp.x = 0.0f;
		if (pMaze->GetPositionCell(b) || pMaze->GetPositionCell(lb)) temp.z = 0.0f;
		if (pMaze->GetPositionCell(rb)) {
			if (!(pMaze->GetPositionCell(r) || pMaze->GetPositionCell(b))) COMPARE(temp.x, temp.z);
		}
	}
	if (temp.x < 0.0f && temp.z > 0.0f) {
		if (pMaze->GetPositionCell(l) || pMaze->GetPositionCell(lt)) temp.x = 0.0f;
		if (pMaze->GetPositionCell(b) || pMaze->GetPositionCell(rb)) temp.z = 0.0f;
		if (pMaze->GetPositionCell(lb)) {
			if (!(pMaze->GetPositionCell(l) || pMaze->GetPositionCell(b))) COMPARE(temp.x, temp.z);
		}
	}
	if (temp.x > 0.0f && temp.z < 0.0f) {
		if (pMaze->GetPositionCell(r) || pMaze->GetPositionCell(rb)) temp.x = 0.0f;
		if (pMaze->GetPositionCell(t) || pMaze->GetPositionCell(lt)) temp.z = 0.0f;
		if (pMaze->GetPositionCell(rt)) {
			if (!(pMaze->GetPositionCell(r) || pMaze->GetPositionCell(t))) COMPARE(temp.x, temp.z);
		}
	}
#undef COMPARE
#if _DEBUG
	if (DINPUT::KeyPressed(DIK_C))
		temp.y = 100.0f * rate;
	if(DINPUT::KeyPressed(DIK_V))
		temp.y = -100.0f * rate;
	if (DINPUT::KeyPressed(DIK_B))
		srt.pos.y = 48.0f;
#endif

	if (fabsf(temp.x) > 0.001f || fabsf(temp.z) > 0.001f) {
		fStamina -= 1.0f * D3D::fAnimationRate;
		if (fStamina <= 0.0f) GameLose();
	}

	srt.pos += D3DXVECTOR3(temp.x, temp.y, temp.z);
}

inline void PLAYER::_update_path(void)
{
	int x, y;
	int w, h;
	pMaze->GetSize(w, h);
	pMaze->GetPositionCell(srt.pos, x, y);
	NODE* start = pMaze->GetNode(x, y);
	NODE* goal = pMaze->GetNode(pMaze->goal.x, pMaze->goal.y);

	pMaze->FindPath(start, goal, path);
	if (!path.empty())
		_update_line(path);
}

void PLAYER::AddStar(void)
{
	nStarGained += 1;
}

void PLAYER::GameWin(void)
{
	pGame->GameOver(0, GAME_RESULT_WIN);
}

void PLAYER::GameLose(void)
{
	pGame->GameOver(0, GAME_RESULT_LOSE);
}

void PLAYER::SetPosition(int x, int y)
{
	int t[4][2] = {
		{ 0 , -1 },
		{ -1 , 0 },
		{ 0 ,  1 },
		{  1 , 0 },
	};

	FLOAT rot_y = 0.0f;
	D3DXVECTOR3 dir;

	srt = SRT(pMaze->GetCellPosition(x, y));
	// Make player always facing to a path.
	for (int i = 0; i < 4; i++) {
		if (pMaze->GetCellType(x + t[i][0], y + t[i][1]) == 0) {
			dir = pMaze->GetCellPosition(x + t[i][0], y + t[i][1]) - pMaze->GetCellPosition(x, y);
			rot_y = atan2f(-dir.z, dir.x);
			break;
		}
	}
	srt.rot.y = rot_y;

	pMaze->portal[0] = new PORTAL(pMaze, x, y);
}
