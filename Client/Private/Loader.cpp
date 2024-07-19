#include "stdafx.h"
#include "..\Public\Loader.h"

#include "GameInstance.h"
#include "Camera_Free.h"
#include "BackGround.h"
#include "Monster.h"
#include "Terrain.h"
#include "Cube.h"

#include "Wall_Stage1.h"
#include "Floor_102.h"

#include "Player.h"
#include "Skill_Player_Bullet.h"

CLoader::CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
	, m_pGameInstance{ CGameInstance::Get_Instance()}
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

/* 이 함수를 호출한 스레드는 어떤 역활? 리소스를 로드한다. 어떤 레벨용? */
_uint APIENTRY LoadingMain(void* pArg)
{
	CoInitializeEx(nullptr, 0);

	CLoader*		pLoader = static_cast<CLoader*>(pArg);

	if (FAILED(pLoader->Loading()))
		return 1;

	return 0;
}

HRESULT CLoader::Initialize(LEVELID eNextLevelID)
{
	/* 어떤 레벨에 대한 준비를 해야하는지 */
	m_eLevelID = eNextLevelID;

	InitializeCriticalSection(&m_Critical_Section);

	/* 정해준 레벨에 자원(텍스쳐, 사운드, 모델 등등)을 준비한다. */
	/* 새로운 스레드를 만들어서 준비한다. 기존 내 코드를 수행하던 메인스레드는 로딩 화면(Update, Render) 에 대한 처리를 한다. */
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;
	
	return S_OK;
}

/* 추가적으로 생성한 스레드가 호출한 함수. */
HRESULT CLoader::Loading()
{
	EnterCriticalSection(&m_Critical_Section);

	HRESULT		hr{};

	m_isFinished = false;

	switch (m_eLevelID)
	{
	case LEVEL_BIN:
		hr = Loading_For_BinaryFile();
		break;
	case LEVEL_LOGO:
		hr = Loading_For_LogoLevel();
		break;
	case LEVEL_GAMEPLAY:
		hr = Loading_For_GamePlayLevel();
		break;

	default:
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	LeaveCriticalSection(&m_Critical_Section);

	return S_OK;
}

HRESULT CLoader::Loading_For_BinaryFile()
{
	lstrcpy(m_szLoadingText, TEXT("바이너리 파일을 로딩 중 입니다."));

	_matrix			PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));

	//Anim 모델을 로드합니다.
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Player"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/GunFire_Resources/Models/Gun/Weapon_Pistol/Pistol.fbx", TEXT("../Bin/DataFiles/Player_Pistol.dat"), PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fiona"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Fiona/Fiona.fbx", TEXT("../Bin/DataFiles/Fiona.dat"), PreTransformMatrix))))
		return E_FAIL;

	//NonAnim 모델을 로드합니다.

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Wall_Stage1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/GunFire_Resources/Models/Element/Wall_Stage/Stage1/Wall_Stage1.fbx", TEXT("../Bin/DataFiles/Wall_Stage.dat"), PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Floor_102"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/GunFire_Resources/Models/Element/Floor/102/Floor_102.fbx", TEXT("../Bin/DataFiles/Floor_102.dat"), PreTransformMatrix))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("바이러리 파일 로드가 완료되었습니다."));

	return S_OK;
}

HRESULT CLoader::Loading_For_LogoLevel()
{
	/* 로고레벨용 자원을 로드하자. */


	lstrcpy(m_szLoadingText, TEXT("콜라이더를 로딩 중 입니다."));

	/* For.Prototype_Component_Collider_AABB */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_AABB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_OBB */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_OBB))))
		return E_FAIL;
	/* For.Prototype_Component_Collider_Sphere */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_Sphere"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_SPHERE))))
		return E_FAIL;

	/* 텍스쳐를 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩 중 입니다."));

	/* For.Prototype_Component_Texture_Logo */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Logo"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/UI/Logo/UI_Login_Logo.png")))))
		return E_FAIL;
	

	/* 네비게이션을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("네비게이션을 로딩 중 입니다."));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/DataFiles/Navigation.dat")))))
		return E_FAIL;

	/* 모델을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("모델을 로딩 중 입니다."));

	/* 객체원형을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("객체원형을 로딩 중 입니다."));

	/* For.Prototype_GameObject_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;
	
	return S_OK;
}

HRESULT CLoader::Loading_For_GamePlayLevel()
{
	/* 게임플레이레벨용 자원을 로드하자. */

	lstrcpy(m_szLoadingText, TEXT("컴포넌트를 로딩 중 입니다."));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_State"),
		CState_Machine::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* 텍스쳐를 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩 중 입니다.")); 

	/* For.Prototype_Component_Texture_Black */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Black"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/Black.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Tile%d.dds"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Bullet_Red */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Skill_Bullet_Red"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/Skill/Player/Bullet/bullet_red.dds"), 1))))
		return E_FAIL;


	/* 모델을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("모델(정점 -> 폴리곤 -> 메시 -> 모델)을 로딩 중 입니다."));	

	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, 100 , 100))))
		return E_FAIL;

	_matrix			PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));

	/* For.Prototype_Component_Model_Wall_State1 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Wall_Stage1"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/DataFiles/Wall_Stage.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Floor_102 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Floor_102"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/DataFiles/Floor_102.dat"), PreTransformMatrix))))
		return E_FAIL;
	
	
	/* For.Prototype_Component_Model_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Player"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_ANIM, TEXT("../Bin/DataFiles/Player_Pistol.dat"), PreTransformMatrix))))
		return E_FAIL;

	
	/* For.Prototype_Component_Model_Fiona*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fiona"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_ANIM, TEXT("../Bin/DataFiles/Fiona.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* 쉐이더를 로드한다. */
	/* For.Prototype_Component_Shader_VtxNorTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxMesh.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxAnimMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimMesh.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;

	/* 객체원형을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("객체원형을 로딩 중 입니다."));


	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Monster */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster"),
		CMonster::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Skill_Bullet_Red */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Skill_Bullet_Red"),
		CSkill_Player_Bullet::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Cube */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Cube"),
		CCube::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Wall_Stage1 */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Wall_Stage"),
		CWall_Stage1::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Floor_102 */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Floor_102"),
		CFloor_102::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

CLoader * CLoader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVELID eNextLevelID)
{
	CLoader*		pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX(TEXT("Failed to Created : CLoader"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLoader::Free()
{
	__super::Free();

	WaitForSingleObject(m_hThread, INFINITE);

	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_Critical_Section);

	Safe_Release(m_pGameInstance);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
