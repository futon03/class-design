#pragma once

#include "../../main.h"
#include "../../AssimpModel.h"

class BaseModel
{
public:
    HRESULT Init(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 scl, const char* modelname);
    void Uninit(void);
	void Update(void);
	void Draw(void);
    void NoAnimDraw(void); // staticモデル用描画

    virtual void Move(void){}

	XMFLOAT3& GetPos(){ return _pos; }
private:
    XMFLOAT4X4		_mtxWorld;

    LPCSTR			_pszModelPath;
    CAssimpModel	_model;	// FBXデータ

    int _nAnimStack;
    double _dAnimTime;
protected:
    XMFLOAT3		_pos;
    XMFLOAT3		_move;
    XMFLOAT3		_rot;
    XMFLOAT3		_rotDest;
    XMFLOAT3		_scl;
};