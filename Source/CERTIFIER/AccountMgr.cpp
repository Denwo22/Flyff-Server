// AccountMgr.cpp: implementation of the CAccountMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AccountMgr.h"
#include "..\Resource\lang.h"

//////////////////////////////////////////////////////////////////////

UINT HashKey( const char* key )
{
	UINT nHash = 0;
	while( *key )
		nHash = (nHash<<5) + nHash + *key++;
	return nHash;
}

//////////////////////////////////////////////////////////////////////

CAccountMgr::CAccountMgr()
{
}

CAccountMgr::~CAccountMgr()
{
	list< ACCOUNT_CACHE* >::iterator it;
	for( it = m_cache.begin(); it != m_cache.end(); ++it )
		safe_delete( *it );
}

//////////////////////////////////////////////////////////////////////

ACCOUNT_CHECK CAccountMgr::Check( DWORD dwIP )
{
	time_t	tmCur = time( NULL );

	// ĳ������ ã�´�.
	list< ACCOUNT_CACHE* >::iterator it;
	for( it = m_cache.begin(); it != m_cache.end(); ++it )
	{
		ACCOUNT_CACHE* pInfo = *it;
		if( pInfo->m_dwIP == dwIP )
		{
			ACCOUNT_CHECK result = CHECK_OK;

			if( pInfo->m_nError >= 3 )
			{
				long nSec = tmCur - pInfo->m_tmError;
				if( nSec <= (15 * 60) )
					result = CHECK_3TIMES_ERROR;
			}
			else if( pInfo->m_nError >= 1 && ::GetLanguage() != LANG_KOR )
			{
				long nSec = tmCur - pInfo->m_tmError;
				if( nSec <= 15 )
					result = CHECK_1TIMES_ERROR;
			}

			m_cache.erase( it );
			m_cache.push_front( pInfo );		// ���� ���� ������ 
			return result;
		}
	}
	
	// MAX���� ũ�� - LRU�� ���� 
	//     or       - new�� �ִ´�. 
	ACCOUNT_CACHE* pInfo = NULL; 
	if( m_cache.size() < 3 )
		pInfo = new ACCOUNT_CACHE;
	else
	{
		pInfo = m_cache.back();					// ���� �� ���� ���� �� �ڿ� ���� �ִ�.
		m_cache.pop_back();
	}

	pInfo->m_nError  = 0;
	pInfo->m_dwIP    = dwIP;
	pInfo->m_tmError = tmCur;

	m_cache.push_front( pInfo );

	return CHECK_OK;
}


void CAccountMgr::SetError( int nError )
{
	ACCOUNT_CACHE* pInfo = m_cache.front();		// MRU
	
	if( nError > 0 )
		++pInfo->m_nError; 
	else
		pInfo->m_nError = 0; 

	pInfo->m_tmError = time( NULL );
}

/*
int main(int argc, char* argv[])
{

	CAccountMgr mgr;
	ACCOUNT_CHECK check = mgr.Check("tester");
	if( check == CHECK_OK )
	{
		mgr.SetError( 1 );
		check = mgr.Check("tester");
		assert( check == CHECK_1TIMES_ERROR );

		Sleep( 1000 * 16 );

		check = mgr.Check("tester");
		assert( check == CHECK_OK );

		mgr.SetError( 1 );
		mgr.SetError( 1 );
		mgr.SetError( 1 );

		check = mgr.Check("tester");
		assert( check == CHECK_3TIMES_ERROR );

		Sleep( 1000 * 60 * 15 + 1000 );

		check = mgr.Check("tester");
		assert( check == CHECK_OK );
	}

	return 0;
}

*/