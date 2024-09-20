#pragma once

#include "Client_Defines.h"
#include "Level.h"



BEGIN(Client)

class CLevel_GamePlay final : public CLevel
{
private:
	CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_GamePlay() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
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
	HRESULT Ready_MiniMap();
	HRESULT Ready_Weapon_Bullet();

	HRESULT Ready_LandObjects();
	HRESULT Ready_Layer_Item(const wstring& strLayerTag);
	HRESULT Ready_Layer_Player(const wstring& strLayerTag);
	HRESULT Ready_Layer_Monster(const _wstring& strLayerTag);
	HRESULT Ready_Layer_NPC(const _wstring& strLayerTag);
	HRESULT Ready_Layer_DecoObject(const _wstring& strLayerTag);

	HRESULT Ready_TestObject();
private:
	class CData_Manager* m_pDataMgr = {};

public:
	static CLevel_GamePlay* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END