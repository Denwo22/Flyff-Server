#pragma once 

#pragma warning(disable:4786)

#define WIN32_LEAN_AND_MEAN		
#define _USE_32BIT_TIME_T
#define POINTER_64 __ptr64
#define	__CLIENT
   
// Windows or Standard        
#include <afxwin.h>      
#include <AFXTEMPL.H>        // MFC Automation classes  
#include <winuser.h>    
#include <basetsd.h>  
#include <memory.h>  
#include <mmsystem.h>   
#include <string.h>   
#include <stdio.h>  
#include <tchar.h>  
#include <time.h> 
#include <stdlib.h>
#include <process.h>

#include <afxdisp.h>        // MFC Automation classes

// STL 
#include <vector>
#include <list>
#include <map>
#include <string> 
#include <queue>
using namespace std;


#define		__CLIENT
// exception handler���� 
#define		ADDR_ACCUMULATOR	"211.33.142.133"		

#include "NeuzVersion.h"  
#include "VersionCommon.h"
#include "DefineCommon.h"
#include "Resource.h"    
#include "authorization.h" 

// DirectX or 3D Core
#include <D3D9.h>
#include <dxerr9.h>
#include <dsound.h>
#include <dshow.h>

#include "memtrace.h"

#include "dxutil.h"
#include "d3denumeration.h" 
#include "d3dsettings.h"
#include "d3dapp.h" 
#include "vutil.h"
#include "d3dfont.h"
#include "d3dutil.h"
#include "DSUtil.h"

// Network
#include "CmnHdr.h"
#include "File.h"
#include "chassert.h"

// Resource
#include "misc.h"
#include "define.h"
#include "defineAttribute.h"
#include "defineItemKind.h"
#include "defineJob.h"
#include "defineWorld.h"
#include "WndStyle.h"

// MultiMedia
#include <atlbase.h> 

// Common
#include "Data.h"
#include "Timer.h"
#include "Scanner.h"
#include "Script.h"
#include "2DRender.h"
#include "SkyBox.h"
#include "patch.h"
#include "Project.h"
#include "path.h"
#include "landscape.h"
#include "xUtil3D.h"
#include "Graphic3D.h"
#include "Debug.h"
#include "Music.h"
#include "HwOption.h"
#include "Messenger.h"
#include "yUtil.h"

#if __VER >= 15 // __BS_CHANGING_ENVIR
#include "TexturePool.h"	//gmpbigsun
#endif	//__BS_CHANGING_ENVIR

// Model
#include "Model.h"
#include "Billboard.h"
#include "Material.h"
#include "Bone.h"
#include "Object3D.h"
#include "ModelObject.h"
#include "ModelMng.h"
#include "Particle.h"

// object
#include "Obj.h"
#include "Ctrl.h"
#include "Item.h"
#include "sfxbase.h"
#include "Mover.h"   
#include "Region.h"
#include "Weather.h"
#include "Camera.h"
#include "Light.h"
#include "World.h"
#include "WorldMng.h"

#include "CreateObj.h"
#include "authorization.h"

// Interface
#include "EditString.h"      // CString�� Ȯ���� CEditString
#include "ToolTip.h"         // ���� 
#include "ITheme.h"          // �������̽��� ����, ��Ų, ��Ʈ ������ ���� Ŭ���� 
#include "WndBase.h"         // ������ �⺻ Ŭ����, ��Ÿ ���� ��Ʈ�� (��ư, ��ũ�ѹ� ���) 
#include "WndControl.h"      // ���� ���� ��Ʈ�� (��ư, ��ũ�ѹ� ���)
#include "wndEditCtrl.h"     // ����Ʈ ��Ʈ��, IME, ���� �Է�
#include "WndItemCtrl.h"     // ���� ���� ��Ʈ�� (��ư, ��ũ�ѹ� ���)
#include "WndTradeCtrl.h"     // ���� ���� ��Ʈ�� (��ư, ��ũ�ѹ� ���)
#include "WndNeuz.h"         // Neuz ������ ���� ��� �⺻ Ŭ���� 
#include "WndMessageBox.h"   // �Ž��� �ڽ�, ���, ���� ��� 
#include "WndGuideSystem.h"
#include "WndTitle.h"        // Title�� ���� Ŭ���� (Login, Character List ���)
#include "WndTaskBar.h"      // �½�ũ �� Ŭ���� ����, �Ŵ� ��� 
#include "WndField.h"        // Field�� ���� Ŭ���� (����, �κ��丮 ���)
#include "WndCommunication.h"// Field�� ���� Ŭ���� (����, �κ��丮 ���)
#include "WndOption.h"       // Option ���� 
#include "WndOptionGame.h"	 // ���� �ɼ�
#include "WndDialog.h"       // ��ȭ  
#include "WndShop.h"         // ���� 
#include "WndMessengerSearch.h" // �޽��� �˻�â
#include "WndManager.h"      // ���� �Ŵ��� 
#include "WndWorld.h"        // ���� ���� 
#include "WndGuild.h"			// ��� ����
#include "WndRankGuild.h"		// ��� ��ŷ
#include "WndRankInfo.h"
#include "WndRankWar.h"
#include "WndGuildVote.h"         // �����ǥ ����
#include "WndUpgradeBase.h"         // ������� 

#ifdef __YCLOTH
#include "cloth.h"
#endif


// Application
#include "DialogMsg.h"
#include "Neuz.h"
#include "tickcount.h"
#include "flyffevent.h"

// console
#ifdef __BS_CONSOLE
#include "Console.h"
#endif

#ifdef __BS_EFFECT_LUA
#include "BsCommon.h"
#endif

#define D3DDEVICE   g_Neuz.m_pd3dDevice

extern BYTE             g_bKeyTable[256]; // Ű �Է� ���̺� 
extern BOOL             g_bSlotSwitchAboutEquipItem[ MAX_SLOT_ITEM ];
#ifdef __XKEYEDGE
extern BYTE             g_bKeyTable2[256]; // Ű �Է� ���̺� (Ű �����˻��)
#endif //__XKEYEDGE
extern CNeuzApp         g_Neuz        ; // ���ø����̼� Ŭ����, DX3D ������ ��ũ 
extern CToolTip         g_toolTip     ; // ���� ��� 
#if __VER >= 15 // __IMPROVE_SYSTEM_VER15
extern CToolTip         g_toolTipSub1 ;
extern CToolTip         g_toolTipSub2 ;
#endif // __IMPROVE_SYSTEM_VER15
extern CMover*          g_pPlayer     ; // �÷��̾� ��ü 
extern CWorldMng	    g_WorldMng    ;
extern CProject         prj;
extern CModelObject*    g_pBipedMesh;
extern CD3DApplication* g_pD3dApp;
extern CGameTimer       g_GameTimer;

struct CULLINFO
{
    D3DXVECTOR3 vecFrustum[8];    // corners of the view frustum
    D3DXPLANE planeFrustum[6];    // planes of the view frustum
};
extern CULLINFO     g_cullinfo;
extern	CTickCount	g_TickCount;
extern CFlyffEvent	g_eLocal; 
extern ADDSMMODE	g_AddSMMode;
extern DWORD		g_tmCurrent;	// �������� ����� ���� �ð�.
extern int			g_nRenderCnt;	// ����ȭ�鶧���� �ϳ��� ī��Ʈ �Ǵ� ����
extern int			g_nProcessCnt;	// ���μ������� �ϳ��� ī��Ʈ �Ǵ� ����.


#if __VER >= 11 // __GUILD_COMBAT_1TO1
#include "GuildCombat1to1.h"
extern CGuildCombat1to1Mng g_GuildCombat1to1Mng;
#endif // __GUILD_COMBAT_1TO1

#ifdef __BS_CONSOLE
extern DlgConsole g_Console;
#endif

#ifdef __CERTIFIER_COLLECTING_SYSTEM
extern vector< CString > g_vecEncryptedValidCertifierIP;
#endif // __CERTIFIER_COLLECTING_SYSTEM




//////////////////////////////////////////////////////////////////////////
//	mulcom	BEGIN100218	�н����� ��ȣȭ.
#include "../_Common/Rijndael.h"
//	mulcom	END100218	�н����� ��ȣȭ.
//////////////////////////////////////////////////////////////////////////

