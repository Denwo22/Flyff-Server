#pragma once

#define WIN32_LEAN_AND_MEAN		
#define POINTER_64 __ptr64
#define _USE_32BIT_TIME_T
#define __ACCOUNT

#include <afxwin.h>

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <mmsystem.h>
#include <dplay.h>
#include <d3dx9tex.h>

#pragma warning(disable:4786)
#include <map>
#include <set>
#include <list>
#include <vector>
using	namespace	std;

#include "VersionCommon.h"

#include "memtrace.h"

#include "DefineCommon.h"
#include "cmnhdr.h"

#include <afxdisp.h>        

#include "dxutil.h"
#include "vutil.h"
#include "data.h"
#include "file.h"
#include "d3dfont.h"
#include "scanner.h"
#include "timer.h"
#include "debug.h"
#include "xutil.h"
#include "buyinginfo.h"

const UINT	IDT_SENDPLAYERCOUNT	= 0;
const UINT	IDT_RECONNECT		= 1;
const UINT  IDT_KEEPALIVE		= 2;
const UINT  IDT_KICKOUT			= 3;
const UINT  IDT_PREVENT_EXCESS	= 4;
const UINT	IDT_BUYING_INFO		= 5;
const UINT  IDT_RELOAD_PROJECT	= 6;
const UINT  IDT_TIME_CHECKADDR	= 7;


const UINT		WM_CONNENT_BILLING = WM_USER + 100;

extern char		DSN_NAME_LOG[];
extern char		DB_ADMIN_ID_LOG[];

extern char		DSN_NAME_LOGIN[];
extern char		DB_ADMIN_ID_LOGIN[];

extern char		DSN_NAME_BILLING[];
extern char		DB_ADMIN_ID_BILLING[];

