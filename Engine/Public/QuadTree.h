#pragma once

#include "Base.h"

BEGIN(Engine)

class CQuadTree final : public CBase
{
public:
	enum CORNER { CORNER_LT, CORNER_RT, CORNER_RB, CORNER_LB, CORNER_END };
private:
	CQuadTree();
	virtual ~CQuadTree() = default;

public:
	HRESULT Initialize(_uint iLT, _uint iRT, _uint iRB, _uint iLB);

private:
	_uint				m_iCorners[CORNER_END] = {};
	_uint				m_iCenter = {};
	CQuadTree* m_pChildren[CORNER_END] = { nullptr, nullptr, nullptr, nullptr };

public:
	static CQuadTree* Create(_uint iLT, _uint iRT, _uint iRB, _uint iLB);
	virtual void Free();
};

END