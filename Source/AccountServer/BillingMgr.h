#ifndef __BILLINGMGR_H__
#define	__BILLINGMGR_H__

const int	MAX_DPBILLING = 32;

enum BILLING_ENUM
{
	BID_FREEPASS,			// ���� ������ Ÿ�� ��ȣ (CAccount�� m_fCheck�� ���Ѵ�.) 
	BID_IP,					// �������� ������ 
	BID_PORT,				// �������� ��Ʈ 
};

class	CAccount;
class	CQuery;
struct	tagDB_OVERLAPPED_PLUS;

class CBillingMgr
{
public:
	virtual bool		Init( HWND hWnd ) = 0;
	virtual void		Release() = 0;
	virtual BYTE		CheckAccount( int nType, DWORD dwKey, const char* szAccount, const char* szAddr ) = 0;
	virtual BOOL		PreTranslateMessage( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ) = 0; 
	virtual BOOL		SetConfig( BILLING_ENUM id, DWORD data ) = 0;
	virtual void		OnTimer( CAccount* pAccount ) = 0;
	virtual void		OnDBQuery( CQuery& query, tagDB_OVERLAPPED_PLUS* pOV ) = 0;
	virtual void		NotifyLogout( LPCTSTR lpszAccount, DWORD dwSession ) = 0;
};

BOOL			CreateBillingMgr();
CBillingMgr*	GetBillingMgr();
char*			GetBillingPWD();
//BOOL			SetBillingPWD( unsigned char* pEncryptedPWD );

extern			char* StrCpyExcludeNull( char* dst, const char* src );
extern			void AppendSpace( char* pCur, char* pEnd );
#endif	// __BILLINGMGR_H__