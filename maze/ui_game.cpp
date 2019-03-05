#include "ui_game.h"
#include "game.h"
#include "player.h"

#define IMAGE_STRING		_T("data/TEXTURE/ui/blank.png")
#define IMAGE_RESULT		_T("data/TEXTURE/ui/blank.png")

LPDIRECT3DTEXTURE9	MAINGAMEUI::pTexStatus = NULL;
LPDIRECT3DTEXTURE9	MAINGAMEUI::pTexResult = NULL;

const DWORD DIGIT_GAP = 24;
const DWORD DIGIT_SIZE = 32;
const DWORD ELEMENT_SIZE = 48;
const DWORD TEXT_SIZE = 96;

MAINGAMEUI::MAINGAMEUI(MAINGAME* game)
{
	pGame = game;

	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();

	D3D::LoadTexture(&pTexStatus, IMAGE_STRING);
	D3D::LoadTexture(&pTexResult, IMAGE_RESULT);
}

MAINGAMEUI::~MAINGAMEUI()
{
	SAFE_RELEASE(pTexStatus);
	SAFE_RELEASE(pTexResult);

	for (int i = 0; i < UI_MAX_DIGITS; i++) {
		SAFE_DELETE(score[0][i]);
		SAFE_DELETE(score[1][i]);
	}
	for (int i = 0; i < GM_MAX_PLAYER; i++) {
		SAFE_DELETE(result[i]);
	}
	for (int i = 0; i < UI_TIMER_DIGITS; i++) {
		SAFE_DELETE(timer[i]);
	}
}

void MAINGAMEUI::Update()
{
}

HRESULT MAINGAMEUI::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();

	pDevice->EndScene();

	pDevice->BeginScene();

	for (int i = 0; i < GM_MAX_PLAYER; i++) {
		for (int j = 0; j < UI_MAX_DIGITS; j++) {
			score[i][j]->Draw();
		}

		if (pGame->bGameOver) {
			result[i]->Draw();
		}
	}
	for (int i = 0; i < UI_TIMER_DIGITS; i++) {
		timer[i]->Draw();
	}

	pDevice->Present(NULL, NULL, NULL, NULL);

	pDevice->EndScene();

	pDevice->BeginScene();

	return S_OK;
}
