#include <time.h>
#include "../../AssimpModel.h"
#include "BaseModel.h"
#include "../../main.h"
#include "../../input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ANIM_IDLE			(0)
#define ANIM_JUMP			(1)
#define ANIM_KICK			(2)
#define ANIM_RUN			(3)
#define ANIM_SLASH			(4)
#define ANIM_WALK			(5)

#define	PLAYER_RADIUS		(50.0f)							// 半径
#define	RATE_MOVE_PLAYER	(0.025f)						// 移動慣性係数
#define	RATE_ROTATE_PLAYER	(0.10f)							// 回転慣性係数

/// <summary>
/// 
/// </summary>
/// <param name="move"></param> 初期位置
/// <param name="rot"></param>
/// <param name="scl"></param>
/// <param name="modelname"></param>
/// <returns></returns>
HRESULT BaseModel::Init(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 scl, const char* modelname)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// 位置・回転・スケールの初期設定
	_move = XMFLOAT3(0, 0, 0);
	_pos = pos;
	_rot = rot;
	_scl = scl;

	_pszModelPath = modelname;

	// モデルデータの読み込み
	if (!_model.Load(pDevice, pDeviceContext, _pszModelPath)) {
		hr = E_FAIL;
		MessageBoxA(GetMainWnd(), "モデルデータ読込エラー", "InitModel", MB_OK | MB_ICONEXCLAMATION);
	}
	return hr;
}

void BaseModel::Uninit(void)
{
	// モデルの解放
	_model.Release();
}

void BaseModel::Update(void)
{
	XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate;

	Move();

	switch (_nAnimStack) {
	case ANIM_IDLE:
		if (GetKeyTrigger(VK_X)) {
			_nAnimStack = ANIM_SLASH;
			_dAnimTime = 0.0;
		}
		if (GetKeyTrigger(VK_C)) {
			_nAnimStack = ANIM_KICK;
			_dAnimTime = 0.0;
		}
		// THRU
	case ANIM_WALK:
	case ANIM_RUN:
		if (GetKeyTrigger(VK_Z)) {
			_nAnimStack = ANIM_JUMP;
			_dAnimTime = 0.0;
		}
		break;
	}

	// 目的の角度までの差分
	float fDiffRotY = _rotDest.y - _rot.y;
	if (fDiffRotY > XM_PI) {
		fDiffRotY -= XM_2PI;
	}
	if (fDiffRotY < -XM_PI) {
		fDiffRotY += XM_2PI;
	}

	// 目的の角度まで慣性をかける
	_rot.y += fDiffRotY * RATE_ROTATE_PLAYER;
	if (_rot.y > XM_PI) {
		_rot.y -= XM_2PI;
	}
	if (_rot.y < -XM_PI) {
		_rot.y += XM_2PI;
	}

	// 位置移動
	_pos.x += _move.x;
	_pos.y += _move.y;
	_pos.z += _move.z;

	// 移動量に慣性をかける
	_move.x += (0.0f - _move.x) * RATE_MOVE_PLAYER;
	_move.y += (0.0f - _move.y) * RATE_MOVE_PLAYER;
	_move.z += (0.0f - _move.z) * RATE_MOVE_PLAYER;

	// アニメーション更新
	float fSpeed = sqrtf(_move.x * _move.x + _move.z * _move.z);
	//PrintDebugProc("Speed=%f\n\n", fSpeed);
	int nStack = ANIM_IDLE;
	if (fSpeed > 3.0f)
		nStack = ANIM_RUN;
	else if (fSpeed > 0.2f)
		nStack = ANIM_WALK;
	switch (_nAnimStack) {
	case ANIM_IDLE:
	case ANIM_WALK:
	case ANIM_RUN:
		if (_nAnimStack != nStack) {
			_nAnimStack = nStack;
			_dAnimTime = 0.0;
		}
		break;
	}
	static double dLastTime = clock() / double(CLOCKS_PER_SEC);
	double dNowTime = clock() / double(CLOCKS_PER_SEC);
	double dSlice = dNowTime - dLastTime;
	dLastTime = dNowTime;
	_dAnimTime += dSlice;
	if (_dAnimTime >= _model.GetAnimDuration(_nAnimStack)) {
		switch (_nAnimStack) {
		case ANIM_JUMP:
		case ANIM_SLASH:
		case ANIM_KICK:
			_nAnimStack = nStack;
			break;
		}
		_dAnimTime = 0.0;
	}

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	// スケールを反映
	mtxScl = XMMatrixScaling(_scl.x, _scl.y, _scl.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// 回転を反映
	mtxRot = XMMatrixRotationRollPitchYaw(_rot.x, _rot.y, _rot.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// 移動を反映
	mtxTranslate = XMMatrixTranslation(_pos.x, _pos.y, _pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ワールドマトリックス設定
	XMStoreFloat4x4(&_mtxWorld, mtxWorld);
}

void BaseModel::Draw(void)
{
	// シャドウボリュームをするときは両面カリングしておく
	//カリング(両面)
	SetCullMode(CULLMODE_NONE);

	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// モデル表示
	_model.SetAnimIndex(_nAnimStack);
	_model.SetAnimTime(_dAnimTime);
	_model.Draw(pDeviceContext, _mtxWorld);

	//カリングを戻しておく
	SetCullMode(CULLMODE_CCW);
}

void BaseModel::NoAnimDraw(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// モデル表示
	_model.Draw(pDeviceContext, _mtxWorld);
}