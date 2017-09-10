#include "stdafx.h"
#include "misc.h"
#include "CooltimeMgr.h"
#include "ProjectCmn.h"

CCooltimeMgr::CCooltimeMgr()
{
	ZeroMemory( m_times, sizeof(m_times) );
	ZeroMemory( m_bases, sizeof(m_bases) );
}
	
CCooltimeMgr::~CCooltimeMgr()
{
}

// ��Ÿ�� �������� ��Ÿ�� �׷��ȣ�� ��´�. 
// ���� �׷��� �������� �ѹ� ����ϸ� ��� ���� ��Ÿ���� ����޴´�.
// ����: �����Ϸ��� ������Ͽ� MAX_COOLTIME_TYPE�� �� ������ �� 
DWORD CCooltimeMgr::GetGroup( ItemProp* pItemProp )
{
	DWORD dwCooltimeItemGroup = 0;

	if( pItemProp->dwSkillReady > 0 )
	{
		switch ( pItemProp->dwItemKind2 )
		{
		case IK2_FOOD:
			dwCooltimeItemGroup = 1;
			if( pItemProp->dwItemKind3 == IK3_PILL )
				dwCooltimeItemGroup = 2;
			break;
		case IK2_SKILL:
			dwCooltimeItemGroup = 3;
			break;
//		case IK2_POTION:
//			dwCooltimeItemGroup = 3;
//			break;
		}

	}

	return dwCooltimeItemGroup;
}


// dwGroup�� ����� �� �ִ°�?
BOOL CCooltimeMgr::CanUse( DWORD dwGroup )
{
	ASSERT( dwGroup > 0 );
	return g_tmCurrent > GetTime( dwGroup );		// ���� �ð��� ������ ��밡�� 
}

// ����� �ð��� ����ؼ�, CanUse���� �Ǵ� �ٰŷ� ����Ѵ�.
void CCooltimeMgr::SetTime( DWORD dwGroup, DWORD dwCoolTime )
{
	ASSERT( dwGroup > 0 && dwCoolTime > 0 );
	DWORD dwTick = g_tmCurrent;

	m_times[dwGroup - 1] = dwTick + dwCoolTime;
	m_bases[dwGroup - 1] = dwTick;
}	


