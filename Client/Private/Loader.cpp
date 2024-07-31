#include "stdafx.h"
#include "..\Public\Loader.h"

#include "GameInstance.h"
#include "Camera_Free.h"
#include "BackGround.h"

#include "Terrain.h"
#include "Sky.h"
#include "Cube.h"

#include "Wall_Stage1.h"
#include "Floor_101.h"
#include "Floor_102.h"
#include "Floor_103.h"
#include "Floor_105.h"
#include "Floor_106.h"
#include "Floor_107.h"
#include "Floor_108.h"
#include "Floor_109.h"
#include "Floor_110.h"
#include "Floor_111.h"
#include "Floor_112.h"
#include "Floor_114.h"

#include "Effect_Deco_Snow.h"

#include "Player.h"
#include "Weapon_Player_Pistol.h"
#include "Weapon_Player_Sniper.h"
#include "Weapon_Player_Shuriken.h"

#include "Skill_Pistol_Bullet.h"
#include "Skill_Shuriken_Bullet.h"

#include "Monster.h"
#include "Mon_Archer.h"
#include "Skill_Archer_Bullet.h"

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

	//Anim 모델을 로드합니다.
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Weapon_Player_Pistol"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/GunFire_Resources/Models/Gun/Weapon_Pistol/Pistol.fbx", TEXT("../Bin/DataFiles/Player_Pistol.dat"), PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Weapon_Player_Sniper"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/GunFire_Resources/Models/Gun/Weapon_Sniper/Sniper.fbx", TEXT("../Bin/DataFiles/Player_Sniper.dat"), PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Weapon_Player_Shuriken"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/GunFire_Resources/Models/Gun/Weapon_Shuriken/Shuriken.fbx", TEXT("../Bin/DataFiles/Player_Shuriken.dat"), PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fiona"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Fiona/Fiona.fbx", TEXT("../Bin/DataFiles/Fiona.dat"), PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster_Archer"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/GunFire_Resources/Models/Monster/Mon_Archer/Archer.fbx", TEXT("../Bin/DataFiles/Monster_Archer.dat"), PreTransformMatrix))))
		return E_FAIL;

	//NonAnim 모델을 로드합니다.
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Wall_Stage1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/GunFire_Resources/Models/Element/Wall_Stage/Stage1/Wall_Stage1.fbx", TEXT("../Bin/DataFiles/Wall_Stage.dat"), PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Floor_101"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/GunFire_Resources/Models/Element/Floor/Floor_101/Floor_101.fbx", TEXT("../Bin/DataFiles/Floor_101.dat"), PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Floor_102"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/GunFire_Resources/Models/Element/Floor/Floor_102/Floor_102.fbx", TEXT("../Bin/DataFiles/Floor_102.dat"), PreTransformMatrix))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Floor_103"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/GunFire_Resources/Models/Element/Floor/Floor_103/Floor_103.fbx", TEXT("../Bin/DataFiles/Floor_103.dat"), PreTransformMatrix))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Floor_105"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/GunFire_Resources/Models/Element/Floor/Floor_105/Floor_105.fbx", TEXT("../Bin/DataFiles/Floor_105.dat"), PreTransformMatrix))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Floor_106"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/GunFire_Resources/Models/Element/Floor/Floor_106/Floor_106.fbx", TEXT("../Bin/DataFiles/Floor_106.dat"), PreTransformMatrix))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Floor_107"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/GunFire_Resources/Models/Element/Floor/Floor_107/Floor_107.fbx", TEXT("../Bin/DataFiles/Floor_107.dat"), PreTransformMatrix))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Floor_108"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/GunFire_Resources/Models/Element/Floor/Floor_108/Floor_108.fbx", TEXT("../Bin/DataFiles/Floor_108.dat"), PreTransformMatrix))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Floor_109"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/GunFire_Resources/Models/Element/Floor/Floor_109/Floor_109.fbx", TEXT("../Bin/DataFiles/Floor_109.dat"), PreTransformMatrix))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Floor_110"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/GunFire_Resources/Models/Element/Floor/Floor_110/Floor_110.fbx", TEXT("../Bin/DataFiles/Floor_110.dat"), PreTransformMatrix))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Floor_111"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/GunFire_Resources/Models/Element/Floor/Floor_111/Floor_111.fbx", TEXT("../Bin/DataFiles/Floor_111.dat"), PreTransformMatrix))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Floor_112"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/GunFire_Resources/Models/Element/Floor/Floor_112/Floor_112.fbx", TEXT("../Bin/DataFiles/Floor_112.dat"), PreTransformMatrix))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Floor_114"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/GunFire_Resources/Models/Element/Floor/Floor_114/Floor_114.fbx", TEXT("../Bin/DataFiles/Floor_114.dat"), PreTransformMatrix))))
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
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/Skill/Player/Bullet/Bullet_%d.dds"), 4))))
		return E_FAIL;

	/* 모델을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("모델(정점 -> 폴리곤 -> 메시 -> 모델)을 로딩 중 입니다."));	

	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, 100 , 100))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 4))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Snow */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Snow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Snow/Snow.png"), 1))))
		return E_FAIL;

	_matrix			PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));

	/* For.Prototype_Component_Model_Weapon_Player_Pistol */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Weapon_Player_Pistol"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_ANIM, TEXT("../Bin/DataFiles/Player_Pistol.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Weapon_Player_Sniper */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Weapon_Player_Sniper"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_ANIM, TEXT("../Bin/DataFiles/Player_Sniper.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Weapon_Player_Sniper */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Weapon_Player_Shuriken"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_ANIM, TEXT("../Bin/DataFiles/Player_Shuriken.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Fiona*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fiona"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_ANIM, TEXT("../Bin/DataFiles/Fiona.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Archer*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster_Archer"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_ANIM, TEXT("../Bin/DataFiles/Monster_Archer.dat"), PreTransformMatrix))))
		return E_FAIL;


	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));

	/* For.Prototype_Component_Model_Wall_State1 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Wall_Stage1"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/DataFiles/Wall_Stage.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Floor_101 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Floor_101"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/DataFiles/Floor_101.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Floor_102 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Floor_102"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/DataFiles/Floor_102.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Floor_103 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Floor_103"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/DataFiles/Floor_103.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Floor_105 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Floor_105"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/DataFiles/Floor_105.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Floor_106 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Floor_106"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/DataFiles/Floor_106.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Floor_107 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Floor_107"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/DataFiles/Floor_107.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Floor_108 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Floor_108"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/DataFiles/Floor_108.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Floor_109 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Floor_109"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/DataFiles/Floor_109.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Floor_110 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Floor_110"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/DataFiles/Floor_110.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Floor_111 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Floor_111"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/DataFiles/Floor_111.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Floor_112 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Floor_112"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/DataFiles/Floor_112.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Floor_114 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Floor_114"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/DataFiles/Floor_114.dat"), PreTransformMatrix))))
		return E_FAIL;
	
	/* For.Prototype_Component_VIBuffer_Cube */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Rect_Instancing */
	CVIBuffer_Instancing::VIBUFFER_INSTANCE_DESC	ParticleDesc{};
	ParticleDesc.iNumInstance = 2000;
	ParticleDesc.vRange = _float3(129.0f, 1.f, 129.f);
	ParticleDesc.vCenter = _float3(64.5f, 20.f, 64.5f);
	ParticleDesc.vPivot = _float3(0.f, -0.f, 0.f);
	ParticleDesc.vSpeed = _float2(1.f, 2.f);
	ParticleDesc.vScale = _float2(0.2f, 0.5f);
	ParticleDesc.vLifeTime = _float2(10.f, 15.f);
	ParticleDesc.isLoop = true;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Particle_Snow"),
		CVIBuffer_Rect_Instancing::Create(m_pDevice, m_pContext, &ParticleDesc))))
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

	/* For.Prototype_Component_Shader_VtxCube */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxCube"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxCube.hlsl"), VTXCUBE::Elements, VTXCUBE::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_Particle_VtxRect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_Particle_VtxRect"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Particle_VtxRect.hlsl"), VTXPARTICLE_RECT::Elements, VTXPARTICLE_RECT::iNumElements))))
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

	/* For.Prototype_GameObject_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Mon_Archer */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Mon_Archer"),
		CMon_Archer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Weapon_Player_Pistol"),
		CWeapon_Player_Pistol::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Weapon_Player_Sniper"),
		CWeapon_Player_Sniper::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Weapon_Player_Shuriken"),
		CWeapon_Player_Shuriken::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Skill_Pistol_Bullet */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Skill_Pistol_Bullet"),
		CSkill_Pistol_Bullet::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Skill_Shuriken_Bullet */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Skill_Shuriken_Bullet"),
		CSkill_Shuriken_Bullet::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Skill_Shuriken_Bullet */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Skill_Archer_Bullet"),
		CSkill_Archer_Bullet::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Cube */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Cube"),
		CCube::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Wall_Stage1 */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Wall_Stage"),
		CWall_Stage1::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Floor_101 */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Floor_101"),
		CFloor_101::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Floor_102 */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Floor_102"),
		CFloor_102::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Floor_103 */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Floor_103"),
		CFloor_103::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Floor_105 */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Floor_105"),
		CFloor_105::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Floor_106 */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Floor_106"),
		CFloor_106::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Floor_107 */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Floor_107"),
		CFloor_107::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Floor_108 */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Floor_108"),
		CFloor_108::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Floor_109 */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Floor_109"),
		CFloor_109::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Floor_110 */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Floor_110"),
		CFloor_110::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Floor_111 */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Floor_111"),
		CFloor_111::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Floor_112 */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Floor_112"),
		CFloor_112::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Floor_114 */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Floor_114"),
		CFloor_114::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Effect_Deco_Snow */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Deco_Snow"),
		CEffect_Deco_Snow::Create(m_pDevice, m_pContext))))
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

	WaitForSingleObject(m_hThread, INFINITE);

	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_Critical_Section);

	Safe_Release(m_pGameInstance);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	__super::Free();
}
