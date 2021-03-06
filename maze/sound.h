#ifndef __SOUND_H__
#define __SOUND_H__

#include "main.h"

// サウンドファイル
typedef enum
{
	SOUND_CLICK,
	SOUND_MOVEMENT,
	SOUND_MAX,
} SOUNDLABEL;

typedef struct
{
	LPCWSTR filename;	// ファイル名
	BOOL bLoop;			// ループするかどうか
} SOUNDPARAM;

struct DSOUND {
	static SOUNDPARAM				sounds[SOUND_MAX];
	static IXAudio2*				pXAudio2;					// XAudio2オブジェクトへのインターフェイス
	static IXAudio2MasteringVoice*	pMasteringVoice;			// マスターボイス
	static IXAudio2SourceVoice*		pSourceVoices[SOUND_MAX];	// ソースボイス
	static BYTE*					pAudioData[SOUND_MAX];		// オーディオデータ
	static DWORD					dwAudioSize[SOUND_MAX];		// オーディオデータサイズ

	static HRESULT					Init(HWND hWnd);
	static void						Destroy(void);
	static HRESULT					Play(SOUNDLABEL label);
	static void						Stop(SOUNDLABEL label);
	static void						StopAll(void);
	static HRESULT					CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	static HRESULT					ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
};

#endif // !__SOUND_H__
