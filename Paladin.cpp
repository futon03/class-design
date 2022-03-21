#include "Paladin.h"
#include "../../input.h"

namespace {
    Paladin _cPaladin;

	const float _valueMove = 0.155f; // �ړ����x
	const float _valueRotate = 0.079f; // ��]���x(Degree)
    const float _rateMove = 0.025f; // �ړ������W��
    const float _rateRotate = 0.10f; // ��]�����W��
}

Paladin* Paladin::_pPaladin = &_cPaladin;

void Paladin::Move(void)
{
	// �J�����̌����擾
	XMFLOAT3 rotCamera = XMFLOAT3(0, 0, 0);//GetCameraAngle();

	if (GetKeyPress(VK_LEFT)) {
	    if (GetKeyPress(VK_UP)) {
	    	// ���O�ړ�
	    	_move.x -= sinf(rotCamera.y + XM_PI * 0.75f) * _valueMove;
	    	_move.z -= cosf(rotCamera.y + XM_PI * 0.75f) * _valueMove;
        
	    	_rotDest.y = rotCamera.y + XM_PI * 0.75f;
	    }
	    else if (GetKeyPress(VK_DOWN)) {
	    	// ����ړ�
	    	_move.x -= sinf(rotCamera.y + XM_PI * 0.25f) * _valueMove;
	    	_move.z -= cosf(rotCamera.y + XM_PI * 0.25f) * _valueMove;
        
	    	_rotDest.y = rotCamera.y + XM_PI * 0.25f;
	    }
	    else {
	    	// ���ړ�
	    	_move.x -= sinf(rotCamera.y + XM_PIDIV2) * _valueMove;
	    	_move.z -= cosf(rotCamera.y + XM_PIDIV2) * _valueMove;
        
	    	_rotDest.y = rotCamera.y + XM_PIDIV2;
	    }
    } else if (GetKeyPress(VK_RIGHT)) {
   	    if (GetKeyPress(VK_UP)) {
   	    	// �E�O�ړ�
   	    	_move.x -= sinf(rotCamera.y - XM_PI * 0.75f) * _valueMove;
   	    	_move.z -= cosf(rotCamera.y - XM_PI * 0.75f) * _valueMove;
        
   	    	_rotDest.y = rotCamera.y - XM_PI * 0.75f;
   	    }
   	    else if (GetKeyPress(VK_DOWN)) {
   	    	// �E��ړ�
   	    	_move.x -= sinf(rotCamera.y - XM_PI * 0.25f) * _valueMove;
   	    	_move.z -= cosf(rotCamera.y - XM_PI * 0.25f) * _valueMove;
        
   	    	_rotDest.y = rotCamera.y - XM_PI * 0.25f;
   	    }
   	    else {
   	    	// �E�ړ�
   	    	_move.x -= sinf(rotCamera.y - XM_PIDIV2) * _valueMove;
   	    	_move.z -= cosf(rotCamera.y - XM_PIDIV2) * _valueMove;
        
   	    	_rotDest.y = rotCamera.y - XM_PIDIV2;
   	    }
    } else if (GetKeyPress(VK_UP)) {
   	    // �O�ړ�
   	    _move.x -= sinf(XM_PI + rotCamera.y) * _valueMove;
   	    _move.z -= cosf(XM_PI + rotCamera.y) * _valueMove;
        
   	    _rotDest.y = XM_PI + rotCamera.y;
    }
   else if (GetKeyPress(VK_DOWN)) {
   	 // ��ړ�
   	 _move.x -= sinf(rotCamera.y) * _valueMove;
   	 _move.z -= cosf(rotCamera.y) * _valueMove;
     
   	 _rotDest.y = rotCamera.y;
   }

    if (GetKeyPress(VK_O)) {
        // ��ړ�
        _move.y += _valueMove;
    }
    if (GetKeyPress(VK_L)) {
        // ���ړ�
        _move.y -= _valueMove;
    }
   
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

   if (GetKeyTrigger(VK_RETURN))
   {
       _pos = XMFLOAT3(0, 0, 0);
       _rot = XMFLOAT3(0, 0, 0);
   }
   
   // �͈̓`�F�b�N
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