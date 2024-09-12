#include "..\Public\Object_Manager.h"

#include "Layer.h"
#include "GameObject.h"
#include "Navigation.h"
#include <locale>
#include <codecvt>

CObject_Manager::CObject_Manager()
{
}

CComponent * CObject_Manager::Get_Component(_uint iLevelIndex, const _wstring & strLayerTag, const _wstring & strComponentTag, _uint iIndex)
{
	if (nullptr == m_pLayers || 
		iLevelIndex >= m_iNumLevels)
		return nullptr;

	CLayer*		pLayer = Find_Layer(iLevelIndex, strLayerTag);
	if (nullptr == pLayer)
		return nullptr;

	return pLayer->Get_Component(strComponentTag, iIndex);	
}

CComponent* CObject_Manager::Get_PartObject_Component(_uint iLevelIndex, const _wstring& strLayerTag, _uint iPartID, const _wstring& strComponentTag, _uint iIndex)
{
	if (nullptr == m_pLayers ||
		iLevelIndex >= m_iNumLevels)
		return nullptr;

	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);
	if (nullptr == pLayer)
		return nullptr;

	return pLayer->Get_PartObject_Component(iPartID, strComponentTag, iIndex);
}

HRESULT CObject_Manager::Initialize(_uint iNumLevels)
{
	m_iNumLevels = iNumLevels;

	m_pLayers = new map<const wstring, class CLayer*>[iNumLevels];

	return S_OK;
}

HRESULT CObject_Manager::Add_Prototype(const wstring & strPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr != Find_Prototype(strPrototypeTag))
		return E_FAIL;

	m_Prototypes.emplace(strPrototypeTag, pPrototype);

	return S_OK;
}

HRESULT CObject_Manager::Add_GameObject_ToLayer(_uint iLevelIndex, const wstring & strPrototypeTag, const wstring & strLayerTag, void* pArg)
{
	/* 복제할 원형객체를 찾자. */
	CGameObject*		pPrototype = Find_Prototype(strPrototypeTag);
	if (nullptr == pPrototype)
		return E_FAIL;

	/* 찾아낸 원형객체를 복제하여 사본객체를 생성해오자. */
	CGameObject*		pGameObject = pPrototype->Clone(pArg);
	if (nullptr == pGameObject)
		return E_FAIL;

	/* 복제한 사본객체를 레이어에 추가한다.. */
	CLayer*		pLayer = Find_Layer(iLevelIndex, strLayerTag);

	if (nullptr == pLayer)
	{
		CLayer*		pLayer = CLayer::Create();

		pLayer->Add_GameObject(pGameObject);

		m_pLayers[iLevelIndex].emplace(strLayerTag, pLayer);
	}
	else
		pLayer->Add_GameObject(pGameObject);

	return S_OK;
}

HRESULT CObject_Manager::AddObjectPrototypesVector(vector<string>* _pVector)
{
	if (_pVector == nullptr)
		return E_FAIL;

	// Create a wstring_convert object to perform conversion
	wstring_convert<codecvt_utf8<wchar_t>> converter;

	for (auto& iter : m_Prototypes)
	{
		// Convert wstring (iter.first) to string
		string converted = converter.to_bytes(iter.first);
		_pVector->push_back(converted);
	}

	return S_OK;
}

HRESULT CObject_Manager::AddObjectLayersVector(_uint _level, vector<pair < string, list<CGameObject*>>>* pVector)
{
	if (pVector == nullptr)
		return E_FAIL;

	// Create a wstring_convert object to perform conversion
	wstring_convert<codecvt_utf8<wchar_t>> converter;
	list<CGameObject*> tempList;
	for (auto& iter : m_pLayers[_level])
	{
		// Convert wstring (iter.first) to string
		string converted = converter.to_bytes(iter.first);
		iter.second->Add_List(&tempList);

		pair<string, list<CGameObject*>> tempPair;
		tempPair = { converted ,tempList };
		pVector->push_back(tempPair);
	}

	return S_OK;
}

HRESULT CObject_Manager::AddObjectLayersVector(_uint _level, vector<pair < wstring, list<CGameObject*>>>* pVector)
{
	if (pVector == nullptr)
		return E_FAIL;

	list<CGameObject*> tempList;
	for (auto& iter : m_pLayers[_level])
	{
		iter.second->Add_List(&tempList);

		pair<wstring, list<CGameObject*>> tempPair;
		tempPair = { iter.first ,tempList };
		pVector->push_back(tempPair);
	}

	return S_OK;
}

HRESULT CObject_Manager::Static_Object_Component_Clear()
{
	for (auto& iter : m_pLayers[0])
	{
		dynamic_cast<CNavigation*>(iter.second->Get_Component(TEXT("Com_Navigation"), 0))->Clear_Navi();
		
	}

	return S_OK;
}

CGameObject* CObject_Manager::GetObjectByIndex(_uint iLevelIndex, const wstring& strLayerTag, _uint index)
{
	CLayer* layer = Find_Layer(iLevelIndex, strLayerTag);
	CGameObject* gameobject = layer->Get_GameObjectByIndex(index);

	if (gameobject == nullptr)
		return nullptr;

	return gameobject;
}

CGameObject* CObject_Manager::Clone_GameObject(const wstring& strPrototypeTag, void* pArg)
{
	/* 복제할 원형객체를 찾자. */
	CGameObject* pPrototype = Find_Prototype(strPrototypeTag);
	if (nullptr == pPrototype)
		return nullptr;

	/* 찾아낸 원형객체를 복제하여 사본객체를 생성해오자. */
	CGameObject* pGameObject = pPrototype->Clone(pArg);
	if (nullptr == pGameObject)
		return nullptr;

	return pGameObject;
}

void CObject_Manager::Priority_Update(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
			Pair.second->Priority_Update(fTimeDelta * m_fFreeDeltaTime);
	}
}

void CObject_Manager::Update(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
			Pair.second->Update(fTimeDelta * m_fFreeDeltaTime);
	}
}

void CObject_Manager::Late_Update(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
			Pair.second->Late_Update(fTimeDelta * m_fFreeDeltaTime);
	}
}

HRESULT CObject_Manager::Clear_Resources(_uint iLevelIndex)
{
	if (iLevelIndex >= m_iNumLevels)
		return E_FAIL;

	for (auto& Pair : m_pLayers[iLevelIndex])	
		Safe_Release(Pair.second);

	m_pLayers[iLevelIndex].clear();
	
	return S_OK;
}

list<class CGameObject*> CObject_Manager::Get_Layer(_uint iLevelIndex, const wstring& strLayerTag)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);

	if (pLayer == nullptr)
	{
		list<class CGameObject*> NullList = {};
		return NullList;
	}

	return pLayer->Get_Object_List();
}

CGameObject * CObject_Manager::Find_Prototype(const wstring & strPrototypeTag)
{
	auto	iter = m_Prototypes.find(strPrototypeTag);

	if (iter == m_Prototypes.end())
		return nullptr;

	return iter->second;	
}

CLayer * CObject_Manager::Find_Layer(_uint iLevelIndex, const wstring & strLayerTag)
{
	auto	iter = m_pLayers[iLevelIndex].find(strLayerTag);

	if (iter == m_pLayers[iLevelIndex].end())
		return nullptr;

	return iter->second;	
}

CObject_Manager * CObject_Manager::Create(_uint iNumLevels)
{
	CObject_Manager*		pInstance = new CObject_Manager();

	if (FAILED(pInstance->Initialize(iNumLevels)))
	{
		MSG_BOX(TEXT("Failed to Created : CObject_Manager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CObject_Manager::Free()
{

	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])		
			Safe_Release(Pair.second);

		m_pLayers[i].clear();		
	}

	Safe_Delete_Array(m_pLayers);


	for (auto& Pair : m_Prototypes)
		Safe_Release(Pair.second);

	m_Prototypes.clear();

	__super::Free();
}
