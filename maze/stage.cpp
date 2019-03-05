#include "stage.h"
#include "game.h"

GAMESTAGE::GAMESTAGE(STAGETYPE eStageType)
{
	pStateTitle = NULL;
	pStateGame = NULL;
	oldStage = newStage = eStageType;

	SetStage(eStageType);
}

GAMESTAGE::~GAMESTAGE(void)
{
	SAFE_DELETE(pStateTitle);
	SAFE_DELETE(pStateGame);
}

void GAMESTAGE::SetStage(STAGETYPE eStageType)
{
	newStage = eStageType;
}

void GAMESTAGE::Update(void)
{
	switch (newStage) {
	case STAGE_TITLE:
		if (!pStateTitle) {
			pStateTitle = new TITLE(this);
		}
		else {
			pStateTitle->Update();
		}
		break;
	case STAGE_EASYMODE:
		if (!pStateGame) {
			pStateGame = new MAINGAME(this, LEVEL_EASY);
		}
		else {
			pStateGame->Update();
		}
		break;
	case STAGE_HARDMODE:
		if (!pStateGame) {
			pStateGame = new MAINGAME(this, LEVEL_HARD);
		}
		else {
			pStateGame->Update();
		}
		break;
	case STAGE_EXIT:
		PostQuitMessage(0);
		break;
	}
}

void GAMESTAGE::Draw(void)
{
	switch (oldStage) {
	case STAGE_TITLE:
		if (pStateTitle) {
			pStateTitle->Draw();
		}
		break;
	case STAGE_EASYMODE:
	case STAGE_HARDMODE:
		if (pStateGame) {
			pStateGame->Draw();
		}
		break;
	}
	if (newStage != oldStage) {
		switch (oldStage) {
		case STAGE_TITLE:
			if (newStage == STAGE_EASYMODE || newStage == STAGE_HARDMODE) {
				SAFE_DELETE(pStateTitle);
			}
			break;
		case STAGE_EASYMODE:
		case STAGE_HARDMODE:
			SAFE_DELETE(pStateGame);
			break;
		}

		oldStage = newStage;
	}
}
