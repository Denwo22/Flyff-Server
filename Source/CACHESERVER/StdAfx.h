#pragma once

#define WIN32_LEAN_AND_MEAN		
#define _USE_32BIT_TIME_T
#define __CACHESERVER
#define POINTER_64 __ptr64
#define MASSIVE

#include <afxwin.h>

#include <d3dx9math.h>
#include <mmsystem.h>

#pragma warning(disable:4786)
#include <map>
#include <list>
#include <vector>

#include "VersionCommon.h"
#include "memtrace.h"
#include "DefineCommon.h"
#include "CmnHdr.h"
#include <afxdisp.h>     
#include "vutil.h"
#include "DXUtil.h"
#include "Data.h"
#include "File.h"
#include "Scanner.h"
#include "Debug.h"
#include "Timer.h"
#include "d3dfont.h"
#include "xutil.h"

using namespace	std;