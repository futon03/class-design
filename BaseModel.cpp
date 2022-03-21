#include <time.h>
#include "../../AssimpModel.h"
#include "BaseModel.h"
#include "../../main.h"
#include "../../input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ANIM_IDLE			(0)
#define ANIM_JUMP			(1)
#define ANIM_KICK			(2)
#define ANIM_RUN			(3)
#define ANIM_SLASH			(4)
#define ANIM_WALK			(5)

#define	PLAYER_RADIUS		(50.0f)							// ���a
#define	RATE_MOVE_PLAYER	(0.025f)						// �ړ������W��
#define	RATE_ROTATE_PLAYER	(0.10f)							// ��]�����W��

/// <summary>
/// 
/// </summary>
/// <param name="move"></param> �����ʒu
/// <param name="rot"></param>
/// <param name="scl"></param>
/// <param name="modelname"></param>
/// <returns></returns>
HRESULT BaseModel::Init(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 scl, const char* modelname)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	_move = XMFLOAT3(0, 0, 0);
	_pos = pos;
	_rot = rot;
	_scl = scl;

	_pszModelPath = modelname;

	// ���f���f�[�^�̓ǂݍ���
	if (!_model.Load(pDevice, pDeviceContext, _pszModelPath)) {
		hr = E_FAIL;
		MessageBoxA(GetMainWnd(), "���f���f�[�^�Ǎ��G���[", "InitModel", MB_OK | MB_ICONEXCLAMATION);
	}
	return hr;
}

void BaseModel::Uninit(void)
{
	// ���f���̉��
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

	// �ړI�̊p�x�܂ł̍���
	float fDiffRotY = _rotDest.y - _rot.y;
	if (fDiffRotY > XM_PI) {
		fDiffRotY -= XM_2PI;
	}
	if (fDiffRotY < -XM_PI) {
		fDiffRotY += XM_2PI;
	}

	// �ړI�̊p�x�܂Ŋ�����������
	_rot.y += fDiffRotY * RATE_ROTATE_PLAYER;
	if (_rot.y > XM_PI) {
		_rot.y -= XM_2PI;
	}
	if (_rot.y < -XM_PI) {
		_rot.y += XM_2PI;
	}

	// �ʒu�ړ�
	_pos.x += _move.x;
	_pos.y += _move.y;
	_pos.z += _move.z;

	// �ړ��ʂɊ�����������
	_move.x += (0.0f - _move.x) * RATE_MOVE_PLAYER;
	_move.y += (0.0f - _move.y) * RATE_MOVE_PLAYER;
	_move.z += (0.0f - _move.z) * RATE_MOVE_PLAYER;

	// �A�j���[�V�����X�V
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

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	// �X�P�[���𔽉f
	mtxScl = XMMatrixScaling(_scl.x, _scl.y, _scl.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// ��]�𔽉f
	mtxRot = XMMatrixRotationRollPitchYaw(_rot.x, _rot.y, _rot.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(_pos.x, _pos.y, _pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�ݒ�
	XMStoreFloat4x4(&_mtxWorld, mtxWorld);
}

void BaseModel::Draw(void)
{
	// �V���h�E�{�����[��������Ƃ��͗��ʃJ�����O���Ă���
	//�J�����O(����)
	SetCullMode(CULLMODE_NONE);

	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// ���f���\��
	_model.SetAnimIndex(_nAnimStack);
	_model.SetAnimTime(_dAnimTime);
	_model.Draw(pDeviceContext, _mtxWorld);

	//�J�����O��߂��Ă���
	SetCullMode(CULLMODE_CCW);
}

void BaseModel::NoAnimDraw(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// ���f���\��
	_model.Draw(pDeviceContext, _mtxWorld);
}