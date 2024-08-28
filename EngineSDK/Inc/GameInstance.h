#pragma once

#include "PipeLine.h"
#include "Renderer.h"
#include "Component_Manager.h"

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public:
	/* 엔진을 초기화한다. */
	HRESULT Initialize_Engine(HINSTANCE hInst, HWND hWnd, _bool isWindowed, _uint iNumLevels, _uint iWinSizeX, _uint iWinSizeY, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext);
	void Update_Engine(_float fTimeDelta);
	HRESULT Render_Engine();
	HRESULT Clear_LevelResources(_uint iLevelIndex);

public: /* For.Graphic_Device */		
	HRESULT Clear_BackBuffer_View(_float4 vClearColor);	
	HRESULT Clear_DepthStencil_View();	
	HRESULT Present();

public: /* For.Input_Device */
	_byte	Get_DIKeyState(_ubyte byKeyID);
	_byte	Get_DIMouseState(MOUSEKEYSTATE eMouseKeyState);
	_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseMoveState);


public: /* For.Level_Manager */
	HRESULT Change_Level(class CLevel* pNewLevel);
	_uint GetCurrentLevelIndex();

public: /* For.Timer_Manager */
	HRESULT Add_Timer(const _wstring& strTimerTag);
	_float Compute_TimeDelta(const _wstring& strTimerTag);

public: /* For.Object_Manager */
	HRESULT Add_Prototype(const wstring& strPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_GameObject_ToLayer(_uint iLevelIndex, const wstring& strPrototypeTag, const wstring& strLayerTag, void* pArg = nullptr);
	class CComponent* Get_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iIndex = 0);
	class list<class CGameObject*> Get_Layer(_uint iLevelIndex, const wstring& strLayerTag);
	class CGameObject* Clone_GameObject(const wstring& strPrototypeTag, void* pArg = nullptr);
	class CComponent* Get_PartObject_Component(_uint iLevelIndex, const _wstring& strLayerTag, _uint iPartID, const _wstring& strComponentTag, _uint iIndex);


	HRESULT AddObjectPrototypesVector(vector<string>* pVector);
	HRESULT AddObjectLayersVector(_uint iLevelIndex, vector<pair<string, list<CGameObject*>>>* pVector);
	HRESULT AddObjectLayersVector(_uint iLevelIndex, vector<pair < wstring, list<CGameObject*>>>* pVector);
	CGameObject* GetObjectByIndex(_uint iLevelIndex, const wstring& strLayerTag, _uint index);

public: /* For.Component_Manager */
	HRESULT Add_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag, class CComponent* pPrototype);
	class CComponent* Clone_Component(_uint iLevelIndex, const _wstring& strPrototypeTag, void* pArg = nullptr);


public: /* For.Renderer */
	HRESULT Add_RenderObject(CRenderer::RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);
	HRESULT Add_DebugComponent(class CComponent* pDebugComponent);

public: /* For.PipeLine */
	_matrix Get_Transform_Matrix(CPipeLine::D3DTRANSFORMSTATE eState) const;
	_float4x4 Get_Transform_Float4x4(CPipeLine::D3DTRANSFORMSTATE eState) const;
	_matrix Get_Transform_Inverse_Matrix(CPipeLine::D3DTRANSFORMSTATE eState) const;
	_float4x4 Get_Transform_Inverse_Float4x4(CPipeLine::D3DTRANSFORMSTATE eState) const;
	_vector Get_CamPosition_Vector() const;
	_float4 Get_CamPosition_Float4() const;
	void Set_Transform(CPipeLine::D3DTRANSFORMSTATE eState, _fmatrix TransformMatrix);

public: /* For.Font_Manager */
	HRESULT Add_Font(ID3D11Device* pDevice,ID3D11DeviceContext* pContext,const _wstring& strFontTag, const _wstring& strFontFilePath);
	HRESULT Draw_Font(const _wstring& strFontTag, const _wstring& strText, const _float2& vPosition, _fvector vColor = XMVectorSet(0.f, 0.f, 0.f, 1.f), _float fRotation = 0.f, _float fScale = 1.f);

public: /* For.Picking */
	_float4 Picked_Position(_bool* pPick);

	void Transform_ToLocalSpace(const _matrix* pWorldMatrix);
	_bool Picked_InLocalSpace(const _vector* pPointA, const _vector* pPointB, const _vector* pPointC, _vector* pPickPos);
	_bool Picked_InWorldSpace(_vector pPointA, _vector pPointB, _vector pPointC, _vector* pPickPos);

public: /* For.FileManager */
	HRESULT SaveObjects(const wstring& filename, void* pArg);
	void* LoadObjects(const wstring& filename);

public: /* For.Key_State */
	_bool 		Key_Pressing(_uint _iKey);
	_bool 			Key_Down(_uint _iKey);
	_bool 			Key_Up(_uint _iKey);

public: /* For.Light */
	const LIGHT_DESC* Get_LightDesc(_uint iLightIndex) const;
	HRESULT Add_Light(const LIGHT_DESC& LightDesc);
	HRESULT Delete_Light();
	HRESULT Render_Lights(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);

public: /* For.Target_Manager */
	HRESULT Add_RenderTarget(const _wstring& strTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, _fvector vClearColor);
	HRESULT Add_MRT(const _wstring& strMRTTag, const _wstring& strTargetTag);
	HRESULT Begin_MRT(const _wstring& strMRTTag, ID3D11DepthStencilView* pDSV = nullptr);
	HRESULT End_MRT();
	HRESULT Copy_RenderTarget(const _wstring& strTargetTag, ID3D11Texture2D* pTexture2D);
	HRESULT Bind_RT_ShaderResource(class CShader* pShader, const _char* pConstantName, const _wstring& strTargetTag);

#ifdef _DEBUG
public:
	HRESULT Ready_RT_Debug(const _wstring& strTargetTag, _float fCenterX, _float fCenterY, _float fSizeX, _float fSizeY);
	HRESULT Render_RT_Debug(const _wstring& strMRTTag, class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
#endif

private:
	class CGraphic_Device*				m_pGraphic_Device = { nullptr };
	class CInput_Device*				m_pInput_Device = { nullptr };
	class CLevel_Manager*				m_pLevel_Manager = { nullptr };
	class CTimer_Manager*				m_pTimer_Manager = { nullptr };
	class CObject_Manager*				m_pObject_Manager = { nullptr };
	class CComponent_Manager*			m_pComponent_Manager = { nullptr };
	class CRenderer*					m_pRenderer = { nullptr };
	class CPipeLine*					m_pPipeLine = { nullptr };
	class CLight_Manager*				m_pLight_Manager = { nullptr };
	class CFont_Manager*				m_pFont_Manager = { nullptr };
	class CKeyState*					m_pKeyState = { nullptr };
	class CFile_Manager*				m_pFileManager = {};
	class CPicking*						m_pPicking = { nullptr };
	class CTarget_Manager*				m_pTarget_Manager = { nullptr };

public:
	void Release_Engine();
	
	virtual void Free() override;

};

END

