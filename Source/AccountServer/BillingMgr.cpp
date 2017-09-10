#include "stdafx.h"
#include "BillingMgr.h"
#include "BillingMgrJP.h"
#include "BillingMgrJP2.h"
#include "BillingMgrTW.h"
#include "BillingMgrTH.h"
#include "query.h"

///////////////////////////////////////////////////////////////////////
// global variable
///////////////////////////////////////////////////////////////////////

static			CBillingMgr* g_pBillingMgr = NULL;	
static char		g_szBillingPWD[256];

///////////////////////////////////////////////////////////////////////
// global function
///////////////////////////////////////////////////////////////////////

// strcpy�� ������� ������, NULL������ �����͸� �����Ѵ�.
char* StrCpyExcludeNull( char* dst, const char* src )
{
	char *cp = dst;

	while( *cp++ = *src++ )
		   ; 
	return( cp - 1 );
}

// pCur���� pEnd���� ����(0x20)�� ���δ�.
void AppendSpace( char* pCur, char* pEnd )
{
	if( pEnd - pCur )
		memset( pCur, 0x20, pEnd - pCur );
}

char* GetBillingPWD()
{
	return g_szBillingPWD;
}

// TODO: LoadLibrary
BOOL CreateBillingMgr()
{
#ifdef __BILLING2_041021						// ����� ���� 
	#if defined(__BILLING_TW)					
		g_pBillingMgr = new CBillingMgrTW;		// �븸 	
	#elif defined(__BILLING_JP)
		g_pBillingMgr = new CBillingMgrJP2;		// �Ϻ� 	
	#else
		#error BILLING TARGET MUST BE DEFINED.	// ������ ���󺰷� �������� �� �־���Ѵ�.
	#endif
#else
	#if defined(__BILLING_TH)
		g_pBillingMgr = new CBillingMgrTH;		// �±� 
	#elif defined(__BILLING_JP)
		g_pBillingMgr = new CBillingMgrJP;	
	#endif
#endif

	return TRUE;
}

CBillingMgr* GetBillingMgr()
{
	ASSERT( g_pBillingMgr );
	return g_pBillingMgr;
}
