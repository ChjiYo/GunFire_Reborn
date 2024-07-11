#include "..\Public\GameInstance.h"

#include "Component_Manager.h"
#include "Graphic_Device.h"
#include "Object_Manager.h"
#include "Level_Manager.h"
#include "Timer_Manager.h"
#include "Light_Manager.h"
#include "Input_Device.h"
#include "Font_Manager.h"
#include "KeyState.h"
#include "Picking.h"
#include "FileManager.h"

#include "PipeLine.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()	
{

}

HRESULT CGameInstance::Initialize_Engine(HINSTANCE hInst, HWND hWnd, _bool isWindowed, _uint iNumLevels, _uint iWinSizeX, _uint iWinSizeY, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext)
{
	m_pGraphic_Device = CGraphic_Device::Create(hWnd, isWindowed, iWinSizeX, iWinSizeY, ppDevice, ppContext);
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pInput_Device = CInput_Device::Create(hInst, hWnd);
	if (nullptr == m_pInput_Device)
		return E_FAIL;

	m_pRenderer = CRenderer::Create(*ppDevice, *ppContext);
	if (nullptr == m_pRenderer)
		return E_FAIL;

	m_pPipeLine = CPipeLine::Create();
	if (nullptr == m_pPipeLine)
		return E_FAIL;

	m_pObject_Manager = CObject_Manager::Create(iNumLevels);
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	m_pComponent_Manager = CComponent_Manager::Create(iNumLevels);
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	m_pLevel_Manager = CLevel_Manager::Create();
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	m_pTimer_Manager = CTimer_Manager::Create();
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	m_pLight_Manager = CLight_Manager::Create(*ppDevice, *ppContext);
	if(nullptr == m_pLight_Manager)
		return E_FAIL;

	m_pFont_Manager = CFont_Manager::Create(*ppDevice, *ppContext);
	if (nullptr == m_pFont_Manager)
		return E_FAIL;

	m_pKeyState = CKeyState::Create();
	if (nullptr == m_pKeyState)
		return E_FAIL;

	m_pFileManager = CFile_Manager::Create();
	if (nullptr == m_pFileManager)
		return E_FAIL;

	m_pPicking = CPicking::Create(*ppDevice, *ppContext, hWnd);
	if (nullptr == m_pPicking)
		return E_FAIL;

	return S_OK;
}

void CGameInstance::Update_Engine(_float fTimeDelta)
{	


	m_pPicking->Update();
	/* 키보드와 마우스의 상태를 저장한다. */
	m_pInput_Device->Update();

	/* 엔진에서 관리하는 객체들 중, 반복적인 갱신이 필요한 객체들이 있다면. */
	/* 여기에서 갱신을 수행해준다. */	
	m_pObject_Manager->Priority_Update(fTimeDelta);

	/* 얘가 들고 있는 뷰랑 투영의 역행렬을 구하고 카메라 위치를 저장한다. */
	m_pPipeLine->Update();

	m_pObject_Manager->Update(fTimeDelta);

	m_pObject_Manager->Late_Update(fTimeDelta);

	m_pLevel_Manager->Update(fTimeDelta);
}

HRESULT CGameInstance::Render_Engine()
{
	/* 엔진에서 관리하는 객체들 중, 반복적인 렌더가 필요한 객체들이 있다면. */
	/* 여기에서 렌더를 수행해준다. */

	if (FAILED(m_pRenderer->Draw()))
		return E_FAIL;

	if (FAILED(m_pLevel_Manager->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGameInstance::Clear_LevelResources(_uint iLevelIndex)
{
	if (FAILED(m_pObject_Manager->Clear_Resources(iLevelIndex)))
		return E_FAIL;

	if(FAILED(m_pComponent_Manager->Clear_Resources(iLevelIndex)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGameInstance::Clear_BackBuffer_View(_float4 vClearColor)
{
	return m_pGraphic_Device->Clear_BackBuffer_View(vClearColor);	
}

HRESULT CGameInstance::Clear_DepthStencil_View()
{
	return m_pGraphic_Device->Clear_DepthStencil_View();
}

HRESULT CGameInstance::Present()
{
	return m_pGraphic_Device->Present();
}

_byte CGameInstance::Get_DIKeyState(_ubyte byKeyID)
{
	return m_pInput_Device->Get_DIKeyState(byKeyID);	
}

_byte CGameInstance::Get_DIMouseState(MOUSEKEYSTATE eMouseKeyState)
{
	return m_pInput_Device->Get_DIMouseState(eMouseKeyState);
}

_long CGameInstance::Get_DIMouseMove(MOUSEMOVESTATE eMouseMoveState)
{
	return m_pInput_Device->Get_DIMouseMove(eMouseMoveState);
}

HRESULT CGameInstance::Change_Level(CLevel * pNewLevel)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Change_Level(pNewLevel);	
}

_uint CGameInstance::GetCurrentLevelIndex()
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->GetCurrentLevelIndex();
}

HRESULT CGameInstance::Add_Timer(const _wstring & strTimerTag)
{
	return m_pTimer_Manager->Add_Timer(strTimerTag);	
}

_float CGameInstance::Compute_TimeDelta(const _wstring & strTimerTag)
{
	return m_pTimer_Manager->Compute_TimeDelta(strTimerTag);
}

HRESULT CGameInstance::Add_Prototype(const wstring & strPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Prototype(strPrototypeTag, pPrototype);	
}

HRESULT CGameInstance::Add_GameObject_ToLayer(_uint iLevelIndex, const wstring & strPrototypeTag, const wstring & strLayerTag, void * pArg)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_GameObject_ToLayer(iLevelIndex, strPrototypeTag, strLayerTag, pArg);
}

CComponent * CGameInstance::Get_Component(_uint iLevelIndex, const _wstring & strLayerTag, const _wstring & strComponentTag, _uint iIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return	m_pObject_Manager->Get_Component(iLevelIndex, strLayerTag, strComponentTag, iIndex);
}

list<class CGameObject*> CGameInstance::Get_Layer(_uint iLevelIndex, const wstring& strLayerTag)
{
	return m_pObject_Manager->Get_Layer(iLevelIndex, strLayerTag);
}

CGameObject* CGameInstance::Clone_GameObject(const wstring& strPrototypeTag, void* pArg)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Clone_GameObject(strPrototypeTag, pArg);
}

CComponent* CGameInstance::Get_PartObject_Component(_uint iLevelIndex, const _wstring& strLayerTag, _uint iPartID, const _wstring& strComponentTag, _uint iIndex)
{
	return nullptr;
}

HRESULT CGameInstance::AddObjectPrototypesVector(vector<string>* pVector)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return	m_pObject_Manager->AddObjectPrototypesVector(pVector);
}


HRESULT CGameInstance::AddObjectLayersVector(_uint iLevelIndex, vector<pair < string, list<CGameObject*>>>* pVector)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return	m_pObject_Manager->AddObjectLayersVector(iLevelIndex, pVector);
}

HRESULT CGameInstance::AddObjectLayersVector(_uint iLevelIndex, vector<pair < wstring, list<CGameObject*>>>* pVector)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return	m_pObject_Manager->AddObjectLayersVector(iLevelIndex, pVector);
}

CGameObject* CGameInstance::GetObjectByIndex(_uint iLevelIndex, const wstring& strLayerTag, _uint index)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return	m_pObject_Manager->GetObjectByIndex(iLevelIndex, strLayerTag, index);
}

HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const _wstring & strPrototypeTag, CComponent * pPrototype)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	return m_pComponent_Manager->Add_Prototype(iLevelIndex, strPrototypeTag, pPrototype);	
}

CComponent * CGameInstance::Clone_Component(_uint iLevelIndex, const _wstring & strPrototypeTag, void * pArg)
{
	if (nullptr == m_pComponent_Manager)
		return nullptr;

	return m_pComponent_Manager->Clone_Component(iLevelIndex, strPrototypeTag, pArg);
}

HRESULT CGameInstance::Add_RenderObject(CRenderer::RENDERGROUP eRenderGroup, CGameObject * pRenderObject)
{
	if (nullptr == m_pRenderer)
		return E_FAIL;

	return m_pRenderer->Add_RenderObject(eRenderGroup, pRenderObject);	
}

_matrix CGameInstance::Get_Transform_Matrix(CPipeLine::D3DTRANSFORMSTATE eState) const
{
	return m_pPipeLine->Get_Transform_Matrix(eState);
}

_float4x4 CGameInstance::Get_Transform_Float4x4(CPipeLine::D3DTRANSFORMSTATE eState) const
{
	return m_pPipeLine->Get_Transform_Float4x4(eState);
}

_matrix CGameInstance::Get_Transform_Inverse_Matrix(CPipeLine::D3DTRANSFORMSTATE eState) const
{
	return m_pPipeLine->Get_Transform_Inverse_Matrix(eState);
}

_float4x4 CGameInstance::Get_Transform_Inverse_Float4x4(CPipeLine::D3DTRANSFORMSTATE eState) const
{
	return m_pPipeLine->Get_Transform_Inverse_Float4x4(eState);
}

_vector CGameInstance::Get_CamPosition_Vector() const
{
	return m_pPipeLine->Get_CamPosition_Vector();
}

_float4 CGameInstance::Get_CamPosition_Float4() const
{
	return m_pPipeLine->Get_CamPosition_Float4();
}

void CGameInstance::Set_Transform(CPipeLine::D3DTRANSFORMSTATE eState, _fmatrix TransformMatrix)
{
	m_pPipeLine->Set_Transform(eState, TransformMatrix);
}

HRESULT CGameInstance::Add_Font(const _wstring& strFontTag, const _wstring& strFontFilePath)
{
	return m_pFont_Manager->Add_Font(strFontTag, strFontFilePath);
}

HRESULT CGameInstance::Render_Font(const _wstring& strFontTag, const _wstring& strText, const _float2& vPosition, _fvector vColor, _float fRotation, _float fScale)
{
	return m_pFont_Manager->Render(strFontTag, strText, vPosition, vColor, fRotation, fScale);
}

void CGameInstance::Transform_ToLocalSpace(const _matrix* pWorldMatrix)
{
	return m_pPicking->Transform_ToLocalSpace(pWorldMatrix);
}

_bool CGameInstance::Picked_InLocalSpace(const _vector* pPointA, const _vector* pPointB, const _vector* pPointC, _vector* pPickPos)
{
	return m_pPicking->Picked_InLocalSpace(pPointA, pPointB, pPointC, pPickPos);
}

_bool CGameInstance::Picked_InWorldSpace(_vector pPointA, _vector pPointB, _vector pPointC, _vector* pPickPos)
{
	return m_pPicking->Picked_InWorldSpace(pPointA, pPointB, pPointC, pPickPos);
}

HRESULT CGameInstance::SaveObjects(const wstring& filename, void* pArg)
{
	if (nullptr == m_pFileManager)
		return E_FAIL;

	return m_pFileManager->SaveObjects(filename, pArg);
}


_bool CGameInstance::Key_Pressing(_uint _iKey)
{
	return m_pKeyState->Key_Pressing(_iKey);
}

_bool CGameInstance::Key_Down(_uint _iKey)
{
	return m_pKeyState->Key_Down(_iKey);
}

_bool CGameInstance::Key_Up(_uint _iKey)
{
	return m_pKeyState->Key_Up(_iKey);
}

const LIGHT_DESC * CGameInstance::Get_LightDesc(_uint iLightIndex) const
{
	return m_pLight_Manager->Get_LightDesc(iLightIndex);
}

HRESULT CGameInstance::Add_Light(const LIGHT_DESC & LightDesc)
{
	return m_pLight_Manager->Add_Light(LightDesc);
}

void CGameInstance::Release_Engine()
{
	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pFont_Manager);
	Safe_Release(m_pKeyState);
	Safe_Release(m_pPicking);
	Safe_Release(m_pFileManager);

	CGameInstance::Get_Instance()->Destroy_Instance();
}

void CGameInstance::Free()
{
	__super::Free();

	
}
