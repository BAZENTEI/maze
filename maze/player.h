/***********************************************
 * [model.h]
 * プレイヤー処理
 * GP11A341 24 張馳騁
 ***********************************************/

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "main.h"
#include "core.h"
#include "gamemain.h"
#include "node.h"

#define YAW_MAX_ANGLE		D3DXToRadian(180.0f)
#define YAW_STEP			D3DXToRadian(180.0f)

#define PITCH_MAX_ANGLE		D3DXToRadian(90.0f)
#define PITCH_STEP			D3DXToRadian(180.0f)

#define MOVE_MAX_SPEED		256.0f
#define MOVE_MIN_SPEED		0.0f
#define MOVE_STEP			256.0f

#define PLAYER_GRAVITY		12.0f
#define PLAYER_JUMP_SPEED	(PLAYER_GRAVITY * 0.5f)
#define PLAYER_SIZE			32.0f

//
// プレイヤー情報
//

using namespace std;

typedef struct PLAYER {
	MAINGAME*				pGame;
	MAZE*					pMaze;
	CAMERA*					camera;
	STAR*					guideStar;
	
#if _DEBUG
	LPD3DXLINE				pGuideLine;
#endif // _DEBUG
	
	vector<NODE*>			path;
	vector<D3DXVECTOR3>		guideVtx;
	DWORD					id;
	BOOL					bDoOnceFlag;
	BOOL					bMoveable;
	BOOL					bJumping;
	FLOAT					fSize;
	FLOAT					fMoveSpeed;
	FLOAT					fJumpSpeed;
	int						nlastX;
	int						nlastY;
	int						nStarGained;
	SRT						srt;

	FLOAT					fStamina;
	FLOAT					fStaminaMax;
	FLOAT					fTimeElapsed;

	void					_update_consume(void);
	void					_update_jumping(void);
	void					_update_line(vector<NODE*> path);
	void					_update_movement(void);
	void					_update_path(void);

	PLAYER(MAINGAME* pGame, int playerId, int totalPlayer);
	~PLAYER();

	HRESULT					Draw(CAMERA* camera);
	void					Update();

	void					AddStar(void);
	void					GameWin(void);
	void					GameLose(void);
	void					SetPosition(int x, int y);
} PLAYER;

#endif // !__PLAYER_H__
