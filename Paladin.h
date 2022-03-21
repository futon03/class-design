#pragma once

#include "BaseModel.h"

class Paladin : public BaseModel
{
public:
	void Move(void) override;

	static Paladin* Get() { return _pPaladin; }
	static void Set(Paladin* pPaladin) { _pPaladin = pPaladin; }
private:
	static Paladin* _pPaladin;
};