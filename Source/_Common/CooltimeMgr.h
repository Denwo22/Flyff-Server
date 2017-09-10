#ifndef	__COOLTIMEMGR_H__
#define __COOLTIMEMGR_H__

const int MAX_COOLTIME_TYPE = 3;

struct ItemProp;

// ��Ÿ�� ������ �����ϴ� Ŭ���� 
class CCooltimeMgr 
{
public:
	CCooltimeMgr();	
	virtual ~CCooltimeMgr();	

	static DWORD		GetGroup( ItemProp* pItemProp );

	BOOL				CanUse( DWORD dwGroup );
	void				SetTime( DWORD dwGroup, DWORD dwCoolTime );	
	DWORD				GetTime( DWORD dwGroup ) 
	{ 
		ASSERT( dwGroup > 0 );
		return m_times[ dwGroup - 1];
	}

	DWORD				GetBase( DWORD dwGroup ) 
	{ 
		ASSERT( dwGroup > 0 );
		return m_bases[ dwGroup - 1];
	}

protected:
	DWORD				m_times[MAX_COOLTIME_TYPE];		// ���� ��밡�� �ð� 	
	DWORD				m_bases[MAX_COOLTIME_TYPE];		// �̺�Ʈ �߻� �ð� ( �������� ���� �ð� ) 
};

#endif	__COOLTIMEMGR_H__