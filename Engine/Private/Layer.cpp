#include "..\Public\Layer.h"

#include "GameObject.h"
#include "ContainerObject.h"

CLayer::CLayer()
{
}

CGameObject* CLayer::Get_GameObjectByIndex(_uint iIndex)
{
	auto it = std::find_if(m_GameObjects.begin(), m_GameObjects.end(),
		[iIndex](CGameObject* pGameObject) {
			return pGameObject->m_iIndex == iIndex;
		});

	return (it != m_GameObjects.end()) ? *it : nullptr;
}

HRESULT CLayer::Add_List(list<class CGameObject*>* pList)
{
	if (pList == nullptr)
		return E_FAIL;

	//얕은복사
	*pList = m_GameObjects;

	for (auto& iter : *pList)
	{
		Safe_AddRef(iter);
	}

	return S_OK;
}

HRESULT CLayer::Add_GameObject(CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_GameObjects.emplace_back(pGameObject);

	return S_OK;
}

void CLayer::Priority_Update(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
		pGameObject->Priority_Update(fTimeDelta);
}

void CLayer::Update(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
		pGameObject->Update(fTimeDelta);
}

void CLayer::Late_Update(_float fTimeDelta)
{
	for (auto it = m_GameObjects.begin(); it != m_GameObjects.end(); )
	{
		if ((*it)->Get_Dead())
		{
			Safe_Release(*it);
			it = m_GameObjects.erase(it); // 현재 요소를 제거하고 반복기 업데이트
			continue; // 다음 반복으로 이동
		}
		else
		{
			(*it)->Late_Update(fTimeDelta);
			++it; // 다음 요소로 이동
		}
	}
}

CComponent* CLayer::Get_Component(const wstring& strComponentTag, _uint iIndex)
{
	if (iIndex >= m_GameObjects.size())
		return nullptr;

	auto	iter = m_GameObjects.begin();

	for (size_t i = 0; i < iIndex; i++)
		++iter;

	if (iter == m_GameObjects.end())
		return nullptr;

	return (*iter)->Get_Component(strComponentTag);
}

CComponent* CLayer::Get_PartObject_Component(_uint iPartID, const _wstring& strComponentTag, _uint iIndex)
{
	auto	iter = m_GameObjects.begin();

	for (size_t i = 0; i < iIndex; i++)
		++iter;

	if (iter == m_GameObjects.end())
		return nullptr;

	return dynamic_cast<CContainerObject*>((*iter))->Get_Component(iPartID, strComponentTag);
}

CLayer * CLayer::Create()
{
	return new CLayer();
}

void CLayer::Free()
{

	for (auto& pGameObject : m_GameObjects)
		Safe_Release(pGameObject);

	m_GameObjects.clear();
	__super::Free();

}

