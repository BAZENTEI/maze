#ifndef __INPUT_H__
#define __INPUT_H__

#include "main.h"

#define NUM_KEY_MAX				256
#define NUM_PAD_MAX				2

/* game pad情報 */
#define BUTTON_UP				0x00000001l	// 方向キー上(.IY<0)
#define BUTTON_DOWN				0x00000002l	// 方向キー下(.IY>0)
#define BUTTON_LEFT				0x00000004l	// 方向キー左(.IX<0)
#define BUTTON_RIGHT			0x00000008l	// 方向キー右(.IX>0)

#define BUTTON_X				0x00000010l	// Ａボタン(.rgbButtons[0]&0x80)
#define BUTTON_A				0x00000020l	// Ｂボタン(.rgbButtons[1]&0x80)
#define BUTTON_B				0x00000040l	// Ｃボタン(.rgbButtons[2]&0x80)
#define BUTTON_Y				0x00000080l	// Ｘボタン(.rgbButtons[3]&0x80)
#define BUTTON_LB				0x00000100l	// Ｙボタン(.rgbButtons[4]&0x80)
#define BUTTON_RB				0x00000200l	// Ｚボタン(.rgbButtons[5]&0x80)
#define BUTTON_LT				0x00000400l	// Ｌボタン(.rgbButtons[6]&0x80)
#define BUTTON_RT				0x00000800l	// Ｒボタン(.rgbButtons[7]&0x80)
#define BUTTON_BACK				0x00001000l	// ＳＴＡＲＴボタン(.rgbButtons[8]&0x80)
#define BUTTON_START			0x00002000l	// Ｍボタン(.rgbButtons[9]&0x80)

#define BUTTON_LROTATE_UP		0x00010000l	// 方向キー上(.IY<0)
#define BUTTON_LROTATE_DOWN		0x00020000l	// 方向キー下(.IY>0)
#define BUTTON_LROTATE_LEFT		0x00040000l	// 方向キー左(.IX<0)
#define BUTTON_LROTATE_RIGHT	0x00080000l	// 方向キー右(.IX>0)

#define BUTTON_RROTATE_UP		0x00100000l	// 方向キー上(.IY<0)
#define BUTTON_RROTATE_DOWN		0x00200000l	// 方向キー下(.IY>0)
#define BUTTON_RROTATE_LEFT		0x00400000l	// 方向キー左(.IX<0)
#define BUTTON_RROTATE_RIGHT	0x00800000l	// 方向キー右(.IX>0)

struct DINPUT {

	static LPDIRECTINPUT8	   	pDInput;							// IDirectInput8インターフェースへのポインタ
	static LPDIRECTINPUTDEVICE8	pDIDevKeyboard;						// IDirectInputDevice8インターフェースへのポインタ(キーボード)
	static BYTE				   	keyStates[NUM_KEY_MAX];				// キーボードの押下状態を保持するワーク
	static BYTE				   	keyStatesTrigger[NUM_KEY_MAX];		// キーボードのトリガー状態を保持するワーク
	static BYTE				   	keyStatesRelease[NUM_KEY_MAX];		// キーボードのリリース状態を保持するワーク
	static BYTE				   	keyStatesRepeat[NUM_KEY_MAX];		// キーボードのリピート状態を保持するワーク
	static int				   	keyStatesRepeatCnt[NUM_KEY_MAX];	// キーボードのリピートカウンタ

	static LPDIRECTINPUTDEVICE8	pGamePad[NUM_PAD_MAX];// パッドデバイス

	static DWORD				padState[NUM_PAD_MAX];	// パッド情報（複数対応）
	static DWORD				padTrigger[NUM_PAD_MAX];
	static UINT					padCount;			// 検出したパッドの数

	static HRESULT				Init(HINSTANCE hInst, HWND hWnd);
	static void					Destroy(void);
	static HRESULT				Update(void);
	static HRESULT				InitKeyboard(HINSTANCE hInst, HWND hWnd);
	static HRESULT				InitGamePad(HINSTANCE hInst, HWND hWnd);
	static HRESULT				UpdateKeyboard(void);
	static HRESULT				UpdateGamePad(void);
	static BOOL CALLBACK		SearchGamePadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID);	// パッド検査コールバック

	static BOOL					KeyPressed(BYTE key);
	static BOOL					KeyTriggered(BYTE key);
	static BOOL					KeyRepeated(BYTE key);
	static BOOL					KeyReleased(BYTE key);

	static BOOL					ButtonPressed(int padNo, DWORD button);
	static BOOL					ButtonPressed(DWORD button);
	static BOOL					ButtonTriggered(int padNo, DWORD button);
	static BOOL					ButtonTriggered(DWORD button);
};

#endif // !__INPUT_H__
