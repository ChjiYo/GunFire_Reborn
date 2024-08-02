#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CBinModel final : public CComponent
{
public:
	enum TYPE { TYPE_NONANIM, TYPE_ANIM, TYPE_END };

private:
	CBinModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBinModel(const CBinModel& Prototype);
	virtual ~CBinModel() = default;

public:
	_uint Get_NumMeshes() const {
		return m_iNumMeshes;
	}

	const _float4x4* Get_BoneMatrixPtr(const _char* pBoneName) const;

public:
	virtual HRESULT Initialize_Prototype(enum TYPE eModelType, const _wstring& strModelFilePath, _fmatrix PreTransformMatrix);
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render(_uint iMeshIndex);

public:
	void Play_Animation(_float fTimeDelta);
	void SetUp_Animation(_uint iAnimationIndex, _bool isLoop);

public:
	HRESULT Bind_MaterialSRV(class CShader* pShader, aiTextureType eType, const _char* pConstantName, _uint iMeshIndex);
	HRESULT Bind_BoneMatrices(class CShader* pShader, const _char* pConstantName, _uint iMeshIndex);

private:
	/* 파일로부터 읽어온 모델을 움직이고 그리기위한 모든 정보를 다 가지고 있는 객체 */
	const aiScene* m_pAIScene = { nullptr };

	/* 지정해준 경로에 해당하는 파일의 정보를 읽어서 aiScene안에 저장해주는 역활. */
	Assimp::Importer			m_Importer;

private:
	TYPE						m_eModelType = { TYPE_END };
	_float4x4					m_PreTransformMatrix = {};

private: /* For.Meshes */
	_uint						m_iNumMeshes = {};
	vector<class CMesh*>		m_Meshes;

private: /* For.Materials */
	_uint						m_iNumMaterials = {};
	vector<MESHMATERIAL>		m_Materials;
	vector<char*>				AiPath;

private: /* For.Bones */
	vector<class CBone*>		m_Bones;

private: /* For.Animation */
	_float						m_fCurrentAnimPosition = {};
	_uint						m_iCurrentAnimationIndex = {};
	_bool						m_isLoopAnim = { false };
	_uint						m_iNumAnimations = {};
	vector<class CAnimation*>	m_Animations;
	vector<vector<_uint>>		m_KeyFrameIndices;

private:
	vector<vector<_uint>>		m_PrevKeyFrameIndices;

public:
	/* 모델 하나가 여러개의 메시(Vertex, Index)로 구분되어있기 때문에. */
	/* 대부분의 모델들은 교체가 필요한 메시들 기준으로 나눠둔다. */
	HRESULT Ready_Meshes(_ulong& dwByte, HANDLE& hFile,_fmatrix PreTransformMatrix);
	HRESULT Ready_Materials(_ulong& dwByte, HANDLE& hFile);

	/* 모델의 뼈들을 로드한다. */
	/* 뼈 : 정점들이 뼈에 붙어서 렌더링 되야하기때문 */
	/* 정점들이 뼈에 붙는다?! : 뼈를 움직여서 애니메이션을 표현하겠다. == 애니메이션이 구동된다. */
	HRESULT Ready_Bones(_ulong& dwByte, HANDLE& hFile);

	/* 추후, 런타임간에 뼈의 시간에 맞는 상태를 구하기위해 필요한 데이터들을 미리 모두 셋팅해둔다.  */
	HRESULT Ready_Animations(_ulong& dwByte, HANDLE& hFile);

public:
	static CBinModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, enum TYPE eModelType, const _wstring& pModelFilePath, _fmatrix PreTransformMatrix);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END