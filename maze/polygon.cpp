#include "polygon.h"

POLYGON::POLYGON(LPDIRECT3DTEXTURE9 pTex, D3DXVECTOR3 vertices[], D3DXVECTOR3 normals[])
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();

	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&pVtx,						// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return;
	}

	this->pTex = pTex;
	this->srt = SRT();
	this->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	vtx[0] = vertices[0];
	vtx[1] = vertices[1];
	vtx[2] = vertices[2];
	vtx[3] = vertices[3];

	SetVertex(vertices, normals);
}

POLYGON::~POLYGON()
{
	SAFE_RELEASE(pVtx);
}

HRESULT POLYGON::SetVertex(D3DXVECTOR3 vertices[], D3DXVECTOR3 normals[])
{
	//頂点バッファの中身を埋める
	VERTEX_3D* pTempVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	pVtx->Lock(0, 0, (void**)&pTempVtx, 0);

	// 頂点座標の設定
	pTempVtx[0].vtx = vertices[0];
	pTempVtx[1].vtx = vertices[1];
	pTempVtx[2].vtx = vertices[2];
	pTempVtx[3].vtx = vertices[3];

	if (normals) {
		pTempVtx[0].nor = normals[0];
		pTempVtx[1].nor = normals[1];
		pTempVtx[2].nor = normals[2];
		pTempVtx[3].nor = normals[3];
	}
	else {
		pTempVtx[0].nor =
		pTempVtx[1].nor =
		pTempVtx[2].nor =
		pTempVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	}
	// 反射光の設定
	pTempVtx[0].dif =
	pTempVtx[1].dif =
	pTempVtx[2].dif =
	pTempVtx[3].dif = col;

	// テクスチャ座標の設定
	pTempVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pTempVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pTempVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pTempVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点データをアンロックする
	pVtx->Unlock();

	return S_OK;
}

HRESULT POLYGON::Draw()
{
	LPDIRECT3DDEVICE9	pDevice = D3D::GetDevice();

	D3DXMATRIX			mtxView, mtxScale, mtxRot, mtxTranslate;
	
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtx);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, srt.rot.y, srt.rot.x, srt.rot.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxRot);
	
	// スケールを反映
	D3DXMatrixScaling(&mtxScale, srt.scl.x, srt.scl.y, srt.scl.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxScale);
	
	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, srt.pos.x, srt.pos.y, srt.pos.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxTranslate);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, pVtx, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, pTex);

#if _DEBUG
	//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
#endif
	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

#if _DEBUG
	//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
#endif
	return S_OK;
}

void POLYGON::Update()
{
}
