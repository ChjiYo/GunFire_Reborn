#include "stdafx.h"
#include "..\Public\Loader.h"

#include "GameInstance.h"
#include "Camera_Free.h"
#include "BackGround.h"
#include "Boss_Logo.h"

#include "Terrain.h"

#include "Terrain_GamePlay.h"
#include "Terrain_Boss.h"

#include "Sky.h"
#include "Sky_Boss.h"
#include "Cube.h"

#include "Fire_Light.h"

#include "Wall_Stage1.h"
#include "Wall_Stage2.h"

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

#include "Stair1.h"
#include "Stair2.h"
#include "Stair101.h"
#include "Stair101_Addon.h"

#include "Door.h"
#include "Box.h"

#include "Ceiling_101.h"
#include "Ceiling_103.h"
#include "Ceiling_104.h"
#include "Ceiling_105.h"

#include "Pillars101.h"

#include "Mountain_105.h"
#include "Mountain_106.h"
#include "Mountain_107.h"
#include "Mountain_114.h"
#include "Mountain_310.h"
#include "Mountain_311.h"

#include "Fish_Body.h"
#include "Fish_Body2.h"
#include "Fish_Head.h"
#include "Fish_Teath.h"

#include "Effect_Deco_Snow.h"

#include "Player.h"
#include "Weapon_Player_Pistol.h"
#include "Weapon_Player_Sniper.h"
#include "Weapon_Player_Shuriken.h"
#include "Weapon_Player_BoomGun.h"

#include "Skill_Pistol_Bullet.h"
#include "Skill_Sniper_Bullet.h"
#include "Skill_Shuriken_Bullet.h"
#include "Skill_BoomGun_Bullet.h"

#include "Monster.h"
#include "Mon_Archer.h"
#include "Mon_Bug.h"
#include "Mon_Knight.h"
#include "Mon_Shield_Knight.h"
#include "Mon_Spear.h"
#include "Mon_Deash_Boom.h"
#include "Mon_Dragon.h"

#include "Skill_Archer_Bullet.h"
#include "Skill_Mon_Boom.h"
#include "Skill_Dragon_Bubble.h"
#include "Skill_Dragon_Floor.h"
#include "Skill_Dragon_Breath.h"
#include "Skill_Dragon_Dust.h"

#include "VFX_Spectrum.h"

#include "VFX_Mon_Breath.h"
#include "VFX_Mon_Dust.h"
#include "VFX_Mon_Circle.h"
#include "VFX_Mon_PosCircle.h"
#include "VFX_Mon_Bubble.h"

#include "Item_BoomGun.h"

#include "Portal.h"

#include "UI_Boss_HpBar.h"
#include "UI_Boss_Icon.h"
#include "UI_Boss_HpPanel.h"

#include "UI_Number.h"

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

	m_pGameInstance->SetLoadingLevelIndex(m_eLevelID);

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

	case LEVEL_BOSS:
		hr = Loading_For_BossLevel();
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

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Weapon_Player_BoomGun"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/GunFire_Resources/Models/Gun/Weapon_BoomGun/BoomGun.fbx", TEXT("../Bin/DataFiles/Player_BoomGun.dat"), PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fiona"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Fiona/Fiona.fbx", TEXT("../Bin/DataFiles/Fiona.dat"), PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster_Archer"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/GunFire_Resources/Models/Monster/Mon_Archer/Archer.fbx", TEXT("../Bin/DataFiles/Monster_Archer.dat"), PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster_Bug"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/GunFire_Resources/Models/Monster/Mon_Bug/Bug.fbx", TEXT("../Bin/DataFiles/Monster_Bug.dat"), PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster_Boom"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/GunFire_Resources/Models/Monster/Mon_Boom/Boom.fbx", TEXT("../Bin/DataFiles/Monster_Boom.dat"), PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster_Knight"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/GunFire_Resources/Models/Monster/Mon_Knight/Knight.fbx", TEXT("../Bin/DataFiles/Monster_Knight.dat"), PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster_Shield_Knight"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/GunFire_Resources/Models/Monster/Mon_Shield_Knight/Shield_Knight.fbx", TEXT("../Bin/DataFiles/Monster_Shielded_Knight.dat"), PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster_Spear"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/GunFire_Resources/Models/Monster/Mon_Spear/Spear.fbx", TEXT("../Bin/DataFiles/Monster_Spear.dat"), PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster_Deash_Boom"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/GunFire_Resources/Models/Monster/Mon_Deash_Boom/Deash_Boom.fbx", TEXT("../Bin/DataFiles/Monster_Deash_Boom.dat"), PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster_Dragon"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/GunFire_Resources/Models/Monster/Boss/Boss_Dragon/Dragon.fbx", TEXT("../Bin/DataFiles/Dragon.dat"), PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Box"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/GunFire_Resources/Models/Element/Box/Box.fbx", TEXT("../Bin/DataFiles/Box.dat"), PreTransformMatrix))))
		return E_FAIL;
	
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f))* XMMatrixTranslation( 6.f  , 19.5f , -72.f );
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Door"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/GunFire_Resources/Models/Element/Door/Door.fbx", TEXT("../Bin/DataFiles/Door.dat"), PreTransformMatrix))))
		return E_FAIL;

	//NonAnim 모델을 로드합니다.
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Wall_Stage1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/GunFire_Resources/Models/Element/Wall_Stage/Stage1/Wall_Stage1.fbx", TEXT("../Bin/DataFiles/Wall_Stage.dat"), PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Wall_Stage2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/GunFire_Resources/Models/Element/Wall_Stage/Stage2/Wall_Stage2.fbx", TEXT("../Bin/DataFiles/Wall_Stage2.dat"), PreTransformMatrix))))
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

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Stair_1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/GunFire_Resources/Models/Element/Stair/Stair1/Stair_1.fbx", TEXT("../Bin/DataFiles/Stair_1.dat"), PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Stair_2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/GunFire_Resources/Models/Element/Stair/Stair2/Stair_2.fbx", TEXT("../Bin/DataFiles/Stair_2.dat"), PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Stair_101"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/GunFire_Resources/Models/Element/Stair/Stair101/Stair101.fbx", TEXT("../Bin/DataFiles/Stair_101.dat"), PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Stair_101_Addon"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/GunFire_Resources/Models/Element/Stair/Stair101/Addon/Stair101_Addon.fbx", TEXT("../Bin/DataFiles/Stair101_Addon.dat"), PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Ceiling_101"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/GunFire_Resources/Models/Element/Ceiling/101/Ceiling_101.fbx", TEXT("../Bin/DataFiles/Ceiling_101.dat"), PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Ceiling_103"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/GunFire_Resources/Models/Element/Ceiling/103/Ceiling_103.fbx", TEXT("../Bin/DataFiles/Ceiling_103.dat"), PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Ceiling_104"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/GunFire_Resources/Models/Element/Ceiling/104/Ceiling_104.fbx", TEXT("../Bin/DataFiles/Ceiling_104.dat"), PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Ceiling_105"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/GunFire_Resources/Models/Element/Ceiling/105/Ceiling_105.fbx", TEXT("../Bin/DataFiles/Ceiling_105.dat"), PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Pillars101"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/GunFire_Resources/Models/Element/Pillars/101/Pillars101.fbx", TEXT("../Bin/DataFiles/Pillars101.dat"), PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Item_Weapon_Sniper"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/GunFire_Resources/Models/Item/Weapon/Sniper/Item_Sniper.fbx", TEXT("../Bin/DataFiles/Item_Sniper.dat"), PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Item_Weapon_BoomGun"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/GunFire_Resources/Models/Item/Weapon/BoomGun/Item_BoomGun.fbx", TEXT("../Bin/DataFiles/Item_BoomGun.dat"), PreTransformMatrix))))
		return E_FAIL;


	// LEVEL_BOSS

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Model_Mountain105"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/GunFire_Resources/Models/Element/Mountain/105/Mountain105.fbx", TEXT("../Bin/DataFiles/Mountain105.dat"), PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Model_Mountain106"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/GunFire_Resources/Models/Element/Mountain/106/Mountain106.fbx", TEXT("../Bin/DataFiles/Mountain106.dat"), PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Model_Mountain107"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/GunFire_Resources/Models/Element/Mountain/107/Mountain107.fbx", TEXT("../Bin/DataFiles/Mountain107.dat"), PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Model_Mountain114"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/GunFire_Resources/Models/Element/Mountain/114/Mountain114.fbx", TEXT("../Bin/DataFiles/Mountain114.dat"), PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Model_Mountain310"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/GunFire_Resources/Models/Element/Mountain/310/Mountain310.fbx", TEXT("../Bin/DataFiles/Mountain310.dat"), PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Model_Mountain311"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/GunFire_Resources/Models/Element/Mountain/311/Mountain311.fbx", TEXT("../Bin/DataFiles/Mountain311.dat"), PreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Model_Fish_Body"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/GunFire_Resources/Models/Element/Fish/Body/Fish_Body.fbx", TEXT("../Bin/DataFiles/Fish_Body.dat"), PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Model_Fish_Body2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/GunFire_Resources/Models/Element/Fish/Body2/Fish_Body2.fbx", TEXT("../Bin/DataFiles/Fish_Body2.dat"), PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Model_Fish_Head"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/GunFire_Resources/Models/Element/Fish/Head/Fish_Head.fbx", TEXT("../Bin/DataFiles/Fish_Head.dat"), PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Model_Fish_Teath"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/GunFire_Resources/Models/Element/Fish/Teath/Fish_Teath.fbx", TEXT("../Bin/DataFiles/Fish_Teath.dat"), PreTransformMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("바이러리 파일 로드가 완료되었습니다."));

	return S_OK;
}

HRESULT CLoader::Loading_For_StaticLevel()
{
	lstrcpy(m_szLoadingText, TEXT("콜라이더를 로딩 중 입니다."));

	/* For.Prototype_Component_Collider_AABB */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_AABB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_OBB */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_OBB))))
		return E_FAIL;
	
	/* For.Prototype_Component_Collider_Sphere */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_SPHERE))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_Ray */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Ray"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_RAY))))
		return E_FAIL;
	
	lstrcpy(m_szLoadingText, TEXT("텍스처를 로드하는 중입니다."));
	/* For.Prototype_Component_Texture_Terrain */

 	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/Deco/Terrain/Terrain%d.dds"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Skill_Bullet_Red */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Bullet_Red"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/Skill/Player/Bullet/Bullet_%d.dds"), 4))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Number */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Number"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/UI/Num//Num_%d.dds"), 11))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("모델을 로딩 중 입니다."));

	_matrix			PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));

	/* For.Prototype_Component_Model_Weapon_Player_Pistol */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Weapon_Player_Pistol"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_ANIM, TEXT("../Bin/DataFiles/Player_Pistol.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Weapon_Player_Sniper */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Weapon_Player_Sniper"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_ANIM, TEXT("../Bin/DataFiles/Player_Sniper.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Weapon_Player_Sniper */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Weapon_Player_Shuriken"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_ANIM, TEXT("../Bin/DataFiles/Player_Shuriken.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Weapon_Player_BoomGun */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Weapon_Player_BoomGun"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_ANIM, TEXT("../Bin/DataFiles/Player_BoomGun.dat"), PreTransformMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("객체 원형을 로드하는 중입니다."));

	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_Number */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Number"),
		CUI_Number::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_VFX_Specrum */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_VFX_Specrum"),
		CVFX_Spectrum::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_For_LogoLevel()
{
	Loading_For_StaticLevel();
	/* 로고레벨용 자원을 로드하자. */

	/* 텍스쳐를 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩 중 입니다."));

	/* For.Prototype_Component_Texture_Logo */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Logo"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/UI/Logo/UI_Login_Logo.png")))))
		return E_FAIL;

	/* 모델을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("모델을 로딩 중 입니다."));

	/* 객체원형을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("객체원형을 로딩 중 입니다."));

	/* For.Prototype_GameObject_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_Boss_Logo */
	if(FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Boss_Logo"),
		CBoss_Logo::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;
	
	return S_OK;
}

HRESULT CLoader::Loading_For_GamePlayLevel()
{
	/* 게임플레이레벨용 자원을 로드하자. */

		/* 네비게이션을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("네비게이션을 로딩 중 입니다."));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/DataFiles/Navigation.dat")))))
		return E_FAIL;

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

	/* For.Prototype_Component_Texture_Portal*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Portal"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/Deco/Portal/Portal_Tex.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Boom_Fire*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Boom_Fire"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/Skill/Monster/Boom/sprite_%d.png"), 15))))
		return E_FAIL;


	/* For.Prototype_Component_Texture_Noise */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Noise"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Models/Monster/NoiseCaustic03.png")))))
		return E_FAIL;

	/* 모델을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("모델(정점 -> 폴리곤 -> 메시 -> 모델)을 로딩 중 입니다."));	


	/* For.Prototype_Component_Texture_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 6))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Snow */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Snow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Snow/Snow.png"), 1))))
		return E_FAIL;

	_matrix			PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));

	/* For.Prototype_Component_Model_Archer*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster_Archer"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_ANIM, TEXT("../Bin/DataFiles/Monster_Archer.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Monster_Archer*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster_Bug"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_ANIM, TEXT("../Bin/DataFiles/Monster_Bug.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Monster_Knight*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster_Knight"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_ANIM, TEXT("../Bin/DataFiles/Monster_Knight.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Archer*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster_Shield_Knight"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_ANIM, TEXT("../Bin/DataFiles/Monster_Shielded_Knight.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Archer*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster_Spear"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_ANIM, TEXT("../Bin/DataFiles/Monster_Spear.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Archer*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster_Boom"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_ANIM, TEXT("../Bin/DataFiles/Monster_Boom.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Archer*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster_Deash_Boom"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_ANIM, TEXT("../Bin/DataFiles/Monster_Deash_Boom.dat"), PreTransformMatrix))))
		return E_FAIL;



	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f)) * XMMatrixTranslation(-5.8f, 19.2f, -87.8f);
	//17.1f, 22.8f, -64.2f
	/* For.Prototype_Component_Model_Door */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Door"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_ANIM, TEXT("../Bin/DataFiles/Door.dat"), PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix =  XMMatrixRotationY(XMConvertToRadians(180.0f)) * XMMatrixTranslation(-38.75f, 1.f, -20.f);

	/* For.Prototype_Component_Model_Box */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Box"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_ANIM, TEXT("../Bin/DataFiles/Box.dat"), PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	/* For.Prototype_Component_Model_Wall_State1 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Wall_Stage1"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/DataFiles/Wall_Stage.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Wall_State2 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Wall_Stage2"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/DataFiles/Wall_Stage2.dat"), PreTransformMatrix))))
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

	/* For.Prototype_Component_Model_Stair1 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Stair1"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/DataFiles/Stair_1.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Stair2 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Stair2"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/DataFiles/Stair_2.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Stair101 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Stair101"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/DataFiles/Stair_101.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Stair101_Addon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Stair101_Addon"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/DataFiles/Stair101_Addon.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Ceiling_101 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Ceiling_101"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/DataFiles/Ceiling_101.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Ceiling_103 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Ceiling_103"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/DataFiles/Ceiling_103.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Ceiling_104 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Ceiling_104"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/DataFiles/Ceiling_104.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Ceiling_105 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Ceiling_105"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/DataFiles/Ceiling_105.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Pillars_101 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Pillars_101"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/DataFiles/Pillars101.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Item_Weapon_Sniper */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Item_Weapon_BoomGun"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/DataFiles/Item_BoomGun.dat"), PreTransformMatrix))))
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

	/* 객체원형을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("객체원형을 로딩 중 입니다."));


	/* For.Prototype_GameObject_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Terrain_GamePlay */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain_GamePlay"),
		CTerrain_GamePlay::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Mon_Archer */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Mon_Archer"),
		CMon_Archer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Mon_Bug */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Mon_Bug"),
		CMon_Bug::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Mon_Deash_Boom */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Mon_Deash_Boom"),
		CMon_Deash_Boom::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Mon_Knight */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Mon_Knight"),
		CMon_Knight::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Mon_Spear */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Mon_Spear"),
		CMon_Spear::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Mon_Shield_Knight */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Mon_Shield_Knight"),
		CMon_Shield_Knight::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Weapon_Player_Pistol */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Weapon_Player_Pistol"),
		CWeapon_Player_Pistol::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Weapon_Player_Sniper */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Weapon_Player_Sniper"),
		CWeapon_Player_Sniper::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Weapon_Player_Shuriken */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Weapon_Player_Shuriken"),
		CWeapon_Player_Shuriken::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Weapon_Player_BoomGun */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Weapon_Player_BoomGun"),
		CWeapon_Player_BoomGun::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Skill_Pistol_Bullet */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Skill_Pistol_Bullet"),
		CSkill_Pistol_Bullet::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Skill_Shuriken_Bullet */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Skill_Shuriken_Bullet"),
		CSkill_Shuriken_Bullet::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Skill_BoomGun_Bullet */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Skill_BoomGun_Bullet"),
		CSkill_BoomGun_Bullet::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Skill_Sniper_Bullet */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Skill_Sniper_Bullet"),
		CSkill_Sniper_Bullet::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Skill_Shuriken_Bullet */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Skill_Archer_Bullet"),
		CSkill_Archer_Bullet::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Skill_Mon_Boom */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Skill_Mon_Boom"),
		CSkill_Mon_Boom::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Cube */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Cube"),
		CCube::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Wall_Stage1 */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Wall_Stage"),
		CWall_Stage1::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Wall_Stage2 */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Wall_Stage2"),
		CWall_Stage2::Create(m_pDevice, m_pContext))))
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

	/* For.Prototype_GameObject_Stair1 */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Stair1"),
		CStair1::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Stair2 */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Stair2"),
		CStair2::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Stair101 */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Stair101"),
		CStair101::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Stair101_Addon */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Stair101_Addon"),
		CStair101_Addon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Ceiling_101 */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Ceiling_101"),
		CCeiling_101::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Ceiling_103 */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Ceiling_103"),
		CCeiling_103::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Ceiling_104 */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Ceiling_104"),
		CCeiling_104::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Ceiling_105 */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Ceiling_105"),
		CCeiling_105::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Pillars_101*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Pillars_101"),
		CPillars101::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Door*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Door"),
		CDoor::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Door*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Box"),
		CBox::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Portal*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Portal"),
		CPortal::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_FireLight*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_FireLight"),
		CFire_Light::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_BoomGun"),
		CItem_BoomGun::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Effect_Deco_Snow */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Deco_Snow"),
		CEffect_Deco_Snow::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_BossLevel()
{

	/* 네비게이션을 로드한다. */

	lstrcpy(m_szLoadingText, TEXT("네비게이션을 로딩 중 입니다."));

	/* Prototype_Component_Navigation_Desert */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/DataFiles/DesertNavigation.dat")))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("컴포넌트를 로딩 중 입니다."));

	/* For.Prototype_Component_VIBuffer_Particle_Dust */
	CVIBuffer_Instancing::VIBUFFER_INSTANCE_DESC	ParticleDesc{};

	ParticleDesc.iNumInstance = 200;
	ParticleDesc.vRange = _float3(0.1f, 0.0f, 0.1f);
	ParticleDesc.vCenter = _float3(0.0f, 0.f, 0.0f);
	ParticleDesc.vPivot = _float3(0.0f, 0.0f, 0.f);
	ParticleDesc.vSpeed = _float2(15.0f, 15.f);
	ParticleDesc.vScale = _float2(0.3f, 1.7f);
	ParticleDesc.vLifeTime = _float2(10.0f, 10.5f);
	ParticleDesc.isLoop = true;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_VIBuffer_Particle_Dust"),
		CVIBuffer_Point_Instancing::Create(m_pDevice, m_pContext, &ParticleDesc))))
		return E_FAIL;

	ParticleDesc.iNumInstance = 200;
	ParticleDesc.vRange = _float3(10.0f, 0.0f, 10.0f);
	ParticleDesc.vCenter = _float3(0.0f, 0.f, 0.0f);
	ParticleDesc.vPivot = _float3(0.0f, 0.0f, 0.f);
	ParticleDesc.vSpeed = _float2(0.0f, 0.f);
	ParticleDesc.vScale = _float2(1.0f, 3.0f);
	ParticleDesc.vLifeTime = _float2(0.5f, 3.5f);
	ParticleDesc.isLoop = true;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_VIBuffer_Particle_Circle"),
		CVIBuffer_Point_Instancing::Create(m_pDevice, m_pContext, &ParticleDesc))))
		return E_FAIL;

	ParticleDesc.iNumInstance = 10;
	ParticleDesc.vRange = _float3(2.0f, 0.0f, 2.0f);
	ParticleDesc.vCenter = _float3(0.0f, 0.f, 0.0f);
	ParticleDesc.vPivot = _float3(0.0f, 0.0f, 0.f);
	ParticleDesc.vSpeed = _float2(1.0f, 3.f);
	ParticleDesc.vScale = _float2(0.1f, 0.25f);
	ParticleDesc.vLifeTime = _float2(0.5f, 1.f);
	ParticleDesc.vMoveDir = {0.f, 0.f , -1.f};
	ParticleDesc.isLoop = true;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_VIBuffer_Particle_Bubble"),
		CVIBuffer_Point_Instancing::Create(m_pDevice, m_pContext, &ParticleDesc))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_State"),
		CState_Machine::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("텍스처을 로딩 중 입니다."));

	/* For.Prototype_Component_Texture_Skill_Dragon_Bubble*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Texture_Skill_Dragon_Bubble"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/Skill/Monster/Dragon/Bubble/Skill_Bubble%d.png"), 4))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Skill_Dragon_Floor*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Texture_Skill_Dragon_Floor"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/Skill/Monster/Dragon/Floor/Floor.png")))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_Skill_Dragon_Floor_Addon*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Texture_Skill_Dragon_Floor_Addon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/Skill/Monster/Dragon/Floor/Floor_Addon.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Skill_Dragon_Breath*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Texture_Skill_Dragon_Breath"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/Skill/Monster/Dragon/Breath/Breath%d.png"), 8))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_VFX_Dragon_Breath*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Texture_VFX_Dragon_Breath"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/Skill/Monster/Dragon/Breath/Breath_Mask.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_VFX_Dragon_Dust*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Texture_VFX_Dragon_Dust"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/Skill/Monster/Dragon/Dust/Dust%d.png"), 14))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_VFX_Pos_Circle */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Texture_VFX_Pos_Circle"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/Skill/Monster/Dragon/Pos_Circle/Pos_Circle.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/SkyBox/Sky_6.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Boss_Icon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Texture_UI_Boss_Icon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/UI/Bar/UI_Boss_Icon.png")))))
		return E_FAIL;
	
	/* For.Prototype_Component_Texture_UI_Boss_HpBar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Texture_UI_Boss_HpBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/UI/Bar/UI_Monster_HpBlankBar.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Boss_Hp_Panel */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Texture_UI_Boss_Hp_Panel"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/UI/Bar/UI_Boss_Hp_Panel.png")))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("모델을 로딩 중 입니다."));

	_matrix			PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));

	/* For.Prototype_Component_Model_Monster_Dragon*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Model_Monster_Dragon"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_ANIM, TEXT("../Bin/DataFiles/Dragon.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Archer*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Model_Monster_Archer"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_ANIM, TEXT("../Bin/DataFiles/Monster_Archer.dat"), PreTransformMatrix))))
		return E_FAIL;

		PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));

		/* For.Prototype_Component_Model_Mountain105*/
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Model_Mountain105"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/DataFiles/Mountain105.dat"), PreTransformMatrix))))
			return E_FAIL;

	/* For.Prototype_Component_Model_Mountain106*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Model_Mountain106"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/DataFiles/Mountain106.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Mountain107*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Model_Mountain107"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/DataFiles/Mountain107.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Mountain114*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Model_Mountain114"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/DataFiles/Mountain114.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Mountain310*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Model_Mountain310"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/DataFiles/Mountain310.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Mountain311 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Model_Mountain311"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/DataFiles/Mountain311.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Fish_Body */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Model_Fish_Body"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/DataFiles/Fish_Body.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Fish_Body2 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Model_Fish_Body2"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/DataFiles/Fish_Body2.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Fish_Head */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Model_Fish_Head"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/DataFiles/Fish_Head.dat"), PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Fish_Teath */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Model_Fish_Teath"),
		CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/DataFiles/Fish_Teath.dat"), PreTransformMatrix))))
		return E_FAIL;

	
	lstrcpy(m_szLoadingText, TEXT("객체원형을 로딩 중 입니다."));

	/* For.Prototype_GameObject_Sky_Boss */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky_Boss"),
		CSky_Boss::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Terrain_Boss */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain_Boss"),
		CTerrain_Boss::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Mountain_105 */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Mountain_105"),
		CMountain_105::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Mountain_106 */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Mountain_106"),
		CMountain_106::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Mountain_107 */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Mountain_107"),
		CMountain_107::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Mountain_114 */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Mountain_114"),
		CMountain_114::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Mountain_310 */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Mountain_310"),
		CMountain_310::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Mountain_311 */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Mountain_311"),
		CMountain_311 ::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Fish_Body */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Fish_Body"),
		CFish_Body::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Fish_Body2 */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Fish_Body2"),
		CFish_Body2::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Fish_Head */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Fish_Head"),
		CFish_Head::Create(m_pDevice, m_pContext))))
		return E_FAIL; 
	/* For.Prototype_GameObject_Fish_Teath */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Fish_Teath"),
		CFish_Teath::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Mon_Dragon */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Mon_Dragon"),
		CMon_Dragon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Skill_Dragon_Bubble*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Skill_Dragon_Bubble"),
		CSkill_Dragon_Bubble::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Skill_Dragon_Floor*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Skill_Dragon_Floor"),
		CSkill_Dragon_Floor::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Skill_Dragon_Breath*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Skill_Dragon_Breath"),
		CSkill_Dragon_Breath::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Skill_Dragon_Dust */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Skill_Dragon_Dust"),
		CSkill_Dragon_Dust::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_VFX_Dragon_Breath */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_VFX_Dragon_Breath"),
		CVFX_Mon_Breath::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_VFX_Dragon_Dust */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_VFX_Dragon_Dust"),
		CVFX_Mon_Dust::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_VFX_Dragon_Circle */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_VFX_Dragon_Circle"),
		CVFX_Mon_Circle::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_VFX_Dragon_PosCircle */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_VFX_Dragon_PosCircle"),
		CVFX_Mon_PosCircle::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_VFX_Dragon_Bubble */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_VFX_Dragon_Bubble"),
		CVFX_Mon_Bubble::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_Boss_HpBar */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Boss_HpBar"),
		CUI_Boss_HpBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	/* For.Prototype_GameObject_UI_Boss_Icon*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Boss_Icon"),
		CUI_Boss_Icon::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	/* For.Prototype_GameObject_UI_Boss_HpPanel*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Boss_HpPanel"),
		CUI_Boss_HpPanel::Create(m_pDevice, m_pContext))))
		return E_FAIL;

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
