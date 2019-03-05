#include "game.h"
#include "camera.h"
#include "light.h"
#include "maze.h"
#include "player.h"
#include "sky.h"
#include "sound.h"
#include "ui_game.h"
#include "stage.h"

MAINGAME::MAINGAME(GAMESTAGE* state, int level)
{
	srand((unsigned)timeGetTime());
	bGameOver = FALSE;
	pState = state;
	gameLevel = level;

	sky = new SKY();
	
	int size = 2 * MAZE_SIZE + 1;
	maze = new MAZE(size, size);

	int x = GET_RANDOM(0, 1);
	int y = GET_RANDOM(0, 1);

	maze->SetGoal((1 - x) * (size - 3) + 1, (1 - y) * (size - 3) + 1);
	players[0] = new PLAYER(this, 0, 1);
	players[0]->SetPosition(x * (size - 3) + 1, y * (size - 3) + 1);
}

MAINGAME::~MAINGAME()
{
	for (int i = 0; i < GM_MAX_PLAYER; i++) {
		SAFE_DELETE(players[i]);
	}

	SAFE_DELETE(sky);
	SAFE_DELETE(maze);
}

void MAINGAME::Update(void)
{
	if (!bGameOver) {
		sky->Update();
	}
	for (int i = 0; i < GM_MAX_PLAYER; i++) {
		if(players[i])
			players[i]->Update();
	}
}

HRESULT MAINGAME::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();
	D3DVIEWPORT9 vp0;

	pDevice->GetViewport(&vp0);
	pDevice->EndScene();

	for (int i = 0; i < GM_MAX_PLAYER; i++) {
		if (players[i]) {
			CAMERA* camera = players[i]->camera;

			camera->SetCamera();

			D3DVIEWPORT9 vp = camera->viewport;

			RECT rect = { (LONG)vp.X, (LONG)vp.Y, (LONG)(vp.X + vp.Width), (LONG)(vp.Y + vp.Height) };

			pDevice->Clear(0, (D3DRECT*)&rect, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(255, 255, 255, 255), 1.0f, 0);

			pDevice->BeginScene();

			pDevice->SetViewport(&vp);

			sky->Draw(camera);

			//D3D::EnableFog(sky->light->GetFogColor());
			
			maze->Draw(camera);

			//D3D::DisableFog();

			players[i]->Draw(camera);

			WCHAR str[256];
			int x, y;
			maze->GetPositionCell(players[i]->srt.pos, x, y);
			swprintf_s(str, _T(" pos x:%d y:%d\n Energy: %f"), x, y, players[i]->fEnergy);
			D3D::ShowText(str, 0, 0);

			pDevice->EndScene();

			pDevice->Present(&rect, &rect, NULL, NULL);
		}
	}

	pDevice->SetViewport(&vp0);

	pDevice->BeginScene();

	return S_OK;
}

void MAINGAME::GameOver(int playerId, BOOL win)
{
	bGameOver = TRUE;
}
