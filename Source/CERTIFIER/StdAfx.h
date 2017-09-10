#pragma once

#define WIN32_LEAN_AND_MEAN	
#define _USE_32BIT_TIME_T
#define POINTER_64 __ptr64
#define __CERTIFIER

#include <afxwin.h>
#include <mmsystem.h>

#pragma warning(disable:4786)
#include <list>
#include <map>
#include <vector>
using namespace std;

#include "VersionCommon.h"

#include "memtrace.h"

#include "DefineCommon.h"
#include "CmnHdr.h"
#include "DXUtil.h"
#include "data.h"
#include "Query.h"
#include "debug.h"
#include "file.h"
#include "scanner.h"
#include "d3dfont.h"
#include "timer.h"
#include "xutil.h"
#include "vutil.h"

#include <afxdisp.h>      

extern	BOOL	g_fInternal;

#include "../_Common/Rijndael.h"
