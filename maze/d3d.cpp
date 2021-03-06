#include "d3d.h"
#include "stage.h"
#include "sound.h"
#include "input.h"

LPDIRECT3D9			D3D::pD3D = NULL;								// Direct3D オブジェクト
LPDIRECT3DDEVICE9	D3D::pDevice = NULL;							// Deviceオブジェクト(描画に必要)
LPD3DXFONT			D3D::pD3DXFont = NULL;							
DWORD				D3D::dwTime = timeGetTime();
FLOAT				D3D::fAnimationRate = 0.0f;

GAMESTAGE* g_pGameState;

void D3D::Destroy(void)
{
	SAFE_DELETE(g_pGameState);
	DSOUND::Destroy();
	DINPUT::Destroy();
	SAFE_RELEASE(D3D::pD3D);
	SAFE_RELEASE(D3D::pDevice);
	SAFE_RELEASE(D3D::pD3DXFont);
}

LPDIRECT3DDEVICE9 D3D::GetDevice(void)
{
	return D3D::pDevice;
}

HRESULT D3D::LoadTexture(LPDIRECT3DTEXTURE9* ppTex, LPCWSTR path)
{
	if (!*ppTex) {
		if (FAILED(D3DXCreateTextureFromFile(pDevice,			// デバイスへのポインタ
			path,										// ファイルの名前
			ppTex)))										// 読み込むメモリー
		{
			return E_FAIL;
		}
	}
	return S_OK;
}

HRESULT D3D::ShowText(LPCWSTR text, LONG left = 0, LONG top = 0)
{
	RECT rect = { left, top, CL_WIDTH, CL_HEIGHT };

	D3D::pD3DXFont->DrawText(NULL, text, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(255, 255, 255, 255));
	
	return S_OK;
}

HRESULT D3D::EnableFog(DWORD fogColor)
{
	FLOAT Start = 0.0f;		// Linear fog distances
	FLOAT End = GR_FAR_Z / 4.0f;

	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_FOGCOLOR, fogColor);

	// Set fog parameters.
	pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
	pDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&Start));
	pDevice->SetRenderState(D3DRS_FOGEND, *(DWORD *)(&End));

	return S_OK;
}

HRESULT D3D::DisableFog()
{
	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
	return S_OK;
}

HRESULT D3D::Init(HINSTANCE hInst, HWND hWnd)
{
	LPDIRECT3D9 pD3D;
	LPDIRECT3DDEVICE9 pDevice;
	LPD3DXFONT pFont;

	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	BOOL bWindow = TRUE;

	// Direct3Dオブジェクトの作成
	pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (pD3D == NULL)
	{
		return E_FAIL;
	}
	D3D::pD3D = pD3D;

	// 現在のディスプレイモードを取得
	if (FAILED(pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));						// ワークをゼロクリア
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = CL_WIDTH;				// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = CL_HEIGHT;			// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;				// カラーモードの指定
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	// 映像信号に同期してフリップする
	d3dpp.Windowed = bWindow;					// ウィンドウモード
	d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;				// デプスバッファとして16bitを使う

	if (bWindow)
	{// ウィンドウモード
		d3dpp.FullScreen_RefreshRateInHz = 0;								// リフレッシュレート
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	// インターバル
	}
	else
	{// フルスクリーンモード
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// リフレッシュレート
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル
	}

	// デバイスの生成
	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &pDevice)))
	{
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &pDevice)))
		{
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &pDevice)))
			{
				// 初期化失敗
				return E_FAIL;
			}
		}
	}
	D3D::pDevice = pDevice;
	// レンダリングステートパラメータの設定
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面をカリング
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);			// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);		// αデスティネーションカラーの指定

	// サンプラーステートパラメータの設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)を設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

	// テクスチャステージステートパラメータの設定
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);		// アルファブレンディング処理
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);		// 最初のアルファ引数
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);		// ２番目のアルファ引数
	
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, _T("consolas"), &pFont);
	D3D::pD3DXFont = pFont;

	// 初期化
	DINPUT::Init(hInst, hWnd);
	DSOUND::Init(hWnd);

	g_pGameState = new GAMESTAGE(STAGE_TITLE);

	return S_OK;
}

HRESULT D3D::Draw(void)
{
	pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_ARGB(255, 0, 0, 0), 1.0f, 0);

	// Direct3Dによる描画の開始
	if (SUCCEEDED(pDevice->BeginScene()))
	{
		g_pGameState->Draw();
/*
		WCHAR strbuf[256];
		swprintf_s(strbuf, _T("%f"), fAnimationRate);
		ShowText(strbuf, 0, 0);
*/
		// Direct3Dによる描画の終了
		pDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	pDevice->Present(NULL, NULL, NULL, NULL);
	return S_OK;
}

HRESULT D3D::Update(void)
{
	DWORD newTime = timeGetTime();
	DWORD dif = newTime - dwTime;

	dwTime = newTime;
	fAnimationRate = dif / 1000.0f;

	DINPUT::Update();

	g_pGameState->Update();

	return S_OK;
}
