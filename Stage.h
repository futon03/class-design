#pragma once

#include "BaseModel.h"

class Stage : public BaseModel
{
public:
	void Move(void) override;

	static Stage* Get() { return _pStage; }
	static void Set(Stage* pStage) { _pStage = pStage; }
private:
	static Stage* _pStage;
};