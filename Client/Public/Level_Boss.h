#pragma once

#include "Client_Defines.h"
#include "Level.h"

<<<<<<< HEAD
=======


>>>>>>> Main/Add
BEGIN(Client)

class CLevel_Boss final : public CLevel
{
private:
	CLevel_Boss(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Boss() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
<<<<<<< HEAD

private:
	/* 내 게임내에 배경을 구성하는 객체들을 추가한다. */
	HRESULT Ready_Layer_BackGround(const _wstring& strLayerTag);
=======
private:
	HRESULT Ready_Lights();
	/* 내 게임내에 배경을 구성하는 객체들을 추가한다. */
	HRESULT Ready_Layer_BackGround(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Camera(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Effect(const _wstring& strLayerTag);

	HRESULT Ready_UI_PlayerState();
	HRESULT Ready_UI_WeaponState();
	HRESULT Ready_UI_SkillState();
	HRESULT Ready_CrossHair();

	HRESULT Ready_LandObjects();
	HRESULT Ready_Layer_Item(const wstring& strLayerTag);
	HRESULT Ready_Layer_Player(const wstring& strLayerTag);
	HRESULT Ready_Layer_Monster(const _wstring& strLayerTag);
	HRESULT Ready_Layer_DecoObject(const _wstring& strLayerTag);

>>>>>>> Main/Add

public:
	static CLevel_Boss* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END