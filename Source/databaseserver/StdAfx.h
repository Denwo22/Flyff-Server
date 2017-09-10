#pragma once

#define WIN32_LEAN_AND_MEAN	
#define _USE_32BIT_TIME_T
#define POINTER_64 __ptr64
#define __DBSERVER


#include <afx.h>
#include <afxwin.h>
#include <afxext.h>         
#include <afxdisp.h>       
#include <afxdtctl.h>		
#include <afxcmn.h>			

#include "VersionCommon.h"

#include "memtrace.h"

#include "DefineCommon.h"
#include "CmnHdr.h"

#include <map>
#include <set>
#include <string>
#include <vector>
using namespace std;

#include <assert.h>


#include "vutil.h"
#include "DXUtil.h"
#include <d3dx9math.h>
#include <mmsystem.h>
#include "xutil.h"
#include "path.h"
#include "Define.h"
#include "DefineNeuz.h"
#include "DefineJob.h"
#include "DefineWorld.h"
#include "resdata.h"
#include "Data.h"
#include "file.h"
#include "authorization.h"
#include "Scanner.h"
#include "ScriptDialog.h"
#include "Project.h"
#include "Timer.h"
#include "Debug.h"
#include "d3dfont.h"
#include "Query.h"
#include "DbManager.h"
#include "defineitemkind.h"

#include "buyinginfo.h"
#include "DefineConMode.h"

#include "memtrace.h"

extern	HWND	hMainWnd;
extern	HINSTANCE	hInst;

typedef	struct	AppInfo
{
	DWORD	dwSys;
	DWORD	dwId;		// server id
}	APP_INFO, *LPAPP_INFO;

extern	char	DSN_NAME_CHARACTER01[260];
extern	char	DB_ADMIN_ID_CHARACTER01[260];
extern	char	DSN_NAME_LOG[260];
extern	char	DB_ADMIN_ID_LOG[260];

#ifdef __S1108_BACK_END_SYSTEM
extern	char	DSN_NAME_BACKSYSTEM[260];
extern	char	DB_ADMIN_ID_BACKSYSTEM[260];
#endif // __S1108_BACK_END_SYSTEM

extern	char	DSN_NAME_ITEMUPDATE[260];
extern	char	DB_ADMIN_ID_ITEMUPDATE[260];
