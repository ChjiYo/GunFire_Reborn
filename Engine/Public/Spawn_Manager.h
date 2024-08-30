#pragma once

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CSpawn_Manager abstract : public CBase
{
private:
	CSpawn_Manager();
	virtual ~CSpawn_Manager() = default;
	


public:
	static CSpawn_Manager* Create(_uint iNumLevels);
	virtual void Free() override;
};

END

