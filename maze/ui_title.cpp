#include "ui_title.h"
#include "title.h"

#define TITLE_TEX_MAIN		_T("data/TEXTURE/ui/title.png")
#define TITLE_TEX_MENU		_T("data/TEXTURE/ui/menu.png")

LPDIRECT3DTEXTURE9	TITLEUI::pTexMenu = NULL;
LPDIRECT3DTEXTURE9	TITLEUI::pTexTitle = NULL;

D3DXCOLOR ElementColor[4] = {
	D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
	D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f),
	D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f),
	D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
};

TITLEUI::TITLEUI(TITLE * pTitle)
{
	pGameTitle = pTitle;

	D3D::LoadTexture(&pTexTitle, TITLE_TEX_MAIN);
	D3D::LoadTexture(&pTexMenu, TITLE_TEX_MENU);
	D3DRECT rect;

	for (int i = 0; i < TITLEMENU_COUNT; i++) {
		rect = { CL_WIDTH / 2 - 256, CL_HEIGHT / 2 + 96 * i, 512, 128 };
		menu[i] = new IMAGE(pTexMenu, &rect);
		menu[i]->nFrameIndex = i;
		menu[i]->nFrameTotal = TITLEMENU_COUNT;
		menu[i]->SetTexture();
	}

	rect = { CL_WIDTH / 2 - 640, CL_HEIGHT / 8, 1280, 320 };
	title = new IMAGE(pTexTitle, &rect);
}

TITLEUI::~TITLEUI()
{
	SAFE_RELEASE(pTexTitle);
	SAFE_RELEASE(pTexMenu);

	SAFE_DELETE(title);
	for (int i = 0; i < TITLEMENU_COUNT; i++) {
		SAFE_DELETE(menu[i]);
	}
}

void TITLEUI::SetTitleColor()
{
	VERTEX_2D* pTempVtx;
	DWORD t = timeGetTime();
	FLOAT s = (t % 4000) / 1000.0f;
	INT n = (INT)floorf(s);
	FLOAT p = s - n;

	title->pVtx->Lock(0, 0, (void**)&pTempVtx, 0);
	
	D3DXCOLOR col;
	int seq[4] = { 0, 1, 3, 2 };
	for (int i = 0; i < 4; i++) {
		D3DXColorLerp(&col, &ElementColor[seq[(n + i) % 4] + 1], &ElementColor[seq[(n + i + 1) % 4] + 1], p);
		pTempVtx[seq[i]].dif = col;
	}
	
	title->pVtx->Unlock();
}

void TITLEUI::Update()
{
	for (int i = 0; i < TITLEMENU_COUNT; i++) {
		if (pGameTitle->menuSelected == i) {
			menu[i]->SetDiffuseColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}
		else {
			menu[i]->SetDiffuseColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		menu[i]->Update();
	}

	SetTitleColor();

	title->Update();
}

HRESULT TITLEUI::Draw()
{
	for (int i = 0; i < TITLEMENU_COUNT; i++) {
		menu[i]->Draw();
	}
	title->Draw();

	return S_OK;
}
