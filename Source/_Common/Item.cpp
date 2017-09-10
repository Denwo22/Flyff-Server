#include "stdafx.h"
#include "defineObj.h"
#include "CreateObj.h"

#if __VER >= 11 // __SYS_IDENTIFY
#include "randomoption.h"
#ifdef __WORLDSERVER
#include "ticket.h"
#endif	// __WORLDSERVER
#endif	// __SYS_IDENTIFY

#include "serialnumber.h"
#if __VER >= 9
#include "definetext.h"
#endif	// 

BOOL IsUsableItem( CItemBase* pItem )
{
	if( pItem == NULL )
		return FALSE;
	
	if( pItem->GetExtra() == 0 )
		return TRUE;
	else
		return FALSE;	// �ŷ����̰ų� ���� ������ �ø� �������� ����� �� ����.
}

BOOL IsUsingItem( CItemBase* pItem )
{
	if( pItem == NULL )
		return FALSE;
	
	if( pItem->GetExtra() != 0 )
		return TRUE;
	else
		return FALSE;	//        .
}
//////////////////////////////////////////////////////////////////////
// CItemBase
//////////////////////////////////////////////////////////////////////
CItemBase::CItemBase() 
{ 
	m_szItemText[0]  = '\0';
	m_dwItemId	     = 0;
	m_dwObjId	     = NULL_ID;
	m_dwObjIndex	 = NULL_ID;
	m_nExtra	     = 0;
	m_nCost		     = 0;
	m_liSerialNumber	= 0;

	SetTexture( NULL );	
}

void CItemBase::Empty()
{
	m_szItemText[0] = '\0';
	m_dwItemId = 0;
	m_nExtra   = 0;
	SetTexture( NULL );
}

ItemProp* CItemBase::GetProp() 
{ 
	return	prj.GetItemProp( m_dwItemId );	
}

CItemBase& CItemBase::operator = ( CItemBase& ib )
{
	_tcscpy( m_szItemText, ib.m_szItemText );
	m_dwItemId	      = ib.m_dwItemId  ;
	m_liSerialNumber	= ib.m_liSerialNumber;

#ifdef __CLIENT
	m_pTexture        = ib.m_pTexture;
#endif

	return *this;
}

void CItemBase::SetTexture()
{
#ifdef __CLIENT
	ItemProp* pProp =GetProp();
	if (!pProp)
	{
		LPCTSTR szErr = Error("CItemBase::SetTexture GetProp() NULL Return %d", m_dwItemId);
		//ADDERRORMSG( szErr );
	}

#if __VER >= 9	// __PET_0410
	CString strIcon	= pProp->szIcon;
	if( pProp->dwItemKind3 == IK3_EGG )
	{
		BYTE nLevel	= 0;
		if( ( (CItemElem*)this )->m_pPet )
			nLevel	= ( (CItemElem*)this )->m_pPet->GetLevel();
		switch( nLevel )
		{
			case PL_D:
			case PL_C:
				strIcon.Replace( ".", "_00." );
				break;
			case PL_B:
			case PL_A:
				strIcon.Replace( ".", "_01." );
				break;
			case PL_S:
				strIcon.Replace( ".", "_02." );
				break;
		}
	}
	m_pTexture = CWndBase::m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ITEM, strIcon ), 0xffff00ff );
#else	// __PET_0410
	m_pTexture = CWndBase::m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ITEM, GetProp()->szIcon), 0xffff00ff );
#endif	// __PET_0410
#endif
}

//
// ������ ������ ����ؼ� �����Ѵ�.
// -1�� �����ϸ� ó���ؼ� �ȵȴ�.
//
int	CItemBase::GetCost( void )
{
	ItemProp *pProp = GetProp();
	if( pProp == NULL )		
		return -1;
	if( pProp->dwCost == 0xFFFFFFFF )	
		return -1;
		
	CItemElem *pItemElem = (CItemElem *)this;

	int nCost;
	if( pItemElem->GetAbilityOption() )
	{		
		//INT((�����۰���+�����۰���*(0.1+(�����۰�ȭ����*�����۰�ȭ����)/18))
		nCost = (int)( pProp->dwCost + pProp->dwCost * ( 0.1f + ( pItemElem->GetAbilityOption() * pItemElem->GetAbilityOption() ) / 18.0f ) );
	}
	else
	{
		nCost = (int)pProp->dwCost;
	}
	
	return nCost;
}

#if __VER >= 11 // __GUILDCOMBATCHIP
DWORD CItemBase::GetChipCost()
{
	ItemProp *pProp = GetProp();
	if( pProp == NULL )		
		return -1;
	if( pProp->dwReferValue1 == 0xFFFFFFFF )	
		return -1;

	return pProp->dwReferValue1;
}
#endif // __GUILDCOMBATCHIP

// ����Ʈ �������ΰ�?
BOOL CItemBase::IsQuest()
{
	ItemProp* p = GetProp();
	if( p->dwItemKind3 == IK3_QUEST )
	{
		if( ::GetLanguage() == LANG_JAP )
		{
			if( p->dwID == II_SYS_SYS_QUE_REDSOCKS )	// ���� 
				return FALSE;
		}

		return TRUE;
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// CItemElem - ������ �ϳ��ϳ��� ��Ҹ� ���Ѵ�.
//////////////////////////////////////////////////////////////////////
/*
	1 ~ 99    
	100 ~ ���� ���� �ΰ�

	�����ۿ� �ΰ� ��ȣ.

*/

CItemElem::CItemElem()
{
	m_idGuild = 0;
	m_nItemNum = 1;
	m_nAbilityOption = 0;
	m_nRepairNumber	= 0;
	m_nHitPoint		= -1;
	m_nRepair	= 0;
	m_byFlag	= 0;
#ifdef __CLIENT
	m_bRepair	= FALSE;
#endif	// __CLIENT

    m_bItemResist = SAI79::NO_PROP;
	m_nResistAbilityOption = 0;	
	m_nResistSMItemId = 0;

#ifdef __WORLDSERVER
	m_bQuery	= FALSE;
#endif	// __WORLDSERVER

//	memset( &m_piercingInfo, 0, sizeof(m_piercingInfo) );
	m_bCharged	= FALSE;
	m_dwKeepTime	= 0;
#if __VER >= 11 // __SYS_IDENTIFY
	m_iRandomOptItemId	= 0;
#else	// __SYS_IDENTIFY
	m_nRandomOptItemId	= 0;
#endif	// __SYS_IDENTIFY


	//	mulcom	BEGIN100405	���� ��ȣ�� �η縶��
	m_n64NewRandomOption	= 0;
	//	mulcom	END100405	���� ��ȣ�� �η縶��


/*
#ifdef __XPET2
	m_nMaxLevel = 0;	
	m_nLevel = 0;		
	m_dwHungry = 0;		
	m_dwFeeling = 0;	
#endif // xPet2
*/
	/*
#ifdef __GIFTBOX0213
#ifdef __WORLDSERVER
	m_nQueryGiftbox	= 0;
#endif	// __WORLDSERVER
#endif	// __GIFTBOX0213
	*/
#if __VER >= 9	// __PET_0410
	m_pPet	= NULL;
#endif	// __PET_0410
#if __VER >= 15 // __PETVIS
	m_bTranformVisPet = FALSE;
#endif // __PETVIS

}

CItemElem::~CItemElem()
{
#if __VER >= 9	// __PET_0410
	SAFE_DELETE( m_pPet );
#endif	// __PET_0410
}

void	CItemElem::Empty( void )
{
	CItemBase::Empty();
#if __VER >= 9
	SAFE_DELETE( m_pPet );
#endif	// __PET_0410
	m_piercing.Clear();
}

// ���� ���� ��´�. 
int	CItemElem::GetGold()
{
	ASSERT( GetProp()->dwItemKind1 == IK1_GOLD );
	return m_nHitPoint;		// int������ ���� ������ �ϱ� ���ؼ� �� ������ ��� 
}

void CItemElem::UseItem()
{
	if( GetProp()->bPermanence != TRUE )
		m_nItemNum--;
}

CItemElem& CItemElem::operator =( CItemElem & ie )
{
	CItemBase::operator =( ie );

	SetTexture( ie.GetTexture() );

	m_nItemNum             = ie.m_nItemNum;
	m_nRepairNumber        = ie.m_nRepairNumber;
	m_nHitPoint            = ie.m_nHitPoint;
	m_nRepair				= ie.m_nRepair;
	m_byFlag               = ie.m_byFlag;
	m_nAbilityOption       = ie.m_nAbilityOption;
	m_idGuild              = ie.m_idGuild;
	m_bItemResist          = ie.m_bItemResist;
	m_nResistAbilityOption = ie.m_nResistAbilityOption;
	m_nResistSMItemId      = ie.m_nResistSMItemId;
	m_dwKeepTime           = ie.m_dwKeepTime;
	m_piercing	= ie.m_piercing;
	m_bCharged	           = ie.m_bCharged;
#if __VER >= 11 // __SYS_IDENTIFY
	m_iRandomOptItemId     = ie.GetRandomOptItemId();
#else	// __SYS_IDENTIFY
	m_nRandomOptItemId     = ie.m_nRandomOptItemId;
#endif	// __SYS_IDENTIFY

	//	mulcom	BEGIN100405	���� ��ȣ�� �η縶��
	m_n64NewRandomOption	= ie.GetNewRandomOption();
	//	mulcom	END100405	���� ��ȣ�� �η縶��

#if __VER >= 9	// __PET_0410
	SAFE_DELETE( m_pPet );
	if( ie.m_pPet )
	{
		m_pPet	= new CPet;
		*m_pPet	= *ie.m_pPet;
	}
#endif	// __PET_0410
#if __VER >= 15 // __PETVIS
	m_bTranformVisPet		= ie.m_bTranformVisPet;
#endif // __PETVIS

	return *this;
}

// ����������ΰ�?
BOOL CItemElem::IsCharged()
{
	if( m_bCharged == 1 || m_nResistSMItemId != 0 )
		return TRUE;

	return FALSE;
}

void CItemElem::GetPiercingAvail( PPIERCINGAVAIL pPiercingAvail )
{
	for( int i = 0; i < GetPiercingSize(); i++ )
	{
		PPIERCINGAVAIL ptr	= CPiercingAvail::GetInstance()->GetPiercingAvail( GetPiercingItem( i ) );
		if( ptr )
		{
			for( int j = 0; j < ptr->nSize; j++ )
			{
				int nFind	= -1;
				for( int k = 0; k < pPiercingAvail->nSize; k++ )
				{
					if(  pPiercingAvail->anDstParam[k] == ptr->anDstParam[j] )
					{
						nFind	= k;
						break;
					}
				}
				if( nFind < 0 )
					nFind	= pPiercingAvail->nSize++;
				pPiercingAvail->anDstParam[nFind]		= ptr->anDstParam[j];
				pPiercingAvail->anAdjParam[nFind]		+= ptr->anAdjParam[j];
			}
		}
	}
}

// �ֻ����� ���� ������ ������ ����
BOOL CItemElem::IsDiceRefineryAble( ItemProp* pProp )
{
	if( !pProp )
		return FALSE;

	if( pProp->dwItemKind2 == IK2_ARMOR ||
		pProp->dwItemKind2 == IK2_WEAPON_MAGIC ||
		pProp->dwItemKind2 == IK2_WEAPON_DIRECT ||
		pProp->dwItemKind2 == IK2_ARMORETC )
		return TRUE;
					
	return FALSE;
}

// ī��� ���� ������ ������ ����
BOOL CItemElem::IsEleRefineryAble( ItemProp* pProp )
{
	if( !pProp )
		return FALSE;

	if( pProp->dwItemKind3 == IK3_SUIT ||
		pProp->dwItemKind2 == IK2_WEAPON_MAGIC ||
		pProp->dwItemKind2 == IK2_WEAPON_DIRECT )
		return TRUE;
	
	return FALSE;
}

#if __VER >= 15 // __15_5TH_ELEMENTAL_SMELT_SAFETY
BOOL CItemElem::IsElementalCard( const DWORD dwItemID )
{
	switch( dwItemID )
	{
	case II_GEN_MAT_ELE_FLAME: case II_GEN_MAT_ELE_RIVER: case II_GEN_MAT_ELE_GENERATOR: case II_GEN_MAT_ELE_DESERT: case II_GEN_MAT_ELE_CYCLON:
		return TRUE;
	default:
		return FALSE;
	}
}
#endif // __15_5TH_ELEMENTAL_SMELT_SAFETY

BOOL CItemElem::IsBinds( void )
{
	ItemProp* pProperty = GetProp();
	if( m_dwKeepTime && pProperty->dwItemKind2 != IK2_WARP )
		return TRUE;

	if( (pProperty->dwFlag & IP_FLAG_BINDS) == IP_FLAG_BINDS )
		return TRUE;

	if( IsFlag( CItemElem::binds ) )
		return TRUE;

#if __VER >= 11 // __SYS_IDENTIFY
	if( g_xRandomOptionProperty->GetRandomOptionSize( GetRandomOptItemId() ) > 0
		&& ( g_xRandomOptionProperty->GetRandomOptionKind( this ) == CRandomOptionProperty::eBlessing || g_xRandomOptionProperty->GetRandomOptionKind( this ) == CRandomOptionProperty::eEatPet ) )
		return TRUE;
	if( GetLevelDown() < 0 )
		return TRUE;
#endif	// __SYS_IDENTIFY

	return FALSE;
}

BOOL CItemElem::IsUndestructable( void )
{
	ItemProp* pProperty = GetProp();
	if( (pProperty->dwFlag & IP_FLAG_UNDESTRUCTABLE ) == IP_FLAG_UNDESTRUCTABLE )
		return TRUE;
	return FALSE;
}

BOOL CItemElem::IsLogable( void )
{
	if( GetProp()->nLog != -1 || GetRandomOptItemId() != 0 )
		return TRUE;
	return FALSE;
}

#ifndef __VM_0820
#ifndef __MEM_TRACE
#ifdef __WORLDSERVER
	#ifdef __VM_0819
	CItemPool*	CItem::m_pPool	= new CItemPool( 512, "CItem" );
	#else	// __VM_0819
	CItemPool*	CItem::m_pPool	= new CItemPool( 512 );
	#endif	// __VM_0819
#else	// __WORLDSERVER
	#ifdef __VM_0819
	CItemPool*	CItem::m_pPool	= new CItemPool( 128, "CItem" );
	#else	// __VM_0819
	CItemPool*	CItem::m_pPool	= new CItemPool( 128 );
	#endif	// __VM_0819
#endif	// __WORLDSERVER
#endif	// __MEM_TRACE
#endif	// __VM_0820

//////////////////////////////////////////////////////////////////////
// CItem - �̰��� ���� ���� �� ������ �����ϴ� ������Ʈ �������̴�. 
//////////////////////////////////////////////////////////////////////
CItem::CItem()
{
	m_dwType = OT_ITEM;
	m_pItemBase = NULL;
	m_idHolder	= 0;
	m_idOwn = NULL_ID;
	m_dwDropTime = 0;
	m_bDropMob = 0;
	m_dwDropTime = timeGetTime();
#ifdef __CLIENT
	m_fGroundY = 0;
	m_vDelta.x = m_vDelta.y = m_vDelta.z = 0;
#endif //__CLIENT
#ifdef __EVENT_MONSTER
	m_IdEventMonster = NULL_ID;
#endif // __EVENT_MONSTER
}

void CItem::SetOwner( OBJID id )
{
	m_idOwn = id;	// �� �������� ������ pAttacker(����Ŀ)������ ǥ��.
	m_dwDropTime = ::timeGetTime();	// ��� ��������� �ð��� �����.
	m_bDropMob = TRUE;		// ���� �׾ ���� ���� ǥ�ø� �ص�.
}


CItem::~CItem()
{
	SAFE_DELETE( m_pItemBase );
	if( GetWorld() )
	{
		#if !defined(__CLIENT)
		#ifdef __LAYER_1021
			GetWorld()->m_respawner.Increment( GetRespawn(), m_nRespawnType, FALSE, GetLayer() );
		#else	// __LAYER_1021
			GetWorld()->m_respawner.Increment( GetRespawn(), m_nRespawnType, FALSE );
		#endif	// __LAYER_1021
		#endif
	}
}

BOOL CItem::Read( CFileIO* pFile )
{
	CObj::Read( pFile );
	return TRUE;
}

#ifdef __CLIENT
// �������� ������ų�� �̰��� �ҷ���� �߷¿� ���� ��������.
void CItem::SetDelta( float fGroundY, D3DXVECTOR3 &vDelta )
{
	m_fGroundY = fGroundY;
	m_vDelta = vDelta;
}
#endif // __CLIENT

void CItem::Process()
{
	CCtrl::Process();
#ifdef __CLIENT
	AddAngle( 0.5f );
	D3DXVECTOR3 vPos = GetPos();

	//--- ��������� ������ ��ǥ�� ��ó���� �ؼ� �ȵȴ�. 
	if( m_fGroundY )	// �̰� 0�̸� �߷�ó���� �ؼ� �ȵȴ�.
	{
		if( vPos.y > m_fGroundY )		// ���߿� ���ִ���?
		{
			m_vDelta.y -= 0.0075f;		// �̵����Ϳ� �߷� ���� ����.
		} 
		else
		{
			vPos.y = m_fGroundY;		// �ٴڿ� ���� ���¸� ������ǥ�� �����ϰ� ����.
			m_vDelta.x = m_vDelta.y = m_vDelta.z = 0;	// �̵����ʹ� ������. Ƣ��� �Ϸ��� �̷��� �ϸ� �ȵȴ�.
			SetPos( vPos );				// ���� ��ǥ ����.
		}

		// �̵����Ͱ� ������ �������ʿ� ����.
		if( m_vDelta.x == 0 && m_vDelta.y == 0 && m_vDelta.z == 0 )
		{
			// ���� ��ǥ�� ����� �ʿ� ����.
		} 
		else
		{
			m_vDelta.x = m_vDelta.z = 0;		// ���⿡ �� ��������.
			vPos += m_vDelta;			// �̵� ���͸� ����.
			SetPos( vPos );
		}
	}
#endif // __CLIENT

#ifdef __WORLDSERVER
	if( g_eLocal.GetState( EVE_SCHOOL ) )
		return;
	if( (int)g_tmCurrent - (int)m_dwDropTime > MIN( 3 ) )
		Delete();
#endif	// __WORLDSERVER
}

void CItem::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
#ifndef __WORLDSERVER
	ItemProp *pItemProp = GetProp();
	if( pItemProp && pItemProp->nReflect > 0 )
	{
		// ����!!! : m_pModel�� CModelObject��°��� �����ϰ� �Ѱ��̴�. �ƴ϶�� �̷��� ���� �ȵȴ�.
		((CModelObject*)m_pModel)->SetEffect( 0, XE_REFLECT );	
	}

	CObj::Render( pd3dDevice );
	if( xRandom( 50 ) == 1 )
		CreateSfx( pd3dDevice, XI_GEN_ITEM_SHINE01, GetPos() );
#endif
}

void CItem::RenderName( LPDIRECT3DDEVICE9 pd3dDevice, CD3DFont* pFont, DWORD dwColor )
{
#ifndef __WORLDSERVER
	// ���� ��ǥ�� ��ũ�� ��ǥ�� �������� �Ѵ�.
	D3DXVECTOR3 vOut, vPos = GetPos(), vPosHeight;
    D3DVIEWPORT9 vp;
	const BOUND_BOX* pBB = m_pModel->GetBBVector();
    pd3dDevice->GetViewport( &vp );
	D3DXMATRIX matTrans;
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixTranslation( &matTrans, vPos.x, vPos.y, vPos.z );
	D3DXMatrixMultiply( &matWorld, &matWorld, &m_matScale );
	D3DXMatrixMultiply( &matWorld, &matWorld, &m_matRotation );
	D3DXMatrixMultiply( &matWorld, &matWorld, &matTrans );
	
	vPosHeight = pBB->m_vPos[0];
	vPosHeight.x = 0;
	vPosHeight.z = 0;
	
	D3DXVec3Project( &vOut, &vPosHeight, &vp, &GetWorld()->m_matProj,
		&GetWorld()->m_pCamera->m_matView, &matWorld);
	vOut.x -= pFont->GetTextExtent( m_pItemBase->GetProp()->szName ).cx / 2;
	pFont->DrawText( vOut.x + 1, vOut.y + 1, 0xff000000, m_pItemBase->GetProp()->szName	);
	pFont->DrawText( vOut.x, vOut.y, dwColor, m_pItemBase->GetProp()->szName );
	return;
#endif	// __WORLDSERVER
}

void	CItemBase::SetSerialNumber( void )
{
	m_liSerialNumber	= CSerialNumber::GetInstance()->Get();
}

CString CItemElem::GetName( void )
{
	ItemProp* pProp	= GetProp();
	CString strName		= pProp->szName;
#if __VER >= 9
	if( pProp->dwItemKind3 == IK3_EGG && m_pPet /*&& m_pPet->GetLevel() > PL_EGG*/ )
	{
		MoverProp* pMoverProp	= prj.GetMoverProp( m_pPet->GetIndex() );
		if( pMoverProp )
		{
#ifdef __PET_1024
			if( m_pPet->HasName() )
				strName.Format( prj.GetText( TID_GAME_CAGE_STRING ), m_pPet->GetName() );
			else
#endif	// __PET_1024
				strName.Format( prj.GetText( TID_GAME_CAGE_STRING ), pMoverProp->szName );
		}
	}
/*
	else if( pProp->IsUltimate() )
	{
		int	nFind	= strName.Find( "@", 0 );
		if( nFind > -1 )
			strName.Delete( nFind - 1, 2 );
	}
*/
#endif	// 
	return strName;
}


enum	{	eNoLevelDown, e5LevelDown, e10LevelDown,	};
	int		GetLevelDown( void );	// 64|63
	void	SetLevelDown( int i );


#if __VER >= 11 // __SYS_IDENTIFY
int	CItemElem::GetLevelDown( void )
{
	if( m_iRandomOptItemId & 0x8000000000000000 )
		return -10;
	else if( m_iRandomOptItemId & 0x4000000000000000 )
		return -5;
	return 0;
}

void CItemElem::SetLevelDown( int i )
{
	//	0x8000000000000000
	//	0x4000000000000000
	m_iRandomOptItemId	&= ~0xC000000000000000;
	if( i == e5LevelDown )
		m_iRandomOptItemId	|= 0x4000000000000000;
	else if( i == e10LevelDown )
		m_iRandomOptItemId	|= 0x8000000000000000;
}

DWORD CItemElem::GetLimitLevel( void )
{
	if( GetProp()->dwLimitLevel1 == 0xFFFFFFFF ) 
		return 0xFFFFFFFF;
	int nLimitLevel	= static_cast<int>( GetProp()->dwLimitLevel1 ) + GetLevelDown();
	if( nLimitLevel < 0 )
		nLimitLevel		= 0;

	return (DWORD)nLimitLevel;
}
#endif	// __SYS_IDENTIFY

#if __VER >= 14 // __NEW_ITEM_LIMIT_LEVEL
BOOL CItemElem::IsLimitLevel( CMover* pMover )
{
	if( pMover->GetJobType() >= JTYPE_MASTER && pMover->GetJobType() > pMover->GetJobType( GetProp()->dwItemJob ) )
		return FALSE;

	if( (DWORD)( pMover->GetLevel() ) >= GetLimitLevel() )
		return FALSE;

	return TRUE;
}
#endif // __NEW_ITEM_LIMIT_LEVEL


#if __VER >= 12 // __EXT_PIERCING
// bSize�� �Ǿ�� ����� �ø� �� �ִ��� �˻��� �� TRUE���� setting �Ѵ�.
// bSize�� TRUE�� �� ��� dwTagetItemKind3�� NULL_ID�� �Ѵ�.
BOOL CItemElem::IsPierceAble( DWORD dwTargetItemKind3, BOOL bSize )
{
	if( !GetProp() )
		return FALSE;

	int nPiercedSize = GetPiercingSize();
	if( bSize ) // �Ǿ�� ����� �ø��� ���
		nPiercedSize++;
	
	if( GetProp()->dwItemKind3 == IK3_SUIT )
	{
		if( nPiercedSize <= MAX_PIERCING_SUIT )
		{
			if( dwTargetItemKind3 == NULL_ID )
				return TRUE;
			else if( dwTargetItemKind3 == IK3_SOCKETCARD )
					return TRUE;
		}
	}
	
	else if( GetProp()->dwItemKind3 == IK3_SHIELD
		|| GetProp()->dwItemKind2 == IK2_WEAPON_DIRECT
		|| GetProp()->dwItemKind2 == IK2_WEAPON_MAGIC
		)
	{
		if( nPiercedSize <= MAX_PIERCING_WEAPON )
		{
			if( dwTargetItemKind3 == NULL_ID )
				return TRUE;
			else if( dwTargetItemKind3 == IK3_SOCKETCARD2 )
				return TRUE;
		}
	}

#if __VER >= 15 // __PETVIS
	else if( IsVisPet() )
	{
		if( nPiercedSize <= MAX_VIS )
		{
			if( dwTargetItemKind3 == NULL_ID )
				return TRUE;
			else if( dwTargetItemKind3 == IK3_VIS )
				return TRUE;
		}
	}
#endif // __PETVIS

	return FALSE;
}
#endif // __EXT_PIERCING

#if __VER >= 11
#ifdef __WORLDSERVER
BOOL CItemElem::IsActiveTicket( DWORD dwItemId )
{
	if( !IsFlag( expired ) && GetProp()->dwItemKind3 == IK3_TICKET && m_dwKeepTime > 0 )
	{
		TicketProp*	pProp1	= CTicketProperty::GetInstance()->GetTicketProp( m_dwItemId );
		TicketProp*	pProp2	= CTicketProperty::GetInstance()->GetTicketProp( dwItemId );
		return ( pProp1->dwWorldId == pProp2->dwWorldId );
	}
	return FALSE;
}
#endif	// __WORLDSERVER
BOOL	IsNeedTarget( ItemProp* pProp )
{	// �������� ����ϱ� ���� ���� Ŭ�� ���� ��
	// Ŀ���� �ٲ�鼭 �κ��丮 �� �ٸ� ������ Ŭ���� �ʿ��� �������ΰ�?
#if __VER >= 12 // __PET_0519
	// ������ �ĺ��� �߰��� ���ŷο� �Ӽ� Ȯ������ ����
	return( pProp->dwExeTarget == EXT_ITEM );
#else	// __PET_0519
	switch( pProp->dwID )
	{
		case II_SYS_SYS_SCR_AWAKE:
		case II_SYS_SYS_SCR_BLESSEDNESS:
		case II_SYS_SYS_SCR_BLESSEDNESS02:
		case II_SYS_SYS_SCR_LEVELDOWN01:
		case II_SYS_SYS_SCR_LEVELDOWN02:
		case II_SYS_SYS_SCR_AWAKECANCEL:
		case II_SYS_SYS_SCR_AWAKECANCEL02:
		case II_SYS_SYS_QUE_PETRESURRECTION02_S:
		case II_SYS_SYS_QUE_PETRESURRECTION02_A:
		case II_SYS_SYS_QUE_PETRESURRECTION02_B:
//		case II_SYS_SYS_SCR_PETAWAKE:
//		case II_SYS_SYS_SCR_PETAWAKECANCEL:
			return TRUE;
		default:
			return FALSE;
	}
#endif	// __PET_0519
}
#endif	// __VER

BOOL CItemElem::IsEgg()
{
	if( !IsPet() )
		return FALSE;
	return ( !m_pPet || m_pPet->GetLevel() == PL_EGG );
}

#if __VER >= 15 // __PETVIS
void  CItemElem::SetSwapVisItem( int nPos1, int nPos2 )
{
	int nSize = GetPiercingSize();
	if( nPos1 >= nSize || nPos2 >= nSize )
		return;

	DWORD dwTempId = GetPiercingItem( nPos1 );
	time_t tmTemp = GetVisKeepTime( nPos1 );

	SetPiercingItem( nPos1, GetPiercingItem( nPos2 ) );
	SetVisKeepTime( nPos1, GetVisKeepTime( nPos2 ) );
	SetPiercingItem( nPos2, dwTempId );
	SetVisKeepTime( nPos2, tmTemp );
}

DWORD CItemElem::GetVisPetSfxId()
{
	int nLevel = 0;
	for( int i=0; i<GetPiercingSize(); i++ )
	{
		ItemProp* pProp = prj.GetItemProp( GetPiercingItem( i ) );
		if( pProp && pProp->dwAbilityMax > (DWORD)( nLevel ) )
			nLevel = pProp->dwAbilityMax;
	}

	switch( nLevel )
	{
		case 1:	return XI_BUFFPET_GRADE1;
		case 2: return XI_BUFFPET_GRADE2;
		case 3: return XI_BUFFPET_GRADE3;
	}

	return NULL_ID;
}
#endif // __PETVIS





//	mulcom	BEGIN100405	���� ��ȣ�� �η縶��
__int64		CItemElem::GetNewRandomOption()
{
	return m_n64NewRandomOption;
}

__int64*	CItemElem::GetNewRandomOptionPtr()
{
	return &m_n64NewRandomOption;
}

void		CItemElem::ResetNewRandomOption()
{
	m_n64NewRandomOption = 0;

	return;
}

void		CItemElem::SelectNewRandomOption()
{
	m_iRandomOptItemId	= (( m_iRandomOptItemId & 0xC0000000000000FF ) | m_n64NewRandomOption );

	return;
}

bool		CItemElem::SelectRandomOption( BYTE bySelectFlag )
{
	bool	bRetValue	= true;

	if( bySelectFlag == _AWAKE_OLD_VALUE )
	{
		ResetNewRandomOption();
	}
	else if( bySelectFlag == _AWAKE_NEW_VALUE )
	{
		SelectNewRandomOption();
		ResetNewRandomOption();
	}
	else
	{
		WriteLog( "bySelectFlag is invalid value. bySelectFlag : [%d]", (int)( bySelectFlag ) );

		bRetValue	= false;
	}

	return	bRetValue;
}

//	mulcom	END100405	���� ��ȣ�� �η縶��




////////////////////////////////////////////////////////////////////////////////