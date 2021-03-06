//
// [stage.h] ステージ遷移 
//

#ifndef __STAGE_H__
#define __STAGE_H__

//
// インクルード
//
#include "main.h"
#include "core.h"
#include "util.h"
#include "gamemain.h"
#include "game.h"
#include "title.h"

//
// ステージの枚挙
//
enum STAGENAME {
	STAGE_TITLE,
	STAGE_EASYMODE,
	STAGE_HARDMODE,
	STAGE_END,
	STAGE_EXIT,
};

typedef struct GAMESTAGE {
	UINT				oldStage;
	UINT				newStage;
	MAINGAME*			pStageGame;
	TITLE*				pStageTitle;

	GAMESTAGE(UINT iStageName);
	~GAMESTAGE(void);

	void				SetStage(UINT iStageName);
	void				Update(void);
	void				Draw(void);
} GAMESTAGE;


#endif // !__STAGE_H__
