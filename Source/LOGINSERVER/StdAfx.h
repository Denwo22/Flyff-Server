#pragma once

#define WIN32_LEAN_AND_MEAN	
#define _USE_32BIT_TIME_T
#define POINTER_64 __ptr64
#define __MASSIVE

#include <afxwin.h>

#include <mmsystem.h>

#pragma warning(disable:4786)
#include <map>
#include <list>
#include <vector>
using	namespace	std;


#include "VersionCommon.h"

#include "memtrace.h"

#include "DefineCommon.h"
#include "CmnHdr.h"

#include <afxdisp.h>      

#include "DXUtil.h"
#include "Data.h"
#include "file.h"
#include "Scanner.h"
#include "Timer.h"
#include "d3dfont.h"
#include "vutil.h"

#include "debug.h"
#include "xutil.h"
#include "tickcount.h"

extern	CTickCount	g_TickCount;
extern	char	g_lpCoreAddr[16];
extern	char	g_lpDBAddr[16];
extern  BOOL	g_bNProtectAuth;
extern	char	g_szMSG_VER[256];