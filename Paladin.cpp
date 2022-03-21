#include "Paladin.h"
#include "../../input.h"

namespace {
    Paladin _cPaladin;

	const float _valueMove = 0.155f; // 移動速度
	const float _valueRotate = 0.079f; // 回転速度(Degree)
    const float _rateMove = 0.025f; // 移動慣性係数
    const float _rateRotate = 0.10f; // 回転慣性係数
}

Paladin* Paladin::_pPaladin = &_cPaladin;

void Paladin::Move(void)
{
	// カメラの向き取得
	XMFLOAT3 rotCamera = XMFLOAT3(0, 0, 0);//GetCameraAngle();

	if (GetKeyPress(VK_LEFT)) {
	    if (GetKeyPress(VK_UP)) {
	    	// 左前移動
	    	_move.x -= sinf(rotCamera.y + XM_PI * 0.75f) * _valueMove;
	    	_move.z -= cosf(rotCamera.y + XM_PI * 0.75f) * _valueMove;
        
	    	_rotDest.y = rotCamera.y + XM_PI * 0.75f;
	    }
	    else if (GetKeyPress(VK_DOWN)) {
	    	// 左後移動
	    	_move.x -= sinf(rotCamera.y + XM_PI * 0.25f) * _valueMove;
	    	_move.z -= cosf(rotCamera.y + XM_PI * 0.25f) * _valueMove;
        
	    	_rotDest.y = rotCamera.y + XM_PI * 0.25f;
	    }
	    else {
	    	// 左移動
	    	_move.x -= sinf(rotCamera.y + XM_PIDIV2) * _valueMove;
	    	_move.z -= cosf(rotCamera.y + XM_PIDIV2) * _valueMove;
        
	    	_rotDest.y = rotCamera.y + XM_PIDIV2;
	    }
    } else if (GetKeyPress(VK_RIGHT)) {
   	    if (GetKeyPress(VK_UP)) {
   	    	// 右前移動
   	    	_move.x -= sinf(rotCamera.y - XM_PI * 0.75f) * _valueMove;
   	    	_move.z -= cosf(rotCamera.y - XM_PI * 0.75f) * _valueMove;
        
   	    	_rotDest.y = rotCamera.y - XM_PI * 0.75f;
   	    }
   	    else if (GetKeyPress(VK_DOWN)) {
   	    	// 右後移動
   	    	_move.x -= sinf(rotCamera.y - XM_PI * 0.25f) * _valueMove;
   	    	_move.z -= cosf(rotCamera.y - XM_PI * 0.25f) * _valueMove;
        
   	    	_rotDest.y = rotCamera.y - XM_PI * 0.25f;
   	    }
   	    else {
   	    	// 右移動
   	    	_move.x -= sinf(rotCamera.y - XM_PIDIV2) * _valueMove;
   	    	_move.z -= cosf(rotCamera.y - XM_PIDIV2) * _valueMove;
        
   	    	_rotDest.y = rotCamera.y - XM_PIDIV2;
   	    }
    } else if (GetKeyPress(VK_UP)) {
   	    // 前移動
   	    _move.x -= sinf(XM_PI + rotCamera.y) * _valueMove;
   	    _move.z -= cosf(XM_PI + rotCamera.y) * _valueMove;
        
   	    _rotDest.y = XM_PI + rotCamera.y;
    }
   else if (GetKeyPress(VK_DOWN)) {
   	 // 後移動
   	 _move.x -= sinf(rotCamera.y) * _valueMove;
   	 _move.z -= cosf(rotCamera.y) * _valueMove;
     
   	 _rotDest.y = rotCamera.y;
   }

    if (GetKeyPress(VK_O)) {
        // 上移動
        _move.y += _valueMove;
    }
    if (GetKeyPress(VK_L)) {
        // 下移動
        _move.y -= _valueMove;
    }
   
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

   if (GetKeyTrigger(VK_RETURN))
   {
       _pos = XMFLOAT3(0, 0, 0);
       _rot = XMFLOAT3(0, 0, 0);
   }
   
   // 範囲チェック
   if (_pos.x < -630.0f) {
       _pos.x = -630.0f;
   }
   if (_pos.x > 630.0f) {
   	   _pos.x = 630.0f;
   }
   if (_pos.z > 630.0f) {
   	   _pos.z = 630.0f;
   }
   if (_pos.z < -630.0f) {
   	   _pos.z = -630.0f;
   }  
}