#ifndef __WANTEDLIST__H
#define __WANTEDLIST__H

#include "misc.h"		// WANTED_ENTRY
#include "ar.h"

typedef std::map<u_long, WANTED_ENTRY *>	WANTED_ENTRY_LIST;


// ����� ����Ʈ��ü 
class CWantedList  
{
public:
	virtual ~CWantedList();

protected:
	WANTED_ENTRY_LIST		m_wantedList;
	CMclCritSec				m_AccessLock;
	__int64					m_nMinGold;			// ǥ�� ����Ʈ�� �ּ� ����� 

public:
	static CWantedList&		GetInstance();
	void					Write( CAr& ar );
	void					Read( CAr& ar );
	BOOL					SetEntry( u_long idPlayer, LPCTSTR szPlayer, int nGold, LPCTSTR szMsg );
	__int64					GetReward( u_long idPlayer, LPTSTR lpszReward, BOOL& bUpdatable );
	void					Rename( u_long idPlayer, const char* lpszPlayer );

protected:
	CWantedList();
	BOOL					IsUpdatable( __int64 nGold );
};


#endif // __WANTEDLIST__H
