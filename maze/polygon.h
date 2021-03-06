#ifndef __POLYGON_H__
#define __POLYGON_H__

#include "main.h"
#include "core.h"
#include "d3d.h"

//
// ポリゴン情報
//
typedef struct POLYGON {
	LPDIRECT3DVERTEXBUFFER9	pVtx;
	LPDIRECT3DTEXTURE9		pTex;
	
	D3DXVECTOR3				vtx[NUM_VERTEX];
	D3DXCOLOR				col;
	D3DXMATRIX				mtx;
	SRT						srt;

	POLYGON(LPDIRECT3DTEXTURE9 pTex, D3DXVECTOR3 vertices[], D3DXVECTOR3 normals[]);
	~POLYGON();

	HRESULT					SetVertex(D3DXVECTOR3 vertices[], D3DXVECTOR3 normals[]);
	HRESULT					Draw();
	void					Update();
} POLYGON;


#endif // !__POLYGON_H__
