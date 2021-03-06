#include "stage.h"

GAMESTAGE::GAMESTAGE(UINT eStageType)
{
	pStageTitle = NULL;
	pStageGame = NULL;
	oldStage = newStage = eStageType;

	SetStage(eStageType);
}

GAMESTAGE::~GAMESTAGE(void)
{
	SAFE_DELETE(pStageTitle);
	SAFE_DELETE(pStageGame);
}

void GAMESTAGE::SetStage(UINT eStageType)
{
	newStage = eStageType;
}

void GAMESTAGE::Update(void)
{
	switch (newStage) {
	case STAGE_TITLE:
		if (!pStageTitle) {
			pStageTitle = new TITLE(this);
		}
		else {
			pStageTitle->Update();
		}
		break;
	case STAGE_EASYMODE:
		if (!pStageGame) {
			pStageGame = new MAINGAME(this, LEVEL_EASY);
		}
		else {
			pStageGame->Update();
		}
		break;
	case STAGE_HARDMODE:
		if (!pStageGame) {
			pStageGame = new MAINGAME(this, LEVEL_HARD);
		}
		else {
			pStageGame->Update();
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
		if (pStageTitle) {
			pStageTitle->Draw();
		}
		break;
	case STAGE_EASYMODE:
	case STAGE_HARDMODE:
		if (pStageGame) {
			pStageGame->Draw();
		}
		break;
	}
	if (newStage != oldStage) {
		switch (oldStage) {
		case STAGE_TITLE:
			if (newStage == STAGE_EASYMODE || newStage == STAGE_HARDMODE) {
				SAFE_DELETE(pStageTitle);
			}
			break;
		case STAGE_EASYMODE:
		case STAGE_HARDMODE:
			SAFE_DELETE(pStageGame);
			break;
		}

		oldStage = newStage;
	}
}
