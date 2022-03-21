#include "Stage.h"
#include "../../input.h"

namespace {
    Stage _cStage;

    const float _valueMove = 1.0f; // �ړ����x
    const float _valueRotate = 0.02f; // ��]���x(Degree)
}

Stage* Stage::_pStage = &_cStage;

void Stage::Move(void)
{
    if (GetKeyPress(VK_A)) {
        // ����]
        _rotDest.y -= _valueRotate;
        if (_rotDest.y < -XM_PI) {
            _rotDest.y += XM_2PI;
        }
    }
    if (GetKeyPress(VK_D)) {
        // �E��]
        _rotDest.y += _valueRotate;
        if (_rotDest.y > XM_PI) {
            _rotDest.y -= XM_2PI;
        }
    }

    if (GetKeyPress(VK_W)) {
        // ��ړ�
        _pos.z += _valueMove;
    }
    if (GetKeyPress(VK_S)) {
        // ���ړ�
        _pos.z -= _valueMove;
    }

    if (GetKeyTrigger(VK_RETURN))
    {
        _pos = XMFLOAT3(0, -20, 300);
        _rot = XMFLOAT3(0, 0, 0);
    }
}