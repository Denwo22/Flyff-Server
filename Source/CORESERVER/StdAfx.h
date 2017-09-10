#pragma once

#define WIN32_LEAN_AND_MEAN		
#define _USE_32BIT_TIME_T
#define POINTER_64 __ptr64
#define __CORESERVER

#include <afxwin.h>

#include <mmsystem.h>
#include <d3dx9math.h>

#pragma warning(disable:4786)
#include <map>
#include <list>
#include <vector>
#include <string>
#include <set>

using	namespace	std;

#include <DPlay.h>
#include "VersionCommon.h"

#include "memtrace.h"

#include "DefineCommon.h"
#include "CmnHdr.h"
#include "chassert.h"
#include "DXUtil.h"
#include "Data.h"
#include "file.h"
#include "vutil.h"
#include "Scanner.h"
#include "projectcmn.h"
#include "Timer.h"
#include "d3dfont.h"
#include "DefineJob.h"
#include "DefineNeuz.h"
#include "DefineObj.h"
#include "authorization.h"
#include "debug.h"
#include "xutil.h"
#include "tickcount.h"
#include "buyinginfo.h"

#include <afxdisp.h>   

extern	CTickCount	g_TickCount;
extern	CGameTimer	g_GameTimer;
extern	float		s_fPartyExpRate;