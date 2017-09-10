#include "stdafx.h"

#ifndef __BUFF_1107

#include "skillinfluence.h"
#include "defineSkill.h"


#ifdef __WORLDSERVER
#include "User.h"
extern	CUserMng	g_UserMng;
#if __VER >= 9
#include "defineobj.h"
#endif	//
#include "DPDatabaseClient.h"
extern	CDPDatabaseClient	g_dpDBClient;
#endif // __WORLDSERVER


CSkillInfluence::CSkillInfluence()
{
	Init();
}

//
CSkillInfluence::~CSkillInfluence()
{
	Destroy();
}

//
void CSkillInfluence::Init( void )
{
	m_pMover		= NULL;
	memset( m_aSkillInfluence, 0, sizeof(m_aSkillInfluence) );
#if __VER < 8 //__CSC_VER8_3
	m_pEmptyNode = NULL;
#endif //__CSC_VER8_3
}

#if __VER >= 8 //__CSC_VER8_3
#if !( defined( __CORESERVER ) || defined( __DBSERVER ) ) 
SKILLINFLUENCE* CSkillInfluence::SortSkillArray()
{
	SKILLINFLUENCE *pList = m_aSkillInfluence, *pNode, *tempNode;
	int	i = 0;
	BOOL swapflag = FALSE;
	
	while(i < MAX_SKILLINFLUENCE)
	{
		pNode = pList++;
		if((int)pNode->wID == 0 && !swapflag)
		{
			tempNode = pNode;
			swapflag = TRUE;
		}
		if(swapflag && (int)pNode->wID > 0)
		{
			*tempNode = *pNode;
			Remove(pNode);
			tempNode++;
		}
		i++;
	}
	return tempNode;
}

#ifdef __PVPDEBUFSKILL
BOOL CSkillInfluence::InsertBuff(SKILLINFLUENCE *pNode, WORD wType, WORD wID, DWORD dwLevel, DWORD dwTime, DWORD dwAttackerID)
#else // __PVPDEBUFSKILL
BOOL CSkillInfluence::InsertBuff(SKILLINFLUENCE *pNode, WORD wType, WORD wID, DWORD dwLevel, DWORD dwTime)
#endif // __PVPDEBUFSKILL
{
	int	i = 0;
	int count = 0;
	SKILLINFLUENCE *pList = m_aSkillInfluence, *pNext, *pFirstSBuff;
	BOOL FirstFlag = FALSE;
	ItemProp* pItem = NULL;
	BOOL bFlag = FALSE;

	while(i < MAX_SKILLINFLUENCE)
	{
		pNext = pList++;
		if( pNext->wType == BUFF_SKILL )
		{
			if(!FirstFlag)
			{
				pFirstSBuff = pNext;
				FirstFlag = TRUE;
			}
			count++;
		}
		i++;
	}
#if __VER >= 9	// __PET_0410
	if( count == MAX_SKILLBUFF_COUNT && wType == BUFF_SKILL )
#else	// __PET_0410
	if( count == MAX_SKILLBUFF_COUNT && wType != BUFF_ITEM )
#endif	// __PET_0410
	{
		pItem = prj.GetSkillProp(pFirstSBuff->wID);
		if(pItem != NULL && pItem->nEvildoing >= 0) //�� ó�� ������ ������� ��� �� �� ����.
		{
			RemoveSkillInfluence(pFirstSBuff->wType, pFirstSBuff->wID);
#ifdef __PVPDEBUFSKILL
			bFlag = Set(pNode, wType, wID, dwLevel, dwTime, dwAttackerID);
#else // __PVPDEBUFSKILL
			bFlag = Set(pNode, wType, wID, dwLevel, dwTime);
#endif // __PVPDEBUFSKILL
		}
	}
	else
#ifdef __PVPDEBUFSKILL
		bFlag = Set(pNode, wType, wID, dwLevel, dwTime, dwAttackerID);
#else // __PVPDEBUFSKILL
		bFlag = Set(pNode, wType, wID, dwLevel, dwTime );
#endif // __PVPDEBUFSKILL

	return bFlag;
}
#endif // #if !( defined( __CORESERVER ) || defined( __DBSERVER ) ) i
#endif //__CSC_VER8_3

LPSKILLINFLUENCE CSkillInfluence::GetItemBuf( DWORD dwItemKind3 )
{
	char szGMCommand[64] = {0,};
	for( int i = 0; i < MAX_SKILLINFLUENCE; i ++ )	
	{
		if( m_aSkillInfluence[i].wType != BUFF_ITEM )
			continue;
		if( (int)m_aSkillInfluence[i].wID <= 0 )
			continue;
		ItemProp *pItemProp = prj.GetItemProp( m_aSkillInfluence[i].wID );
		if( pItemProp == NULL || pItemProp->dwItemKind3 != dwItemKind3 )
			continue;
		//ã��
		return &m_aSkillInfluence[i];
	}
	return NULL;
}

//
// ���ο� ��ų ���¸� �߰��Ѵ�.
//
#ifdef __PVPDEBUFSKILL
BOOL CSkillInfluence::Set( WORD wType, WORD wID, DWORD dwLevel, DWORD dwTime, OBJID dwAttackerID )
#else // __PVPDEBUFSKILL
BOOL CSkillInfluence::Set( WORD wType, WORD wID, DWORD dwLevel, DWORD dwTime )
#endif // __PVPDEBUFSKILL
{
	SKILLINFLUENCE *pList = m_aSkillInfluence, *pNode;
	BOOL	bAdd = FALSE;

#if __VER >= 8 //__CSC_VER8_3
#if !( defined( __CORESERVER ) || defined( __DBSERVER ) )
	//Array Sort
	SKILLINFLUENCE *pNextNode;
	pNextNode = SortSkillArray();
#endif // !( defined( __CORESERVER ) || defined( __DBSERVER ) )
#endif //__CSC_VER8_3

#if __VER >= 8 //__Y_FLAG_SKILL_BUFF
	if( (int)dwTime < 0 )
	{
		LPCTSTR szErr = Error( "CSkillInfluence::Set : dwTime=%d, %d %d %d", (int)dwTime, wType, wID, dwLevel );
		return FALSE;
	}	
#else //__Y_FLAG_SKILL_BUFF
	if( (int)dwTime <= 0 )
	{
		LPCTSTR szErr = Error( "CSkillInfluence::Set : dwTime=%d, %d %d %d", (int)dwTime, wType, wID, dwLevel );
		return FALSE;
	}
#endif //__Y_FLAG_SKILL_BUFF
	
	pNode = Find( wType, wID );		// �̹� ���� ��ų���°� �־����� ã��.

#if __VER >= 8 //__CSC_VER8_3
	if( pNode && pNode->dwLevel == dwLevel ) // 8�� Level Check Add.
#else
	if( pNode )		// �־����� �ð��� �� ������.
#endif //__CSC_VER8_3
	{
		DWORD dwTimeRemain = pNode->tmCount - ( ::timeGetTime() - pNode->tmTime ); // ���� ��ų ���� �ð�
#ifdef __PVPDEBUFSKILL
#ifdef __JEFF_11_1
		if( pNode ->wType == BUFF_ITEM && 
			( pNode->wID == II_SYS_SYS_SCR_PET_FEED_POCKET02 
#if __VER >= 12 // __PET_0519
				// �� �������� �ߺ� ���� �� �ֵ��� ����
				// �� �� �� �߰��ȴٸ� ������Ƽ�� Ȯ���ϴ� ������� ��������
				|| pNode->wID == II_SYS_SYS_SCR_PET_TONIC_A
				|| pNode->wID == II_SYS_SYS_SCR_PET_TONIC_B 
#endif	// __PET_0519
			)
		)
			dwTime	+= dwTimeRemain;
#endif	// __JEFF_11_1

		if(	dwTime > dwTimeRemain )	// ���� �ð��� �� ���� ��츸 �ð� �缼��
			Set( pNode, pNode->wType, pNode->wID, pNode->dwLevel, dwTime, dwAttackerID );		// ���� �ð��� �� ����.
#else // __PVPDEBUFSKILL
		Set( pNode, pNode->wType, pNode->wID, pNode->dwLevel, dwTime );		// ���� �ð��� �� ����.
#endif // __PVPDEBUFSKILL
		return FALSE;
	}
	else	// �������� �߰���.
	{
		int		i = MAX_SKILLINFLUENCE;
#if __VER < 8 //__CSC_VER8_3		
		if( m_pEmptyNode )
		{
#ifdef __PVPDEBUFSKILL
			if( Set( m_pEmptyNode, wType, wID, dwLevel, dwTime, dwAttackerID ) )
#else // __PVPDEBUFSKILL
			if( Set( m_pEmptyNode, wType, wID, dwLevel, dwTime ) )
#endif // __PVPDEBUFSKILL
			{
				bAdd = TRUE;
				// ������ Ȯ�� - Set�� �������� ��쿡�� m_pEmptyNode�� NULL�� �ʱ�ȭ �ؾ� ���� ������?
				m_pEmptyNode = NULL;	// ����� �־����� ������� �Ѵ�.
			}
		} 
		else
#endif //__CSC_VER8_3		
		{
#if __VER >= 8 && !( defined( __CORESERVER ) || defined( __DBSERVER ) ) //__CSC_VER8_3
			if( !(pNode && pNode->dwLevel >= dwLevel) )
			{
				if(pNode)
					RemoveSkillInfluence(pNode->wType, pNode->wID);
				// ������ Ȯ�� - ������ �� �� �ִ� ��� InsertBuff�� �������. �׷� ��� bAdd�� TRUE�� ������ ���� ������, DST PARAM�� ����Ǿ� �������� �ʴ´�.
#ifdef __PVPDEBUFSKILL
				if( InsertBuff(pNextNode, wType, wID, dwLevel, dwTime, dwAttackerID) )
#else // __PVPDEBUFSKILL
				if( InsertBuff(pNextNode, wType, wID, dwLevel, dwTime) )
#endif // __PVPDEBUFSKILL
					bAdd = TRUE;
				else
					bAdd = FALSE;
			}
			else
				bAdd = FALSE;
#else
			while( i-- )
			{
				pNode = pList++;
				if( (int)pNode->wID > 0 )	continue;	// ����ִ� ������ ã��.

#ifdef __PVPDEBUFSKILL
				Set( pNode, wType, wID, dwLevel, dwTime, dwAttackerID );
#else // __PVPDEBUFSKILL
				Set( pNode, wType, wID, dwLevel, dwTime );
#endif // __PVPDEBUFSKILL
				bAdd = TRUE;
				break;
			}
#endif //__CSC_VER8_3
		}
		if( bAdd == FALSE )
		{
		}
	}
	return( bAdd );
}


//
#ifdef __PVPDEBUFSKILL
BOOL CSkillInfluence::Set( SKILLINFLUENCE *pNode, WORD wType, WORD wID, DWORD dwLevel, DWORD dwTime, OBJID dwAttackerID )
#else // __PVPDEBUFSKILL
BOOL CSkillInfluence::Set( SKILLINFLUENCE *pNode, WORD wType, WORD wID, DWORD dwLevel, DWORD dwTime )
#endif // __PVPDEBUFSKILL
{
//	locked
	if( pNode == NULL )
	{
		Error( "CSkillInfluence::Set : pNode�� NULL" );
		return FALSE;
	}
	
#ifdef __WORLDSERVER
	if( m_pMover->IsPlayer() && ((CUser*)m_pMover)->m_nDead )
	{
		ItemProp		*pSkillProp;
		
		if( wType == BUFF_SKILL )
		{
			pSkillProp = prj.GetSkillProp( wID );
			if( pSkillProp && pSkillProp->nEvildoing < 0 )
				return FALSE;
		}
	}
#endif // __WORLDSERVER

	pNode->wType	= wType;
	pNode->wID		= wID;
	pNode->dwLevel	= dwLevel;
	pNode->tmTime	= timeGetTime();
	pNode->tmCount = dwTime;
#ifdef __PVPDEBUFSKILL
	pNode->dwAttackerID = dwAttackerID;
#endif // __PVPDEBUFSKILL
#ifdef __WORLDSERVER
	BOOL bCharged = FALSE;
	if( pNode->wType == BUFF_ITEM
#if __VER >= 11 // __SYS_COLLECTING
		|| pNode->wType == BUFF_ITEM2
#endif	// __SYS_COLLECTING
		)
	{
		ItemProp *pItemProp = prj.GetItemProp( pNode->wID );
		if( pItemProp && pItemProp->bCharged ) // ���ȭ �������� ���ᰡ �ȵǰ� ����
			bCharged = TRUE;
	}
#if __VER >= 9	// __PET_0410
	else if( pNode->wType == BUFF_PET )
		bCharged	= TRUE;
#endif	// __PET_0410
#ifdef __DST_GIFTBOX
	else if( pNode->wType == BUFF_EQUIP )
		bCharged = TRUE;
#endif // __DST_GIFTBOX

	if( bCharged == FALSE && dwTime >= MIN(61) )
		if( m_pMover )
			Error( "SkillInflu::Set : Name=%s Time=%d dwID=%d Str=%d sta=%d int=%d dwLevel=%d GM=%d", m_pMover->GetName(), dwTime, pNode->wID, m_pMover->GetStr(), m_pMover->GetSta(), m_pMover->GetInt(), pNode->dwLevel, m_pMover->m_dwAuthorization );

	g_UserMng.AddSetSkillState( m_pMover, m_pMover, wType, wID, dwLevel, dwTime );
#endif //__WORLDSERVER
	return	TRUE;
}

//
void CSkillInfluence::Serialize( CAr & ar )
{
	int j, nMax = 0;
	DWORD	dwOdd;

	if( ar.IsStoring() )	// Saving
	{
		for( j = 0; j < MAX_SKILLINFLUENCE; j++ )		// ����Ÿ �ִ°͸� ���� ��.
			if( m_aSkillInfluence[j].wID > 0 )
				nMax ++;

		ar << nMax;
		for( j = 0; j < MAX_SKILLINFLUENCE ; ++j )
		{
			if( m_aSkillInfluence[j].wID > 0 )
			{
				ar << m_aSkillInfluence[j].wType;
				ar << m_aSkillInfluence[j].wID;

#if __VER >= 11 // __SYS_COLLECTING
				if( m_aSkillInfluence[j].wType == BUFF_ITEM2 )
				{
					time_t t	= (time_t)m_aSkillInfluence[j].dwLevel - time_null();
					ar << t;
				}
				else
					ar << m_aSkillInfluence[j].dwLevel;
#else	// __SYS_COLLECTING
				ar << m_aSkillInfluence[j].dwLevel;
#endif	// __SYS_COLLECTING

#ifdef __WORLDSERVER			
				if( m_aSkillInfluence[j].tmCount )
				{
					if( m_aSkillInfluence[j].tmTime )
						dwOdd = m_aSkillInfluence[j].tmCount - (timeGetTime() - m_aSkillInfluence[j].tmTime);	// �����ð� ���.
					else
						dwOdd = m_aSkillInfluence[j].tmCount;
				}
				else
					dwOdd = 0;
				if( (int)dwOdd < 0 )		dwOdd = 0;
				ar << dwOdd;		// ���� �ð��� ����.
#else // World
				ar << m_aSkillInfluence[j].tmCount;
#endif // not World
			}
		}
	} else
	// Loading
	{

		memset( m_aSkillInfluence, 0, sizeof(m_aSkillInfluence) );
#if __VER < 8 //__CSC_VER8_3
		m_pEmptyNode = NULL;
#endif //__CSC_VER8_3
		ar >> nMax;
		for( j = 0 ; j < nMax; ++j )		// �ִ¸�ŭ�� �θ���.
		{
			ar >> m_aSkillInfluence[j].wType;
			ar >> m_aSkillInfluence[j].wID;
#if __VER >= 11 // __SYS_COLLECTING
			if( m_aSkillInfluence[j].wType == BUFF_ITEM2 )
			{
				time_t t;
				ar >> t;
				m_aSkillInfluence[j].dwLevel	= time_null() + t;
			}
			else
				ar >> m_aSkillInfluence[j].dwLevel;
#else	// __SYS_COLLECTING
			ar >> m_aSkillInfluence[j].dwLevel;
#endif	// __SYS_COLLECTING
			ar >> dwOdd;
#ifdef __WORLDSERVER				
			BOOL bCharged = FALSE;
			if( m_aSkillInfluence[j].wType == BUFF_ITEM
#if __VER >= 11 // __SYS_COLLECTING
				|| m_aSkillInfluence[j].wType == BUFF_ITEM2
#endif	// __SYS_COLLECTING
				)
			{
				ItemProp *pItemProp = prj.GetItemProp( m_aSkillInfluence[j].wID );
				if( pItemProp )
				{
					if( pItemProp->bCharged ) // ���ȭ �������� ���ᰡ �ȵǰ� ����
						bCharged = TRUE;
				}
			}
#if __VER >= 9	// __PET_0410
			else if( m_aSkillInfluence[j].wType == BUFF_PET )
				bCharged	= TRUE;
#endif	// __PET_0410
#ifdef __DST_GIFTBOX
			else if( m_aSkillInfluence[j].wType == BUFF_EQUIP )
				bCharged	= TRUE;
#endif // __DST_GIFTBOX

			if( bCharged == FALSE && m_pMover && dwOdd >= MIN(61) )
			{
				Error( "SkillInflu::>Load : %s %d", m_pMover->GetName(), dwOdd );
				dwOdd = MIN(60);
			}
#endif //__WORLDSERVER
			m_aSkillInfluence[j].tmCount = dwOdd;			// �ε������� �����ð��� �ε��ؼ� ���� ����.
		} // for

	}
}

//
//


void CSkillInfluence::Reset( void )
{
	int i;
	for( i = 0; i < MAX_SKILLINFLUENCE; i ++ )		// ����Ÿ�̸Ӹ� Ŭ������� ������ų�� �ٽ� ������.
		m_aSkillInfluence[i].tmTime = 0;
}

#ifdef __CLIENT
DWORD CSkillInfluence::GetDisguise( void )
{
	char szGMCommand[64] = {0,};
	for( int i = 0; i < MAX_SKILLINFLUENCE; i ++ )	
	{
		if( m_aSkillInfluence[i].wType != BUFF_ITEM )
			continue;
		if( (int)m_aSkillInfluence[i].wID <= 0 )
			continue;
		ItemProp *pItemProp = prj.GetItemProp( m_aSkillInfluence[i].wID );
		if( pItemProp == NULL || pItemProp->dwItemKind3 != IK3_TEXT_DISGUISE )
			continue;
		//���������� ã�´�.
		char* p1 = strstr( pItemProp->szTextFileName, "/dis ");		//pItemProp->szTextFileName - /dis 20 /nodis
		if( p1 )
		{
			// ���� ��ȣ�� ã�´�.
			int n = atoi( p1 + 5 );	// +5 - "/dis " skip
			if( n > 0 )
			{
				MoverProp* pMoverProp	= prj.GetMoverProp( n );
				if( pMoverProp )
					return pMoverProp->dwID;
			}
		}
	}
	return NULL_ID;
}
#endif // __CLIENT

#if !( defined( __CORESERVER ) || defined( __DBSERVER ) ) 

BOOL CSkillInfluence::HasLikeItemBuf( DWORD dwItemKind3 )
{
	return LikeItemBuf( dwItemKind3 );
}

BOOL CSkillInfluence::LikeItemBuf( DWORD dwItemKind3 )
{
	for( int i = 0; i < MAX_SKILLINFLUENCE; i ++ )	
	{
		if( m_aSkillInfluence[i].wType == BUFF_ITEM 
#if __VER >= 11 // __SYS_COLLECTING
			|| m_aSkillInfluence[i].wType == BUFF_ITEM2
#endif	// __SYS_COLLECTING
			)
		{
			ItemProp *pItemProp = prj.GetItemProp( m_aSkillInfluence[i].wID );
			if( pItemProp && dwItemKind3 == pItemProp->dwItemKind3 )
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

void CSkillInfluence::RemoveLikeItemBuf( DWORD dwItemKind3 )
{
	for( int i = 0; i < MAX_SKILLINFLUENCE; i ++ )	
	{
		if( m_aSkillInfluence[i].wType == BUFF_ITEM
#if __VER >= 11 // __SYS_COLLECTING
			|| m_aSkillInfluence[i].wType == BUFF_ITEM2
#endif	// __SYS_COLLECTING
			)
		{
			ItemProp *pItemProp = prj.GetItemProp( m_aSkillInfluence[i].wID );
			if( pItemProp && dwItemKind3 == pItemProp->dwItemKind3 )
			{
				RemoveSkillInfluence( m_aSkillInfluence[i].wType, pItemProp->dwID );
			}
		}
	}
}



BOOL CSkillInfluence::RemoveAllSkillInfluence()
{
	int		i = MAX_SKILLINFLUENCE;
	SKILLINFLUENCE *pList = m_aSkillInfluence, *pNode;
	
	while( i-- )
	{
		pNode = pList++;

		if( (int)pNode->wID > 0 )
		{
			if( pNode->wType == BUFF_ITEM
#if __VER >= 11 // __SYS_COLLECTING
				|| pNode->wType == BUFF_ITEM2
#endif	// __SYS_COLLECTING
			)
			{
				ItemProp *pItemProp = prj.GetItemProp( pNode->wID );
				if( pItemProp && pItemProp->bCharged ) // ���ȭ �������� ���ᰡ �ȵǰ� ����
					continue;
			}
#ifdef __VER >= 12
			if( pNode->wType == BUFF_EQUIP )
				continue;
#endif // __VER >= 12
#if __VER >= 9	// __PET_0410
			if( pNode->wType == BUFF_PET )
				continue;
#endif	// __PET_0410

			RemoveSkillInfluence( pNode );
		}
	}

	return TRUE;
}		
/*
BOOL CSkillInfluence::IsEmpty()
{
	int     nCount = 0;
	int		i = MAX_SKILLINFLUENCE;
	SKILLINFLUENCE *pList = m_aSkillInfluence, *pNode;
	
	while( i-- )
	{
		pNode = pList++;
		
		if( (int)pNode->wID > 0 )
		{
			nCount ++;
			return FALSE;
		}
	}
	return TRUE;
}
*/
	
//
// ��ϵ� ��ų���µ��� �� ���μ��� �˻��Ͽ� �ð��� �����͵��� �����Ѵ�.
//
void CSkillInfluence::Process( void )
{
#if defined(__WORLDSERVER)
	DWORD dwTimeCurrent	= g_tmCurrent;		// ���� �ð� ����.
#else	//defined(__WORLDSERVER) //�شܽ�ų����ð��Ǵܹ����� Ŭ���̾�Ʈ�� ����
	DWORD dwTimeCurrent	= timeGetTime();		// ���� �ð� ����.
#endif  //defined(__WORLDSERVER)

	CMover *pMover = m_pMover;
	ItemProp *pProp = NULL;
#if __VER >= 11 // __SYS_COLLECTING
	time_t	t	= time_null();
#endif	// __SYS_COLLECTING

	int		i = MAX_SKILLINFLUENCE;
	SKILLINFLUENCE *pList = m_aSkillInfluence, *pNode;

	while( i-- )
	{
		pNode = pList++;
		if( (int)pNode->wID <= 0 )	continue;	// ����ִ� ������ ��ŵ.

		// ����ð����� ���۽ð��� ���� �帥�ð��� ���´�. �帥�ð��� ��ų���ӽð��� �Ѿ�� ����.
	#if defined(__WORLDSERVER)
		// �ð��� �ٵǱ����� �Ư�� ���ǿ� ���ؼ� �߰������� �Ǵ°���� ó��.
		if( pMover )
		{
			if( pNode->wType == BUFF_SKILL )
			{
				switch( pNode->wID )
				{
				case SI_JST_SUP_POISON:
				case SI_JST_SUP_BLEEDING:
				case SI_JST_SUP_ABSORB:
				case SI_ACR_SUP_YOYOMASTER:
				case SI_JST_YOYO_CRITICALSWING:
					if( pMover->GetActiveHandItemProp()->dwWeaponType != WT_MELEE_YOYO )		// �տ���� �ִ� ���Ⱑ ��䰡 �ƴҶ�.
					{
						RemoveSkillInfluence( pNode );		// ����.
						continue;
					}
					break;
				case SI_RAG_SUP_FASTATTACK:
				case SI_ACR_SUP_BOWMASTER:
					if( pMover->GetActiveHandItemProp()->dwWeaponType != WT_RANGE_BOW )		// �տ���� �ִ� ���Ⱑ ��Ŭ�� �ƴҶ�.
					{
						RemoveSkillInfluence( pNode );		// ����.
						continue;
					}
					break;
#if __VER >= 10 // __LEGEND	//	9�� ���½ý���	Neuz, World, Trans
				case SI_BLD_MASTER_ONEHANDMASTER:
					if( !pMover->IsDualWeapon() )		// �տ���� �ִ� ���Ⱑ ��չ��Ⱑ �ƴҶ�.
					{
						RemoveSkillInfluence( pNode );		// ����.
						continue;
					}
					break;
				case SI_KNT_MASTER_TWOHANDMASTER:
					if( pMover->GetActiveHandItemProp()->dwHanded != HD_TWO )		// �տ���� �ִ� ���Ⱑ ��չ��Ⱑ �ƴҶ�.
					{
						RemoveSkillInfluence( pNode );		// ����.
						continue;
					}
					break;
#endif	//__LEGEND	//	9�� ���½ý���	Neuz, World, Trans
				case SI_MER_SUP_IMPOWERWEAPON:		// 
					{
						BOOL bRelease = FALSE;

						CItemElem *pItemElemR = pMover->GetWeaponItem();
						CItemElem *pItemElemL = pMover->GetLWeaponItem();

						if( pItemElemR && pItemElemL )	// ������ ������.
						{
							if( (pItemElemR && pItemElemR->m_bItemResist == SAI79::NO_PROP) &&		// ��մ� �Ӽ��� ������
								(pItemElemL && pItemElemL->m_bItemResist == SAI79::NO_PROP) )
								bRelease = TRUE;		// ����
						} else
						if( pItemElemR && pItemElemL == NULL )	// �����ո� ��� ������.
						{
							if( pItemElemR && pItemElemR->m_bItemResist == SAI79::NO_PROP )		// �����տ� �Ӽ��� ������
								bRelease = TRUE;		// ����
						} else
						if( pItemElemR == NULL && pItemElemL == NULL )	// ��մ� ������
						{
							bRelease = TRUE;		// �̶��� ������ ����
						}

						if( bRelease )
						{
							RemoveSkillInfluence( pNode );		// ����.
							continue;
						}
					}
					break;
				case SI_MER_SUP_BLAZINGSWORD:		// 
				case SI_MER_SUP_SWORDMASTER:		// 
					{
						CItemElem *pItemElem = pMover->GetLWeaponItem();
#ifndef __RIGHTHAND_SKILL
						if( pItemElem && pItemElem->GetProp()->dwWeaponType != WT_MELEE_SWD )		// �޼տ� ���⸦ ��� �ְ� ��� �ִ� ���Ⱑ Į�� �ƴҶ�.
						{
							RemoveSkillInfluence( pNode );		// ����.
							continue;
						}
#endif // __RIGHTHAND_SKILL
					}
					if( pMover->GetActiveHandItemProp()->dwWeaponType != WT_MELEE_SWD )		// �����տ���� �ִ� ���Ⱑ Į�� �ƴҶ�.
					{
						RemoveSkillInfluence( pNode );		// ����.
						continue;
					}
					break;
				case SI_MER_SUP_SMITEAXE:		// 
				case SI_MER_SUP_AXEMASTER:		// 
					{
						CItemElem *pItemElem = pMover->GetLWeaponItem();
#ifndef __RIGHTHAND_SKILL						
						if( pItemElem && pItemElem->GetProp()->dwWeaponType != WT_MELEE_AXE )		// �޼տ� ���⸦ ��� �ְ� ��� �ִ� ���Ⱑ ������ �ƴҶ�.
						{
							RemoveSkillInfluence( pNode );		// ����.
							continue;
						}
#endif // __RIGHTHAND_SKILL
					}
					if( pMover->GetActiveHandItemProp()->dwWeaponType != WT_MELEE_AXE )		// �տ���� �ִ� ���Ⱑ ������ �ƴҶ�.
					{
						RemoveSkillInfluence( pNode );		// ����.
						continue;
					}
					break;
				case SI_BIL_PST_ASMODEUS:
					if( pMover->GetActiveHandItemProp()->dwWeaponType != WT_MELEE_KNUCKLE )		// �տ���� �ִ� ���Ⱑ ��Ŭ�� �ƴҶ�.
					{
						RemoveSkillInfluence( pNode );		// ����.
						continue;
					}
					break;
				} // switch
			} // BuffSkill
		} else	// pMover
		{
			Error( "CSkillInfluence::Process : pMover�� �� %d", pNode->wID );
		}
	#endif // __WORLDSERVER

	#ifdef __CLIENT
		if( pMover )	// �̰� NULL�϶��� �ִ� CWndWorld::m_SkillState ������ ������ ����.
		{
			if( pNode->wType == BUFF_ITEM
#if __VER >= 11 // __SYS_COLLECTING
				|| pNode->wType == BUFF_ITEM2
#endif	// __SYS_COLLECTING
				)
				pProp = prj.GetItemProp( pNode->wID );
			else if( pNode->wType == BUFF_SKILL )
				pProp = prj.GetSkillProp( pNode->wID );
			// if BUFF_PET, pProp = NULL

			if( pProp )
			{
				if( pNode->bEffect == FALSE )	// ����Ʈ �����ȵȰŸ�.
				{
					// ��� �����Ǵ� ����Ʈ�� dwSfxObj4�� ����Ѵ�.
					float fSkillTime = (float)pNode->tmCount / 1000.0f;		// nCount�� �ʴ����� ��ȯ.
					pNode->bEffect = TRUE;
					
					if( pProp->dwSfxObj4 != NULL_ID )
					{
					#if __VER >= 8 //__Y_FLAG_SKILL_BUFF
						if( (int)pNode->tmCount == 0 )
							CreateSfx( g_Neuz.m_pd3dDevice, pProp->dwSfxObj4, pMover->GetPos(), pMover->GetId(), D3DXVECTOR3( 0, 0, 0), pMover->GetId(), 0 );					
						else
					#endif //__Y_FLAG_SKILL_BUFF
							CreateSfx( g_Neuz.m_pd3dDevice, pProp->dwSfxObj4, pMover->GetPos(), pMover->GetId(), D3DXVECTOR3( 0, 0, 0), pMover->GetId(), fSkillTime );					
					}
				}
			}
		}
	#endif // CLIENT
		
		if( m_pMover && pNode->tmTime == 0 )		// Loading�ǰ� ���� SetDestParam�� ���� ���� �ȵƴ�.
		{
			if( pNode->wType == BUFF_SKILL )
			{
				ItemProp *pSkillProp = NULL;
				AddSkillProp *pAddSkillProp = NULL;
				BOOL bRet = m_pMover->GetSkillProp( &pSkillProp, &pAddSkillProp, 
					pNode->wID, pNode->dwLevel, "CSkillInfluence::Process" );
				if( bRet )
				{
					m_pMover->ApplyParam( m_pMover, pSkillProp, pAddSkillProp, FALSE, 0 );	// SetDestParam����. �̶��� Ŭ��� ���� ������ ����.
				}
			}
			else if( pNode->wType == BUFF_ITEM )
			{
				ItemProp *pItemProp = prj.GetItemProp( pNode->wID );
				if( pItemProp )
				{
					m_pMover->ApplyParam( m_pMover, pItemProp, NULL, FALSE, 0 );	// SetDestParam����. �̶��� Ŭ��� ���� ������ ����.
				}
			}
#if __VER >= 9	// __PET_0410
			else if( pNode->wType == BUFF_PET )
			{
				int nDstParam	= HIWORD( pNode->dwLevel );
				int nAdjParam	= LOWORD( pNode->dwLevel );
				m_pMover->SetDestParam( nDstParam, nAdjParam, NULL_CHGPARAM, FALSE );
			}
#endif	// __PET_0410

			pNode->tmTime = dwTimeCurrent;
		}

		BOOL bRemove = TRUE;
		if( pNode->wType == BUFF_ITEM )
		{
			ItemProp *pItemProp = prj.GetItemProp( pNode->wID );
			if( pItemProp && pItemProp->dwSkillTime == 999999999 )
				bRemove = FALSE;
		}
#ifdef __DST_GIFTBOX
		else if( pNode->wType == BUFF_EQUIP )
			bRemove = FALSE;
#endif // __DST_GIFTBOX
		
		if( m_pMover != NULL && pNode->wType == BUFF_SKILL )
		{
			ItemProp *pSkillProp = NULL;
			AddSkillProp *pAddSkillProp = NULL;
			BOOL bRet = m_pMover->GetSkillProp( &pSkillProp, &pAddSkillProp, 
				pNode->wID, pNode->dwLevel, "CSkillInfluence::Process" );

			if( pAddSkillProp )
			{
				if( pAddSkillProp->dwSkillTime > 0 )
					bRemove = TRUE;
				else
					bRemove = FALSE;
			}
		}
#if __VER >= 9	// __PET_0410
		if( pNode->wType == BUFF_PET )
			bRemove	= FALSE;
#endif	// __PET_0410

#if __VER >= 11 // __SYS_COLLECTING
		if( pNode->wType == BUFF_ITEM2 )
		{
			if( t >= pNode->dwLevel )
			{
#ifdef __WORLDSERVER
				ItemProp *pItemProp		= prj.GetItemProp( pNode->wID );
				if( pItemProp )
					g_dpDBClient.SendLogSMItemUse( "2", (CUser*)m_pMover, NULL, pItemProp );
#endif	// __WORLDSERVER
				RemoveSkillInfluence( pNode );
			}
		}
		else
#endif	// __SYS_COLLECTING
		if( bRemove && dwTimeCurrent - pNode->tmTime >= pNode->tmCount )	// tmCount�ð���ŭ ��������� ����.
		{
#if __VER >= 10
#ifdef __WORLDSERVER
			if( pNode->wType == BUFF_SKILL )
			{
				if( pNode->wID == SI_GEN_BURN )		// ȭ��
				{
					// 5000 ������ ����.
					ItemProp* pSkillProp	= prj.GetSkillProp( SI_GEN_BURN );
					int nMin	= pSkillProp->dwAbilityMin;
					int nMax	= pSkillProp->dwAbilityMax;
					int nDamage		= xRandom( nMin, nMax );
					m_pMover->SendDamage( AF_FORCE, pNode->dwAttackerID, nDamage );
					g_UserMng.AddCreateSfxObj( m_pMover, XI_SKILL_ELE_FIRE_BURINGFIELD02 );
				}
			}
			else if( pNode->wType == BUFF_ITEM )
			{
				if( pNode->wID == II_SYS_SYS_SCR_PET_FEED_POCKET02 )
				{
					ItemProp *pItemProp		= prj.GetItemProp( pNode->wID );
					if( pItemProp )
						g_dpDBClient.SendLogSMItemUse( "2", (CUser*)m_pMover, NULL, pItemProp );
				}
			}
#endif	// __WORLDSERVER
#endif	//
			RemoveSkillInfluence( pNode );
		}
	}
}

#endif // #if !( defined( __CORESERVER ) || defined( __DBSERVER ) ) 


#if !( defined( __CORESERVER ) || defined( __DBSERVER ) ) 
void CSkillInfluence::RemoveSkillInfluence( SKILLINFLUENCE *pSkillInfluence )
{
	if( m_pMover )
	{
		if( pSkillInfluence->wType == BUFF_ITEM		// �������� ����ȿ���� �ٵǾ� ����.
#if __VER >= 11 // __SYS_COLLECTING
			|| pSkillInfluence->wType == BUFF_ITEM2
#endif	// __SYS_COLLECTING
			)
		{
#ifdef __WORLDSERVER
			ItemProp *pItemProp = prj.GetItemProp( pSkillInfluence->wID );
			if( pItemProp )
			{
				m_pMover->ResetDestParam( pItemProp->dwDestParam1, pItemProp->nAdjParamVal1, TRUE );
				m_pMover->ResetDestParam( pItemProp->dwDestParam2, pItemProp->nAdjParamVal2, TRUE );
				if( 0 < strlen( pItemProp->szTextFileName ) && pItemProp->dwItemKind3 != IK3_ANGEL_BUFF )
				{
				#if __VER >= 8 //__Y_FLAG_SKILL_BUFF
					// �ش� ���ű��� �ߵ� ��ų�� �˻��Ͽ� ������Ų��..
					if( pItemProp->dwActiveSkill != NULL_ID )
					{
						RemoveSkillInfluence( BUFF_SKILL, pItemProp->dwActiveSkill );
					}
				#endif //__Y_FLAG_SKILL_BUFF
					char szGMCommand[64] = {0,};
					CString szGMText = pItemProp->szTextFileName;
					szGMText.Replace( '(', '\"' );
					szGMText.Replace( ')', '\"' ); 
					int nGMCount = szGMText.Find( "/", 1 );
					if( nGMCount != -1 )
						strncpy( szGMCommand, &pItemProp->szTextFileName[nGMCount], szGMText.GetLength() );
					else
						Error( "GMText �����ϴ� ���� ���� : %d, %s", pItemProp->dwID, pItemProp->szTextFileName );
					ParsingCommand( szGMCommand, (CMover*)m_pMover, TRUE );
				}
			}
#endif
		} 
		else if( pSkillInfluence->wType == BUFF_SKILL )
		{
			m_pMover->OnEndSkillState( pSkillInfluence->wID, pSkillInfluence->dwLevel );		// ��ų���°� �ϳ� ������ �ڵ鷯 ȣ��.
		}
#if __VER >= 9	// __PET_0410
#ifdef __WORLDSERVER
		else if( pSkillInfluence->wType == BUFF_PET )
		{
			int nDstParam	= HIWORD( pSkillInfluence->dwLevel );
			int nAdjParam	= LOWORD( pSkillInfluence->dwLevel );
			m_pMover->ResetDestParam( nDstParam, nAdjParam, TRUE );
		}
#endif	// __WORLDSERVER
#endif	// __PET_0410
	}
#ifdef __WORLDSERVER
	g_UserMng.AddRemoveSkillInfluence( m_pMover, pSkillInfluence->wType, pSkillInfluence->wID );
#endif // __WORLDSERVER
	Remove( pSkillInfluence );	// �̰� �� �������� �ҷ���� �Ѵ�. ������ pSkillInfl->wType �̷��� �����ϴϱ�.
	
}

// dwSkill�� ã�Ƽ� ����.
#ifdef __PVPDEBUFSKILL
BOOL	CSkillInfluence::RemoveSkillInfluenceFromID( OBJID dwAttackerID )
{
	int		i = MAX_SKILLINFLUENCE;
	SKILLINFLUENCE *pList = m_aSkillInfluence, *pNode;
	BOOL	bRet = FALSE;
	
	if( m_pMover == NULL )	return FALSE;
	
	while( i-- )
	{
		pNode = pList++;
		if( pNode->wType == BUFF_SKILL )
		{
			if( ((int)pNode->wID > 0) && (pNode->dwAttackerID == dwAttackerID) )	// ��ų �ִ°� ã��.
			{
				ItemProp		*pSkillProp;
				AddSkillProp	*pAddSkillProp;
				
				// ��ų ������Ƽ�� ������.
				m_pMover->GetSkillProp( &pSkillProp, &pAddSkillProp, pNode->wID, pNode->dwLevel, "RemoveAllSkillDebuff" );
				if( pSkillProp && pSkillProp->nEvildoing < 0 )
					RemoveSkillInfluence( pNode );
			}
		}
	} // loop
	
	return bRet;
}
#endif // __PVPDEBUFSKILL

BOOL	CSkillInfluence::RemoveSkillInfluence( WORD wType, WORD wID )
{
	SKILLINFLUENCE* pSkillInfluence;
	
	pSkillInfluence = Find( wType, wID );
	if( pSkillInfluence )
	{
		RemoveSkillInfluence( pSkillInfluence );
		return TRUE;
	} else
	{
		return FALSE;
	}
}

// dwChrState�� ���¸� ���� ��ų�� ã�� ��� ������
BOOL	CSkillInfluence::RemoveSkillInfluenceState( DWORD dwChrState )
{
	int		i = MAX_SKILLINFLUENCE;
	SKILLINFLUENCE *pList = m_aSkillInfluence, *pNode;
	BOOL	bRet = FALSE;
	
	if( m_pMover == NULL )	return FALSE;

	int	j;
				
	while( i-- )
	{
		pNode = pList++;
		if( pNode->wType == BUFF_SKILL )
		{
			if( (int)pNode->wID > 0 )	// ��ų �ִ°� ã��.
			{
				ItemProp		*pSkillProp;
				AddSkillProp	*pAddSkillProp;
				
				// ��ų ������Ƽ�� ������.
				m_pMover->GetSkillProp( &pSkillProp, &pAddSkillProp, pNode->wID, pNode->dwLevel, "RemoveSkillInfluence" );
				for( j = 0; j < 2; j ++ )
				{
					if( pAddSkillProp->dwDestParam[j] == DST_CHRSTATE )		// DestParam�� �˻�.
					{
						if( pAddSkillProp->nAdjParamVal[j] & dwChrState )	// dwChrState ���¸� ������ �ִ� ��ų�̸�
						{
							RemoveSkillInfluence( pNode );		// �̻��� ����
							bRet = TRUE;
							continue;
						}
					} 

				}
			}
		} else
		if( pNode->wType == BUFF_ITEM
#if __VER >= 11 // __SYS_COLLECTING
			|| pNode->wType == BUFF_ITEM2
#endif	// __SYS_COLLECTING
			)
		{
			if( pNode->wID > 0 )
			{
				ItemProp *pItemProp = prj.GetItemProp( pNode->wID );
				if( pItemProp )
				{
//#ifdef __PROP_0827	// ApplyParam���� ���� 2���� ��������Ƿ�, 3�� �ƴϴ�.
//					for( j = 0; j < 3; j ++ )
//#else	// __PROP_0827
					for( j = 0; j < 2; j ++ )
//#endif	// __PROP_0827
					{
						if( pItemProp->dwDestParam[j] == DST_CHRSTATE )		// DestParam�� �˻�.
						{
							if( pItemProp->nAdjParamVal[j] & dwChrState )	// dwChrState ���¸� ������ �ִ� ��ų�̸�
							{
								RemoveSkillInfluence( pNode );		// �̻��� ����
								bRet = TRUE;
								continue;
							}
						} 
					}
				}
			}
		}
	} // loop
	
	return bRet;
}

#if __VER >= 11 // __MA_VER11_06				// Ȯ����ų ȿ������ world,neuz
// dwDestParam ���¸� ���� ��ų�� ã�� ��� ������
BOOL	CSkillInfluence::RemoveSkillInfluenceDestParam( DWORD dwDestParam )
{
	int		i = MAX_SKILLINFLUENCE;
	SKILLINFLUENCE *pList = m_aSkillInfluence, *pNode;
	BOOL	bRet = FALSE;
	
	if( m_pMover == NULL )	return FALSE;

	int	j;
				
	while( i-- )
	{
		pNode = pList++;
		if( pNode->wType == BUFF_SKILL )
		{
			if( (int)pNode->wID > 0 )	// ��ų �ִ°� ã��.
			{
				ItemProp		*pSkillProp;
				AddSkillProp	*pAddSkillProp;
				
				// ��ų ������Ƽ�� ������.
				m_pMover->GetSkillProp( &pSkillProp, &pAddSkillProp, pNode->wID, pNode->dwLevel, "RemoveSkillInfluence" );
				for( j = 0; j < 2; j ++ )
				{
					// ����� ���۸��� �������� ����Ѵ�.
					// ���� ���� ����� ���, �Ķ���� nEvildoing�� �����Ѵ�.
					if( pSkillProp->nEvildoing < 0 && pAddSkillProp->dwDestParam[j] == dwDestParam )		// DestParam�� �˻�.
					{
						RemoveSkillInfluence( pNode );		// �̻��� ����
						bRet = TRUE;
						continue;
					} 

				}
			}
		}
	} // loop
	return bRet;
}
#endif // __MA_VER11_06				// Ȯ����ų ȿ������ world,neuz

// dwChrState�� ���¸� ���� ��ų�� ã�� ��� ������
BOOL	CSkillInfluence::RemoveAllSkillDebuff( void )
{
	int		i = MAX_SKILLINFLUENCE;
	SKILLINFLUENCE *pList = m_aSkillInfluence, *pNode;
	BOOL	bRet = FALSE;
	
	if( m_pMover == NULL )	return FALSE;
	
	while( i-- )
	{
		pNode = pList++;
		if( pNode->wType == BUFF_SKILL )
		{
			if( (int)pNode->wID > 0 )	// ��ų �ִ°� ã��.
			{
				ItemProp		*pSkillProp;
				AddSkillProp	*pAddSkillProp;
				
				// ��ų ������Ƽ�� ������.
				m_pMover->GetSkillProp( &pSkillProp, &pAddSkillProp, pNode->wID, pNode->dwLevel, "RemoveAllSkillDebuff" );
				if( pSkillProp && pSkillProp->nEvildoing < 0 )
					RemoveSkillInfluence( pNode );
			}
		} else
		if( pNode->wType == BUFF_ITEM
#if __VER >= 11 // __SYS_COLLECTING
			|| pNode->wType == BUFF_ITEM2
#endif	// __SYS_COLLECTING
			)
		{
			if( pNode->wID > 0 )
			{
				ItemProp *pItemProp = prj.GetItemProp( pNode->wID );
				if( pItemProp && pItemProp->nEvildoing < 0 )
					RemoveSkillInfluence( pNode );
			}
		}
	} // loop
	
	return bRet;
}

// ������ų���� ã�� ��� ����.
BOOL	CSkillInfluence::RemoveAllSkillBuff( void )
{
	int		i = MAX_SKILLINFLUENCE;
	SKILLINFLUENCE *pList = m_aSkillInfluence, *pNode;
	BOOL	bRet = FALSE;
	
	if( m_pMover == NULL )	return FALSE;
	
	while( i-- )
	{
		pNode = pList++;
		if( pNode->wType == BUFF_SKILL )
		{
			if( (int)pNode->wID > 0 )	// ��ų �ִ°� ã��.
			{
				ItemProp		*pSkillProp;
				AddSkillProp	*pAddSkillProp;
				
				// ��ų ������Ƽ�� ������.
				m_pMover->GetSkillProp( &pSkillProp, &pAddSkillProp, pNode->wID, pNode->dwLevel, "RemoveAllSkillBuff" );
				if( pSkillProp && pSkillProp->nEvildoing >= 0 )
				{
					RemoveSkillInfluence( pNode );
				}
			}
		}
/*		else
		if( pNode->wType == BUFF_ITEM )
		{
			if( pNode->wID > 0 )
			{
				ItemProp *pItemProp = prj.GetItemProp( pNode->wID );
				if( pItemProp && pItemProp->nEvildoing < 0 )
					RemoveSkillInfluence( pNode );
			}
		} */
	} // loop
	
	return bRet;
	
}

#if __VER >= 11 // __MA_VER11_05	// �ɸ��� ���� �ŷ� ��� world,database,neuz
// ���������� ã�� ��� ����.
BOOL	CSkillInfluence::RemoveAllBuff( void )
{
	int		i = MAX_SKILLINFLUENCE;
	SKILLINFLUENCE *pList = m_aSkillInfluence, *pNode;
	BOOL	bRet = FALSE;
	
	if( m_pMover == NULL )	return FALSE;
	
	while( i-- )
	{
		pNode = pList++;

		if( pNode->wID > 0 )
			RemoveSkillInfluence( pNode );
	} // loop
	
	return bRet;
}
#endif // __MA_VER11_05	// �ɸ��� ���� �ŷ� ��� world,database,neuz


// ������ų(Evildoing >= 0)�ϳ��� ����.
BOOL CSkillInfluence::RemoveOneSkillBuff( void )
{
	int		i = MAX_SKILLINFLUENCE;
	SKILLINFLUENCE *pList = m_aSkillInfluence, *pNode;
	
	if( m_pMover == NULL )	return FALSE;
	
	while( i-- )
	{
		pNode = pList++;
		if( pNode->wType == BUFF_SKILL )
		{
			if( (int)pNode->wID > 0 )	// ��ų �ִ°� ã��.
			{
				ItemProp		*pSkillProp;
				AddSkillProp	*pAddSkillProp;
				
				// ��ų ������Ƽ�� ������.
				m_pMover->GetSkillProp( &pSkillProp, &pAddSkillProp, pNode->wID, pNode->dwLevel, "RemoveAllSkillBuff" );
				if( pSkillProp && pSkillProp->nEvildoing >= 0 )
				{
					RemoveSkillInfluence( pNode );
					return TRUE;
				}
			}
		}
/*		else
		if( pNode->wType == BUFF_ITEM )
		{
			if( pNode->wID > 0 )
			{
				ItemProp *pItemProp = prj.GetItemProp( pNode->wID );
				if( pItemProp && pItemProp->nEvildoing < 0 )
					RemoveSkillInfluence( pNode );
			}
		} */
	} // loop
	
	return FALSE;
}

#if __VER >= 9	// __PET_0410
BOOL CSkillInfluence::RemovePet( void )
{
	SKILLINFLUENCE*	pSkillInfluence	= FindPet();
	if( pSkillInfluence )
	{
		RemoveSkillInfluence( pSkillInfluence );
#if __VER >= 12 // __PET_0519
		CItemElem* pItem	= m_pMover->GetPetItem();
		if( pItem )
			m_pMover->ResetDestParamRandomOptExtension( pItem );
#endif	// __PET_0519
		return TRUE;
	}
	return FALSE;
}
#endif	// __PET_0410

#endif // #if !( defined( __CORESERVER ) || defined( __DBSERVER ) )

#endif	// __BUFF_1107