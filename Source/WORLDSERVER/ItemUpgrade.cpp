#include "stdafx.h"
#include ".\itemupgrade.h"

#if __VER >= 12 // __EXT_PIERCING
#include "defineObj.h"
#include "defineSound.h"
#include "defineText.h"

#if __VER >= 11 // __SYS_COLLECTING
#include "collecting.h"
#include "definesound.h"
#include "defineitem.h"
#endif	// __SYS_COLLECTING

#include "User.h"
extern CUserMng g_UserMng;
#include "DPSrvr.h"
extern CDPSrvr g_DPSrvr;
#include "DPDatabaseClient.h"
extern CDPDatabaseClient g_dpDBClient;

#if __VER >= 15 // __PETVIS
#include "AIPet.h"
#endif // __PETVIS

CItemUpgrade::CItemUpgrade(void)
#ifdef __SYS_ITEMTRANSY
: m_nItemTransyLowLevel(1000000), m_nItemTransyHighLevel(2000000)
#endif // __SYS_ITEMTRANSY
{
	LoadScript();
}

CItemUpgrade::~CItemUpgrade(void)
{
	m_mapSuitProb.clear();
	m_mapWeaponProb.clear();
#if __VER >= 13 // __EXT_ENCHANT
	m_mapGeneralEnchant.clear();
	m_mapAttributeEnchant.clear();
#endif // __EXT_ENCHANT
}

CItemUpgrade* CItemUpgrade::GetInstance( void )
{	
	static CItemUpgrade sItemUpgrade;
	return &sItemUpgrade;
}

void CItemUpgrade::LoadScript()
{
	CLuaBase	lua;
	if( lua.RunScript( "ItemUpgrade.lua" ) != 0 )
	{
		Error( "CItemUpgrade::LoadScript() - ItemUpgrade.lua Run Failed!!!" );
		ASSERT(0);
	}

	// �� �Ǿ��
	lua.GetGloabal( "tSuitProb" );
	lua.PushNil();
	while( lua.TableLoop( -2 ) )
	{
		m_mapSuitProb.insert( make_pair( static_cast<int>(lua.ToNumber(-2)), static_cast<int>(lua.ToNumber(-1)) ) );
		lua.Pop( 1 );
	}
	lua.Pop(0);

	// ���� �Ǿ��
	lua.GetGloabal( "tWeaponProb" );
	lua.PushNil();
	while( lua.TableLoop( -2 ) )
	{
		m_mapWeaponProb.insert( make_pair( static_cast<int>(lua.ToNumber(-2)), static_cast<int>(lua.ToNumber(-1)) ) );
		lua.Pop( 1 );
	}
	lua.Pop(0);
	
#if __VER >= 13 // __EXT_ENCHANT
	// �Ϲ�����
	lua.GetGloabal( "tGeneral" );
	lua.PushNil();
	while( lua.TableLoop( -2 ) )
	{
		m_mapGeneralEnchant.insert( make_pair( static_cast<int>(lua.ToNumber(-2)), static_cast<int>(lua.ToNumber(-1)) ) );
		lua.Pop( 1 );
	}
	lua.Pop(0);

 	// �Ӽ�����
	lua.GetGloabal( "tAttribute" );
 	lua.PushNil();
 	while( lua.TableLoop( -2 ) )
 	{
 		__ATTRIBUTE_ENCHANT attrEnchant;
		attrEnchant.nProb = static_cast<int>(lua.GetFieldToNumber( -1, "nProb" ));
		attrEnchant.nAddDamageRate = static_cast<int>(lua.GetFieldToNumber( -1, "nDamageRate" ));
#if __VER >= 14 // __EXT_ATTRIBUTE
		attrEnchant.nDefenseRate = static_cast<int>(lua.GetFieldToNumber( -1, "nDefenseRate" ));
		attrEnchant.nAddAtkDmgRate = static_cast<int>(lua.GetFieldToNumber( -1, "nAddAtkDmgRate" ));
#endif // __EXT_ATTRIBUTE
		m_mapAttributeEnchant.insert( make_pair( static_cast<int>(lua.ToNumber(-2)), attrEnchant ) );  
		lua.Pop( 1 );
#if __VER < 14 // __EXT_ATTRIBUTE
		if( ( ::GetLanguage() == LANG_FRE || ::GetLanguage() == LANG_GER ) && m_mapAttributeEnchant.size() == 10 )
			break;
#endif // __EXT_ATTRIBUTE
  	}
	lua.Pop(0);
#endif // __EXT_ENCHANT
#ifdef __SYS_ITEMTRANSY
	m_nItemTransyLowLevel = static_cast<int>( lua.GetGlobalNumber( "nItemTransyLowLevel" ) );
	m_nItemTransyHighLevel = static_cast<int>( lua.GetGlobalNumber( "nItemTransyHighLevel" ) );
	lua.Pop(0);
#endif // __SYS_ITEMTRANSY
}


void CItemUpgrade::OnPiercingSize( CUser* pUser, DWORD dwId1, DWORD dwId2, DWORD dwId3 )
{
	CItemElem* pItemElem0	= pUser->m_Inventory.GetAtId( dwId1 );
	CItemElem* pItemElem1	= pUser->m_Inventory.GetAtId( dwId2 );
	CItemElem* pItemElem2	= pUser->m_Inventory.GetAtId( dwId3 );

	if( IsUsableItem( pItemElem0 ) == FALSE || IsUsableItem( pItemElem1 ) == FALSE )
		return;

	if( pUser->m_vtInfo.GetOther() || pUser->m_vtInfo.VendorIsVendor() ) // �ŷ� ���̸�...
		return;	
	
	if( pUser->m_Inventory.IsEquip( pItemElem0->m_dwObjId ) )
	{
		pUser->AddDefinedText( TID_GAME_EQUIPPUT );
		return;
	}	

	//////////////// ù��° ������ //////////////// 
	if( !pItemElem0->IsPierceAble( NULL_ID, TRUE ) )
	{
		pUser->AddDefinedText( TID_PIERCING_POSSIBLE_ITEM );
		return;
	}

	if( pItemElem1->GetProp()->dwID != II_GEN_MAT_MOONSTONE 
		&& pItemElem1->GetProp()->dwID != II_GEN_MAT_MOONSTONE_1 )
	{
		pUser->AddDefinedText( TID_SBEVE_NOTUSEITEM );			// �Ǿ�̿� �ʿ��� �ֻ����� �ƴϸ� �Ұ���
		return;
	}
	
	if( IsUsableItem( pItemElem2 ) && pItemElem2->m_dwItemId != II_SYS_SYS_SCR_PIEPROT )
	{
		pUser->AddDefinedText( TID_SBEVE_NOTUSEITEM );			// ���������� �ƴϸ� �Ұ���
		return;
	}

	LogItemInfo aLogItem;
	aLogItem.SendName = pUser->GetName();
	aLogItem.RecvName = "PIERCING";
	aLogItem.WorldId = pUser->GetWorld()->GetID();

	int nCost = 100000;

	if( 0 < nCost )
	{
		if( pUser->GetGold() < nCost )
		{
			pUser->AddDefinedText( TID_GAME_LACKMONEY , "" );
			return;
		}

		pUser->AddGold( -( nCost ) );

		aLogItem.Gold = pUser->GetGold() + nCost;
		aLogItem.Gold2 = pUser->GetGold();
		aLogItem.Action = "!";
		//aLogItem.ItemName = "SEED";
		_stprintf( aLogItem.szItemName, "%d", II_GOLD_SEED1 );
		aLogItem.itemNumber = nCost;
		g_DPSrvr.OnLogItem( aLogItem );
	}
	else
	{
		return;
	}
	aLogItem.Gold = aLogItem.Gold2 = pUser->GetGold();

	int nPersent = 0;
	if( pItemElem1->GetProp()->dwID == II_GEN_MAT_MOONSTONE
		|| pItemElem1->GetProp()->dwID == II_GEN_MAT_MOONSTONE_1 )
		nPersent = GetSizeProb( pItemElem0 );

	if( nPersent < (int)( xRandom( 10000 ) ) )
	{	// ����
		if( pItemElem2 )								// ���ȭ �������� ����Ͽ�����...
			aLogItem.RecvName = "PIERCING_PROTECTED";
		aLogItem.Action = "!";
		g_DPSrvr.OnLogItem( aLogItem, pItemElem0, pItemElem0->m_nItemNum );
		aLogItem.RecvName = "PIERCING";

		pUser->AddPlaySound( SND_INF_UPGRADEFAIL );
		g_UserMng.AddCreateSfxObj((CMover *)pUser, XI_INT_FAIL, pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z);
		pUser->AddDefinedText( TID_MMI_PIERCINGFAIL , "" );
		
		if( pItemElem2 == NULL )								// ���ȭ �������� ������� �ʾҴٸ� 
			pUser->RemoveItem( (BYTE)( dwId1 ), (short)1 );	// �Ǿ�� ��� �������� �����ȴ�.			
	}
	else
	{	// ����			
		pUser->AddPlaySound( SND_INF_UPGRADESUCCESS );			
		g_UserMng.AddCreateSfxObj((CMover *)pUser, XI_INT_SUCCESS, pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z);			
		pUser->UpdateItem( (BYTE)pItemElem0->m_dwObjId, UI_PIERCING_SIZE, pItemElem0->GetPiercingSize() + 1 );
		pUser->AddDefinedText( TID_MMI_PIERCINGSUCCESS , "" );

		aLogItem.Action = "#";
		g_DPSrvr.OnLogItem( aLogItem, pItemElem0, pItemElem0->m_nItemNum );
	}
	aLogItem.Action = "!";
	g_DPSrvr.OnLogItem( aLogItem, pItemElem1, pItemElem1->m_nItemNum );

	// ���̽��� ��������� �����Ѵ�.
	pUser->RemoveItem( (BYTE)( dwId2 ), (short)1 );

	if( dwId3 != NULL_ID )
	{
		aLogItem.Action = "!";
		if( IsUsableItem( pItemElem2 ) )
		{
			g_DPSrvr.OnLogItem( aLogItem, pItemElem2, pItemElem2->m_nItemNum );
			g_dpDBClient.SendLogSMItemUse( "2", pUser, pItemElem2, pItemElem2->GetProp() );
			pUser->RemoveItem( (BYTE)( dwId3 ), (short)1 );
		}
		else
		{
			g_DPSrvr.OnLogItem( aLogItem, NULL, 0 );
		}
	}
}

int CItemUpgrade::GetSizeProb( CItemElem* pItemElem )
{
	// �������...IK3_SOCKETCARD�� �����̸� ��Ʈ...
	if( pItemElem->IsPierceAble( IK3_SOCKETCARD ) )
	{
		//return m_vecSuitProb.size() >= pItemElem->GetPiercingSize() ? m_vecSuitProb[pItemElem->GetPiercingSize()] : 0;
		map<int, int>::iterator it = m_mapSuitProb.find( pItemElem->GetPiercingSize()+1 );
		if( it != m_mapSuitProb.end() )
			return it->second;
	}
	
	// �������...IK3_SOCKETCARD2�� �����̸� ������...
	if( pItemElem->IsPierceAble( IK3_SOCKETCARD2 ) )
	{
		//return m_vecWeaponProb.size() >= pItemElem->GetPiercingSize() ? m_vecWeaponProb[pItemElem->GetPiercingSize()] : 0;
		map<int, int>::iterator it = m_mapWeaponProb.find( pItemElem->GetPiercingSize()+1 );
		if( it != m_mapWeaponProb.end() )
			return it->second;
	}

	return 0;
}

void CItemUpgrade::OnPiercing( CUser* pUser, DWORD dwItemId, DWORD dwSocketCard )
{
	// �κ��丮�� �ִ��� �����Ǿ� �ִ��� Ȯ���� �ؾ� ��
	CItemElem* pItemElem0	= pUser->m_Inventory.GetAtId( dwItemId );
	CItemElem* pItemElem1	= pUser->m_Inventory.GetAtId( dwSocketCard );
	if( IsUsableItem( pItemElem0 ) == FALSE || IsUsableItem( pItemElem1 ) == FALSE )
		return;

	// �����Ǿ� �ִ� �������� �Ǿ�� ����
	if( pUser->m_Inventory.IsEquip( dwItemId ) )
	{
		pUser->AddDefinedText( TID_GAME_EQUIPPUT , "" );
		return;
	}			

	// ī�尡 �� �������� �Ǿ�� �������� �˻�
	if( !pItemElem0->IsPierceAble() )
	{
		pUser->AddDefinedText(  TID_PIERCING_POSSIBLE_ITEM, "" );
		return;
	}

	//  IK3_SOCKETCARD?�� �ƴϸ� �Ǿ�� ����
	if( !pItemElem0->IsPierceAble( pItemElem1->GetProp()->dwItemKind3 ) )
	{
		pUser->AddDefinedText( TID_UPGRADE_ERROR_WRONGUPLEVEL , "" );			
		return;					
	}

	// �� �Ǿ�̵ȼ��� ��ü ���� ���Ѵ�.
	int nSize = pItemElem0->GetPiercingSize();

	int nCount = 0;
	for( int j = 0; j < nSize; j++ )
	{
		if( pItemElem0->GetPiercingItem( j ) != 0 )
			nCount++;
	}

	// ����� ������ �ߴ�
	if( nCount == nSize )
	{
		pUser->AddDefinedText( TID_PIERCING_ERROR_NOPIERCING, "" );
		return;
	}

	// ˬ
	if( pUser->m_vtInfo.GetOther() )	// �ŷ����� ����� ������?
		return;
	if( pUser->m_vtInfo.VendorIsVendor() )		// ���� �Ȱ� ������?
		return;

	LogItemInfo aLogItem;
	aLogItem.SendName = pUser->GetName();
	aLogItem.RecvName = "PIERCING";
	aLogItem.WorldId = pUser->GetWorld()->GetID();
	aLogItem.Gold = aLogItem.Gold2 = pUser->GetGold();

	if( pUser->Pierce( pItemElem0, pItemElem1->m_dwItemId ) )
	{
		aLogItem.Action = "$";
		g_DPSrvr.OnLogItem( aLogItem, pItemElem0, pItemElem0->m_nItemNum );
		aLogItem.Action = "!";
		g_DPSrvr.OnLogItem( aLogItem, pItemElem1, pItemElem1->m_nItemNum );

		// ������ �ڱ� ����~
		pUser->AddPlaySound( SND_INF_UPGRADESUCCESS );
		g_UserMng.AddCreateSfxObj((CMover *)pUser, XI_INT_SUCCESS, pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z);
		// ��� ������ ����
		pUser->RemoveItem( (BYTE)( dwSocketCard ), (short)1 );
	}

}

void CItemUpgrade::OnPiercingRemove( CUser* pUser, DWORD objId )
{
	CItemElem* pItemElem = pUser->m_Inventory.GetAtId( objId );
	if( !IsUsableItem( pItemElem ) || !pItemElem->IsPierceAble() )
		return;

	if( pUser->m_Inventory.IsEquip( objId ) )
		return;

	// �Ǿ�� �ɼ��� ���� ���
	if( pItemElem->GetPiercingSize() == 0 || pItemElem->GetPiercingItem( 0 ) == 0 )
	{
		pUser->AddDefinedText( TID_GAME_REMOVE_PIERCING_ERROR );
		return;
	}

	int nPayPenya = 1000000; // ������ ���
	if( pUser->GetGold() < nPayPenya )	// ��İ� �����ϴ�.
	{
		pUser->AddDefinedText( TID_GAME_LACKMONEY );
		return;
	}

	for( int i=pItemElem->GetPiercingSize()-1; i>=0; i-- )
	{
		if( pItemElem->GetPiercingItem( i ) != 0 )
		{
			pUser->AddGold( -nPayPenya );	// ��� ����
			pUser->AddDefinedText( TID_GAME_REMOVE_PIERCING_SUCCESS );
			pUser->UpdateItem( (BYTE)( pItemElem->m_dwObjId ), UI_PIERCING, MAKELONG( i, 0 ) );

			LogItemInfo aLogItem;
			aLogItem.Action = "$";
			aLogItem.SendName = pUser->GetName();
			aLogItem.RecvName = "PIERCING_REMOVE";
			aLogItem.WorldId = pUser->GetWorld()->GetID();
			aLogItem.Gold = pUser->GetGold() + nPayPenya;
			aLogItem.Gold2 = pUser->GetGold();
			aLogItem.Gold_1 = -nPayPenya;
			g_DPSrvr.OnLogItem( aLogItem, pItemElem, 1 );
			break;
		}
	}
}
#endif // __EXT_PIERCING

#if __VER >= 13 // __EXT_ENCHANT
void	CItemUpgrade::OnEnchant( CUser* pUser, CItemElem* pItemMain, CItemElem* pItemMaterial )
{
#if __VER >= 14 // __SMELT_SAFETY
	if( !IsUsableItem( pItemMain ) || !IsUsableItem( pItemMaterial ) )
		return;
	// ����� �������ΰ�?
	if( pUser->m_Inventory.IsEquip( pItemMain->m_dwObjId ) )
	{
		pUser->AddDefinedText( TID_GAME_EQUIPPUT , "" );
		return;
	}
#endif // __SMELT_SAFETY
	
	switch( pItemMaterial->GetProp()->dwItemKind3 )
	{
		case IK3_ELECARD:	
			EnchantAttribute( pUser, pItemMain, pItemMaterial );
			break;

		case IK3_ENCHANT:	
			EnchantGeneral( pUser, pItemMain, pItemMaterial );
			break;

		default:
#if __VER >= 14 // __SMELT_SAFETY
			if( pItemMain->IsAccessory() )
				RefineAccessory( pUser, pItemMain, pItemMaterial );

			else if( pItemMain->IsCollector() )
				RefineCollector( pUser, pItemMain, pItemMaterial );
#endif // __SMELT_SAFETY
			break;
	}
}

#if __VER >= 14 // __SMELT_SAFETY
BYTE	CItemUpgrade::OnSmeltSafety( CUser* pUser, CItemElem* pItemMain, CItemElem* pItemMaterial, CItemElem* pItemProtScr, CItemElem* pItemSmeltScr )
{
	// ��ῡ ���� �б�
	switch( pItemMaterial->GetProp()->dwItemKind3 )
	{
		//	����Į���϶�
		case IK3_ENCHANT:
			//	�Ϲ�����(�Ϲ� or ���͸ڿ������� ��˻�)
			return SmeltSafetyNormal( pUser, pItemMain, pItemMaterial, pItemProtScr, pItemSmeltScr );

		//	�������϶�
		case IK3_PIERDICE:
			//	�Ǽ������ΰ�
			if( pItemMain->IsAccessory() )
				return SmeltSafetyAccessory( pUser, pItemMain, pItemMaterial, pItemProtScr );
			//	�Ǿ���ΰ�
			else if( pItemMain->IsPierceAble( NULL_ID, TRUE ) )
				return SmeltSafetyPiercingSize( pUser, pItemMain, pItemMaterial, pItemProtScr );

#if __VER >= 15 // __15_5TH_ELEMENTAL_SMELT_SAFETY
		// �Ӽ�ī�� �϶�
		case IK3_ELECARD:
			return SmeltSafetyAttribute( pUser, pItemMain, pItemMaterial, pItemProtScr, pItemSmeltScr );
#endif // __15_5TH_ELEMENTAL_SMELT_SAFETY

		default:
			break;
	}
	return 0;
}

BYTE	CItemUpgrade::SmeltSafetyNormal( CUser* pUser, CItemElem* pItemMain, CItemElem* pItemMaterial, CItemElem* pItemProtScr, CItemElem* pItemSmeltScr )
{
	//	����Į���ΰ�, ������ ����Į���ΰ�
	switch( pItemMaterial->GetProp()->dwID )
	{
		//	����Į���϶� �Ϲ�����
		case II_GEN_MAT_ORICHALCUM01:
		case II_GEN_MAT_ORICHALCUM01_1:
			return SmeltSafetyGeneral( pUser, pItemMain, pItemMaterial, pItemProtScr, pItemSmeltScr );

		//	������ ����Į���϶� ���͸ڿ��� ����
		case II_GEN_MAT_ORICHALCUM02:
			return prj.m_UltimateWeapon.SmeltSafetyUltimate( pUser, pItemMain, pItemMaterial, pItemProtScr );

		default:
			break;
	}
	return 0;
}

BYTE	CItemUpgrade::SmeltSafetyGeneral( CUser* pUser, CItemElem* pItemMain, CItemElem* pItemMaterial, CItemElem* pItemProtScr, CItemElem* pItemSmeltScr )
{
	//	���ð����� �������� �ƴҰ�� ����
	if( !CItemElem::IsDiceRefineryAble(pItemMain->GetProp()) )
	{
		//pUser->AddDefinedText( TID_GAME_NOTEQUALITEM );
		return 0;
	}
	
	//	���͸ڿ��� �̰ų� �Ϲݺ�ȣ�� �η縶���� �ƴҰ�� ����
	if( pItemMain->GetProp()->dwReferStat1 == WEAPON_ULTIMATE || pItemProtScr->GetProp()->dwID != II_SYS_SYS_SCR_SMELPROT )
	{
		//pUser->AddDefinedText( TID_GAME_NOTEQUALITEM );
		return 0;
	}
	
	//	���ü�ġ�� maxġ�� �Ѿ����� ����
	if( pItemMain->GetAbilityOption() >= GetMaxGeneralEnchantSize() )
	{
		//pUser->AddDefinedText( TID_UPGRADE_MAXOVER );
		return 3;
	}
	
	// 1000������ ���� �ۼ�Ʈ
	int nPercent = GetGeneralEnchantProb( pItemMain->GetAbilityOption() );
	
	//	������ �η縶���� ����ߴٸ�
	if( pItemSmeltScr != NULL )
	{
		//	������ �η縶���� �´��� Ȯ��
		if( IsUsableItem( pItemSmeltScr ) && pItemSmeltScr->GetProp()->dwID == II_SYS_SYS_SCR_SMELTING )
		{
			//	������ �η縶�� ��밡�� ��ġ�ΰ�
			if( pItemMain->GetAbilityOption() < 7 )
			{
				nPercent += 1000;
				ItemProp* pItemProp = pItemSmeltScr->GetProp();
				if( pItemProp)
				{
					g_dpDBClient.SendLogSMItemUse( "1", pUser, pItemSmeltScr, pItemProp );
					g_dpDBClient.SendLogSMItemUse( "2", pUser, NULL, pItemProp );
				}
				pUser->RemoveItem( (BYTE)( pItemSmeltScr->m_dwObjId ), 1 );
			}
		}
		//	������ �η縶���� �ƴҶ�
		else
			return 0;
	}

	LogItemInfo aLogItem;
	aLogItem.Action = "N";
	aLogItem.SendName = pUser->GetName();
	aLogItem.RecvName = "UPGRADEITEM_SMELTSAFETY";
	aLogItem.WorldId = pUser->GetWorld()->GetID();
	aLogItem.Gold = pUser->GetGold();
	aLogItem.Gold2 = pUser->GetGold();

	g_DPSrvr.OnLogItem( aLogItem, pItemMaterial, pItemMaterial->m_nItemNum );
	// ���þ��� ���� - �����̴�, ���д�...
	pUser->RemoveItem( (BYTE)( pItemMaterial->m_dwObjId ), 1 );
	//	��ȣ�� �η縶�� �α׳����
	ItemProp* pItemProp = pItemProtScr->GetProp();
	if( pItemProp )
	{
		g_dpDBClient.SendLogSMItemUse( "1", pUser, pItemProtScr, pItemProp );
		g_dpDBClient.SendLogSMItemUse( "2", pUser, NULL, pItemProp );
	}
	pUser->RemoveItem( (BYTE)( pItemProtScr->m_dwObjId ), 1 );

	// �ش� �������� �Ӽ�, �Ϲ� ������ ��� Ȯ���� ������.
	if( (int)( xRandom( 10000 ) ) > nPercent )
	{
		// ���� �޼��� ���
		//pUser->AddDefinedText( TID_UPGRADE_FAIL );
		pUser->AddPlaySound( SND_INF_UPGRADEFAIL );
		
		if((pUser->IsMode( TRANSPARENT_MODE ) ) == 0)
			g_UserMng.AddCreateSfxObj( (CMover *)pUser, XI_INT_FAIL, pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z );
		
		aLogItem.Action = "F";
		g_DPSrvr.OnLogItem( aLogItem, pItemMain, pItemMain->m_nItemNum );

		return 2;
	}
	else
	{
		// ����
		//pUser->AddDefinedText( TID_UPGRADE_SUCCEEFUL );
		pUser->AddPlaySound( SND_INF_UPGRADESUCCESS );
		
		if((pUser->IsMode( TRANSPARENT_MODE ) ) == 0)
			g_UserMng.AddCreateSfxObj( (CMover *)pUser, XI_INT_SUCCESS, pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z );
		
		pUser->UpdateItem( (BYTE)pItemMain->m_dwObjId, UI_AO,  pItemMain->GetAbilityOption() + 1 );
		aLogItem.Action = "H";
		g_DPSrvr.OnLogItem( aLogItem, pItemMain, pItemMain->m_nItemNum );

		return 1;
	}
	return 0;
}

BYTE	CItemUpgrade::SmeltSafetyAccessory(CUser* pUser, CItemElem* pItemMain, CItemElem* pItemMaterial, CItemElem* pItemProtScr )
{
	// ��ᰡ �������ΰ�?
	if( pItemMaterial->GetProp()->dwID != II_GEN_MAT_MOONSTONE && pItemMaterial->GetProp()->dwID != II_GEN_MAT_MOONSTONE_1 )
	{
		//pUser->AddDefinedText( TID_GAME_NOTEQUALITEM );
		return 0;
	}
	//	�Ǽ����� ��ȣ�� �η縶���� �ƴҰ�� ����
	if( pItemProtScr->GetProp()->dwID != II_SYS_SYS_SCR_SMELPROT4 )
	{
		//pUser->AddDefinedText( TID_GAME_NOTEQUALITEM );
		return 0;
	}
	//	���ü�ġ�� maxġ�� �Ѿ����� ����
	if( pItemMain->GetAbilityOption() >= MAX_AAO )	// 20
	{
		//pUser->AddDefinedText( TID_GAME_ACCESSORY_MAX_AAO );
		return 3;
	}

	// log
	LogItemInfo aLogItem;
	aLogItem.SendName	= pUser->GetName();
	aLogItem.RecvName	= "UPGRADEITEM_SMELTSAFETY";
	aLogItem.WorldId	= pUser->GetWorld()->GetID();
	aLogItem.Gold	= pUser->GetGold();
	aLogItem.Gold2	= pUser->GetGold();

	aLogItem.Action	= "N";
	g_DPSrvr.OnLogItem( aLogItem, pItemMaterial, pItemMaterial->m_nItemNum );
	pUser->RemoveItem( (BYTE)( pItemMaterial->m_dwObjId ), 1 );
	// �׼����� ��ȣ�� �η縶�� �α� �����
	ItemProp* pItemProp = pItemProtScr->GetProp();
	if( pItemProp )
	{
		g_dpDBClient.SendLogSMItemUse( "1", pUser, pItemProtScr, pItemProp );
		g_dpDBClient.SendLogSMItemUse( "2", pUser, NULL, pItemProp );
	}
	pUser->RemoveItem( (BYTE)( pItemProtScr->m_dwObjId ), 1 );

	// ���� Ȯ��
	DWORD dwProbability = CAccessoryProperty::GetInstance()->GetProbability( pItemMain->GetAbilityOption() );

	if( xRandom( 10000 ) > dwProbability )	// ����
	{
		pUser->AddPlaySound( SND_INF_UPGRADEFAIL );
		if( pUser->IsMode( TRANSPARENT_MODE ) == 0 )
			g_UserMng.AddCreateSfxObj( (CMover *)pUser, XI_INT_FAIL, pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z );

		aLogItem.Action	= "L";
		g_DPSrvr.OnLogItem( aLogItem, pItemMain, pItemMain->m_nItemNum );

		return 2;
	}
	else	// ����
	{
		pUser->AddPlaySound( SND_INF_UPGRADESUCCESS );
			
		if( pUser->IsMode( TRANSPARENT_MODE ) == 0)
			g_UserMng.AddCreateSfxObj( (CMover *)pUser, XI_INT_SUCCESS, pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z);

		pUser->UpdateItem( (BYTE)pItemMain->m_dwObjId, UI_AO,  pItemMain->GetAbilityOption()+1 );

		aLogItem.Action		= "H";
		g_DPSrvr.OnLogItem( aLogItem, pItemMain, pItemMain->m_nItemNum );

		return 1;
	}
	return 0;
}

BYTE	CItemUpgrade::SmeltSafetyPiercingSize(CUser* pUser, CItemElem* pItemMain, CItemElem* pItemMaterial, CItemElem* pItemProtScr )
{
	// ��ᰡ �������ΰ�?
	if( pItemMaterial->GetProp()->dwID != II_GEN_MAT_MOONSTONE && pItemMaterial->GetProp()->dwID != II_GEN_MAT_MOONSTONE_1 )
	{
		//pUser->AddDefinedText( TID_GAME_NOTEQUALITEM );
		return 0;
	}
	//	�Ǿ�� ��ȣ�� �η縶���ΰ�
	if( pItemProtScr->m_dwItemId != II_SYS_SYS_SCR_PIEPROT )
	{
		//pUser->AddDefinedText( TID_SBEVE_NOTUSEITEM );			// ���������� �ƴϸ� �Ұ���
		return 0;
	}

	LogItemInfo aLogItem;
	aLogItem.SendName = pUser->GetName();
	aLogItem.RecvName = "PIERCING_SMELTSAFETY";
	aLogItem.WorldId = pUser->GetWorld()->GetID();

	int nCost = 100000;

	if( 0 < nCost )
	{
		if( pUser->GetGold() < nCost )
		{
			pUser->AddDefinedText( TID_GAME_LACKMONEY );
			return 0;
		}

		pUser->AddGold( -( nCost ) );

		aLogItem.Gold = pUser->GetGold() + nCost;
		aLogItem.Gold2 = pUser->GetGold();
		aLogItem.Action = "!";
		//aLogItem.ItemName = "SEED";
		_stprintf( aLogItem.szItemName, "%d", II_GOLD_SEED1 );
		aLogItem.itemNumber = nCost;
		g_DPSrvr.OnLogItem( aLogItem );
	}
	else
	{
		return 0;
	}
	aLogItem.Gold = aLogItem.Gold2 = pUser->GetGold();

	// ���� �η縶�� ����.
	aLogItem.Action = "!";
	g_DPSrvr.OnLogItem( aLogItem, pItemMaterial, pItemMaterial->m_nItemNum );
	pUser->RemoveItem( (BYTE)( pItemMaterial->m_dwObjId ), 1 );
	//	�Ǿ�� ��ȣ�� �η縶�� �α� �����
	ItemProp* pItemProp = pItemProtScr->GetProp();
	if( pItemProp )
	{
		g_dpDBClient.SendLogSMItemUse( "1", pUser, pItemProtScr, pItemProp );
		g_dpDBClient.SendLogSMItemUse( "2", pUser, NULL, pItemProp );
	}
	pUser->RemoveItem( (BYTE)( pItemProtScr->m_dwObjId ), 1 );
	
	int nPercent = GetSizeProb( pItemMain );

	if( nPercent < (int)( xRandom( 10000 ) ) )
	{	// ����
		//pUser->AddDefinedText( TID_MMI_PIERCINGFAIL );
		pUser->AddPlaySound( SND_INF_UPGRADEFAIL );
		if( pUser->IsMode( TRANSPARENT_MODE ) == 0)
			g_UserMng.AddCreateSfxObj( (CMover *)pUser, XI_INT_FAIL, pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z);
		
		aLogItem.Action = "!";
		g_DPSrvr.OnLogItem( aLogItem, pItemMain, pItemMain->m_nItemNum );

		return 2;
	}
	else
	{	// ����
		//pUser->AddDefinedText( TID_MMI_PIERCINGSUCCESS );
		pUser->AddPlaySound( SND_INF_UPGRADESUCCESS );
		if( pUser->IsMode( TRANSPARENT_MODE ) == 0)
			g_UserMng.AddCreateSfxObj( (CMover *)pUser, XI_INT_SUCCESS, pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z );
		pUser->UpdateItem( (BYTE)pItemMain->m_dwObjId, UI_PIERCING_SIZE, pItemMain->GetPiercingSize() + 1 );

		aLogItem.Action = "#";
		g_DPSrvr.OnLogItem( aLogItem, pItemMain, pItemMain->m_nItemNum );

		return 1;
	}
	return 0;
}

void	CItemUpgrade::RefineAccessory( CUser* pUser, CItemElem* pItemMain, CItemElem* pItemMaterial )
{
	// ��ᰡ �������ΰ�?
	if( pItemMaterial->GetProp()->dwID != II_GEN_MAT_MOONSTONE && pItemMaterial->GetProp()->dwID != II_GEN_MAT_MOONSTONE_1 )
	{
		pUser->AddDefinedText( TID_GAME_NOTEQUALITEM );
		return;
	}
	if( pItemMain->GetAbilityOption() >= MAX_AAO )	// 20
	{
		pUser->AddDefinedText( TID_GAME_ACCESSORY_MAX_AAO );
		return;
	}

	// log
	LogItemInfo aLogItem;
	aLogItem.SendName	= pUser->GetName();
	aLogItem.RecvName	= "UPGRADEITEM";
	aLogItem.WorldId	= pUser->GetWorld()->GetID();
	aLogItem.Gold	= pUser->GetGold();
	aLogItem.Gold2	= pUser->GetGold();

	DWORD dwProbability		= CAccessoryProperty::GetInstance()->GetProbability( pItemMain->GetAbilityOption() );
	// �׼����� ��ȣ�� �η縶��
	BOOL bSmelprot	= FALSE;
	if( pUser->HasBuff( BUFF_ITEM, II_SYS_SYS_SCR_SMELPROT4 ) )
	{
		bSmelprot	= TRUE;
		pUser->RemoveBuff( BUFF_ITEM, II_SYS_SYS_SCR_SMELPROT4 );
		ItemProp* pItemProp = prj.GetItemProp( II_SYS_SYS_SCR_SMELPROT4 );
		if( pItemProp )
			g_dpDBClient.SendLogSMItemUse( "2", pUser, NULL, pItemProp );
	}

	if( xRandom( 10000 ) < dwProbability )	// ����
	{
		pUser->AddDefinedText( TID_UPGRADE_SUCCEEFUL );
		pUser->AddPlaySound( SND_INF_UPGRADESUCCESS );
			
		if( pUser->IsMode( TRANSPARENT_MODE ) == 0)
			g_UserMng.AddCreateSfxObj( (CMover *)pUser, XI_INT_SUCCESS, pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z);

		pUser->UpdateItem( (BYTE)pItemMain->m_dwObjId, UI_AO,  pItemMain->GetAbilityOption()+1 );

		aLogItem.Action		= "H";
		g_DPSrvr.OnLogItem( aLogItem, pItemMain, pItemMain->m_nItemNum );
	}
	else	// ����
	{
		pUser->AddDefinedText( TID_UPGRADE_FAIL );
		pUser->AddPlaySound( SND_INF_UPGRADEFAIL );
		if( pUser->IsMode( TRANSPARENT_MODE ) == 0 )
			g_UserMng.AddCreateSfxObj( (CMover *)pUser, XI_INT_FAIL, pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z );

		if( !bSmelprot )
		{
			if( pItemMain->GetAbilityOption() >= 3 )		// ����
			{
				aLogItem.Action	= "L";
				g_DPSrvr.OnLogItem( aLogItem, pItemMain, pItemMain->m_nItemNum );
				pUser->UpdateItem( (BYTE)pItemMain->m_dwObjId, UI_NUM, 0 );
			}
		}
	}
	aLogItem.Action	= "N";
	g_DPSrvr.OnLogItem( aLogItem, pItemMaterial, pItemMaterial->m_nItemNum );
	pUser->UpdateItem( (BYTE)pItemMaterial->m_dwObjId, UI_NUM, pItemMaterial->m_nItemNum - 1 );
}

void	CItemUpgrade::RefineCollector( CUser* pUser, CItemElem* pItemMain, CItemElem* pItemMaterial )
{
	// ��ᰡ �������ΰ�?
	if( pItemMaterial->GetProp()->dwID != II_GEN_MAT_MOONSTONE && pItemMaterial->GetProp()->dwID != II_GEN_MAT_MOONSTONE_1 )
	{
		pUser->AddDefinedText( TID_GAME_NOTEQUALITEM );
		return;
	}

	CCollectingProperty* pProperty	= CCollectingProperty::GetInstance();
	if( pItemMain->GetAbilityOption() >= pProperty->GetMaxCollectorLevel() )
	{
		pUser->AddDefinedText( TID_GAME_MAX_COLLECTOR_LEVEL );
		return;
	}
	
	int nProb	= pProperty->GetEnchantProbability( pItemMain->GetAbilityOption() );
	if( nProb == 0 )
		return;

	// log
	LogItemInfo aLogItem;
	aLogItem.SendName	= pUser->GetName();
	aLogItem.RecvName	= "UPGRADEITEM";
	aLogItem.WorldId	= pUser->GetWorld()->GetID();
	aLogItem.Gold	= pUser->GetGold();
	aLogItem.Gold2	= pUser->GetGold();

	DWORD dwRand	= xRandom( 1000 );	// 0 - 999
	if( (int)( dwRand ) < nProb )
	{
		pUser->AddDefinedText( TID_UPGRADE_SUCCEEFUL );
		pUser->AddPlaySound( SND_INF_UPGRADESUCCESS );
		if( pUser->IsMode( TRANSPARENT_MODE ) == 0 )
			g_UserMng.AddCreateSfxObj( (CMover *)pUser, XI_INT_SUCCESS, pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z );
		pUser->UpdateItem( (BYTE)pItemMain->m_dwObjId, UI_AO,  pItemMain->GetAbilityOption()+1 );
		aLogItem.Action		= "H";
		g_DPSrvr.OnLogItem( aLogItem, pItemMain, pItemMain->m_nItemNum );
	}
	else
	{
		pUser->AddDefinedText( TID_UPGRADE_FAIL );
		pUser->AddPlaySound( SND_INF_UPGRADEFAIL );
		if( pUser->IsMode( TRANSPARENT_MODE ) == 0 )
			g_UserMng.AddCreateSfxObj( (CMover *)pUser, XI_INT_FAIL, pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z );
		// log
	}
	aLogItem.Action	= "N";
	g_DPSrvr.OnLogItem( aLogItem, pItemMaterial, pItemMaterial->m_nItemNum );
	pUser->UpdateItem( (BYTE)pItemMaterial->m_dwObjId, UI_NUM, pItemMaterial->m_nItemNum - 1 );
}

#if __VER >= 15 // __15_5TH_ELEMENTAL_SMELT_SAFETY
BYTE	CItemUpgrade::SmeltSafetyAttribute(CUser* pUser, CItemElem* pItemMain, CItemElem* pItemMaterial, CItemElem* pItemProtScr, CItemElem* pItemSmeltScr )
{
	// �Ӽ� ������ �����Ѱ�
	if( !CItemElem::IsEleRefineryAble( pItemMain->GetProp() ) )
		return 0;

	// �Ӽ��� �Ѱ�����
	if( pItemMain->m_bItemResist != SAI79::NO_PROP )
	{
		if( pItemMain->m_bItemResist != pItemMaterial->GetProp()->eItemType )
			return 0;
	}

	// �Ӽ� �� �ϳ��� �Ӽ� ���� ī�带 ����ϵ��� ����
	if( pItemMaterial->GetProp()->dwID != WhatEleCard( pItemMaterial->GetProp()->eItemType ) )
		return 0;

	// �ִ� ���� �Ѵ� ��� �ߴ�
	if( pItemMain->m_nResistAbilityOption >= GetMaxAttributeEnchantSize() )
		return 3;
	
	// 10000������ ���� �ۼ�Ʈ
	int nPercent = GetAttributeEnchantProb( pItemMain->m_nResistAbilityOption );

	//	�Ӽ� ������ �η縶���� ����ߴٸ�
	if( pItemSmeltScr != NULL )
	{
		//	�Ӽ� ������ �η縶���� �´��� Ȯ��
		if( IsUsableItem( pItemSmeltScr ) && pItemSmeltScr->GetProp()->dwID == II_SYS_SYS_SCR_SMELTING2 )
		{
			//	�Ӽ� ������ �η縶�� ��밡�� ��ġ�ΰ�
			if( pItemMain->m_nResistAbilityOption < 10 )
			{
				nPercent	+= 1000;
				//	�Ӽ� ������ �η縶�� �α� �����
				ItemProp* pItemProp = pItemSmeltScr->GetProp();
				if( pItemProp )
				{
					g_dpDBClient.SendLogSMItemUse( "1", pUser, pItemSmeltScr, pItemProp );
					g_dpDBClient.SendLogSMItemUse( "2", pUser, NULL, pItemProp );
				}
				pUser->RemoveItem( (BYTE)( pItemSmeltScr->m_dwObjId ), 1 );
			}
		}
		//	�Ӽ� ������ �η縶���� �ƴҶ�
		else
			return 0;
	}

	LogItemInfo aLogItem;
	aLogItem.Action = "N";
	aLogItem.SendName = pUser->GetName();
	aLogItem.RecvName = "UPGRADEITEM_SMELTSAFETY";
	aLogItem.WorldId = pUser->GetWorld()->GetID();
	aLogItem.Gold = pUser->GetGold();
	aLogItem.Gold2 = pUser->GetGold();

	g_DPSrvr.OnLogItem( aLogItem, pItemMaterial, pItemMaterial->m_nItemNum );
	DWORD dwValue = pItemMaterial->GetProp()->eItemType;
	pUser->RemoveItem( (BYTE)( pItemMaterial->m_dwObjId ), (short)1 );

	ItemProp* pItemProp = pItemProtScr->GetProp();
	if( pItemProp )
	{
		g_dpDBClient.SendLogSMItemUse( "1", pUser, pItemProtScr, pItemProp );
		g_dpDBClient.SendLogSMItemUse( "2", pUser, NULL, pItemProp );
	}
	pUser->RemoveItem( (BYTE)( pItemProtScr->m_dwObjId ), 1 );

	// �ش� �������� �Ӽ�, �Ϲ� ������ ��� Ȯ���� ������.
	if( (int)( xRandom( 10000 ) ) > nPercent )
	{
		// ����
		pUser->AddPlaySound( SND_INF_UPGRADEFAIL );

		if( !pUser->IsMode( TRANSPARENT_MODE ) )
			g_UserMng.AddCreateSfxObj( (CMover *)pUser, XI_INT_FAIL, pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z );

		aLogItem.Action = "J";
		g_DPSrvr.OnLogItem( aLogItem, pItemMain, pItemMain->m_nItemNum );

		return 2;
	}
	else
	{
		// ����
		pUser->AddPlaySound( SND_INF_UPGRADESUCCESS );

		if( !pUser->IsMode( TRANSPARENT_MODE ) )
			g_UserMng.AddCreateSfxObj( (CMover *)pUser, XI_INT_SUCCESS, pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z );

		pUser->UpdateItem( (BYTE)pItemMain->m_dwObjId, UI_IR, dwValue );
		pUser->UpdateItem( (BYTE)pItemMain->m_dwObjId, UI_RAO,  pItemMain->m_nResistAbilityOption + 1 );
		aLogItem.Action = "O";
		g_DPSrvr.OnLogItem( aLogItem, pItemMain, pItemMain->m_nItemNum );

		return 1;
	}
	return 0;
}
#endif // __15_5TH_ELEMENTAL_SMELT_SAFETY
#endif // __SMELT_SAFETY

void	CItemUpgrade::EnchantGeneral( CUser* pUser, CItemElem* pItemMain, CItemElem* pItemMaterial )
{	
	int*  pAbilityOption = pItemMain->GetAbilityOptionPtr();
	if( pAbilityOption == NULL )
		return;

	// �Ϲ������� ��, ����
#if __VER >= 9 // __ULTIMATE
	if( pItemMain->GetProp()->dwReferStat1 == WEAPON_ULTIMATE )
	{
		pUser->AddDefinedText( TID_GAME_NOTEQUALITEM );
		return;
	}

	if( pItemMaterial->GetProp()->dwID != II_GEN_MAT_ORICHALCUM01
		&& pItemMaterial->GetProp()->dwID != II_GEN_MAT_ORICHALCUM01_1 )
	{
		pUser->AddDefinedText( TID_GAME_NOTEQUALITEM );
		return;
	}
#endif // __ULTIMATE

	if( !CItemElem::IsDiceRefineryAble(pItemMain->GetProp()) )
	{
		pUser->AddDefinedText( TID_GAME_NOTEQUALITEM );			
		return;								
	}

	if( *pAbilityOption >= GetMaxGeneralEnchantSize() )
	{
		pUser->AddDefinedText( TID_UPGRADE_MAXOVER );			
		return;
	}

	// 1000������ ���� �ۼ�Ʈ 
	int nPercent = GetGeneralEnchantProb( *pAbilityOption );

	LogItemInfo aLogItem;
	aLogItem.SendName = pUser->GetName();
	aLogItem.RecvName = "UPGRADEITEM";
	aLogItem.WorldId = pUser->GetWorld()->GetID();
	aLogItem.Gold = pUser->GetGold();
	aLogItem.Gold2 = pUser->GetGold();

	BOOL bSmelprot	= FALSE;
	if( pUser->HasBuff( BUFF_ITEM, II_SYS_SYS_SCR_SMELPROT ) )
	{
		bSmelprot	= TRUE;
		pUser->RemoveBuff( BUFF_ITEM, II_SYS_SYS_SCR_SMELPROT );

		ItemProp* pItemProp = prj.GetItemProp( II_SYS_SYS_SCR_SMELPROT );
		if( pItemProp )
			g_dpDBClient.SendLogSMItemUse( "2", pUser, NULL, pItemProp );
	}

	if( pUser->HasBuff( BUFF_ITEM, II_SYS_SYS_SCR_SMELTING ) )
	{
		if( *pAbilityOption < 7 )
		{
			nPercent	+= 1000;
			pUser->RemoveBuff( BUFF_ITEM, II_SYS_SYS_SCR_SMELTING );

			ItemProp* pItemProp = prj.GetItemProp( II_SYS_SYS_SCR_SMELTING );
			if( pItemProp )
				g_dpDBClient.SendLogSMItemUse( "2", pUser, NULL, pItemProp );
		}
	}
#ifdef __SM_ITEM_2ND_EX
	BOOL bSmelprot2	= FALSE;
	if( !bSmelprot && pUser->HasBuff( BUFF_ITEM, II_SYS_SYS_SCR_SMELPROT2 ) )
	{
		bSmelprot2	= TRUE;
		pUser->RemoveBuff( BUFF_ITEM, II_SYS_SYS_SCR_SMELPROT2 );
	}
#endif	// __SM_ITEM_2ND_EX

	if( pUser->HasBuff( BUFF_ITEM, II_SYS_SYS_SCR_SUPERSMELTING ) )
	{
		if( *pAbilityOption < 7 )
		{
			nPercent	+= 10000;
			pUser->RemoveBuff( BUFF_ITEM, II_SYS_SYS_SCR_SUPERSMELTING );

			ItemProp* pItemProp = prj.GetItemProp( II_SYS_SYS_SCR_SUPERSMELTING );
			if( pItemProp )
				g_dpDBClient.SendLogSMItemUse( "2", pUser, NULL, pItemProp );
		}				
	}

	// �ش� �������� �Ӽ�, �Ϲ� ������ ��� Ȯ���� ������.
	if( (int)( xRandom( 10000 ) ) > nPercent )
	{
		// ���� �޼��� ���
		pUser->AddDefinedText( TID_UPGRADE_FAIL );
		pUser->AddPlaySound( SND_INF_UPGRADEFAIL );

		if((pUser->IsMode( TRANSPARENT_MODE ) ) == 0)
			g_UserMng.AddCreateSfxObj((CMover *)pUser, XI_INT_FAIL, pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z);

		// �����ϸ� 3�̻��̸� ������ ����
		if( *pAbilityOption >= 3 )
		{
			if( !bSmelprot )
			{	// �����ϸ� ����.. ��� ������ ���� ���� �α�
#ifdef __SM_ITEM_2ND_EX
				if( bSmelprot2 )
				{
					pUser->UpdateItem( (BYTE)pItemMain->m_dwObjId, UI_AO,  pItemMain->GetAbilityOption() - 1 );
					aLogItem.Action = "9";
					g_DPSrvr.OnLogItem( aLogItem, pItemMain, pItemMain->m_nItemNum );
				}
				else
#endif	// __SM_ITEM_2ND_EX
				{
					aLogItem.Action = "L";
					g_DPSrvr.OnLogItem( aLogItem, pItemMain, pItemMain->m_nItemNum );
					pUser->RemoveItem( (BYTE)( pItemMain->m_dwObjId ), (short)1 );
				}
			}
			else
			{
				aLogItem.Action = "F";
				g_DPSrvr.OnLogItem( aLogItem, pItemMain, pItemMain->m_nItemNum );
			}
		}
	}
	else
	{
		// ����
		pUser->AddDefinedText( TID_UPGRADE_SUCCEEFUL );
		pUser->AddPlaySound( SND_INF_UPGRADESUCCESS );

		if((pUser->IsMode( TRANSPARENT_MODE ) ) == 0)
			g_UserMng.AddCreateSfxObj((CMover *)pUser, XI_INT_SUCCESS, pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z);

		pUser->UpdateItem( (BYTE)pItemMain->m_dwObjId, UI_AO,  pItemMain->GetAbilityOption()+1 );
		aLogItem.Action = "H";
		g_DPSrvr.OnLogItem( aLogItem, pItemMain, pItemMain->m_nItemNum );
	}

	aLogItem.Action = "N";
	g_DPSrvr.OnLogItem( aLogItem, pItemMaterial, pItemMaterial->m_nItemNum );
	// ���þ��� ���� - �����̴�, ���д�...
	pUser->RemoveItem( (BYTE)( pItemMaterial->m_dwObjId ), (short)1 );
}

int		CItemUpgrade::GetGeneralEnchantProb( int nAbilityOption )
{
	int nProb = 0;
	
	map<int, int>::iterator it = m_mapGeneralEnchant.find( nAbilityOption + 1 );
	if( it != m_mapGeneralEnchant.end() )
		nProb = it->second;

	if( ::GetLanguage() != LANG_KOR && nAbilityOption >= 3 )	// ���� 4���� 10% Ȯ�� ����(�ؿܸ�)
		nProb = static_cast<int>(static_cast<float>(nProb) * 0.9f);
	
	return nProb;
}

void	CItemUpgrade::EnchantAttribute( CUser* pUser, CItemElem* pItemMain, CItemElem* pItemMaterial )
{
	int*  pAbilityOption = &(pItemMain->m_nResistAbilityOption);
	if( pAbilityOption == NULL )
		return;

	// 2���� �Ӽ��� �����Ҽ� ����
	if( pItemMain->m_bItemResist != SAI79::NO_PROP )
	{
		if( pItemMain->m_bItemResist != pItemMaterial->GetProp()->eItemType )
		{
			pUser->AddDefinedText( TID_UPGRADE_ERROR_TWOELEMENT );								
			return;
		}
	}

	if( !CItemElem::IsEleRefineryAble(pItemMain->GetProp()) )
	{
		pUser->AddDefinedText( TID_GAME_NOTEQUALITEM );			
		return;								
	}

#if __VER >= 12 // __J12_0
	// �Ӽ� �� �ϳ��� �Ӽ� ���� ī�带 ����ϵ��� ����
	DWORD dwReqCard	= WhatEleCard( pItemMaterial->GetProp()->eItemType );
#else	// __J12_0
	DWORD dwReqCard = WhatEleCard( *pAbilityOption, pItemMaterial->GetProp()->eItemType );
#endif	// __J12_0

	if( pItemMaterial->GetProp()->dwID != dwReqCard )
	{
		pUser->AddDefinedText( TID_UPGRADE_ERROR_WRONGUPLEVEL );			
		return;					
	}

	// �ִ� ���� �Ѵ� ��� �ߴ�
	if( *pAbilityOption >= GetMaxAttributeEnchantSize() )
	{
		pUser->AddDefinedText( TID_UPGRADE_MAXOVER );			
		return;
	}
	// 10000������ ���� �ۼ�Ʈ 
	int nPercent = GetAttributeEnchantProb( *pAbilityOption );

	LogItemInfo aLogItem;
	aLogItem.SendName = pUser->GetName();
	aLogItem.RecvName = "UPGRADEITEM";
	aLogItem.WorldId = pUser->GetWorld()->GetID();
	aLogItem.Gold = pUser->GetGold();
	aLogItem.Gold2 = pUser->GetGold();


	BOOL bSmelprot	= FALSE;
	if( pUser->HasBuff( BUFF_ITEM, II_SYS_SYS_SCR_SMELPROT ) )
	{
		bSmelprot	= TRUE;
		pUser->RemoveBuff( BUFF_ITEM, II_SYS_SYS_SCR_SMELPROT );

		ItemProp* pItemProp = prj.GetItemProp( II_SYS_SYS_SCR_SMELPROT );
		if( pItemProp )
			g_dpDBClient.SendLogSMItemUse( "2", pUser, NULL, pItemProp );
	}
	
#if __VER >= 14 // __EXT_ATTRIBUTE
	if( pUser->HasBuff( BUFF_ITEM, II_SYS_SYS_SCR_SMELTING2 ) )	// �Ӽ� ������ �η縶��
#else // __EXT_ATTRIBUTE
	if( pUser->HasBuff( BUFF_ITEM, II_SYS_SYS_SCR_SMELTING ) ) // ������ �η縶��
#endif // __EXT_ATTRIBUTE
	{
#if __VER >= 14 // __EXT_ATTRIBUTE
		if( *pAbilityOption < 10 )
#else // __EXT_ATTRIBUTE
		if( *pAbilityOption < 7 )
#endif // __EXT_ATTRIBUTE
		{
			nPercent	+= 1000;
#if __VER >= 14 // __EXT_ATTRIBUTE
			pUser->RemoveBuff( BUFF_ITEM, II_SYS_SYS_SCR_SMELTING2 );
			ItemProp* pItemProp = prj.GetItemProp( II_SYS_SYS_SCR_SMELTING2 );
#else // __EXT_ATTRIBUTE
			pUser->RemoveBuff( BUFF_ITEM, II_SYS_SYS_SCR_SMELTING );
			ItemProp* pItemProp = prj.GetItemProp( II_SYS_SYS_SCR_SMELTING );
#endif // __EXT_ATTRIBUTE
			if( pItemProp )
				g_dpDBClient.SendLogSMItemUse( "2", pUser, NULL, pItemProp );
		}
	}
#ifdef __SM_ITEM_2ND_EX
	BOOL bSmelprot2	= FALSE;
	if( !bSmelprot && pUser->HasBuff( BUFF_ITEM, II_SYS_SYS_SCR_SMELPROT2 ) )
	{
		bSmelprot2	= TRUE;
		pUser->RemoveBuff( BUFF_ITEM, II_SYS_SYS_SCR_SMELPROT2 );
	}
#endif	// __SM_ITEM_2ND_EX

	if( pUser->HasBuff( BUFF_ITEM, II_SYS_SYS_SCR_SUPERSMELTING ) )
	{
		if( *pAbilityOption < 7 )
		{
			nPercent	+= 10000;
			pUser->RemoveBuff( BUFF_ITEM, II_SYS_SYS_SCR_SUPERSMELTING );

			ItemProp* pItemProp = prj.GetItemProp( II_SYS_SYS_SCR_SUPERSMELTING );
			if( pItemProp )
				g_dpDBClient.SendLogSMItemUse( "2", pUser, NULL, pItemProp );
		}				
	}

	// �ش� �������� �Ӽ�, �Ϲ� ������ ��� Ȯ���� ������.
	if( (int)( xRandom( 10000 ) ) > nPercent )
	{
		// ���� �޼��� ���
		pUser->AddDefinedText( TID_UPGRADE_FAIL );
		pUser->AddPlaySound( SND_INF_UPGRADEFAIL );

		if((pUser->IsMode( TRANSPARENT_MODE ) ) == 0)
			g_UserMng.AddCreateSfxObj((CMover *)pUser, XI_INT_FAIL, pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z);

		// �����ϸ� nLevDown�̻��̸� ������ ����
		if( *pAbilityOption >= 3 )
		{
			if( !bSmelprot )
			{	// �����ϸ� ����.. ��� ������ ���� ���� �α�
#ifdef __SM_ITEM_2ND_EX
				if( bSmelprot2  )
				{
					pUser->UpdateItem( (BYTE)pItemMain->m_dwObjId, UI_RAO, pItemMain->m_nResistAbilityOption - 1 );
					aLogItem.Action = "8";
					g_DPSrvr.OnLogItem( aLogItem, pItemMain, pItemMain->m_nItemNum );
				}
				else
#endif	// __SM_ITEM_2ND_EX
				{
					aLogItem.Action = "L";
					g_DPSrvr.OnLogItem( aLogItem, pItemMain, pItemMain->m_nItemNum );
					pUser->RemoveItem( (BYTE)( pItemMain->m_dwObjId ), (short)1 );
				}
			}
		}
		else
		{	// ����� �ϸ� ���� �α�
			aLogItem.Action = "J";
			g_DPSrvr.OnLogItem( aLogItem, pItemMain, pItemMain->m_nItemNum );
		}
	}
	else
	{
		// ����
		pUser->AddDefinedText( TID_UPGRADE_SUCCEEFUL );
		pUser->AddPlaySound( SND_INF_UPGRADESUCCESS );

		if((pUser->IsMode( TRANSPARENT_MODE ) ) == 0)
			g_UserMng.AddCreateSfxObj((CMover *)pUser, XI_INT_SUCCESS, pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z);

		pUser->UpdateItem( (BYTE)pItemMain->m_dwObjId, UI_IR,  pItemMaterial->GetProp()->eItemType );
		pUser->UpdateItem( (BYTE)pItemMain->m_dwObjId, UI_RAO,  pItemMain->m_nResistAbilityOption+1 );
		aLogItem.Action = "O";
		g_DPSrvr.OnLogItem( aLogItem, pItemMain, pItemMain->m_nItemNum );
	}

	aLogItem.Action = "N";
	g_DPSrvr.OnLogItem( aLogItem, pItemMaterial, pItemMaterial->m_nItemNum );
	// ���þ��� ���� - �����̴�, ���д�...
	pUser->RemoveItem( (BYTE)( pItemMaterial->m_dwObjId ), (short)1 );
}

void CItemUpgrade::ChangeAttribute( CUser* pUser, OBJID dwTargetItem, OBJID dwUseItem , SAI79::ePropType nAttribute )
{
	CItemElem* pTargetItemElem	= pUser->m_Inventory.GetAtId( dwTargetItem );
	CItemElem* pUseItemElem = pUser->m_Inventory.GetAtId( dwUseItem );		

	if( pUser->m_vtInfo.GetOther() )	// �ŷ����� ����� ������?
		return;
	if( pUser->m_vtInfo.VendorIsVendor() )		// ���� �Ȱ� ������?
		return;

#if __VER >= 11 // __SYS_COLLECTING
#if __VER < 14 // __SMELT_SAFETY
	if( pUser->PreRefine( dwTargetItem, dwUseItem ) )
		return;
#endif // __SMELT_SAFETY
#endif	// __SYS_COLLECTING
	if( !IsUsableItem( pTargetItemElem ) || !IsUsableItem( pUseItemElem ) )
		return;

	// ����� �������ΰ�?
	if( pUser->m_Inventory.IsEquip( dwTargetItem ) )
	{
		pUser->AddDefinedText( TID_GAME_EQUIPPUT , "" );
		return;
	}
	
	if( !CItemElem::IsEleRefineryAble( pTargetItemElem->GetProp() ) )	// �Ӽ����� ������ �������� �ƴϿ�..
		return;

	if( pUseItemElem->m_dwItemId != II_SYS_SYS_SCR_SOKCHANG )	// �Ӽ����� �������� �ƴϳ�...
		return;

	if( nAttribute >= SAI79::END_PROP || nAttribute <= SAI79::NO_PROP )
		return;

	if( pTargetItemElem->m_bItemResist == nAttribute )	// ���� �Ӽ��� ��� �Ӽ����� �Ұ�!!
	{
		pUser->AddDefinedText( TID_GAME_NOCHANGE_SAME_ATTRIBUTE );
		return;
	}
	
	if( (pTargetItemElem->m_bItemResist != SAI79::NO_PROP) && (pTargetItemElem->m_nResistAbilityOption > 0) )
	{
		pUser->AddPlaySound( SND_INF_UPGRADESUCCESS );
		if( ( pUser->IsMode( TRANSPARENT_MODE ) ) == 0 )
			g_UserMng.AddCreateSfxObj((CMover *)pUser, XI_INT_SUCCESS, pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z);
		pUser->UpdateItem( (BYTE)pTargetItemElem->m_dwObjId, UI_IR,  nAttribute );	// �Ӽ� ����

		// �Ӽ����� ���� ���� �α�
		LogItemInfo aLogItem;
		aLogItem.SendName = pUser->GetName();
		aLogItem.RecvName = "CHANGE_ATTRIBUTE_TARGET";
		aLogItem.WorldId = pUser->GetWorld()->GetID();
		aLogItem.Gold = pUser->GetGold();
		aLogItem.Gold2 = pUser->GetGold();
		aLogItem.Action = "O";
		g_DPSrvr.OnLogItem( aLogItem, pTargetItemElem );
		
		aLogItem.RecvName = "CHANGE_ATTRIBUTE_MATERIAL";
		g_DPSrvr.OnLogItem( aLogItem, pUseItemElem );
		pUser->RemoveItem( (BYTE)( dwUseItem ), 1 );	// ���� ������ ����
	}
	else
		pUser->AddDefinedText( TID_GAME_NOTELEMENT );
}

int CItemUpgrade::GetAttributeEnchantProb( int nAbilityOption )
{
	map<int, __ATTRIBUTE_ENCHANT>::iterator it = m_mapAttributeEnchant.find( nAbilityOption + 1 );
	if( it != m_mapAttributeEnchant.end() )
		return it->second.nProb;

	return 0;
}

int CItemUpgrade::GetAttributeDamageFactor( int nAbilityOption )
{
	if( nAbilityOption > GetMaxAttributeEnchantSize() )
		nAbilityOption = GetMaxAttributeEnchantSize();

	map<int, __ATTRIBUTE_ENCHANT>::iterator it = m_mapAttributeEnchant.find( nAbilityOption );
	if( it != m_mapAttributeEnchant.end() )
		return it->second.nAddDamageRate;

	return 0;
}

#if __VER >= 14 // __EXT_ATTRIBUTE
int CItemUpgrade::GetAttributeDefenseFactor( int nAbilityOption )
{
	if( nAbilityOption > GetMaxAttributeEnchantSize() )
		nAbilityOption = GetMaxAttributeEnchantSize();

	map<int, __ATTRIBUTE_ENCHANT>::iterator it = m_mapAttributeEnchant.find( nAbilityOption );
	if( it != m_mapAttributeEnchant.end() )
		return it->second.nDefenseRate;

	return 0;
}

int CItemUpgrade::GetAttributeAddAtkDmgFactor( int nAbilityOption )
{
	if( nAbilityOption > GetMaxAttributeEnchantSize() )
		nAbilityOption = GetMaxAttributeEnchantSize();

	map<int, __ATTRIBUTE_ENCHANT>::iterator it = m_mapAttributeEnchant.find( nAbilityOption );
	if( it != m_mapAttributeEnchant.end() )
		return it->second.nAddAtkDmgRate;

	return 0;
}
#endif // __EXT_ATTRIBUTE

DWORD CItemUpgrade::WhatEleCard( DWORD dwItemType )
{	// �Ӽ� ���� �� ī���� ������ 
	// �Ӽ� �� �ϳ��� ���յ�
	switch( dwItemType )
	{
	case SAI79::FIRE:
		return II_GEN_MAT_ELE_FLAME;
	case SAI79::WATER:
		return II_GEN_MAT_ELE_RIVER;
	case SAI79::ELECTRICITY:
		return II_GEN_MAT_ELE_GENERATOR;
	case SAI79::EARTH:
		return II_GEN_MAT_ELE_DESERT;
	case SAI79::WIND:
		return II_GEN_MAT_ELE_CYCLON;
	default:
		return 0;
	}
}
#endif // __EXT_ENCHANT

#ifdef __SYS_ITEMTRANSY
void CItemUpgrade::OnItemTransy( CUser* pUser, OBJID objidTarget, OBJID objidTransy, DWORD dwChangeId, BOOL bCash )
{
	CItemElem* pItemElemTarget = pUser->m_Inventory.GetAtId( objidTarget );	
	if( !IsUsableItem( pItemElemTarget ) )
		return;

	// �����Ǿ� �ִ� �������̸� ����( ������~~~ ���� )
	if( pUser->m_Inventory.IsEquip( objidTarget ) )
	{
		pUser->AddDefinedText( TID_GAME_EQUIPPUT , "" );
		return;
	}

	if( bCash )
	{
		CItemElem* pItemElemTransy = pUser->m_Inventory.GetAtId( objidTransy );
		if( !IsUsableItem( pItemElemTransy ) )
			return;

		// ��ᰡ Ʈ����(ITM)���� �˻�
		if( pItemElemTransy->GetProp()->dwID != II_CHR_SYS_SCR_ITEMTRANSY_A && pItemElemTransy->GetProp()->dwID != II_CHR_SYS_SCR_ITEMTRANSY_B )
			return;
		
		// ���� �˻�
		if( pItemElemTransy->GetProp()->dwID == II_CHR_SYS_SCR_ITEMTRANSY_A )
		{
			if( pItemElemTarget->GetProp()->dwLimitLevel1 > 60 )
				return;
		}
		else
		{
			if( pItemElemTarget->GetProp()->dwLimitLevel1 < 61 )
				return;
		}

		if( RunItemTransy( pUser, pItemElemTarget, dwChangeId ) )
		{
			g_dpDBClient.SendLogSMItemUse( "1", pUser, pItemElemTransy, pItemElemTransy->GetProp(), "RemoveItem" );	
			pUser->RemoveItem( (BYTE)( objidTransy ), (short)1 );		// ���� ������ ��� ����
		}
	}
	else
	{
		//	mulcom	BEGIN100312	��ķ� ������ Ʈ���� ����.
		#ifdef	__ITEMTRANSY_PENYA
			int nPayPenya = 0;
			if( pItemElemTarget->GetProp()->dwLimitLevel1 < 61 )
				nPayPenya = m_nItemTransyLowLevel;
			else
				nPayPenya = m_nItemTransyHighLevel;

			if( pUser->GetGold() < nPayPenya || objidTransy != NULL_ID )
			{
				pUser->AddDefinedText( TID_GAME_LACKMONEY );
				return;
			}

			if( RunItemTransy( pUser, pItemElemTarget, dwChangeId ) )
			{
				pUser->AddGold( -nPayPenya );
				g_DPSrvr.PutPenyaLog( pUser, "O", "TRANSYITEM_PAY", nPayPenya );
			}
		#endif
		//	mulcom	END100312	��ķ� ������ Ʈ���� ����.
	}
}

BOOL CItemUpgrade::RunItemTransy( CUser* pUser, CItemElem* pItemElemTarget, DWORD dwChangeId )
{
	ItemProp* pItemProp = pItemElemTarget->GetProp();
	ItemProp* pItemPropChange = prj.GetItemProp( dwChangeId );
	
	// ����� �������� ������ �´��� �˻�.
	if( !pItemProp || !pItemPropChange || pItemProp->dwID == pItemPropChange->dwID 
		|| ( pItemProp->dwItemKind2 != IK2_ARMOR && pItemProp->dwItemKind2 != IK2_ARMORETC )
		|| ( pItemProp->dwItemSex != SEX_MALE && pItemProp->dwItemSex != SEX_FEMALE )
		|| ( pItemPropChange->dwItemKind2 != IK2_ARMOR && pItemPropChange->dwItemKind2 != IK2_ARMORETC )
		|| ( pItemPropChange->dwItemSex != SEX_MALE && pItemPropChange->dwItemSex != SEX_FEMALE )
		|| pItemProp->dwItemSex == pItemPropChange->dwItemSex
		|| pItemProp->dwItemKind1 != pItemPropChange->dwItemKind1 || pItemProp->dwItemKind2 != pItemPropChange->dwItemKind2
		|| pItemProp->dwItemKind3 != pItemPropChange->dwItemKind3 || pItemProp->dwItemJob != pItemPropChange->dwItemJob 
		|| pItemProp->dwHanded != pItemPropChange->dwHanded || pItemProp->dwParts != pItemPropChange->dwParts  
		|| pItemProp->dwItemLV != pItemPropChange->dwItemLV || pItemProp->dwAbilityMin != pItemPropChange->dwAbilityMin
		|| pItemProp->dwAbilityMax != pItemPropChange->dwAbilityMax || pItemProp->fAttackSpeed != pItemPropChange->fAttackSpeed
		)
		return FALSE;


	// ������ Ʈ���� ����
	pUser->AddPlaySound( SND_INF_UPGRADESUCCESS );			
	g_UserMng.AddCreateSfxObj((CMover *)pUser, XI_INT_SUCCESS, pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z);			

	// ���� �������� Elem ������ ���� �ϰ� ����...
	CItemElem ItemElemSend;
	ItemElemSend = *pItemElemTarget;
	ItemElemSend.m_dwItemId = pItemPropChange->dwID;
	ItemElemSend.m_nHitPoint	= pItemPropChange->dwEndurance;		// ������ 100%

	g_dpDBClient.SendLogSMItemUse( "1", pUser, pItemElemTarget, pItemElemTarget->GetProp(), "RemoveItem" );	
	g_dpDBClient.SendLogSMItemUse( "1", pUser, &ItemElemSend, ItemElemSend.GetProp(), "CreateItem" );	
	pUser->AddDefinedText( TID_GAME_ITEM_TRANSY_SUCCESS, "\"%s\" \"%s\"", pItemElemTarget->GetProp()->szName, ItemElemSend.GetProp()->szName );

	// ���� ������ ��� ����
	pUser->RemoveItem( (BYTE)( pItemElemTarget->m_dwObjId ), (short)1 );
	
	// ���ο� ������ ����
	pUser->CreateItem( &ItemElemSend );
	
	return TRUE;
}
#endif // __SYS_ITEMTRANSY

#if __VER >= 15 // __PETVIS
void CItemUpgrade::PetVisSize( CUser* pUser, OBJID objIdMaterial )
{
	if( !IsValidObj( pUser ) )
		return;

	CItemElem* pItemElemPet = pUser->GetVisPetItem();
	CItemElem* pItemElemMaterial = pUser->m_Inventory.GetAtId( objIdMaterial );

	if( !IsUsableItem( pItemElemPet ) || !pItemElemPet->IsVisPet() )
	{
		pUser->AddDefinedText( TID_GAME_BUFFPET_NOSUMMON02 );	
		return;
	}

	if( !IsUsableItem( pItemElemMaterial ) )
		return;

	if( pItemElemMaterial->m_dwItemId != II_SYS_SYS_VIS_KEY01 )
		return;


	if( !pItemElemPet->IsPierceAble( NULL_ID, TRUE ) ) // �� ���� Ȯ�� ���� �˻�
	{
		pUser->AddDefinedText( TID_GAME_BUFFPET_EXPANSION );
		return;
	}
		
	g_DPSrvr.PutItemLog( pUser, "!", "VIS_SLOT_MATERIAL", pItemElemMaterial );
	if( pItemElemMaterial->m_bCharged )
		g_dpDBClient.SendLogSMItemUse( "1", pUser, pItemElemMaterial, pItemElemMaterial->GetProp() );	
	pUser->UpdateItem( (BYTE)( pItemElemPet->m_dwObjId ), UI_PETVIS_SIZE, pItemElemPet->GetPiercingSize() + 1 );
	g_DPSrvr.PutItemLog( pUser, "#", "VIS_SLOT_SIZE", pItemElemPet );
	pUser->RemoveItem( (BYTE)( objIdMaterial ), 1 );
}

void CItemUpgrade::SetPetVisItem( CUser* pUser, OBJID objIdVis )
{
	if( !IsValidObj( pUser ) )
		return;

	CItemElem* pItemElemPet = pUser->GetVisPetItem();
	CItemElem* pItemElemVis = pUser->m_Inventory.GetAtId( objIdVis );

	if( !IsUsableItem( pItemElemPet ) )
	{
		pUser->AddDefinedText( TID_GAME_BUFFPET_NOSUMMON01 );
		return;
	}
		
	if( !IsUsableItem( pItemElemVis ) )
		return;

	ItemProp* pVisProp = prj.GetItemProp( pItemElemVis->m_dwItemId );
	if( !pVisProp )
		return;

	if( !pItemElemPet->IsPierceAble( pVisProp->dwItemKind3 ) )	// �� ���� ���� �˻�.
		return;

	int nFirstEmptySlot = NULL_ID;
	for( int i=0; i<pItemElemPet->GetPiercingSize(); i++ )
	{
		DWORD dwVisId = pItemElemPet->GetPiercingItem( i );
		if( dwVisId == pVisProp->dwID )	// �̹� ���� ������ �񽺰� �����Ǿ� �ִ�.
		{
			pUser->AddDefinedText( TID_GAME_BUFFPET_EQUIPVIS );
			return;
		}

		if( nFirstEmptySlot == NULL_ID && dwVisId == 0 )
			nFirstEmptySlot = i;
	}

	if( nFirstEmptySlot == NULL_ID )	// ����ִ� �� ������ ����.
	{
		pUser->AddDefinedText( TID_GAME_BUFFPET_LACKSLOT );
		return;
	}

	if( pUser->IsSatisfyNeedVis( pItemElemPet, pVisProp ) != SUCCSESS_NEEDVIS )
	{
		pUser->AddDefinedText( TID_GAME_BUFFPET_REQVIS );
		return;
	}

	
	pUser->ResetPetVisDST( pItemElemPet );
	pUser->UpdateItem( (BYTE)( pItemElemPet->m_dwObjId ), UI_PETVIS_ITEM, MAKELONG( nFirstEmptySlot, pItemElemVis->m_dwItemId ), pVisProp->dwAbilityMin );
	PutPetVisItemLog( pUser, "!", "VIS_MATERIAL", pItemElemPet, nFirstEmptySlot );
	if( pItemElemVis->m_bCharged )		// ���ȭ ������ �α�
		g_dpDBClient.SendLogSMItemUse( "1", pUser, pItemElemVis, pVisProp );		
	g_DPSrvr.PutItemLog( pUser, "$", "VIS_PIERCING", pItemElemPet );
	pUser->RemoveItem( (BYTE)( objIdVis ), 1 );
	pUser->SetPetVisDST( pItemElemPet );
	ChangeVisPetSfx( pUser, pItemElemPet );
}

void CItemUpgrade::RemovePetVisItem( CUser* pUser, int nPosition, BOOL bExpired )
{
	if( !IsValidObj( pUser ) )
		return;

	CItemElem* pItemElemPet = pUser->GetVisPetItem();
	if( !IsUsableItem( pItemElemPet ) )
		return;

	DWORD dwItemId = pItemElemPet->GetPiercingItem( nPosition );
	if( dwItemId  == 0 )	// �̹� ����ִ� ����
		return;

	pUser->ResetPetVisDST( pItemElemPet );
	if( bExpired )
		PutPetVisItemLog( pUser, "$", "VIS_REMOVE_EXPIRED", pItemElemPet, nPosition );
	else
		PutPetVisItemLog( pUser, "$", "VIS_REMOVE_BYUSER", pItemElemPet, nPosition );
	pUser->UpdateItem( (BYTE)( pItemElemPet->m_dwObjId ), UI_PETVIS_ITEM, MAKELONG( nPosition, 0 ), 0 ); // �ش� ������ ����.
	ItemProp* pItemProp = prj.GetItemProp( dwItemId );
	if( pItemProp )
		pUser->AddDefinedText( TID_GAME_BUFFPET_REMOVEVIS, "\"%s\"", pItemProp->szName );
	pUser->SetPetVisDST( pItemElemPet );
	ChangeVisPetSfx( pUser, pItemElemPet );
}

void CItemUpgrade::PutPetVisItemLog( CUser* pUser, const char* szAction, const char* szContext, CItemElem* pItem, int nPosition )
{	// �������� ���ŵ� ���Ŀ� ȣ����� �ʵ��� �����ؾ� ��
	LogItemInfo	log;
	log.Action	=  szAction;
	log.SendName	= pUser->GetName();
	log.RecvName	= szContext;
	log.WorldId		= pUser->GetWorld() ? pUser->GetWorld()->GetID() : WI_WORLD_NONE;	// chipi_090623 ���� - ù ���ӽ� ����� ������ ��� ���尡 ���� ���·� ���´�. 
	log.Gold	= pUser->GetGold();
	log.Gold2	= pItem->GetVisKeepTime( nPosition ) - time_null();
	g_DPSrvr.OnLogItem( log, pItem, pItem->m_nItemNum );
}

void CItemUpgrade::SwapVis( CUser* pUser, int nPos1, int nPos2 )
{
	if( !IsValidObj( pUser ) )
		return;

	CItemElem* pItemElemPet = pUser->GetVisPetItem();
	if( !IsUsableItem( pItemElemPet ) )
		return;
	
	pUser->UpdateItem( (BYTE)( pItemElemPet->m_dwObjId ), UI_PETVIS_ITEMSWAP, MAKELONG( nPos1, nPos2 ) );
}

// �ְ��� �񽺿� ��ȭ�� ���� ��� SFX�� ��ȭ��Ų��.
void CItemUpgrade::ChangeVisPetSfx( CUser* pUser, CItemElem* pItemElemPet )
{
	CMover* pVisPet = prj.GetMover( pUser->GetEatPetId() );
	if( IsValidObj( pUser ) && IsValidObj( pVisPet ) )
	{
		DWORD dwSfxId = pItemElemPet->GetVisPetSfxId();
		if( pVisPet->m_dwMoverSfxId != dwSfxId )
		{
			pVisPet->m_dwMoverSfxId = dwSfxId;
			g_UserMng.AddChangeMoverSfxId( pVisPet );
		}
	}
}

void CItemUpgrade::TransFormVisPet( CUser* pUser, OBJID objIdMaterial )
{
	CMover* pEatPet = prj.GetMover( pUser->GetEatPetId() );
	if( IsValidObj( pEatPet ) )
	{
		CAIPet* pAI = static_cast<CAIPet*>( pEatPet->m_pAIInterface );
		if( pAI )
		{
			CItemElem* pItemEatPet = pUser->m_Inventory.GetAtId( pAI->GetPetItemId() );
			CItemElem* pItemMaterial = pUser->m_Inventory.GetAtId( objIdMaterial );
			if( IsUsableItem( pItemEatPet ) && IsUsableItem( pItemMaterial ) )
			{
				if( pItemEatPet->IsVisPet() )
				{
					pUser->AddDefinedText( TID_GAME_PET_TRAN_FAILURE );
					return;
				}
								
				if( pItemMaterial->m_bCharged )		// ���ȭ ������ �α�
					g_dpDBClient.SendLogSMItemUse( "1", pUser, pItemMaterial, pItemMaterial->GetProp() );
				pUser->RemoveItem( (BYTE)( objIdMaterial ), 1 );
				pUser->UpdateItem( (BYTE)( pItemEatPet->m_dwObjId ), UI_TRANSFORM_VISPET, TRUE );
				g_DPSrvr.PutItemLog( pUser, "!", "TRANSFORM_VISPET", pItemEatPet );
				pUser->AddDefinedText( TID_GAME_PET_TRAN_SUCCESS, "\"%s\"", pItemEatPet->GetProp()->szName );
			}
		}
	}
	else
	{
		pUser->AddDefinedText( TID_GAME_PET_TRAN_FAILURE );
	}
}
#endif // __PETVIS