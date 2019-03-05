#ifndef __STAGE_H__
#define __STAGE_H__

#include "main.h"
#include "core.h"
#include "util.h"
#include "gamemain.h"
#include "title.h"

enum STAGETYPE {
	STAGE_TITLE,
	STAGE_EASYMODE,
	STAGE_HARDMODE,
	STAGE_END,
	STAGE_EXIT,
};

typedef struct GAMESTAGE {
	STAGETYPE			oldStage;
	STAGETYPE			newStage;
	MAINGAME*			pStateGame;
	TITLE*				pStateTitle;

	GAMESTAGE(STAGETYPE eStageType);
	~GAMESTAGE(void);

	void				SetStage(STAGETYPE eStageType);
	void				Update(void);
	void				Draw(void);
} GAMESTAGE;


#endif // !__STAGE_H__
