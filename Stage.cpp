#include "Stage.h"
#include "../../input.h"

namespace {
    Stage _cStage;

    const float _valueMove = 1.0f; // 移動速度
    const float _valueRotate = 0.02f; // 回転速度(Degree)
}

Stage* Stage::_pStage = &_cStage;

void Stage::Move(void)
{
    if (GetKeyPress(VK_A)) {
        // 左回転
        _rotDest.y -= _valueRotate;
        if (_rotDest.y < -XM_PI) {
            _rotDest.y += XM_2PI;
        }
    }
    if (GetKeyPress(VK_D)) {
        // 右回転
        _rotDest.y += _valueRotate;
        if (_rotDest.y > XM_PI) {
            _rotDest.y -= XM_2PI;
        }
    }

    if (GetKeyPress(VK_W)) {
        // 上移動
        _pos.z += _valueMove;
    }
    if (GetKeyPress(VK_S)) {
        // 下移動
        _pos.z -= _valueMove;
    }

    if (GetKeyTrigger(VK_RETURN))
    {
        _pos = XMFLOAT3(0, -20, 300);
        _rot = XMFLOAT3(0, 0, 0);
    }
}