#pragma once

#define WIN32_LEAN_AND_MEAN		
#define _USE_32BIT_TIME_T
#define POINTER_64 __ptr64
#define __WORLDSERVER

#pragma warning(disable:4786)
#pragma comment(linker, "/subsystem:Windows,5")
#pragma comment(linker, "/version:5")

#include <afxwin.h>

#include <AFXTEMPL.H>
#include <process.h>
#include <basetsd.h> 

#include <vector>
#include <string>
#include <map>
#include <queue>
#include <set>
using namespace std;

#include <afxdisp.h>        // MFC Automation classes

#include <d3d9.h>
#include <dxerr9.h>
#include <dshow.h>
#include <strmif.h>
#include <control.h>
#include <d3dx9math.h>

#include "VersionCommon.h"
#include "memtrace.h"
#ifdef __MEM_TRACE
#ifndef _DEBUG
#define	new	MT_NEW
#endif	// _DEBUG
#endif	// __MEM_TRACE
#include "DefineCommon.h"
#include "CmnHdr.h"
#include "chassert.h"
#include "Authorization.h"
#include "File.h"
#include "MoverMsg.h"

#include "d3dfile.h"
#include "d3dfont.h"
#include "d3dutil.h"
#include "dxutil.h"
#include "vutil.h"
#include "dsutil.h"

#include "debug.h"
#include "Define.h"
#include "DefineAttribute.h"
#include "DefineItemKind.h"
#include "DefineJob.h"
#include "DefineWorld.h"

#include "Data.h"
#include "Timer.h"
#include "patch.h"
#include "Path.h"
#include "Scanner.h"
#include "ScriptDialog.h"
#include "Script.h"
#include "Project.h"
#include "landscape.h"
#include "XUtil.h"
#include "XUtil3d.h"

// Model
#include "Model.h"
#include "Bone.h"
#include "Object3d.h"
#include "ModelObject.h"
#include "ModelGlobal.h"
#include "ModelMng.h"

// object
#include "Obj.h"
#include "Ctrl.h"
#include "Item.h"
#include "Mover.h"  
#include "Region.h"
#include "Weather.h"
#include "World.h"

// ai
#include "..\_AIInterface\AIInterface.h"
#include "..\_AIInterface\AIMonster.h"
#include "..\_AIInterface\Action.h"
#include "..\_AIInterface\ActionMover.h"

#include "CreateObj.h"
#include "tickcount.h"
#include "flyffevent.h"
#include "buyinginfo.h"

extern	CProject	prj;
extern	CTickCount	g_TickCount;
extern	u_long		g_uIdofMulti;
extern	CGameTimer	g_GameTimer;
extern	CFlyffEvent	g_eLocal;
extern ADDSMMODE	g_AddSMMode;
extern	ULONG		g_uKey;
extern DWORD		g_tmCurrent;		// 전역으로 사용할 현재 시간.
extern int			g_nProcessCnt;	// 프로세스마다 하나씩 카운트 되는 변수.
extern int			g_nRenderCnt;	// 렌더화면때마다 하나씩 카운트 되는 변수

#if __VER >= 11 // __GUILD_COMBAT_1TO1
#include "GuildCombat1to1.h"
extern CGuildCombat1to1Mng	g_GuildCombat1to1Mng;
#endif // __GUILD_COMBAT_1TO1

