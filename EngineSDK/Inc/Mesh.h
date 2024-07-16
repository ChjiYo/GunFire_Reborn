#pragma once

#include "VIBuffer.h"
#include "Model.h"

/* VIBuffer_Rect랑 크게 다를게 없다. */

BEGIN(Engine)

class CMesh final : public CVIBuffer
{
public:
	typedef struct
	{
		_char				szName[MAX_PATH];
		_uint				iMaterialIndex;
		_uint				iNumVertices;
		_uint				iNumIndices;
		vector<XMFLOAT3>	vPosition;
		vector<XMFLOAT3>	vNormal;
		vector<XMFLOAT3>	vTexcoord;
		vector<XMFLOAT3>	vTangent;
		_uint				mNumFaces;
		vector<_uint>		vIndices;
	}MESH_DESC;

private:
	CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CMesh() = default;

public:
	_uint Get_MaterialIndex() const {
		return m_iMaterialIndex;
	}

	MESH_DESC Get_Desc() { return m_tMeshDesc; }

public:
	virtual HRESULT Initialize_Prototype(CModel::TYPE eModelType, const aiMesh* pAIMesh, const vector<class CBone*>& Bones, _fmatrix PreTransformMatrix);

public:
	void SetUp_Matrices(_float4x4* pMatrices, const vector<class CBone*>& Bones);
private:
	_char				m_szName[MAX_PATH] = {};
	_uint				m_iMaterialIndex = {};
	/* 이 메시에게 영향을 주는 뼈의 갯수 .*/
	_uint				m_iNumBones = {};

	/* 이 메시에게 영향을 주는 뼈의 인덱스를 저장한다. (전체뼈기준) */
	vector<_uint>		m_BoneIndices; 
	vector<_float4x4>	m_OffsetMatrices;

	MESH_DESC			m_tMeshDesc = {};
private:
	void* Create_NonAnimVertices(const aiMesh* pAIMesh, _fmatrix PreTransformMatrix);
	void* Create_AnimVertices(const aiMesh* pAIMesh, const vector<CBone*>& Bones);

public:
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::TYPE eModelType, const aiMesh* pAIMesh, const vector<class CBone*>& Bones, _fmatrix PreTransformMatrix);
	virtual CComponent* Clone(void* pArg) { return nullptr; }
	virtual void Free() override;
};

END