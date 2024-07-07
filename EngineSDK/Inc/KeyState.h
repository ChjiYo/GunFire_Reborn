#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CKeyState final:public CBase
{
private:
	CKeyState();
	virtual ~CKeyState() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

public:
	_bool 		Key_Pressing(_uint _iKey);
	_bool 			Key_Down(_uint _iKey);
	_bool 			Key_Up(_uint _iKey);

private:
	_bool m_bKeyState[VK_MAX] = {};

public:
	static CKeyState* Create();
	virtual void Free() override;
};

END