#include "stdafx.h"
#include "WantedList.h"
#include <algorithm>
#include "dpdatabaseclient.h"

extern	CDPDatabaseClient	g_dpDatabaseClient;

//////////////////////////////////////////////////////////////////////
// file global
//////////////////////////////////////////////////////////////////////

// 30������ ��¥�� ���Ѵ�. 
static long CalcEnd()
{
	CTime t = CTime::GetCurrentTime();
	CTimeSpan delta1( 0, t.GetHour(), t.GetMinute(), t.GetSecond() );
	t -= delta1;

	CTimeSpan delta2( 30, 23, 59, 30 );		
	t += delta2;

	return	( (long)( t.GetTime() ) );
}

struct wantedComparison 
{
   bool operator () (const WANTED_ENTRY * left, const WANTED_ENTRY * right) const
   { 
	   return left->nGold > right->nGold; 
   }
};

//////////////////////////////////////////////////////////////////////
// CWantedList
//////////////////////////////////////////////////////////////////////
CWantedList& CWantedList::GetInstance()
{
	static CWantedList wantedList;
	return wantedList;
}


CWantedList::CWantedList()
{
	m_nMinGold = 0;
}

CWantedList::~CWantedList()
{
	WANTED_ENTRY_LIST::iterator it;
	for( it = m_wantedList.begin(); it != m_wantedList.end(); ++it )
		safe_delete( it->second );
}

void CWantedList::Read( CAr& ar )
{
	CMclAutoLock Lock( m_AccessLock );

	m_wantedList.clear();

	int nCount;				

	ar >> nCount;						
	for( int i=0; i<nCount; i++)
	{
		WANTED_ENTRY *pEntry = new WANTED_ENTRY;
		u_long idPlayer;
		
		ar >> idPlayer;
		ar.ReadString( pEntry->szPlayer, 64 );
		ar >> pEntry->nGold;
		ar >> pEntry->nEnd;
		ar.ReadString( pEntry->szMsg, WANTED_MSG_MAX + 1 );

		m_wantedList.insert( make_pair(idPlayer, pEntry) );
	}
}

void CWantedList::Write( CAr& ar )
{
	CMclAutoLock Lock( m_AccessLock );

	vector<WANTED_ENTRY *> array;
	array.reserve( 100 );
	long lCurrent = time( NULL );

	WANTED_ENTRY_LIST::iterator it;
	for( it = m_wantedList.begin(); it != m_wantedList.end();  )
	{
		WANTED_ENTRY* pEntry = it->second;
		if( lCurrent > pEntry->nEnd )
		{
			safe_delete( pEntry );
			m_wantedList.erase( it++ );
		}
		else
		{
			array.push_back( pEntry );
			++it;
		}
	}

	std::sort( array.begin(), array.end(), wantedComparison() );
	int nCount = array.size();
	nCount = min( 100, nCount );

	ar << lCurrent;
	ar << nCount;
	for( int i=0; i<nCount; i++ )
	{
		ar.WriteString( array[i]->szPlayer );
		ar << array[i]->nGold;
		ar << array[i]->nEnd; 
		ar.WriteString( array[i]->szMsg );
	}

	if( nCount >= 1 )
	{
		m_nMinGold = array[nCount-1]->nGold;
		TRACE("set m_nMinGold = %d\n", m_nMinGold );
	}
}

BOOL CWantedList::SetEntry( u_long idPlayer, LPCTSTR szPlayer, int nGold, LPCTSTR szMsg )
{
	CMclAutoLock Lock( m_AccessLock );

	__int64 nResult;
	long nEnd = CalcEnd();

	WANTED_ENTRY* pEntry = NULL; 
	WANTED_ENTRY_LIST::iterator it = m_wantedList.find( idPlayer );
	if( it != m_wantedList.end() )
	{
		pEntry = it->second; 
		pEntry->nEnd = nEnd;
		
		if( nGold >= (0.30f * pEntry->nGold) )	
			strcpy( pEntry->szMsg, szMsg );		// �� ���� �ݾ��� 30%�̻��� �� ������ 10������ ���� �ȴ�.
			
		pEntry->nGold += nGold;
	}
	else
	{
		pEntry = new WANTED_ENTRY;

		strcpy( pEntry->szPlayer, szPlayer );
		pEntry->nGold = nGold;
		pEntry->nEnd = nEnd;
		strcpy( pEntry->szMsg, szMsg );
		
		m_wantedList.insert( make_pair(idPlayer, pEntry) );
	}
	nResult = pEntry->nGold;
	g_dpDatabaseClient.SendCTWanted( CT_WANTED_SET, idPlayer, nGold, nEnd, szMsg );	// DBó�� 

	return IsUpdatable( nResult );
}

__int64	CWantedList::GetReward( u_long idPlayer, LPTSTR lpszReward, BOOL& bUpdatable )
{
	CMclAutoLock Lock( m_AccessLock );

	__int64 nGold = 0;

	WANTED_ENTRY_LIST::iterator it = m_wantedList.find( idPlayer );
	if( it != m_wantedList.end() )
	{
		WANTED_ENTRY* pEntry = it->second; 
		long lCurrent = time( NULL );
		if( pEntry->nEnd > lCurrent )		// ������� �ʾ�����, �ش� ����� ���� 
		{
			nGold = pEntry->nGold;
			strcpy( lpszReward, pEntry->szPlayer );
		}

		safe_delete( pEntry );
		m_wantedList.erase( it );

		g_dpDatabaseClient.SendCTWanted( CT_WANTED_SET, idPlayer, 0, 0, "" );			// DBó��
	}

	bUpdatable = IsUpdatable( nGold );
	return nGold;
}


BOOL CWantedList::IsUpdatable( __int64 nGold )
{
	return nGold >= m_nMinGold;
}

void CWantedList::Rename( u_long idPlayer, const char* lpszPlayer )
{
	WANTED_ENTRY_LIST::iterator i	= m_wantedList.find( idPlayer );
	if( i != m_wantedList.end() )
	{
		WANTED_ENTRY* pEntry	= i->second;
		lstrcpy( pEntry->szPlayer, lpszPlayer );
	}
}

