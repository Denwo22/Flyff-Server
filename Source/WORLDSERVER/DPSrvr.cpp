#include "stdafx.h"
#include "defineText.h"
#include "defineItem.h"
#include "defineSkill.h"
#include "dpcoreclient.h"
#include "dpdatabaseclient.h"
#include "dpsrvr.h"
#include "definesound.h"
#include "defineObj.h"
#include "user.h"
#include "worldmng.h"
#include "misc.h"
#include "lang.h"
#include "npchecker.h"
#include "guild.h"
#include "..\_Common\Ship.h"
#include "Party.h"
#include "Chatting.h"
#include "post.h"
#include "..\_Network\ErrorCode.h"
#include "WantedListSnapshot.h"
#include "definequest.h"

#if __VER >= 12 // __PET_0519
#include "pet.h"
#endif	// __PET_0519

#if __VER >= 12 // __LORD
#include "slord.h"
#include "lordskillexecutable.h"
#endif	// __LORD

#if __VER >= 11 // __SYS_IDENTIFY
#include "randomoption.h"
#endif	// __SYS_IDENTIFY

#if __VER >= 11 // __SYS_PLAYER_DATA
#include "playerdata.h"
#endif	// __SYS_PLAYER_DATA

#if __VER >= 12 // __ITEMCREATEMON_S0602
	#include "CreateMonster.h"
#endif // __ITEMCREATEMON_S0602

#if __VER >= 13 // __HONORABLE_TITLE			// ����
	#include "honor.h"
#endif	// __HONORABLE_TITLE			// ����

#if __VER >= 12 // __SECRET_ROOM
#include "SecretRoom.h"
#endif // __SECRET_ROOM

#if __VER >= 12 // __TAX
#include "Tax.h"
#endif // __TAX

#if __VER >= 12 // __EXT_PIERCING
#include "ItemUpgrade.h"
#endif // __EXT_PIERCING

#include "tools.h"

#if __VER >= 13 // __COUPLE_1117
#include "couplehelper.h"
#endif	// __COUPLE_1117

#ifdef __QUIZ
#include "Quiz.h"
#endif // __QUIZ

#if __VER >= 15 // __GUILD_HOUSE
#include "GuildHouse.h"
#endif // __GUILD_HOUSE

#if __VER >= 15 // __CAMPUS
#include "CampusHelper.h"
#endif // __CAMPUS

#if __VER >= 8 //__CSC_VER8_5
struct ItemCountSet{
	OBJID itemid;
	int extracount;
};
#endif //__CSC_VER8_5

#define	MAX_RANGE_ASYNC			1024
#define	MAX_RANGE_NPC_MENU		1024

extern	CGuildMng			g_GuildMng;
extern	CPartyMng			g_PartyMng;
extern	CUserMng			g_UserMng;
extern	CDPDatabaseClient	g_dpDBClient;
extern	CDPCoreClient		g_DPCoreClient;
extern	CWorldMng			g_WorldMng;
extern	CChattingMng		g_ChattingMng;
extern	CGuildCombat		g_GuildCombatMng;

#if __VER >= 12 // __ITEMCREATEMON_S0602
#if __VER < 12 // __NEW_ITEMCREATEMON_SERVER
extern	CCreateMonster		g_CreateMonster;
#endif // __NEW_ITEMCREATEMON_SERVER
#endif // __ITEMCREATEMON_S0602

CCommonCtrl* CreateExpBox( CUser* pUser );
CDPSrvr		g_DPSrvr;

// dwGold�� nPlus�� �� �� �� �ִ°�?
BOOL CanAdd( DWORD dwGold, int nPlus )
{
	if( nPlus <= 0 )		// ���Ϸ��� ���� 0�����̸� �ͼ��� 
		return FALSE;

	int nGold = dwGold;
	ASSERT( nGold >= 0 );
	return ( (nGold + nPlus) > nGold );		// ���� ���� overflow�̸� ���ƾ��Ѵ�.
}

CDPSrvr::CDPSrvr()
{
	BEGIN_MSG;

	ON_MSG( PACKETTYPE_JOIN, &CDPSrvr::OnAddUser );
	ON_MSG( PACKETTYPE_LEAVE, &CDPSrvr::OnRemoveUser );
	ON_MSG( PACKETTYPE_REPLACE, &CDPSrvr::OnReplace );
	ON_MSG( PACKETTYPE_SUMMONPLAYER, &CDPSrvr::OnSummonPlayer );
	ON_MSG( PACKETTYPE_TELEPORTPLAYER, &CDPSrvr::OnTeleportPlayer );
	ON_MSG( PACKETTYPE_CORR_REQ, &CDPSrvr::OnCorrReq );
	ON_MSG( PACKETTYPE_SCRIPTDLG, &CDPSrvr::OnScriptDialogReq );
	ON_MSG( PACKETTYPE_TRADEPUT, &CDPSrvr::OnTradePut );
	ON_MSG( PACKETTYPE_TRADEPULL, &CDPSrvr::OnTradePull );
	ON_MSG( PACKETTYPE_TRADEPUTGOLD, &CDPSrvr::OnTradePutGold );
//raiders.2006.11.28
//	ON_MSG( PACKETTYPE_TRADECLEARGOLD, OnTradeClearGold );
	ON_MSG( PACKETTYPE_SFX_ID, &CDPSrvr::OnSfxID );
	ON_MSG( PACKETTYPE_SFX_CLEAR, &CDPSrvr::OnSfxClear );
	ON_MSG( PACKETTYPE_CREATE_GUILDCLOAK, &CDPSrvr::OnCreateGuildCloak );
	ON_MSG( PACKETTYPE_CHAT, &CDPSrvr::OnChat );
	ON_MSG( PACKETTYPE_CTRL_COOLTIME_CANCEL, &CDPSrvr::OnCtrlCoolTimeCancel );
	ON_MSG( PACKETTYPE_DOEQUIP, &CDPSrvr::OnDoEquip );
	ON_MSG( PACKETTYPE_MOVEITEM, &CDPSrvr::OnMoveItem );
	ON_MSG( PACKETTYPE_SNAPSHOT, &CDPSrvr::OnSnapshot );
	ON_MSG( PACKETTYPE_INC_STAT_LEVEL, &CDPSrvr::OnIncStatLevel );
	ON_MSG( PACKETTYPE_SEND_TO_SERVER_CHANGEJOB, &CDPSrvr::OnChangeJob );
	ON_MSG( PACKETTYPE_SETLODELIGHT, &CDPSrvr::OnSetLodelight );
	ON_MSG( PACKETTYPE_MODIFYMODE, &CDPSrvr::OnModifyMode );
	ON_MSG( PACKETTYPE_REVIVAL, &CDPSrvr::OnRevival );
	ON_MSG( PACKETTYPE_REVIVAL_TO_LODESTAR, &CDPSrvr::OnRevivalLodestar );
	ON_MSG( PACKETTYPE_REVIVAL_TO_LODELIGHT, &CDPSrvr::OnRevivalLodelight );
	ON_MSG( PACKETTYPE_OPENSHOPWND, &CDPSrvr::OnOpenShopWnd );
	ON_MSG( PACKETTYPE_CLOSESHOPWND, &CDPSrvr::OnCloseShopWnd );
	ON_MSG( PACKETTYPE_SEND_TO_SERVER_EXP, &CDPSrvr::OnExpUp );
	ON_MSG( PACKETTYPE_OPENBANKWND, &CDPSrvr::OnOpenBankWnd );
	ON_MSG( PACKETTYPE_GUILD_BANK_WND, &CDPSrvr::OnOpenGuildBankWnd );
	ON_MSG( PACKETTYPE_PUTITEMGUILDBANK, &CDPSrvr::OnPutItemGuildBank );
	ON_MSG( PACKETTYPE_GETITEMGUILDBANK, &CDPSrvr::OnGetItemGuildBank );
	ON_MSG( PACKETTYPE_GUILD_BANK_WND_CLOSE, &CDPSrvr::OnCloseGuildBankWnd );
	ON_MSG( PACKETTYPE_GUILD_BANK_MOVEITEM, &CDPSrvr::OnGuildBankMoveItem );
	ON_MSG( PACKETTYPE_CLOSEBANKWND, &CDPSrvr::OnCloseBankWnd );
	ON_MSG( PACKETTYPE_PUTITEMBACK, &CDPSrvr::OnPutItemBank );
	ON_MSG( PACKETTYPE_DOUSESKILLPOINT, &CDPSrvr::OnDoUseSkillPoint );
	ON_MSG( PACKETTYPE_PUTBACKTOBANK, &CDPSrvr::OnBankToBank );
	ON_MSG( PACKETTYPE_GETITEMBACK, &CDPSrvr::OnGetItemBank );
	ON_MSG( PACKETTYPE_PUTGOLDBACK, &CDPSrvr::OnPutGoldBank );
	ON_MSG( PACKETTYPE_GETGOLDBACK, &CDPSrvr::OnGetGoldBank );
	ON_MSG( PACKETTYPE_MOVEBANKITEM, &CDPSrvr::OnMoveBankItem );
	ON_MSG( PACKETTYPE_CHANGEBANKPASS, &CDPSrvr::OnChangeBankPass );
	ON_MSG( PACKETTYPE_CONFIRMBANK, &CDPSrvr::OnConfirmBank );	
	ON_MSG( PACKETTYPE_PLAYERMOVED, &CDPSrvr::OnPlayerMoved );
	ON_MSG( PACKETTYPE_PLAYERBEHAVIOR, &CDPSrvr::OnPlayerBehavior );
	ON_MSG( PACKETTYPE_PLAYERMOVED2, &CDPSrvr::OnPlayerMoved2 );
	ON_MSG( PACKETTYPE_PLAYERBEHAVIOR2, &CDPSrvr::OnPlayerBehavior2 );
	ON_MSG( PACKETTYPE_PLAYERANGLE, &CDPSrvr::OnPlayerAngle );
	ON_MSG( PACKETTYPE_PLAYERCORR, &CDPSrvr::OnPlayerCorr );
	ON_MSG( PACKETTYPE_PLAYERCORR2, &CDPSrvr::OnPlayerCorr2 );
	ON_MSG( PACKETTYPE_PLAYERSETDESTOBJ, &CDPSrvr::OnPlayerSetDestObj );
	ON_MSG( PACKETTYPE_TRADE, &CDPSrvr::OnTrade );
	ON_MSG( PACKETTYPE_CONFIRMTRADE, &CDPSrvr::OnConfirmTrade );
	ON_MSG( PACKETTYPE_CONFIRMTRADECANCEL, &CDPSrvr::OnConfirmTradeCancel )
	ON_MSG( PACKETTYPE_TRADECANCEL, &CDPSrvr::OnTradeCancel );
	ON_MSG( PACKETTYPE_DOUSEITEM, &CDPSrvr::OnDoUseItem );
#if __VER >= 11 // __SYS_IDENTIFY
	ON_MSG( PACKETTYPE_DO_USE_ITEM_TARGET, &CDPSrvr::OnDoUseItemTarget );
	ON_MSG( PACKETTYPE_REMOVE_ITEM_LEVEL_DOWN, &CDPSrvr::OnRemoveItemLevelDown );
	ON_MSG( PACKETTYPE_AWAKENING, &CDPSrvr::OnAwakening );
	ON_MSG( PACKETTYPE_BLESSEDNESS_CANCEL, &CDPSrvr::OnBlessednessCancel );
#endif	// __SYS_IDENTIFY
#ifdef __AZRIA_1023
	ON_MSG( PACKETTYPE_DO_USE_ITEM_INPUT, &CDPSrvr::OnDoUseItemInput );
#endif	// __AZRIA_1023
#ifdef __PET_1024
	ON_MSG( PACKETTYPE_CLEAR_PET_NAME, &CDPSrvr::OnClearPetName );
#endif	// __PET_1024
#if __VER >= 11 // __SYS_POCKET
	ON_MSG( PACKETTYPE_AVAIL_POCKET, &CDPSrvr::OnAvailPocket );
	ON_MSG( PACKETTYPE_MOVE_ITEM_POCKET, &CDPSrvr::OnMoveItemOnPocket );
#endif	// __SYS_POCKET

#ifdef __JEFF_11
	ON_MSG( PACKETTYPE_QUE_PETRESURRECTION, &CDPSrvr::OnQuePetResurrection );
#endif	// __JEFF_11

#ifdef __JEFF_11_4
	ON_MSG( PACKETTYPE_ARENA_ENTER, &CDPSrvr::OnArenaEnter );
	ON_MSG( PACKETTYPE_ARENA_EXIT, &CDPSrvr::OnArenaExit );
#endif	// __JEFF_11_4

	ON_MSG( PACKETTYPE_TRADECONFIRM, &CDPSrvr::OnTradelastConfrim );
	ON_MSG( PACKETTYPE_MOVERFOCOUS, &CDPSrvr::OnMoverFocus );
	ON_MSG( PACKETTYPE_DROPITEM, &CDPSrvr::OnDropItem );
	ON_MSG( PACKETTYPE_DROPGOLD, &CDPSrvr::OnDropGold );
	ON_MSG( PACKETTYPE_BUYITEM, &CDPSrvr::OnBuyItem );
#if __VER >= 11 // __GUILDCOMBATCHIP
	ON_MSG( PACKETTYPE_BUYCHIPITEM, &CDPSrvr::OnBuyChipItem );
#endif // __GUILDCOMBATCHIP
	ON_MSG( PACKETTYPE_SELLITEM, &CDPSrvr::OnSellItem );
	ON_MSG( PACKETTYPE_TRADEOK, &CDPSrvr::OnTradeOk );
	ON_MSG( PACKETTYPE_MELEE_ATTACK, &CDPSrvr::OnMeleeAttack );
	ON_MSG( PACKETTYPE_MELEE_ATTACK2, &CDPSrvr::OnMeleeAttack2 );
	ON_MSG( PACKETTYPE_MAGIC_ATTACK, &CDPSrvr::OnMagicAttack );
	ON_MSG( PACKETTYPE_RANGE_ATTACK, &CDPSrvr::OnRangeAttack );
	ON_MSG( PACKETTYPE_SFX_HIT, &CDPSrvr::OnSfxHit );
	ON_MSG( PACKETTYPE_USESKILL, &CDPSrvr::OnUseSkill );
	ON_MSG( PACKETTYPE_SETTARGET, &CDPSrvr::OnSetTarget );	// Core�� ������ �ϴ��� Ȯ���� �ٰ�.
	ON_MSG( PACKETTYPE_TELESKILL, &CDPSrvr::OnTeleSkill );	
	ON_MSG( PACKETTYPE_SKILLTASKBAR, &CDPSrvr::OnSkillTaskBar );
	ON_MSG( PACKETTYPE_ADDAPPLETTASKBAR, &CDPSrvr::OnAddAppletTaskBar );
	ON_MSG( PACKETTYPE_REMOVEAPPLETTASKBAR, &CDPSrvr::OnRemoveAppletTaskBar );
	ON_MSG( PACKETTYPE_ADDITEMTASKBAR, &CDPSrvr::OnAddItemTaskBar );
	ON_MSG( PACKETTYPE_REMOVEITEMTASKBAR, &CDPSrvr::OnRemoveItemTaskBar );
	ON_MSG( PACKETTYPE_QUERYGETPOS, &CDPSrvr::OnQueryGetPos );
	ON_MSG( PACKETTYPE_GETPOS, &CDPSrvr::OnGetPos );
	ON_MSG( PACKETTYPE_QUERYGETDESTOBJ, &CDPSrvr::OnQueryGetDestObj );
	ON_MSG( PACKETTYPE_GETDESTOBJ, &CDPSrvr::OnGetDestObj );
	ON_MSG( PACKETTYPE_MEMBERREQUEST, &CDPSrvr::OnPartyRequest );
	ON_MSG( PACKETTYPE_MEMBERREQUESTCANCLE, &CDPSrvr::OnPartyRequestCancle );
	ON_MSG( PACKETTYPE_PARTYSKILLUSE, &CDPSrvr::OnPartySkillUse );
	ON_MSG( PACKETTYPE_ADDFRIENDREQEST, &CDPSrvr::OnAddFriendReqest );
	ON_MSG( PACKETTYPE_ADDFRIENDNAMEREQEST, &CDPSrvr::OnAddFriendNameReqest );
	ON_MSG( PACKETTYPE_ADDFRIENDCANCEL, &CDPSrvr::OnAddFriendCancel );
	ON_MSG( PACKETTYPE_DUELREQUEST, &CDPSrvr::OnDuelRequest );
	ON_MSG( PACKETTYPE_DUELYES, &CDPSrvr::OnDuelYes );
	ON_MSG( PACKETTYPE_DUELNO, &CDPSrvr::OnDuelNo );
	ON_MSG( PACKETTYPE_DUELPARTYREQUEST, &CDPSrvr::OnDuelPartyRequest );
	ON_MSG( PACKETTYPE_DUELPARTYYES, &CDPSrvr::OnDuelPartyYes );
	ON_MSG( PACKETTYPE_DUELPARTYNO, &CDPSrvr::OnDuelPartyNo );
	ON_MSG( PACKETTYPE_SEND_TO_SERVER_AP, &CDPSrvr::OnActionPoint );
#if __VER >= 11 // __SYS_PLAYER_DATA
	ON_MSG( PACKETTYPE_QUERY_PLAYER_DATA, &CDPSrvr::OnQueryPlayerData );
	ON_MSG( PACKETTYPE_QUERY_PLAYER_DATA2, &CDPSrvr::OnQueryPlayerData2 );
#else	// __SYS_PLAYER_DATA
	ON_MSG( PACKETTYPE_QUERYPLAYERSTRING, &CDPSrvr::OnQueryPlayerString );
	ON_MSG( PACKETTYPE_QUERYPLAYERLISTSTRING, &CDPSrvr::OnQueryPlayerListString );
	ON_MSG( PACKETTYPE_GETFRIENDNAME, &CDPSrvr::OnGetFriendName );
#endif	// __SYS_PLAYER_DATA
	ON_MSG( PACKETTYPE_GUILD_INVITE, &CDPSrvr::OnGuildInvite );
	ON_MSG( PACKETTYPE_IGNORE_GUILD_INVITE, &CDPSrvr::OnIgnoreGuildInvite );
	ON_MSG( PACKETTYPE_NW_GUILDLOGO, &CDPSrvr::OnGuildLogo );			// �ΰ� ���� 
	ON_MSG( PACKETTYPE_NW_GUILDCONTRIBUTION, &CDPSrvr::OnGuildContribution );		// ���嵵 
	ON_MSG( PACKETTYPE_NW_GUILDNOTICE, &CDPSrvr::OnGuildNotice );		// ��������  
	ON_MSG( PACKETTYPE_REQUEST_GUILD_RANKING, &CDPSrvr::OnRequestGuildRank );
	ON_MSG( PACKETTYPE_PVENDOR_OPEN, &CDPSrvr::OnPVendorOpen );
	ON_MSG( PACKETTYPE_PVENDOR_CLOSE, &CDPSrvr::OnPVendorClose );
	ON_MSG( PACKETTYPE_REGISTER_PVENDOR_ITEM, &CDPSrvr::OnRegisterPVendorItem );
	ON_MSG( PACKETTYPE_UNREGISTER_PVENDOR_ITEM, &CDPSrvr::OnUnregisterPVendorItem );
	ON_MSG( PACKETTYPE_QUERY_PVENDOR_ITEM, &CDPSrvr::OnQueryPVendorItem );
	ON_MSG( PACKETTYPE_BUY_PVENDOR_ITEM, &CDPSrvr::OnBuyPVendorItem );
	ON_MSG( PACKETTYPE_REPAIRITEM, &CDPSrvr::OnRepairItem );
	ON_MSG( PACKETTYPE_SET_HAIR, &CDPSrvr::OnSetHair );
	ON_MSG( PACKETTYPE_BLOCK, &CDPSrvr::OnBlock );
	ON_MSG( PACKETTYPE_MOTION, &CDPSrvr::OnMotion );
	ON_MSG( PACKETTYPE_SHIP_ACTMSG, &CDPSrvr::OnShipActMsg );
	ON_MSG( PACKETTYPE_LOCALPOSFROMIA, &CDPSrvr::OnLocalPosFromIA );
	ON_MSG( PACKETTYPE_UPGRADEBASE, &CDPSrvr::OnUpgradeBase );
	ON_MSG( PACKETTYPE_ENCHANT, &CDPSrvr::OnEnchant );
#if __VER >= 14 // __SMELT_SAFETY
	ON_MSG( PACKETTYPE_SMELT_SAFETY, &CDPSrvr::OnSmeltSafety );
#endif // __SMELT_SAFETY
#if __VER >= 10 // __REMOVE_ATTRIBUTE
	ON_MSG( PACKETTYPE_REMVOE_ATTRIBUTE, &CDPSrvr::OnRemoveAttribute );
#endif // __REMOVE_ATTRIBUTE
#if __VER >= 13 // __EXT_ENCHANT
	ON_MSG( PACKETTYPE_CHANGE_ATTRIBUTE, &CDPSrvr::OnChangeAttribute );
#endif // __EXT_ENCHANT
	ON_MSG( PACKETTYPE_PIERCING_SIZE, &CDPSrvr::OnPiercingSize );
	ON_MSG( PACKETTYPE_EXPBOXINFO, &CDPSrvr::OnExpBoxInfo );
	ON_MSG( PACKETTYPE_RANDOMSCROLL, &CDPSrvr::OnRandomScroll );
	ON_MSG( PACHETTYPE_ITEMTRANSY, &CDPSrvr::OnItemTransy );
	ON_MSG( PACKETTYPE_PIERCING, &CDPSrvr::OnPiercing );
#if __VER >= 11 // __PIERCING_REMOVE
	ON_MSG( PACKETTYPE_PIERCINGREMOVE, &CDPSrvr::OnPiercingRemove );
#endif // __PIERCING_REMOVE
	ON_MSG( PACKETTYPE_BUYING_INFO, &CDPSrvr::OnBuyingInfo );
	ON_MSG( PACKETTYPE_ENTERCHTTING, &CDPSrvr::OnEnterChattingRoom );
	ON_MSG( PACKETTYPE_CHATTING, &CDPSrvr::OnChatting );
	ON_MSG( PACKETTYPE_OPENCHATTINGROOM, &CDPSrvr::OnOpenChattingRoom );
	ON_MSG( PACKETTYPE_CLOSECHATTINGROOM, &CDPSrvr::OnCloseChattingRoom );
	ON_MSG( PACKETTYPE_COMMONPLACE, &CDPSrvr::OnCommonPlace );
	ON_MSG( PACKETTYPE_SETNAVIPOINT, &CDPSrvr::OnSetNaviPoint );
	ON_MSG( PACKETTYPE_DO_ESCAPE, &CDPSrvr::OnDoEscape );
	ON_MSG( PACKETTYPE_LOG_GAMEMASTER_CHAT, &CDPSrvr::OnGameMasterWhisper );
	ON_MSG( PACKETTYPE_RETURNSCROLL, &CDPSrvr::OnReturnScroll );
	ON_MSG( PACKETTYPE_ENDSKILLQUEUE, &CDPSrvr::OnEndSkillQueue );
	ON_MSG( PACKETTYPE_FOCUSOBJ, &CDPSrvr::OnFoucusObj );

	ON_MSG( PACKETTYPE_REMOVEQUEST, &CDPSrvr::OnRemoveQuest );
	ON_MSG( PACKETTYPE_COMMERCIALELEM, &CDPSrvr::OnCommercialElem );
//	ON_MSG( PACKETTYPE_DO_COLLECT, OnDoCollect );

	ON_MSG( PACKETTYPE_NW_WANTED_GOLD, &CDPSrvr::OnNWWantedGold );
	ON_MSG( PACKETTYPE_NW_WANTED_LIST, &CDPSrvr::OnNWWantedList );
	ON_MSG( PACKETTYPE_NW_WANTED_INFO, &CDPSrvr::OnNWWantedInfo );
	ON_MSG( PACKETTYPE_NW_WANTED_NAME, &CDPSrvr::OnNWWantedName );
	ON_MSG( PACKETTYPE_REQ_LEAVE, &CDPSrvr::OnReqLeave );
	ON_MSG( PACKETTYPE_RESURRECTION_OK, &CDPSrvr::OnResurrectionOK );
	ON_MSG( PACKETTYPE_RESURRECTION_CANCEL, &CDPSrvr::OnResurrectionCancel );
	ON_MSG( PACKETTYPE_MODE, &CDPSrvr::OnChangeMode );

	ON_MSG( PACKETTYPE_STATEMODE, &CDPSrvr::OnStateMode );
	ON_MSG( PACKETTYPE_QUERYSETPLAYERNAME, &CDPSrvr::OnQuerySetPlayerName );
	ON_MSG( PACKETTYPE_QUERYSETGUILDNAME, &CDPSrvr::OnQuerySetGuildName );
	ON_MSG( PACKETTYPE_CHEERING, &CDPSrvr::OnCheering );
	ON_MSG( PACKETTYPE_QUERYEQUIP, &CDPSrvr::OnQueryEquip );
	ON_MSG( PACKETTYPE_QUERYEQUIPSETTING, &CDPSrvr::OnQueryEquipSetting );
	ON_MSG( PACKETTYPE_QUERYPOSTMAIL, &CDPSrvr::OnQueryPostMail );
	ON_MSG( PACKETTYPE_QUERYREMOVEMAIL, &CDPSrvr::OnQueryRemoveMail );
	ON_MSG( PACKETTYPE_QUERYGETMAILITEM, &CDPSrvr::OnQueryGetMailItem );
	ON_MSG( PACKETTYPE_QUERYGETMAILGOLD, &CDPSrvr::OnQueryGetMailGold );
	ON_MSG( PACKETTYPE_READMAIL, &CDPSrvr::OnQueryReadMail );
	ON_MSG( PACKETTYPE_QUERYMAILBOX, &CDPSrvr::OnQueryMailBox );
	ON_MSG( PACKETTYPE_CHANGEFACE, &CDPSrvr::OnChangeFace );

#if __VER >= 12 // __ITEMCREATEMON_S0602
	ON_MSG( PACKETTYPE_CREATEMONSTER, &CDPSrvr::OnCreateMonster );
#endif // __ITEMCREATEMON_S0602

	ON_MSG( PACKETTYPE_IN_GUILDCOMBAT, &CDPSrvr::OnGCApp );
	ON_MSG( PACKETTYPE_OUT_GUILDCOMBAT, &CDPSrvr::OnGCCancel );	
	ON_MSG( PACKETTYPE_REQUEST_STATUS, &CDPSrvr::OnGCRequestStatus );
	ON_MSG( PACKETTYPE_SELECTPLAYER_GUILDCOMBAT, &CDPSrvr::OnGCSelectPlayer );
	ON_MSG( PACKETTYPE_SELECTMAP_GUILDCOMBAT, &CDPSrvr::OnGCSelectMap );
	ON_MSG( PACKETTYPE_JOIN_GUILDCOMBAT, &CDPSrvr::OnGCJoin );	
	ON_MSG( PACKETTYPE_GETPENYAGUILD_GUILDCOMBAT, &CDPSrvr::OnGCGetPenyaGuild );
	ON_MSG( PACKETTYPE_GETPENYAPLAYER_GUILDCOMBAT, &CDPSrvr::OnGCGetPenyaPlayer );
#if __VER < 8 // #ifndef __GUILDCOMBAT_85
	ON_MSG( PACKETTYPE_GETITEM_GUILDCOMBAT, &CDPSrvr::OnGCGetItem );
#endif // __VER < 8
	ON_MSG( PACKETTYPE_TELE_GUILDCOMBAT, &CDPSrvr::OnGCTele );
	ON_MSG( PACKETTYPE_PLAYERPOINT_GUILDCOMBAT, &CDPSrvr::OnGCPlayerPoint );
	ON_MSG( PACKETTYPE_SUMMON_FRIEND, &CDPSrvr::OnSummonFriend );
	ON_MSG( PACKETTYPE_SUMMON_FRIEND_CONFIRM, &CDPSrvr::OnSummonFriendConfirm );
	ON_MSG( PACKETTYPE_SUMMON_FRIEND_CANCEL, &CDPSrvr::OnSummonFriendCancel );
	ON_MSG( PACKETTYPE_SUMMON_PARTY, &CDPSrvr::OnSummonParty );
	ON_MSG( PACKETTYPE_SUMMON_PARTY_CONFIRM, &CDPSrvr::OnSummonPartyConfirm );

	ON_MSG( PACKETTYPE_REMOVEINVENITEM, &CDPSrvr::OnRemoveInvenItem );
	////////////////////////////////////////////////////////////////////////
#if __VER >= 8 //__CSC_VER8_5
	ON_MSG( PACKETTYPE_CREATEANGEL, &CDPSrvr::OnCreateAngel );
	ON_MSG( PACKETTYPE_ANGELBUFF, &CDPSrvr::OnAngleBuff );
#endif //__CSC_VER8_5
	
#ifdef __EVE_MINIGAME
	ON_MSG( PACKETTYPE_KAWIBAWIBO_START, &CDPSrvr::OnKawibawiboStart );
	ON_MSG( PACKETTYPE_KAWIBAWIBO_GETITEM, &CDPSrvr::OnKawibawiboGetItem );
	ON_MSG( PACKETTYPE_REASSEMBLE_START, &CDPSrvr::OnReassembleStart );
	ON_MSG( PACKETTYPE_REASSEMBLE_OPENWND, &CDPSrvr::OnReassembleOpenWnd );
	ON_MSG( PACKETTYPE_ALPHABET_OPENWND, &CDPSrvr::OnAlphabetOpenWnd );
	ON_MSG( PACKETTYPE_ALPHABET_START, &CDPSrvr::OnAlphabetStart );
	ON_MSG( PACKETTYPE_FIVESYSTEM_OPENWND, &CDPSrvr::OnFiveSystemOpenWnd );
	ON_MSG( PACKETTYPE_FIVESYSTEM_BET, &CDPSrvr::OnFiveSystemBet );
	ON_MSG( PACKETTYPE_FIVESYSTEM_START, &CDPSrvr::OnFiveSystemStart );
	ON_MSG( PACKETTYPE_FIVESYSTEM_DESTROYWND, &CDPSrvr::OnFiveSystemDestroyWnd );
#endif // __EVE_MINIGAME
	
#if __VER >= 9 // __ULTIMATE
	ON_MSG( PACKETTYPE_ULTIMATE_MAKEITEM, &CDPSrvr::OnUltimateMakeItem );
	ON_MSG( PACKETTYPE_ULTIMATE_MAKEGEM, &CDPSrvr::OnUltimateMakeGem );
	ON_MSG( PACKETTYPE_ULTIMATE_TRANSWEAPON, &CDPSrvr::OnUltimateTransWeapon );
	ON_MSG( PACKETTYPE_ULTIMATE_SETGEM, &CDPSrvr::OnUltimateSetGem );
	ON_MSG( PACKETTYPE_ULTIMATE_REMOVEGEM, &CDPSrvr::OnUltimateRemoveGem );
	ON_MSG( PACKETTYPE_ULTIMATE_ENCHANTWEAPON, &CDPSrvr::OnUltimateEnchantWeapon );
#endif // __ULTIMATE

#ifdef __TRADESYS
	ON_MSG( PACKETTYPE_EXCHANGE, &CDPSrvr::OnExchange );
#endif // __TRADESYS

#if __VER >= 9	// __PET_0410
	ON_MSG( PACKETTYPE_PET_RELEASE, &CDPSrvr::OnPetRelease );
	ON_MSG( PACKETTYPE_USE_PET_FEED, &CDPSrvr::OnUsePetFeed );
	ON_MSG( PACKETTYPE_MAKE_PET_FEED, &CDPSrvr::OnMakePetFeed );
	ON_MSG( PACKETTYPE_PET_TAMER_MISTAKE, &CDPSrvr::OnPetTamerMistake );
	ON_MSG( PACKETTYPE_PET_TAMER_MIRACLE, &CDPSrvr::OnPetTamerMiracle );
	ON_MSG( PACKETTYPE_FEED_POCKET_INACTIVE, &CDPSrvr::OnFeedPocketInactive );
#endif	// __PET_0410

#if __VER >= 10 // __LEGEND	//	9�� ���½ý���	Neuz, World, Trans
	ON_MSG( PACKETTYPE_LEGENDSKILLUP_START, &CDPSrvr::OnLegendSkillStart );
#endif	//__LEGEND	//	9�� ���½ý���	Neuz, World, Trans

#if __VER >= 9 // __CSC_VER9_2
	ON_MSG( PACKETTYPE_MODIFY_STATUS, &CDPSrvr::OnModifyStatus );
#endif //__CSC_VER9_2

#if __VER >= 11 // __GUILD_COMBAT_1TO1
	ON_MSG( PACKETTYPE_GC1TO1_TENDEROPENWND, &CDPSrvr::OnGC1to1TenderOpenWnd );
	ON_MSG( PACKETTYPE_GC1TO1_TENDERVIEW, &CDPSrvr::OnGC1to1TenderView );
	ON_MSG( PACKETTYPE_GC1TO1_TENDER, &CDPSrvr::OnGC1to1Tender );
	ON_MSG( PACKETTYPE_GC1TO1_TENDERCANCEL, &CDPSrvr::OnGC1to1CancelTender );
	ON_MSG( PACKETTYPE_GC1TO1_TENDERFAILED, &CDPSrvr::OnGC1to1FailedTender );
	ON_MSG( PACKETTYPE_GC1TO1_MEMBERLINEUPOPENWND, &CDPSrvr::OnGC1to1MemberLineUpOpenWnd );
	ON_MSG( PACKETTYPE_GC1TO1_MEMBERLINEUP, &CDPSrvr::OnGC1to1MemberLineUp );
	ON_MSG( PACKETTYPE_GC1TO1_TELEPORTTONPC, &CDPSrvr::OnGC1to1TeleportToNPC );
	ON_MSG( PACKETTYPE_GC1TO1_TELEPORTTOSTAGE, &CDPSrvr::OnGC1to1TeleportToStage );	
#endif // __GUILD_COMBAT_1TO1

#if __VER >= 11 // __MA_VER11_04	// ��� â�� �α� ��� world,database,neuz
	ON_MSG( PACKETTYPE_GUILDLOG_VIEW, &CDPSrvr::OnQueryGuildBankLogList );
#endif //__MA_VER11_04	// ��� â�� �α� ��� world,database,neuz
#if __VER >= 11 // __MA_VER11_05	// �ɸ��� ���� �ŷ� ��� world,database,neuz
	ON_MSG( PACKETTYPE_SEALCHAR_REQ, &CDPSrvr::OnSealCharReq );
	ON_MSG( PACKETTYPE_SEALCHARCONM_REQ, &CDPSrvr::OnSealCharConmReq );
	ON_MSG( PACKETTYPE_SEALCHARGET_REQ, &CDPSrvr::OnSealCharGetReq );
#endif // __MA_VER11_05	// �ɸ��� ���� �ŷ� ��� world,database,neuz
#if __VER >= 13 // __HONORABLE_TITLE			// ����
	ON_MSG( PACKETTYPE_HONOR_LIST_REQ, &CDPSrvr::OnHonorListReq );
	ON_MSG( PACKETTYPE_HONOR_CHANGE_REQ, &CDPSrvr::OnHonorChangeReq );
#endif	// __HONORABLE_TITLE			// ����

#if __VER >= 11 // __SYS_COLLECTING
	ON_MSG( PACKETTYPE_QUERY_START_COLLECTING, &CDPSrvr::OnQueryStartCollecting );
	ON_MSG( PACKETTYPE_QUERY_STOP_COLLECTING, &CDPSrvr::OnQueryStopCollecting );
#endif	// __SYS_COLLECTING

#ifdef __NPC_BUFF
	ON_MSG( PACKETTYPE_NPC_BUFF, &CDPSrvr::OnNPCBuff );
#endif // __NPC_BUFF

#if __VER >= 12 // __SECRET_ROOM
	ON_MSG( PACKETTYPE_SECRETROOM_TENDEROPENWND, &CDPSrvr::OnSecretRoomTenderOpenWnd );
	ON_MSG( PACKETTYPE_SECRETROOM_TENDER, &CDPSrvr::OnSecretRoomTender );
	ON_MSG( PACKETTYPE_SECRETROOM_TENDERCANCELRETURN, &CDPSrvr::OnSecretRoomTenderCancelReturn );
	ON_MSG( PACKETTYPE_SECRETROOM_LINEUPOPENWND, &CDPSrvr::OnSecretRoomLineUpOpenWnd );
	ON_MSG( PACKETTYPE_SECRETROOM_LINEUPMEMBER, &CDPSrvr::OnSecretRoomLineUpMember );
	ON_MSG( PACKETTYPE_SECRETROOM_ENTRANCE, &CDPSrvr::OnSecretRoomEntrance );
	ON_MSG( PACKETTYPE_SECRETROOM_TELEPORTTONPC, &CDPSrvr::OnSecretRoomTeleportToNPC );
	ON_MSG( PACKETTYPE_SECRETROOM_TENDERVIEW, &CDPSrvr::OnSecretRoomTenderView );
	ON_MSG( PACKETTYPE_SECRETROOM_TELEPORTTODUNGEON, &CDPSrvr::OnTeleportSecretRoomDungeon );
#endif // __SECRET_ROOM

#if __VER >= 12 // __LORD
	ON_MSG( PACKETTYPE_ELECTION_ADD_DEPOSIT, &CDPSrvr::OnElectionAddDeposit );
	ON_MSG( PACKETTYPE_ELECTION_SET_PLEDGE, &CDPSrvr::OnElectionSetPledge );
	ON_MSG( PACKETTYPE_ELECTION_INC_VOTE, &CDPSrvr::OnElectionIncVote );
	ON_MSG( PACKETTYPE_L_EVENT_CREATE, &CDPSrvr::OnLEventCreate );
	ON_MSG( PACKETTYPE_LORD_SKILL_USE, &CDPSrvr::OnLordSkillUse );
#endif	// __LORD
#if __VER >= 12 // __PET_0519
	// �˺�ȯ �ڵ鷯
	ON_MSG( PACKETTYPE_TRANSFORM_ITEM, &CDPSrvr::OnTransformItem );
#endif	// __PET_0519

#if __VER >= 12 // __TAX
	ON_MSG( PACKETTYPE_TAX_SET_TAXRATE, &CDPSrvr::OnSetTaxRate );
#endif // __TAX

#if __VER >= 12 // __HEAVEN_TOWER
	ON_MSG( PACKETTYPE_HEAVENTOWER_TELEPORT, &CDPSrvr::OnTeleportToHeavenTower );
#endif // __HEAVEN_TOWER

#if __VER >= 12 // __MOD_TUTORIAL
	ON_MSG( PACKETTYPE_TUTORIAL_STATE, &CDPSrvr::OnTutorialState );
#endif	// __MOD_TUTORIAL

#if __VER >= 12 // __PET_0519
	ON_MSG( PACKETTYPE_PICKUP_PET_AWAKENING_CANCEL, &CDPSrvr::OnPickupPetAwakeningCancel );
#endif	// __PET_0519

#if __VER >= 12 // __UPDATE_OPT
	ON_MSG( PACKETTYPE_OPTION_ENABLE_RENDER_MASK, &CDPSrvr::OnOptionEnableRenderMask );
#endif	// __UPDATE_OPT

#if __VER >= 13 // __RAINBOW_RACE
	ON_MSG( PACKETTYPE_RAINBOWRACE_PREVRANKING_OPENWND, &CDPSrvr::OnRainbowRacePrevRankingOpenWnd );
	ON_MSG( PACKETTYPE_RAINBOWRACE_APPLICATION_OPENWND, &CDPSrvr::OnRainbowRaceApplicationOpenWnd );
	ON_MSG( PACKETTYPE_RAINBOWRACE_APPLICATION, &CDPSrvr::OnRainbowRaceApplication );
	ON_MSG( PACKETTYPE_RAINBOWRACE_MINIGAME_PACKET, &CDPSrvr::OnRainbowRaceMiniGamePacket );
	ON_MSG( PACKETTYPE_RAINBOWRACE_REQ_FINISH, &CDPSrvr::OnRainbowRaceReqFinish );
#endif // __RAINBOW_RACE

#if __VER >= 13 // __HOUSING
	ON_MSG( PACKETTYPE_HOUSING_SETUPFURNITURE, &CDPSrvr::OnHousingSetupFurniture );
	ON_MSG( PACKETTYPE_HOUSING_SETVISITALLOW, &CDPSrvr::OnHousingSetVisitAllow );
	ON_MSG( PACKETTYPE_HOUSING_VISITROOM, &CDPSrvr::OnHousingVisitRoom );
	ON_MSG( PACKETTYPE_HOUSING_REQVISITABLELIST, &CDPSrvr::OnHousingVisitableList );
	ON_MSG( PACKETTYPE_HOUSING_GOOUT, &CDPSrvr::OnHousingGoOut );

#endif // __HOUSING

#if __VER >= 13 // __QUEST_HELPER
	ON_MSG( PACKETTYPE_QUESTHELPER_REQNPCPOS, &CDPSrvr::OnReqQuestNPCPos );
#endif // __QUEST_HELPER

#if __VER >= 13 // __COUPLE_1117
	ON_MSG( PACKETTYPE_PROPOSE, &CDPSrvr::OnPropose );
	ON_MSG( PACKETTYPE_REFUSE, &CDPSrvr::OnRefuse );
	ON_MSG( PACKETTYPE_COUPLE, &CDPSrvr::OnCouple );
	ON_MSG( PACKETTYPE_DECOUPLE, &CDPSrvr::OnDecouple );
#endif	// __COUPLE_1117
#ifdef __MAP_SECURITY
	ON_MSG( PACKETTYPE_MAP_KEY, &CDPSrvr::OnMapKey );
#endif // __MAP_SECURITY
#ifdef __QUIZ
	ON_MSG( PACKETTYPE_QUIZ_ENTRANCE, &CDPSrvr::OnQuizEventEntrance );
	ON_MSG( PACKETTYPE_QUIZ_TELEPORT, &CDPSrvr::OnQuizEventTeleport );
#endif // __QUIZ
#if __VER >= 15 // __PETVIS
	ON_MSG( PACKETTYPE_VISPET_REMOVEVIS, &CDPSrvr::OnRemoveVis );
	ON_MSG( PACKETTYPE_VISPET_SWAPVIS, &CDPSrvr::OnSwapVis );
#endif // __PETVIS
#if __VER >= 15 // __GUILD_HOUSE
	ON_MSG( PACKETTYPE_GUILDHOUSE_BUY, &CDPSrvr::OnBuyGuildHouse );
	ON_MSG( PACKETTYPE_GUILDHOUSE_PACKET, &CDPSrvr::OnGuildHousePacket );
	ON_MSG( PACKETTYPE_GUILDHOUSE_ENTER, &CDPSrvr::OnGuildHouseEnter );
	ON_MSG( PACKETTYPE_GUILDHOUSE_GOOUT, &CDPSrvr::OnGuildHouseGoOut );
#endif // __GUILD_HOUSE
#if __VER >= 15 // __TELEPORTER
	ON_MSG( PACKETTYPE_TELEPORTER, &CDPSrvr::OnTeleporterReq );
#endif // __TELEPORTER
#if __VER >= 15 // __IMPROVE_QUEST_INTERFACE
	ON_MSG( PACKETTYPE_QUEST_CHECK, &CDPSrvr::OnCheckedQuest );
#endif // __IMPROVE_QUEST_INTERFACE
#if __VER >= 15 // __CAMPUS
	ON_MSG( PACKETTYPE_CAMPUS_INVITE, &CDPSrvr::OnInviteCampusMember );
	ON_MSG( PACKETTYPE_CAMPUS_ACCEPT, &CDPSrvr::OnAcceptCampusMember );
	ON_MSG( PACKETTYPE_CAMPUS_REFUSE, &CDPSrvr::OnRefuseCampusMember );
	ON_MSG( PACKETTYPE_CAMPUS_REMOVE_MEMBER, &CDPSrvr::OnRemoveCampusMember );
#endif // __CAMPUS


	//	mulcom	BEGIN100405	���� ��ȣ�� �η縶��
	ON_MSG( PACKETTYPE_ITEM_SELECT_AWAKENING_VALUE, &CDPSrvr::OnItemSelectAwakeningValue );
	//	mulcom	END100405	���� ��ȣ�� �η縶��

#ifdef __GUILD_HOUSE_MIDDLE
	ON_MSG( PACKETTYPE_GUILDHOUSE_TENDER_MAINWND, &CDPSrvr::OnGuildHouseTenderMainWnd );
	ON_MSG( PACKETTYPE_GUILDHOUSE_TENDER_INFOWND, &CDPSrvr::OnGuildHouseTenderInfoWnd );
	ON_MSG( PACKETTYPE_GUILDHOUSE_TENDER_JOIN, &CDPSrvr::OnGuildHouseTenderJoin );
#endif // __GUILD_HOUSE_MIDDLE
}

CDPSrvr::~CDPSrvr()
{
	m_dpidCache = DPID_UNKNOWN;			// ĳ������ DPID
}

void CDPSrvr::SysMessageHandler( LPDPMSG_GENERIC lpMsg, DWORD dwMsgSize, DPID idFrom )
{
	switch( lpMsg->dwType )
	{
		case DPSYS_CREATEPLAYERORGROUP:
			{
				LPDPMSG_CREATEPLAYERORGROUP lpCreatePlayer	= (LPDPMSG_CREATEPLAYERORGROUP)lpMsg;
				OnAddConnection( lpCreatePlayer->dpId );
				break;
			}
		case DPSYS_DESTROYPLAYERORGROUP:
			{
				LPDPMSG_DESTROYPLAYERORGROUP lpDestroyPlayer	= (LPDPMSG_DESTROYPLAYERORGROUP)lpMsg;
				OnRemoveConnection( lpDestroyPlayer->dpId );
				break;
			}
	}
}

void CDPSrvr::UserMessageHandler( LPDPMSG_GENERIC lpMsg, DWORD dwMsgSize, DPID idFrom )
{
	LPBYTE lpBuffer		= (LPBYTE)lpMsg + sizeof(DPID);
	u_long uBufSize		= dwMsgSize - sizeof(DPID);

	CAr ar( lpBuffer, uBufSize );
	GETTYPE( ar )

	void ( theClass::*pfn )( theParameters )	=	GetHandler( dw );

	if( pfn ) 
	{
		( this->*( pfn ) )( ar, idFrom, *(UNALIGNED LPDPID)lpMsg, lpBuffer, uBufSize );
	}

}

void CDPSrvr::OnAddConnection( DPID dpid )
{
}

void CDPSrvr::OnRemoveConnection( DPID dpid )
{
	// ���� ĳ�� ������ ���� ��쵵 �ְ�, �׽�Ʈ ������ telnet�� ���� ��쵵 �ִ�.
	// ĳ�������� ������ �����, ĳ�� ������ ����� �������� ����� �Ѵ�.
	// �� 2������ ����ؼ�, ������ ��ϵ� ���Ḹ�� ���� ĳ�������� �����ϰ�
	// ����ϰ� �Ѵ�.
	if( dpid == m_dpidCache )
	{
		g_UserMng.RemoveAllUsers();		
		m_dpidCache = DPID_UNKNOWN;	
	}
}

void CDPSrvr::OnAddUser( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long )
{
	m_dpidCache = dpidCache;			// ĳ�������� DPID�� �����Ѵ�.

	TCHAR	lpszAccount[MAX_ACCOUNT], lpszpw[MAX_PASSWORD], lpAddr[16];
	DWORD	dwAuthKey;
	u_long	idPlayer;
	BYTE	nSlot;
	DPID	dpidSocket;

	ar >> dwAuthKey >> idPlayer >> nSlot;
	ar >> dpidSocket;
	ar.ReadString( lpszAccount, MAX_ACCOUNT );
	ar.ReadString( lpszpw, MAX_PASSWORD );
	ar.ReadString( lpAddr, 16 );

	if( nSlot >= 3 )
		return;

	CUser* pUser = (CUser*)prj.GetUserByID( idPlayer );
	if( pUser )
	{
		//WriteLog( "CDPSrvr::OnAddUser idPlayer:%d account:%s DPID:%d", idPlayer, lpszAccount, dpidUser );
		// ĳ���������� socket��ȣ�� ������ �Ѵ�. ( pUser->m_Snapshot.dpidUser�� ���Ϲ�ȣ )
		QueryDestroyPlayer( pUser->m_Snapshot.dpidCache, pUser->m_Snapshot.dpidUser, pUser->m_dwSerial, pUser->m_idPlayer ); // pUser->m_Snapshot.dpidUser���� ���Ϲ�ȣ�� �� �ִ�.
		QueryDestroyPlayer( dpidCache, dpidSocket, dpidUser, idPlayer );	
		return;
	}

	pUser = g_UserMng.AddUser( dpidCache, dpidUser, dpidSocket );
	if( pUser == NULL )
		return;

	pUser->m_dwAuthKey = dwAuthKey;
	memcpy( pUser->m_playAccount.lpAddr, lpAddr, 16 );

	//	TRANS
	BEFORESENDDUAL( arJoin, PACKETTYPE_JOIN, dpidCache, dpidUser );
	arJoin << dwAuthKey;
	arJoin.WriteString( lpszAccount );
	arJoin.WriteString( lpszpw );
	arJoin << nSlot << idPlayer;
	SEND( arJoin, &g_dpDBClient, DPID_SERVERPLAYER );
}


void CDPSrvr::OnRemoveUser( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long )
{
	g_UserMng.RemoveUser( (DWORD)dpidUser ); // dpidUser�� CACHE���� ���Ǵ� serial�� �� 
}

void CDPSrvr::OnChat( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	static	TCHAR	sChat[1024];
	if( uBufSize > 1031 )	// 4 + 4 + 1024 - 1		= 1031
		return;

	ar.ReadString( sChat, 1024 );
	CString strChat	= sChat;
	strChat.Replace( "\\n", " " );
	
	CUser* pUser	=	g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) ) 
	{
		if( pUser->m_dwAuthorization >= AUTH_LOGCHATTING )		// �Ϲ������� �ƴϸ� �α׳��� ��� �α׳���
		{
			g_dpDBClient.SendLogGamemaChat( pUser, strChat );
		}

		if( sChat[0] == '/'  && ParsingCommand( strChat, (CMover*)pUser ) )
			return;
		if( pUser->IsMode( TALK_MODE ) )
			return;
#ifdef __JEFF_9_20
#if __VER >= 12 // __LORD
		int nText	= pUser->GetMuteText();
		if(  nText )
		{
			pUser->AddDefinedText( nText );
			return;
		}
#else	// __LORD
		if( pUser->IsMute() )
			return;
#endif	// __LORD
#endif	// __JEFF_9_20

		if( !( pUser->HasBuff( BUFF_ITEM, II_SYS_SYS_SCR_FONTEDIT ) ) )
		{
//			strChat		= sChat;
//			strChat.Replace( "\\n", " " );
			ParsingEffect( sChat, strlen(sChat) );
		}
		strChat	= sChat;
		strChat.Replace( "\\n", " " );
//		RemoveCRLF( sChat );

	
		g_UserMng.AddChat( pUser, strChat );
	}
}

void CDPSrvr::OnCtrlCoolTimeCancel( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser;
	CCommonCtrl* pCtrl;
	pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	
	if( IsValidObj( pUser ) == FALSE )
		return;
	
	((CMover*)pUser)->m_dwCtrlReadyTime = 0xffffffff;

	pCtrl = (CCommonCtrl*)prj.GetCtrl( ((CMover*)pUser)->m_dwCtrlReadyId );
	if( IsValidObj( pCtrl ) == FALSE )
		return;

	((CMover*)pUser)->m_dwCtrlReadyId = NULL_ID;

	pCtrl->m_dwCtrlReadyTime = 0xffffffff;
	pCtrl->m_bAction         = FALSE;
}

void CDPSrvr::OnDoEquip( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	DWORD nId;
	int nPart;

	ar >> nId;
	ar >> nPart;		
	
	if( nPart >= MAX_HUMAN_PARTS )	
		return;

	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) == FALSE )
		return;

	CItemElem* pItemElem = pUser->m_Inventory.GetAtId( nId );
	if( IsUsableItem( pItemElem ) == FALSE )
			return;
	if( nPart > 0 )
	{
		if( pUser->m_Inventory.IsEquip( nId ) )
		{
			if( pItemElem != pUser->m_Inventory.GetEquip( nPart ) )
				return;
		}
	}
	else
	{
		if( pUser->m_Inventory.IsEquip( nId ) )
			return;
	}
#ifdef __HACK_1023
	ItemProp* pItemProp		= pItemElem->GetProp();
	if( pItemProp && pItemProp->dwParts == PARTS_RIDE )
	{
		if( !pUser->m_Inventory.IsEquip( nId ) )
		{
			FLOAT fVal;
			ar >> fVal;
			if( fVal != pItemProp->fFlightSpeed )
			{
				pUser->AddDefinedText( TID_GAME_MODIFY_FLIGHT_SPEED );
				return;
			}
		}
	}
#endif	// __HACK_1023

	if( pUser->IsDie() == FALSE )
		pUser->DoUseEquipmentItem( pItemElem, nId, nPart );
}

void CDPSrvr::OnMoveItem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize)
{
	BYTE nItemType, nSrcIndex, nDstIndex;
	ar >> nItemType >> nSrcIndex >> nDstIndex;

	if( nSrcIndex == nDstIndex )
		return;
	if( nDstIndex >= MAX_INVENTORY || nSrcIndex >= MAX_INVENTORY )
		return;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) == FALSE )
		return;

	CItemElem* pItemSrc = pUser->m_Inventory.GetAt( nSrcIndex );
	CItemElem* pItemDst = pUser->m_Inventory.GetAt( nDstIndex );
	if( pItemDst == NULL || IsUsableItem( pItemDst ) ) // �� ���� or �ŷ������� �ʴ� ������ ?			
	{	
		if( IsUsableItem( pItemSrc ) )					// �ŷ������� �ʴ� ������ ?
		{
			pUser->m_Inventory.Swap( nSrcIndex, nDstIndex );
			pUser->AddMoveItem( 0, nSrcIndex, nDstIndex );
		}
	}
}

void CDPSrvr::OnDropItem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	DWORD dwItemType;
	DWORD dwItemId;
	short nDropNum;
	D3DXVECTOR3 vPos;

	ar >> dwItemType >> dwItemId >> nDropNum >> vPos;

	if( nDropNum <= 0 )
		return;

	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj(pUser) )
	{
		if( g_eLocal.GetState( EVE_DROPITEMREMOVE ) )
		{
			CItemElem* pItemElem	= (CItemElem*)pUser->GetItemId( dwItemId );
			if( IsUsableItem( pItemElem ) && pUser->IsDropable( pItemElem, FALSE ) )
				pUser->RemoveItem( (BYTE)dwItemId, nDropNum );
		}
		else
		{
			pUser->DropItem( (DWORD)dwItemId, nDropNum, vPos );
		}
	}
}

void CDPSrvr::OnDropGold( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{

#if __VER >= 8  	//8�����ӳ�����ӱ���

	return;

#else	//  __VER >= 8  

	DWORD dwGold;
	D3DXVECTOR3 vPos;
	ar >> dwGold >> vPos;

	BOOL bLimit = FALSE;
	if( dwGold > 30000 )	// 3�� ��� �̻��� ���� ����߸� �� �����ϴ�. 
	{
		dwGold = 30000;
		bLimit = TRUE;
	}

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		if( g_eLocal.GetState( EVE_DROPITEMREMOVE ) )
		{
			if( pUser->GetGold() >= (int)dwGold )	// raiders.2006.11.28
				pUser->AddGold( -dwGold );
		}
		else
		{
			if( pUser->DropGold( dwGold, vPos ) != NULL && bLimit )
				pUser->AddDefinedText( TID_GAME_LIMITMONEY, "" );
		}
	}

#endif	//	  __VER >= 8  

}


void CDPSrvr::OnReplace( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long )
{
	ASSERT( 0 );
	Error("CDPSrvr::OnReplace called\n");
}

void CDPSrvr::OnScriptDialogReq( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	static TCHAR lpKey[256];
	OBJID objid;
	int nGlobal1, nGlobal2, nGlobal3, nGlobal4;

	ar >> objid;
 	ar.ReadString( lpKey, 256 );

	ar >> nGlobal1 >> nGlobal2 >> nGlobal3 >> nGlobal4;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
#ifdef __QUEST_1208
		DWORD dwTickCount	= GetTickCount();
		if( dwTickCount < pUser->m_tickScript + 400 )
			return;
		pUser->m_tickScript	= dwTickCount;
#endif	// __QUEST_1208
		CMover* pMover	= prj.GetMover( objid );
		if( IsValidObj( pMover ) )
		{
#ifdef __HACK_1130
			char lpOutputString[512]	= { 0,};
			sprintf( lpOutputString, "npc = %s, key = %s, n1 = %d, n2 = %d, n3 = %d, n4 = %d",
				pMover->GetName(), lpKey, nGlobal1, nGlobal2, nGlobal3, nGlobal4 );
			OutputDebugString( lpOutputString );
#endif	// __HACK_1130

			D3DXVECTOR3 vOut	= pUser->GetPos() - pMover->GetPos();
			if( fabs( (double)D3DXVec3LengthSq( &vOut ) ) > MAX_LEN_MOVER_MENU )
			{
				return;
			}
		#if !defined(__REMOVE_SCIRPT_060712)
			CScriptDialog::SetLatestDialog( pMover->GetName(), lpKey );
		#endif
			if( pMover->m_pNpcProperty &&
				pMover->m_pNpcProperty->IsDialogLoaded() )
			{
				if( lstrlen( lpKey ) == 0 )
					lstrcpy( lpKey, _T( "#init" ) );
				if( nGlobal3 == 0 )	
					nGlobal3 = (int)pMover->GetId();
				if( nGlobal4 == 0 )	
					nGlobal4 = (int)pUser->GetId();

				pMover->m_pNpcProperty->RunDialog( lpKey, NULL, nGlobal1, (int)pMover->GetId(), (int)pUser->GetId(), nGlobal2 );

				// ����Ʈ ���� ��ȭ�� �´� Ű�� ���� ��ȭ ���� �÷��� �����ϰ� ����Ʈ ������ Ŭ���̾�Ʈ�� �����ش�.
				for( int i = 0; i < pUser->m_nQuestSize; i++ )
				{
					LPQUEST lpQuest = &pUser->m_aQuest[ i ];
					QuestProp* pQuestProp = pUser->m_aQuest[ i ].GetProp();
					if( pQuestProp )
					{
						if( strcmp( pQuestProp->m_szEndCondDlgCharKey, pMover->m_szCharacterKey ) == 0 )
						{
							if( strcmp( pQuestProp->m_szEndCondDlgAddKey, lpKey ) == 0 )
							{
								lpQuest->m_bDialog = TRUE;
								pUser->AddSetQuest( lpQuest );
								break;
							}
						}
					}
				}
			}
		}
	}
}

void CDPSrvr::OnRevival( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CWorld* pWorld;
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) && ( pWorld = pUser->GetWorld() ) )
	{
		pUser->m_Resurrection_Data.bUseing = FALSE;

		if( pUser->IsDie() == FALSE )
		{
			Error( "OnRevival\t// %s is not dead", pUser->GetName() );
			return;
		}
		
		BOOL bGuildCombat = FALSE;

		CItemElem* pItemElem = pUser->m_Inventory.GetAtItemId( II_SYS_SYS_SCR_RESURRECTION );
		if( IsUsableItem( pItemElem ) )
		{
			pUser->m_nDead = PROCESS_COUNT * 5;		// ���� �� 5�ʰ��� ����

#if __VER >= 8 // __S8_PK
			if( pUser->IsChaotic() )
			{
				pUser->SubDieDecExp( TRUE, 0, TRUE );		// �׾ ��Ȱ�ϸ� ��ġ ����,.
			}
			else
			{
#if __VER < 12 // __ANGEL_NODIE
				if( !pUser->m_bLastPK && !pUser->m_bGuildCombat )
					pUser->RemoveAngel();
#endif // __ANGEL_NODIE
			}
#endif // __VER >= 8 // __S8_PK
			float fRate = 0.1f;

#if __VER >= 9 // __S_9_ADD
			fRate = 0.2f;
#else // __S_9_ADD
			int nLevel	= pUser->GetLevel();
			if( nLevel == 1 )		{ fRate	= 0.8f; }
			else if( nLevel == 2 )	{ fRate	= 0.6f; }
			else if( nLevel <= 5 )	{ fRate	= 0.5f; }
			else if( nLevel <= 10 )	{ fRate	= 0.4f; }
			else if( nLevel <= 15 )	{ fRate	= 0.3f; }
#endif // __S_9_ADD

			pUser->m_pActMover->ClearState();

			if( pWorld->GetID() == WI_WORLD_GUILDWAR )
				fRate = 1.0f;

#if __VER < 9 // __S_9_ADD	// �Ʒ����� ä���ְ� ��Ŷ���� ���������� ������ ����
			pUser->SetHitPoint( (int)(pUser->GetMaxHitPoint() * fRate) );	// hp ȸ��

			int nVal	= (int)(pUser->GetMaxManaPoint() * fRate);			// mp ȸ��
			if( pUser->GetManaPoint() < nVal )
				pUser->SetManaPoint( nVal );
			
			nVal	= (int)(pUser->GetMaxFatiguePoint() * fRate);			// fp ȸ��
			if( pUser->GetFatiguePoint() < nVal )
				pUser->SetFatiguePoint( nVal );
#endif // __S_9_ADD

			if( pWorld->GetID() == WI_WORLD_GUILDWAR )
			{
				g_UserMng.AddHdr( pUser, SNAPSHOTTYPE_REVIVAL );
				g_GuildCombatMng.JoinObserver( pUser );
			}
			else
			{
				g_UserMng.AddHdr( pUser, SNAPSHOTTYPE_REVIVAL );

				ItemProp* pItemProp = pItemElem->GetProp();
				if( pItemProp && pItemProp->dwSfxObj3 != -1 )
					g_UserMng.AddCreateSfxObj((CMover *)pUser, pItemElem->GetProp()->dwSfxObj3 );

				// ���ȭ ������ ��� �α� ����
				g_dpDBClient.SendLogSMItemUse( "1", pUser, pItemElem, pItemProp );
				pUser->RemoveItem( (BYTE)pItemElem->m_dwObjId, 1 );
			}

#if __VER >= 9 // __S_9_ADD
			pUser->SetPointParam( DST_HP, (int)( pUser->GetMaxHitPoint() * fRate ) );
					
			int nVal	= (int)(pUser->GetMaxManaPoint() * fRate);			// mp ȸ��
			if( pUser->GetManaPoint() < nVal )
				pUser->SetPointParam( DST_MP, nVal );
			
			nVal	= (int)(pUser->GetMaxFatiguePoint() * fRate);			// fp ȸ��
			if( pUser->GetFatiguePoint() < nVal )
				pUser->SetPointParam( DST_FP, nVal );			
#endif // __S_9_ADD
		}
		else
		{
			Error( "Error CMover::OnRevival Not Inventory RevivalItem" );
		}
	}
}

void CDPSrvr::OnRevivalLodestar( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CWorld* pWorld;
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) && ( pWorld = pUser->GetWorld() ) )
	{
		pUser->m_Resurrection_Data.bUseing = FALSE;

		if( pUser->IsDie() == FALSE )
		{
			// �̸��� ������ ��찡 �����. ������ �̹� �ѹ� ���� ���Դٰ� �� �ɸ����� 
			// �ε彺Ÿ�� �� ������ �����°� �ƴѰ� �ʹ�.
			return;
		}

		CCommonCtrl* pCtrl	= CreateExpBox( pUser );
		if( pCtrl )
		{
			pCtrl->AddItToGlobalId();
			pWorld->ADDOBJ( pCtrl, FALSE, pUser->GetLayer() );
			g_dpDBClient.SendLogExpBox( pUser->m_idPlayer, pCtrl->GetId(), pCtrl->m_nExpBox );
		}
		
		BOOL bGuildCombat = FALSE;
		
		g_dpDBClient.SendLogLevelUp( (CMover*)pUser, 9 );	// �ε彺Ÿ�� ��Ȱ �α�

		pUser->m_nDead = PROCESS_COUNT * 5;		// ���� �� 5�ʰ��� ����
		float fRate		= pUser->SubDieDecExp();		// �׾ ��Ȱ�ϸ� ��ġ ����,.
		pUser->m_pActMover->ClearState();

		if( pWorld->GetID() == WI_WORLD_GUILDWAR )
			fRate = 1.0f;
#if __VER >= 11 // __GUILD_COMBAT_1TO1
		else if( g_GuildCombat1to1Mng.IsPossibleUser( pUser ) )
			fRate = 1.0f;
#endif // __GUILD_COMBAT_1TO1
#if __VER >= 9 // __S_9_ADD
		else
			fRate = 0.2f;
#endif // __S_9_ADD
		
#if __VER < 9 // __S_9_ADD	// �Ʒ����� ä���ְ� ��Ŷ���� ���������� ������ ����
		pUser->SetHitPoint( (int)(pUser->GetMaxHitPoint() * fRate) );	// hp ȸ��
		
		int nVal	= (int)(pUser->GetMaxManaPoint() * fRate);			// mp ȸ��
		if( pUser->GetManaPoint() < nVal )
			pUser->SetManaPoint( nVal );
		
		nVal	= (int)(pUser->GetMaxFatiguePoint() * fRate);			// fp ȸ��
		if( pUser->GetFatiguePoint() < nVal )
			pUser->SetFatiguePoint( nVal );
#endif // __S_9_ADD

		if( pWorld->GetID() == WI_WORLD_GUILDWAR )
		{
			g_UserMng.AddHdr( pUser, SNAPSHOTTYPE_REVIVAL );
			g_GuildCombatMng.JoinObserver( pUser );
		}
		else
		{
			g_UserMng.AddHdr( pUser, SNAPSHOTTYPE_REVIVAL_TO_LODESTAR );
		}		

#if __VER >= 9 // __S_9_ADD
		pUser->SetPointParam( DST_HP, (int)(pUser->GetMaxHitPoint() * fRate) );	// hp ȸ��
		
		int nVal	= (int)(pUser->GetMaxManaPoint() * fRate);			// mp ȸ��
		//if( pUser->GetManaPoint() < nVal )
			pUser->SetPointParam( DST_MP, nVal );
		
		nVal	= (int)(pUser->GetMaxFatiguePoint() * fRate);			// fp ȸ��
		//if( pUser->GetFatiguePoint() < nVal )
			pUser->SetPointParam( DST_FP, nVal );
#endif // __S_9_ADD

		if( pWorld->GetID() == WI_WORLD_GUILDWAR )
			return;

#if __VER >= 12 // __SECRET_ROOM
		if( CSecretRoomMng::GetInstance()->IsInTheSecretRoom( pUser ) )
		{
			pUser->REPLACE( g_uIdofMulti, WI_WORLD_MADRIGAL, CSecretRoomMng::GetInstance()->GetRevivalPos( pUser ), REPLACE_NORMAL, nDefaultLayer );
			return;
		}
#endif // __SECRET_ROOM
		
		// ������ �ʿ��� �׾���...
		// �׷��� ��Ȱ�� ��������...
		if( pWorld->GetID() == WI_DUNGEON_MUSCLE || pWorld->GetID() == WI_DUNGEON_KRRR || pWorld->GetID() == WI_DUNGEON_BEAR )
		{			
			pUser->REPLACE( g_uIdofMulti, WI_WORLD_MADRIGAL, D3DXVECTOR3( 6968.0f, 0.0f, 3328.8f ), REPLACE_NORMAL, nDefaultLayer );
			return;
		}

		PRegionElem	pRgnElem	= NULL;
		if( pUser->IsChaotic()
#ifdef __JEFF_11_4
			&& !pWorld->IsArena()
#endif	// __JEFF_11_4
			)
		{
#if __VER >= 8 // __S8_PK
			if( pWorld->GetID() != pWorld->m_dwIdWorldRevival && pWorld->m_dwIdWorldRevival != 0 )
				pRgnElem	= g_WorldMng.GetRevivalPosChao( pWorld->m_dwIdWorldRevival, pWorld->m_szKeyRevival );
			if( NULL == pRgnElem )	// Find near revival pos
				pRgnElem	= g_WorldMng.GetNearRevivalPosChao( pWorld->GetID(), pUser->GetPos() );	
#else // __VER >= 8 // __S8_PK
			pRgnElem	= g_WorldMng.GetNearRevivalPosChao( pWorld->GetID(), pUser->GetPos() );
#endif // __VER >= 8 // __S8_PK
		}
		else
		{
			if( pWorld->GetID() != pWorld->m_dwIdWorldRevival && pWorld->m_dwIdWorldRevival != 0 )
				pRgnElem	= g_WorldMng.GetRevivalPos( pWorld->m_dwIdWorldRevival, pWorld->m_szKeyRevival );
			
			if( NULL == pRgnElem )	// Find near revival pos
				pRgnElem	= g_WorldMng.GetNearRevivalPos( pWorld->GetID(), pUser->GetPos() );	
		}

		if( pRgnElem ) 
			pUser->REPLACE( g_uIdofMulti, pRgnElem->m_dwWorldId, pRgnElem->m_vPos, REPLACE_FORCE, nRevivalLayer );
		else 
			pUser->REPLACE( g_uIdofMulti, pWorld->GetID(), pUser->GetPos(), REPLACE_FORCE, pUser->GetLayer() );
	}
}

void CDPSrvr::OnRevivalLodelight( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
}

void CDPSrvr::OnSetLodelight( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		if( !CNpcChecker::GetInstance()->IsCloseNpc( MMI_MARKING, pUser->GetWorld(), pUser->GetPos() ) )
			return;
		pUser->SetMarkingPos();
		pUser->AddDefinedText( TID_GAME_LODELIGHT, "" );
	}
}

void CDPSrvr::OnIncStatLevel( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CHAR chID;
	ar >> chID;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		long nValue;
		
		switch( chID )
		{
			case 100:	
				pUser->IncStrLevel();
				nValue	= pUser->m_nStr;
				break;
			case 101:
				pUser->IncStaLevel();
				nValue	= pUser->m_nSta;
				break;
			case 102:
				pUser->IncDexLevel();
				nValue	= pUser->m_nDex;
				break;
			case 103:
				pUser->IncIntLevel();
				nValue	= pUser->m_nInt;
				break;
			default:
				return;			
		}
//		pUser->AddSetStatLevel( chID, nValue, pUser->m_nRemainGP );
		g_UserMng.AddSetStateLevel( pUser, chID, nValue );
		g_dpDBClient.SendLogLevelUp( pUser, 2 );
	}
}

void CDPSrvr::OnCorrReq( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	OBJID idObj;
	ar >> idObj;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );		// ��������κ��� ���ƿ°ų�.
	if( IsValidObj( pUser ) )
	{
		CMover *pMover = prj.GetMover( idObj );	// ���õ� ������Ʈ�� ������
		if( IsValidObj( pMover ) )
		{
			pUser->AddCorrReq( pMover );	// ��û�� Ŭ�󿡰� ���õ� ������Ʈ�� ������ ����.
		}
	}
}

void CDPSrvr::OnCreateGuildCloak( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	if( g_eLocal.GetState( ENABLE_GUILD_INVENTORY ) == FALSE )		
		return;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );		// ��������κ��� ���ƿ°ų�.
	if( IsValidObj( pUser ) )
	{
		CGuild *pGuild = pUser->GetGuild();
		if( pGuild == NULL )
			return;
		if( pGuild->m_dwLogo == 0 ||							//  �ΰ� �����Ǿ� ���� �ʰų�
			pGuild->IsMaster( pUser->m_idPlayer ) == FALSE )	// �����Ͱ� �ƴϰų�
		{
			if( pGuild->m_dwLogo == 0 )
			{
				// �αװ� �������� �ʾƼ� ������
				pUser->AddDefinedText( TID_GAME_GUILDSETTINGLOGO, "" );
			}
			else
			{
				// �����Ͱ� �ƴϹǷ� ������� ����.
				pUser->AddDefinedText( TID_GAME_GUILDONLYMASTERLOGO, "" );
			}
			return;
		}

		// ���â�� ���並 ���� ������ ������� üũ�Ѵ�. ���� ��� ���並 ��� â�� �������̴�. �ƴ϶�� �ּ�ó�� ���ּ���
		if ( MAX_BANK <= pGuild->m_GuildBank.GetCount() )
		{
			pUser->AddDefinedText( TID_GAME_GUILDBANKFULL, "" );		// ���â�� ��á����~
			return;
		}

		if ( pGuild->m_nGoldGuild >= 10000 )	 // ���â�� ���� ����ϳ�?
		{
			pGuild->m_nGoldGuild -= 10000;

			// ���â���� �� �����ϰ�, ��� ���並 ���â�� ������Ŵ.
//			BYTE nId;
			CItemElem itemElem;
			if( pGuild->m_dwLogo == 999 )	// Ŀ���� �ΰ�� �����Ǿ� ������.
				itemElem.m_dwItemId	= II_ARM_S_CLO_CLO_BLANK;		// Ŀ���ҿ� ��¥ �������.
			else
			{
				if(pUser->IsAuthHigher(AUTH_GAMEMASTER) && pGuild->m_dwLogo > CUSTOM_LOGO_MAX - 7) // GM Guild Logo��� �� ���Ƿ� ���� ����
					itemElem.m_dwItemId	= II_ARM_S_CLO_CLO_SYSCLOAK01;
				else
					itemElem.m_dwItemId	= II_ARM_S_CLO_CLO_SYSCLOAK01 + (pGuild->m_dwLogo - 1);
			}
			itemElem.m_nItemNum		= 1;
			// ��� ���̵� ���信 ����. Ŭ�󿡼� ���ڸ� ������� Ŀ���� ���並 �д´�.
			// Ŀ���� ���䰡 �ƴѰ��� �̹�ȣ�� ����̸��� �����ش�.
			itemElem.m_idGuild	= pGuild->m_idGuild;			
			

			// a. ��û�� Ŭ�󿡰� ���â�� ��İ� �Ҹ�Ǿ����� �˸���.
			// b. ���� ���� ������ �ִ� ���� ������ Ŭ���̾�Ʈ���� ��İ� �Ҹ�Ǿ����� �˸���.
			// c. �ٸ� ��Ƽ�����¿� �ִ� ���� ������ Ŭ���̾�Ʈ���� ��İ� �Ҹ�Ǿ����� �˸���.
			itemElem.SetSerialNumber();
			ItemProp* pItemProp		= itemElem.GetProp();
			if( pItemProp )
				itemElem.m_nHitPoint	= pItemProp->dwEndurance;
			else
				itemElem.m_nHitPoint	= 0;

			pUser->AddPutItemGuildBank( &itemElem );
			pGuild->m_GuildBank.Add( &itemElem );
			g_UserMng.AddPutItemElem( pUser, &itemElem );
						
			// �ڽ��� �������� ������ ���鼭 �����並 �缭 10000��İ� �Ҹ�Ǿ��ٰ� �˷��ش�.
			// ���� �������� ��û�� Ŭ���̾�Ʈ���Ե� �޽����� �Բ� ������.
			CGuildMember*	pMember;
			CUser*			pUsertmp;
			CGuild*			pGuild = pUser->GetGuild();
			for( map<u_long, CGuildMember*>::iterator i = pGuild->m_mapPMember.begin();
			i != pGuild->m_mapPMember.end(); ++i )
			{
				pMember		= i->second;
				pUsertmp	= (CUser*)prj.GetUserByID( pMember->m_idPlayer );
				if( IsValidObj( pUsertmp ) )
				{
					pUsertmp->AddGetGoldGuildBank( 10000, 2, pMember->m_idPlayer, 1 );	// 2�� ������Ʈ �ؾ��� Ŭ���̰�
				}
			}
			// �� ��Ƽ�� �������� �� ��ƾ�� ��� 10000��İ� �Ҹ���� �˷����Ƿ� DPCoreClient�� ĳ�ü����� ��û�Ͽ� 
			// ��� ��Ƽ�¿� 10000��İ� �Ҹ�Ǿ��ٰ� �˸���. ���� ������ �� ��Ƽ�� ������ �� �޽����� �����ؾ� �Ѵ�. ( �����ϰ� �س����� �� �ɷ��� -_- )
			g_DPCoreClient.SendGuildMsgControl_Bank_Penya( pUser, 10000, 2, 1 ); 	// 2�� ������Ʈ �ؾ��� �ٸ� ���弭���� Ŭ���̾�Ʈ
			UpdateGuildBank(pGuild, GUILD_CLOAK, 0, pUser->m_idPlayer, &itemElem, 10000, 1 ); // 0�� ��� ��ĸ� ������Ʈ �Ѵٴ� ���̴�.(���� ������ ������Ʈ������ -_-)
			pUser->AddDefinedText( TID_GAME_GUILDCREATECLOAK, "" );
		} 
		else
		{
			pUser->AddDefinedText( TID_GAME_GUILDNEEDGOLD, "" );		// ���â�� ���� ����!
		}
	}
}

void CDPSrvr::OnQueryGetDestObj( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	OBJID objid;
	ar >> objid;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		CMover* pMover	= prj.GetMover( objid );
		if( IsValidObj( pMover ) && !pMover->IsEmptyDestObj() )
			pUser->AddGetDestObj( objid, pMover->GetDestId(), pMover->m_fArrivalRange );
	}
}

void CDPSrvr::OnGetDestObj( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	OBJID objid, objidDest;
	ar >> objid >> objidDest;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
		pUser->SetDestObj( objidDest );
/*
	{
		if( NULL_ID == objid )
		{
			pUser->SetDestObj( objidDest );
		}
		else
		{
			CUser* ptr	= prj.GetUser( objid );
			if( IsValidObj( ptr ) )
				ptr->AddGetDestObj( pUser->GetId(), objidDest, ptr->m_fArrivalRange );
		}
	}
*/
}

void CDPSrvr::OnQueryGetPos( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	OBJID objid;
	ar >> objid;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		CMover* pMover	= prj.GetMover( objid );
		if( IsValidObj( pMover ) )
		{
			if( FALSE == pMover->IsPlayer() )
			{
				pUser->AddGetPos( objid, pMover->GetPos(), pMover->GetAngle() );
			}
			else
			{
				( (CUser*)pMover )->AddQueryGetPos( pUser->GetId() );
			}
		}
	}
}

void CDPSrvr::OnGetPos( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	D3DXVECTOR3 vPos;
	float fAngle;
	OBJID objid;
	ar >> vPos >> fAngle >> objid;
	
	if( _isnan((double)fAngle) )
		return;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		D3DXVECTOR3 vDistance	= pUser->GetPos() - vPos;
		if( D3DXVec3LengthSq( &vDistance ) > 1000000.0F )
		{
//			Error( "PACKETTYPE_GETPOS" );
			return;
		}

		if( NULL_ID == objid )
		{
			pUser->SetPos( vPos );
			pUser->SetAngle( fAngle );
			
			pUser->m_fWaitQueryGetPos	= FALSE;
			
			if( FALSE == pUser->IsEmptyDestPos() )
				pUser->SetDestPos( pUser->GetDestPos(), pUser->m_bForward, FALSE );
		}
		else
		{
			CUser* ptr	= prj.GetUser( objid );
			if( IsValidObj( ptr ) )
				ptr->AddGetPos( pUser->GetId(), vPos, fAngle );
		}
	}
}

void CDPSrvr::OnPartyRequest( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	u_long uLeaderid, uMemberid;
	BOOL bTroup;
	ar >> uLeaderid >> uMemberid;
	ar >> bTroup;

	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) && pUser->m_idPlayer == uLeaderid )
		InviteParty( uLeaderid, uMemberid, bTroup );
}

void CDPSrvr::OnPartyRequestCancle( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	u_long uLeaderid, uMemberid;
	int nMode;
	ar >> uLeaderid >> uMemberid >> nMode;

	CUser* pUser = g_UserMng.GetUserByPlayerID( uLeaderid );
	if( IsValidObj( pUser ) )
		pUser->AddPartyRequestCancel( uLeaderid, uMemberid, nMode );
}

void CDPSrvr::OnPartySkillUse( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	u_long uLeaderid;
	int nSkill;
	ar >> uLeaderid >> nSkill;	

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsInvalidObj( pUser ) )		
		return;
	
	CParty* pParty	= g_PartyMng.GetParty( pUser->m_idparty );
	if( pParty )
	{
		if( pParty->IsLeader( pUser->m_idPlayer ) )
			pParty->DoUsePartySkill( pUser->m_idparty, pUser->m_idPlayer, nSkill );
	}
}

void CDPSrvr::OnAddFriendReqest( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	u_long uLeaderid, uMemberid;
	ar >> uLeaderid >> uMemberid;
	
	CUser* pLeader	= g_UserMng.GetUserByPlayerID( uLeaderid );
	CUser* pMember	= g_UserMng.GetUserByPlayerID( uMemberid );
	if( IsValidObj( pMember ) && IsValidObj( pLeader ) )
	{

#if __VER >= 8 // 8�� ��� 061226 ma
		if( 0 < pLeader->m_nDuel ||  0 < pMember->m_nDuel )
		{
			return;
		}
#endif // __VER >= 8 // 8�� ��� 061226 ma

		// �������忡�� ģ���߰��� �Ҽ� �����ϴ�
		CWorld* pWorldLeader = pLeader->GetWorld();
		CWorld* pWorldMember = pMember->GetWorld();
		if( ( pWorldLeader && pWorldLeader->GetID() == WI_WORLD_GUILDWAR ) ||
			( pWorldMember && pWorldMember->GetID() == WI_WORLD_GUILDWAR ) )
		{			
			pLeader->AddText( prj.GetText(TID_GAME_GUILDCOMBAT_CANNOT_FRIENDADD) );
			return;
		}
#if __VER >= 11 // __GUILD_COMBAT_1TO1
		if( g_GuildCombat1to1Mng.IsPossibleUser( pLeader ) || g_GuildCombat1to1Mng.IsPossibleUser( pMember ) )
		{
			pLeader->AddText( prj.GetText(TID_GAME_GUILDCOMBAT_CANNOT_FRIENDADD) );
			return;
		}
#endif // __GUILD_COMBAT_1TO1

#ifdef __RT_1025
		if( !pLeader->m_RTMessenger.GetFriend( uMemberid ) )
#else	// __RT_1025
		if( FALSE == pLeader->m_Messenger.IsFriend( uMemberid ) )
#endif	// __RT_1025
		{
			if( pMember->IsAttackMode() )
				pLeader->AddDefinedText( TID_GAME_BATTLE_NOTFRIEND, "" );
			else
				pMember->AddFriendReqest( uLeaderid, pLeader->m_nJob, (BYTE)pLeader->GetSex(), pLeader->GetName() );	// ģ�� ��� ���� ����
		}
	}
}

// �ٸ� ��Ƽ������ �ִ� ĳ���� �߰� ��Ű���� �ھ�� ������ �Ѵ�.
// �̸����� ���Ƿ� ���忡�� idPlayer�� �ٲ㼭 ����
void CDPSrvr::OnAddFriendNameReqest( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	u_long uLeaderid, uMember;
	char szMemberName[64] = {0,};

	ar >> uLeaderid;
	ar.ReadString( szMemberName, 64 );

#if __VER >= 11 // __SYS_PLAYER_DATA
	uMember	= CPlayerDataCenter::GetInstance()->GetPlayerId( szMemberName );
#else	// __SYS_PLAYER_DATA
	uMember = prj.GetPlayerID( szMemberName );
#endif	// __SYS_PLAYER_DATA
	CUser* pLeader	= g_UserMng.GetUserByPlayerID( uLeaderid );	

	if( IsValidObj( pLeader ) )
	{
		if( uMember > 0 )
		{
#ifdef __RT_1025
			if( !pLeader->m_RTMessenger.GetFriend( uMember ) )
#else	// __RT_1025
			if( pLeader->m_Messenger.IsFriend( uMember ) == FALSE )
#endif	// __RT_1025
				g_DPCoreClient.SendAddFriendNameReqest( uLeaderid, pLeader->m_nJob, (BYTE)pLeader->GetSex(), uMember, pLeader->GetName(), szMemberName );
			else
				pLeader->AddFriendError( 1, szMemberName );
		}
		else
		{
			// �� �̸��� ������ �մ� ĳ���� ����.
			pLeader->AddFriendError( 2, szMemberName );
		}
	}
}


void CDPSrvr::OnAddFriendCancel( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	u_long uLeaderid, uMemberid;
	ar >> uLeaderid >> uMemberid;

	CUser* pLeader = g_UserMng.GetUserByPlayerID( uLeaderid );
	if( IsValidObj( pLeader ) )
		pLeader->AddFriendCancel();	// uMemberid
}

void CDPSrvr::OnActionPoint( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	int nAP;
	ar >> nAP;

	if( nAP < 0 )
		return;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		pUser->m_playTaskBar.m_nActionPoint = nAP;
	}
}


void CDPSrvr::OnRemoveQuest( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	DWORD dwQuestCancelID;
	ar >> dwQuestCancelID;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
#ifdef __QUEST_1208
		DWORD dwTickCount	= GetTickCount();
		if( dwTickCount < pUser->m_tickScript + 400 )
			return;
		pUser->m_tickScript	= dwTickCount;
#endif	// __QUEST_1208
		LPQUEST lpQuest = pUser->GetQuest( dwQuestCancelID );
		if( lpQuest )
		{
			if( lpQuest->m_nState != QS_END )
			{
				QuestProp* pQuestProp	= prj.m_aPropQuest.GetAt( lpQuest->m_wId );
				if( pQuestProp && pQuestProp->m_bNoRemove == FALSE )
				{
					pUser->RemoveQuest( dwQuestCancelID );
					pUser->AddCancelQuest( dwQuestCancelID );
					g_dpDBClient.CalluspLoggingQuest( pUser->m_idPlayer, dwQuestCancelID, 30 );
					// ���۽� ������ ������ ����Ʈ ������ ���� ������Ų��.
					if( pQuestProp->m_nBeginSetDisguiseMoverIndex )
					{
						pUser->NoDisguise();
						g_UserMng.AddNoDisguise( pUser );
					}
				}
			}
		}
	}
}

#if __VER >= 11 // __SYS_PLAYER_DATA
void CDPSrvr::OnQueryPlayerData( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	u_long idPlayer;
	ar >> idPlayer;
	int nVer;
	ar >> nVer;
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		PlayerData* pPlayerData	= CPlayerDataCenter::GetInstance()->GetPlayerData( idPlayer );
		if( pPlayerData && pPlayerData->data.nVer != nVer )
			pUser->AddQueryPlayerData( idPlayer, pPlayerData );
	}
}

void CDPSrvr::OnQueryPlayerData2( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	int nSize;
//	u_long idPlayer;
	ar >> nSize;

	if( nSize > 1024 )
		return;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		for( int i = 0; i < nSize; i++ )
		{
			PDVer	pdv;
			ar.Read( &pdv, sizeof(PDVer) );
			PlayerData* pPlayerData		= CPlayerDataCenter::GetInstance()->GetPlayerData( pdv.idPlayer );
			if( pPlayerData && pPlayerData->data.nVer != pdv.nVer )
				pUser->AddQueryPlayerData( pdv.idPlayer, pPlayerData );
		}
	}
}
#else	// __SYS_PLAYER_DATA
void CDPSrvr::OnQueryPlayerString( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	u_long idPlayer;
	BYTE nQuery;
	ar >> idPlayer >> nQuery;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		LPCSTR lpszPlayer	= prj.GetPlayerString( idPlayer );
		if( lpszPlayer )
			pUser->AddQueryPlayerString( idPlayer, lpszPlayer, nQuery );
	}
}

void CDPSrvr::OnQueryPlayerListString( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	int nSize;
	u_long idPlayer;
	BYTE nQuery;
	ar >> nSize >> nQuery;

	if( nSize > 1024 )
		return;
		
	CUser* pUser;

	pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	
	if( IsValidObj( pUser ) )
	{
		for( int i=0; i<nSize; i++ )
		{
			ar >> idPlayer;
			
			LPCSTR lpszPlayer	= prj.GetPlayerString( idPlayer );
			if( lpszPlayer )
			{
				pUser->AddQueryPlayerString( idPlayer, lpszPlayer, nQuery );
			}
			else
			{
				pUser->AddQueryPlayerString( idPlayer, "Unknown", nQuery );
			}
		}

		if( nSize > 0 )
			pUser->AddQueryPlayerListString( nQuery );
	}
}

void CDPSrvr::OnGetFriendName( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	int nCount;
	ar >> nCount;		
	if( nCount <= 0 || nCount > MAX_FRIEND )
		return;

	FRIEND aFriend[MAX_FRIEND];
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		for( int i = 0 ; i < nCount ; i++ )
		{
			ar >> aFriend[i].dwUserId;
#if __VER >= 11 // __SYS_PLAYER_DATA
//			LPCSTR lpszPlayer	= CPlayerDataCenter::GetInstance()->GetPlayerString( aFriend[i].dwUserId );
#else	// __SYS_PLAYER_DATA
			LPCSTR lpszPlayer	= prj.GetPlayerString( aFriend[i].dwUserId );
			strcpy( aFriend[i].szName, lpszPlayer? lpszPlayer: "" );
#endif	// __SYS_PLAYER_DATA
			
		}
		pUser->AddGetFriendName( nCount, aFriend );
	}
}
#endif	// __SYS_PLAYER_DATA

void CDPSrvr::OnGuildInvite( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	OBJID objid;
	ar >> objid;
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( !IsValidObj( pUser ) )
		return;

	InviteCompany( pUser, objid );
}

void CDPSrvr::OnIgnoreGuildInvite( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	u_long idPlayer;
	ar >> idPlayer;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		CUser* pPlayer	= g_UserMng.GetUserByPlayerID( idPlayer );		// kingpin
		if( IsValidObj( pPlayer ) )
		{
			pPlayer->AddDefinedText( TID_GAME_COMACCEPTDENY, "%s", pUser->GetName( TRUE ) );
		}
	}
}

// �ΰ� ���� 
void CDPSrvr::OnGuildLogo( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	DWORD dwLogo;
	ar >> dwLogo;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) == FALSE )
		return;

	if( dwLogo > CUSTOM_LOGO_MAX )
		return;

	if( dwLogo > 20 && !pUser->IsAuthHigher( AUTH_GAMEMASTER ) )
		return;

	g_DPCoreClient.SendGuildStat( pUser, GUILD_STAT_LOGO, dwLogo );
}

// ���嵵 
void CDPSrvr::OnGuildContribution( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	BYTE cbPxpCount, cbItemFlag;
	int nGold;

	cbItemFlag = 0;
	ar >> cbPxpCount >> nGold;
	ar >> cbItemFlag;

	if( g_eLocal.GetState( ENABLE_GUILD_INVENTORY ) == FALSE )
		return;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) == FALSE ) 
		return;

	if( nGold > 0 )
	{
		if( pUser->GetGold() >= nGold ) 
		{
			if( g_DPCoreClient.SendGuildStat( pUser, GUILD_STAT_PENYA, nGold ) )
			{
				pUser->AddGold( -nGold );

				LogItemInfo aLogItem;
				aLogItem.Action = "W";
				aLogItem.SendName = pUser->GetName();
				aLogItem.RecvName = "GUILDBANK";
				aLogItem.WorldId = pUser->GetWorld()->GetID();
				aLogItem.Gold = pUser->GetGold() + nGold;
				aLogItem.Gold2 = pUser->GetGold();
				//aLogItem.ItemName = "SEED";
				_stprintf( aLogItem.szItemName, "%d", II_GOLD_SEED1 );
				aLogItem.itemNumber = nGold;
				OnLogItem( aLogItem );
			}
		} 
		else
		{
			pUser->AddDefinedText( TID_GAME_GUILDNOTENGGOLD, "" );	// �κ��� ���̺���
		}
	}
	else if( cbItemFlag )
	{
		for( int i=0; i< pUser->m_Inventory.GetMax(); ++i )
		{
			CItemElem* pItemElem = pUser->m_Inventory.GetAtId( i );
			if( IsUsableItem( pItemElem ) == FALSE )
				continue;

			if( pItemElem->GetProp()->dwItemKind3 != IK3_GEM )
				continue;

			int nValue = 0;

			if( pItemElem->m_nItemNum > 0 )
			{
				// ������ ������ ���� ���嵵�� �ٸ��� �Ѵ�.
				nValue = (pItemElem->GetProp()->dwItemLV + 1) / 2;	
				nValue *= pItemElem->m_nItemNum;					
			}

			if( nValue > 0 )
			{
				if( g_DPCoreClient.SendGuildStat( pUser, GUILD_STAT_PXPCOUNT, nValue ) )
				{
					LogItemInfo aLogItem;
					aLogItem.Action = "V";
					aLogItem.SendName = pUser->GetName();
					aLogItem.RecvName = "GUILDBANK";
					aLogItem.WorldId = pUser->GetWorld()->GetID();
					aLogItem.Gold = aLogItem.Gold2 = pUser->GetGold();
					OnLogItem( aLogItem, pItemElem, pItemElem->m_nItemNum );
					pUser->RemoveItem( (BYTE)i, pItemElem->m_nItemNum );
				}
			}
		}

	}
}

// ��������
void CDPSrvr::OnGuildNotice( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	char szNotice[MAX_BYTE_NOTICE];
	ar.ReadString( szNotice, MAX_BYTE_NOTICE );
	szNotice[MAX_BYTE_NOTICE-1] = '\0';

	if( strlen( szNotice ) == 0 )
		return;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) == FALSE )
		return;

	g_DPCoreClient.SendGuildStat( pUser, GUILD_STAT_NOTICE, (DWORD)szNotice );
}

void CDPSrvr::OnDuelRequest( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	u_long uidSrc, uidDst;
	ar >> uidSrc >> uidDst;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	CUser* pDstUser = g_UserMng.GetUserByPlayerID( uidDst );
	if( IsValidObj( pUser ) && IsValidObj( pDstUser ) )
	{
		if( 0 < pUser->m_idparty && pUser->m_idparty == pDstUser->m_idparty )
		{
			pUser->AddDefinedText( TID_PK_PARTY_LIMIT, "" );	// ������Ƽ��
			return;
		}

#if __VER >= 8 // 8�� ��� 061226 ma
		if( 0 < pUser->m_idparty ||  0 < pDstUser->m_idparty )
			return;
		if( pUser->m_vtInfo.GetOther() )	// �ŷ��� �̸� ��� �Ұ� 
			return;
		if( pDstUser->m_vtInfo.GetOther() )	// �ŷ��� �̸� ��� �Ұ� 
			return;
	
		int	nState = pUser->GetSummonState();
		if( nState != 0 )
			return;
		nState = pUser->GetSummonState();
		if( nState != 0 )
			return;

#endif // __VER >= 8 // 8�� ��� 061226 ma

		if( pUser->IsPVPInspection( pDstUser, 1 ) )
		{
			if( pDstUser->IsMode( PVPCONFIRM_MODE ) )
			{
				pUser->AddDefinedText( TID_PK_MODE_REJECT, "" );	// PVP���� ����Դϴ�
			}
			else
			{
#ifdef __HACK_1130
				pUser->m_tmDuelRequest	= GetTickCount();
#endif	// __HACK_1130
				pDstUser->AddDuelRequest( uidSrc, uidDst );
			}
		}
	}
}

// ���¶��� ����.  ��ĳ���Ϳ��� �����϶�� ������� ��.
void CDPSrvr::OnDuelYes( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	u_long uidSrc, uidDst;
	ar >> uidSrc >> uidDst;

	CUser* pSrc = g_UserMng.GetUserByPlayerID( uidSrc );
#ifdef __HACK_1130
	CUser* pDst	=	g_UserMng.GetUser( dpidCache, dpidUser );
#else	// __HACK_1130
	CUser* pDst = g_UserMng.GetUserByPlayerID( uidDst );
#endif	// __HACK_1130

	if( IsValidObj(pSrc) && IsValidObj(pDst) )
	{
		if( 0 < pSrc->m_idparty && pSrc->m_idparty == pDst->m_idparty )
		{
			pSrc->AddDefinedText( TID_PK_PARTY_LIMIT, "" );	// ������Ƽ��
			pDst->AddDefinedText( TID_PK_PARTY_LIMIT, "" );	// ������Ƽ��
			return;
		}

#if __VER >= 8 // 8�� ��� 061226 ma
		if( 0 < pSrc->m_idparty ||  0 < pDst->m_idparty )
		{
			return;
		}
		//���λ��� �߿��� ��� �Ұ� 
		if( pSrc->m_vtInfo.VendorIsVendor() || pSrc->m_vtInfo.IsVendorOpen() ||
            pDst->m_vtInfo.VendorIsVendor() || pDst->m_vtInfo.IsVendorOpen() )
		{
			return;	//
		}
#endif // __VER >= 8 // 8�� ��� 061226 ma


		if( pSrc->IsPVPInspection( pDst, 1 ) )
		{
#ifdef __HACK_1130
			if( pSrc->m_tmDuelRequest + SEC( 10 ) < GetTickCount() )	// ��� ��û �ð��� 10�� �ʰ��ϸ�
			{
				pSrc->m_tmDuelRequest	= 0;
				return;
			}
#endif	// __HACK_1130
			pSrc->m_nDuel = 1;
			pSrc->m_nDuelState = 104;
			pSrc->m_idDuelOther = pDst->GetId();
			pDst->m_nDuel = 1;
			pDst->m_nDuelState = 104;
			pDst->m_idDuelOther = pSrc->GetId();
			pSrc->AddDuelStart( uidDst );	// ���� ���濡 ���� ���̵� �����ָ� �ȴ�.
			pDst->AddDuelStart( uidSrc );
			pSrc->m_dwTickEndDuel = ::timeGetTime() + NEXT_TICK_ENDDUEL;
			pDst->m_dwTickEndDuel = ::timeGetTime() + NEXT_TICK_ENDDUEL;
			pSrc->SetPosChanged( TRUE );
			pDst->SetPosChanged( TRUE );
		}
	}
}

// pUser�� ��� ��û�� �ź��ߴ�. pSrc���� �˷��� �Ѵ�.
void CDPSrvr::OnDuelNo( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	u_long uidSrc; //, uidDst;
	ar >> uidSrc;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj(pUser) )
	{
		CUser* pSrc = g_UserMng.GetUserByPlayerID( uidSrc );
		if( IsValidObj(pSrc) )
			pSrc->AddDuelNo( pUser->GetId() );	// pSrc���� pUser�� �ź��ߴٴ°� �˸�.
	}
}

// ��Ƽ��� ----------------------------------------------------------------
// Src�� Dst���� ���� ���ڰ� ��û�ؿԴ�.
void CDPSrvr::OnDuelPartyRequest( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	u_long uidSrc, uidDst;
	ar >> uidSrc >> uidDst;
	
	CUser* pSrcUser	= g_UserMng.GetUser( dpidCache, dpidUser );	// ��û�� ����.
	CUser* pDstUser	= g_UserMng.GetUserByPlayerID( uidDst );	// ��� ����
	if( IsValidObj( pDstUser ) && IsValidObj( pSrcUser ) )
	{
		if( pDstUser->IsMode( PVPCONFIRM_MODE ) )
		{
			pSrcUser->AddDefinedText( TID_PK_MODE_REJECT, "" );	// PVP���� ����Դϴ�
		}
		else
		{

			if( pSrcUser->IsPVPInspection( pDstUser, 2 ) )
			{
				CParty* pSrcParty = g_PartyMng.GetParty( pSrcUser->m_idparty );		// ��û���� ��Ƽ����
				if( pSrcParty == NULL || pSrcParty->IsLeader( pSrcUser->m_idPlayer ) == FALSE )
				{
					pSrcUser->AddDefinedText( TID_PK_NO_IPARTYLEADER, "" );	// ��Ƽ���� �ƴմϴ�
					return;
				}
				CParty *pDstParty = g_PartyMng.GetParty( pDstUser->m_idparty );		// �����޴����� ��Ƽ����.
				if( pDstParty == NULL || pDstParty->IsLeader( pDstUser->m_idPlayer ) == FALSE )
				{
					pSrcUser->AddDefinedText( TID_PK_NO_UPARTYLEADER, "" );		// ������ ��Ƽ���� �ƴմϴ�
					return;
				}
				pDstUser->AddDuelPartyRequest( uidSrc, uidDst );
			}
		}

	}
}

// ��Ƽ���¶��� ����.  ��� ���� ��Ƽ���鿡�� ����� ���۵��� �˸�.
void CDPSrvr::OnDuelPartyYes( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	u_long uidSrc, uidDst;
	ar >> uidSrc >> uidDst;
	
	CUser* pSrc = g_UserMng.GetUserByPlayerID( uidSrc );	
	CUser* pDst = g_UserMng.GetUserByPlayerID( uidDst );
	if( IsValidObj(pSrc) && IsValidObj(pDst) )
	{

		if( pSrc->IsPVPInspection( pDst, 2 ) )
		{
			CParty* pSrcParty = g_PartyMng.GetParty( pSrc->m_idparty );		// ��û���� ��Ƽ����
			if( pSrcParty == NULL || pSrcParty->IsLeader( pSrc->m_idPlayer ) == FALSE )
			{
				Error( "CDPSrvr::OnDuelPartyYes : ��û�� ��Ƽ �б� ���� %d %s", pSrc->m_idparty, pSrc->GetName() );
				return;
			}
			CParty *pDstParty = g_PartyMng.GetParty( pDst->m_idparty );		// �����޴����� ��Ƽ����.
			//			if( pDstParty == NULL || pDstParty->IsMember( pDst->m_idPlayer ) == FALSE )
			if( pDstParty == NULL || pDstParty->IsLeader( pDst->m_idPlayer ) == FALSE )
			{
				Error( "CDPSrvr::OnDuelPartyYes : ��� ��Ƽ �б� ���� %d %s", pDst->m_idparty, pDst->GetName() );
				return;
			}

			pSrc->m_dwTickEndDuel = ::timeGetTime() + NEXT_TICK_ENDDUEL;
			pDst->m_dwTickEndDuel = ::timeGetTime() + NEXT_TICK_ENDDUEL;
			pSrc->SetPosChanged( TRUE );	// UpdateRegionAttr
			pDst->SetPosChanged( TRUE );

			g_DPCoreClient.SendSetPartyDuel( pSrcParty->m_uPartyId, pDstParty->m_uPartyId, TRUE );

			pSrcParty->DoDuelPartyStart( pDstParty );		// �����Ƽ�� ������ ���۵ƴٴ°� ����.
			pDstParty->DoDuelPartyStart( pSrcParty );		// �����Ƽ�� ������ ���۵ƴٴ°� ����.
		}
	}
}

// pUser�� ��� ��û�� �ź��ߴ�. pSrc���� �˷��� �Ѵ�.
void CDPSrvr::OnDuelPartyNo( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	u_long uidSrc; //, uidDst;
	ar >> uidSrc;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj(pUser) )
	{
		CUser* pSrc = g_UserMng.GetUserByPlayerID( uidSrc );
		if( IsValidObj(pSrc) )
			pSrc->AddDuelPartyNo( pUser->GetId() );		// pSrc���� pUser�� �ź��ߴٴ°� �˸�.
	}
}

void CDPSrvr::OnMoverFocus( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	u_long uidPlayer;
	ar >> uidPlayer;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj(pUser) )
	{
		CUser* pFocus = g_UserMng.GetUserByPlayerID( uidPlayer );
		if( IsValidObj(pFocus) )
			pUser->AddMoverFocus( pFocus );
	}
}

void CDPSrvr::OnSkillTaskBar( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	int nCount;
	ar >> nCount;

	if( nCount > MAX_SLOT_QUEUE )
		return;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		for( int i = 0 ; i < nCount ; i++)
		{
			BYTE nIndex;
			ar >> nIndex;

			if( nIndex >= MAX_SLOT_QUEUE )
				return;

			ar >> pUser->m_playTaskBar.m_aSlotQueue[nIndex].m_dwShortcut >> pUser->m_playTaskBar.m_aSlotQueue[nIndex].m_dwId >> pUser->m_playTaskBar.m_aSlotQueue[nIndex].m_dwType;
			ar >> pUser->m_playTaskBar.m_aSlotQueue[nIndex].m_dwIndex >> pUser->m_playTaskBar.m_aSlotQueue[nIndex].m_dwUserId >> pUser->m_playTaskBar.m_aSlotQueue[nIndex].m_dwData;
		}
	}
}
void CDPSrvr::OnAddAppletTaskBar( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	BYTE nIndex;
	ar >> nIndex;

	if( nIndex >= MAX_SLOT_APPLET )
		return;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		ar >> pUser->m_playTaskBar.m_aSlotApplet[nIndex].m_dwShortcut >> pUser->m_playTaskBar.m_aSlotApplet[nIndex].m_dwId >> pUser->m_playTaskBar.m_aSlotApplet[nIndex].m_dwType;
		ar >> pUser->m_playTaskBar.m_aSlotApplet[nIndex].m_dwIndex >> pUser->m_playTaskBar.m_aSlotApplet[nIndex].m_dwUserId >> pUser->m_playTaskBar.m_aSlotApplet[nIndex].m_dwData;
		if( pUser->m_playTaskBar.m_aSlotApplet[nIndex].m_dwShortcut == SHORTCUT_CHAT )
		{
			ar.ReadString( pUser->m_playTaskBar.m_aSlotApplet[nIndex].m_szString, MAX_SHORTCUT_STRING );
			pUser->m_playTaskBar.m_aSlotApplet[nIndex].m_szString[MAX_SHORTCUT_STRING-1] = '\0';
		}
	}
}
void CDPSrvr::OnRemoveAppletTaskBar( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	BYTE nIndex;
	ar >> nIndex;

	if( nIndex >= MAX_SLOT_APPLET )
		return;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		if( pUser->m_playTaskBar.m_aSlotApplet[nIndex].m_dwShortcut == SHORTCUT_CHAT )
		{
			memset( pUser->m_playTaskBar.m_aSlotApplet[nIndex].m_szString, 0, sizeof( pUser->m_playTaskBar.m_aSlotApplet[nIndex].m_szString ) );
		}
		pUser->m_playTaskBar.m_aSlotApplet[nIndex].m_dwShortcut = SHORTCUT_NONE;
	}
}
void CDPSrvr::OnAddItemTaskBar( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	BYTE nSlotIndex, nIndex;
	ar >> nSlotIndex >> nIndex;

	if( nSlotIndex >= MAX_SLOT_ITEM_COUNT || nIndex >= MAX_SLOT_ITEM )
		return;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
#if __VER >= 11 // __CSC_VER11_5
		DWORD dwShortCut;
		ar >> dwShortCut;

		// Chat Shortcut 10���� ����
		if(dwShortCut == SHORTCUT_CHAT)
		{
			int nchatshortcut = 0;
			for( int i=0; i<MAX_SLOT_ITEM_COUNT; i++ )
			{
				for( int j=0; j<MAX_SLOT_ITEM; j++ )
				{
					if( pUser->m_playTaskBar.m_aSlotItem[i][j].m_dwShortcut == SHORTCUT_CHAT )
						nchatshortcut++;
				}
			}

			if(nchatshortcut > 9)
			{
				pUser->AddDefinedText( TID_GAME_MAX_SHORTCUT_CHAT );
				return;
			}
		}

		pUser->m_playTaskBar.m_aSlotItem[nSlotIndex][nIndex].m_dwShortcut = dwShortCut;
	 	ar >> pUser->m_playTaskBar.m_aSlotItem[nSlotIndex][nIndex].m_dwId >> pUser->m_playTaskBar.m_aSlotItem[nSlotIndex][nIndex].m_dwType;
#else //__CSC_VER11_5
		ar >> pUser->m_playTaskBar.m_aSlotItem[nSlotIndex][nIndex].m_dwShortcut >> pUser->m_playTaskBar.m_aSlotItem[nSlotIndex][nIndex].m_dwId >> pUser->m_playTaskBar.m_aSlotItem[nSlotIndex][nIndex].m_dwType;
#endif //__CSC_VER11_5
		ar >> pUser->m_playTaskBar.m_aSlotItem[nSlotIndex][nIndex].m_dwIndex >> pUser->m_playTaskBar.m_aSlotItem[nSlotIndex][nIndex].m_dwUserId >> pUser->m_playTaskBar.m_aSlotItem[nSlotIndex][nIndex].m_dwData;
		if( pUser->m_playTaskBar.m_aSlotItem[nSlotIndex][nIndex].m_dwShortcut == SHORTCUT_CHAT )
		{
			ar.ReadString( pUser->m_playTaskBar.m_aSlotItem[nSlotIndex][nIndex].m_szString, MAX_SHORTCUT_STRING );
			pUser->m_playTaskBar.m_aSlotItem[nSlotIndex][nIndex].m_szString[MAX_SHORTCUT_STRING-1] = '\0';
		}
	}
}
void CDPSrvr::OnRemoveItemTaskBar( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	BYTE nSlotIndex, nIndex;
	ar >> nSlotIndex >> nIndex;

	if( nSlotIndex >= MAX_SLOT_ITEM_COUNT || nIndex >= MAX_SLOT_ITEM )
		return;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		if( pUser->m_playTaskBar.m_aSlotItem[nSlotIndex][nIndex].m_dwShortcut == SHORTCUT_CHAT )
		{
			memset( pUser->m_playTaskBar.m_aSlotItem[nSlotIndex][nIndex].m_szString, 0, sizeof( pUser->m_playTaskBar.m_aSlotItem[nSlotIndex][nIndex].m_szString ) );
		}
		pUser->m_playTaskBar.m_aSlotItem[nSlotIndex][nIndex].m_dwShortcut = SHORTCUT_NONE;
	}
}


void CDPSrvr::OnPlayerMoved( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsInvalidObj( pUser ) )
		return;

	if( pUser->GetIndex() == 0 )
	{
		WriteError( "PACKETTYPE_PLAYERMOVED" );
		return;
	}

	if( pUser->m_pActMover->IsDie() )
		return;

	D3DXVECTOR3 v, vd;
	float f;
	DWORD dwState, dwStateFlag, dwMotion	= 0;
	int nMotionEx	= 0, nLoop;
	DWORD dwMotionOption;
	__int64 nTickCount;

	ar >> v >> vd >> f;
	ar >> dwState >> dwStateFlag;
	ar >> dwMotion >> nMotionEx;
	ar >> nLoop >> dwMotionOption;
	ar >> nTickCount;

	if( pUser->m_pActMover->IsFly() )	return;		// ��������ε� �Ϸ� ���Դٸ� ��ҽ�Ű��.

	D3DXVECTOR3 vDistance	= pUser->GetPos() - v;
	if( D3DXVec3LengthSq( &vDistance ) > 1000000.0F )
	{
//		Error( "PACKETTYPE_PLAYERMOVED" );
		return;
	}

	int delay	= (int)( (float)g_TickCount.GetOffset( nTickCount ) / 66.6667f );

	if( delay >= MAX_CORR_SIZE_45 ) {
		pUser->ActionForceSet( v, vd, f, dwState, dwStateFlag, dwMotion, nMotionEx, nLoop, dwMotionOption );
	}
	else 	
	{
		if( pUser->m_pActMover->IsStateFlag( OBJSTAF_FLY ) || dwStateFlag & OBJSTAF_FLY ) 
		{
			pUser->ActionForceSet( v, vd, f, dwState, dwStateFlag, dwMotion, nMotionEx, nLoop, dwMotionOption );
		}
		else 
		{
			pUser->m_pActMover->DefaultSet();

			int nRemnant	= (int)( MAX_CORR_SIZE_45 - delay );
			D3DXVECTOR3 d	= v - pUser->GetPos();
			float fLength	= D3DXVec3Length( &d );
			float fSpeed	= ( fLength / (float)nRemnant ) / 4 + 0.020f;
			
			pUser->m_fCrrSpd	= pUser->m_pActMover->IsStateFlag( OBJSTAF_WALK )? fSpeed*4.0f: fSpeed;

			pUser->m_CorrAction.v	= v;
			pUser->m_CorrAction.vd		= vd;
			pUser->m_CorrAction.f	= f;
			pUser->m_CorrAction.dwState	= dwState;
			pUser->m_CorrAction.dwStateFlag	= dwStateFlag;
			pUser->m_CorrAction.dwMotion	= dwMotion;
			pUser->m_CorrAction.nMotionEx	= nMotionEx;
			pUser->m_CorrAction.nLoop	= nLoop;
			pUser->m_CorrAction.dwMotionOption	= dwMotionOption;
			pUser->m_CorrAction.fValid	= TRUE;

			pUser->SetDestPos( v, ( dwState & OBJSTA_MOVE_ALL ) != OBJSTA_BMOVE );
		}
	}

	g_UserMng.AddMoverMoved
		( pUser, v, vd, f, dwState, dwStateFlag, dwMotion,  nMotionEx, nLoop, dwMotionOption, nTickCount );
}

void CDPSrvr::OnPlayerBehavior( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
//	TRACE( "OnPlayerBehavior()\n" );
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );

	if( IsInvalidObj( pUser ) )
		return;

	if( pUser->GetIndex() == 0 )
	{
		WriteError( "PACKETTYPE_PLAYERBEHAVIOR" );
		return;
	}

	if( pUser->m_pActMover->IsDie() )
			return;
	
	if( pUser->m_pActMover->IsState( OBJSTA_ATK_CASTING1 | OBJSTA_ATK_CASTING2 | OBJSTA_ATK_MAGIC1 | OBJSTA_ATK_RANGE1 | OBJSTA_ATK_MELEESKILL | OBJSTA_ATK_RANGESKILL ) )
		return;

	D3DXVECTOR3 v, vd;
	float f;
	DWORD dwState, dwStateFlag, dwMotion	= 0;
	int nMotionEx	= 0, nLoop;
	DWORD dwMotionOption;
	__int64 nTickCount;

	ar >> v >> vd >> f;
	ar >> dwState >> dwStateFlag;
	ar >> dwMotion >> nMotionEx;

	ar >> nLoop >> dwMotionOption;
	ar >> nTickCount;

	if( pUser->m_pActMover->IsFly() )
		return;
	
	int delay	= (int)( (float)g_TickCount.GetOffset( nTickCount ) / 66.6667f );


	if( !pUser->IsEmptyDest() && pUser->m_CorrAction.fValid == FALSE ) 
		pUser->ActionForceSet( v, vd, f, dwState, dwStateFlag, dwMotion, nMotionEx, nLoop, dwMotionOption );
	else 
		pUser->ActionForceSet( v, vd, f, dwState, dwStateFlag, dwMotion, nMotionEx, nLoop, dwMotionOption );

	g_UserMng.AddMoverBehavior( pUser, v, vd, f, dwState, dwStateFlag, dwMotion,  nMotionEx, nLoop, dwMotionOption, nTickCount );
}

void CDPSrvr::OnPlayerMoved2( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) ) 
	{
		if( pUser->GetIndex() == 0 )
		{
			WriteError( "PACKETTYPE_PLAYERMOVED2" );
			return;
		}

		if( pUser->m_pActMover->IsDie() )
			return;

		D3DXVECTOR3 v, vd;
		float f, fAngleX, fAccPower, fTurnAngle;
		DWORD dwState, dwStateFlag, dwMotion	= 0;
		int nMotionEx	= 0, nLoop;
		DWORD dwMotionOption;
		__int64 nTickCount;
		BYTE nFrame;

		ar >> v >> vd >> f >> fAngleX >> fAccPower >> fTurnAngle;
		ar >> dwState >> dwStateFlag;
		ar >> dwMotion >> nMotionEx;

		ar >> nLoop >> dwMotionOption;
		ar >> nTickCount;
		ar >> nFrame;
		int delay	= (int)( (float)g_TickCount.GetOffset( nTickCount ) / 66.6667f );

		if( pUser->m_pActMover->IsFly() )
		{
			// 1
			// 2
			// 3
	//		pUser->ActionForceSet2( v, vd, f, fAngleX, fAccPower, fTurnAngle, dwState, dwStateFlag, dwMotion, nMotionEx, nLoop, dwMotionOption );
//			if( false/*nTickCount < 0*/ ) {
			if( delay >= nFrame ) {
				pUser->ActionForceSet2( v, vd, f, fAngleX, fAccPower, fTurnAngle, dwState, dwStateFlag, dwMotion, nMotionEx, nLoop, dwMotionOption );
			}
			else {
				pUser->m_pActMover->DefaultSet();

				pUser->m_CorrAction.v	= v;
				pUser->m_CorrAction.vd		= vd;
				pUser->m_CorrAction.f	= f;
				pUser->m_CorrAction.dwState	= dwState;
				pUser->m_CorrAction.dwStateFlag	= dwStateFlag;
				pUser->m_CorrAction.dwMotion	= dwMotion;
				pUser->m_CorrAction.nMotionEx	= nMotionEx;
				pUser->m_CorrAction.nLoop	= nLoop;
				pUser->m_CorrAction.dwMotionOption	= dwMotionOption;
				pUser->m_CorrAction.fAngleX	= fAngleX;
				pUser->m_CorrAction.fAccPower	= fAccPower;
				pUser->m_CorrAction.fTurnAngle	= fTurnAngle;
				pUser->m_CorrAction.fValid	= TRUE;

				int nRemnant	= (int)nFrame - delay;
				pUser->m_uRemnantCorrFrm	= nRemnant;
				pUser->SetDestPos( v );
			}

			g_UserMng.AddMoverMoved2
				( pUser, v, vd, f, fAngleX, fAccPower, fTurnAngle, dwState, dwStateFlag, dwMotion,  nMotionEx, nLoop, dwMotionOption, nTickCount, nFrame );
		} 
	}
}

void CDPSrvr::OnPlayerBehavior2( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		if( pUser->GetIndex() == 0 )
		{
			WriteError( "PACKETTYPE_PLAYERBEHAVIOR2" );
			return;
		}

		if( pUser->m_pActMover->IsDie() )
			return;

		D3DXVECTOR3 v, vd;
		float f, fAngleX, fAccPower, fTurnAngle;
		DWORD dwState, dwStateFlag, dwMotion;
		int nMotionEx, nLoop;
		DWORD dwMotionOption;
		__int64 nTickCount;

		ar >> v >> vd >> f >> fAngleX >> fAccPower >> fTurnAngle;
		ar >> dwState >> dwStateFlag;
		ar >> dwMotion >> nMotionEx;
		ar >> nLoop >> dwMotionOption;
		ar >> nTickCount;
#ifdef _DEBUG
		if( dwStateFlag & OBJSTAF_ACC )	
		{
			int a = 0;
		}
#endif

		if( pUser->m_pActMover->IsFly() == FALSE )	return;	// ������°� �ƴѵ� �̸��� ���Դٸ� ���.

		// 1
		// 2
		// 3
		pUser->ActionForceSet2( v, vd, f, fAngleX, fAccPower, fTurnAngle, dwState, dwStateFlag, dwMotion, nMotionEx, nLoop, dwMotionOption );
		// 4
		g_UserMng.AddMoverBehavior2
			( pUser, v, vd, f, fAngleX, fAccPower, fTurnAngle, dwState, dwStateFlag, dwMotion,  nMotionEx, nLoop, dwMotionOption, nTickCount );
	}
}

// Ŭ��κ��� �ö�� ������ ����.
// ���� ������ ���.
void CDPSrvr::OnPlayerAngle( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );

	if( IsValidObj( pUser ) )
	{
		if( pUser->GetIndex() == 0 )
		{
			WriteError( "PACKETTYPE_PLAYERANGLE" );
			return;
		}

		D3DXVECTOR3 v, vd;
		float f, fAngleX, fAccPower, fTurnAngle;
		__int64 nTickCount;

		ar >> v >> vd >> f >> fAngleX >> fAccPower >> fTurnAngle;
		ar >> nTickCount;

		int delay	= (int)( (float)g_TickCount.GetOffset( nTickCount ) / 66.6667f );

		if( pUser->m_pActMover->IsFly() )
		{
			if( delay >= MAX_CORR_SIZE_150 )
			{
				if( pUser->m_CorrAction.fValid &&  !pUser->m_CorrAction.fHalf )
				{
						pUser->ActionForceSet2( v, vd, f, fAngleX, fAccPower, fTurnAngle,
						pUser->m_CorrAction.dwState, pUser->m_CorrAction.dwStateFlag, pUser->m_CorrAction.dwMotion, pUser->m_CorrAction.nMotionEx,
						pUser->m_CorrAction.nLoop, pUser->m_CorrAction.dwMotionOption );
				}
				else
				{
					pUser->HalfForceSet( v, vd, f, fAngleX, fAccPower, fTurnAngle );
				}
			}
			else
			{
				pUser->m_pActMover->DefaultSet();

				pUser->m_CorrAction.v	= v;
				pUser->m_CorrAction.vd		= vd;
				pUser->m_CorrAction.f	= f;
				pUser->m_CorrAction.fAngleX	= fAngleX;
				pUser->m_CorrAction.fAccPower	= fAccPower;
				pUser->m_CorrAction.fTurnAngle	= fTurnAngle;
				pUser->m_CorrAction.fHalf	= !pUser->m_CorrAction.fValid;
				pUser->m_CorrAction.fValid	= TRUE;

				int nRemnant	= MAX_CORR_SIZE_150 - delay;
				pUser->m_uRemnantCorrFrm	= nRemnant;
				pUser->SetDestPos( v );
			}
			g_UserMng.AddMoverAngle( pUser, v, vd, f, fAngleX, fAccPower, fTurnAngle, nTickCount );
		}
	}
}

void CDPSrvr::OnPlayerSetDestObj( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		OBJID objid;
		float fRange;
		ar >> objid >> fRange;

		CCtrl* pCtrl;

		pCtrl	= prj.GetCtrl( objid );
		if( IsValidObj( pCtrl ) )
		{
#ifdef __TRAFIC_1222
			if( pUser->GetDestId() == objid )
				return;
#endif	// __TRAFIC_1222
			pUser->m_pActMover->DefaultSet();
			pUser->BehaviorActionForceSet();
			pUser->SetDestObj( objid, fRange );

			g_UserMng.AddMoverSetDestObj( (CMover*)pUser, objid, fRange );
		}

	}
}


// raider_test ���� �������� ����ߴٰ� �ϸ�?
void CDPSrvr::OnDoUseItem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	DWORD dwData;
	OBJID objid;
	int	  nPart;

	ar >> dwData >> objid >> nPart;
	if( nPart >= MAX_HUMAN_PARTS )	
		return;

	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) == FALSE )
		return;

	WORD nId = HIWORD( dwData );
	if( nPart > 0 )
	{
		CItemElem* pItemElem	= pUser->m_Inventory.GetAtId( nId );
		if( IsUsableItem( pItemElem ) == FALSE )
			return;
		if( pUser->m_Inventory.IsEquip( nId ) )
		{
			if( pItemElem != pUser->m_Inventory.GetEquip( nPart ) )
				return;
		}
	}
#ifdef __HACK_1023
	CItemElem* pItemElem	= pUser->m_Inventory.GetAtId( nId );
	if( IsUsableItem( pItemElem ) )
	{
		ItemProp* pItemProp	= pItemElem->GetProp();
		if( pItemProp && pItemProp->dwParts == PARTS_RIDE )
		{
			if( !pUser->m_Inventory.IsEquip( nId ) )
			{
				FLOAT fVal;
				ar >> fVal;
				if( fVal != pItemProp->fFlightSpeed )
				{
					pUser->AddDefinedText( TID_GAME_MODIFY_FLIGHT_SPEED );
					return;
				}
			}
		}
	}
#endif	// __HACK_1023

	pUser->OnDoUseItem( dwData, objid, nPart );
}

void CDPSrvr::OnPlayerCorr( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
//	TRACE( "OnPlayerCorr\n" );
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		if( pUser->GetIndex() == 0 )
		{
			WriteError( "PACKETTYPE_PLAYERCORR" );
			return;
		}

		D3DXVECTOR3 v, vd;
		float f;
		DWORD dwState, dwStateFlag, dwMotion;
		int nMotionEx, nLoop;
		DWORD dwMotionOption;
		__int64 nTickCount;

		ar >> v >> vd >> f;
		ar >> dwState >> dwStateFlag;
		ar >> dwMotion >> nMotionEx;
		ar >> nLoop >> dwMotionOption;
		ar >> nTickCount;

		if( pUser->m_pActMover->IsFly() == FALSE )
		{
			D3DXVECTOR3 vDistance	= pUser->GetPos() - v;
			if( D3DXVec3LengthSq( &vDistance ) > 1000000.0F )
			{
//				Error( "PACKETTYPE_PLAYERCORR" );
				return;
			}
			// 1
			// 2
			// 3
			pUser->ClearDest();
			pUser->ClearDestAngle();
			memset( &pUser->m_CorrAction, 0, sizeof(CORR_ACTION) );

			pUser->ActionForceSet( v, vd, f, dwState, dwStateFlag, dwMotion, nMotionEx, nLoop, dwMotionOption );

			// 4
			g_UserMng.AddMoverCorr
				( pUser, v, vd, f, dwState, dwStateFlag, dwMotion,  nMotionEx, nLoop, dwMotionOption, nTickCount );
		}
	}
}

void CDPSrvr::OnPlayerCorr2( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
//	TRACE( "OnPlayerCorr2()\n" );
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		if( pUser->GetIndex() == 0 )
		{
			WriteError( "PACKETTYPE_PLAYERCORR2" );
			return;
		}

		D3DXVECTOR3 v, vd;
		float f, fAngleX, fAccPower, fTurnAngle;
		DWORD dwState, dwStateFlag, dwMotion;
		int nMotionEx, nLoop;
		DWORD dwMotionOption;
		__int64 nTickCount;

		ar >> v >> vd >> f >> fAngleX >> fAccPower >> fTurnAngle;
		ar >> dwState >> dwStateFlag;
		ar >> dwMotion >> nMotionEx;

		ar >> nLoop >> dwMotionOption;
		ar >> nTickCount;

		if( pUser->m_pActMover->IsFly() )
		{
			// 1
			// 2
			// 3
			pUser->ClearDest();
			pUser->ClearDestAngle();
			memset( &pUser->m_CorrAction, 0, sizeof(CORR_ACTION) );

			pUser->ActionForceSet2( v, vd, f, fAngleX, fAccPower, fTurnAngle, dwState, dwStateFlag, dwMotion, nMotionEx, nLoop, dwMotionOption );

			// 4
			g_UserMng.AddMoverCorr2
				( pUser, v, vd, f, fAngleX, fAccPower, fTurnAngle, dwState, dwStateFlag, dwMotion,  nMotionEx, nLoop, dwMotionOption, nTickCount );
		}
	}
}

void CDPSrvr::OnOpenShopWnd( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		OBJID objid;
		ar >> objid;
		
		CMover* pVendor	= prj.GetMover( objid );
		if( IsValidObj( pVendor ) && pUser->m_vtInfo.GetOther() == NULL )
		{
			if( pVendor->IsNPC() == FALSE )		// ����� NPC�� �ƴϸ�?
				return;

#if __VER >= 8 // __S8_PK
			if( pUser->IsChaotic() )
			{
				CHAO_PROPENSITY Propensity = prj.GetPropensityPenalty( pUser->GetPKPropensity() );
				if( !Propensity.nShop )
					return;
			}
#endif // __VER >= 8 // __S8_PK

			if( pVendor->IsVendorNPC() == FALSE )
			{
				WriteError( "VENDOR//%s", pVendor->GetName() );
				return;
			}

			if( pUser->m_bBank )
			{
				pUser->AddDefinedText( TID_GAME_TRADELIMITNPC, "" );
				return;
			}

			if( pUser->m_vtInfo.VendorIsVendor() )
				return;

#ifdef __S_SERVER_UNIFY
			if( pUser->m_bAllAction == FALSE )
				return;
#endif // __S_SERVER_UNIFY

			pUser->m_vtInfo.SetOther( pVendor );
			pUser->AddOpenShopWnd( pVendor );
		}
	}
}

void CDPSrvr::OnCloseShopWnd( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) ) 
	{
		CMover* pMover = pUser->m_vtInfo.GetOther();
		if( IsValidObj( pMover ) && pMover->IsNPC() )
			pUser->m_vtInfo.SetOther( NULL );
	}
}

void CDPSrvr::OnBuyItem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CHAR cTab;
	BYTE nId;
	short nNum;
	DWORD dwItemId;

	ar >> cTab >> nId >> nNum >> dwItemId;
	if( cTab >= MAX_VENDOR_INVENTORY_TAB || nNum < 1 )
		return;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) && pUser->m_vtInfo.GetOther() )
	{
		CMover* pVendor = pUser->m_vtInfo.GetOther();
#if __VER >= 11 // __GUILDCOMBATCHIP
		LPCHARACTER lpChar = prj.GetCharacter( pVendor->m_szCharacterKey );
		if( lpChar && lpChar->m_nVenderType != 0 )	// 0 - ��� ����
			return;
#endif // __GUILDCOMBATCHIP

		if( pVendor->IsNPC() == FALSE )		// �Ǹ��� ����� NPC�� �ƴϸ�?
			return;

		if( !CNpcChecker::GetInstance()->IsCloseNpc( MMI_TRADE, pUser->GetWorld(), pUser->GetPos() ) )
			return;

		CItemElem* pItemElem = pVendor->m_ShopInventory[cTab]->GetAtId( nId );
		if( NULL == pItemElem )
			return;

		if( dwItemId != pItemElem->m_dwItemId )
			return;

		if( nNum > pItemElem->m_nItemNum )
			nNum = pItemElem->m_nItemNum;

#ifdef __OCCUPATION_SHOPITEM
		if( CTax::GetInstance()->IsOccupationShopItem( dwItemId ) && !CTax::GetInstance()->IsOccupationGuildMember( pUser ) )
		{
			pUser->AddDefinedText( TID_GAME_SECRETROOM_STORE_BUY );
			return;
		}
#endif // __OCCUPATION_SHOPITEM
		
		int nCost = (int)pItemElem->GetCost();
		nCost = (int)( prj.m_fShopCost * nCost );
#ifdef __SHOP_COST_RATE
		nCost = (int)( prj.m_EventLua.GetShopBuyFactor() * nCost );
#endif // __SHOP_COST_RATE
		
#if __VER < 8 // __S8_PK
		// ������ ���� �������� ����
		KarmaProp* pProp = prj.GetKarmaProp( pUser->m_nSlaughter );
		if( pProp )
		{
			if( pProp->nGrade == -6 )
				return;

			if( pProp->fDiscountRate != 0.0f )
			{
				nCost = (int)( nCost * pProp->fDiscountRate );
			}
		}
#endif // __VER < 8 // __S8_PK

#if __VER >= 11 // __MA_VER11_02
		if( pItemElem->m_dwItemId == II_SYS_SYS_SCR_PERIN )
			nCost = PERIN_VALUE;
#endif //__MA_VER11_02
		if( nCost < 1 )
			nCost = 1;
		
		int nPracticable = pUser->GetGold() / nCost;
		if( nNum > nPracticable )
			nNum = (short)nPracticable;

		if( nNum <= 0 )
		{
			pUser->AddDefinedText( TID_GAME_LACKMONEY, "" );
			return;
		}

#if __VER >= 12 // __TAX
		int nTax = 0;
		if( CTax::GetInstance()->IsApplyTaxRate( pUser, pItemElem ) )
			nTax = (int)( nCost * CTax::GetInstance()->GetPurchaseTaxRate( pUser ) );
		nCost += nTax;
		nTax *= nNum;
#endif // __TAX
		int nGold = nCost * nNum;
		if( nGold <= 0 )
			return;

		if( pUser->GetGold() >= nGold )
		{
#ifdef __PERIN_BUY_BUG
			if( pUser->m_dwLastBuyItemTick + 500 > GetTickCount() ) // ������ ���Խõ� �� 0.5���̳��� �ٽ� ���Խõ��� ���
			{
				Error( "CDPSrvr::OnBuyItem : __PERIN_BUY_BUG -> [PlayerId:%07d(%s)], [LastTick:%d], [CurTick:%d], [LastTryItem:%d], [Packet:%d,%d,%d,%d]",
						pUser->m_idPlayer, pUser->GetName(), pUser->m_dwLastBuyItemTick, GetTickCount(), pUser->m_dwLastTryBuyItem, cTab, nId, nNum, dwItemId );
				//g_DPSrvr.QueryDestroyPlayer( pUser->m_Snapshot.dpidCache, pUser->m_Snapshot.dpidUser, pUser->m_dwSerial, pUser->m_idPlayer );
				return;
			}
			pUser->m_dwLastTryBuyItem = pItemElem->m_dwItemId;
			pUser->m_dwLastBuyItemTick = GetTickCount();
#endif // __PERIN_BUY_BUG
	
			CItemElem itemElem;
			itemElem	= *pItemElem;
			itemElem.m_nItemNum	      = nNum;
			itemElem.SetSerialNumber();

			if( pUser->CreateItem( &itemElem ) )
			{
				LogItemInfo aLogItem;
				aLogItem.Action = "B";
				aLogItem.SendName = pUser->GetName();
				aLogItem.RecvName = pVendor->GetName();
				aLogItem.WorldId = pUser->GetWorld()->GetID();
				aLogItem.Gold = pUser->GetGold();
				aLogItem.Gold2 = pUser->GetGold() - nGold;
				aLogItem.Gold_1 = pVendor->GetGold();

				pItemElem->SetSerialNumber( itemElem.GetSerialNumber() );
				OnLogItem( aLogItem, pItemElem, nNum );		// why do not pass &itemElem as argument?
				pItemElem->SetSerialNumber( 0 );
				pUser->AddGold( -nGold );	
#if __VER >= 12 // __TAX
				if( nTax )
					CTax::GetInstance()->AddTax( CTax::GetInstance()->GetContinent( pUser ), nTax, TAX_PURCHASE );
#endif // __TAX
			}
			else
				pUser->AddDefinedText( TID_GAME_LACKSPACE, "" );
		}
	}
}

#if __VER >= 11 // __GUILDCOMBATCHIP
// Ĩ���� ������ ����
void CDPSrvr::OnBuyChipItem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CHAR cTab;
	BYTE nId;
	short nNum;
	DWORD dwItemId;

	ar >> cTab >> nId >> nNum >> dwItemId;
	if( cTab >= MAX_VENDOR_INVENTORY_TAB || nNum < 1 )
		return;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) && pUser->m_vtInfo.GetOther() )
	{
		CMover* pVendor = pUser->m_vtInfo.GetOther();
		LPCHARACTER lpChar = prj.GetCharacter( pVendor->m_szCharacterKey );
		if( lpChar && lpChar->m_nVenderType != 1 )	// 1 - Ĩ ����
			return;

		if( pVendor->IsNPC() == FALSE )		// �Ǹ��� ����� NPC�� �ƴϸ�?
			return;

		if( !CNpcChecker::GetInstance()->IsCloseNpc( MMI_TRADE, pUser->GetWorld(), pUser->GetPos() ) )
			return;

		CItemElem* pItemElem = pVendor->m_ShopInventory[cTab]->GetAtId( nId );
		if( NULL == pItemElem )
			return;

		if( dwItemId != pItemElem->m_dwItemId )
			return;

#ifdef __OCCUPATION_SHOPITEM
		if( CTax::GetInstance()->IsOccupationShopItem( dwItemId ) && !CTax::GetInstance()->IsOccupationGuildMember( pUser ) )
		{
			pUser->AddDefinedText( TID_GAME_SECRETROOM_STORE_BUY );
			return;
		}
#endif // __OCCUPATION_SHOPITEM
		if( nNum > pItemElem->m_nItemNum )
			nNum = pItemElem->m_nItemNum;
		
		// ������ Ĩ�� ������ ������ ��
		if( pUser->m_Inventory.GetAtItemNum( II_CHP_RED ) < (int)( pItemElem->GetChipCost() * nNum ) )
		{
			// Ĩ ���� ���� �ؽ�Ʈ ���
			pUser->AddDefinedText( TID_GAME_LACKCHIP );
			return;
		}

		// �κ��丮�� �� á�� �� 
		if( pUser->m_Inventory.IsFull( pItemElem, pItemElem->GetProp(), nNum ) )
		{
			int nChipNum = pItemElem->GetChipCost() * nNum;
			ItemProp* pChipItemProp = prj.GetItemProp( II_CHP_RED );
			if( nChipNum < (int)( pChipItemProp->dwPackMax ) )
			{
				CItemElem* pTempElem;
				int bEmpty = FALSE;
				for( int i = 0; i < pUser->m_Inventory.GetSize(); i++ )
				{
					pTempElem = pUser->m_Inventory.GetAtId( i );
					if( IsUsableItem(pTempElem) && pChipItemProp->dwID == pTempElem->m_dwItemId )
					{
						if( pTempElem->m_nItemNum <= nChipNum )
							bEmpty = TRUE;
						break;
					}
				}
				if( !bEmpty )
				{
					// �κ��� ��á�ٴ� �ؽ�Ʈ ���
					pUser->AddDefinedText( TID_GAME_LACKSPACE, "" );
					return;
				}
			}
		}
		
		// ���� ���� ��ŭ�� Ĩ ����
		DWORD dwChipCost = pItemElem->GetChipCost() * nNum;
		if( dwChipCost > 0x7fff )
		{
			for( ; dwChipCost > 0x7fff; )
			{
				pUser->RemoveItemA( II_CHP_RED, 0x7fff );
				dwChipCost -= 0x7fff;
			}
		}
		pUser->RemoveItemA( II_CHP_RED, (short)dwChipCost );
				
		// ���� ������ ����
		CItemElem itemElem;
		itemElem	= *pItemElem;
		itemElem.m_nItemNum = nNum;
		itemElem.SetSerialNumber();
		if( pUser->CreateItem( &itemElem ) )
		{
			// �α� ����
			LogItemInfo aLogItem;
			aLogItem.Action = "B";
			aLogItem.SendName = pUser->GetName();
			CString strTemp;
			strTemp.Format( "%s_C", pVendor->GetName() );
			aLogItem.RecvName = (LPCTSTR)strTemp;
			aLogItem.WorldId = pUser->GetWorld()->GetID();
			aLogItem.Gold = pUser->GetItemNum( II_CHP_RED ) + itemElem.GetChipCost() * nNum;
			aLogItem.Gold2 = pUser->GetItemNum( II_CHP_RED );
			aLogItem.Gold_1 = (DWORD)( (-1) * (int)( (itemElem.GetChipCost() * nNum) ) );
			OnLogItem( aLogItem, &itemElem, nNum );
		}
		else
		{
			LogItemInfo aLogItem;
			aLogItem.Action = "B";
			aLogItem.SendName = pUser->GetName();
			CString strTemp;
			strTemp.Format( "%s_CF", pVendor->GetName() );
			aLogItem.RecvName = (LPCTSTR)strTemp;
			aLogItem.WorldId = pUser->GetWorld()->GetID();
			aLogItem.Gold = pUser->GetItemNum( II_CHP_RED ) + itemElem.GetChipCost() * nNum;
			aLogItem.Gold2 = pUser->GetItemNum( II_CHP_RED );
			aLogItem.Gold_1 = (DWORD)( (-1) * (int)( (itemElem.GetChipCost() * nNum) ) );
			OnLogItem( aLogItem, &itemElem, nNum );
		}
	}
}
#endif // __GUILDCOMBATCHIP

//NPC���� �Ĵ� ���
void CDPSrvr::OnSellItem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	BYTE nId;
	short nNum;

	ar >> nId >> nNum;
	if( nNum < 1 )
		return;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) && IsValidObj( pUser->m_vtInfo.GetOther() ) )
	{
		if( pUser->m_vtInfo.GetOther()->IsNPC() == FALSE )		// �Ǹ��� ����� NPC�� �ƴϸ�?
			return;

		if( !CNpcChecker::GetInstance()->IsCloseNpc( MMI_TRADE, pUser->GetWorld(), pUser->GetPos() ) )
			return;

		CItemElem* pItemElem = pUser->m_Inventory.GetAtId( nId );		
		if( IsUsableItem( pItemElem ) == FALSE )
			return;

		ItemProp* pProp	= pItemElem->GetProp();
		if( pProp->dwItemKind3 == IK3_EVENTMAIN )
			return;
		
		if( pItemElem->IsQuest() )
			return;

		if( pUser->m_Inventory.IsEquip( nId ) )
		{
			pUser->AddDefinedText( TID_GAME_EQUIPTRADE, "" );
			return;
		}
#if __VER >= 11 // __MA_VER11_05	// �ɸ��� ���� �ŷ� ��� world,database,neuz
		if( pItemElem->m_dwItemId == II_SYS_SYS_SCR_SEALCHARACTER )
			return;
#endif // __MA_VER11_05	// �ɸ��� ���� �ŷ� ��� world,database,neuz
#if __VER >= 11 // __MA_VER11_02				// ��ǥ ���� ȭ�� '�丰' �߰�
		if( pItemElem->m_dwItemId == II_SYS_SYS_SCR_PERIN )
			return;
#endif // __MA_VER11_02				// ��ǥ ���� ȭ�� '�丰' �߰�

		if( nNum > pItemElem->m_nItemNum )
			nNum = pItemElem->m_nItemNum;
		if( nNum < 1 )
			nNum = 1;

#if __VER >= 12 // __TAX
		int nGold = ( pItemElem->GetCost() / 4 );
#ifdef __SHOP_COST_RATE
		nGold = (int)( prj.m_EventLua.GetShopSellFactor() * nGold );
#endif // __SHOP_COST_RATE
		if( nGold == 0 )
			nGold = 1;
		int nTax = 0;
		if( CTax::GetInstance()->IsApplyTaxRate( pUser, pItemElem ) )
			nTax = (int)( nGold * CTax::GetInstance()->GetSalesTaxRate( pUser ) );
		nGold -= nTax;
		nGold *= nNum;
		nTax  *= nNum;
		float fTmpGold = (float)( pUser->GetGold() + nGold );
#else // __TAX
		float fTmpGold = pUser->GetGold() + ( pItemElem->GetCost() / 4 ) * nNum;
#endif // __TAX
		if( fTmpGold >= 2100000000 )
		{
			return;
		}
//		if( pItemElem->m_dwItemId == II_RID_RID_BOR_EVEINSHOVER || pItemElem->m_dwItemId == II_RID_RID_BOR_LADOLF )
//			return;
		if( pProp->dwParts == PARTS_RIDE && pProp->dwItemJob == JOB_VAGRANT )
			return;

		if( pUser->IsUsing( pItemElem ) )
		{
			pUser->AddDefinedText( TID_GAME_CANNOT_DO_USINGITEM );
			return;
		}

		LogItemInfo aLogItem;
		aLogItem.Action = "S";
		aLogItem.SendName = pUser->GetName();
		aLogItem.RecvName = pUser->m_vtInfo.GetOther()->GetName();
		aLogItem.WorldId = pUser->GetWorld()->GetID();
		aLogItem.Gold = pUser->GetGold();
#if __VER >= 12 // __TAX
		aLogItem.Gold2 = pUser->GetGold() + nGold;
#else // __TAX
		aLogItem.Gold2 = pUser->GetGold() + ( pItemElem->GetCost() / 4 ) * nNum;
#endif // __TAX
		OnLogItem( aLogItem, pItemElem, nNum );
		int nCost	= (int)pItemElem->GetCost() / 4;

#if __VER < 8 // __S8_PK
		KarmaProp* pProp = prj.GetKarmaProp( pUser->m_nSlaughter );
		if( pProp )
		{
			if( pProp->nGrade == -6 )
				return;

			if( pProp->fSellPenaltyRate != 0.0f )
			{
				nCost = (int)( nCost * pProp->fSellPenaltyRate );
			}
		}
#endif // __VER < 8 // __S8_PK

		if( nCost < 1 )	
			nCost = 1;

#if __VER >= 12 // __TAX
		if( nGold < 1 )
			nGold = 1;
		
		pUser->AddGold( nGold );
		if( nTax )
			CTax::GetInstance()->AddTax( CTax::GetInstance()->GetContinent( pUser ), nTax, TAX_SALES );
#else // __TAX
		pUser->AddGold( nCost * nNum );
#endif // __TAX
		pUser->RemoveItem( nId, nNum );
	}
}

// �н����� ����â�� �������� �н����� Ȯ��â�� ������������ �˷��ش�
void CDPSrvr::OnOpenBankWnd( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	DWORD	dwId, dwItemId;
	ar >> dwId >> dwItemId;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		if( dwId == NULL_ID && !CNpcChecker::GetInstance()->IsCloseNpc( MMI_BANKING, pUser->GetWorld(), pUser->GetPos() ) )
			return;
#if __VER >= 8 // __S8_PK
		if( pUser->IsChaotic() )
		{
			CHAO_PROPENSITY Propensity = prj.GetPropensityPenalty( pUser->GetPKPropensity() );
			if( !Propensity.nBank )
				return;
		}
#endif // __VER >= 8 // __S8_PK
		if( 0 == strcmp( pUser->m_szBankPass, "0000") )
		{
			// ����â�� ����� ��
			pUser->AddBankWindow( 0, dwId, dwItemId );
		}
		else
		{
			// Ȯ��â�� ����� ��
			pUser->AddBankWindow( 1, dwId, dwItemId );
		}
	}
}

void CDPSrvr::OnOpenGuildBankWnd(CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	if( g_eLocal.GetState( ENABLE_GUILD_INVENTORY ) == FALSE )		
		return;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		if( pUser->m_vtInfo.GetOther() )	// �ŷ����� ����� ������?
			return;
		if( pUser->m_vtInfo.VendorIsVendor() )		// ���� �Ȱ� ������?
			return;
		if( pUser->m_bBank )				// â�� ���� ������?
			return;
#ifdef __S_SERVER_UNIFY
		if( pUser->m_bAllAction == FALSE )
			return;
#endif // __S_SERVER_UNIFY

		pUser->AddGuildBankWindow( 0 );
		pUser->m_bGuildBank = TRUE;
	}
}

void CDPSrvr::OnCloseBankWnd( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );	
	if( IsValidObj( pUser ) )
	{
		pUser->m_bBank = FALSE;
		pUser->m_bInstantBank	= FALSE;
	}
}

void CDPSrvr::OnDoUseSkillPoint( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	SKILL aJobSkill[ MAX_SKILL_JOB ] = {0, };
	
	for( int i = 0; i < MAX_SKILL_JOB; i++ ) 
		ar >> aJobSkill[i].dwSkill >> aJobSkill[i].dwLevel;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );	
	if( IsValidObj( pUser ) )
	{	// �ٲܼ� ������?  Ȯ��
		int nChangePoint = 0;
		for( int i = 0; i < MAX_SKILL_JOB; i++ ) 
		{		
			if( aJobSkill[i].dwSkill == NULL_ID )
				continue;
			ItemProp* pSkillProp    = prj.GetSkillProp( aJobSkill[i].dwSkill );			
			if( pSkillProp == NULL )
				continue;

			if( aJobSkill[i].dwLevel < pUser->m_aJobSkill[i].dwLevel || aJobSkill[i].dwLevel > pSkillProp->dwExpertMax )
			{
				pUser->AddDefinedText(TID_RESKILLPOINT_ERROR);
				return;						
			}
			int nPoint = (aJobSkill[i].dwLevel - pUser->m_aJobSkill[i].dwLevel) * prj.GetSkillPoint( pSkillProp );
			nChangePoint += nPoint;
		}

		if( nChangePoint == 0 ) 
			return;
		if( pUser->m_nSkillPoint < nChangePoint )
		{
			pUser->AddDefinedText(TID_RESKILLPOINT_ERROR);
			return;						
		}
		
		// ��ų�� ��й� �ϱ� // ��ų ���� ����
		pUser->m_nSkillPoint -= nChangePoint;

		for( int i = 0; i < MAX_SKILL_JOB; i++ ) 
		{				
			LPSKILL lpSkill = &(pUser->m_aJobSkill[i]);
			if( lpSkill )
			{
				if( aJobSkill[i].dwSkill == NULL_ID )
					continue;
				ItemProp* pSkillProp    = prj.GetSkillProp( lpSkill->dwSkill );			
				if( pSkillProp == NULL )
					continue;

				int nPoint = (aJobSkill[i].dwLevel - lpSkill->dwLevel) * prj.GetSkillPoint( pSkillProp );
				if( 0 < nPoint ) // ��ų �ο��Ѱ͸� �α׿� ������
					g_dpDBClient.SendLogSkillPoint( LOG_SKILLPOINT_USE, nPoint, (CMover*)pUser, &aJobSkill[i] );

				lpSkill->dwLevel = aJobSkill[i].dwLevel;
			}
		}	
		g_UserMng.AddCreateSfxObj((CMover *)pUser, XI_SYS_EXCHAN01, pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z);
		pUser->AddDoUseSkillPoint( &aJobSkill[0], pUser->m_nSkillPoint );
#ifdef __S_NEW_SKILL_2
		g_dpDBClient.SaveSkill( pUser );
#endif // __S_NEW_SKILL_2
	}
}

void CDPSrvr::OnCloseGuildBankWnd( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	if( g_eLocal.GetState( ENABLE_GUILD_INVENTORY ) == FALSE )		
		return;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	
	if( IsValidObj( pUser ) )
	{
		pUser->m_bGuildBank = FALSE;
	}
}

void CDPSrvr::OnBankToBank( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	BYTE nFlag, nPutSlot, nSlot, nId;
	short nItemNum;
	DWORD dwGold;
	ar >> nFlag >> nPutSlot >> nSlot;

	if( nPutSlot >= 3 || nSlot >= 3 )
		return;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		if( pUser->m_idPlayerBank[nPutSlot] != 0 && pUser->m_idPlayerBank[nSlot] != 0 && pUser->IsCommBank() )
		{
			if( nFlag == 1 )	// Item
			{
				ar >> nId >> nItemNum;
				CItemElem* pItemElem	= pUser->m_Bank[nPutSlot].GetAtId( nId );
				if( pItemElem == NULL )
					return;

				if( nItemNum > pItemElem->m_nItemNum )
					nItemNum	= pItemElem->m_nItemNum;
				if( nItemNum < 1 )
					nItemNum	= 1;

				if( MAX_BANK > pUser->m_Bank[nSlot].GetCount() )
				{
					CItemElem itemElem;
					itemElem	= *pItemElem;
					itemElem.m_nItemNum		= nItemNum;
					pUser->AddPutItemBank( nSlot, &itemElem );
					pUser->m_Bank[nSlot].Add( &itemElem );
					pUser->UpdateItemBank( nPutSlot, nId, UI_NUM, pItemElem->m_nItemNum - nItemNum );		// ���࿡ ����� ����

					LogItemInfo aLogItem;
					aLogItem.Action = "A";
					aLogItem.SendName = pUser->GetName();
					aLogItem.RecvName = "BANK";
					aLogItem.WorldId = pUser->GetWorld()->GetID();
					aLogItem.Gold = aLogItem.Gold2 = pUser->GetGold();
					aLogItem.Gold_1 = pUser->m_dwGoldBank[pUser->m_nSlot];
					aLogItem.nSlot = nSlot;
					aLogItem.nSlot1 = nPutSlot;
					OnLogItem( aLogItem, &itemElem, itemElem.m_nItemNum );
				}
				else
				{
					// ������ ������ �� ����. �޼��� ó��
					pUser->AddBankIsFull();
				}
			}
			else		// Gold
			{
				 ar >> dwGold;
				
				 if( dwGold > pUser->m_dwGoldBank[nPutSlot] )
					 dwGold = pUser->m_dwGoldBank[nPutSlot];
				 
				 int nGold = dwGold;
				 if( CanAdd( pUser->m_dwGoldBank[nSlot], nGold ) )
				 {
					 pUser->m_dwGoldBank[nSlot]	+= nGold;
					 pUser->m_dwGoldBank[nPutSlot] -= nGold;
					 
					 pUser->AddPutGoldBank( nSlot, pUser->GetGold(), pUser->m_dwGoldBank[nSlot] );
					 pUser->AddPutGoldBank( nPutSlot, pUser->GetGold(), pUser->m_dwGoldBank[nPutSlot] );

					 LogItemInfo aLogItem;
					 aLogItem.Action = "A";
					 aLogItem.SendName = pUser->GetName();
					 aLogItem.RecvName = "BANK";
					 aLogItem.WorldId = pUser->GetWorld()->GetID();
					 aLogItem.Gold = aLogItem.Gold2 = pUser->GetGold();
					 //aLogItem.ItemName = "SEED";
					 _stprintf( aLogItem.szItemName, "%d", II_GOLD_SEED1 );
					 aLogItem.itemNumber = nGold;
					 aLogItem.Gold_1 = pUser->m_dwGoldBank[pUser->m_nSlot];
					 aLogItem.nSlot = nSlot;
					 aLogItem.nSlot1 = nPutSlot;
					 OnLogItem( aLogItem );
				 }	
			}
		}
	}
}
void CDPSrvr::OnPutItemBank( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	BYTE nSlot, nId;
	short nItemNum;

	ar >> nSlot >> nId >> nItemNum;
	if( nSlot >= 3 )
		return;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		if( !pUser->m_bInstantBank )
		{
			if( !CNpcChecker::GetInstance()->IsCloseNpc( MMI_BANKING, pUser->GetWorld(), pUser->GetPos() ) )
				return;
		}

		if( pUser->m_nSlot == nSlot || ( pUser->m_idPlayerBank[nSlot] != 0 && pUser->IsCommBank() ) )
		{
			CItemElem* pItemElem = pUser->m_Inventory.GetAtId( nId );
			if( IsUsableItem( pItemElem ) == FALSE )
				return;

			if( nSlot != pUser->m_nSlot )
			{
				if( pItemElem->IsQuest() )
					return;
			}
				
#if __VER >= 11 // __SYS_IDENTIFY
			if( pItemElem->IsBinds() )
				return;
#else	// __SYS_IDENTIFY
			if( pItemElem->IsFlag( CItemElem::binds ) )
				return;
#endif	// __SYS_IDENTIFY

			ItemProp* pProp	= pItemElem->GetProp();
			if( !pProp )
				return;

			if( pUser->IsUsing( pItemElem ) )
			{
				pUser->AddDefinedText( TID_GAME_CANNOT_DO_USINGITEM );
				return;
			}
			
			if( pProp->dwItemKind3 == IK3_EVENTMAIN ||
				pProp->dwItemKind3 == IK3_LINK ||
				( pProp->dwItemKind3 == IK3_CLOAK && pItemElem->m_idGuild != 0 ) )
				return;

//			if( pItemElem->m_dwItemId == II_RID_RID_BOR_EVEINSHOVER || pItemElem->m_dwItemId == II_RID_RID_BOR_LADOLF )
//				return;
			if( pProp->dwParts == PARTS_RIDE && pProp->dwItemJob == JOB_VAGRANT )
				return;


			if( pUser->m_Inventory.IsEquip( nId ) )
			{
				pUser->AddDefinedText( TID_GAME_EQUIPTRADE, "" );
				return;
			}
			
			if( nItemNum > pItemElem->m_nItemNum )
				nItemNum	= pItemElem->m_nItemNum;
			if( nItemNum < 1 )
				nItemNum	= 1;

//			if( MAX_BANK > pUser->m_Bank[nSlot].GetCount() )
			if( pUser->m_Bank[nSlot].IsFull( pItemElem, pProp, nItemNum ) == FALSE )
			{
				CItemElem itemElem;
				itemElem	= *pItemElem;
				itemElem.m_nItemNum		= nItemNum;
				pUser->AddPutItemBank( nSlot, &itemElem );
				pUser->m_Bank[nSlot].Add( &itemElem );

				LogItemInfo aLogItem;
				aLogItem.Action = "P";
				aLogItem.SendName = pUser->GetName();
				aLogItem.RecvName = "BANK";
				aLogItem.WorldId = pUser->GetWorld()->GetID();
				aLogItem.Gold = aLogItem.Gold2 = pUser->GetGold();
				aLogItem.Gold_1 = pUser->m_dwGoldBank[pUser->m_nSlot];
				aLogItem.nSlot1 = nSlot;
				OnLogItem( aLogItem, pItemElem, nItemNum );

				pUser->RemoveItem( nId, nItemNum );
			}
			else
			{
				// ������ ������ �� ����. �޼��� ó��
				pUser->AddBankIsFull();
			}
		}
	}
}

void CDPSrvr::OnPutItemGuildBank( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	if( g_eLocal.GetState( ENABLE_GUILD_INVENTORY ) == FALSE )		
		return;

	BYTE nId, mode;
	DWORD nItemNum;

	ar >> nId >> nItemNum >> mode;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );	
	if( IsValidObj( pUser ) )
	{
#if __VER >= 15 // __GUILD_HOUSE
		if( !pUser->GetWorld() || !GuildHouseMng->IsGuildHouse( pUser->GetWorld()->GetID() ) )
#endif // __GUILD_HOUSE
			if( !CNpcChecker::GetInstance()->IsCloseNpc( MMI_GUILDBANKING, pUser->GetWorld(), pUser->GetPos() ) )
				return;

		if( mode == 0 ) // ���â���� Gold�� ������ �����ϴ�.
			return;

		CItemElem* pItemElem = pUser->m_Inventory.GetAtId( nId );		
		if( IsUsableItem( pItemElem ) == FALSE )
			return;

		if( pItemElem->IsQuest() )
			return;
		
		if( pItemElem->IsBinds() )
			return;

		if( pUser->IsUsing( pItemElem ) )
		{
			pUser->AddDefinedText( TID_GAME_CANNOT_DO_USINGITEM );
			return;
		}

		ItemProp* pProp	= pItemElem->GetProp();
		if( pProp->dwParts == PARTS_RIDE && pProp->dwItemJob == JOB_VAGRANT )
			return;

		if( pUser->m_Inventory.IsEquip( nId ) )
		{
			pUser->AddDefinedText( TID_GAME_EQUIPTRADE, "" );
			return;
		}

		if( pItemElem->IsCharged() )
			return;
	
		if( (int)( nItemNum ) > pItemElem->m_nItemNum )
			nItemNum = pItemElem->m_nItemNum;
		if( nItemNum < 1 )
			nItemNum = 1;
		
		//	GUILD_BANK_STR 'S1','000000','01' 
		//	GUILD BANK ��ü �ҷ����� ex ) GUILD_BANK_STR 'S1',@im_idGuild,@iserverindex GUILD_BANK_STR 'S1','000000','01'  
		//	GUILD BANK �����ϱ� ex ) GUILD_BANK_STR 'U1',@im_idGuild,@iserverindex,@im_nGoldGuild,@im_apIndex,@im_dwObjIndex,@im_GuildBank GUILD_BANK_STR 'U1','000001','01',0,'$','$','$' 			

		CGuild*	pGuild = pUser->GetGuild();
		if( pGuild )
		{
			CItemElem itemElem;
			itemElem	= *pItemElem;
			itemElem.m_nItemNum	= (short)nItemNum;
			if ( pGuild->m_GuildBank.Add( &itemElem ) )
			{
				LogItemInfo aLogItem;
				aLogItem.Action = "W";
				aLogItem.SendName = pUser->GetName();
				aLogItem.RecvName = "GUILDBANK";
				aLogItem.WorldId = pUser->GetWorld()->GetID();
				aLogItem.Gold = aLogItem.Gold2 = pUser->GetGold();
				OnLogItem( aLogItem, &itemElem, nItemNum );

				pUser->RemoveItem( (BYTE)( nId ), (short)( nItemNum ) );
				UpdateGuildBank( pGuild, GUILD_PUT_ITEM, 0, pUser->m_idPlayer, &itemElem, 0, (short)( nItemNum ) );
				pUser->AddPutItemGuildBank( &itemElem );
				g_UserMng.AddPutItemElem( pUser, &itemElem );
			}
			else
			{
				pUser->AddDefinedText( TID_GAME_GUILDBANKFULL, "" );		// ���â�� ��á����~
			}
		}
	}
}

void CDPSrvr::OnGetItemGuildBank( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	if( g_eLocal.GetState( ENABLE_GUILD_INVENTORY ) == FALSE )		
		return;

	BYTE nId, mode;
	DWORD dwItemNum;

	ar >> nId >> dwItemNum >> mode;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );	
	if( IsValidObj( pUser ) )
	{
#if __VER >= 15 // __GUILD_HOUSE
		if( !pUser->GetWorld() || !GuildHouseMng->IsGuildHouse( pUser->GetWorld()->GetID() ) )
#endif // __GUILD_HOUSE
			if( !CNpcChecker::GetInstance()->IsCloseNpc( MMI_GUILDBANKING, pUser->GetWorld(), pUser->GetPos() ) )
				return;
		
		if (mode == 0) // Gold�� ���â���� ������
		{
			int nGold = (int)dwItemNum;
			if( nGold <= 0 || CanAdd( pUser->GetGold(), nGold ) == FALSE )
				return;

			CGuild*	pGuild = pUser->GetGuild();
			if (pGuild && pGuild->IsGetPenya(pUser->m_idPlayer))
			{
				if( (DWORD)nGold > pGuild->m_nGoldGuild )
					return;

				pUser->AddGold( nGold, FALSE );
				pGuild->m_nGoldGuild -= nGold;
				pUser->AddGetGoldGuildBank( nGold, 0, pUser->m_idPlayer, 0 );	// 0�� ������Ʈ ��Ų Ŭ�󿡰� 
				pGuild->DecrementMemberContribution( pUser->m_idPlayer, nGold, 0 );
				UpdateGuildBank(pGuild, GUILD_GET_PENYA, 1, pUser->m_idPlayer, NULL, nGold );

				LogItemInfo aLogItem;
				aLogItem.Action = "Y";
				aLogItem.SendName = "GUILDBANK";
				aLogItem.RecvName = pUser->GetName();
				aLogItem.WorldId = pUser->GetWorld()->GetID();
				aLogItem.Gold = pUser->GetGold() - nGold;
				aLogItem.Gold2 = pUser->GetGold();
				//aLogItem.ItemName = "SEED";
				_stprintf( aLogItem.szItemName, "%d", II_GOLD_SEED1 );
				aLogItem.itemNumber = nGold;
				OnLogItem( aLogItem );

				CGuildMember*	pMember;
				CUser*			pUsertmp;
				map<u_long, CGuildMember*>::iterator i = pGuild->m_mapPMember.begin();
				for( ; i != pGuild->m_mapPMember.end(); ++i )
				{
					pMember		= i->second;
					pUsertmp	= (CUser*)prj.GetUserByID( pMember->m_idPlayer );
					if( IsValidObj( pUsertmp ) && pUsertmp != pUser )
					{
						pUsertmp->AddGetGoldGuildBank( nGold, 2, pUser->m_idPlayer, 0 );	// 2�� ������Ʈ �ؾ��� Ŭ���̰�
					}
				}

				g_DPCoreClient.SendGuildMsgControl_Bank_Penya( pUser, nGold, 2, 0 ); 	// 2�� ������Ʈ �ؾ��� �ٸ� ���弭���� Ŭ���̾�Ʈ
			}
			//	Core ������ �� ������ ������Ʈ �Ǿ����� �˸���.
		}
		else if (mode == 1) // �������� ���â���� ������
		{
			CGuild*			pGuild = pUser->GetGuild();
			if (pGuild && pGuild->IsGetItem(pUser->m_idPlayer))
			{
				CItemElem* pItemElem	= pGuild->m_GuildBank.GetAtId( nId );
				
				if( NULL == pItemElem )
					return;
				
				if( (int)( dwItemNum ) > pItemElem->m_nItemNum )
					dwItemNum = pItemElem->m_nItemNum;
				if( dwItemNum < 1 )
					dwItemNum	= 1;
				
				CItemElem itemElem;
				itemElem	= *pItemElem;
				itemElem.m_nItemNum		= (short)( dwItemNum );
				itemElem.m_dwObjId		= pItemElem->m_dwObjId;

				if( pUser->m_Inventory.Add( &itemElem ) )
				{
					if (pItemElem->m_nItemNum > (int)( dwItemNum ) )
						pItemElem->m_nItemNum	= (short)( pItemElem->m_nItemNum - dwItemNum );
					else 
						pGuild->m_GuildBank.RemoveAtId( nId );
					UpdateGuildBank(pGuild, GUILD_GET_ITEM, 0, pUser->m_idPlayer, &itemElem, 0, (short)( dwItemNum ) );
					LogItemInfo aLogItem;
					aLogItem.Action = "Y";
					aLogItem.SendName = "GUILDBANK";
					aLogItem.RecvName = pUser->GetName();
					aLogItem.WorldId = pUser->GetWorld()->GetID();
					aLogItem.Gold = aLogItem.Gold2 = pUser->GetGold();
					OnLogItem( aLogItem, &itemElem, dwItemNum );
				
					// Ŭ���̾�Ʈ���� �������� �κ��丮�� �߰����� �˸���.
					pUser->AddGetItemGuildBank( &itemElem );
					// �ڽ��� ������ ��� Ŭ���̾�Ʈ���� �˷��ش�.
					g_UserMng.AddGetItemElem( pUser, &itemElem );
				}
				else
				{
					// ������ ������ �� ����. �޼��� ó��
					pUser->AddBankIsFull();
				}
			}
		}
	}
}


void CDPSrvr::OnGuildBankMoveItem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize)
{
	if( g_eLocal.GetState( ENABLE_GUILD_INVENTORY ) == FALSE )		
		return;

	BYTE nSrcIndex, nDestIndex;
	
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		ar >> nSrcIndex >> nDestIndex;
		
		// ��� â���� �������� �����Ѵ�.
		CGuild* pGuild = pUser->GetGuild();
		if ( pGuild )
		{
			pGuild->m_GuildBank.Swap( nSrcIndex, nDestIndex );
			// ��� Ŭ���̾�Ʈ���� ���â���� �������� �̵������� �˷��ش�.
			// ���â�� ������Ʈ�Ѵ�.
			UpdateGuildBank(pGuild, 4); // 4���� �������� ���ҵȰ���
		}
	}
}

void CDPSrvr::UpdateGuildBank(CGuild* p_GuildBank, int p_Mode, BYTE cbUpdate, u_long idPlayer, CItemElem* pItemElem, DWORD dwPenya, short nItemCount )
{
	BEFORESENDDUAL( ar, PACKETTYPE_GUILD_BANK_UPDATE, DPID_UNKNOWN, DPID_UNKNOWN );

	if (p_GuildBank)
	{
		ar << p_GuildBank->m_idGuild;
		ar << p_GuildBank->m_nGoldGuild;
		p_GuildBank->m_GuildBank.Serialize(ar);
		ar << cbUpdate;	// ����� ������ĸ� �����ؾ��ϴ°�? 
		ar << idPlayer;
		ar << p_Mode;
		if( pItemElem == NULL )
		{
			ar << (DWORD)0;
			ar << (int)0;
			ar << (SERIALNUMBER)0;
		}
		else
		{
			ar << pItemElem->m_dwItemId;
			ar << pItemElem->GetAbilityOption();
			ar << pItemElem->GetSerialNumber();
		}		
		ar << nItemCount;
		ar << dwPenya;
		SEND( ar, &g_dpDBClient, DPID_SERVERPLAYER );
	}
}


void CDPSrvr::OnGetItemBank( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	BYTE nSlot, nId;
	short nItemNum;

	ar >> nSlot >> nId >> nItemNum;
	if( nSlot >= 3 )
		return;
	
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	
	if( IsValidObj( pUser ) )
	{
		if( !pUser->m_bInstantBank )
		{
			if( !CNpcChecker::GetInstance()->IsCloseNpc( MMI_BANKING, pUser->GetWorld(), pUser->GetPos() ) )
				return;
		}
		if( pUser->m_nSlot == nSlot || ( pUser->m_idPlayerBank[nSlot] != 0 && pUser->IsCommBank() ) )
		{
			CItemElem* pItemElem	= pUser->m_Bank[nSlot].GetAtId( nId );
			if( NULL == pItemElem )
				return;
	
			if( nItemNum > pItemElem->m_nItemNum )
				nItemNum = pItemElem->m_nItemNum;
			if( nItemNum < 1 )
				nItemNum	= 1;
			
			if( MAX_INVENTORY > pUser->m_Inventory.GetCount() )
			{
				CItemElem itemElem;
				itemElem	= *pItemElem;
				itemElem.m_nItemNum		= nItemNum;
				pUser->AddGetItemBank( &itemElem );			// �������� ����
				pUser->m_Inventory.Add( &itemElem );		// �κ��� �ֱ�

				LogItemInfo aLogItem;
				aLogItem.Action = "G";
				aLogItem.SendName = "BANK";
				aLogItem.RecvName = pUser->GetName();
				aLogItem.WorldId = pUser->GetWorld()->GetID();
				aLogItem.Gold = aLogItem.Gold2 = pUser->GetGold();
				aLogItem.Gold_1 = pUser->m_dwGoldBank[pUser->m_nSlot];
				aLogItem.nSlot = nSlot;
				OnLogItem( aLogItem, pItemElem, nItemNum );
				pUser->UpdateItemBank( nSlot, nId, UI_NUM, pItemElem->m_nItemNum - nItemNum );		// ���࿡ ����� ����
			}
			else
			{
				// ������ ������ �� ����. �޼��� ó��
				pUser->AddBankIsFull();
			}
		}		
	}
}

void CDPSrvr::OnPutGoldBank( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpbuf, u_long uBufSize )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	DWORD dwGold;
	BYTE nSlot;
	ar >> nSlot >> dwGold;
	
	int nGold = dwGold;
	if( nGold <= 0 )
		return;

	if( nSlot >= 3 )
		return;

	if( IsValidObj( pUser ) )
	{
		if( !pUser->m_bInstantBank )
		{
			if( !CNpcChecker::GetInstance()->IsCloseNpc( MMI_BANKING, pUser->GetWorld(), pUser->GetPos() ) )
				return;
		}
		if( pUser->m_nSlot == nSlot || ( pUser->m_idPlayerBank[nSlot] != 0 && pUser->IsCommBank() ) )
		{
			if( nGold > pUser->GetGold() )
				nGold = pUser->GetGold();
			
			// ���൷�� overflow���� �ʰ� �Ѵ�.
			if( CanAdd(pUser->m_dwGoldBank[nSlot], nGold) )
			{
				LogItemInfo aLogItem;
				aLogItem.Action = "P";
				aLogItem.SendName = pUser->GetName();
				aLogItem.RecvName = "BANK";
				aLogItem.WorldId = pUser->GetWorld()->GetID();
				aLogItem.Gold = pUser->GetGold();
				aLogItem.Gold2 = pUser->GetGold() - nGold;
				//aLogItem.ItemName = "SEED";
				_stprintf( aLogItem.szItemName, "%d", II_GOLD_SEED1 );
				aLogItem.itemNumber = nGold;
				aLogItem.Gold_1 = pUser->m_dwGoldBank[pUser->m_nSlot];
				aLogItem.nSlot1 = nSlot;
				OnLogItem( aLogItem );
				
				pUser->m_dwGoldBank[nSlot] += nGold;
				pUser->AddGold( -nGold, FALSE );
				
				pUser->AddPutGoldBank( nSlot, pUser->GetGold(), pUser->m_dwGoldBank[nSlot] );
			}
		}		
	}
}

void CDPSrvr::OnGetGoldBank( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpbuf, u_long uBufSize )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );

	DWORD dwGold;
	BYTE nSlot;
	ar >> nSlot >> dwGold;

	int nGold = dwGold;		
	if( nGold <= 0 )
		return;

	if( nSlot >= 3 )
		return;

	if( IsValidObj( pUser ) )
	{
		if( !pUser->m_bInstantBank )
		{
			if( !CNpcChecker::GetInstance()->IsCloseNpc( MMI_BANKING, pUser->GetWorld(), pUser->GetPos() ) )
				return;
		}
		if( pUser->m_nSlot == nSlot || ( pUser->m_idPlayerBank[nSlot] != 0 && pUser->IsCommBank() ) )
		{
			if( nGold > (int)( pUser->m_dwGoldBank[nSlot] ) )
				nGold = pUser->m_dwGoldBank[nSlot];

			if( CanAdd(pUser->GetGold(), nGold) )
			{
				LogItemInfo aLogItem;
				aLogItem.Action = "G";
				aLogItem.SendName = "BANK";
				aLogItem.RecvName = pUser->GetName();
				aLogItem.WorldId = pUser->GetWorld()->GetID();
				aLogItem.Gold = pUser->GetGold();
				aLogItem.Gold2 = pUser->GetGold() + nGold;
				//aLogItem.ItemName = "SEED";
				_stprintf( aLogItem.szItemName, "%d", II_GOLD_SEED1 );
				aLogItem.itemNumber = nGold;
				aLogItem.Gold_1 = pUser->m_dwGoldBank[pUser->m_nSlot];
				aLogItem.nSlot = nSlot;
				OnLogItem( aLogItem );

				pUser->AddGold( nGold, FALSE );
				pUser->m_dwGoldBank[nSlot] -= nGold;
				pUser->AddPutGoldBank( nSlot, pUser->GetGold(), pUser->m_dwGoldBank[nSlot] );		
			}
		}		
	}
}

void CDPSrvr::OnMoveBankItem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize)
{
}

void CDPSrvr::OnChangeBankPass( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	char szLastPass[10] ={0,};
	char szNewPass[10] ={0,};
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		ar.ReadString( szLastPass, 10 );
		ar.ReadString( szNewPass, 10 );

		if( strlen( szLastPass ) > 4 || strlen( szNewPass ) > 4 )
		{
			WriteError( "%s %d, %s, %s", __FILE__, __LINE__, szLastPass, szNewPass );
			return;
		}

		DWORD dwId, dwItemId;
		ar >> dwId >> dwItemId;

		// ���⼭ ��й�ȣ Ȯ���۾�
		if( 0 == strcmp( szLastPass, pUser->m_szBankPass ) )
		{
			// �н����尡 �ٲ����Ƿ� DB�� Ŭ���̾�Ʈ�� �� �م��ٰ� ������
			strcpy( pUser->m_szBankPass, szNewPass );
			g_dpDBClient.SendChangeBankPass( pUser->GetName(), szNewPass, pUser->m_idPlayer );
			pUser->AddChangeBankPass( 1, dwId, dwItemId );
		}
		else
		{
			// �ٽ� �Է��϶�� �˷���
			// �н����尡 Ʋ����
			pUser->AddChangeBankPass( 0, dwId, dwItemId );
		}
	}
}

void CDPSrvr::OnConfirmBank( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	char szPass[10] ={0,};
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		if( pUser->m_vtInfo.GetOther() )
			return;
		if( pUser->m_vtInfo.VendorIsVendor() )
			return;
		if( pUser->m_bGuildBank )
			return;
#ifdef __S_SERVER_UNIFY
		if( pUser->m_bAllAction == FALSE )
			return;
#endif // __S_SERVER_UNIFY

		// ���⼭ ��й�ȣ Ȯ���۾�
		ar.ReadString( szPass, 10 );

		DWORD dwId, dwItemId;
		ar >> dwId >> dwItemId;

		if( dwId == NULL_ID && !CNpcChecker::GetInstance()->IsCloseNpc( MMI_BANKING, pUser->GetWorld(), pUser->GetPos() ) )
				return;

		if( 0 == strcmp( szPass, pUser->m_szBankPass ) )
		{
			// ��й�ȣ�� Ȯ�� �Ͽ����Ƿ� ������ ���� �հ� ����
			if( dwId != NULL_ID )
			{
				CItemElem* pItemElem = pUser->m_Inventory.GetAtId( dwId );
				if( IsUsableItem( pItemElem ) == FALSE || pItemElem->m_dwItemId != dwItemId )
				{
					return;
				}
				else
				{
					pUser->m_bInstantBank	= TRUE;
					pUser->RemoveItem( (BYTE)( pItemElem->m_dwObjId ), 1 );
				}
			}
			pUser->m_bBank = TRUE;
			pUser->AddconfirmBankPass( 1, dwId, dwItemId );
		}
		else
		{
			// �ٽ� �Է��϶�� �˷���
			// �н����尡 Ʋ����
			pUser->AddconfirmBankPass( 0, dwId, dwItemId );
		}
	}
}

void CDPSrvr::OnSfxHit( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	int idSfxHit;
	int nMagicPower;
	DWORD dwSkill;
	OBJID idAttacker;
	int	nDmgCnt;	// �Ϲ������� 0, ���ӵ������� ��� ù�����Ĵ� 1�̻��� �Ѿ�´�. �̰��� �������� 10%�� �ش�.
	float fDmgAngle, fDmgPower;
	PSfxHit pSfxHit		= NULL;
	CMover* pAttacker	= NULL;

	ar >> idSfxHit >> nMagicPower >> dwSkill >> idAttacker >> nDmgCnt >> fDmgAngle >> fDmgPower;		

	// idAttacker�� NULL_ID�� ����Ŀ�� dpidUser�� �Ѵ�.
	if( idAttacker == NULL_ID )
		pAttacker = g_UserMng.GetUser( dpidCache, dpidUser );	
	else
		pAttacker = prj.GetMover( idAttacker );

	if( IsValidObj( pAttacker ) == FALSE ) 
		return;

	pSfxHit	= pAttacker->m_sfxHitArray.GetSfxHit( idSfxHit );
	if( pSfxHit == NULL ) 
		return;

	CMover* pTarget	= prj.GetMover( pSfxHit->objid );

	/*
	// ˬ	// 06-10-23
	if( dwSkill == SI_MAG_FIRE_HOTAIR )	
	{
		if( IsValidObj( pTarget ) && pTarget->IsLive() )
		{
			SFXHIT_INFO si	=
				{ pTarget->GetId(), nMagicPower, dwSkill, nDmgCnt, fDmgAngle, fDmgPower, pSfxHit->dwAtkFlags	};
			AttackBySFX( pAttacker, si );
		}
		return;
	}
	*/

	pAttacker->RemoveSFX( pSfxHit->objid, idSfxHit, ( IsInvalidObj( pTarget ) || pTarget->IsDie() ), dwSkill );
	pAttacker->m_sfxHitArray.RemoveSfxHit( idSfxHit, TRUE );	// ������ ����
}

// Ŭ��κ��� ���� idSfx�� ��̿� �߰����ѵ�
void CDPSrvr::OnSfxID( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	OBJID	idTarget;
	int		idSfxHit;
	DWORD	dwType,	dwSkill;
	int		nMaxDmgCnt;

	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		ar >> idTarget >> idSfxHit >> dwType >> dwSkill >> nMaxDmgCnt;
		pUser->m_sfxHitArray.Add( idSfxHit, idTarget, dwType, dwSkill, nMaxDmgCnt );		
	}
}

// ������ �������� ������ ���������� ���� ���.
void CDPSrvr::OnSfxClear( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	OBJID	idMover = NULL_ID;
	int	  idSfxHit;

	ar >> idSfxHit;
	ar >> idMover;

	CMover *pMover;

	if( idMover == NULL_ID )
		pMover = g_UserMng.GetUser( dpidCache, dpidUser );
	else
		pMover = prj.GetMover( idMover );

	if( IsValidObj( pMover ) )
	{
		PSfxHit pSfxHit	= pMover->m_sfxHitArray.GetSfxHit( idSfxHit );
		pMover->m_sfxHitArray.RemoveSfxHit( idSfxHit, TRUE );
	}
}


void CDPSrvr::OnMeleeAttack( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	DWORD dwAtkMsg;
	OBJID objid;
	int nParam2, nParam3;
#ifdef __HACK_1023
	FLOAT fVal;
	ar >> dwAtkMsg >> objid >> nParam2 >> nParam3 >> fVal;
#else	// __HACK_1023
	ar >> dwAtkMsg >> objid >> nParam2 >> nParam3;
#endif	// __HACK_1023

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		if( pUser->GetIndex() == 0 )
		{
			WriteError( "PACKETTYPE_MELEE_ATTACK" );
			return;
		}
#ifdef __HACK_1023
		ItemProp* pHandItemProp	= pUser->GetActiveHandItemProp();
		FLOAT f	= pHandItemProp? pHandItemProp->fAttackSpeed: 0.0F;
		if( f != fVal )
		{
			pUser->AddDefinedText( TID_GAME_MODIFY_ATTACK_SPEED );
			return;
		}
#endif	// __HACK_1023

		CMover* pTargetObj	= prj.GetMover( objid );
		if( IsValidObj( pTargetObj ) )
		{
			int nRet = pUser->SendActMsg( (OBJMSG)dwAtkMsg, objid, nParam2, nParam3 );
			
			if( nRet == 0 && pUser->IsFly() == FALSE )
#ifdef __OPT_MEM_0811
				pUser->m_pActMover->m_qMeleeAtkMsg.AddTail( new ACTMSG( dwAtkMsg, objid, nParam2, nParam3 ) );
#else	// __OPT_MEM_0811
				pUser->m_pActMover->m_qMeleeAtkMsg.AddTail( ACTMSG( dwAtkMsg, objid, nParam2, nParam3 ) );
#endif	// __OPT_MEM_0811
			if( nRet != -2 )	// -2�� ��� ���� ����.
			{
				g_UserMng.AddMeleeAttack( pUser, dwAtkMsg, objid, nParam2, nParam3 );
			}
		}
	}
}

void CDPSrvr::OnMeleeAttack2( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	DWORD dwAtkMsg;
	OBJID objid;
	int nParam2, nParam3;
	ar >> dwAtkMsg >> objid >> nParam2 >> nParam3;
	
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		if( pUser->GetIndex() == 0 )
		{
			WriteError( "PACKETTYPE_MELEE_ATTACK2" );
			return;
		}

		CMover* pTargetObj	= prj.GetMover( objid );
		if( IsValidObj( pTargetObj ) )
		{			
			int nRet = pUser->SendActMsg( (OBJMSG)dwAtkMsg, objid, nParam2, nParam3 );
			if( nRet == 0 )
#ifdef __OPT_MEM_0811
				pUser->m_pActMover->m_qMeleeAtkMsg.AddTail( new ACTMSG( dwAtkMsg, objid, nParam2, nParam3 ) );
#else	// __OPT_MEM_0811
				pUser->m_pActMover->m_qMeleeAtkMsg.AddTail( ACTMSG( dwAtkMsg, objid, nParam2, nParam3 ) );
#endif	// __OPT_MEM_0811
			
			if( nRet != -2 )	// -2�� ��� ���� ����.
				g_UserMng.AddMeleeAttack2( pUser, dwAtkMsg, objid, nParam2, nParam3 );
		}
	}
}


void CDPSrvr::OnMagicAttack( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	DWORD dwAtkMsg;
	OBJID objid;
	int nParam2, nParam3, nMagicPower, idSfxHit;
	ar >> dwAtkMsg >> objid >> nParam2 >> nParam3 >> nMagicPower >> idSfxHit;

	nParam2 = 0;		//  m_qMagicAtkMsg���� nParam2�� 0�̸� range attack���� ���ֵȴ�.

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		CMover* pTargetObj	= prj.GetMover( objid );

		if( IsValidObj( pTargetObj ) ) 
		{
			int nRet = pUser->DoAttackMagic( pTargetObj, nMagicPower, idSfxHit );
			
			if( nRet == 0 && pUser->IsFly() == FALSE )
#ifdef __OPT_MEM_0811
				pUser->m_pActMover->m_qMagicAtkMsg.AddTail( new MAGICATKMSG( dwAtkMsg, objid, nParam2, nParam3, nMagicPower, idSfxHit ) );
#else	// __OPT_MEM_0811
				pUser->m_pActMover->m_qMagicAtkMsg.AddTail( (OBJMSG)dwAtkMsg, objid, nParam2, nParam3, nMagicPower, idSfxHit );
#endif	// __OPT_MEM_0811
		}
	}
}

void CDPSrvr::OnRangeAttack( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{	
	DWORD dwAtkMsg;
	OBJID objid;
	DWORD dwItemID;
	int idSfxHit;

	ar >> dwAtkMsg >> objid >> dwItemID >> idSfxHit;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		CMover* pTargetObj	= prj.GetMover( objid );

		if( IsValidObj( pTargetObj ) ) 
		{
			if( pUser->DoAttackRange( pTargetObj, dwItemID, idSfxHit ) == 0 )
#ifdef __OPT_MEM_0811
				pUser->m_pActMover->m_qMagicAtkMsg.AddTail( new MAGICATKMSG( dwAtkMsg, objid, 1, dwItemID, 0, idSfxHit ) );
#else	// __OPT_MEM_0811
				pUser->m_pActMover->m_qMagicAtkMsg.AddTail( (OBJMSG)dwAtkMsg, objid, 1, dwItemID, 0, idSfxHit );
#endif	// __OPT_MEM_0811
		}
	}
}

void CDPSrvr::OnTeleSkill( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
//	OBJID objid;
	D3DXVECTOR3 vPos;
	ar >> vPos;
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		LPSKILL pSkill	= pUser->GetSkill( SI_MAG_MAG_BLINKPOOL );
		if( pSkill == NULL || pSkill->dwLevel == 0 )
			return;
		// ���� ��ġ - Ŭ����ũ ���� x, ��� ��ġ - Ŭ����ũ ���� o = �Ұ�
//		if( prj.IsGuildQuestRegion( pUser->GetPos() ) == FALSE && prj.IsGuildQuestRegion( vPos ) == TRUE )
		D3DXVECTOR3 v	= pUser->GetPos();
		if( prj.IsGuildQuestRegion( v ) == TRUE || prj.IsGuildQuestRegion( vPos ) )		// �� ��ǥ, Ȥ�� ��� ��ǥ�� Ŭ����ũ �����̴�.
		{
			CWorld* pWorld	= pUser->GetWorld();
			if( !pWorld )
				return;
			if( fabs( pWorld->GetLandHeight( v ) - v.y ) > 1.0F || fabs( pWorld->GetLandHeight( vPos ) - vPos.y ) > 1.0F )
				return;
		}
		pUser->REPLACE( g_uIdofMulti, pUser->GetWorld()->GetID(), vPos, REPLACE_NORMAL, pUser->GetLayer() );
	}
}


void CDPSrvr::OnSetTarget( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	OBJID idTarget;
	BYTE bClear;
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		ar >> idTarget >> bClear;	// idTarget�� MOVER��� ��������.

		if( bClear == 2 )		// 2 : Ÿ���������� ���.
			pUser->m_idSetTarget = idTarget;
		if( bClear < 2 )		// 0 / 1 : Ÿ�������𿡰� ���� ��� / Ÿ�������𿡰Լ� ���� ����.
		{
			CMover *pTarget = prj.GetMover( idTarget );		// Ÿ���� ������
			if( IsValidObj( pTarget ) )
			{
				if( bClear )	// Ÿ���� �������.
				{
					if( pTarget->m_idTargeter == pUser->GetId() )	// �ڱⰡ ��Ҵ� Ÿ�ٸ� �ڱⰡ Ǯ���ִ�.
					{
						pTarget->m_idTargeter = NULL_ID;
#if __VER >= 10 // __LEGEND
						DWORD	dwTmpTick = GetTickCount();
						int	nTmpSkillID = pUser->m_pActMover->GetCastingSKillID();
						if( pUser->m_pActMover->GetCastingEndTick() > dwTmpTick && ( nTmpSkillID == SI_KNT_HERO_DRAWING || nTmpSkillID == SI_RIG_HERO_RETURN ) )
						{
							pUser->m_pActMover->SetCastingEndTick(0);
							pUser->m_pActMover->ClearState();				// ���� Ŭ�����ϰ� �ٽ� ����.
						}
#endif  //#if __VER >= 10 // __LEGEND
					}
				}
				else
				{
					if( pTarget->m_idTargeter == NULL_ID )			// Ÿ������ ����� �������� Ÿ���͸� ���� �� �ִ�. 0819
						pTarget->m_idTargeter = pUser->GetId();		// pUser�� Ÿ���� ��Ҵ�.
				}
			}
		}
	}
}


void CDPSrvr::OnSnapshot( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	BYTE c;
	WORD	wHdr;
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		ar >> c;
		while( c-- )
		{
			ar >> wHdr;
			switch( wHdr )
			{
				case SNAPSHOTTYPE_DESTPOS:	OnPlayerDestPos( ar, pUser );	break;
				default:
					{
						// handler not found
						ASSERT( 0 );
						break;
					}
			}
		}
	}
}


void CDPSrvr::OnPlayerDestPos( CAr & ar, CUser* pUser )
{
	D3DXVECTOR3 vPos;
	BYTE fForward;
	ar >> vPos >> fForward;

	D3DXVECTOR3 vDistance	= pUser->GetPos() - vPos;
	if( D3DXVec3LengthSq( &vDistance ) > 1000000.0F )
	{
//		Error( "SNAPSHOTTYPE_DESTPOS" );
		return;
	}

#ifdef __IAOBJ0622
	CShip* pIAObj	= NULL;
	OBJID objidIAObj;
	ar >> objidIAObj;

	if( objidIAObj != NULL_ID )
	{
		pIAObj	= (CShip*)prj.GetCtrl( objidIAObj );
		if( IsValidObj( pIAObj ) == FALSE )
		{
			Error( "CShip object not found" );
			return;
		}
	}
#endif	// __IAOBJ0622

	bool bForward = (fForward != 0);

	pUser->m_pActMover->DefaultSet();
	pUser->BehaviorActionForceSet();	// ����� �ൿ ����̸� ���� �����ϰ�, ���� ����̸� �����Ѵ�.

#ifdef __IAOBJ0622
	if( pIAObj )
		pUser->SetDestPos( pIAObj, vPos );
	else
		pUser->SetDestPos( vPos, bForward );
#else	// __IAOBJ0622
	pUser->SetDestPos( vPos, bForward );
#endif	// __IAOBJ0622

#ifdef __IAOBJ0622
	g_UserMng.AddSetDestPos( pUser, vPos, bForward, objidIAObj );
#else	// __IAOBJ0622
	g_UserMng.AddSetDestPos( pUser, vPos, bForward );
#endif	// __IAOBJ0622
}
/*
void CDPSrvr::OnPlayerDestAngle( CAr & ar, CUser* pUser )
{
	float fDestAngle;
	BYTE fLeft;
	ar >> fDestAngle >> fLeft;

	pUser->SetDestAngle( fDestAngle, (bool)(fLeft != 0) );
	g_UserMng.AddSetDestAngle( pUser, fDestAngle, fLeft );
}
*/
void CDPSrvr::OnModifyMode( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		DWORD dwMode;
		BYTE f;
		u_long idFrom;

		ar >> dwMode >> f >> idFrom;

#ifdef __HACK_0516
		DPID dpid;
		ar >> dpid;
		if( pUser->m_Snapshot.dpidUser != dpid )
		{
			Error( "[%s] try to hack : PACKETTYPE_MODIFYMODE", pUser->GetName() );
			return;
		}
#endif	// __HACK_0516

		if( f )
			pUser->m_dwMode		|= dwMode;
		else
			pUser->m_dwMode		&= ~dwMode;

		g_UserMng.AddModifyMode( pUser );
	}
}

// ����� ��ȯ ��ɾ� 
void CDPSrvr::OnSummonPlayer( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		u_long idOperator;
		DWORD dwWorldID;
		D3DXVECTOR3 vPos;
		u_long uIdofMulti;

		ar >> idOperator;
		ar >> dwWorldID;
		ar >> vPos;
		ar >> uIdofMulti;

#ifdef __HACK_0516
		DPID dpid;
		ar >> dpid;
		if( pUser->m_Snapshot.dpidUser != dpid )
		{
			Error( "[%s] try to hack : PACKETTYPE_SUMMONPLAYER", pUser->GetName() );
			return;
		}
#endif	// __HACK_0516
#ifdef __LAYER_1015
		int nLayer;
		ar >> nLayer;
#endif	// __LAYER_1015

		if( !pUser->GetWorld() )
		{
			WriteError( "PACKETTYPE_SUMMONPLAYER//1" );
			return;
		}

		pUser->REPLACE( uIdofMulti, dwWorldID, vPos, REPLACE_FORCE, nLayer );
	}
}

void CDPSrvr::OnTeleportPlayer( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long )
{
	u_long idOperator;
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	
	CWorld* pWorld;
	if( IsValidObj( pUser ) && ( pWorld = pUser->GetWorld() ) )
	{
		ar >> idOperator;
#ifdef __HACK_0516
		DPID dpid;
		ar >> dpid;
		if( pUser->m_Snapshot.dpidUser != dpid )
		{
			Error( "[%s] try to hack : PACKETTYPE_TELEPORTPLAYER", pUser->GetName() );
			return;
		}
#endif	// __HACK_0516
#ifdef __LAYER_1015
		g_DPCoreClient.SendSummonPlayer( pUser->m_idPlayer, pWorld->GetID(), pUser->GetPos(), idOperator, pUser->GetLayer() );
#else	// __LAYER_1015
		g_DPCoreClient.SendSummonPlayer( pUser->m_idPlayer, pWorld->GetID(), pUser->GetPos(), idOperator );
#endif	// __LAYER_1015
	}
}

void CDPSrvr::OnChangeFace( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	u_long objId;
	DWORD dwFaceNum;
	int cost;

#ifdef __NEWYEARDAY_EVENT_COUPON
	BOOL bUseCoupon;
	ar >> objId >> dwFaceNum >> cost >> bUseCoupon;
#else //__NEWYEARDAY_EVENT_COUPON
	ar >> objId >> dwFaceNum >> cost;
#endif //__NEWYEARDAY_EVENT_COUPON
	
	cost = CHANGE_FACE_COST;
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{	
#ifdef __NEWYEARDAY_EVENT_COUPON
		if(!bUseCoupon)
		{
			if( pUser->GetGold() < cost )
			{
				pUser->AddDefinedText( TID_GAME_LACKMONEY, "" );
				return;
			}
			pUser->AddGold( -( cost ) );
		}
		else
		{
			CItemElem* pItemElem = NULL;
			pItemElem = pUser->m_Inventory.GetAtItemId( II_SYS_SYS_SCR_FACEOFFFREE );
			if( IsUsableItem( pItemElem ) == FALSE )
			{
				pUser->AddDefinedText( TID_GAME_WARNNING_COUPON, "" );
				return;
			}
			pUser->UpdateItem( (BYTE)( pItemElem->m_dwObjId ), UI_NUM, pItemElem->m_nItemNum - 1 );
		}
		pUser->SetHead(dwFaceNum);
#else //__NEWYEARDAY_EVENT_COUPON
		if( pUser->GetGold() < cost )
		{
			pUser->AddDefinedText( TID_GAME_LACKMONEY, "" );
			return;
		}
		pUser->SetHead(dwFaceNum);
		pUser->AddGold( -( cost ) );
#endif //__NEWYEARDAY_EVENT_COUPON
		g_UserMng.AddChangeFace( objId, dwFaceNum );
	}
}

void CDPSrvr::OnExpUp( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{	
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) ) 
	{
		if( (DWORD)AUTH_GAMEMASTER <= pUser->m_dwAuthorization )
		{
			EXPINTEGER nExp;
			ar >> nExp;

			if( pUser->AddExperience( nExp, TRUE, TRUE, TRUE ) )
				pUser->LevelUpSetting();
			else
				pUser->ExpUpSetting();

			pUser->AddSetExperience( pUser->GetExp1(), (WORD)pUser->m_nLevel, pUser->m_nSkillPoint, pUser->m_nSkillLevel );
		}
	}
}

void	CDPSrvr::OnChangeJob( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
// ˬ: hacked
/*
	int nJob;
	BOOL bGamma = TRUE;
	ar >> nJob;
	ar >> bGamma;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( TRUE == IsValidObj( ( CObj* )pUser ) )
	{
		if( bGamma )
		{			
			if( pUser->IsBaseJob() )	// 1�� ����
			{
				if( pUser->GetLevel() != MAX_JOB_LEVEL )
				{
					pUser->AddDefinedText( TID_GAME_CHGJOBLEVEL15 ); // "���� 15�� �Ǿ� �����Ҽ� �ֽ��ϴ�"
					return;
				}			
				
				if( pUser->AddChangeJob( nJob ) )
				{
					( (CUser*)pUser )->AddSetChangeJob( nJob );
					g_UserMng.AddNearSetChangeJob( (CMover*)pUser, nJob, &pUser->m_aJobSkill[MAX_JOB_SKILL] );
					g_dpDBClient.SendLogLevelUp( (CUser*)pUser, 4 );
#if __VER >= 11 // __SYS_PLAYER_DATA
					g_dpDBClient.SendUpdatePlayerData( pUser );
#else	// __SYS_PLAYER_DATA
					g_DPCoreClient.SendPartyMemberJob( (CUser*)pUser );
					g_DPCoreClient.SendFriendChangeJob( (CUser*)pUser );
					if( pUser->m_idGuild != 0 )
						g_DPCoreClient.SendGuildChangeJobLevel( (CUser*)pUser );
#endif	// __SYS_PLAYER_DATA

					return;
				}
				
			}
			else
			if( pUser->IsExpert() )
			{
				if( pUser->GetLevel() < MAX_JOB_LEVEL + MAX_EXP_LEVEL )
				{
					pUser->AddDefinedText( TID_LIMIT_CHANGEJOBLEVEL, "" );	// 60������ �Ǿ� ������ �Ҽ� �ֽ��ϴ�
					return;
				}
				
				if( pUser->AddChangeJob( nJob ) )
				{
					( (CUser*)pUser )->AddSetChangeJob( nJob );
					g_UserMng.AddNearSetChangeJob( (CMover*)pUser, nJob, &pUser->m_aJobSkill[MAX_JOB_SKILL] );
					g_dpDBClient.SendLogLevelUp( (CUser*)pUser, 4 );
#if __VER >= 11 // __SYS_PLAYER_DATA
					g_dpDBClient.SendUpdatePlayerData( pUser );
#else	// __SYS_PLAYER_DATA
					g_DPCoreClient.SendPartyMemberJob( (CUser*)pUser );
					g_DPCoreClient.SendFriendChangeJob( (CUser*)pUser );
					if( pUser->m_idGuild != 0 )
						g_DPCoreClient.SendGuildChangeJobLevel( (CUser*)pUser );
#endif	// __SYS_PLAYER_DATA
					return;
				}
			}
		}
		else
		{
			CItemElem* pItemElem = pUser->m_Inventory.GetAtItemId( II_SYS_SYS_SCR_CHACLA );
			if( IsUsableItem( pItemElem ) == FALSE || pUser->IsBaseJob() || pUser->GetJob() == nJob ) 
			{	// �κ��丮�� �������� ���ų� ����ڰų� ���� ������ �ٱ����� ����
				return;
			}

			if( pUser->IsExpert() )
			{
				if( JOB_VAGRANT == nJob || MAX_EXPERT <= nJob)	// Expert �迭�� �ƴϸ� ����
					return;
			}
			else
			{
				if( nJob < MAX_EXPERT || MAX_PROFESSIONAL <= nJob )	// Pro �迭�� �ƴϸ� ����
					return;
			}

			for( DWORD dwParts = 0; dwParts < MAX_HUMAN_PARTS; dwParts++ )
			{
				if( dwParts == PARTS_HEAD || dwParts == PARTS_HAIR || dwParts == PARTS_RIDE )
					continue;
				CItemElem* pArmor	= pUser->m_Inventory.GetEquip( dwParts );
				if( pArmor )
					return;
			}

			pUser->InitLevel( nJob, pUser->GetLevel(), FALSE );
			ItemProp* pItemProp = pItemElem->GetProp();
			if( pItemProp && pItemProp->dwSfxObj3 != -1 )
				g_UserMng.AddCreateSfxObj((CMover *)pUser, pItemElem->GetProp()->dwSfxObj3, pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z);
			pUser->AddDefinedText( TID_GAME_CHANGECLASS, "%s", prj.m_aJob[pUser->GetJob()].szName );
			
			// ���ȭ ������ ��� �α� ����
			g_dpDBClient.SendLogSMItemUse( "1", pUser, pItemElem, pItemProp );
			pUser->RemoveItem( (BYTE)pItemElem->m_dwObjId, 1 );
		}
	}
*/
}

void	CDPSrvr::OnLogItem( LogItemInfo & info, CItemElem* pItemElem, int nItemCount )
{
	if( pItemElem != NULL )
	{
		info.ItemNo = pItemElem->GetSerialNumber();
		//info.ItemName = pItemElem->GetProp()->szName;
		_stprintf( info.szItemName, "%d", pItemElem->GetProp()->dwID );
		info.itemNumber = nItemCount;
		info.nItemResist = pItemElem->m_bItemResist;
		info.nResistAbilityOption = pItemElem->m_nResistAbilityOption;
		info.nAbilityOption = pItemElem->GetAbilityOption();
		info.Negudo = pItemElem->m_nHitPoint;
		info.MaxNegudo	= pItemElem->m_nRepair;
		info.m_bCharged = pItemElem->m_bCharged;
		info.m_dwKeepTime = pItemElem->m_dwKeepTime;
#if __VER >= 12 // __EXT_PIERCING
		info.nPiercedSize = pItemElem->GetPiercingSize();
		for( int i=0; i<pItemElem->GetPiercingSize(); i++ )
			info.adwItemId[i] = pItemElem->GetPiercingItem( i );
		info.nUMPiercedSize = pItemElem->GetUltimatePiercingSize();
		for( int i=0; i<pItemElem->GetUltimatePiercingSize(); i++ )
			info.adwUMItemId[i] = pItemElem->GetUltimatePiercingItem( i );
#else // __EXT_PIERCING
		info.nPiercedSize = pItemElem->GetPiercingSize();
		info.adwItemId0 = pItemElem->GetPiercingItem( 0 );
		info.adwItemId1 = pItemElem->GetPiercingItem( 1 );
		info.adwItemId2 = pItemElem->GetPiercingItem( 2 );
		info.adwItemId3 = pItemElem->GetPiercingItem( 3 );
#if __VER >= 9 // __ULTIMATE
		info.adwItemId4 = pItemElem->GetPiercingItem( 4 );
#endif // __ULTIMATE
#endif // __EXT_PIERCING
#if __VER >= 11 // __SYS_IDENTIFY
		info.m_iRandomOptItemId = pItemElem->GetRandomOptItemId();
#else	// __SYS_IDENTIFY
		info.m_nRandomOptItemId = pItemElem->GetRandomOptItemId();
#endif	// __SYS_IDENTIFY
#if __VER >= 9 // __PET_0410
		if( pItemElem->m_pPet )
		{
			CPet* pPet = pItemElem->m_pPet;

			info.nPetKind = pPet->GetKind();
			info.nPetLevel = pPet->GetLevel();
			info.dwPetExp = pPet->GetExp();
			info.wPetEnergy = pPet->GetEnergy();
			info.wPetLife = pPet->GetLife();
			info.nPetAL_D = pPet->GetAvailLevel( PL_D );
			info.nPetAL_C = pPet->GetAvailLevel( PL_C );
			info.nPetAL_B = pPet->GetAvailLevel( PL_B );
			info.nPetAL_A = pPet->GetAvailLevel( PL_A );
			info.nPetAL_S = pPet->GetAvailLevel( PL_S );
		}
		// mirchang_100514 TransformVisPet_Log
#if __VER >= 15 // __PETVIS
		else if( pItemElem->IsTransformVisPet() == TRUE )
		{
			info.nPetKind = (BYTE)100;
		}
#endif // __PETVIS
		// mirchang_100514 TransformVisPet_Log
#endif // __PET_0410
	}

	BEFORESENDDUAL( ar, PACKETTYPE_LOG_ALLITEM, DPID_UNKNOWN, DPID_UNKNOWN );
	ar.WriteString( info.Action );
	ar.WriteString( info.SendName );
	ar.WriteString( info.RecvName );
	ar << info.WorldId;
	ar << info.Gold;
	ar << info.Gold2;
	ar << info.ItemNo; // ������ ������ȣ
	ar << info.Negudo; // ������ 
	ar << info.MaxNegudo; // ������ 
	if( _tcslen( info.szItemName ) == 0 )
	{
		_stprintf( info.szItemName, "%d", -1 );
	}
	ar.WriteString( info.szItemName );
	ar << info.itemNumber;
	ar << info.nAbilityOption;
	ar << info.Gold_1;
	ar << info.nSlot;
	ar << info.nSlot1;
	ar << info.nItemResist;
	ar << info.nResistAbilityOption;
	ar << info.m_bCharged;
	ar << info.m_dwKeepTime;
#if __VER >= 12 // __EXT_PIERCING
	ar << info.nPiercedSize;
	for( int i=0; i<info.nPiercedSize; i++ )
		ar << info.adwItemId[i];
	ar << info.nUMPiercedSize;
	for( int i=0; i<info.nUMPiercedSize; i++ )
		ar << info.adwUMItemId[i];
#else // __EXT_PIERCING
	ar << info.nPiercedSize;
	ar << info.adwItemId0;
	ar << info.adwItemId1;
	ar << info.adwItemId2;
	ar << info.adwItemId3;
#if __VER >= 9 // __ULTIMATE
	ar << info.adwItemId4;
#endif // __ULTIMATE
#endif // __EXT_PIERCING
#if __VER >= 11 // __SYS_IDENTIFY
	ar << info.m_iRandomOptItemId;
#else	// __SYS_IDENTIFY
	ar << info.m_nRandomOptItemId;
#endif	// __SYS_IDENTIFY
#if __VER >= 9 // __PET_0410
	ar << info.nPetKind;
	ar << info.nPetLevel;
	ar << info.dwPetExp;
	ar << info.wPetEnergy;
	ar << info.wPetLife;
	ar << info.nPetAL_D;
	ar << info.nPetAL_C;
	ar << info.nPetAL_B;
	ar << info.nPetAL_A;
	ar << info.nPetAL_S;
#endif // __PET_0410
	
	SEND( ar, &g_dpDBClient, DPID_SERVERPLAYER );
}


void CDPSrvr::OnMotion( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		if( pUser->GetIndex() == 0 )
		{
			WriteError( "PACKETTYPE_MOTION" );
			return;
		}

		DWORD dwMsg;
		ar >> dwMsg;

		if( pUser->SendActMsg( (OBJMSG)dwMsg ) == 1 )
		{
//#ifdef __JEFF_VER_8
//			if( dwMsg != OBJMSG_STANDUP )
//#endif	// __JEFF_VER_8
			{
				pUser->ClearDest();
				pUser->ClearDestAngle();
				memset( &pUser->m_CorrAction, 0, sizeof(CORR_ACTION) );
			}
			g_UserMng.AddMotion( pUser, dwMsg );
		}
		else
			pUser->AddMotionError();
	}
}

void CDPSrvr::OnRepairItem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		BYTE c, nId;
		ar >> c;

		if( c == 0 || c > MAX_REPAIRINGITEM )
			return;

		CItemElem* apItemRepair[MAX_REPAIRINGITEM];
		int		anRepair[MAX_REPAIRINGITEM];
		memset( anRepair, 0, sizeof(int) * MAX_REPAIRINGITEM );

		CItemElem* pItemElem;
		int nCost	= 0;
		for( int i = 0; i < c; i++ )
		{
			apItemRepair[i]		= NULL;
			ar >> nId;
			pItemElem	= pUser->m_Inventory.GetAtId( nId );
			if( pItemElem )
			{
				ItemProp* pItemProp	= pItemElem->GetProp();
				if( pItemProp && pItemProp->dwItemKind2 >= IK2_WEAPON_HAND && pItemProp->dwItemKind2 <= IK2_ARMORETC && 
					pItemElem->m_nHitPoint < (int)( pItemProp->dwEndurance ) && pItemElem->m_nRepair < pItemProp->nMaxRepair )
				{
					int nRepair	= 100 - ( ( pItemElem->m_nHitPoint * 100 ) / pItemProp->dwEndurance );
					if( nRepair == 0 )
						continue;
					nCost	+= nRepair * ( pItemProp->dwCost / 1000 + 1 );
					apItemRepair[i]		= pItemElem;
					anRepair[i]		= nRepair;
				}
			}
		}

		if( pUser->GetGold() < nCost )
			return;

		for( int i = 0; i < c; i++ )
		{
			if( apItemRepair[i] )
			{
				pUser->UpdateItem( (BYTE)apItemRepair[i]->m_dwObjId, UI_HP, apItemRepair[i]->GetProp()->dwEndurance );
				pUser->UpdateItem( (BYTE)apItemRepair[i]->m_dwObjId, UI_RN, apItemRepair[i]->m_nRepairNumber + anRepair[i] );
			}
		}
#if __VER < 11 // __REMOVE_ENDURANCE
		pUser->AddGold( -nCost );
#endif // __REMOVE_ENDURANCE
		pUser->AddDefinedText( TID_GAME_REPAIRITEM );
	}
}

void CDPSrvr::OnSetHair( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	BYTE nHair, nR, nG, nB;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
#ifdef __NEWYEARDAY_EVENT_COUPON
		BOOL bUseCoupon;
		ar >> nHair >> nR >> nG >> nB >> bUseCoupon;
#else //__NEWYEARDAY_EVENT_COUPON
		ar >> nHair >> nR >> nG >> nB;// >> nCost;
#endif //__NEWYEARDAY_EVENT_COUPON

		int nCost;
	
		nCost	= CMover::GetHairCost( (CMover*)pUser, nR, nG, nB, nHair );

#ifdef __NEWYEARDAY_EVENT_COUPON
		if( pUser->GetGold() < nCost  && !bUseCoupon)
#else //__NEWYEARDAY_EVENT_COUPON
		if( pUser->GetGold() < nCost )
#endif //__NEWYEARDAY_EVENT_COUPON
		{
			pUser->AddDefinedText( TID_GAME_LACKMONEY, "" );
			return;
		}

		pUser->SetHair( nHair );
		float r, g, b;
		r	= (float)nR / 255.0f;
		g	= (float)nG / 255.0f;
		b	= (float)nB / 255.0f;

#ifdef __NEWYEARDAY_EVENT_COUPON
		if(!bUseCoupon)
			pUser->AddGold( -( nCost ) );
		else
		{
			CItemElem* pItemElem = NULL;
			pItemElem = pUser->m_Inventory.GetAtItemId( II_SYS_SYS_SCR_HAIRCHANGE );
			if( IsUsableItem( pItemElem ) == FALSE )
			{
				pUser->AddDefinedText( TID_GAME_WARNNING_COUPON, "" );
				return;
			}
			pUser->UpdateItem( (BYTE)( pItemElem->m_dwObjId ), UI_NUM, pItemElem->m_nItemNum - 1 );
		}
#else //__NEWYEARDAY_EVENT_COUPON
		pUser->AddGold( -( nCost ) );
#endif //__NEWYEARDAY_EVENT_COUPON

		pUser->SetHairColor( r, g, b );
		g_UserMng.AddSetHair( pUser, nHair, nR, nG, nB );
	}
}

void CDPSrvr::OnBlock( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	BYTE nGu;
	u_long uidPlayerTo, uidPlayerFrom;
	char szNameTo[MAX_NAME] = {0,};
	char szNameFrom[MAX_NAME] = {0,};
	ar >> nGu;
	ar.ReadString( szNameTo, MAX_NAME );
	ar.ReadString( szNameFrom, MAX_NAME );

#if __VER >= 11 // __SYS_PLAYER_DATA
	uidPlayerTo	 = CPlayerDataCenter::GetInstance()->GetPlayerId( szNameTo );
	uidPlayerFrom	= CPlayerDataCenter::GetInstance()->GetPlayerId( szNameFrom );
#else	// __SYS_PLAYER_DATA
	uidPlayerTo = prj.GetPlayerID( szNameTo );
	uidPlayerFrom = prj.GetPlayerID( szNameFrom );
#endif	// __SYS_PLAYER_DATA

	if( uidPlayerTo > 0 && uidPlayerFrom > 0 )
		g_DPCoreClient.SendBlock( nGu, uidPlayerTo, szNameTo, uidPlayerFrom );
}

#if __VER >= 12 // __J12_0
DWORD WhatEleCard( DWORD dwItemType )
{	// �Ӽ� ���� �� ī���� ������ 
	// �Ӽ� �� �ϳ��� ���յ�
	switch( dwItemType )
	{
		case SAI79::FIRE:
			return II_GEN_MAT_ELE_FLAME;
		case SAI79::WATER:
			return II_GEN_MAT_ELE_RIVER;
		case SAI79::ELECTRICITY:
			return II_GEN_MAT_ELE_GENERATOR;
		case SAI79::EARTH:
			return II_GEN_MAT_ELE_DESERT;
		case SAI79::WIND:
			return II_GEN_MAT_ELE_CYCLON;
		default:
			return 0;
	}
}
#else	// __J12_0
DWORD WhatEleCard( DWORD dwLV, DWORD dwItemType )
{
	switch( dwItemType )
	{
		case SAI79::FIRE:
			return ( dwLV <= 4 ) ? II_GEN_MAT_ELE_TOUCH: II_GEN_MAT_ELE_FLAME;
		case SAI79::WATER:
			return ( dwLV <= 4 ) ? II_GEN_MAT_ELE_LAKE: II_GEN_MAT_ELE_RIVER;
		case SAI79::ELECTRICITY:
			return ( dwLV <= 4 ) ? II_GEN_MAT_ELE_VOLTAGE: II_GEN_MAT_ELE_GENERATOR;
		case SAI79::EARTH:
			return ( dwLV <= 4 ) ? II_GEN_MAT_ELE_STONE: II_GEN_MAT_ELE_DESERT;
		case SAI79::WIND:
			return ( dwLV <= 4 ) ? II_GEN_MAT_ELE_GALE: II_GEN_MAT_ELE_CYCLON;
		default:
			return 0;
	}
}
#endif	// __J12_0

void CDPSrvr::OnPiercingSize( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize)
{
	DWORD dwId1, dwId2, dwId3;
	
	ar >> dwId1;
	ar >> dwId2;
	ar >> dwId3;
	
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
#if __VER >= 12 // __EXT_PIERCING
	if( IsValidObj( pUser ) )
		CItemUpgrade::GetInstance()->OnPiercingSize( pUser, dwId1, dwId2, dwId3 );
#else // __EXT_PIERCING
	if( IsValidObj( pUser ) )
	{
		CItemElem* pItemElem0	= pUser->m_Inventory.GetAtId( dwId1 );
		CItemElem* pItemElem1	= pUser->m_Inventory.GetAtId( dwId2 );
		CItemElem* pItemElem2	= pUser->m_Inventory.GetAtId( dwId3 );

		if( IsUsableItem( pItemElem0 ) == FALSE || IsUsableItem( pItemElem1 ) == FALSE )
			return;

		// ˬ
		if( pUser->m_vtInfo.GetOther() )	// �ŷ����� ����� ������?
			return;
		if( pUser->m_vtInfo.VendorIsVendor() )		// ���� �Ȱ� ������?
			return;

		int nError = 1;
		if( IsPiercingSize( pUser, pItemElem0, pItemElem1, pItemElem2, nError ) == FALSE )
		{
			if( nError != 1 )
				pUser->AddDefinedText( nError );
			return;
		}

		LogItemInfo aLogItem;
		aLogItem.SendName = pUser->GetName();
		aLogItem.RecvName = "PIERCING";
		aLogItem.WorldId = pUser->GetWorld()->GetID();
		
		int nCost = pItemElem0->GetProp()->dwItemRare * ( 200  + ( pItemElem0->GetPiercingSize()+1) * 200 );

		if( 0 < nCost )
		{
			if( pUser->GetGold() < nCost )
			{
				pUser->AddDefinedText( TID_GAME_LACKMONEY , "" );
				return;
			}
				
			pUser->AddGold( -( nCost ) );

			aLogItem.Gold = pUser->GetGold() + nCost;
			aLogItem.Gold2 = pUser->GetGold();
			aLogItem.Action = "!";
			//aLogItem.ItemName = "SEED";
			_stprintf( aLogItem.szItemName, "%d", II_GOLD_SEED1 );
			aLogItem.itemNumber = nCost;
			OnLogItem( aLogItem );
		}
		else
		{
			return;
		}
		aLogItem.Gold = aLogItem.Gold2 = pUser->GetGold();


		int nPersent = 0;
		int n8DicePersent[4] = { 80, 50, 20, 5 };
#if __VER >= 8 //__Y_NEW_ENCHANT
		if( pItemElem1->GetProp()->dwID == II_GEN_MAT_MOONSTONE
			|| pItemElem1->GetProp()->dwID == II_GEN_MAT_MOONSTONE_1 )
			nPersent = n8DicePersent[pItemElem0->GetPiercingSize()];
#else //__Y_NEW_ENCHANT
		int n10DicePersent[4] = { 85, 55, 25, 10 };
		if( pItemElem1->GetProp()->dwID == II_GEN_MAT_DIE_EIGHT )
			nPersent = n8DicePersent[pItemElem0->GetPiercingSize()];
		else if( pItemElem1->GetProp()->dwID == II_GEN_MAT_DIE_TEN )
			nPersent = n10DicePersent[pItemElem0->GetPiercingSize()];
#endif //__Y_NEW_ENCHANT

		if( xRandom( 100 ) > nPersent )
		{	// ����
			aLogItem.Action = "!";
			OnLogItem( aLogItem, pItemElem0, pItemElem0->m_nItemNum );
			pUser->AddPlaySound( SND_INF_UPGRADEFAIL );
			g_UserMng.AddCreateSfxObj((CMover *)pUser, XI_INT_FAIL, pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z);
			pUser->AddDefinedText( TID_MMI_PIERCINGFAIL , "" );

			if( pItemElem2 == NULL )								// ���ȭ �������� ������� �ʾҴٸ� 
				pUser->RemoveItem( dwId1, (short)1 );	// �Ǿ�� ��� �������� �����ȴ�.
		}
		else
		{	// ����			
			pUser->AddPlaySound( SND_INF_UPGRADESUCCESS );			
			g_UserMng.AddCreateSfxObj((CMover *)pUser, XI_INT_SUCCESS, pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z);			
			pUser->UpdateItem( (BYTE)pItemElem0->m_dwObjId, UI_PIERCING_SIZE, pItemElem0->GetPiercingSize() + 1 );
			pUser->AddDefinedText( TID_MMI_PIERCINGSUCCESS , "" );

			aLogItem.Action = "#";
			OnLogItem( aLogItem, pItemElem0, pItemElem0->m_nItemNum );
		}
		aLogItem.Action = "!";
		OnLogItem( aLogItem, pItemElem1, pItemElem1->m_nItemNum );
		
		// ���̽��� ��������� �����Ѵ�.
		pUser->RemoveItem( dwId2, (short)1 );
		
		if( dwId3 != NULL_ID )
		{
			aLogItem.Action = "!";
			if( IsUsableItem( pItemElem2 ) )
			{
				OnLogItem( aLogItem, pItemElem2, pItemElem2->m_nItemNum );
				g_dpDBClient.SendLogSMItemUse( "2", pUser, pItemElem2, pItemElem2->GetProp() );
				pUser->RemoveItem( dwId3, (short)1 );
			}
			else
			{
				OnLogItem( aLogItem, NULL, 0 );
			}
		}
	}
#endif // __EXT_PIERCING
}

void CDPSrvr::OnItemTransy( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	OBJID objidTarget, objidTransy;
	ar >> objidTarget;
	ar >> objidTransy;
#ifdef __SYS_ITEMTRANSY
	DWORD dwChangeId;
	BOOL bCash;
	ar >> dwChangeId;
	ar >> bCash;
#endif // __SYS_ITEMTRANSY
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
#ifdef __SYS_ITEMTRANSY
		CItemUpgrade::GetInstance()->OnItemTransy( pUser, objidTarget, objidTransy, dwChangeId, bCash );
#else // __SYS_ITEMTRANSY
		CItemElem* pItemElemTarget = pUser->m_Inventory.GetAtId( objidTarget );
		CItemElem* pItemElemTransy = pUser->m_Inventory.GetAtId( objidTransy );

		// ������� ���������� �˻�
		if( IsUsableItem( pItemElemTarget ) == FALSE || IsUsableItem( pItemElemTransy ) == FALSE )
			return;

		// �����Ǿ� �ִ� �������̸� ����( ������~~~ ���� )
		if( pUser->m_Inventory.IsEquip( objidTarget ) || pUser->m_Inventory.IsEquip( objidTransy ) )
		{
			pUser->AddDefinedText( TID_GAME_EQUIPPUT , "" );
			return;
		}
		
		// ��ᰡ Ʈ����(ITM)���� �˻�
		if( !(pItemElemTransy->GetProp()->dwID == II_CHR_SYS_SCR_ITEMTRANSY_A || pItemElemTransy->GetProp()->dwID == II_CHR_SYS_SCR_ITEMTRANSY_B) )
			return;
		
		// ���� �˻�
		if( pItemElemTransy->GetProp()->dwID == II_CHR_SYS_SCR_ITEMTRANSY_A )
		{
			if( 61 <= pItemElemTarget->GetProp()->dwLimitLevel1 )
				return;
		}
		else
		{
			if( pItemElemTarget->GetProp()->dwLimitLevel1 < 61 )
				return;
		}

		// ������ �ִ� ������ ���� �˻�
		ItemProp* pItemPropChange = NULL;
		ItemProp* pItemProp = pItemElemTarget->GetProp();
		
		pItemPropChange = pUser->GetTransyItem( pItemProp );

		if( pItemPropChange == NULL )
			return;

		// ������ Ʈ���� ����
		pUser->AddPlaySound( SND_INF_UPGRADESUCCESS );			
		g_UserMng.AddCreateSfxObj((CMover *)pUser, XI_INT_SUCCESS, pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z);			

		// ���� �������� Elem ������ ���� �ϰ� ����...
		CItemElem ItemElemSend;
		ItemElemSend = *pItemElemTarget;
		ItemElemSend.m_dwItemId = pItemPropChange->dwID;
		ItemElemSend.m_nHitPoint	= pItemPropChange->dwEndurance;		// ������ 100%

		g_dpDBClient.SendLogSMItemUse( "1", pUser, pItemElemTransy, pItemElemTransy->GetProp() );
		g_dpDBClient.SendLogSMItemUse( "1", pUser, pItemElemTarget, pItemElemTarget->GetProp(), "RemoveItem" );	
		g_dpDBClient.SendLogSMItemUse( "1", pUser, &ItemElemSend, ItemElemSend.GetProp(), "CreateItem" );	
		pUser->AddDefinedText( TID_GAME_ITEM_TRANSY_SUCCESS, "\"%s\" \"%s\"", pItemElemTarget->GetProp()->szName, ItemElemSend.GetProp()->szName );

		// ���� ������ ��� ����
		pUser->RemoveItem( objidTarget, (short)1 );
		pUser->RemoveItem( objidTransy, (short)1 );		

		// ���ο� ������ ����
		pUser->CreateItem( &ItemElemSend );
#endif // __SYS_ITEMTRANSY
	}
}

void CDPSrvr::OnExpBoxInfo( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize)
{
	u_long uIdPlayer;
	OBJID objid;
	
	ar >> uIdPlayer >> objid;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		CCtrl* pCtrl;

		pCtrl	= prj.GetCtrl( objid );
		if( IsValidObj( pCtrl ) )
			pUser->AddExpBoxInfo( objid,  ((CCommonCtrl*)pCtrl)->m_CtrlElem.m_dwSet, ((CCommonCtrl*)pCtrl)->m_dwDelete - timeGetTime(), ((CCommonCtrl*)pCtrl)->m_idExpPlayer );
	}
}

void CDPSrvr::OnPiercing( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize)
{
	DWORD dwId1, dwId2;
	
	ar >> dwId1;
	ar >> dwId2;
	
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
#if __VER >= 12 // __EXT_PIERCING
	if( IsValidObj( pUser ) )
		CItemUpgrade::GetInstance()->OnPiercing( pUser, dwId1, dwId2 );
#else // __EXT_PIERCING
	if( IsValidObj( pUser ) )
	{
		// �κ��丮�� �ִ��� �����Ǿ� �ִ��� Ȯ���� �ؾ� ��
		CItemElem* pItemElem0	= pUser->m_Inventory.GetAtId( dwId1 );
		CItemElem* pItemElem1	= pUser->m_Inventory.GetAtId( dwId2 );
		if( IsUsableItem( pItemElem0 ) == FALSE || IsUsableItem( pItemElem1 ) == FALSE )
			return;

		// �����Ǿ� �ִ� �������� �Ǿ�� ����
		if( pUser->m_Inventory.IsEquip( dwId1 ) )
		{
			pUser->AddDefinedText( TID_GAME_EQUIPPUT , "" );
			return;
		}			

		// ī�尡 IK3_SOCKETCARD�� �ƴϸ� �Ǿ�� ����
		if( pItemElem1->GetProp()->dwItemKind3 != IK3_SOCKETCARD )
		{
			pUser->AddDefinedText( TID_UPGRADE_ERROR_WRONGUPLEVEL , "" );			
			return;					
		}

		// ī�尡 �� �������� ��Ʈ( IK3_SUIT ) ���� �˻�
		if( pItemElem0->GetProp()->dwItemKind3 != IK3_SUIT )
		{
			pUser->AddDefinedText(  TID_PIERCING_POSSIBLE_ITEM, "" );
			return;
		}
		
		// �� �Ǿ�̵ȼ��� ��ü ���� ���Ѵ�.
		int nSize = pItemElem0->GetPiercingSize();
		
		int nCount = 0;
		for( int j = 0; j < nSize; j++ )
		{
			if( pItemElem0->GetPiercingItem( j ) != 0 )
				nCount++;
		}
		
		// ����� ������ �ߴ�
		if( nCount == nSize )
		{
			pUser->AddDefinedText( TID_PIERCING_ERROR_NOPIERCING, "" );
			return;
		}

		// ˬ
		if( pUser->m_vtInfo.GetOther() )	// �ŷ����� ����� ������?
			return;
		if( pUser->m_vtInfo.VendorIsVendor() )		// ���� �Ȱ� ������?
			return;
		
		LogItemInfo aLogItem;
		aLogItem.SendName = pUser->GetName();
		aLogItem.RecvName = "PIERCING";
		aLogItem.WorldId = pUser->GetWorld()->GetID();
		aLogItem.Gold = aLogItem.Gold2 = pUser->GetGold();
		
		if( pUser->Pierce( pItemElem0, pItemElem1->m_dwItemId ) )
		{
			aLogItem.Action = "$";
			OnLogItem( aLogItem, pItemElem0, pItemElem0->m_nItemNum );
			aLogItem.Action = "!";
			OnLogItem( aLogItem, pItemElem1, pItemElem1->m_nItemNum );

			// ������ �ڱ� ����~
			pUser->AddPlaySound( SND_INF_UPGRADESUCCESS );			
			g_UserMng.AddCreateSfxObj((CMover *)pUser, XI_INT_SUCCESS, pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z);			
			// ��� ������ ����
			pUser->RemoveItem( dwId2, (short)1 );		
		}
	}
#endif // __EXT_PIERCING
}

#if __VER >= 11 // __PIERCING_REMOVE
// �Ǿ�� �ɼ� ����(ī�� ����)
void CDPSrvr::OnPiercingRemove( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize)
{
	OBJID objId;
	ar >> objId;
	
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	if( !IsValidObj( pUser ) )
		return;
	
#if __VER >= 12 // __EXT_PIERCING
	CItemUpgrade::GetInstance()->OnPiercingRemove( pUser, objId );
#else // __EXT_PIERCING
	CItemElem* pItemSuit = pUser->m_Inventory.GetAtId( objId );
	if( !IsUsableItem( pItemSuit ) || pItemSuit->GetProp()->dwItemKind3 != IK3_SUIT )
		return;

	if( pUser->m_Inventory.IsEquip( objId ) )
		return;

	// �Ǿ�� �ɼ��� ���� ���
	if( pItemSuit->GetPiercingSize() == 0 || pItemSuit->GetPiercingItem( 0 ) == 0 )
	{
		pUser->AddDefinedText( TID_GAME_REMOVE_PIERCING_ERROR );
		return;
	}

	int nPayPenya = 1000000; // ������ ���
	if( pUser->GetGold() < nPayPenya )	// ��İ� �����ϴ�.
	{
		pUser->AddDefinedText( TID_GAME_LACKMONEY );
		return;
	}

	for( int i=pItemSuit->GetPiercingSize()-1; i>=0; i-- )
	{
		if( pItemSuit->GetPiercingItem( i ) != 0 )
		{
			pUser->AddGold( -nPayPenya );	// ��� ����
			pUser->AddDefinedText( TID_GAME_REMOVE_PIERCING_SUCCESS );
			pUser->UpdateItem( pItemSuit->m_dwObjId, UI_PIERCING, MAKELONG( i, 0 ) );
			
			LogItemInfo aLogItem;
			aLogItem.Action = "$";
			aLogItem.SendName = pUser->GetName();
			aLogItem.RecvName = "PIERCING_REMOVE";
			aLogItem.WorldId = pUser->GetWorld()->GetID();
			aLogItem.Gold = pUser->GetGold() + nPayPenya;
			aLogItem.Gold2 = pUser->GetGold();
			aLogItem.Gold_1 = -nPayPenya;
			OnLogItem( aLogItem, pItemSuit, 1 );
			break;
		}
	}
#endif // __EXT_PIERCING
}
#endif // __PIERCING_REMOVE

void CDPSrvr::OnCreateSfxObj( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize)
{
	DWORD dwSfxId;
	u_long uIdPlayer;
	BOOL bFlag;
	ar >> dwSfxId >> uIdPlayer >> bFlag;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		if( uIdPlayer == NULL_ID )
		{
			g_UserMng.AddCreateSfxObj((CMover *)pUser, dwSfxId, pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z, bFlag);
		}
		else
		{
			CUser* pUsertmp	= (CUser*)prj.GetUserByID( uIdPlayer );
			if( IsValidObj( pUsertmp ) )
				g_UserMng.AddCreateSfxObj((CMover *)pUsertmp, dwSfxId, pUsertmp->GetPos().x, pUsertmp->GetPos().y, pUsertmp->GetPos().z, bFlag);
		}		
	}
}

// 1000 ������ �ۼ�Ʈ�� �ѱ��.
// dwID - ���� ������ ���̵�, n - ���� �ܰ� 
int GetEnchantPercent( DWORD dwID, int n )
{
#if __VER >= 8 //__Y_NEW_ENCHANT	
	static int nPersent1[10] = { 1000, 1000, 700, 500, 400, 300, 200, 100, 50, 20 };
#else //__Y_NEW_ENCHANT
	static int nPersent1[10] = {  900,  850, 800, 600, 450, 300, 200, 100, 50, 20 };
#endif //__Y_NEW_ENCHANT
	static int nPersent2[10] = { 1000, 1000, 900, 750, 550, 400, 250, 150, 80, 40 };

	float fFactor = 1.0f;
	if( ::GetLanguage() != LANG_KOR && n >= 3 )	// ���� 4���� 10% Ȯ�� ���� 
		fFactor = 0.9f;

	if( dwID == II_GEN_MAT_DIE_TWELVE )
		return	( (int)( nPersent2[n] * fFactor ) );
	else
		return	( (int)( nPersent1[n] * fFactor ) );
}

#if __VER >= 11 // __SYS_IDENTIFY
void CDPSrvr::OnRemoveItemLevelDown( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize)
{
	DWORD dwId;
	ar >> dwId;
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		CItemElem* pItemElem	= (CItemElem*)pUser->GetItemId( dwId );
		if( !IsUsableItem( pItemElem ) )
			return;
		if( pItemElem->GetLevelDown() == 0 )
		{
			pUser->AddDefinedText( TID_GAME_INVALID_TARGET_ITEM );
			return;
		}
		PutItemLog( pUser, "v", "OnRemoveItemLevelDown", pItemElem );
		pItemElem->SetLevelDown();
		pUser->UpdateItemEx( (BYTE)( pItemElem->m_dwObjId ), UI_RANDOMOPTITEMID, pItemElem->GetRandomOptItemId() );
	}
}

void CDPSrvr::OnDoUseItemTarget( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize)
{
	DWORD dwMaterial, dwTarget;
	ar >> dwMaterial >> dwTarget;
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		CItemElem* pMaterial	= (CItemElem*)pUser->GetItemId( dwMaterial );
		CItemElem* pTarget	= (CItemElem*)pUser->GetItemId( dwTarget );
		if( !IsUsableItem( pMaterial ) || !IsUsableItem( pTarget ) )
			return;
		if( pUser->m_Inventory.IsEquip( dwTarget ) )
		{
			pUser->AddDefinedText( TID_GAME_EQUIPPUT );
			return;
		}
		BOOL	b	= FALSE;
		switch( pMaterial->m_dwItemId ) 
		{
			case II_SYS_SYS_QUE_PETRESURRECTION02_S:
			case II_SYS_SYS_QUE_PETRESURRECTION02_A:
			case II_SYS_SYS_QUE_PETRESURRECTION02_B:
				{
					CPet* pPet	= pTarget->m_pPet;
					if( !pPet || !pTarget->IsFlag( CItemElem::expired ) )
					{
						pUser->AddDefinedText( TID_GAME_PETRESURRECTION_WRONG_TARGET_01 );
					}
					else
					{
						BYTE nLevel		= pPet->GetLevel();
						if( ( nLevel == PL_B && pMaterial->m_dwItemId != II_SYS_SYS_QUE_PETRESURRECTION02_B )
							|| ( nLevel == PL_A && pMaterial->m_dwItemId != II_SYS_SYS_QUE_PETRESURRECTION02_A )
							|| ( nLevel == PL_S && pMaterial->m_dwItemId != II_SYS_SYS_QUE_PETRESURRECTION02_S )
							|| ( nLevel < PL_B )
							)
						{
							pUser->AddDefinedText( TID_GAME_PETRESURRECTION_WRONG_TARGET_02 );
						}
						else
						{
							PutItemLog( pUser, "r", "::PetResurrection", pTarget );

							pTarget->ResetFlag( CItemElem::expired );
							pUser->UpdateItem( (BYTE)( pTarget->m_dwObjId ), UI_FLAG, MAKELONG( pTarget->m_dwObjIndex, pTarget->m_byFlag ) );
							pPet->SetLife( 0 );
							pPet->SetEnergy( pPet->GetMaxEnergy() / 2 );
							pUser->AddPetState( pTarget->m_dwObjId, pPet->GetLife(), pPet->GetEnergy(), pPet->GetExp() );
							pUser->AddDefinedText( TID_GAME_PETRESURRECTION_SUCCESS );
							b	= TRUE;
						}
					}
					break;
				}
			case II_SYS_SYS_SCR_AWAKECANCEL:
			case II_SYS_SYS_SCR_AWAKECANCEL02:
				b	= DoUseItemTarget_InitializeRandomOption( pUser, pTarget, CRandomOptionProperty::eAwakening,
					TID_GAME_AWAKECANCEL_INFO, TID_GAME_AWAKECANCEL,
					"r", "::AwakeCancel" );
				break;
			case II_SYS_SYS_SCR_AWAKE:
				b	= DoUseItemTarget_GenRandomOption( pUser, pTarget, CRandomOptionProperty::eAwakening, 
					0, TID_GAME_INVALID_TARGET_ITEM, TID_GAME_AWAKE_OR_BLESSEDNESS01,
					"r", "::Awake" );
				break;
			case II_SYS_SYS_SCR_BLESSEDNESS:
			case II_SYS_SYS_SCR_BLESSEDNESS02:
				b	= DoUseItemTarget_GenRandomOption( pUser, pTarget, CRandomOptionProperty::eBlessing, 
					0, TID_GAME_USE_BLESSEDNESS_INFO, TID_GAME_BLESSEDNESS_INVALID_ITEM,
					"r", "::Blessedness" );
				break;
#if __VER >= 12 // __PET_0519
			case II_SYS_SYS_SCR_EATPETAWAKE:	// ���� ����
				b	= DoUseItemTarget_GenRandomOption( pUser, pTarget, CRandomOptionProperty::eEatPet,
					TID_GAME_PETAWAKE_S00, TID_GAME_PETAWAKE_E00, TID_GAME_PETAWAKE_E00,
					"r", "EATPETAWAKE" );
				break;
			case II_SYS_SYS_SCR_PETAWAKE:	// �ý��� �� ����
				b	= DoUseItemTarget_GenRandomOption( pUser, pTarget, CRandomOptionProperty::eSystemPet, 
					TID_GAME_PETAWAKE_S00, TID_GAME_PETAWAKE_E00, TID_GAME_PETAWAKE_E00,
					"r", "PETAWAKE" );
				break;
			case II_SYS_SYS_SCR_PETAWAKECANCEL:		// �ý��� �� ���� ���
				b	= DoUseItemTarget_InitializeRandomOption( pUser, pTarget, CRandomOptionProperty::eSystemPet,
					TID_GAME_PETAWAKECANCEL_S00, TID_GAME_PETAWAKECANCEL_E00,
					"r", "PETAWAKECANCEL" );
				break;
#endif	// __PET_0519
			case II_SYS_SYS_SCR_LEVELDOWN01:
			case II_SYS_SYS_SCR_LEVELDOWN02:
				b	= DoUseItemTarget_ItemLevelDown( pUser, pMaterial, pTarget );
				break;
			default:
				break;
		}
		if( b )
		{
			pUser->AddPlaySound( SND_INF_UPGRADESUCCESS );
			if( pUser->IsMode( TRANSPARENT_MODE ) == 0 )
				g_UserMng.AddCreateSfxObj( pUser, XI_INT_SUCCESS, pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z );

			PutItemLog( pUser, "u", "OnDoUseItemTarget", pMaterial );

			pUser->UpdateItem( (BYTE)( pMaterial->m_dwObjId ), UI_NUM, pMaterial->m_nItemNum - 1 );	
		}
	}
}
#endif	// __SYS_IDENTIFY

#if __VER >= 14 // __SMELT_SAFETY
void CDPSrvr::OnSmeltSafety( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize)
{
	OBJID dwItemId, dwItemMaterialId, dwItemProtScrId, dwItemSmeltScrId;

	//	pItemSmeltScrId - �Ϲ����ý��� ���õη縶��(�����ҽÿ� Client���� NULL_ID�� �Է�)
	ar >> dwItemId >> dwItemMaterialId >> dwItemProtScrId >> dwItemSmeltScrId;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );

	if( IsValidObj( pUser ) )
	{
#ifdef __QUIZ
		if( pUser->GetWorld() && pUser->GetWorld()->GetID() == WI_WORLD_QUIZ )
		{
			pUser->AddSmeltSafety( 0 );
			return;
		}
#endif // __QUIZ
		// ˬ
		if( pUser->m_vtInfo.GetOther() || pUser->m_vtInfo.VendorIsVendor() )	// �ŷ����� ����� ������?
		{
			pUser->AddSmeltSafety( 0 );
			return;
		}

		// �κ��丮�� �ִ��� �����Ǿ� �ִ��� Ȯ���� �ؾ� ��
		CItemElem* pItemElem0	= pUser->m_Inventory.GetAtId( dwItemId );
		CItemElem* pItemElem1	= pUser->m_Inventory.GetAtId( dwItemMaterialId );
		CItemElem* pItemElem2	= pUser->m_Inventory.GetAtId( dwItemProtScrId );
		CItemElem* pItemElem3	= NULL;
		if( dwItemSmeltScrId != NULL_ID )
		{
			pItemElem3	= pUser->m_Inventory.GetAtId( dwItemSmeltScrId );
			if( !IsUsableItem( pItemElem3 ) )
				return;
		}

		if( IsUsableItem( pItemElem0 ) == FALSE || IsUsableItem( pItemElem1 ) == FALSE || IsUsableItem( pItemElem2 ) == FALSE )
		{
			pUser->AddSmeltSafety( 0 );
			return;
		}

		// �����Ǿ� �ִ� �������� ���� ����
		if( pUser->m_Inventory.IsEquip( dwItemId ) )
		{
			pUser->AddSmeltSafety( 0 );
			return;
		}

		if( pItemElem0->m_nResistSMItemId != 0 ) // ���ȭ ������ �������̸� �Ұ���
		{
			pUser->AddSmeltSafety( 0 );
			return;
		}
		
		BYTE nResult = CItemUpgrade::GetInstance()->OnSmeltSafety( pUser, pItemElem0, pItemElem1, pItemElem2, pItemElem3 );

		pUser->AddSmeltSafety( nResult );
	}
}
#endif // __SMELT_SAFETY

void CDPSrvr::OnEnchant( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize)
{
	DWORD dwItemId, dwItemMaterialId;
	
	ar >> dwItemId;
	ar >> dwItemMaterialId;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
#ifdef __QUIZ
		if( pUser->GetWorld() && pUser->GetWorld()->GetID() == WI_WORLD_QUIZ )
			return;
#endif // __QUIZ
		// ˬ
		if( pUser->m_vtInfo.GetOther() )	// �ŷ����� ����� ������?
			return;
		if( pUser->m_vtInfo.VendorIsVendor() )		// ���� �Ȱ� ������?
			return;
#if __VER >= 11 // __SYS_COLLECTING
#if __VER < 14 // __SMELT_SAFETY
		if( pUser->PreRefine( dwItemId, dwItemMaterialId ) )
			return;
#endif // __SMELT_SAFETY
#endif	// __SYS_COLLECTING


		// �κ��丮�� �ִ��� �����Ǿ� �ִ��� Ȯ���� �ؾ� ��
		CItemElem* pItemElem0	= pUser->m_Inventory.GetAtId( dwItemId );
		CItemElem* pItemElem1	= pUser->m_Inventory.GetAtId( dwItemMaterialId );		

		if( IsUsableItem( pItemElem0 ) == FALSE || IsUsableItem( pItemElem1 ) == FALSE )
			return;

		if( pItemElem0->m_nResistSMItemId != 0 ) // ���ȭ ������ �������̸� �Ұ���
		{
			pUser->AddDefinedText( TID_GAME_NOTUPGRADE , "" );
			return;
		}

#if __VER < 11 // __SYS_COLLECTING
		if( pUser->m_Inventory.IsEquip( dwItemId ) )
		{
			pUser->AddDefinedText( TID_GAME_EQUIPPUT , "" );
			return;
		}
#endif	// __SYS_COLLECTING

#if __VER < 14 // __SMELT_SAFETY
	#if __VER >= 8 //__Y_NEW_ENCHANT
		if( pItemElem1->GetProp()->dwItemKind3 != IK3_ELECARD && pItemElem1->GetProp()->dwItemKind3 != IK3_ENCHANT )
	#else //__Y_NEW_ENCHANT
		if( pItemElem1->GetProp()->dwItemKind3 != IK3_ELECARD && pItemElem1->GetProp()->dwItemKind3 != IK3_DICE )
	#endif //__Y_NEW_ENCHANT
		{
			pUser->AddDefinedText( TID_UPGRADE_ERROR_WRONGUPLEVEL , "" );			
			return;					
		}
#endif // __SMELT_SAFETY
#if __VER >= 13 // __EXT_ENCHANT
		CItemUpgrade::GetInstance()->OnEnchant( pUser, pItemElem0, pItemElem1 );
#else // __EXT_ENCHANT
		int nLevDown = 3;		
		int*  pAbilityOption = NULL;
			
		// �Ӽ������� ��Ʈ��, ���⿡�� ����
		if( pItemElem1->GetProp()->dwItemKind3 == IK3_ELECARD )
		{
			pAbilityOption = &(pItemElem0->m_nResistAbilityOption);

			if( pAbilityOption == NULL )
				return;

			// 2���� �Ӽ��� �����Ҽ� ����
			if( pItemElem0->m_bItemResist != SAI79::NO_PROP )
			{
				if( pItemElem0->m_bItemResist != pItemElem1->GetProp()->eItemType )
				{
					pUser->AddDefinedText( TID_UPGRADE_ERROR_TWOELEMENT , "" );								
					return;
				}
			}

			if( !CItemElem::IsEleRefineryAble(pItemElem0->GetProp()) )
			{
				pUser->AddDefinedText( TID_GAME_NOTEQUALITEM , "" );			
				return;								
			}

#if __VER >= 12 // __J12_0
			// �Ӽ� �� �ϳ��� �Ӽ� ���� ī�带 ����ϵ��� ����
			DWORD dwReqCard	= WhatEleCard( pItemElem1->GetProp()->eItemType );
#else	// __J12_0
			DWORD dwReqCard = WhatEleCard( *pAbilityOption, pItemElem1->GetProp()->eItemType );
#endif	// __J12_0

			if( pItemElem1->GetProp()->dwID != dwReqCard )
			{
				pUser->AddDefinedText( TID_UPGRADE_ERROR_WRONGUPLEVEL , "" );			
				return;					
			}
		}
		else
		// �Ϲ������� ��, ����
	#if __VER >= 8 //__Y_NEW_ENCHANT
		if( pItemElem1->GetProp()->dwItemKind3 == IK3_ENCHANT )
	#else //__Y_NEW_ENCHANT
		if( pItemElem1->GetProp()->dwItemKind3 == IK3_DICE )
	#endif //__Y_NEW_ENCHANT
		{
#if __VER >= 9 // __ULTIMATE
			if( pItemElem0->GetProp()->dwReferStat1 == WEAPON_ULTIMATE )
			{
				pUser->AddDefinedText( TID_GAME_NOTEQUALITEM , "" );
				return;
			}
			
			if( pItemElem1->GetProp()->dwID != II_GEN_MAT_ORICHALCUM01
				&& pItemElem1->GetProp()->dwID != II_GEN_MAT_ORICHALCUM01_1 )
			{
				pUser->AddDefinedText( TID_GAME_NOTEQUALITEM , "" );
				return;
			}
#endif // __ULTIMATE
			pAbilityOption = pItemElem0->GetAbilityOptionPtr();

			if( pAbilityOption == NULL )
				return;

			if( !CItemElem::IsDiceRefineryAble(pItemElem0->GetProp()) )
			{
				pUser->AddDefinedText( TID_GAME_NOTEQUALITEM , "" );			
				return;								
			}

		#if __VER < 8 //__Y_NEW_ENCHANT
			if( pItemElem1->GetProp()->dwID != II_GEN_MAT_DIE_TWELVE )
			{
				if( *pAbilityOption <= 4 )
				{
					if( pItemElem1->GetProp()->dwID != II_GEN_MAT_DIE_FOUR )
					{
						pUser->AddDefinedText( TID_UPGRADE_ERROR_WRONGUPLEVEL , "" );			
						return;					
					}
				}
				else
				{
					if( pItemElem1->GetProp()->dwID != II_GEN_MAT_DIE_SIX )
					{
						pUser->AddDefinedText( TID_UPGRADE_ERROR_WRONGUPLEVEL , "" );			
						return;					
					}
				}
			}
		#endif //__Y_NEW_ENCHANT
		}	
		
		if( *pAbilityOption >= 10 )
		{
			pUser->AddDefinedText( TID_UPGRADE_MAXOVER , "" );			
			return;
		}
		// 1000������ ���� �ۼ�Ʈ 
		int nPercent = ::GetEnchantPercent( pItemElem1->GetProp()->dwID, *pAbilityOption );

		LogItemInfo aLogItem;
		aLogItem.SendName = pUser->GetName();
		aLogItem.RecvName = "UPGRADEITEM";
		aLogItem.WorldId = pUser->GetWorld()->GetID();
		aLogItem.Gold = pUser->GetGold();
		aLogItem.Gold2 = pUser->GetGold();


		BOOL bSmelprot	= FALSE;
		if( pUser->HasBuff( BUFF_ITEM, II_SYS_SYS_SCR_SMELPROT ) )
		{
			bSmelprot	= TRUE;
			pUser->RemoveBuff( BUFF_ITEM, II_SYS_SYS_SCR_SMELPROT );

			ItemProp* pItemProp = prj.GetItemProp( II_SYS_SYS_SCR_SMELPROT );
			if( pItemProp )
				g_dpDBClient.SendLogSMItemUse( "2", pUser, NULL, pItemProp );
		}
		if( pUser->HasBuff( BUFF_ITEM, II_SYS_SYS_SCR_SMELTING ) )
		{
			if( *pAbilityOption < 7 )
			{
				nPercent	+= 100;
				pUser->RemoveBuff( BUFF_ITEM, II_SYS_SYS_SCR_SMELTING );

				ItemProp* pItemProp = prj.GetItemProp( II_SYS_SYS_SCR_SMELTING );
				if( pItemProp )
					g_dpDBClient.SendLogSMItemUse( "2", pUser, NULL, pItemProp );
			}
		}
#ifdef __SM_ITEM_2ND_EX
		BOOL bSmelprot2	= FALSE;
		if( !bSmelprot && pUser->HasBuff( BUFF_ITEM, II_SYS_SYS_SCR_SMELPROT2 ) )
		{
			bSmelprot2	= TRUE;
			pUser->RemoveBuff( BUFF_ITEM, II_SYS_SYS_SCR_SMELPROT2 );
		}
#endif	// __SM_ITEM_2ND_EX

		if( pUser->HasBuff( BUFF_ITEM, II_SYS_SYS_SCR_SUPERSMELTING ) )
		{
			if( *pAbilityOption < 7 )
			{
				nPercent	+= 1000;
				pUser->RemoveBuff( BUFF_ITEM, II_SYS_SYS_SCR_SUPERSMELTING );
				
				ItemProp* pItemProp = prj.GetItemProp( II_SYS_SYS_SCR_SUPERSMELTING );
				if( pItemProp )
					g_dpDBClient.SendLogSMItemUse( "2", pUser, NULL, pItemProp );
			}				
		}

		// �ش� �������� �Ӽ�, �Ϲ� ������ ��� Ȯ���� ������.
		if( xRandom( 1000 ) > nPercent )
		{
			// ���� �޼��� ���
			pUser->AddDefinedText( TID_UPGRADE_FAIL, "" );
			pUser->AddPlaySound( SND_INF_UPGRADEFAIL );

			if((pUser->IsMode( TRANSPARENT_MODE ) ) == 0)
				g_UserMng.AddCreateSfxObj((CMover *)pUser, XI_INT_FAIL, pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z);

			// �����ϸ� nLevDown�̻��̸� ������ ����
			if( *pAbilityOption >= nLevDown )
			{
				if( !bSmelprot )
				{	// �����ϸ� ����.. ��� ������ ���� ���� �α�
				#ifdef __SM_ITEM_2ND_EX
					if( bSmelprot2  )
					{
						if( pItemElem1->GetProp()->dwItemKind3 == IK3_ELECARD )
						{
							pUser->UpdateItem( (BYTE)pItemElem0->m_dwObjId, UI_RAO, pItemElem0->m_nResistAbilityOption - 1 );
							aLogItem.Action = "8";
						}
						else 
					#if __VER >= 8 //__Y_NEW_ENCHANT
						if( pItemElem1->GetProp()->dwItemKind3 == IK3_ENCHANT )
					#else //__Y_NEW_ENCHANT
						if( pItemElem1->GetProp()->dwItemKind3 == IK3_DICE )
					#endif //__Y_NEW_ENCHANT
						{
							pUser->UpdateItem( (BYTE)pItemElem0->m_dwObjId, UI_AO,  pItemElem0->GetAbilityOption() - 1 );
							aLogItem.Action = "9";
						}
						OnLogItem( aLogItem, pItemElem0, pItemElem0->m_nItemNum );
					}
					else
				#endif	// __SM_ITEM_2ND_EX
					{
						aLogItem.Action = "L";
						OnLogItem( aLogItem, pItemElem0, pItemElem0->m_nItemNum );
						pUser->RemoveItem( dwItemId, (short)1 );
					}
				}
			}
			else
			{	// ����� �ϸ� ���� �α�
				if( pItemElem1->GetProp()->dwItemKind3 == IK3_ELECARD )
				{
					aLogItem.Action = "J";
					OnLogItem( aLogItem, pItemElem0, pItemElem0->m_nItemNum );
				}
				else 
			#if __VER >= 8 //__Y_NEW_ENCHANT
				if( pItemElem1->GetProp()->dwItemKind3 == IK3_ENCHANT )
			#else //__Y_NEW_ENCHANT
				if( pItemElem1->GetProp()->dwItemKind3 == IK3_DICE )
			#endif //__Y_NEW_ENCHANT
				{
					aLogItem.Action = "F";
					OnLogItem( aLogItem, pItemElem0, pItemElem0->m_nItemNum );
				}		
			}
		}
		else
		{
			// ����
			pUser->AddDefinedText( TID_UPGRADE_SUCCEEFUL, "" );
			pUser->AddPlaySound( SND_INF_UPGRADESUCCESS );
			
			if((pUser->IsMode( TRANSPARENT_MODE ) ) == 0)
				g_UserMng.AddCreateSfxObj((CMover *)pUser, XI_INT_SUCCESS, pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z);
			
			if( pItemElem1->GetProp()->dwItemKind3 == IK3_ELECARD )
			{
				pUser->UpdateItem( (BYTE)pItemElem0->m_dwObjId, UI_IR,  pItemElem1->GetProp()->eItemType );
				pUser->UpdateItem( (BYTE)pItemElem0->m_dwObjId, UI_RAO,  pItemElem0->m_nResistAbilityOption+1 );
				aLogItem.Action = "O";
				OnLogItem( aLogItem, pItemElem0, pItemElem0->m_nItemNum );
			}
			else 
		#if __VER >= 8 //__Y_NEW_ENCHANT
			if( pItemElem1->GetProp()->dwItemKind3 == IK3_ENCHANT ) // // �ֻ���
		#else //__Y_NEW_ENCHANT
			if( pItemElem1->GetProp()->dwItemKind3 == IK3_DICE ) // // �ֻ���
		#endif //__Y_NEW_ENCHANT
			{
				pUser->UpdateItem( (BYTE)pItemElem0->m_dwObjId, UI_AO,  pItemElem0->GetAbilityOption()+1 );
				aLogItem.Action = "H";
				OnLogItem( aLogItem, pItemElem0, pItemElem0->m_nItemNum );
			}
		}

		aLogItem.Action = "N";
		OnLogItem( aLogItem, pItemElem1, pItemElem1->m_nItemNum );
		// ���þ��� ���� - �����̴�, ���д�...
		pUser->RemoveItem( dwItemMaterialId, (short)1 );
#endif // __EXT_ENCHANT
	}	
}

#if __VER >= 10 // __REMOVE_ATTRIBUTE
void CDPSrvr::OnRemoveAttribute( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize)
{
	int nPayPenya = 100000; //�Ӽ����� ���Ž� �ʿ��� ���

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( !IsValidObj( pUser ) )
		return;

	OBJID objItemId;
	ar >> objItemId;

	CItemElem* pItemElem = pUser->m_Inventory.GetAtId( objItemId );

	if( IsUsableItem( pItemElem ) == FALSE )
	{
		pUser->AddRemoveAttribute( FALSE );
		return;
	}
	// ���⳪ ���� ����
	if( !CItemElem::IsEleRefineryAble(pItemElem->GetProp()) )
	{
		pUser->AddRemoveAttribute( FALSE );
		pUser->AddDefinedText( TID_GAME_NOTEQUALITEM , "" );
		return;
	}
	if( pItemElem->m_nResistSMItemId != 0 ) // ���ȭ ������ �������̸� �Ұ���
	{
		pUser->AddRemoveAttribute( FALSE );
		pUser->AddDefinedText( TID_GAME_NOTUPGRADE , "" );
		return;
	}
	// �����Ǿ� �ִ��� Ȯ���� �ؾ� ��.
	if( pUser->m_Inventory.IsEquip( objItemId ) )
	{
		pUser->AddRemoveAttribute( FALSE );
		pUser->AddDefinedText( TID_GAME_EQUIPPUT , "" );
		return;
	}
	// 10�� ��� �̻��� �����ؾ߸� �Ӽ� ���� ����.
	if( pUser->GetGold() < nPayPenya )
	{
		pUser->AddRemoveAttribute( FALSE );
		pUser->AddDefinedText( TID_GAME_LACKMONEY , "" );
		return;
	}
	
	// ���� ���⿡ �Ӽ��� ����Ǿ� �־�� ����.
	if( (pItemElem->m_bItemResist != SAI79::NO_PROP) && (pItemElem->m_nResistAbilityOption > 0) )
	{
		pUser->AddGold( -nPayPenya );
		pUser->AddPlaySound( SND_INF_UPGRADESUCCESS );
		if((pUser->IsMode( TRANSPARENT_MODE ) ) == 0)
			g_UserMng.AddCreateSfxObj((CMover *)pUser, XI_INT_SUCCESS, pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z);
	
		pUser->UpdateItem( (BYTE)pItemElem->m_dwObjId, UI_IR,  SAI79::NO_PROP );
		pUser->UpdateItem( (BYTE)pItemElem->m_dwObjId, UI_RAO,  0 );
		pUser->AddRemoveAttribute( TRUE );
		
		// �Ӽ����� ���� ���� �α�
		LogItemInfo aLogItem;
		aLogItem.SendName = pUser->GetName();
		aLogItem.RecvName = "REMOVE_ATTRIBUTE";
		aLogItem.WorldId = pUser->GetWorld()->GetID();
		aLogItem.Gold = pUser->GetGold() + nPayPenya;
		aLogItem.Gold2 = pUser->GetGold();
		aLogItem.Gold_1 = -nPayPenya;
		aLogItem.Action = "O";
		OnLogItem( aLogItem, pItemElem, 1 );
	}
	else
		pUser->AddRemoveAttribute( FALSE );
	
}
#endif // __REMOVE_ATTRIBUTE

#if __VER >= 13 // __EXT_ENCHANT
void CDPSrvr::OnChangeAttribute( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpbuf, u_long uBufSize )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( !IsValidObj( pUser ) )
		return;

	OBJID objTargetItem, objMaterialItem;
	int nAttribute;

	ar >> objTargetItem >> objMaterialItem;
	ar >> nAttribute;

	CItemUpgrade::GetInstance()->ChangeAttribute( pUser, objTargetItem, objMaterialItem, static_cast<SAI79::ePropType>(nAttribute) );
}
#endif // __EXT_ENCHANT

void CDPSrvr::OnRandomScroll( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpbuf, u_long uBufSize )
{
	DWORD dwId1, dwId2;
	
	ar >> dwId1;
	ar >> dwId2;
	
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		// �κ��丮�� �ִ��� �����Ǿ� �ִ��� Ȯ���� �ؾ� ��
		CItemElem* pItemElem0	= pUser->m_Inventory.GetAtId( dwId1 );
		CItemElem* pItemElem1	= pUser->m_Inventory.GetAtId( dwId2 );
		if( IsUsableItem( pItemElem0 ) == FALSE || IsUsableItem( pItemElem1 ) == FALSE )
		{
			return;
		}

		if( pUser->m_Inventory.IsEquip( dwId1 ) )
		{
			pUser->AddDefinedText( TID_GAME_EQUIPPUT , "" );
			return;
		}			

		if( pItemElem1->GetProp()->dwItemKind3 != IK3_RANDOM_SCROLL )
		{
			return;					
		}

		if( !(pItemElem0->GetProp()->dwItemKind1 == IK1_WEAPON || pItemElem0->GetProp()->dwItemKind2 == IK2_ARMOR || pItemElem0->GetProp()->dwItemKind2 == IK2_ARMORETC ) )
		{
			pUser->AddDefinedText(  TID_GAME_RANDOMSCROLL_ERROR, "" );
			return;
		}

		int nSTR[4] = { 1,  9, 21, 37 };
		int nDEX[4] = { 2, 10, 22, 38 };
		int nINT[4] = { 3, 11, 23, 39 };
		int nSTA[4] = { 4, 12, 24, 40 };

		int nValue = 0;
		int nRandom = xRandom( 100 );
		if( nRandom < 64 )
			nValue = 1;
		else if( nRandom < 94 )
			nValue = 2;
		else if( nRandom < 99 )
			nValue = 3;
		else if( nRandom < 100 )
			nValue = 4;

		if( 0 < nValue )
		{
			int nKind = 0;
			int nToolKind = 0;
			if( pItemElem1->GetProp()->dwID == II_SYS_SYS_SCR_RANDOMSTR )
			{
				nKind = nSTR[nValue-1];
				nToolKind = DST_STR;
			}
			else if( pItemElem1->GetProp()->dwID == II_SYS_SYS_SCR_RANDOMDEX )
			{
				nKind = nDEX[nValue-1];
				nToolKind = DST_DEX;
			}
			else if( pItemElem1->GetProp()->dwID == II_SYS_SYS_SCR_RANDOMINT )
			{
				nKind = nINT[nValue-1];
				nToolKind = DST_INT;
			}
			else if( pItemElem1->GetProp()->dwID == II_SYS_SYS_SCR_RANDOMSTA )
			{
				nKind = nSTA[nValue-1];
				nToolKind = DST_STA;
			}

			pUser->UpdateItem( (BYTE)pItemElem0->m_dwObjId, UI_RANDOMOPTITEMID, nKind );

			LogItemInfo aLogItem;
			aLogItem.SendName = pUser->GetName();
			aLogItem.RecvName = "RANDOMSCROLL";
			aLogItem.WorldId = pUser->GetWorld()->GetID();
			aLogItem.Gold = aLogItem.Gold2 = pUser->GetGold();
			aLogItem.Action = "(";
			OnLogItem( aLogItem, pItemElem0, pItemElem0->m_nItemNum );
			aLogItem.Action = ")";
			OnLogItem( aLogItem, pItemElem1, pItemElem1->m_nItemNum );

			pUser->RemoveItem( (BYTE)( dwId2 ), (short)1 );		

			// ������ �ڱ� ����~
			pUser->AddPlaySound( SND_INF_UPGRADESUCCESS );			
			g_UserMng.AddCreateSfxObj((CMover *)pUser, XI_INT_SUCCESS, pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z);			

			DWORD dwStringNum = 0;
			switch( nToolKind )
			{
			case DST_STR:
				dwStringNum = TID_TOOLTIP_STR;
				break;
			case DST_DEX:
				dwStringNum = TID_TOOLTIP_DEX;
				break;
			case DST_STA:
				dwStringNum = TID_TOOLTIP_STA;
				break;
			default: //case DST_INT:
				dwStringNum = TID_TOOLTIP_INT;
				break;
			}
			CString strMessage;
			strMessage.Format( prj.GetText( TID_GAME_RANDOMSCROLL_SUCCESS ), pItemElem0->GetProp()->szName, prj.GetText( dwStringNum ), nValue );
			pUser->AddText( strMessage );
		}
	}
}
	
void CDPSrvr::OnUpgradeBase( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize)
{
}


void CDPSrvr::OnCommercialElem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	DWORD dwItemId0, dwItemId1;
	ar >> dwItemId0 >> dwItemId1;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		// �κ��丮�� �ִ��� �����Ǿ� �ִ��� Ȯ���� �ؾ� ��
		// �κ��丮�� �ִ��� �˻�
		CItemElem* pItemElem0	= pUser->m_Inventory.GetAtId( dwItemId0 );
		CItemElem* pItemElem1	= pUser->m_Inventory.GetAtId( dwItemId1 );

		if( IsUsableItem( pItemElem0 ) == FALSE || 
			IsUsableItem( pItemElem1 ) == FALSE || 
			pItemElem0->GetProp()->nLog >=2 || 
			pItemElem1->GetProp()->nLog >=2 )
		{
			return;
		}

		if( pUser->m_Inventory.IsEquip( dwItemId0 ) || pUser->m_Inventory.IsEquip( dwItemId1 ) )
		{
			pUser->AddDefinedText( TID_GAME_EQUIPPUT , "" );
			return;
		}

		// ��(��Ʈ��), ������� �ƴϸ� ���úҰ���
		if( !( ( pItemElem0->GetProp()->dwItemKind2 == IK2_WEAPON_MAGIC ||
			pItemElem0->GetProp()->dwItemKind2 == IK2_WEAPON_DIRECT ) ||
			( ( pItemElem0->GetProp()->dwItemKind2 == IK2_ARMOR || pItemElem0->GetProp()->dwItemKind2 == IK2_ARMORETC ) 
			&& pItemElem0->GetProp()->dwItemKind3 == IK3_SUIT )
			) )		
		{
			return;			
		}

		if( pItemElem0->m_nResistSMItemId != 0 ) // �̹������� �������̸� �Ұ���
		{
			return;	
		}

		BOOL bResistDelete = FALSE;

		if( pItemElem1->GetProp()->dwItemKind2 == IK2_SYSTEM )
		{
			// �Ӽ� ���ݷ� �߰�
			if( pItemElem1->m_dwItemId == II_CHR_SYS_SCR_FIREASTONE ||
				pItemElem1->m_dwItemId == II_CHR_SYS_SCR_WATEILSTONE ||
				pItemElem1->m_dwItemId == II_CHR_SYS_SCR_WINDYOSTONE ||
				pItemElem1->m_dwItemId == II_CHR_SYS_SCR_LIGHTINESTONE ||
				pItemElem1->m_dwItemId == II_CHR_SYS_SCR_EARTHYSTONE ) 
				
			{
				if( pItemElem0->GetProp()->dwItemKind2 == IK2_ARMOR ||
					pItemElem0->GetProp()->dwItemKind2 == IK2_ARMORETC )
				{
					return;	
				}
			}
			else // �Ӽ� ���� �߰�
			if(	pItemElem1->m_dwItemId == II_CHR_SYS_SCR_DEFIREASTONE ||
				pItemElem1->m_dwItemId == II_CHR_SYS_SCR_DEWATEILSTONE ||
				pItemElem1->m_dwItemId == II_CHR_SYS_SCR_DEWINDYOSTONE ||
				pItemElem1->m_dwItemId == II_CHR_SYS_SCR_DELIGHTINESTONE ||
				pItemElem1->m_dwItemId == II_CHR_SYS_SCR_DEEARTHYSTONE )
			{
				if( pItemElem0->GetProp()->dwItemKind2 == IK2_WEAPON_MAGIC ||
					pItemElem0->GetProp()->dwItemKind2 == IK2_WEAPON_DIRECT )
				{
					return;	
				}
			}
			else // �Ӽ� ����
			if( pItemElem1->m_dwItemId == II_CHR_SYS_SCR_TINEINEDSTONE )
			{
				if( pItemElem0->m_bItemResist == SAI79::NO_PROP )
				{
					return;	
				}
				pItemElem0->m_bItemResist = SAI79::NO_PROP;
				pItemElem0->m_nResistAbilityOption = 0;
				bResistDelete = TRUE;
			}
			else
			{
				return;	
			}
		}
		else 
		{
			return;	
		}

		// ����
		
		if( bResistDelete )
		{
			pUser->AddCommercialElem( pItemElem0->m_dwObjId, 9999 );
		}
		else
		{
			pItemElem0->m_nResistSMItemId = pItemElem1->m_dwItemId;
		}
		
		// �α�
		g_dpDBClient.SendLogSMItemUse( "5", pUser, pItemElem1, pItemElem1->GetProp() );
		LogItemInfo aLogItem;
		aLogItem.Action = "5";
		aLogItem.SendName = pUser->GetName();
		aLogItem.RecvName = pUser->GetName();
		aLogItem.WorldId = pUser->GetWorld()->GetID();
		aLogItem.Gold = pUser->GetGold();
		aLogItem.Gold2 = pUser->GetGold();
		aLogItem.ItemNo = pItemElem0->GetSerialNumber();
		//aLogItem.ItemName = pItemElem0->GetProp()->szName;
		_stprintf( aLogItem.szItemName, "%d", pItemElem0->GetProp()->dwID );
		aLogItem.itemNumber = pItemElem0->m_nItemNum;
		aLogItem.nAbilityOption = pItemElem0->GetAbilityOption();
		aLogItem.nItemResist = pItemElem0->m_bItemResist;
		aLogItem.nResistAbilityOption = pItemElem0->m_nResistAbilityOption;
		aLogItem.Negudo = int((float)pItemElem0->m_nHitPoint * 100 / (float)pItemElem0->GetProp()->dwEndurance );
		OnLogItem( aLogItem );

		pUser->RemoveItem( (BYTE)dwItemId1, (short)1 );
		
		g_UserMng.AddCreateSfxObj((CMover *)pUser, XI_SYS_EXPAN01, pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z);
		pUser->AddCommercialElem( pItemElem0->m_dwObjId, pItemElem0->m_nResistSMItemId );
		
	}
}

void CDPSrvr::OnRequestGuildRank( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CTime		tm			= CTime::GetCurrentTime();
	CTimeSpan	tm_update	= tm - CGuildRank::Instance()->m_UpdateTime;

	DWORD	ver;
	
	//	���� ������ �޴´�.
	ar >> ver;

	// ���ŵ��� �Ϸ簡 �������� üũ
	if ( tm_update.GetHours() >= 24 )
	{
		// TRANS �������� �ٽ� ��ũ ������ ������ ���� ��û�Ѵ�.
		g_dpDBClient.UpdateGuildRanking();
	}
	else
	{
		// ��ũ ���� ������ �ٸ� ��쿣 ��ũ ������ ������ �ȴ�.
		if ( CGuildRank::Instance()->m_Version != ver )
		{
			// ��ŷ ������ ������.
			CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
			if( IsValidObj( pUser ) )
			{
				pUser->SendGuildRank();
			}
		}
	}
}

void CDPSrvr::OnBuyingInfo( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	return;
}

void CDPSrvr::OnEnterChattingRoom( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	u_long uidChattingRoom;
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		BOOL bChatting = FALSE;
		if( pUser->m_idChatting == 0 )
		{
			ar >> uidChattingRoom;

			CChatting* pChatting = g_ChattingMng.GetChttingRoom( uidChattingRoom );
			if( pChatting )
			{
				if( pChatting->AddChattingMember( pUser->m_idPlayer ) )
				{
					CUser*			pUsertmp;
					pUser->m_idChatting = uidChattingRoom;

					for( int i = 0 ; i < pChatting->GetChattingMember() - 1 ; ++i )
					{
						pUsertmp	= (CUser*)prj.GetUserByID( pChatting->m_idMember[i] );
						if( IsValidObj( pUsertmp ) )
							pUsertmp->AddEnterChatting( pUser );
					}

					pUser->AddNewChatting( pChatting );
				}
				else
				{
					bChatting	= TRUE;
				}
			}

		}
		else
		{
			bChatting	= TRUE;
		}
	}
}

void CDPSrvr::OnChatting( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	static	TCHAR	sChat[1024];
	ar.ReadString( sChat, 1024 );

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
#ifdef __JEFF_9_20
#if __VER >= 12 // __LORD
		int nText	= pUser->GetMuteText();
		if(  nText )
		{
			pUser->AddDefinedText( nText );
			return;
		}
#else	// __LORD
		if( pUser->IsMute() )
			return;
#endif	// __LORD
#endif	// __JEFF_9_20

		if( !( pUser->HasBuff( BUFF_ITEM, II_SYS_SYS_SCR_FONTEDIT)))
			ParsingEffect( sChat, strlen(sChat) );

		RemoveCRLF( sChat );

		CChatting* pChatting = g_ChattingMng.GetChttingRoom( pUser->m_idChatting );
		if( pChatting )
		{
			int nFind = pChatting->FindChattingMember( pUser->m_idPlayer );
			if( nFind != -1 )
			{
				CUser* pUsertmp;
				for( int i = 0 ; i < pChatting->GetChattingMember() ; ++i )
				{
					pUsertmp	= (CUser*)prj.GetUserByID( pChatting->m_idMember[i] );
					if( IsValidObj( pUsertmp ) )
						pUsertmp->AddChatting( pUser->m_idPlayer, sChat );
				}
			}
			else
			{
				// ä�ø���� �ƴ�
				pUser->m_idChatting = 0;
				pUser->AddDeleteChatting();
			}
		}
		else
		{
			// ä�ù��� ����.
			pUser->m_idChatting = 0;
			pUser->AddDeleteChatting();
		}

	}
}


void CDPSrvr::OnOpenChattingRoom( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{

		CChatting* pChatting	= g_ChattingMng.GetChttingRoom( pUser->m_idChatting );
		if( pChatting )
		{
			pChatting->m_bState		= TRUE;
			CUser* pUsertmp;
			for( int i = 0 ; i < pChatting->GetChattingMember() ; ++i )
			{
				pUsertmp	= (CUser*)prj.GetUserByID( pChatting->m_idMember[i] );
				if( IsValidObj( pUsertmp ) && pUser->m_idPlayer != pUsertmp->m_idPlayer )
					pUsertmp->AddChttingRoomState( pChatting->m_bState );
			}
		}
	}
}

void CDPSrvr::OnCloseChattingRoom( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{

		CChatting* pChatting = g_ChattingMng.GetChttingRoom( pUser->m_idChatting );
		if( pChatting )
		{
			pChatting->m_bState = FALSE;
			CUser*			pUsertmp;
			
			for( int i = 0 ; i < pChatting->GetChattingMember() ; ++i )
			{
				pUsertmp	= (CUser*)prj.GetUserByID( pChatting->m_idMember[i] );
				if( IsValidObj( pUsertmp ) && pUser->m_idPlayer != pUsertmp->m_idPlayer )
				{
					pUsertmp->AddChttingRoomState( pChatting->m_bState );
				}						
			}
		}

	}
}

void CDPSrvr::OnCommonPlace( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
//		BYTE nType;
//		ar >> nType;
}


void CDPSrvr::QueryDestroyPlayer( DPID dpidCache, DPID dpidSocket, DWORD dwSerial, u_long idPlayer )
{
	BEFORESENDSOLE( ar, PACKETTYPE_QUERY_DESTROY_PLAYER, dpidSocket );
	ar << dwSerial;
	ar << idPlayer;
	SEND( ar, this, dpidCache );
}

void CDPSrvr::OnSetNaviPoint( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	NaviPoint nv;
	OBJID objidTarget;
	ar >> nv.Pos >> objidTarget;

	CUser* pUser	=	g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		if( objidTarget == NULL_ID )
		{
			CParty* pParty	= g_PartyMng.GetParty( pUser->m_idparty );
			if( pParty )
			{
				for( int i = 0 ; i < pParty->GetSizeofMember() ; ++i )
				{
					CUser* pUsertmp = (CUser *)prj.GetUserByID( pParty->GetPlayerId( i ) );
					if( IsValidObj( pUsertmp ) )
					{
						pUsertmp->AddSetNaviPoint( nv, pUser->GetId(), pUser->GetName( TRUE ) );
					}
				}
			}
		}
		else
		{
			CUser* pUsertmp	= prj.GetUser( objidTarget );
			if( IsValidObj( pUsertmp ) )
			{
				pUser->AddSetNaviPoint( nv, pUser->GetId(), pUser->GetName( TRUE ) );
				pUsertmp->AddSetNaviPoint( nv, pUser->GetId(), pUser->GetName( TRUE ) );
			}
		}
	}
}

void CDPSrvr::OnGameMasterWhisper( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CHAR	sPlayerFrom[MAX_PLAYER], lpString[260], szChat[1024];
	ar.ReadString( sPlayerFrom, MAX_PLAYER );
	ar.ReadString( lpString, 260 );

	CUser* pUser	=	g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{	
		sprintf( szChat, "%s -> %s", sPlayerFrom, lpString );
		g_dpDBClient.SendLogGamemaChat( pUser, szChat );
	}
}
/*
#ifdef __S0114_RELOADPRO
void CDPSrvr::SendReloadProject( DPID dpidCache, DPID dpidUser )
{
//	BEFORESENDSOLE( ar1, PACKETTYPE_RELOAD_PROJECT, dpidUser );
//	SEND( ar1, &g_DPSrvr, dpidCache );
}
#endif // __S0114_RELOADPRO
*/
// ����� �ɱ� ��Ŷ 
void CDPSrvr::OnNWWantedGold( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	int		nGold;
	char	szMsg[WANTED_MSG_MAX + 1];

	ar >> nGold;
	ar.ReadString( szMsg, WANTED_MSG_MAX + 1 );

	CUser* pUser	=	g_UserMng.GetUser( dpidCache, dpidUser );
	if( !IsValidObj( pUser ) )
		return;

	if( pUser->m_idMurderer == 0 )		// ���� �����ڰ� �������� ������� �� �� ����.
		return;

	if( szMsg[0] == '\0' )
		return;
	
	if( strlen(szMsg) > WANTED_MSG_MAX )
		return;

	if( nGold < MIN_INPUT_REWARD || nGold > MAX_INPUT_REWARD )			// ������� �ּ� 1000�гĿ��� �ִ� 2�� �гı��� �� �� �ִ�. 
		return;

	int nTax = MulDiv( nGold, 10, 100 );					// �� ������� 10%�� ������� ���޵ȴ�. 

	if( pUser->GetGold() >= (nGold + nTax) ) 
		pUser->AddGold( -(nGold + nTax) );
	else
		return;

#if __VER >= 11 // __SYS_PLAYER_DATA
	const char* lpszPlayer	= CPlayerDataCenter::GetInstance()->GetPlayerString( pUser->m_idMurderer );
#else	// __SYS_PLAYER_DATA
	LPCSTR lpszPlayer = prj.GetPlayerString( pUser->m_idMurderer );
#endif	// __SYS_PLAYER_DATA
	if( lpszPlayer == NULL )
		lpszPlayer = "";
	g_DPCoreClient.SendWCWantedGold( lpszPlayer, pUser->m_idMurderer, nGold, szMsg );
}


// ����� ����Ʈ ��û ��Ŷ
void CDPSrvr::OnNWWantedList( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	=	g_UserMng.GetUser( dpidCache, dpidUser );
	if( !IsValidObj( pUser ) )
		return;

	{
		BEFORESENDSOLE( out, PACKETTYPE_WN_WANTED_LIST, pUser->m_Snapshot.dpidUser );

		CWantedListSnapshot& wantedListSnapshot = CWantedListSnapshot::GetInstance();
		wantedListSnapshot.Write( out );

		SEND( out, this, dpidCache );
	}
}

void CDPSrvr::OnNWWantedName( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	=	g_UserMng.GetUser( dpidCache, dpidUser );
	if( !IsValidObj( pUser ) )
		return;

	LPCSTR lpszPlayer = "";
	if( pUser->m_idMurderer )		
	{
#if __VER >= 11 // __SYS_PLAYER_DATA
		lpszPlayer	= CPlayerDataCenter::GetInstance()->GetPlayerString( pUser->m_idMurderer );
#else	// __SYS_PLAYER_DATA
		lpszPlayer = prj.GetPlayerString( pUser->m_idMurderer );
#endif	// __SYS_PLAYER_DATA
		if( lpszPlayer == NULL )
			lpszPlayer	= "";
	}
	
	{
		BEFORESENDSOLE( ar, PACKETTYPE_WN_WANTED_NAME, pUser->m_Snapshot.dpidUser );
		ar.WriteString( lpszPlayer );
		SEND( ar, this, dpidCache );
	}
}


// ����� �ڼ������� ��û ��Ŷ 
void CDPSrvr::OnNWWantedInfo( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	char szPlayer[64];
	ar.ReadString( szPlayer, 64 );

	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	if( !IsValidObj( pUser ) )
		return;

	CWantedListSnapshot& wantedListSnapshot = CWantedListSnapshot::GetInstance();
	int nIndex = wantedListSnapshot.GetPlayerIndex( szPlayer );
	if( nIndex < 0 )
		return;

	{
		int nGold = REQ_WANTED_GOLD;
		if( pUser->GetGold() >= nGold ) 
		{
			D3DXVECTOR3 vPos( 0.0f, 0.0f, 0.0f );		// ������� ��ġ 
			BYTE		byOnline = 0;					// 1 �̸� online
			DWORD		dwWorldID = 0;
			LPCTSTR		lpszWorld = "";

#if __VER >= 11 // __SYS_PLAYER_DATA
			u_long idPlayer		= CPlayerDataCenter::GetInstance()->GetPlayerId( szPlayer );
#else	// __SYS_PLAYER_DATA
			u_long idPlayer = prj.GetPlayerID( szPlayer );
#endif	// __SYS_PLAYER_DATA
			CUser* pTarget	= g_UserMng.GetUserByPlayerID( idPlayer );	
			if( IsValidObj(pTarget) && pTarget->GetWorld() )
			{
				vPos      = pTarget->GetPos();
				byOnline  = 1;
				dwWorldID = pTarget->GetWorld()->GetID();
				lpszWorld = pTarget->GetWorld()->m_szWorldName;

				pUser->AddGold( -nGold );
			}

			pUser->AddWantedInfo( vPos, byOnline, dwWorldID, lpszWorld );
		} 
		else
		{
			pUser->AddDefinedText( TID_GAME_LACKMONEY, "" );	// �κ��� ���̺���
		}
	}
}

void CDPSrvr::OnReqLeave( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	if( !IsValidObj( pUser ) )
		return;

	if( pUser->m_dwLeavePenatyTime == 0 )	// ���Ƽ Ÿ���� ������ ���� ���°�?
		pUser->m_dwLeavePenatyTime = ::timeGetTime() + TIMEWAIT_CLOSE * 1000;	//  ���� 
}

void CDPSrvr::OnStateMode( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	DWORD dwStateMode;
	BYTE nFlag;
	ar >> dwStateMode;
	ar >> nFlag;


	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj(pUser) )
	{	
		if( pUser->IsStateMode( dwStateMode ) )
		{
			if( nFlag == STATEMODE_BASEMOTION_CANCEL )
			{
				pUser->SetStateNotMode( STATE_BASEMOTION_MODE, STATEMODE_BASEMOTION_CANCEL );
				pUser->m_nReadyTime = 0;
				pUser->m_dwUseItemId = 0;
			}
		}
	}
}

// ��Ȱ���
void CDPSrvr::OnResurrectionCancel( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj(pUser) && pUser->m_Resurrection_Data.bUseing )
	{	
		pUser->m_Resurrection_Data.bUseing = FALSE;
	}
}

// ����ڰ� OK�ϸ� ��Ȱ�� ���� �Ѵ�
void CDPSrvr::OnResurrectionOK( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );

	// �������� ��Ȱ����ΰ�??
	if( IsValidObj(pUser) && pUser->m_Resurrection_Data.bUseing )
	{	
		if( pUser->IsDie() == FALSE )
		{
			pUser->m_Resurrection_Data.bUseing = FALSE;
			return;
		}
		// �ణ�� �������� ä������
		if( pUser->GetType() == OT_MOVER )	// Ÿ���� �����϶���.
		{		
			RESURRECTION_DATA* pData = &(((CMover *)pUser)->m_Resurrection_Data);
			CUser* pSrc	= (CUser*)prj.GetUserByID( pData->dwPlayerID );

			if( IsValidObj(pSrc) )
			{
				// ��Ȱ SFXȿ��
				g_UserMng.AddCreateSfxObj( pUser, XI_SKILL_ASS_HEAL_RESURRECTION01 );
				
				// ��Ȱ�ϱ�
				g_UserMng.AddHdr( pUser, SNAPSHOTTYPE_RESURRECTION );
				pUser->m_pActMover->SendActMsg( OBJMSG_RESURRECTION );
				pUser->m_Resurrection_Data.bUseing = FALSE;
				
				pUser->ApplyParam( (CMover *)pSrc, pData->pSkillProp, pData->pAddSkillProp, TRUE , 0 );
				
				if( pData->pAddSkillProp->dwDestParam2 == DST_RECOVERY_EXP )
					pUser->SubDieDecExp(TRUE, pData->pAddSkillProp->nAdjParamVal2 );	// ��Ȱ�� �Ǹ鼭 ��ġ�� ���� ����.			
			}	
			else
				pUser->m_Resurrection_Data.bUseing = FALSE;

		}
	}
}

void CDPSrvr::OnChangeMode( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	int nFlag;
	DWORD dwMode;
	ar >> dwMode;
	ar >> nFlag;
	
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj(pUser) )
	{	
		switch( nFlag )
		{
		case 0:	// �ڵ� PK ON
			pUser->SetMode( FREEPK_MODE );
			break;
		case 1:	// �ڵ� PK OFF
			pUser->SetNotMode( FREEPK_MODE );
			break;
		case 2:	// �ڵ� PVP ON
			pUser->SetMode( PVPCONFIRM_MODE );
			break;
		case 3:	// �ڵ� PVP OFF
			pUser->SetNotMode( PVPCONFIRM_MODE );
			break;
		}
		g_UserMng.AddModifyMode( pUser );
	}
}

void CDPSrvr::OnQuerySetPlayerName( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
// II_SYS_SYS_SCR_CHANAM
	DWORD dwData;
	char lpszPlayer[MAX_PLAYER]	= { 0, };

	ar >> dwData;
	ar.ReadString( lpszPlayer, MAX_PLAYER );

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
#ifdef __QUIZ
		if( pUser->GetWorld() && pUser->GetWorld()->GetID() == WI_WORLD_QUIZ )
			return;
#endif // __QUIZ
#ifdef __RULE_0615
		if( prj.IsInvalidName( lpszPlayer ) || prj.IsAllowedLetter( lpszPlayer ) == FALSE )
			pUser->AddDiagText( prj.GetText( TID_DIAG_0020 ) ); 
		prj.Formalize( lpszPlayer );
#endif	// __RULE_0615

#ifdef __S_SERVER_UNIFY
		if( pUser->m_bAllAction )
#endif // __S_SERVER_UNIFY
		{
			WORD wId	= LOWORD( dwData );
			WORD wMode	= HIWORD( dwData );
			if( (short)wId >= 0 )
			{
				CItemElem* pItemElem = (CItemElem*)pUser->GetItemId( wId );
				if( IsUsableItem( pItemElem ) && pItemElem->m_dwItemId == II_SYS_SYS_SCR_CHANAM && pItemElem->m_bQuery == FALSE )
				{
					pItemElem->m_bQuery	= TRUE;
					g_dpDBClient.SendQuerySetPlayerName( pUser->m_idPlayer, lpszPlayer, dwData );
				}
			}
		}
#ifdef __S_SERVER_UNIFY
		else
		{
			g_dpDBClient.SendQuerySetPlayerName( pUser->m_idPlayer, lpszPlayer, dwData );
		}
#endif // __S_SERVER_UNIFY
	}
}

void CDPSrvr::OnQuerySetGuildName( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	BYTE nId;
	char lpszGuild[MAX_G_NAME]	= { 0, };

	ar >> nId;
	ar.ReadString( lpszGuild, MAX_G_NAME );

	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
#ifdef __RULE_0615
		if( prj.IsInvalidName( lpszGuild ) || prj.IsAllowedLetter( lpszGuild ) == FALSE )
			pUser->AddDiagText( prj.GetText( TID_DIAG_0020 ) ); 
#endif	// __RULE_0615

#ifdef __S_SERVER_UNIFY
		if( pUser->m_bAllAction )
#endif // __S_SERVER_UNIFY
		{
			CItemElem* pItemElem = (CItemElem*)pUser->GetItemId( nId );
			if( IsUsableItem( pItemElem ) && pItemElem->m_bQuery == FALSE )
			{
				pItemElem->m_bQuery	= TRUE;
				g_DPCoreClient.SendQuerySetGuildName( pUser->m_idPlayer, pUser->m_idGuild, lpszGuild, nId );
			}
		}
#ifdef __S_SERVER_UNIFY
		else
		{
			g_DPCoreClient.SendQuerySetGuildName( pUser->m_idPlayer, pUser->m_idGuild, lpszGuild, nId );
		}
#endif // __S_SERVER_UNIFY
	}
}

void CDPSrvr::PutCreateItemLog( CUser* pUser, CItemElem* pItemElem, const char* szAction, const char* recv )
{
	LogItemInfo logitem;
	logitem.Action		= szAction;
	logitem.SendName = pUser->GetName();
	logitem.RecvName = recv;
	logitem.WorldId = pUser->GetWorld()->GetID();
	logitem.Gold = pUser->GetGold();
	logitem.Gold2 = 0;
	logitem.ItemNo = pItemElem->GetSerialNumber();
	//logitem.ItemName = pItemElem->GetProp()->szName;
	_stprintf( logitem.szItemName, "%d", pItemElem->GetProp()->dwID );
	logitem.itemNumber = pItemElem->m_nItemNum;
	logitem.nAbilityOption = 0;
	logitem.Gold_1 = 0;
	logitem.nItemResist = pItemElem->m_bItemResist;
	logitem.nResistAbilityOption = pItemElem->m_nResistAbilityOption;
	logitem.Negudo		= 100;
	OnLogItem( logitem );
}

// Ŭ��κ��� Ż���û�� ����
void CDPSrvr::OnDoEscape( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		ItemProp* pItemProp = prj.GetItemProp( II_CHR_SYS_SCR_ESCAPEBLINKWING );
		if( pItemProp )
		{
			if( !pUser->IsSMMode( SM_ESCAPE ) )
				pUser->DoUseItemVirtual( II_CHR_SYS_SCR_ESCAPEBLINKWING, FALSE );
			else
				pUser->AddDefinedText( TID_GAME_STILLNOTUSE );		
		}
	}
}

void CDPSrvr::OnCheering( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	OBJID objid;
	ar >> objid;

	CUser* pUser	=	g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		if( pUser->GetId() == objid )
			return;

		CMover* pTarget		= prj.GetMover( objid );
		if( IsValidObj( pTarget ) && pTarget->GetType() == OT_MOVER && pTarget->IsPlayer() )
		{
			DWORD dwTickCount	= GetTickCount();
			if( pUser->m_nCheerPoint <= 0 )
			{
				pUser->AddDefinedText( TID_CHEER_NO1, "%d", (pUser->m_dwTickCheer - dwTickCount) / 60000 );
				return;
			}

			if( pUser->m_nCheerPoint == MAX_CHEERPOINT )
				pUser->SetCheerParam( pUser->m_nCheerPoint - 1, dwTickCount, TICK_CHEERPOINT );
			else
				pUser->SetCheerParam( pUser->m_nCheerPoint - 1, dwTickCount, pUser->m_dwTickCheer - dwTickCount );

			FLOAT fAngle = GetDegree(pTarget->GetPos(), pUser->GetPos());
			pUser->SetAngle(fAngle);

			if( pTarget->GetSex() == pUser->GetSex() )
			{
				((CUser*)pTarget)->AddDefinedText( TID_CHEER_MESSAGE3, "%s", pUser->GetName() );
				pUser->SendActMsg( OBJMSG_MOTION, MTI_CHEERSAME, ANILOOP_1PLAY );
			}
			else
			{
				((CUser*)pTarget)->AddDefinedText( TID_CHEER_MESSAGE4, "%s", pUser->GetName() );
				pUser->SendActMsg( OBJMSG_MOTION, MTI_CHEEROTHER, ANILOOP_1PLAY );
			}

			g_UserMng.AddCreateSfxObj((CMover *)pUser, XI_CHEERSENDEFFECT );
			g_UserMng.AddCreateSfxObj(pTarget, XI_CHEERRECEIVEEFFECT );


			CMover* pSrc = (CMover*)pUser ;
			g_UserMng.AddMoverBehavior( pSrc, pSrc->GetPos(), pSrc->m_pActMover->m_vDelta,
				pSrc->GetAngle(), pSrc->m_pActMover->GetState(), pSrc->m_pActMover->GetStateFlag(), 
				pSrc->m_dwMotion, pSrc->m_pActMover->m_nMotionEx, pSrc->m_pModel->m_nLoop, pSrc->m_dwMotionOption, g_TickCount.GetTickCount(), TRUE );

			ItemProp *pItemProp = prj.GetItemProp( II_CHEERUP ); // ���� ������
			if( pItemProp )
				pTarget->DoApplySkill( pTarget, pItemProp, NULL );
		}
		else
		{
			pUser->AddDefinedText( TID_CHEER_NO2, "" );	
		}
	}
}

void CDPSrvr::OnQueryEquip( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	OBJID objid;
	ar >> objid;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		CUser* pUsertmp		= prj.GetUser( objid );
		if( IsValidObj( pUsertmp ) )
		{
			
			if( pUsertmp->IsMode( EQUIP_DENIAL_MODE ) && pUser->IsAuthHigher( AUTH_GAMEMASTER ) == FALSE )
			{
				pUser->AddDefinedText( TID_DIAG_0088 );
				return;
			}
			pUser->AddQueryEquip( pUsertmp );
		}
	}
}

void CDPSrvr::OnQueryEquipSetting( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	BOOL bAllow;
	ar >> bAllow;

	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		if( bAllow )
			pUser->SetNotMode( EQUIP_DENIAL_MODE );
		else
			pUser->SetMode( EQUIP_DENIAL_MODE );
		g_UserMng.AddModifyMode( pUser );
	}
}

#if __VER >= 12 // __UPDATE_OPT
void CDPSrvr::OnOptionEnableRenderMask( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		BOOL bEnable;
		ar >> bEnable;
		if( bEnable )
			pUser->SetNotMode( MODE_OPTION_DONT_RENDER_MASK );
		else
			pUser->SetMode( MODE_OPTION_DONT_RENDER_MASK );
		g_UserMng.AddModifyMode( pUser );
	}
}
#endif	// __UPDATE_OPT

void CDPSrvr::OnReturnScroll( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	int nSelect;
	ar >> nSelect;
	if( nSelect < -1 || nSelect > 2 )
		return;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) == FALSE )
		return;

	if( nSelect >= 0 )
	{
		static char* szPos[] = { "flaris",	"saintmorning",	"darkon" };
		pUser->m_lpszVillage = szPos[nSelect];
		pUser->AddReturnScroll();	//������ ������ '��ȯ�� �η縶��'�������� ����Ѵ�.	
	}
	else
	{
		// ����� ��ġ�� ���ư��� 
		if( pUser->HasBuff( BUFF_ITEM, II_SYS_SYS_SCR_RETURN ) )
			pUser->DoUseItemVirtual( II_SYS_SYS_SCR_RETURN, TRUE );
	}
}

void CDPSrvr::OnEndSkillQueue( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
		pUser->m_playTaskBar.OnEndSkillQueue( pUser );
}

void CDPSrvr::OnQueryPostMail( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	char lpszReceiver[MAX_PLAYER]	= { 0, };
	char lpszTitle[MAX_MAILTITLE]	= { 0, };
	char lpszText[MAX_MAILTEXT]		= { 0, };
	BYTE nItem;
	short nItemNum;
	int nGold;

	ar >> nItem >> nItemNum;
	ar.ReadString( lpszReceiver, MAX_PLAYER );
	
	ar >> nGold;
	ar.ReadString( lpszTitle, MAX_MAILTITLE );
	ar.ReadString( lpszText, MAX_MAILTEXT );

	DWORD nPostGold = 100;

	if( ::GetLanguage() != LANG_KOR )
		nPostGold = 500;
					
	if( nItem < 0 || nGold < 0 || ( nGold + nPostGold ) <= 0 )
		return;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		//raiders.2006.11.27
		if( pUser->m_vtInfo.GetOther() )	// �ŷ����� ����� ������?
			return;
		if( pUser->m_vtInfo.VendorIsVendor() )		// ���� �Ȱ� ������?
			return;
		if( pUser->m_bBank )				// â�� ���� ������?
			return;
#ifdef __S_SERVER_UNIFY
		if( pUser->m_bAllAction == FALSE )
			return;
#endif // __S_SERVER_UNIFY
		//--
#if __VER >= 11 // __SYS_PLAYER_DATA
		u_long idReceiver	= CPlayerDataCenter::GetInstance()->GetPlayerId( lpszReceiver );
#else	// __SYS_PLAYER_DATA
		u_long idReceiver	= prj.GetPlayerID( lpszReceiver );
#endif	// __SYS_PLAYER_DATA
		if( idReceiver > 0 )
		{
#if __VER >= 11 // __SYS_PLAYER_DATA
			if( CPlayerDataCenter::GetInstance()->GetPlayerId( (char*)pUser->GetName() ) == idReceiver )
#else	// __SYS_PLAYER_DATA
			if( prj.GetPlayerID( pUser->GetName() ) == idReceiver )
#endif	// __SYS_PLAYER_DATA
			{
				pUser->AddDiagText(prj.GetText(TID_GAME_MSGSELFSENDERROR));
				return;
			}

			if( !CNpcChecker::GetInstance()->IsCloseNpc( MMI_POST, pUser->GetWorld(), pUser->GetPos() ) )
			{
				return;
			}
			CMailBox* pMailBox	= CPost::GetInstance()->GetMailBox( idReceiver );
			if( pMailBox && pMailBox->size() >= MAX_MAIL )
			{
				pUser->AddDefinedText( TID_GAME_MAILBOX_FULL, "%s", lpszReceiver );
				return;
			}

			CItemElem* pItemElem	= pUser->m_Inventory.GetAtId( nItem );
			if( pItemElem )
			{
				if( IsUsableItem( pItemElem ) == FALSE )
				{
					pUser->AddDiagText( prj.GetText( TID_GAME_CANNOT_POST ) );
					return;
				}
				if( pUser->m_Inventory.IsEquip( pItemElem->m_dwObjId ) )
				{
					pUser->AddDiagText( prj.GetText( TID_GAME_CANNOT_POST ) );
					return;
				}
				if( pItemElem->IsQuest() )
				{
					pUser->AddDiagText( prj.GetText( TID_GAME_CANNOT_POST ) );
					return;
				}
				if( pItemElem->IsBinds() )
				{
					pUser->AddDiagText( prj.GetText( TID_GAME_CANNOT_POST ) );
					return;
				}
				if( pUser->IsUsing( pItemElem ) )
				{
					pUser->AddDiagText( prj.GetText( TID_GAME_CANNOT_DO_USINGITEM ) );
					return;
				}
				ItemProp* pProp	= pItemElem->GetProp();
				if( pProp->dwItemKind3 == IK3_CLOAK && pItemElem->m_idGuild != 0 )
				{
					pUser->AddDiagText( prj.GetText( TID_GAME_CANNOT_POST ) );
					return;
				}

//				if( pItemElem->m_dwItemId == II_RID_RID_BOR_EVEINSHOVER || pItemElem->m_dwItemId == II_RID_RID_BOR_LADOLF )
//				{
//					pUser->AddDiagText( prj.GetText( TID_GAME_CANNOT_POST ) );
//					return;
//				}
				if( pProp->dwParts == PARTS_RIDE && pProp->dwItemJob == JOB_VAGRANT )
				{
					pUser->AddDiagText( prj.GetText( TID_GAME_CANNOT_POST ) );
					return;
				}

				if( nItemNum <= 0 )			// hacking
					return;
				if( pItemElem->m_nItemNum < nItemNum )
					nItemNum	= pItemElem->m_nItemNum;
				if( pItemElem->IsCharged() )
				{
					pUser->AddDiagText( prj.GetText( TID_GAME_CANNOT_POST ) );
					return;
				}
			}
			if( pUser->GetGold() < (int)( ( nPostGold + nGold ) ) )
			{
				pUser->AddDiagText( prj.GetText( TID_GAME_LACKMONEY ) );
				return;
			}

			pUser->AddGold( (int)( (int)( nPostGold + nGold ) * (-1) ), TRUE );	// ���� ����
			
			CItemElem	itemElem;
			if( pItemElem )
			{
				itemElem	= *pItemElem;
				itemElem.m_nItemNum		= nItemNum;
				pUser->RemoveItem( (BYTE)pItemElem->m_dwObjId, nItemNum );
				CWorld* pWorld	= pUser->GetWorld();
				if( pWorld )
#ifdef __LAYER_1015
					g_dpDBClient.SavePlayer( pUser, pWorld->GetID(), pUser->GetPos(), pUser->GetLayer() );
#else	// __LAYER_1015
					g_dpDBClient.SavePlayer( pUser, pWorld->GetID(), pUser->GetPos() );
#endif	// __LAYER_1015
			}

// 			//	BEGINTEST
// 			Error( "OnQueryPostMail  Receiver[%d] Sender[%d]", idReceiver, pUser->m_idPlayer );

			g_dpDBClient.SendQueryPostMail( idReceiver, pUser->m_idPlayer, itemElem, nGold, lpszTitle, lpszText );
		}
		else
		{
			// input name is wrong
			pUser->AddDiagText(prj.GetText(TID_MAIL_UNKNOW));
		}
	}
	else
	{
// 		//	BEGINTEST
// 		Error( "OnQueryPostMail  pUser == NULL [%d]", dpidUser );
	}

}

void CDPSrvr::OnQueryRemoveMail( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	u_long nMail;
	ar >> nMail;

	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		CMailBox* pMailBox = CPost::GetInstance()->GetMailBox( pUser->m_idPlayer );
		if( pMailBox != NULL )
		{
			CMail* pMail = pMailBox->GetMail( nMail );
			if( pMail != NULL )
			{
				g_dpDBClient.SendQueryRemoveMail( pUser->m_idPlayer, nMail );
			}
			else
			{
				Error( "CDPSrvr::OnQueryRemoveMail - Invalid nMail. idReceiver : %07d, nMail :[%d]", pUser->m_idPlayer, nMail );
			}
		}
		else
		{
			Error( "CDPSrvr::OnQueryRemoveMail - Invalid pMailBox. idReceiver : %07d, nMail :[%d]", pUser->m_idPlayer, nMail );
		}
	}
}

void CDPSrvr::OnQueryGetMailItem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	u_long nMail;
	ar >> nMail;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		if( pUser->m_Inventory.GetEmptyCount() < 1 )
		{
			pUser->AddDiagText(  prj.GetText( TID_GAME_LACKSPACE ) );
			return;
		}
		
		CMailBox* pMailBox	= CPost::GetInstance()->GetMailBox( pUser->m_idPlayer );
		if( pMailBox )
		{
			CMail* pMail = pMailBox->GetMail(nMail);
			if( pMail && pMail->m_pItemElem )
			{
				// �⺻ �����ϼ� ���������� �˻��Ͽ� ������ �ΰ��Ѵ�.
				int nDay = 0;
				DWORD dwTime = 0;
				pMail->GetMailInfo( &nDay, &dwTime );

				// �⺻ �����ϼ��� ������!!!
				if( (MAX_KEEP_MAX_DAY*24) - dwTime > (MAX_KEEP_BASIC_DAY*24) )
				{
					FLOAT fCustody = 0.0f;
					FLOAT fPay = 0.0f;
					fCustody = (FLOAT)( (FLOAT)( MAX_KEEP_MAX_DAY - MAX_KEEP_BASIC_DAY - nDay ) / (FLOAT)( MAX_KEEP_MAX_DAY - MAX_KEEP_BASIC_DAY ) );
					fPay = pMail->m_pItemElem->GetCost() * fCustody;
					if( fPay < 0.0f )
						fPay = 0.0f;

					if( (int)fPay > pUser->GetGold() )
					{
						pUser->AddDiagText( prj.GetText( TID_GAME_LACKMONEY ) );
						return;
					}
					pUser->AddGold( -((int)fPay) );
				}
			}
		}
		g_dpDBClient.SendQueryGetMailItem( pUser->m_idPlayer, nMail );
	}
}

void CDPSrvr::OnQueryGetMailGold( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	u_long nMail;
	ar >> nMail;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		CMailBox* pMailBox = CPost::GetInstance()->GetMailBox( pUser->m_idPlayer );
		if( pMailBox != NULL )
		{
			CMail* pMail = pMailBox->GetMail( nMail );
			if( pMail != NULL )
			{
				g_dpDBClient.SendQueryGetMailGold( pUser->m_idPlayer, nMail );
			}
			else
			{
				Error( "CDPSrvr::OnQueryGetMailGold - Invalid nMail. idReceiver : %07d, nMail :[%d]", pUser->m_idPlayer, nMail );
			}
		}
		else
		{
			Error( "CDPSrvr::OnQueryGetMailGold - Invalid pMailBox. idReceiver : %07d, nMail :[%d]", pUser->m_idPlayer, nMail );
		}
	}
}

void CDPSrvr::OnQueryReadMail( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	u_long nMail;
	ar >> nMail;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		CMailBox* pMailBox = CPost::GetInstance()->GetMailBox( pUser->m_idPlayer );
		if( pMailBox != NULL )
		{
			CMail* pMail = pMailBox->GetMail( nMail );
			if( pMail != NULL )
			{
				g_dpDBClient.SendQueryReadMail( pUser->m_idPlayer, nMail );
			}
			else
			{
				Error( "CDPSrvr::OnQueryReadMail - Invalid nMail. idReceiver : %07d, nMail :[%d]", pUser->m_idPlayer, nMail );
			}
		}
		else
		{
			Error( "CDPSrvr::OnQueryReadMail - Invalid pMailBox. idReceiver : %07d, nMail :[%d]", pUser->m_idPlayer, nMail );
		}
	}
}

void CDPSrvr::OnQueryMailBox( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
// 	//	BEGINTEST
// 	Error( "OnQueryMailBox [%d]", dpidUser );

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );

	int	nClientReqCount	= 1;

	if( IsValidObj( pUser ) )
	{
		if( pUser->CheckClientReq()== false )
		{
			nClientReqCount	= 1;
		}
		else
		{
			nClientReqCount	= pUser->GetCountClientReq();
		}

		CMailBox* pMailBox	= CPost::GetInstance()->GetMailBox( pUser->m_idPlayer );
		if( pMailBox )
		{
			switch( pMailBox->m_nStatus )
			{
				case CMailBox::data:	// �����Ͱ� ����ִ� ���� �ڽ��� �ٷ� ����� ����
					{
						if( nClientReqCount <= 1 )
						{
							pUser->AddMailBox( pMailBox );
						}
						else
						{
							g_dpDBClient.SendQueryMailBoxCount( pUser->m_idPlayer, pUser->GetCountClientReq() );
						}

// 						//	BEGINTEST
// 						Error( "OnQueryMailBox CMailBox::data [%d]", dpidUser );
					}
					break;
				case CMailBox::nodata:	// �����Ͱ� ���� ���� �ڽ��� Ʈ���� ������ ���� ��û, ���´� �д� ��
					{
						if( nClientReqCount >= 2 )
						{
							g_dpDBClient.SendQueryMailBoxCount( pUser->m_idPlayer, pUser->GetCountClientReq() );
						}
						else
						{
							g_dpDBClient.SendQueryMailBox( pUser->m_idPlayer );
						}

						pMailBox->m_nStatus		= CMailBox::read;
					}
					break;
				case CMailBox::read:	// �����͸� ��û�ϰ� ����ϴ� ���¸� ����
					{
						if( nClientReqCount >= 2 )
						{
							g_dpDBClient.SendQueryMailBoxCount( pUser->m_idPlayer, pUser->GetCountClientReq() );
						}
// 						//	BEGINTEST
// 						Error( "OnQueryMailBox CMailBox::read [%d]", dpidUser );
					}
					break;
				default:
					{
// 						//	BEGINTEST
// 						Error( "OnQueryMailBox default [%d]", dpidUser );
					}
					break;
			}
		}
		else // ���弭���� ������ ���Ϲڽ��� ����. Ʈ������ ��û
		{
			if( pUser->GetCheckTransMailBox() == FALSE )
			{
				g_dpDBClient.SendQueryMailBoxReq( pUser->m_idPlayer );
			}
			else
			{
				pUser->SendCheckMailBoxReq( FALSE );
			}
		}
	}
	else
	{
		Error( "CDPSrvr::OnQueryMailBox - Invalid User : %d", dpidUser );
	}
}

void CDPSrvr::OnGCApp( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	BYTE nState;
	ar >> nState;
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		CGuild *pGuild = pUser->GetGuild();

		// ��尡 ���ų� ������� �ƴϸ� ��û �Ұ�
		if( pGuild == NULL || pGuild->IsMaster( pUser->m_idPlayer ) == FALSE )
		{
			pUser->AddDiagText( prj.GetText( TID_GAME_GUILDCOMBAT_NOT_GUILD_LEADER ) );
			return;
		}

		if( nState == GC_IN_WINDOW )
		{
			CGuild* pGuild	= g_GuildMng.GetGuild( pUser->m_idGuild );
			if( pGuild && pGuild->IsMaster( pUser->m_idPlayer ) )
			{
#if __VER >= 11 // __GUILD_COMBAT_1TO1
				// 1:1�������� ������ ���� ���� �Ұ����ϴ�.
				int nIndex = g_GuildCombat1to1Mng.GetTenderGuildIndexByUser( pUser );
				if( nIndex != NULL_ID )
				{
					pUser->AddDefinedText( TID_GAME_GUILDCOMBAT1TO1_ISGC1TO1TENDER );
					return;
				}
#endif // __GUILD_COMBAT_1TO1		
				pUser->AddGCWindow( g_GuildCombatMng.GetPrizePenya( 2 ), g_GuildCombatMng.GetRequstPenya( pUser->m_idGuild ), g_GuildCombatMng.m_nJoinPanya );
			}
		}
		else if( nState == GC_IN_APP )
		{
			DWORD dwPenya;
			ar >> dwPenya;
			g_GuildCombatMng.GuildCombatRequest( pUser, dwPenya );	// 
		}
	}
}
void CDPSrvr::OnGCCancel( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
		g_GuildCombatMng.GuildCombatCancel( pUser );
}
void CDPSrvr::OnGCRequestStatus( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
		pUser->AddGCRequestStatus( g_GuildCombatMng.GetPrizePenya( 2 ), g_GuildCombatMng.vecRequestRanking );
}
void CDPSrvr::OnGCSelectPlayer( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	BOOL bWindow;
	ar >> bWindow;
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		// ĳ���͸� �����Ҽ� �ִ� �ð����� �˻�
		if( g_GuildCombatMng.m_nGCState != CGuildCombat::NOTENTER_COUNT_STATE ) 
		{
 			pUser->AddText( prj.GetText(TID_GAME_GUILDCOMBAT_CANNOT_MAKEUP) ); //������ ����ۼ��� �� �� �����ϴ�.		
			return;
		}

		// ĳ���͸� �����Ҽ� �ִ� ��� ���� �˻�
		if( g_GuildCombatMng.IsRequestWarGuild( pUser->m_idGuild, FALSE ) == FALSE )
		{
			pUser->AddText( prj.GetText(TID_GAME_GUILDCOMBAT_CANNOT_MAKEUP_FAIL) );	//������ ������ ���� �ʾҰų� ���� ���� �������� ���� ���߽��ϴ�.		
			return;
		}

		CGuild* pGuild	= g_GuildMng.GetGuild( pUser->m_idGuild );
		// ĳ���͸� �����Ҽ� �մ°��� �����Ϳ� ŷ�ɱ��̴�.
		BOOL bMK = FALSE;
		if( pGuild )
		{
			CGuildMember* pGuildMember = pGuild->GetMember( pUser->m_idPlayer );
			if( pGuildMember )
			{
				if( pGuildMember->m_nMemberLv == GUD_KINGPIN || pGuildMember->m_nMemberLv == GUD_MASTER )
					bMK = TRUE;
			}
		}
		
		
		if( pGuild && bMK )
		{
			if( bWindow == FALSE )
			{
				// ������ �޼����� �ƴϹǷ� Settting
				int nSize;
				u_long uidPlayer, uidDefender;
				vector< u_long > vecSelectPlayer;
				BOOL bMasterOrKinpin = FALSE;
				BOOL bDefender = FALSE;
				BOOL bLevel = FALSE;
				BOOL bLogOut = FALSE;
				BOOL bGuildMember = FALSE;
				BOOL bMastertoDefender = FALSE;
				vecSelectPlayer.clear();
				ar >> uidDefender;
				ar >> nSize;
				if( nSize > g_GuildCombatMng.m_nMaxJoinMember )
					return;

				for( int i = 0 ; i < nSize ; ++i )
				{
					ar >> uidPlayer;
					vecSelectPlayer.push_back( uidPlayer );
					CUser* pUsertmp = g_UserMng.GetUserByPlayerID( uidPlayer );
					if( IsValidObj( pUsertmp ) )
					{
						CGuildMember* pGuildMember = pGuild->GetMember( uidPlayer );
						// ����� �ɹ�
						if( pGuildMember )
						{
							// �����Ͱ� ����Ʈ�� �ִ���?
							if( pGuild->IsMaster( pUsertmp->m_idPlayer ) )
								bMasterOrKinpin = TRUE;
							// ŷ���� ����Ʈ�� �ִ���?
							if( pGuildMember->m_nMemberLv == GUD_KINGPIN )
								bMasterOrKinpin = TRUE;
							// ������� ����Ʈ�� �ִ���?
							if( pUsertmp->m_idPlayer == uidDefender )
								bDefender = TRUE;
						}
					}
				}
				// �����ʹ� ������� �ɼ� ����.
				if( 1 < nSize && pGuild->IsMaster( uidDefender ) )
					bMastertoDefender = TRUE;

				if( bMasterOrKinpin && bMastertoDefender == FALSE 
#ifndef _DEBUG
					&& bDefender
#endif // _DEBUG
					)
				{
					g_GuildCombatMng.SelectPlayerClear( pUser->m_idGuild );
					for( int veci = 0 ; veci < (int)( vecSelectPlayer.size() ) ; ++veci )
					{
						// �ִ� �ο��� �̻��� �ȵ���.
						if( veci >= g_GuildCombatMng.m_nMaxJoinMember )
							break;

						u_long uidSelectPlayer = vecSelectPlayer[veci];
						CUser* pUsertmp = g_UserMng.GetUserByPlayerID( uidSelectPlayer );
						{
							if( IsValidObj( pUsertmp ) )
							{
								CGuildMember* pGuildMember = pGuild->GetMember( uidPlayer );
								// ����� �ɹ�
								if( pGuildMember )
								{
									// ������ 30�̻� ��������
									if( 30 <= pUsertmp->GetLevel() )
									{
										g_GuildCombatMng.AddSelectPlayer( pUser->m_idGuild, uidSelectPlayer );
									}
								}
							}
						}						
					}
					g_GuildCombatMng.SetDefender( pUser->m_idGuild, uidDefender );			

					WriteLog( "GuildCombat SelectPlayer GuildID=%d", pUser->m_idGuild );
				}
			}

			vector<CGuildCombat::__JOINPLAYER> vecSelectList;
			g_GuildCombatMng.GetSelectPlayer( pUser->m_idGuild, vecSelectList );
			pUser->AddGCSelectPlayerWindow( vecSelectList, g_GuildCombatMng.GetDefender(pUser->m_idGuild), bWindow, g_GuildCombatMng.IsRequestWarGuild(pUser->m_idGuild, FALSE) );
			if( bWindow == FALSE )
			{
				g_UserMng.AddGCGuildStatus( pUser->m_idGuild );
				g_UserMng.AddGCWarPlayerlist( pUser->m_idGuild );
			}
		}
	}
}
void CDPSrvr::OnGCSelectMap( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	int nMap;
	ar >> nMap;
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
		g_GuildCombatMng.SetSelectMap( pUser, nMap );
}
void CDPSrvr::OnGCJoin( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		if( !CNpcChecker::GetInstance()->IsCloseNpc( MMI_GUILDWAR_JOIN, pUser->GetWorld(), pUser->GetPos() ) )
			return;
		g_GuildCombatMng.GuildCombatEnter( pUser );
	}
}
void CDPSrvr::OnGCGetPenyaGuild( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		CGuild *pGuild = pUser->GetGuild();		
		if( pGuild && pGuild->IsMaster( pUser->m_idPlayer ) )
		{
			// ��û�ݾ� �� ������ �ִ°�?
			__int64 nGetPenya = 0;
			BOOL bFind = FALSE;
			CGuildCombat::__GCRESULTVALUEGUILD ResultValueGuild;
			for( int veci = 0 ; veci < (int)( g_GuildCombatMng.m_GCResultValueGuild.size() ) ; ++veci )
			{
				ResultValueGuild = g_GuildCombatMng.m_GCResultValueGuild[veci];
				if( pGuild->m_idGuild == ResultValueGuild.uidGuild )
				{
					bFind = TRUE;
					nGetPenya = ResultValueGuild.nReturnCombatFee + ResultValueGuild.nReward;
					break;
				}
			}

			if( bFind )
			{
				__int64 nTotal = (__int64)pUser->GetGold() + nGetPenya;
				if( nGetPenya > INT_MAX || nTotal > INT_MAX )
					pUser->AddGCGetPenyaGuild( 3, nGetPenya );
				else
					g_dpDBClient.SendGCGetPenyaGuild( pUser->m_idPlayer, ResultValueGuild.nCombatID, ResultValueGuild.uidGuild );
			}
			else
			{
				pUser->AddGCGetPenyaGuild( 2, nGetPenya );
			}
		}
	}
}

void CDPSrvr::OnGCGetPenyaPlayer( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		// ������ �ִ°�?
		__int64 nGetPenya = 0;
		BOOL bFind = FALSE;
		CGuildCombat::__GCRESULTVALUEPLAYER ResultValuePlayer;
		for( int veci = 0 ; veci < (int)( g_GuildCombatMng.m_GCResultValuePlayer.size() ) ; ++veci )
		{
			ResultValuePlayer = g_GuildCombatMng.m_GCResultValuePlayer[veci];
			if( pUser->m_idPlayer == ResultValuePlayer.uidPlayer )
			{
				bFind = TRUE;
				nGetPenya = ResultValuePlayer.nReward;
				break;
			}
		}

		if( bFind )
		{
			__int64 nTotal = (__int64)pUser->GetGold() + nGetPenya;
			if( nGetPenya > INT_MAX || nTotal > INT_MAX )
				pUser->AddGCGetPenyaPlayer( 2, nGetPenya );
			else
				g_dpDBClient.SendGCGetPenyaPlayer( pUser->m_idPlayer, ResultValuePlayer.nCombatID, ResultValuePlayer.uidGuild );
		}
		else
		{
			pUser->AddGCGetPenyaPlayer( 1, nGetPenya );
		}
	}
}

#if __VER < 8 // #ifndef __GUILDCOMBAT_85
void CDPSrvr::OnGCGetItem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		CGuild *pGuild = pUser->GetGuild();		
		if( pGuild && pGuild->IsMember( pUser->m_idPlayer ) )
		{
			if( pGuild->GetGuildId() != g_GuildCombatMng.m_uWinGuildId )
				return;

			if( g_GuildCombatMng.m_nItemPenya > pUser->GetGold() )
			{
				pUser->AddText( prj.GetText(TID_GAME_LACKMONEY) ); //���(��)�� �����մϴ�.
				return;
			}

			if( g_GuildCombatMng.m_nGCState != CGuildCombat::WAR_CLOSE_STATE && g_GuildCombatMng.m_nGCState != CGuildCombat::WAR_TELEPORT_STATE )
			{
				pUser->AddText( prj.GetText(TID_GAME_GUILDCOMBAT_NOT_GETITEM) ); // ������ �߿��� ����Ҽ� �����ϴ�.
				return;
			}

			// ���� ��ǰ ����
			CItemElem itemElem;
			
			if( g_GuildCombatMng.m_nWinGuildCount == 1 )
				itemElem.m_dwItemId	= II_ARM_S_CLO_CLO_DRAGON1;		// ����� ����
			else if( g_GuildCombatMng.m_nWinGuildCount == 2 )
				itemElem.m_dwItemId	= II_ARM_S_CLO_CLO_DRAGON2;		// ����� ����
			else if( g_GuildCombatMng.m_nWinGuildCount == 3 )
				itemElem.m_dwItemId	= II_ARM_S_CLO_CLO_DRAGON3;		// ����� ����
			else if( g_GuildCombatMng.m_nWinGuildCount >= 4 )
				itemElem.m_dwItemId	= II_ARM_S_CLO_CLO_DRAGON4;		// ����� ����
			itemElem.m_nItemNum		= 1;
			itemElem.m_idGuild	= pGuild->m_idGuild;
			itemElem.SetSerialNumber();
			ItemProp* pItemProp		= itemElem.GetProp();
			if( pItemProp )
				itemElem.m_nHitPoint	= pItemProp->dwEndurance;
			else
				itemElem.m_nHitPoint	= 0;

			CTime tNextCombat = g_GuildCombatMng.GetNextGuildCobmatTime();
			itemElem.m_dwKeepTime = tNextCombat.GetTime();
			
			if( pUser->CreateItem( &itemElem ) )
			{
				LogItemInfo aLogItem;
				aLogItem.Action = "9";
				aLogItem.SendName = pUser->GetName();
				aLogItem.RecvName = "GUILDCOMBAT";
				aLogItem.WorldId = pUser->GetWorld()->GetID();
				aLogItem.Gold = pUser->GetGold();
				aLogItem.Gold2 = pUser->GetGold() - g_GuildCombatMng.m_nItemPenya;
				OnLogItem( aLogItem, &itemElem, 1 );		// why do not pass &itemElem as argument?
				pUser->AddGold( -g_GuildCombatMng.m_nItemPenya );	
			}
			else
			{
				pUser->AddDefinedText( TID_GAME_LACKSPACE, "" );
			}
		}
	}
}
#endif // __VER < 8
void CDPSrvr::OnGCTele( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		PRegionElem pRgnElem = g_WorldMng.GetRevivalPos( WI_WORLD_MADRIGAL, "flaris" );
		if( pRgnElem )
			((CMover*)pUser)->REPLACE( g_uIdofMulti, WI_WORLD_MADRIGAL, D3DXVECTOR3( 6983.0f, 0.0f, 3330.0f ), REPLACE_NORMAL, nDefaultLayer );
	}
}
void CDPSrvr::OnGCPlayerPoint( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
		pUser->AddGCPlayerPoint();
}

void CDPSrvr::OnSummonFriend( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	DWORD dwData;
	char lpszPlayer[MAX_PLAYER]	= { 0, };
	
	ar >> dwData;
	ar.ReadString( lpszPlayer, MAX_PLAYER );

	if( strlen(lpszPlayer) >= MAX_NAME )
		return;
	
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		int nState = pUser->GetSummonState();
		if( nState != 0 )
		{
			DWORD dwMsgId = 0;
			if( nState == 1 )	// �ŷ���
				dwMsgId = TID_GAME_TRADE_NOTUSE;
			else if( nState == 2 ) // ����
				dwMsgId = TID_GAME_DIE_NOTUSE;
			else if( nState == 3 ) // ���λ��� ��
				dwMsgId = TID_GAME_VENDOR_NOTUSE;
			else if( nState == 4 ) // ������
				dwMsgId = TID_GAME_ATTACK_NOTUSE;
			else if( nState == 5 ) // ������
				dwMsgId = TID_GAME_FLY_NOTUSE;
#if __VER >= 8 // 8�� ��� 061226 ma
			else if( nState == 6 ) // �����
				dwMsgId = TID_GAME_ATTACK_NOTUSE;
#endif // __VER >= 8 // 8�� ��� 061226 ma

			pUser->AddDefinedText( TID_GAME_STATE_NOTUSE, "\"%s\"", prj.GetText( dwMsgId ) );
			return;
		}

		WORD wId	= LOWORD( dwData );
		WORD wMode	= HIWORD( dwData );
		CItemElem* pItemElem = (CItemElem*)pUser->GetItemId( wId );
		if( IsUsableItem( pItemElem ) )
		{
			if( pItemElem->m_dwItemId != II_SYS_SYS_SCR_FRIENDSUMMON_A && pItemElem->m_dwItemId != II_SYS_SYS_SCR_FRIENDSUMMON_B )
				return;
			if( pItemElem->m_bQuery )
				return;

#if __VER >= 11 // __SYS_PLAYER_DATA
			CUser* pUsertmp = g_UserMng.GetUserByPlayerID( CPlayerDataCenter::GetInstance()->GetPlayerId( lpszPlayer ) );
#else	// __SYS_PLAYER_DATA
			CUser* pUsertmp = g_UserMng.GetUserByPlayerID( prj.GetPlayerID( lpszPlayer ) );
#endif	// __SYS_PLAYER_DATA
			if( IsValidObj( (CObj*)pUsertmp ) )
			{
				nState = pUsertmp->GetSummonState();
				if( nState != 0 )
				{
					DWORD dwMsgId = 0;
					if( nState == 1 )	// �ŷ���
						dwMsgId = TID_GAME_TRADE_NOTUSE1;
					else if( nState == 2 ) // ����
						dwMsgId = TID_GAME_DIE_NOTUSE1;
					else if( nState == 3 ) // ���λ��� ��
						dwMsgId = TID_GAME_VENDOR_NOTUSE1;
					else if( nState == 4 ) // ������
						dwMsgId = TID_GAME_ATTACK_NOTUSE1;
					else if( nState == 5 ) // ������
						dwMsgId = TID_GAME_FLY_NOTUSE1;
#if __VER >= 8 // 8�� ��� 061226 ma
					else if( nState == 6 ) // �����
						dwMsgId = TID_GAME_ATTACK_NOTUSE1;
#endif // __VER >= 8 // 8�� ��� 061226 ma
					
					pUser->AddDefinedText( TID_GAME_STATE_NOTUSE, "\"%s\"", prj.GetText( dwMsgId ) );
				}
				else if( pUser->m_idPlayer == pUsertmp->m_idPlayer )
				{
					pUser->AddDefinedText( TID_GAME_SUMMON_FRIEND_MY_NOUSE );
				}
				else if( prj.IsGuildQuestRegion( pUser->GetPos() ) )
				{
					pUser->AddDefinedText( TID_GAME_STATE_NOTUSE, "\"%s\"", prj.GetText( TID_GAME_EVENT_WORLD_NOTUSE ) );
				}
				else if( prj.IsGuildQuestRegion( pUsertmp->GetPos() ) )
				{
					pUser->AddDefinedText( TID_GAME_STATE_NOTUSE, "\"%s\"", prj.GetText( TID_GAME_EVENT_WORLD_NOTUSE1 ) );
				}
				else if( pUser->GetWorld()->GetID() != pUsertmp->GetWorld()->GetID()
#if __VER >= 14 // __INSTANCE_DUNGEON
						|| pUser->GetLayer() != pUsertmp->GetLayer()
#endif // __INSTANCE_DUNGEON
					)
				{
					CString strtmp;
					strtmp.Format( prj.GetText( TID_GAME_WORLD_NOTUSE ), pUser->GetWorld()->m_szWorldName, pUsertmp->GetWorld()->m_szWorldName );
					pUser->AddDefinedText( TID_GAME_STATE_NOTUSE, "\"%s\"", strtmp );
				}
#if __VER >= 13 // __RAINBOW_RACE
				else if( CRainbowRaceMng::GetInstance()->IsEntry( pUser->m_idPlayer )
						|| CRainbowRaceMng::GetInstance()->IsEntry( pUsertmp->m_idPlayer ) )
				{
					pUser->AddDefinedText( TID_GAME_RAINBOWRACE_NOTELEPORT );
					return;
				}
#endif // __RAINBOW_RACE
#ifdef __QUIZ
				else if( pUser->GetWorld()->GetID() == WI_WORLD_QUIZ 
						|| pUsertmp->GetWorld()->GetID() == WI_WORLD_QUIZ )
					pUser->AddDefinedText( TID_GAME_QUIZ_DO_NOT_USE );
						
#endif // __QUIZ
				else
				{
#ifdef __RT_1025
					if( pUsertmp->m_RTMessenger.IsBlock( pUser->m_idPlayer ) )
#else	//__RT_1025
					LPFRIEND pFriend	= pUsertmp->m_Messenger.GetFriend( pUser->m_idPlayer );
					if( pFriend && pFriend->dwState == FRS_BLOCK )
#endif	// __RT_1025
					{
						pUser->AddDefinedText( TID_ERROR_SUMMONFRIEND_NOUSER, "\"%s\"", lpszPlayer );
						return;
					}
					pItemElem->m_bQuery		= TRUE;
					pUsertmp->AddSummonFriendConfirm( pUser->GetId(), dwData, pUser->GetName(), pUser->GetWorld()->m_szWorldName );
					pUser->AddDefinedText( TID_GAME_SUMMONFRIEND_CONFIRM, "\"%s\"", lpszPlayer );
				}
			}
			else
			{
				pUser->AddDefinedText( TID_ERROR_SUMMONFRIEND_NOUSER, "\"%s\"", lpszPlayer );
			}
		}	
		else
		{
			ItemProp* pItemProp = prj.GetItemProp( II_SYS_SYS_SCR_FRIENDSUMMON_A );
			if( pItemProp )
				pUser->AddDefinedText( TID_ERROR_SUMMONFRIEND_NOITEM, "\"%s\" \"%s\"", pItemProp->szName, lpszPlayer );
		}
	}
}
void CDPSrvr::OnSummonFriendConfirm( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	OBJID objid;
	DWORD dwData;

	ar >> objid >> dwData;
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		CUser* pUsertmp = prj.GetUser( objid );
		if( IsValidObj( (CObj*)pUsertmp ) )
		{
			int nState = pUser->GetSummonState();
			if( nState != 0 )
			{
				DWORD dwMsgId = 0;
				if( nState == 1 )	// �ŷ���
					dwMsgId = TID_GAME_TRADE_NOTUSE;
				else if( nState == 2 ) // ����
					dwMsgId = TID_GAME_DIE_NOTUSE;
				else if( nState == 3 ) // ���λ��� ��
					dwMsgId = TID_GAME_VENDOR_NOTUSE;
				else if( nState == 4 ) // ������
					dwMsgId = TID_GAME_ATTACK_NOTUSE;
				else if( nState == 5 ) // ������
					dwMsgId = TID_GAME_FLY_NOTUSE;
#if __VER >= 8 // 8�� ��� 061226 ma
				else if( nState == 6 ) // �����
					dwMsgId = TID_GAME_ATTACK_NOTUSE;
#endif // __VER >= 8 // 8�� ��� 061226 ma
				
				pUser->AddDefinedText( TID_GAME_STATE_NOTSUMMONOK, "\"%s\"", prj.GetText( dwMsgId ) );
				pUsertmp->AddDefinedText( TID_GAME_STATE_NOTSUMMON, "\"%s\"", prj.GetText( dwMsgId + 1 ) );
				return;
			}

			nState = pUsertmp->GetSummonState();
			if( nState != 0 )
			{
				DWORD dwMsgId = 0;
				if( nState == 1 )	// �ŷ���
					dwMsgId = TID_GAME_TRADE_NOTUSE1;
				else if( nState == 2 ) // ����
					dwMsgId = TID_GAME_DIE_NOTUSE1;
				else if( nState == 3 ) // ���λ��� ��
					dwMsgId = TID_GAME_VENDOR_NOTUSE1;
				else if( nState == 4 ) // ������
					dwMsgId = TID_GAME_ATTACK_NOTUSE1;
				else if( nState == 5 ) // ������
					dwMsgId = TID_GAME_FLY_NOTUSE1;
#if __VER >= 8 // 8�� ��� 061226 ma
				else if( nState == 6 ) // �����
					dwMsgId = TID_GAME_ATTACK_NOTUSE1;
#endif // __VER >= 8 // 8�� ��� 061226 ma
				
				pUser->AddDefinedText( TID_GAME_STATE_NOTSUMMONOK , "\"%s\"", prj.GetText( dwMsgId ) );
				pUsertmp->AddDefinedText( TID_GAME_STATE_NOTSUMMON , "\"%s\"", prj.GetText( dwMsgId - 1 ) );
				return;
			}

			WORD wId	= LOWORD( dwData );
			WORD wMode	= HIWORD( dwData );
			CItemElem* pItemElem = (CItemElem*)pUsertmp->GetItemId( wId );
			if( IsUsableItem( pItemElem ) )
			{
				if( ( pItemElem->m_dwItemId != II_SYS_SYS_SCR_FRIENDSUMMON_A && pItemElem->m_dwItemId != II_SYS_SYS_SCR_FRIENDSUMMON_B ) || pItemElem->m_bQuery == FALSE )
					return;

				if( prj.IsGuildQuestRegion( pUser->GetPos() ) )
				{
					pUser->AddDefinedText( TID_GAME_STATE_NOTSUMMONOK , "\"%s\"", prj.GetText( TID_GAME_EVENT_WORLD_NOTUSE ) );
					pUsertmp->AddDefinedText( TID_GAME_STATE_NOTSUMMON , "\"%s\"", prj.GetText( TID_GAME_EVENT_WORLD_NOTUSE1 ) );
				}
				else if( prj.IsGuildQuestRegion( pUsertmp->GetPos() ) )
				{
					pUser->AddDefinedText( TID_GAME_STATE_NOTSUMMONOK , "\"%s\"", prj.GetText( TID_GAME_EVENT_WORLD_NOTUSE1 ) );
					pUsertmp->AddDefinedText( TID_GAME_STATE_NOTSUMMON , "\"%s\"", prj.GetText( TID_GAME_EVENT_WORLD_NOTUSE ) );
				}
				else if( pUser->GetWorld()->GetID() != pUsertmp->GetWorld()->GetID()
#if __VER >= 14 // __INSTANCE_DUNGEON
						|| pUser->GetLayer() != pUsertmp->GetLayer()
#endif // __INSTANCE_DUNGEON
					)
				{
					CString strtmp;
					strtmp.Format( prj.GetText( TID_GAME_WORLD_NOTUSE ), pUser->GetWorld()->m_szWorldName, pUsertmp->GetWorld()->m_szWorldName );
					pUser->AddDefinedText( TID_GAME_STATE_NOTSUMMONOK, "\"%s\"", strtmp );
					strtmp.Format( prj.GetText( TID_GAME_WORLD_NOTUSE ), pUsertmp->GetWorld()->m_szWorldName, pUser->GetWorld()->m_szWorldName );
					pUsertmp->AddDefinedText( TID_GAME_STATE_NOTSUMMON, "\"%s\"", strtmp );
				}
#ifdef __QUIZ
				else if( pUser->GetWorld()->GetID() == WI_WORLD_QUIZ 
						|| pUsertmp->GetWorld()->GetID() == WI_WORLD_QUIZ )
					pUser->AddDefinedText( TID_GAME_QUIZ_DO_NOT_USE );
#endif // __QUIZ
				else 
				{
					g_dpDBClient.SendLogSMItemUse( "1", pUsertmp, pItemElem, pItemElem->GetProp(), pUser->GetName() );

					pUser->REPLACE( g_uIdofMulti, pUsertmp->GetWorld()->GetID(), pUsertmp->GetPos(), REPLACE_NORMAL, pUsertmp->GetLayer() );

					pItemElem->m_bQuery		= FALSE;
					pUsertmp->RemoveItem( (BYTE)( wId ), (short)1 );
					pUser->AddDefinedText( TID_GAME_SUMMON_SUCCESS1, "\"%s\"", pUsertmp->GetName() );
					pUsertmp->AddDefinedText( TID_GAME_SUMMON_SUCCESS, "\"%s\"", pUser->GetName() );
				}
			}
			else
			{
				ItemProp* pItemProp = prj.GetItemProp( II_SYS_SYS_SCR_FRIENDSUMMON_A );
				if( pItemProp )
					pUsertmp->AddDefinedText( TID_ERROR_SUMMONFRIEND_NOITEM, "\"%s\" \"%s\"", pItemProp->szName, pUser->GetName() );
			}
		}
	}
}

void CDPSrvr::OnSummonFriendCancel( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	OBJID objid;
	DWORD dwData;
	ar >> objid >> dwData;
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		CUser* pUsertmp = prj.GetUser( objid );
		if( IsValidObj( (CObj*)pUsertmp ) )
		{
			WORD wId	= LOWORD( dwData );
			CItemElem* pItemElem = (CItemElem*)pUsertmp->GetItemId( wId );
			if( IsUsableItem( pItemElem ) && ( pItemElem->m_dwItemId == II_SYS_SYS_SCR_FRIENDSUMMON_A || pItemElem->m_dwItemId == II_SYS_SYS_SCR_FRIENDSUMMON_B ) )
				pItemElem->m_bQuery		= FALSE;
			pUsertmp->AddDefinedText( TID_GAME_SUMMON_FRIEND_CANCEL, "\"%s\"", pUser->GetName() );
		}
	}
}

void CDPSrvr::OnSummonParty( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	DWORD dwData;
	
	ar >> dwData;

	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		int nState = pUser->GetSummonState();
		if( nState != 0 )
		{
			DWORD dwMsgId = 0;
			if( nState == 1 )	// �ŷ���
				dwMsgId = TID_GAME_TRADE_NOTUSE;
			else if( nState == 2 ) // ����
				dwMsgId = TID_GAME_DIE_NOTUSE;
			else if( nState == 3 ) // ���λ��� ��
				dwMsgId = TID_GAME_VENDOR_NOTUSE;
			else if( nState == 4 ) // ������
				dwMsgId = TID_GAME_ATTACK_NOTUSE;
			else if( nState == 5 ) // ������
				dwMsgId = TID_GAME_FLY_NOTUSE;
#if __VER >= 8 // 8�� ��� 061226 ma
			else if( nState == 6 ) // �����
				dwMsgId = TID_GAME_ATTACK_NOTUSE;
#endif // __VER >= 8 // 8�� ��� 061226 ma
			
			pUser->AddDefinedText( TID_GAME_STATE_NOTUSE, "\"%s\"", prj.GetText( dwMsgId ) );
			return;
		}

		if( prj.IsGuildQuestRegion( pUser->GetPos() ) )
		{
			pUser->AddDefinedText( TID_GAME_STATE_NOTUSE, "\"%s\"", prj.GetText( TID_GAME_EVENT_WORLD_NOTUSE ) );
			return;
		}
#if __VER >= 13 // __RAINBOW_RACE
		if( CRainbowRaceMng::GetInstance()->IsEntry( pUser->m_idPlayer ) )
		{
			pUser->AddDefinedText( TID_GAME_RAINBOWRACE_NOTELEPORT );
			return;
		}
#endif // __RAINBOW_RACE

		
		WORD wId	= LOWORD( dwData );
		WORD wMode	= HIWORD( dwData );
		CItemElem* pItemElem = (CItemElem*)pUser->GetItemId( wId );
		if( IsUsableItem( pItemElem ) )
		{
			CParty* pParty;
			pParty = g_PartyMng.GetParty( pUser->GetPartyId() );
			if( pParty && pParty->IsLeader( pUser->m_idPlayer ) )
			{
				if( !pUser->HasBuff( BUFF_ITEM, (WORD)( pItemElem->GetProp()->dwID ) ) )
				{
					pParty->m_dwWorldId = pUser->GetWorld()->GetID();

					g_dpDBClient.SendLogSMItemUse( "1", pUser, pItemElem, pItemElem->GetProp(), pParty->m_sParty );
					
					ItemProp* pItemProptmp = prj.GetItemProp( II_SYS_SYS_SCR_PARTYSUMMON );
					for( int i = 1 ; i < pParty->m_nSizeofMember ; i++ )
					{
						CUser* pUsertmp		= g_UserMng.GetUserByPlayerID( pParty->GetPlayerId( i ) );
						if( IsValidObj( (CObj*)pUsertmp ) )
						{
//								if( pUser->GetWorld()->GetID() == pUsertmp->GetWorld()->GetID() && !prj.IsGuildQuestRegion( pUsertmp->GetPos() ) )
							{
								pUser->DoApplySkill( (CCtrl*)pUsertmp, pItemElem->GetProp(), NULL );
								pUsertmp->AddSummonPartyConfirm( pUser->GetId(), dwData, pUser->GetWorld()->m_szWorldName );
								if( pItemProptmp )
									g_UserMng.AddCreateSfxObj((CMover *)pUsertmp, pItemProptmp->dwSfxObj3 );
								pUser->AddDefinedText( TID_GAME_SUMMONFRIEND_CONFIRM, "\"%s\"", pUsertmp->GetName() );
							}
//								else
//								{
//									pUser->AddDefinedText( TID_ERROR_SUMMONFRIEND_NOTWORLD );
//								}
						}
						else
						{
#if __VER >= 11 // __SYS_PLAYER_DATA
							pUser->AddDefinedText( TID_ERROR_SUMMONFRIEND_NOUSER, "\"%s\"", CPlayerDataCenter::GetInstance()->GetPlayerString( pParty->GetPlayerId( i ) ) );
#else	// __SYS_PLAYER_DATA
							pUser->AddDefinedText( TID_ERROR_SUMMONFRIEND_NOUSER, "\"%s\"", prj.GetPlayerString( pParty->GetPlayerId( i ) ) );
#endif	// __SYS_PLAYER_DATA
						}
					}
					pUser->DoApplySkill( (CCtrl*)pUser, pItemElem->GetProp(), NULL );
					pUser->RemoveItem( (BYTE)( wId ), (short)1 );

					if( pItemProptmp )
						g_UserMng.AddCreateSfxObj((CMover *)pUser, pItemProptmp->dwSfxObj3 );
				}
				else
				{
					pUser->AddDefinedText( TID_GAME_LIMITED_USE );
				}
			}
		}	
		else
		{
			ItemProp* pItemProp = prj.GetItemProp( II_SYS_SYS_SCR_PARTYSUMMON );
			if( pItemProp )
				pUser->AddDefinedText( TID_ERROR_SUMMONPARTY_NOITEM, "\"%s\"", pItemProp->szName );
		}
	}	
}

void CDPSrvr::OnSummonPartyConfirm( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	OBJID objid;
	DWORD dwData;
	ar >> objid;
	ar >> dwData;
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		CParty* pParty;
		pParty = g_PartyMng.GetParty( pUser->GetPartyId() );
		if( pParty )
		{
			CMover* pLeader = prj.GetUser( objid );
			if( IsValidObj( pLeader ) && pParty->IsLeader( pLeader->m_idPlayer ) && pLeader->HasBuff( BUFF_ITEM, II_SYS_SYS_SCR_PARTYSUMMON ) )
			{
				if( pUser == pLeader )
					return;
				int nState = pUser->GetSummonState();
				if( nState != 0 && nState != 5 )
				{
					DWORD dwMsgId = 0;
					if( nState == 1 )	// �ŷ���
						dwMsgId = TID_GAME_TRADE_NOTUSE;
					else if( nState == 2 ) // ����
						dwMsgId = TID_GAME_DIE_NOTUSE;
					else if( nState == 3 ) // ���λ��� ��
						dwMsgId = TID_GAME_VENDOR_NOTUSE;
					else if( nState == 4 ) // ������
						dwMsgId = TID_GAME_ATTACK_NOTUSE;
#if __VER >= 8 // 8�� ��� 061226 ma
					else if( nState == 6 ) // �����
						dwMsgId = TID_GAME_ATTACK_NOTUSE;
#endif // __VER >= 8 // 8�� ��� 061226 ma
					
					pUser->AddDefinedText( TID_GAME_STATE_NOTSUMMONOK , "\"%s\"", prj.GetText( dwMsgId ) );
					return;
				}
				
				nState = pLeader->GetSummonState();
				if( nState != 0 )
				{
					DWORD dwMsgId = 0;
					if( nState == 1 )	// �ŷ���
						dwMsgId = TID_GAME_TRADE_NOTUSE1;
					else if( nState == 2 ) // ����
						dwMsgId = TID_GAME_DIE_NOTUSE1;
					else if( nState == 3 ) // ���λ��� ��
						dwMsgId = TID_GAME_VENDOR_NOTUSE1;
					else if( nState == 4 ) // ������
						dwMsgId = TID_GAME_ATTACK_NOTUSE1;
					else if( nState == 5 ) // ������
						dwMsgId = TID_GAME_FLY_NOTUSE1;
#if __VER >= 8 // 8�� ��� 061226 ma
					else if( nState == 6 ) // �����
						dwMsgId = TID_GAME_ATTACK_NOTUSE1;
#endif // __VER >= 8 // 8�� ��� 061226 ma
					
					pUser->AddDefinedText( TID_GAME_STATE_NOTSUMMONOK , "\"%s\"", prj.GetText( dwMsgId ) );
					return;
				}
				
				if( prj.IsGuildQuestRegion( pLeader->GetPos() ) )
				{
					pUser->AddDefinedText( TID_GAME_STATE_NOTSUMMONOK, "\"%s\"", prj.GetText( TID_GAME_EVENT_WORLD_NOTUSE1) );
				}
				else if( prj.IsGuildQuestRegion( pUser->GetPos() ) )
				{
					pUser->AddDefinedText( TID_GAME_STATE_NOTSUMMONOK, "\"%s\"", prj.GetText( TID_GAME_EVENT_WORLD_NOTUSE ) );
				}
				else if( pUser->GetWorld()->GetID() != pParty->m_dwWorldId )
				{
					CWorld* pWorld = g_WorldMng.GetWorld( pParty->m_dwWorldId );
					if( pWorld )
					{
						CString strtmp;
						strtmp.Format( prj.GetText( TID_GAME_WORLD_NOTUSE ), pUser->GetWorld()->m_szWorldName, pWorld->m_szWorldName );
						pUser->AddDefinedText( TID_GAME_STATE_NOTSUMMONOK, "\"%s\"", strtmp );
					}
				}
				else if( pParty->m_dwWorldId != pLeader->GetWorld()->GetID() )
				{
					CWorld* pWorld = g_WorldMng.GetWorld( pParty->m_dwWorldId );
					if( pWorld )
					{
						CString strtmp;
						strtmp.Format( prj.GetText( TID_GAME_WORLDLEADER_NOTUSE ) );
						pUser->AddDefinedText( TID_GAME_STATE_NOTSUMMONOK, "\"%s\"", strtmp );
					}
				}
#if __VER >= 14 // __INSTANCE_DUNGEON
				else if( pLeader->GetLayer() != pUser->GetLayer() )
				{
					CString strtmp;
					strtmp.Format( prj.GetText( TID_GAME_WORLD_NOTUSE ) );
					pUser->AddDefinedText( TID_GAME_STATE_NOTSUMMONOK, "\"%s\"", strtmp );
				}
#endif // __INSTANCE_DUNGEON
#if __VER >= 13 // __RAINBOW_RACE
				else if( CRainbowRaceMng::GetInstance()->IsEntry( pUser->m_idPlayer ) )
				{
					pUser->AddDefinedText( TID_GAME_RAINBOWRACE_NOTELEPORT );
				}
#endif // __RAINBOW_RACE
#ifdef __QUIZ
				else if( pLeader->GetWorld()->GetID() == WI_WORLD_QUIZ 
					|| pUser->GetWorld()->GetID() == WI_WORLD_QUIZ )
					pUser->AddDefinedText( TID_GAME_QUIZ_DO_NOT_USE );
#endif // __QUIZ
				else
				{
					pUser->RemoveBuff( BUFF_ITEM, II_SYS_SYS_SCR_PARTYSUMMON );
					pUser->REPLACE( g_uIdofMulti, pLeader->GetWorld()->GetID(), pLeader->GetPos(), REPLACE_FORCE, pLeader->GetLayer() );
					pUser->AddDefinedText( TID_GAME_SUMMON_SUCCESS1, "\"%s\"", pLeader->GetName() );
					((CUser*)pLeader)->AddDefinedText( TID_GAME_SUMMON_SUCCESS, "\"%s\"", pUser->GetName() );					
					ItemProp* pItemProptmp = prj.GetItemProp( II_SYS_SYS_SCR_PARTYSUMMON );
					if( pItemProptmp )
						g_UserMng.AddCreateSfxObj((CMover *)pUser, pItemProptmp->dwSfxObj3 );
				}
			}
			else
			{
				pUser->AddDefinedText( TID_ERROR_SUMMONPARTY_NOTTIME );
			}
		}
		else
		{
			pUser->AddDefinedText( TID_GAME_NOPARTY );
		}
	}
}

void CDPSrvr::OnRemoveInvenItem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	DWORD	dwId;
	int		nNum;
	ar >> dwId;
	ar >> nNum;

	if( nNum <= 0 )
		return;

	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) == FALSE )
		return;
	
	CItemElem* pItemElem = pUser->m_Inventory.GetAtId( dwId );
	if( IsUsableItem( pItemElem ) == FALSE )
		return;

	if( pUser->m_Inventory.IsEquip( dwId ) )
		return;
	
	if( pUser->IsUsing( pItemElem ) )
	{
		pUser->AddDefinedText( TID_GAME_CANNOT_DO_USINGITEM );
		return;
	}
	
	if( pItemElem->m_nItemNum < nNum )
		return ;

	if( pItemElem->IsUndestructable() )
		return;

	CString strNum;
	strNum.Format("%d", nNum );
	pUser->AddDefinedText( TID_GAME_SUCCESS_REMOVE_ITEM, "\"%s\" \"%s\"", pItemElem->GetProp()->szName, strNum );

	LogItemInfo aLogItem;
	aLogItem.Action = "*";
	aLogItem.SendName = pUser->GetName();
	aLogItem.RecvName = "GARBAGE";
	aLogItem.WorldId = pUser->GetWorld()->GetID();
	aLogItem.Gold = aLogItem.Gold2 = pUser->GetGold();
	OnLogItem( aLogItem, pItemElem, nNum );

	pUser->RemoveItem( (BYTE)( dwId ), (short)nNum );
}

#if __VER >= 12 // __ITEMCREATEMON_S0602
void CDPSrvr::OnCreateMonster( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	DWORD dwItemIdRec;
	D3DXVECTOR3 vPos;
	ar >> dwItemIdRec;
	ar >> vPos;

	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) == FALSE )
		return;
	
	DWORD dwId = HIWORD( dwItemIdRec );
	
	if( pUser->IsUsableState( dwId ) == FALSE )
		return;
	
#if __VER >= 12 // __NEW_ITEMCREATEMON_SERVER
	CCreateMonster::GetInstance()->CreateMonster( pUser, dwId, vPos );

#else // __NEW_ITEMCREATEMON_SERVER
	CItemElem* pItemElem = pUser->m_Inventory.GetAtId( dwId );
	if( IsUsableItem( pItemElem ) )
	{
		CWorld* pWorld	= pUser->GetWorld();
		MoverProp* pMoverProp	= NULL;

		DWORD dwMonsterId = g_CreateMonster.GetCreateMonster( pItemElem->GetProp()->dwID );
		if( dwMonsterId )
			pMoverProp	= prj.GetMoverProp( dwMonsterId );
				
		if( pWorld && pMoverProp && pMoverProp->dwID != 0 )
		{
			D3DXVECTOR3 vDist2 = pUser->GetPos() - vPos;
			float fDist = D3DXVec3Length( &vDist2 );			// ����ǥ���� �Ÿ�
			if( 15.f < fDist )
				return;

			int nAttr = pWorld->GetHeightAttribute( vPos.x, vPos.z );		// �̵��� ��ġ�� �Ӽ� ����.
			if( nAttr == HATTR_NOWALK || nAttr == HATTR_NOMOVE )		// �� �����̴� ���̸� Pass
				return;
			if( pUser->IsRegionAttr( RA_SAFETY ) )		// ���������̸� Pass
				return;
			if( pWorld->GetID() == WI_WORLD_GUILDWAR )
				return;

//			g_UserMng.AddCreateSfxObj((CMover *)pUser, XI_CHR_CURSOR2, vPos.x, vPos.y, vPos.z);
			for( DWORD dw = 0; dw < 1; dw++ )
			{
				CObj* pObj	= CreateObj( D3DDEVICE, OT_MOVER, pMoverProp->dwID );
				if( NULL == pObj )	return;	// ASSERT( pObj );
				pObj->SetPos( vPos );
				pObj->InitMotion( MTI_STAND );
				pObj->UpdateLocalMatrix();
			
				((CMover*)pObj)->m_bActiveAttack = 0;
				pWorld->AddObj( pObj, TRUE );
			}
			pUser->AddDefinedText( TID_GAME_CREATEMON_S, "\"%s\"", pMoverProp->szName );
			pUser->RemoveItem( dwId, (short)1 );
		}
	}
	else
	{
		// �������� ��� ����Ҽ� ����.
	}
	#endif // __NEW_ITEMCREATEMON_SERVER
}
#endif // __ITEMCREATEMON_S0602

void CDPSrvr::OnFoucusObj( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	OBJID objid;
	ar >> objid;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
		pUser->AddFocusObj(objid);
}

void CDPSrvr::OnTrade( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	OBJID objidTrader;
	ar >> objidTrader;

	CWorld* pWorld;
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) && ( pWorld = pUser->GetWorld() ) && pUser->m_vtInfo.GetOther() == NULL )
	{
		CMover* pTrader		= prj.GetMover( objidTrader );
		if( IsValidObj( pTrader ) && pTrader->GetWorld() && pTrader->m_vtInfo.GetOther() == NULL )
		{
			if( pTrader->IsPlayer() )	// pc
			{

#if __VER >= 8 // 8�� ��� 061226 ma
				if( 0 < pUser->m_nDuel ||  0 < pTrader->m_nDuel )
				{
					return;
				}
#endif // __VER >= 8 // 8�� ��� 061226 ma


				//���λ��� �߿��� �ŷ� �Ұ� 
				if( pUser->m_vtInfo.VendorIsVendor() || pUser->m_vtInfo.IsVendorOpen() ||
                    pTrader->m_vtInfo.VendorIsVendor() || pTrader->m_vtInfo.IsVendorOpen() )
				{
					return;	//
				}
#ifdef __S_SERVER_UNIFY
				if( pUser->m_bAllAction == FALSE || ((CUser*)pTrader)->m_bAllAction == FALSE )
					return;
#endif // __S_SERVER_UNIFY

				if( pUser->m_bBank || pTrader->m_bBank )
				{
					pUser->AddDefinedText( TID_GAME_TRADELIMITPC, "" );
					return;	//
				}

				if( pUser->m_bGuildBank || pTrader->m_bGuildBank )
				{
					pUser->AddDefinedText( TID_GAME_TRADELIMITPC, "" );
					return;	//
				}

				pUser->m_vtInfo.SetOther( pTrader );
				pTrader->m_vtInfo.SetOther( pUser );

				pUser->AddTrade( (CUser*)pTrader, pUser->m_idPlayer );
				( (CUser*)pTrader )->AddTrade( pUser, pUser->m_idPlayer );
			}
		}
	}
}

void CDPSrvr::OnConfirmTrade( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	OBJID objidTrader;
	ar >> objidTrader;

	CWorld* pWorld;
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) && ( pWorld = pUser->GetWorld() ) && pUser->m_vtInfo.GetOther() == NULL )
	{
		// �����忡���� �ŷ��� �Ҽ� �����ϴ�.
		if( pWorld && pWorld->GetID() == WI_WORLD_GUILDWAR )
		{			
			pUser->AddText( prj.GetText(TID_GAME_GUILDCOMBAT_CANNOT_TRADE) ); //�������� ������ �ŷ��� ���� ���͵��� �̿� �� �� �����ϴ�.
			return;
		}
#if __VER >= 11 // __GUILD_COMBAT_1TO1
		if( g_GuildCombat1to1Mng.IsPossibleUser( pUser ) )
		{
			pUser->AddText( prj.GetText(TID_GAME_GUILDCOMBAT_CANNOT_TRADE) ); //�������� ������ �ŷ��� ���� ���͵��� �̿� �� �� �����ϴ�.
			return;
		}
#endif // __GUILD_COMBAT_1TO1

		CMover* pTrader		= prj.GetMover( objidTrader );
		if( IsValidObj( pTrader ) && pTrader->GetWorld() && pTrader->m_vtInfo.GetOther() == NULL )
		{
			if( pTrader->IsPlayer() == FALSE )
				return;

#if __VER >= 8 // 8�� ��� 061226 ma
			if( 0 < pUser->m_nDuel ||  0 < pTrader->m_nDuel )
			{
				return;
			}
#endif // __VER >= 8 // 8�� ��� 061226 ma

			if( pTrader->IsAttackMode() )
				pUser->AddDefinedText( TID_GAME_BATTLE_NOTTRADE, "" );
			else
				((CUser*)pTrader)->AddComfirmTrade( pUser->GetId() );
		}

		pUser->RemoveInvisible();		// �ŷ��� �ϸ� ������ Ǯ����.
	}
}

void CDPSrvr::OnConfirmTradeCancel( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	OBJID objidTrader;
	ar >> objidTrader;

	CMover* pTrader		= prj.GetMover( objidTrader );
	if( IsValidObj( pTrader ) && pTrader->GetWorld() && pTrader->m_vtInfo.GetOther() == NULL )
		( (CUser*)pTrader )->AddComfirmTradeCancel( objidTrader );
}

void CDPSrvr::OnTradePut( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	BYTE i, nItemType, nId;
	short nItemNum;

	ar >> i >> nItemType >> nId >> nItemNum;
	if( i >= MAX_TRADE || nItemNum < 1 )
		return;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	CMover* pTrader	= ( pUser ? pUser->m_vtInfo.GetOther() : NULL );
	CWorld* pWorld;

	if( IsValidObj( pUser ) && ( pWorld = pUser->GetWorld() ) && 
		IsValidObj( pTrader ) && pTrader->GetWorld() == pWorld && pTrader->m_vtInfo.GetOther() == pUser )
	{
#if __VER >= 9 // JYJ
		CItemElem* pItem	= (CItemElem*)pUser->GetItemId( nId );
		if( pItem && pItem->IsFlag( CItemElem::expired ) )
			return;
#endif // __VER >= 9 // JYJ

		if( pUser->m_vtInfo.TradeGetState() == TRADE_STEP_ITEM && pTrader->m_vtInfo.TradeGetState() == TRADE_STEP_ITEM )
		{
			short nItemNumResult	= nItemNum;
			DWORD dwText	= pUser->m_vtInfo.TradeSetItem2( nId, i, nItemNumResult );
			if( dwText == 0 )
			{
				pUser->AddTradePut( pUser->GetId(), i, nItemType, nId, nItemNumResult );
				( (CUser*)pTrader )->AddTradePut( pUser->GetId(), i, nItemType, nId, nItemNumResult );
			}
			else
			{
				pUser->AddDefinedText( dwText );
			}
		}
		else
		{
			pUser->AddTradePutError();
		}
	}
}

void CDPSrvr::OnTradePull( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	BYTE i;
	ar >> i;

	if( i >= MAX_TRADE )
		return;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	CMover* pTrader	= ( pUser ? pUser->m_vtInfo.GetOther() : NULL );
	CWorld* pWorld;

	if( IsValidObj( pUser ) && ( pWorld = pUser->GetWorld() ) && 
		IsValidObj( pTrader ) && pTrader->GetWorld() == pWorld && pTrader->m_vtInfo.GetOther() == pUser )
	{

		if( pUser->m_vtInfo.TradeGetState() == TRADE_STEP_ITEM && pTrader->m_vtInfo.TradeGetState() == TRADE_STEP_ITEM )
		{
			if( pUser->m_vtInfo.TradeClearItem( i ) )
			{
				pUser->AddTradePull( pUser->GetId(), i );
				( (CUser*)pUser->m_vtInfo.GetOther() )->AddTradePull( pUser->GetId(), i );
			}
		}
	}
}

void CDPSrvr::OnTradePutGold( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	DWORD dwGold;
	ar >> dwGold;
	
	int nGold = dwGold;
	if( nGold <= 0 )
		return;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	CMover* pTrader	= ( pUser ? pUser->m_vtInfo.GetOther() : NULL );
	CWorld* pWorld;

	if( IsValidObj( pUser ) && ( pWorld = pUser->GetWorld() ) && 
		IsValidObj( pTrader ) && pTrader->GetWorld() == pWorld && pTrader->m_vtInfo.GetOther() == pUser )
	{
		if( pUser->m_vtInfo.TradeGetState() == TRADE_STEP_ITEM && pTrader->m_vtInfo.TradeGetState() == TRADE_STEP_ITEM )
		{
			if( nGold > pUser->GetGold() )
				nGold = pUser->GetGold();
			
			pUser->m_vtInfo.TradeSetGold( nGold );
			pUser->AddGold( -nGold, FALSE );	// raiders.2006.11.28  �κ��� = �κ��� - �ŷ�â �� 

			pUser->AddTradePutGold( pUser->GetId(), nGold );
			( (CUser*)pTrader )->AddTradePutGold( pUser->GetId(), nGold );
		}
	}
}
//raiders.2006.11.28
/*
void CDPSrvr::OnTradeClearGold( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	CMover* pTrader	= ( pUser ? pUser->m_vtInfo.GetOther() : NULL );
	CWorld* pWorld;

	if( IsValidObj( pUser ) && ( pWorld = pUser->GetWorld() ) && 
		IsValidObj( pTrader ) && pTrader->GetWorld() == pWorld && pTrader->m_vtInfo.GetOther() == pUser )
	{
		if( pUser->m_vtInfo.TradeGetState() == TRADE_STEP_ITEM && pTrader->m_vtInfo.TradeGetState() == TRADE_STEP_ITEM )
		{
			pUser->m_vtInfo.TradeSetGold( 0 );

			pUser->AddTradeClearGold( pUser->GetId() );
			( (CUser*)pTrader )->AddTradeClearGold( pUser->GetId() );
		}
	}
}
*/
void CDPSrvr::OnTradeCancel( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	int nMode;
	ar >> nMode;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	CMover* pTrader	= ( pUser ? pUser->m_vtInfo.GetOther() : NULL );
	CWorld* pWorld;

	if( IsValidObj( pUser ) && ( pWorld = pUser->GetWorld() ) &&
		IsValidObj( pTrader ) && pTrader->GetWorld() == pWorld && pTrader->m_vtInfo.GetOther() == pUser )
	{
		pUser->m_vtInfo.TradeClear();
		pTrader->m_vtInfo.TradeClear();

		pUser->AddTradeCancel( pUser->GetId(), pUser->m_idPlayer, nMode );
		( (CUser*)pTrader )->AddTradeCancel( pUser->GetId(), pUser->m_idPlayer, nMode );
	}
}


// pPlayer�� ���� ok�� ���� ������̰� pTrdaer�� ���� ok�� ���� ������̴�.	
void CDPSrvr::OnTradelastConfrim( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	CMover* pTrader	= ( pUser ? pUser->m_vtInfo.GetOther() : NULL );	
	CWorld* pWorld;

	if( IsValidObj( pUser ) && ( pWorld = pUser->GetWorld() ) && 
		IsValidObj( pTrader ) && pTrader->GetWorld() == pWorld && pTrader->m_vtInfo.GetOther() == pUser )
	{
		if( pUser->m_vtInfo.TradeGetState() != TRADE_STEP_OK )
			return;

		switch( pTrader->m_vtInfo.TradeGetState() )
		{
		case TRADE_STEP_OK:
			pUser->m_vtInfo.TradeSetState( TRADE_STEP_CONFIRM );	

			pUser->AddTradelastConfirmOk( pUser->GetId() );					// ok�� �������� ǥ���ϰ� �Ѵ�.
			( (CUser*)pTrader )->AddTradelastConfirmOk( pUser->GetId() );	// ok�� �������� ǥ���ϰ� �Ѵ�.
			break;

		case TRADE_STEP_CONFIRM:
			{
				BEFORESENDDUAL( out, PACKETTYPE_LOG_ALLITEM, DPID_UNKNOWN, DPID_UNKNOWN );		// log�� ar
				TRADE_CONFIRM_TYPE type = pUser->m_vtInfo.TradeLastConfirm( out );
				switch( type )
				{
				case TRADE_CONFIRM_ERROR:	
					pUser->AddTradeCancel( NULL_ID, pUser->m_idPlayer );	
					( (CUser*)pTrader )->AddTradeCancel( NULL_ID, pUser->m_idPlayer );
					break;
				case TRADE_CONFIRM_OK:
					SEND( out, &g_dpDBClient, DPID_SERVERPLAYER );

					pUser->AddTradeConsent();	
					( (CUser*)pTrader )->AddTradeConsent();
					break;
				}					
			}
			break;	
		} 
	}
}

// ����ڰ� ok ��ư�� ������ ��
void CDPSrvr::OnTradeOk( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	CMover* pTrader	= ( pUser ? pUser->m_vtInfo.GetOther() : NULL );
	CWorld* pWorld;

	if( IsValidObj( pUser ) && ( pWorld = pUser->GetWorld() ) && 
		IsValidObj( pTrader ) && pTrader->GetWorld() == pWorld && pTrader->m_vtInfo.GetOther() == pUser )
	{
		if( pUser->m_vtInfo.TradeGetState() == TRADE_STEP_ITEM )
		{
			pUser->m_vtInfo.TradeSetState( TRADE_STEP_OK ); 

			if( pTrader->m_vtInfo.TradeGetState() == TRADE_STEP_OK )	// ��밡 ���� ok�� ������ ��ȯ�� �����Ǵ� ���
			{
				( (CUser*)pUser )->AddTradelastConfirm();
				( (CUser*)pTrader )->AddTradelastConfirm();
			}
			else	// ���� ���� ok ��ư�� ���� ���
			{
				pUser->AddTradeOk( pUser->GetId() );				// Ŭ�� ������ ok�� �������� ǥ���ϰ� �Ѵ�.
				( (CUser*)pTrader )->AddTradeOk( pUser->GetId() );	// Ŭ�� ������ ok�� �������� ǥ���ϰ� �Ѵ�.
			}
		}
	}
}

void CDPSrvr::OnPVendorOpen( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	if( uBufSize > 55 )	// 4 + 4 + 48 - 1		= 55
		return;

	char szPVendor[MAX_VENDORNAME];	// ���� ���� �̸�( 48 )
	ar.ReadString( szPVendor, MAX_VENDORNAME );	

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
/*		
#ifdef __Y_BEAUTY_SHOP_CHARGE
		if( ::GetLanguage() == LANG_TWN )
		{
			if( !(pUser->HasBuff( BUFF_ITEM, II_SYS_SYS_SCR_VENDOR ) ) )
			{
				return;
			}
		}
#endif //__Y_BEAUTY_SHOP_CHARGE
*/

#ifdef __RULE_0615
		if( prj.IsInvalidName( szPVendor ) || prj.IsAllowedLetter( szPVendor, TRUE ) == FALSE )
			pUser->AddDiagText( prj.GetText( TID_DIAG_0020 ) ); 
#endif	// __RULE_0615

		if( !( pUser->HasBuff( BUFF_ITEM, II_SYS_SYS_SCR_FONTEDIT ) ) )
			ParsingEffect(szPVendor, strlen(szPVendor) );
		
		// �����忡���� ���λ����� ���� �����ϴ�.
		CWorld* pWorld = pUser->GetWorld();
		if( pWorld && pWorld->GetID() == WI_WORLD_GUILDWAR )
		{			
			pUser->AddText( prj.GetText(TID_GAME_GUILDCOMBAT_CANNOT_TRADE) ); //�������� ������ �ŷ��� ���� ���͵��� �̿� �� �� �����ϴ�.
			return;
		}
#if __VER >= 11 // __GUILD_COMBAT_1TO1
		if( g_GuildCombat1to1Mng.IsPossibleUser( pUser ) )
		{
			pUser->AddText( prj.GetText(TID_GAME_GUILDCOMBAT_CANNOT_TRADE) ); //�������� ������ �ŷ��� ���� ���͵��� �̿� �� �� �����ϴ�.
			return;
		}
#endif // __GUILD_COMBAT_1TO1
#if __VER >= 13 // __HOUSING
		if( pUser->GetWorld() && pUser->GetWorld()->GetID() == WI_WORLD_MINIROOM )
			return;
#endif // __HOUSING
		if( pUser->m_vtInfo.GetOther() )	// �ŷ��� �̸� ���λ��� �Ұ� 
			return;
#if __VER >= 8 // 8�� ��� 061226 ma
		if( 0 < pUser->m_nDuel )
		{
			return;
		}
#endif // __VER >= 8 // 8�� ��� 061226 ma

		if( pUser->IsAttackMode() )
			return;

		pUser->m_vtInfo.SetTitle( szPVendor );

		if( pUser->m_pActMover->IsFly() )
			return;

#if __VER >= 8 // __S8_PK
		if( pUser->IsChaotic() )
		{
			CHAO_PROPENSITY Propensity = prj.GetPropensityPenalty( pUser->GetPKPropensity() );
			if( !Propensity.nVendor )
			{
				pUser->AddDefinedText( TID_GAME_CHAOTIC_NOT_VENDOR );
				return;
			}
		}
#endif // __VER >= 8 // __S8_PK

#ifdef __S_SERVER_UNIFY
		if( pUser->m_bAllAction == FALSE )
			return;
#endif // __S_SERVER_UNIFY

		if( pUser->m_vtInfo.IsVendorOpen() )
		{
			if( pUser->m_vtInfo.VendorIsVendor() )	
			{
				g_ChattingMng.NewChattingRoom( pUser->m_idPlayer );
				CChatting * pChatting	= g_ChattingMng.GetChttingRoom( pUser->m_idPlayer );
				
				g_UserMng.AddPVendorOpen( pUser );
				if( pChatting )
				{
					pChatting->m_bState		= TRUE;
					if( pChatting->AddChattingMember( pUser->m_idPlayer ) )
						pUser->m_idChatting		= pUser->m_idPlayer;
					pUser->AddNewChatting( pChatting );
				}
#if __VER >= 13 // __HONORABLE_TITLE			// ����
				pUser->m_dwHonorCheckTime = GetTickCount();
#endif	// __HONORABLE_TITLE			// ����
			}
		}
	}
}

void CDPSrvr::OnPVendorClose( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	OBJID objidVendor;
	ar >> objidVendor;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		ClosePVendor( pUser, objidVendor );
	}	
}

BOOL CDPSrvr::ClosePVendor( CUser* pUser, OBJID objidVendor )
{
	if( pUser->GetId() == objidVendor )
	{
		if( pUser->m_vtInfo.IsVendorOpen() == FALSE && 
			pUser->m_vtInfo.VendorIsVendor() == FALSE )
		{
			return FALSE;
		}
		
		CMover* pTrader	= pUser->m_vtInfo.GetOther();   //raiders.2006.11.27

		pUser->m_vtInfo.VendorClose();
		
		//raiders.2006.11.27
		pUser->m_vtInfo.TradeClear();
		if( pTrader )
			pTrader->m_vtInfo.TradeClear();
		//--

		g_UserMng.AddPVendorClose( pUser );

		CChatting* pChatting = g_ChattingMng.GetChttingRoom( pUser->m_idChatting );
		if( pChatting )
		{
			for( int i = 0 ; i < pChatting->GetChattingMember() ; ++i )
			{
				CUser* pUserBuf = (CUser*)prj.GetUserByID( pChatting->m_idMember[i] );
				if( IsValidObj( pUserBuf ) )
				{
					// ä�ù��� ������
					pUserBuf->AddDeleteChatting();
					pUserBuf->m_idChatting	= 0;
				}
			}
		}
		g_ChattingMng.DeleteChattingRoom( pUser->m_idPlayer );
		pUser->m_idChatting		= 0;
	}
	else
	{
		if( IsValidObj( pUser->m_vtInfo.GetOther() ) )
		{
			pUser->m_vtInfo.SetOther( NULL );
			pUser->AddPVendorClose( objidVendor );

			CChatting* pChatting	= g_ChattingMng.GetChttingRoom( pUser->m_idChatting );
			if( pChatting )
			{
				for( int i = 0 ; i < pChatting->GetChattingMember() ; ++i )
				{
					CUser* pUserBuf = (CUser*)prj.GetUserByID( pChatting->m_idMember[i] );
					if( IsValidObj( pUserBuf ) )		// ä�ÿ��� ����
					{							
						pUserBuf->AddRemoveChatting( pUser->m_idPlayer );
					}
				}
				pChatting->RemoveChattingMember( pUser->m_idPlayer );
				pUser->m_idChatting		= 0;
			}
		}
	}

	return TRUE;
}

void CDPSrvr::OnBuyPVendorItem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	OBJID objidVendor;
	BYTE nItem;
	DWORD dwItemId;
	short nNum;

	ar >> objidVendor >> nItem >> dwItemId >> nNum;
	if( nItem >= MAX_VENDITEM || nNum <= 0 )
		return;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) == FALSE )
		return;

	CUser* pPVendor	= prj.GetUser( objidVendor );
	if( IsValidObj( pPVendor ) )
	{
		VENDOR_SELL_RESULT result;
		BOOL bOK = pPVendor->m_vtInfo.VendorSellItem( pUser, nItem, dwItemId, nNum, result );
		if( bOK )
		{
#if __VER < 11 // __MOD_VENDOR
			g_UserMng.AddPVendorItemNum( pPVendor, nItem, result.nRemain );
#endif	// __MOD_VENDOR

			LogItemInfo info;

			info.Action = "Z";
			info.SendName = pUser->GetName();
			info.RecvName = pPVendor->GetName();
			info.WorldId = pUser->GetWorld()->GetID();
			info.Gold = pUser->GetGold() + ( result.item.m_nCost * nNum );
			info.Gold2 = pUser->GetGold();
			info.Gold_1 = pPVendor->GetGold();
			OnLogItem( info, &result.item, nNum );

			info.Action = "X";
			info.SendName = pPVendor->GetName();
			info.RecvName = pUser->GetName();
			info.WorldId = pPVendor->GetWorld()->GetID();
			info.Gold = pPVendor->GetGold() - ( result.item.m_nCost * nNum );
			info.Gold2 = pPVendor->GetGold();
			info.Gold_1 = pUser->GetGold();
			OnLogItem( info, &result.item, nNum );
		}
		else
		{
			if( result.nErrorCode )
				pUser->AddDefinedText( result.nErrorCode, "" );
		}
	}
}

void CDPSrvr::OnQueryPVendorItem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	OBJID objidVendor;
	ar >> objidVendor;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		if( pUser->m_vtInfo.GetOther() )
			return;
		if( pUser->m_pActMover->IsFly() )
			return;

		CUser* pPVendor	= prj.GetUser( objidVendor );
		if( IsValidObj( pPVendor ) )
		{
			BOOL bChatting = TRUE;
			CChatting* pChatting = g_ChattingMng.GetChttingRoom( pPVendor->m_idChatting );
			if( pChatting )
				bChatting = pChatting->m_bState;

			if( pPVendor->m_vtInfo.IsVendorOpen() )
			{
				pUser->m_vtInfo.SetOther( pPVendor );
				pUser->AddPVendorItem( pPVendor, bChatting );
			}
		}

	}
}

void CDPSrvr::OnUnregisterPVendorItem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	BYTE i;
	ar >> i;
	if( i >= MAX_VENDITEM )
		return;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		if( pUser->m_vtInfo.GetOther() )
			return;
		if( pUser->m_vtInfo.IsVendorOpen() )
			return;

		if( pUser->m_vtInfo.VendorClearItem( i ) )
			pUser->AddUnregisterPVendorItem( i );
	}
}

void CDPSrvr::OnRegisterPVendorItem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	BYTE iIndex, nType, nId;
	short nNum;
	int nCost;
	ar >> iIndex >> nType >> nId >> nNum >> nCost;

#if __VER >= 8 //__S8_VENDOR_REVISION
	if( iIndex >= MAX_VENDOR_REVISION )
		return;
#else //__S8_VENDOR_REVISION
	if( iIndex >= MAX_VENDITEM )
		return;
#endif //__S8_VENDOR_REVISION
	if( nCost < 1 )	
		nCost = 1;

	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		if( pUser->m_vtInfo.GetOther() )
			return;
		if( pUser->m_vtInfo.IsVendorOpen() )
			return;

		if( pUser->IsRegionAttr( RA_FIGHT ) )
		{
			pUser->AddDefinedText( TID_GAME_FAIL_TO_OPEN_SHOP );
			return;
		}

#if __VER >= 13 // __HOUSING
		if( pUser->GetWorld() && pUser->GetWorld()->GetID() == WI_WORLD_MINIROOM )
			return;
#endif // __HOUSING

#if __VER >= 9	// __JEFF_9
		if( CNpcChecker::GetInstance()->IsCloseNpc( pUser->GetWorld(), pUser->GetPos() ) )
		{
			// NPC��ó ���� ���� �Ұ�(3m) - �õ� �� Ȯ��â�� ���� �ǵ��� ó��
			pUser->AddDiagText( prj.GetText( TID_GAME_NPC_RADIUS ) );
//			pUser->AddDefinedText( TID_GAME_NPC_RADIUS );
			return;
		}
#endif	// __JEFF_9

#ifdef __QUIZ
		if( pUser->GetWorld() && pUser->GetWorld()->GetID() == WI_WORLD_QUIZ )
		{
			pUser->AddDefinedText( TID_GAME_FAIL_TO_OPEN_SHOP );
			return;
		}
#endif // __QUIZ

		CItemElem* pItemElem = (CItemElem *)pUser->GetItemId( nId );
		if( IsUsableItem( pItemElem ) )		
		{
			if( pItemElem->IsQuest() )
				return;

			if( pItemElem->IsBinds() )
				return;
			if( pUser->IsUsing( pItemElem ) )
			{
				pUser->AddDefinedText( TID_GAME_CANNOT_DO_USINGITEM );
				return;
			}
			
			ItemProp* pProp	= pItemElem->GetProp();
			if( pProp->dwItemKind3 == IK3_CLOAK  && pItemElem->m_idGuild != 0 )
				return;

//			if( pItemElem->m_dwItemId == II_RID_RID_BOR_EVEINSHOVER || pItemElem->m_dwItemId == II_RID_RID_BOR_LADOLF )
//				return;
			if( pProp->dwParts == PARTS_RIDE && pProp->dwItemJob == JOB_VAGRANT )
				return;

			if( pUser->m_Inventory.IsEquip( pItemElem->m_dwObjId ) )
				return;

#if __VER >= 9 // JYJ
			if( pItemElem->IsFlag( CItemElem::expired ) )
				return;
			/*
			if(pProp->dwItemKind3 == IK3_EGG && pItemElem->m_pPet) //����� ���� �ŷ� �Ұ�
			{
				if(pItemElem->m_pPet->GetLife() <= 0)
					return;
			}
			*/
#endif // __VER >= 9 // JYJ
			
			if( nNum > pItemElem->m_nItemNum )
				nNum = pItemElem->m_nItemNum;
			if( nNum < 1 )	
				nNum = 1;

			ItemProp* pItemProp	= pItemElem->GetProp();

#if __VER < 8     //8�����ӳ��������ǸŰ�������Ǯ��
			if( (DWORD)nCost > (pItemProp->dwCost * 1000) )
				return;
#endif	//  __VER < 8  

			pUser->m_vtInfo.VendorSetItem( nId, iIndex, nNum, nCost );
			pUser->AddRegisterPVendorItem( iIndex, 0, nId, nNum, nCost );
		}
	}
}

#if __VER >= 8 //__CSC_VER8_5
void CDPSrvr::OnCreateAngel( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	int nOrichalcum, nMoonstone;
	ItemCountSet itemset[20];
	float greenAngelRate, whiteAngelRate, blueAngelRate, redAngelRate;
	FLOAT fRate[4];
	static DWORD adwItemId[4]	= { II_SYS_SYS_QUE_ANGEL_RED, II_SYS_SYS_QUE_ANGEL_BLUE, II_SYS_SYS_QUE_ANGEL_GREEN,  II_SYS_SYS_QUE_ANGEL_WHITE };

	BOOL isSummonWhite = FALSE;
	BOOL isSummonSuccess = FALSE;
	TCHAR sendstr[1024];
	TCHAR tempchr[10];
	CHAR temp[2];
//	TCHAR createAngel[12];
	int j, k, itemcount;
	CItemElem* pItemElem;

	ar.ReadString( sendstr, 1024 );
	CString infostr = sendstr;
	
	int strlen = infostr.GetLength();
	j = 0;
	k = 0;
	itemcount = 0;
	memset(tempchr, 0, sizeof(TCHAR)*10);
	
	for(int i=0; i<strlen; i++)
	{
		temp[0] = infostr.GetAt(i);
		temp[1] = NULL;
		if(strcmp(temp, "D") == 0)
		{
			CString tempstr;
			tempstr.Format("%s", tempchr);
			
			temp[0] = infostr.GetAt(i+1);
			temp[1] = NULL;
			if(strcmp(temp, "D") == 0)
			{
				if(k <= 20)
				{
					itemset[k].extracount = atoi(tempstr);
					memset(tempchr, 0, sizeof(TCHAR)*10);
					k++;
					i++;
					itemcount++;
					j = 0;
				}
				else
					return; //k���� MAX�� 20�� ���� �� ����.
			}
			else
			{
				itemset[k].itemid = atoi(tempstr);
				memset(tempchr, 0, sizeof(TCHAR)*10);
				j = 0;
			}
		}
		else
		{
			tempchr[j] = temp[0];
			j++;
		}
	}
	
	nOrichalcum = 0;
	nMoonstone = 0;

	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	if( !IsValidObj( pUser ) )
		return;
	
	for( int i=0; i<itemcount; i++)
	{
		pItemElem = (CItemElem*)pUser->GetItemId( itemset[i].itemid );
		if(pItemElem != NULL)
		{
			switch(pItemElem->GetProp()->dwID) 
			{
				case II_GEN_MAT_ORICHALCUM01:
				case II_GEN_MAT_ORICHALCUM01_1:
					nOrichalcum += itemset[i].extracount;
					break;
				case II_GEN_MAT_MOONSTONE:
				case II_GEN_MAT_MOONSTONE_1:
					nMoonstone += itemset[i].extracount;
					break;
			};
		}
		else
			return;
	}

	// R/B/G/W
	greenAngelRate = (nOrichalcum * 1.0f) + (nMoonstone * 1.0f);
	whiteAngelRate = greenAngelRate / 10.0f;
	blueAngelRate = greenAngelRate * 2.0f;
	redAngelRate = 100.0f - ( whiteAngelRate + greenAngelRate + blueAngelRate );
	
	fRate[0]	= redAngelRate;
	fRate[1]	= fRate[0] + blueAngelRate;
	fRate[2]	= fRate[1] + greenAngelRate;
	fRate[3]	= fRate[2] + whiteAngelRate;

	float rand = xRandom(1000) / 10.0f;
	DWORD dwItemId	= 0;
	for( int i = 0; i < 4; i++ )
	{
		if( rand <= fRate[i] )
		{
			dwItemId = adwItemId[i];
			break;
		}
	}	

	if( dwItemId > 0 )
	{
		if( IsValidObj(pUser) )
		{
			for( int i=0; i<itemcount; i++)
			{
				pItemElem = (CItemElem*)pUser->GetItemId( itemset[i].itemid );
				if( !IsUsableItem( pItemElem ) ) //�Һ�� �������� ��ȿ�� �˻�. ���� �� RETURN
					return;
			}

			if( pUser->m_Inventory.GetEmptyCount() < 1 )
			{
				pUser->AddDiagText(  prj.GetText( TID_GAME_LACKSPACE ) );
				return;
			}

			LogItemInfo aLogItem;
			aLogItem.Action = "&";
			aLogItem.SendName = pUser->GetName();
			aLogItem.RecvName = "ANGEL_MATERIAL";
			aLogItem.WorldId = pUser->GetWorld()->GetID();
			aLogItem.Gold = aLogItem.Gold2 = pUser->GetGold();
			for( int i=0; i<itemcount; i++)
			{
				pItemElem = (CItemElem*)pUser->GetItemId( itemset[i].itemid );
				if( pItemElem->GetExtra() > 0 )
					pItemElem->SetExtra(0);
//#ifdef __ANGEL_LOG				
				OnLogItem( aLogItem, pItemElem, itemset[i].extracount );	
//#endif // __ANGEL_LOG
				pUser->RemoveItem( (BYTE)( pItemElem->m_dwObjId ), itemset[i].extracount );	
			}

		}
		CItemElem itemElem;
		itemElem.m_dwItemId	= dwItemId;
		itemElem.m_nItemNum	= 1;
		if( pUser->CreateItem( &itemElem ) )
		{
//#ifdef __ANGEL_LOG
			LogItemInfo aLogItem;
			aLogItem.Action = "&";
			aLogItem.SendName = pUser->GetName();
			aLogItem.RecvName = "ANGEL_CREATE";
			aLogItem.WorldId = pUser->GetWorld()->GetID();
			aLogItem.Gold = aLogItem.Gold2 = pUser->GetGold();
			OnLogItem( aLogItem, &itemElem, 1 );
//#endif // __ANGEL_LOG
		}
	}
}

void CDPSrvr::OnAngleBuff( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	// ������ ���ְ� ������ȭ �κ� �ڸ��� ���ٸ� �޼��� ó��
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );

	if( IsValidObj( pUser ) )
	{
		if( pUser->HasBuffByIk3( IK3_ANGEL_BUFF ) == FALSE )
			return;

#ifdef __BUFF_1107
		IBuff* pBuff	= pUser->m_buffs.GetBuffByIk3( IK3_ANGEL_BUFF );
		if( !pBuff )
			return;
		ItemProp* pItemProp	= pBuff->GetProp();
#else	// __BUFF_1107
		LPSKILLINFLUENCE lpSkillIn = pUser->m_SkillState.GetItemBuf( IK3_ANGEL_BUFF );
		if( lpSkillIn == NULL )
			return;
		ItemProp* pItemProp = prj.GetItemProp( lpSkillIn->wID );
#endif	// __BUFF_1107
		if( pItemProp == NULL )
			return;

		int nAngel = (int)( (float)pItemProp->nAdjParamVal1 );
		if( nAngel <= 0 || 100 < nAngel  )
			nAngel = 100;

		EXPINTEGER nMaxAngelExp = prj.m_aExpCharacter[pUser->m_nAngelLevel].nExp1 / 100 * nAngel;
		if( pUser->m_nAngelExp < nMaxAngelExp )
			return;

		CItemElem itemElem;
		switch( pItemProp->dwID )
		{
		case II_SYS_SYS_QUE_ANGEL_RED:
			itemElem.m_dwItemId	= II_SYS_SYS_QUE_ANGEL_RED100;						
			break;
		case II_SYS_SYS_QUE_ANGEL_BLUE:
			itemElem.m_dwItemId	= II_SYS_SYS_QUE_ANGEL_BLUE100;
			break;
		case II_SYS_SYS_QUE_ANGEL_GREEN:
			itemElem.m_dwItemId	= II_SYS_SYS_QUE_ANGEL_GREEN100;
			break;
		default:
			itemElem.m_dwItemId	= II_SYS_SYS_QUE_ANGEL_WHITE100;
			break;
		}

		itemElem.m_nItemNum		= 1;
		if( pUser->CreateItem( &itemElem ) )
		{
			pUser->RemoveIk3Buffs( IK3_ANGEL_BUFF );

			LogItemInfo aLogItem;
			aLogItem.Action = "&";
			aLogItem.SendName = pUser->GetName();
			aLogItem.RecvName = "ANGEL_CREATE_COMPLETED";
			aLogItem.WorldId = pUser->GetWorld()->GetID();
			aLogItem.Gold = aLogItem.Gold2 = pUser->GetGold();
			OnLogItem( aLogItem, &itemElem, 1 );

			char szMessage[512] = {0,};
			sprintf( szMessage, prj.GetText( TID_EVE_REAPITEM ), itemElem.GetProp()->szName );
			pUser->AddText( szMessage );			
		}
		else
			pUser->AddDefinedText( TID_GAME_NOT_INVEN_ANGEL, "" );
	}
}
#endif //__CSC_VER8_5

#ifdef __EVE_MINIGAME
void CDPSrvr::OnKawibawiboStart( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	
	if( IsValidObj( pUser ) == FALSE )
		return;

	pUser->m_nKawibawiboState = prj.m_MiniGame.Result_Kawibawibo( pUser );
	if( pUser->m_nKawibawiboState == CMiniGame::KAWIBAWIBO_WIN )
	{
		CMiniGame::__KAWIBAWIBO Kawibawibo = prj.m_MiniGame.FindKawibawibo( pUser->m_nKawibawiboWin );
		CMiniGame::__KAWIBAWIBO KawibawiboNext = prj.m_MiniGame.FindKawibawibo( pUser->m_nKawibawiboWin + 1 );
		pUser->AddKawibawiboResult( pUser->m_nKawibawiboState, pUser->m_nKawibawiboWin, Kawibawibo.dwItemId, Kawibawibo.nItemCount, KawibawiboNext.dwItemId, KawibawiboNext.nItemCount );
	}
	else
		pUser->AddKawibawiboResult( pUser->m_nKawibawiboState, pUser->m_nKawibawiboWin );
}
void CDPSrvr::OnKawibawiboGetItem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	
	if( IsValidObj( pUser ) == FALSE )
		return;
	
	prj.m_MiniGame.ResultItem_Kawibawibo( pUser );
}

void CDPSrvr::OnReassembleStart( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	
	if( IsValidObj( pUser ) == FALSE )
		return;
	
	OBJID objItemId[9];
	
	for( int i=0; i<9; ++i )
		ar >> objItemId[i];
		
	BOOL nResult = prj.m_MiniGame.Result_Reassemble( pUser, objItemId, 9 );
	if( nResult == TRUE )
		prj.m_MiniGame.ResultItem_Reassemble( pUser );
		
}

void CDPSrvr::OnReassembleOpenWnd( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	
	if( IsValidObj( pUser ) == FALSE )
		return;

	prj.m_MiniGame.OpenWnd_Reassemble( pUser );
}

void CDPSrvr::OnAlphabetOpenWnd( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	
	if( IsValidObj( pUser ) == FALSE )
		return;

	prj.m_MiniGame.OpenWnd_Alphabet( pUser );
}

void CDPSrvr::OnAlphabetStart( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	
	if( IsValidObj( pUser ) == FALSE )
		return;
	
	int nQuestionID = 0;
	OBJID objItemId[5];

	ar >> nQuestionID;
	for( int i=0; i<5; ++i )
		ar >> objItemId[i];

	int nResult = prj.m_MiniGame.Result_Alphabet( pUser, objItemId, 5, nQuestionID );
	if( nResult != CMiniGame::ALPHABET_FAILED && nResult != CMiniGame::ALPHABET_NOTENOUGH_MONEY )
		prj.m_MiniGame.ResultItem_Alphabet( pUser, nResult );
	else
		pUser->AddAlphabetResult( nResult );
//	else
}

void CDPSrvr::OnFiveSystemOpenWnd( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	
	if( IsValidObj( pUser ) == FALSE )
		return;
	
	prj.m_MiniGame.OpenWnd_FiveSystem( pUser );
}

void CDPSrvr::OnFiveSystemBet( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	
	if( IsValidObj( pUser ) == FALSE )
		return;

	int nBetNum;
	int nBetPenya;
	
	ar >> nBetNum;
	ar >> nBetPenya;

	int nResult = prj.m_MiniGame.Bet_FiveSystem( pUser, nBetNum, nBetPenya );
	if( nResult == CMiniGame::FIVESYSTEM_NOTENOUGH )
	{
		pUser->AddFiveSystemResult( nResult );
	}
	else if( nResult == CMiniGame::FIVESYSTEM_OVERMAX )
	{
		pUser->AddFiveSystemResult( nResult );
	}
	else if( nResult == CMiniGame::FIVESYSTEM_FAILED )
	{
		pUser->AddFiveSystemResult( nResult );
	}
}

void CDPSrvr::OnFiveSystemStart( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	
	if( IsValidObj( pUser ) == FALSE )
		return;

	int nResult = prj.m_MiniGame.Result_FiveSystem( pUser );

	if( !( prj.m_MiniGame.ResultPenya_FiveSystem( pUser, nResult ) ) )
		pUser->AddFiveSystemResult( CMiniGame::FIVESYSTEM_FAILED );
}	

void CDPSrvr::OnFiveSystemDestroyWnd( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	
	if( IsValidObj( pUser ) == FALSE )
		return;
	
	prj.m_MiniGame.DestroyWnd_FiveSystem( pUser );
}
#endif // __EVE_MINIGAME

#if __VER >= 9 // __ULTIMATE
void CDPSrvr::OnUltimateMakeItem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	
	if( IsValidObj( pUser ) == FALSE )
		return;

	OBJID objItemId[MAX_JEWEL];
//	for( int i=0; i<MAX_JEWEL; i++ )
//		ar >> objItemId[i];
	ar.Read( objItemId, sizeof(OBJID) * MAX_JEWEL );

	int nResult = prj.m_UltimateWeapon.MakeItem( pUser, objItemId );
	pUser->AddUltimateMakeItem( nResult );
}

void CDPSrvr::OnUltimateMakeGem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	
	if( IsValidObj( pUser ) == FALSE )
		return;
	
	OBJID objItemId;
	ar >> objItemId;

	int nNum;
	int nResult = prj.m_UltimateWeapon.MakeGem( pUser, objItemId, nNum );
	pUser->AddUltimateMakeGem( nResult, nNum );
}

void CDPSrvr::OnUltimateTransWeapon( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	
	if( IsValidObj( pUser ) == FALSE )
		return;

	OBJID objItemWeapon;
	OBJID objItemGem1;
	OBJID objItemGem2;

	ar >> objItemWeapon;
	ar >> objItemGem1;
	ar >> objItemGem2;

	int nResult = prj.m_UltimateWeapon.TransWeapon( pUser, objItemWeapon, objItemGem1, objItemGem2 );
	pUser->AddUltimateWeapon( ULTIMATE_TRANSWEAPON, nResult );
}

void CDPSrvr::OnUltimateSetGem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	
	if( IsValidObj( pUser ) == FALSE )
		return;

	OBJID objItemWeapon;
	OBJID objItemGem;

	ar >> objItemWeapon;
	ar >> objItemGem;

	int nResult = prj.m_UltimateWeapon.SetGem( pUser, objItemWeapon, objItemGem );
	pUser->AddUltimateWeapon( ULTIMATE_SETGEM, nResult );
}

void CDPSrvr::OnUltimateRemoveGem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	
	if( IsValidObj( pUser ) == FALSE )
		return;
	
	OBJID objItemWeapon;
	OBJID objItemGem;
	
	ar >> objItemWeapon;
	ar >> objItemGem;
	
	int nResult = prj.m_UltimateWeapon.RemoveGem( pUser, objItemWeapon, objItemGem );

	if(nResult == CUltimateWeapon::ULTIMATE_SUCCESS)
	{
		//���� �޼��� ���
		pUser->AddDefinedText( TID_GAME_REMOVEGEM_SUCCESS, "" );
		pUser->AddPlaySound( SND_INF_UPGRADESUCCESS );		
		if((pUser->IsMode( TRANSPARENT_MODE ) ) == 0)
			g_UserMng.AddCreateSfxObj((CMover *)pUser, XI_INT_SUCCESS, pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z);		
	}
	else if(nResult == CUltimateWeapon::ULTIMATE_FAILED)
	{
		// ���� �޼��� ���
		pUser->AddDefinedText( TID_GAME_REMOVEGEM_FAILED, "" );
		pUser->AddPlaySound( SND_INF_UPGRADEFAIL );
		if((pUser->IsMode( TRANSPARENT_MODE ) ) == 0)
			g_UserMng.AddCreateSfxObj((CMover *)pUser, XI_INT_FAIL, pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z);				
	}
	pUser->AddUltimateWeapon( ULTIMATE_REMOVEGEM, nResult );
}

void CDPSrvr::OnUltimateEnchantWeapon( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	
	if( IsValidObj( pUser ) == FALSE )
		return;

	OBJID objItemWeapon;
	OBJID objItemGem;
	
	ar >> objItemWeapon;
	ar >> objItemGem;

	int nResult = prj.m_UltimateWeapon.EnchantWeapon( pUser, objItemWeapon, objItemGem );

	if(nResult == CUltimateWeapon::ULTIMATE_SUCCESS)
	{
		//���� �޼��� ���
		pUser->AddDefinedText( TID_UPGRADE_SUCCEEFUL, "" );
		pUser->AddPlaySound( SND_INF_UPGRADESUCCESS );		
		if((pUser->IsMode( TRANSPARENT_MODE ) ) == 0)
			g_UserMng.AddCreateSfxObj((CMover *)pUser, XI_INT_SUCCESS, pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z);		
	}
	else if(nResult == CUltimateWeapon::ULTIMATE_FAILED)
	{
		// ���� �޼��� ���
		pUser->AddDefinedText( TID_UPGRADE_FAIL, "" );
		pUser->AddPlaySound( SND_INF_UPGRADEFAIL );
		if((pUser->IsMode( TRANSPARENT_MODE ) ) == 0)
			g_UserMng.AddCreateSfxObj((CMover *)pUser, XI_INT_FAIL, pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z);				
	}
	pUser->AddUltimateWeapon( ULTIMATE_ENCHANTWEAPON, nResult );
}
#endif // __ULTIMATE

#ifdef __TRADESYS
void CDPSrvr::OnExchange( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) == FALSE )
		return;

	int nMMIid;
	int nListNum;

	ar >> nMMIid;
	ar >> nListNum;

	int	nResult = prj.m_Exchange.ResultExchange( pUser, nMMIid, nListNum );

	pUser->AddExchangeResult( EXCHANGE_RESULT, nResult );
}
#endif // __TRADESYS

void CDPSrvr::InviteParty( u_long uLeaderid, u_long uMemberid, BOOL bTroup )
{
	CUser* pUser = g_UserMng.GetUserByPlayerID( uMemberid );
	CUser* pLeaderUser = g_UserMng.GetUserByPlayerID( uLeaderid );
	
	if( IsValidObj( pLeaderUser ) && IsValidObj( pUser ) )
	{
		// �����忡���� ��Ƽ�� �Ҽ� �����ϴ�.
		CWorld* pWorld = pUser->GetWorld();
		if( ( pWorld && pWorld->GetID() == WI_WORLD_GUILDWAR ) || pLeaderUser->GetWorld() && pLeaderUser->GetWorld()->GetID() == WI_WORLD_GUILDWAR )
		{			
			pLeaderUser->AddText( prj.GetText(TID_GAME_GUILDCOMBAT_CANNOT_PARTY) );// "�����ؾ��� : �������忡�� ��Ƽ�� �Ҽ� �����ϴ�" );
			return;
		}
#if __VER >= 11 // __GUILD_COMBAT_1TO1
		if( g_GuildCombat1to1Mng.IsPossibleUser( pUser ) )
		{
			pLeaderUser->AddText( prj.GetText(TID_GAME_GUILDCOMBAT_CANNOT_PARTY) );// "�����ؾ��� : �������忡�� ��Ƽ�� �Ҽ� �����ϴ�" );
			return;
		}
#endif // __GUILD_COMBAT_1TO1

#if __VER >= 8 // 8�� ��� 061226 ma
		if( 0 < pUser->m_nDuel ||  0 < pLeaderUser->m_nDuel )
		{
			return;
		}
#endif // __VER >= 8 // 8�� ��� 061226 ma
		
		if( pLeaderUser->m_nDuel == 2 )
		{			
			pLeaderUser->AddDefinedText( TID_GAME_PPVP_ADDPARTY, "" );		// �ش� ����߿� ��û ���Դ�.
		} 
		else
		{
			if( 0 < (CMover*)pUser->GetPartyId() )	// �̹� ��Ƽ�� ������
			{
				pLeaderUser->AddPartyRequestCancel( uLeaderid, uMemberid, 1 );
			}
			else
			{
				if( pUser->IsAttackMode() )
					pLeaderUser->AddDefinedText( TID_GAME_BATTLE_NOTPARTY, "" );
				else
					pUser->AddPartyRequest( pLeaderUser, pUser, bTroup );
			}
		}
	}
	else
	{
		if( IsValidObj( pLeaderUser ) )
			pLeaderUser->AddPartyRequestCancel( uLeaderid, uMemberid, 4 );
	}
}

void CDPSrvr::InviteCompany( CUser* pUser, OBJID objid )
{
#if __VER >= 12 // __SECRET_ROOM
	if( !IsInviteAbleGuild( pUser ) )
	{
		return;
	}
#endif // __SECRET_ROOM
	
	if( IsValidObj( pUser ) )
	{
		CUser* pUsertmp	= prj.GetUser( objid );
		if( IsValidObj( pUsertmp ) )
		{
			CGuild* pGuild	= g_GuildMng.GetGuild( pUser->m_idGuild );
			if( pGuild )
			{
				CGuildMember* pMember	= pGuild->GetMember( pUser->m_idPlayer );
				if( !pMember )
				{
					// is not member
					return;
				}
				if( !pGuild->IsCmdCap( pMember->m_nMemberLv, PF_INVITATION ) )
				{
					// have no power
					pUser->AddDefinedText( TID_GAME_GUILDINVAITNOTWARR );
					return;
				}
				CGuild* pGuildtmp	= g_GuildMng.GetGuild( pUsertmp->m_idGuild );
				if( pGuildtmp && pGuildtmp->IsMember( pUsertmp->m_idPlayer ) )
				{
					// is already guild member
					pUser->AddDefinedText( TID_GAME_COMACCEPTHAVECOM, "%s", pUsertmp->GetName( TRUE ) );
				}
				else
				{
#if __VER >= 8 // 8�� ��� 061226 ma
					if( 0 < pUsertmp->m_nDuel )
					{
						return;
					}
#endif // __VER >= 8 // 8�� ��� 061226 ma

					if( !pGuild->GetWar() )
					{
						if( pUsertmp->IsAttackMode() )
						{
							pUser->AddDefinedText( TID_GAME_BATTLE_NOTGUILD, "" );
						} 
						else
						{
							
							pUsertmp->m_idGuild	= 0;
							pUsertmp->AddGuildInvite( pGuild->m_idGuild, pUser->m_idPlayer );
							pUser->AddDefinedText( TID_GAME_COMACCEPTKINGPIN, "%s", pUsertmp->GetName( TRUE ) );
						}
					}
					else
					{
						pUser->AddDefinedText( TID_GAME_GUILDWARNOMEMBER );
					}
				}
			}
		}
	}	
}

#if __VER >= 12 // __SECRET_ROOM
BOOL CDPSrvr::IsInviteAbleGuild( CUser* pUser )
{
	CGuild* pGuild = pUser->GetGuild();
	if( !pGuild )
		return FALSE;

	// ������
	if( g_GuildCombatMng.m_nState != CGuildCombat::CLOSE_STATE )
	{
		if( g_GuildCombatMng.FindGuildCombatMember( pUser->m_idGuild ) &&
			g_GuildCombatMng.FindGuildCombatMember( pUser->m_idGuild )->bRequest )
		{
			pUser->AddDefinedText( TID_GAME_GUILDCOMBAT_NOT_INVITATION_GUILD );
			return FALSE;
		}
	}

	// 1:1 ������
	if( g_GuildCombat1to1Mng.m_nState != CGuildCombat1to1Mng::GC1TO1_CLOSE )
	{
		if( g_GuildCombat1to1Mng.GetTenderGuildIndexByUser( pUser ) != NULL_ID )
		{
			pUser->AddDefinedText( TID_GAME_GUILDCOMBAT1TO1_NOT_INVITATION_GUILD );
			return FALSE;
		}
	}

	// ����� ��
	if( CSecretRoomMng::GetInstance()->m_nState != SRMNG_CLOSE )
	{
		map<BYTE, CSecretRoomContinent*>::iterator it = CSecretRoomMng::GetInstance()->m_mapSecretRoomContinent.begin();
		for( ; it!=CSecretRoomMng::GetInstance()->m_mapSecretRoomContinent.end(); it++ )
		{
			CSecretRoomContinent* pSRCont = it->second;
			if( pSRCont && ( pSRCont->GetTenderGuild( pGuild->GetGuildId() ) != NULL_ID ) )
			{
				pUser->AddDefinedText( TID_GAME_SECRETROOM_NOT_INVITATION_GUILD );
				return FALSE;
			}
		}
	}

	return TRUE;
}
#endif // __SECRET_ROOM

#if __VER < 12 // __EXT_PIERCING
BOOL CDPSrvr::IsPiercingSize( CUser* pUser, CItemElem* pItemElem0, CItemElem* pItemElem1, CItemElem* pItemElem2, int& nError )
{
	if( IsUsableItem( pItemElem0 ) == FALSE || IsUsableItem( pItemElem1 ) == FALSE )
		return FALSE;
	
	if( pUser->m_Inventory.IsEquip( pItemElem0->m_dwObjId ) )
	{
		nError = TID_GAME_EQUIPPUT;
		return FALSE;
	}	
	
	//////////////// ù��° ������ //////////////// 
	if( pItemElem0->GetProp()->dwItemKind3 != IK3_SUIT )
	{
		nError = TID_PIERCING_POSSIBLE_ITEM;
		return FALSE;
	}
		
	// �Ǿ�� ������ �˻� ������ 4���� �ְ�	
	if( pItemElem0->GetPiercingSize() >= MAX_PIERCING_SUIT )
	{
		nError = TID_PIERCING_POSSIBLE;
		return FALSE;
	}
	//////////////// �ι�° ������ //////////////// 
	// ���̽����� �˻�
	if( pItemElem1->GetProp()->dwItemKind3 != IK3_PIERDICE )
	{
		nError = TID_UPGRADE_ERROR_WRONGUPLEVEL;			
		return FALSE;					
	}
	
#if __VER >= 8 //__Y_NEW_ENCHANT
	if( pItemElem1->GetProp()->dwID != II_GEN_MAT_MOONSTONE 
		&& pItemElem1->GetProp()->dwID != II_GEN_MAT_MOONSTONE_1 )
#else //__Y_NEW_ENCHANT
	if( pItemElem1->GetProp()->dwID != II_GEN_MAT_DIE_EIGHT && pItemElem1->GetProp()->dwID != II_GEN_MAT_DIE_TEN )
#endif //__Y_NEW_ENCHANT
	{
		nError = TID_SBEVE_NOTUSEITEM;			// �Ǿ�̿� �ʿ��� �ֻ����� �ƴϸ� �Ұ���
		return FALSE;
	}
	//////////////// ����° ������ //////////////// 
	if( IsUsableItem( pItemElem2 ) && pItemElem2->m_dwItemId != II_SYS_SYS_SCR_PIEPROT )
	{
		nError = TID_SBEVE_NOTUSEITEM;			// ���������� �ƴϸ� �Ұ���
		return FALSE;
	}
	
	return TRUE;
}
#endif // __EXT_PIERCING

CCommonCtrl* CreateExpBox( CUser* pUser )
{
//#if __VER >= 9 // __S_9_ADD
//	return NULL;
//#endif // __S_9_ADD
	CWorld* pWorld = pUser->GetWorld();

	if( pWorld == NULL  )
		return NULL;

	///////////////////////////////////////////////////////////////////////
	// ����ġ�� ������ ������ ����� ����!!!
	CMover* pMover = (CMover*)pUser;
	
	float fRate = 0.1f, fDecExp = 0.0f;
	BOOL  bPxpClear = FALSE, bLvDown = FALSE;
	int   nLevel	= pMover->GetLevel();	
	pMover->GetDieDecExp( nLevel, fRate, fDecExp, bPxpClear, bLvDown );
	if( fDecExp )
	{
#if __VER >= 8 // __S8_PK
		pMover->GetDieDecExpRate( fDecExp, 0, FALSE );
#else // __VER >= 8 // __S8_PK
		pMover->GetDieDecExpRate( fDecExp, 0, pMover->m_nSlaughter );
#endif // __VER >= 8 // __S8_PK
	}

	// �ູ�� �η縶�� ���ÿ��� ����ġ ���� ������ �ʴ´�...

	if( pMover->IsSMMode( SM_REVIVAL ) )
		fDecExp = 0.0f;

	if( pMover->GetExp1() == 0 )
		fDecExp = 0.0f;	

	if( pMover->m_bLastPK || pMover->m_bGuildCombat || pMover->m_bLastDuelParty )		// ������ ����ġ �ȱ�´�...
		fDecExp = 0.0f;			
		
	if( fDecExp == 0.0f )
		return NULL;
	
	CCommonCtrl* pCtrl	= (CCommonCtrl*)CreateObj( D3DDEVICE, OT_CTRL, 46 );
	
	if( !pCtrl )
		return NULL;
	///////////////////////////////////////////////////////////////////////

	pCtrl->m_CtrlElem.m_dwSet    = UA_PLAYER_ID;
	pCtrl->m_idExpPlayer = pUser->m_idPlayer;
	
	EXPINTEGER	nDecExp = (EXPINTEGER)(prj.m_aExpCharacter[pUser->m_nLevel+1].nExp1 * fDecExp );	// ���緹���� �ִ����ġ * �ۼ�Ʈ

	if( nDecExp > pMover->GetExp1() )
		nDecExp = pMover->GetExp1();
	
	pCtrl->m_nExpBox     = (EXPINTEGER)(nDecExp * 0.3f);
	pCtrl->m_dwDelete    = timeGetTime() + MIN( 30 );
	
	pCtrl->SetPos( pUser->GetPos() );
	
	pUser->AddDefinedText(TID_GAME_EXPBOX_INFO_MSG, "" );
	pUser->AddChatText(   TID_GAME_EXPBOX_INFO_MSG, "");
	return pCtrl;
}

#if __VER >= 9	// __PET_0410
void CDPSrvr::OnPetRelease( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	=	g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		if( !pUser->HasActivatedSystemPet() )
		{
			pUser->AddDefinedText( TID_GAME_PET_NOT_FOUND );
		}
		else
		{
			CItemElem* pItemElem	= pUser->GetPetItem();
			if( pItemElem && pItemElem->m_pPet )
				pUser->PetRelease();
		}
	}
}

void CDPSrvr::OnUsePetFeed( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	=	g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		CPet* pPet	= pUser->GetPet();
		if( pPet == NULL )
		{
			pUser->AddDefinedText( TID_GAME_PET_NOT_FOUND );
			return;
		}
		DWORD dwFeedId;		// ���� �ĺ���
		short nNum;
		ar >> dwFeedId;
#if __VER < 12 // __PET_0519
		ar >> nNum;		// ���� ������ �������� ���ϵ��� ����
#endif	// __PET_0519
		CItemElem* pFeed	= static_cast<CItemElem*>( pUser->GetItemId( dwFeedId ) );
		if( IsUsableItem( pFeed ) == FALSE )
			return;
		if( !pFeed->IsFeed() )
			return;

#if __VER >= 12 // __PET_0519
		nNum	= pFeed->m_nItemNum;
#else	// __PET_0519
		if( nNum <= 0 )
			nNum	= 1;
		if( nNum > pFeed->m_nItemNum )
			nNum	= pFeed->m_nItemNum;
		if( nNum > 1000 )	// 0723
			nNum	= 1000;
#endif	// __PET_0519

		int nMaxNum	= 0;
		if( pPet->GetLevel() == PL_EGG )
		{
			nMaxNum	= MAX_PET_EGG_EXP - pPet->GetExp();
		}
		else
		{
			nMaxNum	= pPet->GetMaxEnergy() - pPet->GetEnergy();
			nMaxNum	/= 2;	// ���� 1�� ��� 2ȸ��	// 0723
		}

		if( nNum > nMaxNum )
			nNum	= nMaxNum;
		if( nNum == 0 )
			return;

		if( pPet->GetLevel() == PL_EGG )
		{
			pPet->SetExp( pPet->GetExp() + nNum );
			pUser->AddPetSetExp( pPet->GetExp() );
		}
		else
		{
			pPet->SetEnergy( pPet->GetEnergy() + nNum * 2 );	// ���� 1�� ��� 2ȸ��	// 0723
			g_UserMng.AddPetFeed( pUser, pPet->GetEnergy() );
		}
		pUser->UpdateItem( (BYTE)( pFeed->m_dwObjId ), UI_NUM, pFeed->m_nItemNum - nNum );
#if __VER >= 12 // __PET_0519
		pUser->AddDefinedText( TID_GAME_PETFEED_S01, "%d", nNum );
#endif	// __PET_0519

		// log
		CItemElem* pPetItem	= pUser->GetPetItem();
		g_dpDBClient.CalluspPetLog( pUser->m_idPlayer, pPetItem->GetSerialNumber(), nNum, PETLOGTYPE_FEED, pPet );
	}
}

void CDPSrvr::OnMakePetFeed( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	=	g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		DWORD dwMaterialId, dwToolId;
		short nNum;

		ar >> dwMaterialId >> dwToolId >> nNum;

		BOOL bTool	= FALSE;
		CItemElem* pTool	= NULL;
		if( dwToolId != NULL_ID )
		{
			pTool	= (CItemElem*)pUser->GetItemId( dwToolId );
			if( IsUsableItem( pTool ) )
			{
				if( pTool->m_dwItemId == II_SYS_SYS_FEED_MAKER )
					bTool	= TRUE;
			}
			if( !bTool )	// error
				return;
		}

		// �� ���̸ӿ� ���� ���� ���� ��, �� ���̸ӿ� ������ ���� ������ ����
		if( bTool == FALSE && CNpcChecker::GetInstance()->IsCloseNpc( MMI_PET_FOODMILL, pUser->GetWorld(), pUser->GetPos() ) == FALSE )
			return;

		CItemElem* pMaterial	= (CItemElem*)pUser->GetItemId( dwMaterialId );
		if( IsUsableItem( pMaterial ) == FALSE )
		{
			// error
			return;
		}
		ItemProp* pProp	= pMaterial->GetProp();
		if( pProp == NULL )
		{
			// error
			return;
		}

		if( pMaterial->IsCharged() )
		{
			pUser->AddDefinedText( TID_GAME_PET_FEED_CHARGED );
			return;
		}

		if( pProp->dwItemKind3 != IK3_GEM )
		{
			pUser->AddDefinedText( TID_GAME_PET_IS_NOT_FEED );
			return;
		}

		if( nNum <= 0 )
			nNum	= 1;
		if( nNum > pMaterial->m_nItemNum )
			nNum	= pMaterial->m_nItemNum;

		if( pUser->m_Inventory.IsEquip( dwMaterialId ) )
		{
			pUser->AddDefinedText( TID_GAME_PET_FEED_EQUIPED );
			return;
		}

		CItemElem itemElem;
		itemElem.m_dwItemId		= II_SYS_SYS_FEED_01;
		int nTotalFeed	= 0;
		int nPackMax	= itemElem.GetProp()->dwPackMax;
		for( int i = 0; i < nNum; i++ )
		{
			int nFeed	= CPetProperty::GetInstance()->GetFeedEnergy( pProp->dwCost, (int)bTool );
			if( nTotalFeed + nFeed > nPackMax )
			{
				nNum	= i;
				break;
			}
			nTotalFeed	+= nFeed;
		}
		itemElem.m_nItemNum		= nTotalFeed;
		itemElem.m_nHitPoint	= -1;

		int nResult = pUser->CreateItem( &itemElem );
		if( nResult )
		{
			//���� ����� �α� : pMaterial�� ������ ���� ���, itemElem.m_nItemNum�� ���� ������ ���� ��
			LogItemInfo aLogItem;
			aLogItem.Action = "~";
			aLogItem.SendName = pUser->GetName();
			aLogItem.RecvName = "PET_FOOD_MATERIAL_REMOVE";
			aLogItem.WorldId = pUser->GetWorld()->GetID();
			aLogItem.Gold = aLogItem.Gold2 = pUser->GetGold();
			OnLogItem( aLogItem, pMaterial, nNum );

			aLogItem.RecvName = "PET_FOOD_CREATE";
			OnLogItem( aLogItem, &itemElem, itemElem.m_nItemNum );

			pUser->UpdateItem( (BYTE)( pMaterial->m_dwObjId ), UI_NUM, pMaterial->m_nItemNum - nNum );
			if( bTool )
				pUser->UpdateItem( (BYTE)( pTool->m_dwObjId ), UI_NUM, pTool->m_nItemNum - 1 );
		}
		pUser->AddPetFoodMill(nResult, itemElem.m_nItemNum);
	}
}

void CDPSrvr::OnPetTamerMistake( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	DWORD dwId;
	CUser* pUser	=	g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		ar >> dwId;
		CItemElem* pItemElem	= (CItemElem*)pUser->GetItemId( dwId );
		if( pItemElem && pItemElem->m_dwItemId == II_SYS_SYS_SCR_PET_TAMER_MISTAKE ) // m_dwItemId
		{
			CPet* pPet	= pUser->GetPet();
			if( pPet )
			{
				if( pPet->GetLevel() >= PL_C && pPet->GetLevel() <= PL_S )
				{
					// ���� �������� ���� �ɷ�ġ�� ��ҵǸ�,
					// �� �ܰ� ����, ����ġ 100%�� ���ư���.
					pPet->SetAvailLevel( pPet->GetLevel(), 0 );		// �ɷ�ġ ���
					pPet->SetLevel( pPet->GetLevel() - 1 );
					pPet->SetExp( MAX_PET_EXP );
					// ���� ���� ����
					if( pUser->HasPet() )
						pUser->RemovePet();
					pUser->AddPet( pPet, PF_PET_LEVEL_DOWN );	// �
					g_UserMng.AddPetLevelup( pUser, MAKELONG( (WORD)pPet->GetIndex(), (WORD)pPet->GetLevel() ) );	// ��
					pUser->UpdateItem( (BYTE)( pItemElem->m_dwObjId ), UI_NUM, pItemElem->m_nItemNum - 1 );

					// log
					CItemElem* pPetItem		= pUser->GetPetItem();
					g_dpDBClient.CalluspPetLog( pUser->m_idPlayer, pPetItem->GetSerialNumber(), 0, PETLOGTYPE_MISTAKE, pPet );
				}
				else
				{
					pUser->AddDefinedText( TID_GAME_PET_BETWEEN_C_TO_S );
				}
			}
			else
			{
				pUser->AddDefinedText( TID_GAME_PET_NOT_FOUND );
			}
		}
		else
		{
			// error
		}
	}
}

void CDPSrvr::OnPetTamerMiracle( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	DWORD dwId;
	CUser* pUser	=	g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		ar >> dwId;
		CItemElem* pItemElem	= (CItemElem*)pUser->GetItemId( dwId );
		if( pItemElem && pItemElem->m_dwItemId == II_SYS_SYS_SCR_PET_TAMER_MIRACLE )
		{
			CPet* pPet	= pUser->GetPet();
			if( pPet )
			{
				if( pPet->GetLevel() >= PL_B && pPet->GetLevel() <= PL_S )
				{
					// ���� ������ �� �� �������� ���� �ɷ�ġ�� ��ҵǸ�,
					// �ٽ� �����ϰ� ��� �ȴ�.
					pPet->SetAvailLevel( pPet->GetLevel() - 1, 0 );		// �ɷ�ġ ���
					pPet->SetAvailLevel( pPet->GetLevel(), 0 );		// �ɷ�ġ ���
					// ���� �ɷ�ġ ���
					BYTE nAvailLevel	= CPetProperty::GetInstance()->GetLevelupAvailLevel( pPet->GetLevel() - 1 );
					pPet->SetAvailLevel( pPet->GetLevel() - 1, nAvailLevel );
					nAvailLevel	= CPetProperty::GetInstance()->GetLevelupAvailLevel( pPet->GetLevel() );
					pPet->SetAvailLevel( pPet->GetLevel(), nAvailLevel );

					if( pUser->HasPet() )
						pUser->RemovePet();
					pUser->AddPet( pPet, PF_PET_GET_AVAIL );	// �	// PF_PET_GET_AVAIL 
					g_UserMng.AddPetLevelup( pUser, MAKELONG( (WORD)pPet->GetIndex(), (WORD)pPet->GetLevel() ) );	// ��
					pUser->UpdateItem( (BYTE)( pItemElem->m_dwObjId ), UI_NUM, pItemElem->m_nItemNum - 1 );

					// log
					CItemElem* pPetItem		= pUser->GetPetItem();
					g_dpDBClient.CalluspPetLog( pUser->m_idPlayer, pPetItem->GetSerialNumber(), 0, PETLOGTYPE_MIRACLE, pPet );
				}
				else
				{
					pUser->AddDefinedText( TID_GAME_PET_BETWEEN_B_TO_S );
				}
			}
			else
			{
				pUser->AddDefinedText( TID_GAME_PET_NOT_FOUND );
			}
		}
		else
		{
			// error
		}
	}
}

void CDPSrvr::OnFeedPocketInactive( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	=	g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		if( pUser->HasBuff( BUFF_ITEM, II_SYS_SYS_SCR_PET_FEED_POCKET ) )
			pUser->RemoveBuff( BUFF_ITEM, II_SYS_SYS_SCR_PET_FEED_POCKET );
	}
}
#endif	// __PET_0410

#if __VER >= 9 // __CSC_VER9_2
void CDPSrvr::OnModifyStatus( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	int nStrCount, nStaCount, nDexCount, nIntCount;
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	
	if( IsValidObj( pUser ) == FALSE )
		return;

	ar >> nStrCount >> nStaCount >> nDexCount >> nIntCount;
	
	if((nStrCount < 0 || nStaCount < 0 || nDexCount < 0 || nIntCount < 0) ||
		(nStrCount + nStaCount + nDexCount + nIntCount <= 0)) //��� �˻� �� ���� 0������ ��� �ߴ�.
		return;

	if( pUser->m_nRemainGP >= (nStrCount + nStaCount + nDexCount + nIntCount) )
	{
		pUser->m_nStr += nStrCount;
		pUser->m_nSta += nStaCount;
		pUser->m_nDex += nDexCount;
		pUser->m_nInt += nIntCount;
		pUser->m_nRemainGP = pUser->m_nRemainGP - (nStrCount + nStaCount + nDexCount + nIntCount);
		pUser->AddSetState( pUser->m_nStr, pUser->m_nSta, pUser->m_nDex, pUser->m_nInt, pUser->m_nRemainGP );
#if __VER >= 13 // __HONORABLE_TITLE			// ����
		pUser->CheckHonorStat();
		pUser->AddHonorListAck();//09.02.12
		g_UserMng.AddHonorTitleChange( pUser, pUser->m_nHonor);
#endif	// __HONORABLE_TITLE			// ����
	}
}	
#endif //__CSC_VER9_2

#if __VER >= 10 // __LEGEND	//	9�� ���½ý���	Neuz, World, Trans
void CDPSrvr::OnLegendSkillStart( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	
	if( IsValidObj( pUser ) == FALSE )
		return;
	
	OBJID objItemId[5];
	
	for( int i=0; i<5; ++i )
		ar >> objItemId[i];

	if(pUser->IsHero() == FALSE)
		return;

	for( int i = 0; i < MAX_SKILL_JOB; i++ ) 
	{				
		LPSKILL lpSkill = &(pUser->m_aJobSkill[i]);
		if( lpSkill && lpSkill->dwSkill != NULL_ID )
		{
			ItemProp* pSkillProp    = prj.GetSkillProp( lpSkill->dwSkill );			
			if( pSkillProp == NULL )
				continue;
			if( pSkillProp->dwItemKind1 != JTYPE_HERO)
				continue;
			if( lpSkill->dwLevel > 4 )
			{
				pUser->AddLegendSkillResult(-1);
				return;
			}
		}
	}	

	CItemElem* pItemElem[5];
	// ��ġ�ϴ��� �˻� (�κ��丮���� �˻�)
	for( int i=0; i<5; i++ )
	{
		pItemElem[i]	= (CItemElem*)pUser->m_Inventory.GetAtId( objItemId[i] );
		if( IsUsableItem( pItemElem[i] ) == FALSE )
			return ;
	}

	if( pItemElem[0]->m_dwItemId != II_GEN_MAT_DIAMOND ||
		pItemElem[1]->m_dwItemId != II_GEN_MAT_EMERALD ||
		pItemElem[2]->m_dwItemId != II_GEN_MAT_SAPPHIRE ||
		pItemElem[3]->m_dwItemId != II_GEN_MAT_RUBY ||
		pItemElem[4]->m_dwItemId != II_GEN_MAT_TOPAZ )
		return;

	// ��� ��ġ�ϸ� ���� ������ ����
	for( int i=0; i<5 ; i++ )
	{
		LogItemInfo aLogItem;
		aLogItem.Action = "+";
		aLogItem.SendName = pUser->GetName();
		aLogItem.RecvName = "LEGENDSKILL_USE";
		aLogItem.WorldId = pUser->GetWorld()->GetID();
		OnLogItem( aLogItem, pItemElem[i], 1 );
		pUser->RemoveItem( (BYTE)( objItemId[i] ), (short)1 );
	}

	
	if( xRandom(1000) > 766 )
	{
		for( int i = 0; i < MAX_SKILL_JOB; i++ ) 
		{				
			LPSKILL lpSkill = &(pUser->m_aJobSkill[i]);
			if( lpSkill && lpSkill->dwSkill != NULL_ID )
			{
				ItemProp* pSkillProp    = prj.GetSkillProp( lpSkill->dwSkill );			
				if( pSkillProp == NULL )
					continue;
				if( pSkillProp->dwItemKind1 != JTYPE_HERO)
					continue;
				lpSkill->dwLevel++;

				g_dpDBClient.SendLogSkillPoint( LOG_SKILLPOINT_USE, 1, (CMover*)pUser, &(pUser->m_aJobSkill[i]) );
			}
		}	
		g_UserMng.AddCreateSfxObj((CMover *)pUser, XI_SYS_EXCHAN01, pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z);
		pUser->AddDoUseSkillPoint( &(pUser->m_aJobSkill[0]), pUser->m_nSkillPoint );
#ifdef __S_NEW_SKILL_2
		g_dpDBClient.SaveSkill( pUser );
#endif // __S_NEW_SKILL_2
		pUser->AddLegendSkillResult(TRUE);	
	}
	else
	{
		pUser->AddLegendSkillResult(FALSE);
	}
}
#endif	//__LEGEND	//	9�� ���½ý���	Neuz, World, Trans

#if __VER >= 11 // __GUILD_COMBAT_1TO1
void CDPSrvr::OnGC1to1TenderOpenWnd( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	
	if( IsValidObj( pUser ) == FALSE )
		return;

	g_GuildCombat1to1Mng.SendTenderGuildOpenWnd( pUser );
}

void CDPSrvr::OnGC1to1TenderView( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	
	if( IsValidObj( pUser ) == FALSE )
		return;

	g_GuildCombat1to1Mng.SendTenderGuildView( pUser );
}

void CDPSrvr::OnGC1to1Tender( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	
	if( IsValidObj( pUser ) == FALSE )
		return;
	
	int nPenya;
	ar >> nPenya;

	g_GuildCombat1to1Mng.SetTenderGuild( pUser, nPenya );
}

void CDPSrvr::OnGC1to1CancelTender( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	
	if( IsValidObj( pUser ) == FALSE )
		return;

	g_GuildCombat1to1Mng.SetCancelTenderGuild( pUser );
}

void CDPSrvr::OnGC1to1FailedTender( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	
	if( IsValidObj( pUser ) == FALSE )
		return;

	g_GuildCombat1to1Mng.SetFailedTenderGuild( pUser );
}


void CDPSrvr::OnGC1to1MemberLineUpOpenWnd( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	
	if( IsValidObj( pUser ) == FALSE )
		return;

	g_GuildCombat1to1Mng.SendMemberLineUpOpenWnd( pUser );
}

void CDPSrvr::OnGC1to1MemberLineUp( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	
	if( IsValidObj( pUser ) == FALSE )
		return;

	vector<u_long> vecMemberId;
	u_long nTemp;
	int nSize;

	ar >> nSize;

	if( nSize > g_GuildCombat1to1Mng.m_nMaxJoinPlayer )
		return;

	for( int i=0; i<nSize; i++ )
	{
		ar >> nTemp;
		vecMemberId.push_back( nTemp );
	}
	
	g_GuildCombat1to1Mng.SetMemberLineUp( pUser, vecMemberId );
}

void CDPSrvr::OnGC1to1TeleportToNPC( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	
	if( IsValidObj( pUser ) == FALSE )
		return;

	g_GuildCombat1to1Mng.SetTeleportToNPC( pUser );
}

void CDPSrvr::OnGC1to1TeleportToStage( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	
	if( IsValidObj( pUser ) == FALSE )
		return;

	g_GuildCombat1to1Mng.SetTeleportToStage( pUser );
}
#endif // __GUILD_COMBAT_1TO1

#if __VER >= 11 // __SYS_COLLECTING
void CDPSrvr::OnQueryStartCollecting( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	=	g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) ) 
	{
		if( pUser->IsDisguise() )
			return;
		pUser->StartCollecting();
	}
}

void CDPSrvr::OnQueryStopCollecting( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	=	g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) ) 
		pUser->StopCollecting();
}
#endif	// __SYS_COLLECTING

	
#if __VER >= 11 // __MA_VER11_04	// ��� â�� �α� ��� world,database,neuz
void CDPSrvr::OnQueryGuildBankLogList( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	bool bMK = FALSE;
	CUser* pUser	=	g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) ) 
	{
		BYTE byListType;
		ar >> byListType;
		CGuild *pGuild = pUser->GetGuild();

		if( pGuild )
		{
			CGuildMember* pGuildMember = pGuild->GetMember( pUser->m_idPlayer );
			if( pGuildMember )
			{
				if( pGuildMember->m_nMemberLv == GUD_KINGPIN || pGuildMember->m_nMemberLv == GUD_MASTER )
					g_dpDBClient.SendQueryGetGuildBankLogList( pUser->m_idPlayer, pGuild->m_idGuild,byListType );
			}
		}
		// ��尡 ���ų� ������� �ƴϸ� ��û �Ұ�
	}
}
#endif //__MA_VER11_04	// ��� â�� �α� ��� world,database,neuz
#if __VER >= 13 // __HONORABLE_TITLE			// ����
void CDPSrvr::OnHonorListReq( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	=	g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) ) 
	{
		pUser->AddHonorListAck();
	}
}
void CDPSrvr::OnHonorChangeReq( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	=	g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) ) 
	{
		int nChange;
		ar >> nChange;
		if( nChange > -1 && nChange < MAX_HONOR_TITLE )
		{
			int nNeed =  CTitleManager::Instance()->GetNeedCount( nChange,-1);
			if( nNeed < 0)
				return;
			if( pUser->GetHonorTitle(nChange) >= nNeed )
			{
				pUser->m_nHonor = nChange;
				g_UserMng.AddHonorTitleChange( pUser, nChange );
			}
		}
		else if(nChange == -1)
		{
			pUser->m_nHonor = nChange;
			g_UserMng.AddHonorTitleChange( pUser, nChange );
		}
	}
}
#endif	// __HONORABLE_TITLE			// ����

#if __VER >= 11 // __MA_VER11_05	// �ɸ��� ���� �ŷ� ��� world,database,neuz
void CDPSrvr::OnSealCharReq( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	=	g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) ) 
	{
		CGuild *pGuild = pUser->GetGuild();
		if( pGuild != NULL )
		{
			pUser->AddDefinedText( TID_GAME_SEALCHAR_NO_CLEANGUILD );
			return;
		}
		if( pUser->IsChaotic() )
		{
			pUser->AddDefinedText( TID_GAME_SEALCHAR_NO_CLEANEQUIP );
			return;
		}
		int nSize	= 0;
		for( int i=0; i< pUser->m_Inventory.GetMax(); ++i )
		{
			CItemElem* pItemElem = pUser->m_Inventory.GetAtId( i );
			if( IsUsableItem( pItemElem ) == FALSE )
				continue;
			if( pItemElem->m_nItemNum > 0 )
				nSize++;
		}
		
		if( nSize > 1 )
		{
			pUser->AddDefinedText( TID_GAME_SEALCHAR_NO_CLEANINVEN );
			return;
		}
		else
		{
			CItemElem* pItemElem = NULL;
			pItemElem = pUser->m_Inventory.GetAtItemId( II_SYS_SYS_SCR_SEAL );
			if( IsUsableItem( pItemElem ) == FALSE )
			{
				pUser->AddDefinedText( TID_GAME_SEALCHAR_NO_CLEANINVEN );
				return;
			}
			else if(pItemElem->m_nItemNum > 1)
			{
				pUser->AddDefinedText( TID_GAME_SEALCHAR_NO_CLEANINVEN );
				return;
			}
		}

		if( pUser->GetGold() > 0 )
		{
			pUser->AddDefinedText( TID_GAME_SEALCHAR_NO_CLEANEQUIP );
			return;
		}
		int nBankSize	= 0;
		for( int i=0; i< pUser->m_Bank[pUser->m_nSlot].GetMax(); ++i )
		{
			CItemElem* pItemElem = pUser->m_Bank[pUser->m_nSlot].GetAtId( i );
			if( IsUsableItem( pItemElem ) == FALSE )
				continue;
			if( pItemElem->m_nItemNum > 0 )
				nBankSize++;
		}
#if __VER >= 11 // __SYS_POCKET
		if( !pUser->m_Pocket.IsAllClean() )
		{
			pUser->AddDefinedText( TID_GAME_SEALCHAR_NO_CLEANBANK );
			return;
		}
#endif		
		if( nBankSize > 0 || pUser->m_dwGoldBank[pUser->m_nSlot] > 0)
		{
			pUser->AddDefinedText( TID_GAME_SEALCHAR_NO_CLEANBANK );
			return;
		}
		g_dpDBClient.SendQueryGetSealChar( pUser->m_idPlayer,pUser->m_playAccount.lpszAccount);
//		pUser->AddDefinedText( TID_GAME_SEALCHAR_NO_CHARSEND );
	}
}
void CDPSrvr::OnSealCharConmReq( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	OBJID objidSend;
	ar >> objidSend;

	CUser* pUser	=	g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) ) 
	{
		CGuild *pGuild = pUser->GetGuild();
		if( pGuild != NULL )
		{
			pUser->AddDefinedText( TID_GAME_SEALCHAR_NO_CLEANGUILD );
			return;
		}
		int nSize	= 0;
		for( int i=0; i< pUser->m_Inventory.GetMax(); ++i )
		{
			CItemElem* pItemElem = pUser->m_Inventory.GetAtId( i );
			if( IsUsableItem( pItemElem ) == FALSE )
				continue;
			if( pItemElem->m_nItemNum > 0 )
				nSize++;
		}

		CItemElem* pItemElemTrue = NULL;
		CItemElem* pItemElemtmp = NULL;
		
		if( nSize > 1 )
		{
			pUser->AddDefinedText( TID_GAME_SEALCHAR_NO_CLEANINVEN );
			return;
		}
		else
		{
			pItemElemTrue = pUser->m_Inventory.GetAtItemId( II_SYS_SYS_SCR_SEAL );
			if( IsUsableItem( pItemElemTrue ) == FALSE )
			{
				pUser->AddDefinedText( TID_GAME_SEALCHAR_NO_CLEANINVEN );
				return;
			}
			else if(pItemElemTrue->m_nItemNum > 1)
			{
				pUser->AddDefinedText( TID_GAME_SEALCHAR_NO_CLEANINVEN );
				return;
			}
			pItemElemtmp = pItemElemTrue;
		}

		if( pUser->GetGold() > 0 )
		{
			pUser->AddDefinedText( TID_GAME_SEALCHAR_NO_CLEANEQUIP );
			return;
		}
		int nBankSize	= 0;
		for( int i=0; i< pUser->m_Bank[pUser->m_nSlot].GetMax(); ++i )
		{
			CItemElem* pItemElem = pUser->m_Bank[pUser->m_nSlot].GetAtId( i );
			if( IsUsableItem( pItemElem ) == FALSE )
				continue;
			if( pItemElem->m_nItemNum > 0 )
				nBankSize++;
		}
#if __VER >= 11 // __SYS_POCKET
		if( !pUser->m_Pocket.IsAllClean() )
		{
			pUser->AddDefinedText( TID_GAME_SEALCHAR_NO_CLEANBANK );
			return;
		}
#endif
		if( nBankSize > 0 || pUser->m_dwGoldBank[pUser->m_nSlot] > 0)
		{
			pUser->AddDefinedText( TID_GAME_SEALCHAR_NO_CLEANBANK );
			return;
		}
	
#if __VER >= 11 // __SYS_PLAYER_DATA
		const char* lpszPlayer	= CPlayerDataCenter::GetInstance()->GetPlayerString( objidSend );
#else	// __SYS_PLAYER_DATA
		LPCSTR lpszPlayer = prj.GetPlayerString( objidSend );
#endif	// __SYS_PLAYER_DATA

		if( lpszPlayer == NULL )
		{
			pUser->AddDefinedText( TID_GAME_SEALCHAR_NO_CHARSEND );
			return;
		}


		CItemElem itemElemSend;
		itemElemSend.m_dwItemId = II_SYS_SYS_SCR_SEALCHARACTER;
		itemElemSend.m_nItemNum	= 1;
		itemElemSend.SetSerialNumber();

//		lstrcpy( itemElemSend.m_szItemText, pUser->GetName() );
		memcpy( itemElemSend.m_szItemText, pUser->GetName(),sizeof(itemElemSend.m_szItemText) );

		itemElemSend.m_nRepairNumber	= pUser->GetLevel();//nlevel
		itemElemSend.m_nHitPoint	= pUser->m_idPlayer;//m_idPlayer
		itemElemSend.m_nRepair	= pUser->GetJob();//njob

		itemElemSend.m_nResistAbilityOption	= pUser->GetRemainGP();//nPOINT

		itemElemSend.SetPiercingSize( 4 );
		itemElemSend.SetPiercingItem( 0, pUser->GetSta() );		//nSTA
		itemElemSend.SetPiercingItem( 1, pUser->GetStr() );		//nSTR
		itemElemSend.SetPiercingItem( 2, pUser->GetDex() );		//nDEX
		itemElemSend.SetPiercingItem( 3, pUser->GetInt() );		//nINT
		
		LogItemInfo aLogItem;
		aLogItem.RecvName = "SEALCHAR";
		g_dpDBClient.SendQueryPostMail( objidSend, 0, itemElemSend, 0, itemElemSend.GetProp()->szName, 	(char*)GETTEXT( TID_MMI_SEALCHARITEM ) );

		aLogItem.Action = "+";
		aLogItem.SendName = pUser->GetName();
		aLogItem.WorldId = pUser->GetWorld()->GetID();
		OnLogItem( aLogItem, &itemElemSend, 1 );


		ItemProp* pItemProp  = pItemElemtmp->GetProp();
		g_dpDBClient.SendLogSMItemUse( "1", pUser, pItemElemtmp, pItemProp );		
//		pItemElemtmp->UseItem();
		OBJID       dwTmpObjId = pItemElemtmp->m_dwObjId;
		pUser->RemoveItem( (BYTE)( dwTmpObjId ), (short)1 );

		pUser->ClearAllSMMode();
		pUser->RemoveAllBuff();

		g_dpDBClient.SendQueryGetSealCharConm( pUser->m_idPlayer);

//		pUser->UpdateItem( pItemElemtmp->m_dwObjId, UI_NUM, pItemElemtmp->m_nItemNum );
//		g_UserMng.DestroyPlayer( pUser );
//		g_UserMng.RemoveUser( pUser->m_dwSerial );
//		g_DPCoreClient.SendKillPlayer( pUser->m_idPlayer, pUser->m_idPlayer );
		QueryDestroyPlayer( pUser->m_Snapshot.dpidCache, pUser->m_Snapshot.dpidUser, pUser->m_dwSerial, pUser->m_idPlayer ); // pUser->m_Snapshot.dpidUser���� ���Ϲ�ȣ�� �� �ִ�.
	}
}
void CDPSrvr::OnSealCharGetReq( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	DWORD dwData;
	DWORD dwId;

	ar >> dwData ;

	CUser* pUser	=	g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) ) 
	{
		dwId = HIWORD( dwData );

		if( pUser->IsUsableState( dwId ) == FALSE )
			return;

		CItemElem* pItemElem = pUser->m_Inventory.GetAtId( dwId );
		if( IsUsableItem( pItemElem ) )
		{
			if(pItemElem->m_dwItemId != II_SYS_SYS_SCR_SEALCHARACTER )
				return;
			g_dpDBClient.SendQueryGetSealCharGet( pUser->m_idPlayer,pUser->m_playAccount.lpszAccount,dwId);
		}
	}
}

#endif // __MA_VER11_05	// �ɸ��� ���� �ŷ� ��� world,database,neuz

#if __VER >= 11 // __SYS_POCKET
void	CDPSrvr::OnMoveItemOnPocket( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		int nPocket1, nItem, nPocket2;
		short nNum;
		ar >> nPocket1 >> nItem >> nNum >> nPocket2;

		if( nPocket1 == nPocket2 )
			return;

		// mirchang 091214 - �������� ���������� üũ
		if( nPocket1 == -1 )	// ������ �κ��丮���� �˻�!
		{
			if( pUser->m_Inventory.IsEquip( nItem ) )
				return;
		}

		CItemElem* pItem	= pUser->GetItemId2( nPocket1, nItem );	// ���⼭ �޴밡�� ����˻� ������.
		if( IsUsableItem( pItem ) )
		{
			if( nPocket1 < 0 && pUser->IsUsing( pItem ) )
				return;
			if( nNum > pItem->m_nItemNum )
				return;

			CItemElem item;
			item	= *pItem;
			item.m_nItemNum		= nNum;
			if( pUser->CreateItem2( &item, nPocket2 ) )
			{
				pUser->RemoveItem2( nItem, nNum, nPocket1 );
				// log
				LogItemInfo	log;
				log.Action	= "m";
				log.SendName	= pUser->GetName();
				log.RecvName	= "OnMoveItemOnPocket";
				log.WorldId		= pUser->GetWorld()->GetID();
				log.Gold	= nPocket1;
				log.Gold2	= nPocket2;
				OnLogItem( log, &item, nNum );
			}
		}
	}
}

void	CDPSrvr::OnAvailPocket( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	=	g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) ) 
	{
		int nPocket, nItem;
		ar >> nPocket >> nItem;
		if( nPocket <= 0 && nPocket >= MAX_POCKET )
			return;
		CItemElem* pItemElem	= pUser->m_Inventory.GetAtId( nItem );
		if( IsUsableItem( pItemElem ) )
		{
			ItemProp* pItemProp		= pItemElem->GetProp();
			if( pItemProp->dwItemKind3 == IK3_POCKET )
			{
				if( !pUser->m_Pocket.IsAvailable( nPocket ) )
				{
					pUser->m_Pocket.SetAttribute( CPocketController::avail, nPocket, pItemProp->dwSkillTime );
					// log
					LogItemInfo	log;
					log.Action	= "u";
					log.SendName	= pUser->GetName();
					log.RecvName	= "OnAvailPocket";
					log.WorldId		= pUser->GetWorld()->GetID();
					log.Gold	= pUser->GetGold();
					log.Gold2	= pUser->GetGold();
					log.Gold_1	= nPocket;
					OnLogItem( log, pItemElem, 1 );
					pUser->UpdateItem( (BYTE)( pItemElem->m_dwObjId ), UI_NUM, pItemElem->m_nItemNum - 1 );
#ifdef __INTERNALSERVER
					pUser->AddPocketView();
#endif	// __INTERNALSERVER
				}
			}
		}
	}
}
#endif	// __SYS_POCKET

#if __VER >= 11 // __SYS_IDENTIFY
void	CDPSrvr::OnBlessednessCancel( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	=	g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) ) 
	{
		if( !CNpcChecker::GetInstance()->IsCloseNpc( MMI_BLESSING_CANCEL, pUser->GetWorld(), pUser->GetPos() ) )
			return;

		int nItem;
		ar >> nItem;
		CItemElem* pItem	= (CItemElem*)pUser->GetItemId( nItem );
		if( IsUsableItem( pItem ) )
		{
			if( g_xRandomOptionProperty->GetRandomOptionKind( pItem ) == CRandomOptionProperty::eBlessing
				&& g_xRandomOptionProperty->GetRandomOptionSize( pItem->GetRandomOptItemId() ) > 0 )
			{
				g_xRandomOptionProperty->InitializeRandomOption( pItem->GetRandomOptItemIdPtr() );
				pUser->UpdateItemEx( (BYTE)( pItem->m_dwObjId ), UI_RANDOMOPTITEMID, pItem->GetRandomOptItemId() );
				pUser->AddDiagText( prj.GetText( TID_GAME_BLESSEDNESS_CANCEL_INFO ) );
				// log
				LogItemInfo	log;
				log.Action	= "r";
				log.SendName	= pUser->GetName();
				log.RecvName	= "OnBlessednessCancel";
				log.WorldId		= pUser->GetWorld()->GetID();
				log.Gold	= pUser->GetGold();
				log.Gold2	= pUser->GetGold();
				OnLogItem( log, pItem, 1 );
			}
			else
			{
				pUser->AddDefinedText( TID_GAME_BLESSEDNESS_CANCEL );	
			}
		}		
	}
}

void	CDPSrvr::OnAwakening( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	=	g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) ) 
	{
		if( !CNpcChecker::GetInstance()->IsCloseNpc( MMI_ITEM_AWAKENING, pUser->GetWorld(), pUser->GetPos() ) )
			return;

		const int	nCost	= 100000;
		if( pUser->GetGold() < nCost )
		{
			pUser->AddDefinedText( TID_GAME_LACKMONEY, "" );
			return;
		}
		int	nItem;
		ar >> nItem;
		CItemElem* pItem	= pUser->m_Inventory.GetAtId( nItem );
		if( IsUsableItem( pItem ) )
		{
			int nRandomOptionKind	= g_xRandomOptionProperty->GetRandomOptionKind( pItem );

			if( nRandomOptionKind != CRandomOptionProperty::eAwakening )
			{
				pUser->AddDefinedText( TID_GAME_INVALID_TARGET_ITEM );
				return;
			}
			if( g_xRandomOptionProperty->GetRandomOptionSize( pItem->GetRandomOptItemId() ) > 0 )
			{
				pUser->AddDefinedText( TID_GAME_AWAKE_OR_BLESSEDNESS01 );
				return;
			}
			g_xRandomOptionProperty->InitializeRandomOption( pItem->GetRandomOptItemIdPtr() );
			g_xRandomOptionProperty->GenRandomOption( pItem->GetRandomOptItemIdPtr(), nRandomOptionKind, pItem->GetProp()->dwParts );
			pUser->UpdateItemEx( (BYTE)( pItem->m_dwObjId ), UI_RANDOMOPTITEMID, pItem->GetRandomOptItemId() );
			pUser->AddGold( -nCost );
			pUser->AddDefinedText( TID_GAME_AWAKENING_SUCCESS );
			// log
			LogItemInfo	log;
			log.Action	= "r";
			log.SendName	= pUser->GetName();
			log.RecvName	= "OnAwakening";
			log.WorldId		= pUser->GetWorld()->GetID();
			log.Gold	= pUser->GetGold() + nCost;
			log.Gold2	= pUser->GetGold();
			log.Gold_1	= -nCost;
			OnLogItem( log, pItem, 1 );
		}
	}
}
#endif	// __SYS_IDENTIFY

#ifdef __NPC_BUFF
void	CDPSrvr::OnNPCBuff( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	=	g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) ) 
	{
		CHAR	m_szKey[64];
		ar.ReadString(m_szKey, 64);
		LPCHARACTER lpChar = prj.GetCharacter( m_szKey );
		
		if( lpChar )
		{
			if( !CNpcChecker::GetInstance()->IsCloseNpc(MMI_NPC_BUFF, pUser->GetWorld(), pUser->GetPos() ) )
				return;

			vector<NPC_BUFF_SKILL> vecNPCBuff = lpChar->m_vecNPCBuffSkill;
			for( int i=0; i<(int)( vecNPCBuff.size() ); i++ )
			{
				if( pUser->GetLevel() >= vecNPCBuff[i].nMinPlayerLV && pUser->GetLevel() <= vecNPCBuff[i].nMaxPlayerLV )
				{
					ItemProp* pSkillProp = prj.GetSkillProp( vecNPCBuff[i].dwSkillID );
					if( pSkillProp )
					{
						if( vecNPCBuff[i].dwSkillLV < pSkillProp->dwExpertLV || vecNPCBuff[i].dwSkillLV > pSkillProp->dwExpertMax )
							continue;
						
						if(	( pSkillProp->dwID == SI_GEN_EVE_QUICKSTEP && pUser->HasBuff(BUFF_SKILL, SI_ASS_CHEER_QUICKSTEP) )
							|| ( pSkillProp->dwID == SI_GEN_EVE_HASTE && pUser->HasBuff(BUFF_SKILL, SI_ASS_CHEER_HASTE) )
							|| ( pSkillProp->dwID == SI_GEN_EVE_HEAPUP && pUser->HasBuff(BUFF_SKILL, SI_ASS_CHEER_HEAPUP) )
	  						|| ( pSkillProp->dwID == SI_GEN_EVE_ACCURACY && pUser->HasBuff(BUFF_SKILL, SI_ASS_CHEER_ACCURACY) ) )
						{
							pUser->AddDefinedText( TID_GAME_NPCBUFF_FAILED, "\"%s\"", pSkillProp->szName );
							continue;
						}
						
						AddSkillProp* pAddSkillProp = prj.GetAddSkillProp( pSkillProp->dwSubDefine + vecNPCBuff[i].dwSkillLV - 1 );
						if( pAddSkillProp )
						{
							// skill property�� �����ϱ� ������ ����صξ��ٰ�
							// ��ų ������ restore��Ų��.
							DWORD dwReferTarget1Backup = pSkillProp->dwReferTarget1;
							pSkillProp->dwReferTarget1 = NULL_ID;
							DWORD dwReferTarget2Backup = pSkillProp->dwReferTarget2;
							pSkillProp->dwReferTarget2 = NULL_ID;
							DWORD dwSkillTimeBackup = pAddSkillProp->dwSkillTime;
							pAddSkillProp->dwSkillTime = vecNPCBuff[i].dwSkillTime;

							pUser->DoApplySkill( pUser, pSkillProp, pAddSkillProp );
							g_UserMng.AddDoApplySkill( pUser, pUser->GetId(), vecNPCBuff[i].dwSkillID, vecNPCBuff[i].dwSkillLV );

							pSkillProp->dwReferTarget1 = dwReferTarget1Backup;
							pSkillProp->dwReferTarget2 = dwReferTarget2Backup;
							pAddSkillProp->dwSkillTime = dwSkillTimeBackup;
						}
					}
				}
				else
				{
					ItemProp* pSkillProp = prj.GetSkillProp( vecNPCBuff[i].dwSkillID );
					if( pSkillProp )
						pUser->AddDefinedText( TID_GAME_NPCBUFF_LEVELLIMIT, "%d %d \"%s\"", vecNPCBuff[i].nMinPlayerLV, vecNPCBuff[i].nMaxPlayerLV, pSkillProp->szName );
				}
			}
		}
	}
}
#endif // __NPC_BUFF

#ifdef __JEFF_11_4
void	CDPSrvr::OnArenaEnter( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
// �Ʒ��� ���� ����
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		if( pUser->IsBaseJob() )	// 1�� ������ �Ϸ��� ������ ����
			return;
		pUser->SetMarkingPos();
		pUser->REPLACE( g_uIdofMulti, WI_WORLD_ARENA, D3DXVECTOR3( 540.0F, 140.0F, 485.0F ), REPLACE_NORMAL, nDefaultLayer );
	}
}

void	CDPSrvr::OnArenaExit( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
		pUser->REPLACE( g_uIdofMulti, pUser->m_idMarkingWorld, pUser->m_vMarkingPos, REPLACE_NORMAL, nTempLayer );
}
#endif	// __JEFF_11_4

#ifdef __JEFF_11
// ���� �������� ��ȯ
void	CDPSrvr::OnQuePetResurrection( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		int nItem;
		ar >> nItem;
		CItemElem*	pItem	= (CItemElem*)pUser->GetItemId( nItem );
		if( !IsUsableItem( pItem ) )
			return;

		CPet* pPet	= pItem->m_pPet;
		if( !pPet || pUser->GetPetId() == pItem->m_dwObjId || pItem->IsFlag( CItemElem::expired ) )
		{
			pUser->AddQuePetResurrectionResult( FALSE );
			return;
		}

		BYTE nLevel		= pPet->GetLevel();
		if( nLevel < PL_B || nLevel > PL_S )
		{
			pUser->AddQuePetResurrectionResult( FALSE );
			return;
		}

		CItemElem itemElem;
		itemElem.m_nItemNum	= 1;
		switch( nLevel )
		{
			case PL_B:	itemElem.m_dwItemId		= II_SYS_SYS_QUE_PETRESURRECTION01_B;	break;
			case PL_A:	itemElem.m_dwItemId		= II_SYS_SYS_QUE_PETRESURRECTION01_A;	break;
			case PL_S:	itemElem.m_dwItemId		= II_SYS_SYS_QUE_PETRESURRECTION01_S;	break;
		}
		// log
		LogItemInfo	log;
		log.Action	= "x";
		log.SendName	= pUser->GetName();
		log.RecvName	= "OnQuePetResurrection";
		OnLogItem( log, pItem, 1 );

		pUser->RemoveItem( nItem, 1 );
		pUser->CreateItem( &itemElem );
		pUser->AddQuePetResurrectionResult( TRUE );
	}
}
#endif	// __JEFF_11

#if __VER >= 12 // __SECRET_ROOM
void	CDPSrvr::OnSecretRoomTenderOpenWnd( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );

	if( IsValidObj( pUser ) == FALSE )
		return;

	CSecretRoomMng::GetInstance()->SetTenderOpenWnd( pUser );
}

void	CDPSrvr::OnSecretRoomTender( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	
	if( IsValidObj( pUser ) == FALSE )
		return;

	int nPenya;
	ar >> nPenya;

	if( nPenya <= 0 )
		return;

	CSecretRoomMng::GetInstance()->SetTender( pUser, nPenya );
}

void	CDPSrvr::OnSecretRoomTenderCancelReturn( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );

	if( IsValidObj( pUser ) == FALSE )
		return;

	CSecretRoomMng::GetInstance()->SetTenderCancelReturn( pUser );
}

void	CDPSrvr::OnSecretRoomLineUpOpenWnd( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );

	if( IsValidObj( pUser ) == FALSE )
		return;

	CSecretRoomMng::GetInstance()->SetLineUpOpenWnd( pUser );
}

void	CDPSrvr::OnSecretRoomLineUpMember( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	
	if( IsValidObj( pUser ) == FALSE )
		return;

	set<DWORD> checker;
	vector<DWORD> vecLineUpMember;
	int nSize;
	ar >> nSize;

	if( nSize > CSecretRoomMng::GetInstance()->m_nMaxGuildMemberNum )
		return;

	for( int i=0; i<nSize; i++ )
	{
		DWORD dwIdPlayer;
		ar >> dwIdPlayer;

		// ��ȿ�� Player ID �ΰ�?
		PlayerData* pData	= CPlayerDataCenter::GetInstance()->GetPlayerData( dwIdPlayer );
		if( !pData )
			return;

		// ID �ߺ�üũ		
		if( !checker.insert( dwIdPlayer ).second )
			return;

		vecLineUpMember.push_back( dwIdPlayer );
	}

	CSecretRoomMng::GetInstance()->SetLineUp( pUser, vecLineUpMember );
}

void	CDPSrvr::OnSecretRoomEntrance( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );

	if( IsValidObj( pUser ) == FALSE )
		return;

	CSecretRoomMng::GetInstance()->SetTeleportSecretRoom( pUser );
}

void	CDPSrvr::OnSecretRoomTeleportToNPC( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );

	if( IsValidObj( pUser ) == FALSE )
		return;
	pUser->REPLACE( g_uIdofMulti, WI_WORLD_MADRIGAL, CSecretRoomMng::GetInstance()->GetRevivalPos( pUser ), REPLACE_NORMAL, nDefaultLayer );
}

void CDPSrvr::OnSecretRoomTenderView( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );

	if( IsValidObj( pUser ) == FALSE )
		return;

	CSecretRoomMng::GetInstance()->GetTenderView( pUser );
}

void CDPSrvr::OnTeleportSecretRoomDungeon( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );

	if( IsValidObj( pUser ) == FALSE )
		return;
#if __VER >= 12 // __TAX
	if( !CNpcChecker::GetInstance()->IsCloseNpc( MMI_SECRET_ENTRANCE_1, pUser->GetWorld(), pUser->GetPos() ) )
		return;

	__TAXINFO* pTaxInfo = CTax::GetInstance()->GetTaxInfo( CTax::GetInstance()->GetContinent( pUser ) );
	if( pTaxInfo && pUser->GetGuild() && (pTaxInfo->dwId == pUser->GetGuild()->GetGuildId()) )
	{
		pUser->SetMarkingPos();
		pUser->SetAngle( 180.0f );
		int nRandx = xRandom(4) - 2;
		int nRandz = xRandom(4) - 2;
		pUser->REPLACE( g_uIdofMulti, WI_DUNGEON_SECRET_0, D3DXVECTOR3( (float)( 295 + nRandx ), 102.0f, (float)( 530 + nRandz ) ), REPLACE_NORMAL, nDefaultLayer );
	}
	else
		pUser->AddDefinedText( TID_GAME_SECRETROOM_NOENTRANCE_1 );
#endif // __TAX
}
#endif // __SECRET_ROOM

#if __VER >= 12 // __LORD
void CDPSrvr::OnElectionAddDeposit( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	=	g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		__int64 iTotal;
		ar >> iTotal;

		__int64 iDeposit;
		int nRet	= election::AddDepositRequirements( pUser, iTotal, iDeposit );
		if( nRet )
		{
			pUser->AddDefinedText( nRet );
			return;
		}
		pUser->AddGold( -static_cast<int>( iDeposit ) );
		g_dpDBClient.SendElectionAddDeposit( pUser->m_idPlayer, iDeposit );

		PutPenyaLog( pUser, "d", "DEPOSIT", static_cast<int>( iDeposit ) );
	}
}

void CDPSrvr::PutItemLog( CUser* pUser, const char* szAction, const char* szContext, CItemElem* pItem, int nNum )
{	// �������� ���ŵ� ���Ŀ� ȣ����� �ʵ��� �����ؾ� ��
	LogItemInfo	log;
	log.Action	=  szAction;
	log.SendName	= pUser->GetName();
	log.RecvName	= szContext;
	log.WorldId		= pUser->GetWorld() ? pUser->GetWorld()->GetID() : WI_WORLD_NONE;	// chipi_090623 ���� - ù ���ӽ� ����� ������ ��� ���尡 ���� ���·� ���´�. 
	log.Gold	= pUser->GetGold();
	log.Gold2	= pUser->GetGold();
	if( nNum == 0 )
		nNum	= pItem->m_nItemNum;
	OnLogItem( log, pItem, nNum );
}

void CDPSrvr::PutPenyaLog( CUser* pUser, const char* szAction, const char* szContext,  int nCost )
{	// ��� ��� ó�� �� ȣ��Ǿ�� ��
	LogItemInfo	log;
	log.Action	= szAction;
	log.SendName	= pUser->GetName();
	log.RecvName	= szContext;
	log.WorldId		= pUser->GetWorld() ? pUser->GetWorld()->GetID() : WI_WORLD_NONE;	// chipi_090623 ���� - ù ���ӽ� ����� ������ ��� ���尡 ���� ���·� ���´�. 
	//log.ItemName	= "SEED";
	_stprintf( log.szItemName, "%d", II_GOLD_SEED1 );
	log.Gold	= pUser->GetGold() + nCost;
	log.Gold2	= pUser->GetGold();
	log.Gold_1	= -nCost;
	OnLogItem( log );
}

void CDPSrvr::OnElectionSetPledge( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	char szPledge[CCandidate::nMaxPledgeLen]	= { 0,};
	ar.ReadString( szPledge, CCandidate::nMaxPledgeLen );
	if( strlen( szPledge ) == 0 )
		return;

	CUser* pUser	=	g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		int nCost;
		int nRet	= election::SetPledgeRequirements( pUser, nCost );
		if( nRet )
		{
			pUser->AddDefinedText( nRet );
			return;
		}
		pUser->AddGold( -nCost );
		g_dpDBClient.SendElectionSetPledge( pUser->m_idPlayer, szPledge );

		PutPenyaLog( pUser, "p", "PLEDGE", nCost );
	}
}

void CDPSrvr::OnElectionIncVote( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	=	g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		u_long idPlayer;
		ar >> idPlayer;

		int nRet	= election::IncVoteRequirements( pUser, idPlayer );
		if( nRet )
		{
			pUser->AddDefinedText( nRet );
			return;
		}
		g_dpDBClient.SendElectionIncVote( idPlayer, pUser->m_idPlayer );
	}
}

void CDPSrvr::OnLEventCreate( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	=	g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		int iEEvent, iIEvent;
		ar >> iEEvent >> iIEvent;
		int nRet	= lordevent::CreateRequirements( pUser, iEEvent, iIEvent );
		if( nRet )
		{
			pUser->AddDefinedText( nRet );
			return;
		}
		ILordEvent* pEvent		= CSLord::Instance()->GetEvent();

		int nPerin	= pUser->RemoveTotalGold( pEvent->GetCost( iEEvent, iIEvent ) );
		char szContext[100]		= { 0,};
		sprintf( szContext, "OnLEventCreate: cost: %d(perin), %I64d(penya)", nPerin, pEvent->GetCost( iEEvent, iIEvent ) - ( nPerin * PERIN_VALUE ) );
		PutPenyaLog( pUser, "e", szContext, 0 );

		g_dpDBClient.SendLEventCreate( pUser->m_idPlayer, iEEvent, iIEvent );
	}
}

void CDPSrvr::OnLordSkillUse( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		int nSkill;
		char szTarget[MAX_PLAYER]	= { 0,};
		ar >> nSkill;
		ar.ReadString( szTarget, MAX_PLAYER );
		u_long idTarget;
		int nRet	= lordskill::UseRequirements( pUser, szTarget, nSkill, idTarget );
		if( nRet )
		{
			pUser->AddDefinedText( nRet );
			return;
		}
		g_dpDBClient.SendLordSkillUse( pUser->m_idPlayer, idTarget, nSkill );
	}
}
#endif	// __LORD

#if __VER >= 12 // __TAX
void CDPSrvr::OnSetTaxRate( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	=	g_UserMng.GetUser( dpidCache, dpidUser );
	if( !IsValidObj( pUser ) )
		return;

	BYTE nCont;
	int nSalesTaxRate, nPurchaseTaxRate;
	ar >> nCont;
	ar >> nSalesTaxRate >> nPurchaseTaxRate;

	__TAXINFO* pTaxInfo = CTax::GetInstance()->GetTaxInfo( nCont );
	if( pTaxInfo && pUser->m_idGuild == pTaxInfo->dwNextId && pUser->IsGuildMaster() )
		CTax::GetInstance()->SetNextTaxRate( nCont, nSalesTaxRate, nPurchaseTaxRate );
	else
		Error( "\"%\"s User Is Not Next Win Guild Master!!!", pUser->GetName() );
}
#endif // __TAX

#if __VER >= 12 // __HEAVEN_TOWER
void CDPSrvr::OnTeleportToHeavenTower( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	CUser* pUser	=	g_UserMng.GetUser( dpidCache, dpidUser );
	if( !IsValidObj( pUser ) )
		return;

	if( !CNpcChecker::GetInstance()->IsCloseNpc( MMI_HEAVEN_TOWER, pUser->GetWorld(), pUser->GetPos() ) )
		return;
    	
	int nFloor;
	ar >> nFloor;

	int nCost = 0;
	DWORD dwWorldId = NULL_ID;
	float fAngle = 0.0f;
	D3DXVECTOR3 vPos;

	switch( nFloor )
	{
		case 1 :	// 1��
			nCost = 10000;	dwWorldId = WI_WORLD_HEAVEN01; vPos = D3DXVECTOR3( 253, 102, 78 ); fAngle = 183.0f;
			break;
		case 2 :	// 2��
			nCost = 30000;	dwWorldId = WI_WORLD_HEAVEN02; vPos = D3DXVECTOR3( 251, 102, 95 ); fAngle = 183.0f;
			break;
		case 3 :	// 3��
			nCost = 50000;	dwWorldId = WI_WORLD_HEAVEN03; vPos = D3DXVECTOR3( 264, 102, 227 ); fAngle = 183.0f;
			break;
		case 4 :	// 4��
			nCost = 70000;	dwWorldId = WI_WORLD_HEAVEN04; vPos = D3DXVECTOR3( 253, 102, 86 ); fAngle = 174.0f;
			break;
		case 5 :	// 5��
			nCost = 100000;	dwWorldId = WI_WORLD_HEAVEN05; vPos = D3DXVECTOR3( 218, 102, 101); fAngle = 176.0f;
			break;

		default :
			Error( "CDPSrvr::OnTeleportToHeavenTower() - �߸��� �� : %d, Name = %s", nFloor, pUser->GetName() );
			return;
	}

	if( pUser->GetGold() < nCost )
	{
		pUser->AddDefinedText( TID_GAME_LACKMONEY );
		return;
	}

#if __VER >= 12 // __TAX
	BYTE nCont = CTax::GetInstance()->GetContinent( pUser );
#endif // __TAX	
	// �ش� ������ �ڷ���Ʈ -> ���н� �׳� ����...
	if( pUser->REPLACE( g_uIdofMulti, dwWorldId, vPos, REPLACE_NORMAL, nDefaultLayer ) )
	{
		pUser->AddGold( -nCost );
		pUser->SetAngle( fAngle );
#if __VER >= 12 // __TAX
		__TAXINFO* pTaxInfo = CTax::GetInstance()->GetTaxInfo( nCont );
		if( pTaxInfo && pTaxInfo->dwId != NULL_ID )
			CTax::GetInstance()->AddTax( nCont, nCost, TAX_ADMISSION );
#endif // __TAX		
		CString strFloor;
		strFloor.Format( "HEAVEN_TOWER_%2d", nFloor );
		PutPenyaLog( pUser, "h", strFloor, nCost );
	}
	else
		return;
}
#endif // __HEAVEN_TOWER

BOOL CDPSrvr::DoUseItemTarget_GenRandomOption(
											  CUser* pUser, CItemElem* pTarget, int nKind,
											  int nOk, int nMismatchKind, int nHasOption,
											  const char* szOperation, const char* szReceive )
{
	int nRandomOptionKind	= g_xRandomOptionProperty->GetRandomOptionKind( pTarget );
	if( nRandomOptionKind != nKind || pTarget->IsFlag( CItemElem::expired ) )
	{
		pUser->AddDefinedText( nMismatchKind );
		return FALSE;
	}

	if( 
#if __VER >= 12 // __J12_0
		// ������ �ູ�� ���� ������ ���� ��� ���� ���� �� �� �ְ� �Ѵ�
		nKind != CRandomOptionProperty::eBlessing && nKind != CRandomOptionProperty::eEatPet &&
#endif	// __J12_0
		g_xRandomOptionProperty->GetRandomOptionSize( pTarget->GetRandomOptItemId() ) > 0
	)
	{
		pUser->AddDefinedText( nHasOption );
		return FALSE;
	}

	if( nOk > 0 )
		pUser->AddDefinedText( nOk );

	// Ȱ��ȭ �� �Ⱦ����̸�  ���� ȿ���� ����	// ˬ: 2008-09-29 �߰�
	if( pUser->IsUsing( pTarget ) && nKind == CRandomOptionProperty::eEatPet )
		pUser->ResetDestParamRandomOptExtension( pTarget );


	//	mulcom	BEGIN100405	���� ��ȣ�� �η縶��
	//g_xRandomOptionProperty->InitializeRandomOption( pTarget->GetRandomOptItemIdPtr() );
	//g_xRandomOptionProperty->GenRandomOption( pTarget->GetRandomOptItemIdPtr(), nRandomOptionKind, pTarget->GetProp()->dwParts );
	bool	bCheckedSafeFlag	= false;
	bCheckedSafeFlag	= g_xRandomOptionProperty->IsCheckedSafeFlag( pTarget->GetRandomOptItemId() );

	if( bCheckedSafeFlag == true )
	{
		g_xRandomOptionProperty->ResetSafeFlag( pTarget->GetRandomOptItemIdPtr() );

		g_xRandomOptionProperty->InitializeRandomOption( pTarget->GetNewRandomOptionPtr() );
		g_xRandomOptionProperty->GenRandomOption( pTarget->GetNewRandomOptionPtr(), nRandomOptionKind, pTarget->GetProp()->dwParts, true );

		//	mulcom	BEGIN100426	���� ��ȣ�� �η縶�� �α�.
// 		WriteLog( "UserID [%d] : ItemSerialNumber[%d] Protect GenNewRandomOption [%I64d]", (int)( pUser->GetId() ), (int)( pTarget->GetSerialNumber() ), pTarget->GetNewRandomOption() );
 
		TCHAR	szNewOption[128];
		::memset( szNewOption, 0, sizeof(szNewOption) );
		::_stprintf( szNewOption, "%I64d", pTarget->GetNewRandomOption() );
		g_DPSrvr.PutItemLog( pUser, "z", szNewOption, pTarget, 1 );
		//	mulcom	END100426	���� ��ȣ�� �η縶�� �α�.

		pUser->SendNewRandomOption( (BYTE)( pTarget->m_dwObjId ), (DWORD)( pTarget->GetSerialNumber() ), pTarget->GetNewRandomOption() );

	}
	else
	{
		g_xRandomOptionProperty->InitializeRandomOption( pTarget->GetRandomOptItemIdPtr() );
		g_xRandomOptionProperty->GenRandomOption( pTarget->GetRandomOptItemIdPtr(), nRandomOptionKind, pTarget->GetProp()->dwParts );
	}
	//	mulcom	END100405	���� ��ȣ�� �η縶��

	pUser->UpdateItemEx( (BYTE)( pTarget->m_dwObjId ), UI_RANDOMOPTITEMID, pTarget->GetRandomOptItemId() );

#if __VER >= 12 // __PET_0519
	// Ȱ��ȭ �� �Ⱦ��� �Ǵ� ������ ���� �� �� ȿ�� ����
	if( pUser->IsUsing( pTarget ) 
		&& ( nKind == CRandomOptionProperty::eSystemPet || nKind == CRandomOptionProperty::eEatPet ) )
		pUser->SetDestParamRandomOptExtension( pTarget );
#endif	// __PET_0519

	// log
	LogItemInfo	log;
	log.Action	= szOperation;
	log.SendName	= pUser->GetName();
	log.RecvName	= szReceive;
	log.WorldId		= pUser->GetWorld()->GetID();
	log.Gold	= pUser->GetGold();
	log.Gold2	= pUser->GetGold();
	OnLogItem( log, pTarget, 1 );
	return TRUE;
}

BOOL CDPSrvr::DoUseItemTarget_InitializeRandomOption(
											  CUser* pUser, CItemElem* pTarget, int nKind,
											  int nOk, int nError,
											  const char* szOperation, const char* szRecv )
{
	int nRandomOptionKind	= g_xRandomOptionProperty->GetRandomOptionKind( pTarget );
	if( nRandomOptionKind == nKind && g_xRandomOptionProperty->GetRandomOptionSize( pTarget->GetRandomOptItemId() ) > 0 )
	{
#if __VER >= 12 // __PET_0519
		// ���� �Ǵ� �ý��� �� ���� ��� �� ȿ�� ����
		if( pUser->IsUsing( pTarget ) 
			&& ( nKind == CRandomOptionProperty::eSystemPet || nKind == CRandomOptionProperty::eEatPet ) )
			pUser->ResetDestParamRandomOptExtension( pTarget );
#endif	// __PET_0519

		//	mulcom	BEGIN100405	���� ��ȣ�� �η縶��
		//g_xRandomOptionProperty->InitializeRandomOption( pTarget->GetRandomOptItemIdPtr() );
#ifdef __PROTECT_AWAKE
		if( pUser->HasBuff( BUFF_ITEM, II_SYS_SYS_SCR_AWAKESAFE ) == TRUE && nKind == CRandomOptionProperty::eAwakening )
		{
			nOk = TID_GAME_REGARDLESS_USE04;
			g_xRandomOptionProperty->SetSafeFlag( pTarget->GetRandomOptItemIdPtr() );

			pUser->RemoveBuff( BUFF_ITEM, II_SYS_SYS_SCR_AWAKESAFE );

			//	mulcom	BEGIN100426	���� ��ȣ�� �η縶�� �α�.
			g_DPSrvr.PutItemLog( pUser, "z", "USE_AWAKESAVE_ITEM", pTarget, 1 );
			//	mulcom	END100426	���� ��ȣ�� �η縶�� �α�.

		}
		else
		{
			g_xRandomOptionProperty->InitializeRandomOption( pTarget->GetRandomOptItemIdPtr() );
		}
#else	//__PROTECT_AWAKE
		g_xRandomOptionProperty->InitializeRandomOption( pTarget->GetRandomOptItemIdPtr() );
#endif	//__PROTECT_AWAKE
		//	mulcom	END100405	���� ��ȣ�� �η縶��

		pUser->UpdateItemEx( (BYTE)( pTarget->m_dwObjId ), UI_RANDOMOPTITEMID, pTarget->GetRandomOptItemId() );
		pUser->AddDiagText( prj.GetText( nOk ) );
		// log
		LogItemInfo	log;
		log.Action	= szOperation;
		log.SendName	= pUser->GetName();
		log.RecvName	= szRecv;
		log.WorldId		= pUser->GetWorld()->GetID();
		log.Gold	= pUser->GetGold();
		log.Gold2	= pUser->GetGold();
		OnLogItem( log, pTarget, 1 );
		return TRUE;
	}
	pUser->AddDefinedText( nError );	
	return FALSE;
}

BOOL CDPSrvr::DoUseItemTarget_ItemLevelDown( CUser* pUser, CItemElem* pMaterial, CItemElem* pTarget )
{
	ItemProp* pProp		= pTarget->GetProp();
	if( pProp->dwParts == NULL_ID || pProp->dwLimitLevel1 == 0xFFFFFFFF )
	{
		pUser->AddDefinedText( TID_GAME_INVALID_TARGET_ITEM );
		return FALSE;
	}

	int nLevelDown	= pTarget->GetLevelDown();
	if( nLevelDown == 0 || ( nLevelDown == -5 && pMaterial->m_dwItemId == II_SYS_SYS_SCR_LEVELDOWN02 ) )
	{
		pTarget->SetLevelDown( pMaterial->m_dwItemId == II_SYS_SYS_SCR_LEVELDOWN01? CItemElem::e5LevelDown: CItemElem::e10LevelDown );
		pUser->UpdateItemEx( (BYTE)( pTarget->m_dwObjId ), UI_RANDOMOPTITEMID, pTarget->GetRandomOptItemId() );
		// log
		LogItemInfo	log;
		log.Action	= "r";
		log.SendName	= pUser->GetName();
		log.RecvName	= "::ItemLevelDown";
		log.WorldId		= pUser->GetWorld()->GetID();
		log.Gold	= pUser->GetGold();
		log.Gold2	= pUser->GetGold();
		OnLogItem( log, pTarget, 1 );
		return TRUE;
	}
	else
		pUser->AddDefinedText( TID_GAME_ITEM_LEVELDOWN01 );

	return FALSE;
}

#if __VER >= 12 // __PET_0519
void CDPSrvr::OnTransformItem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long )
{	// �˺�ȯ
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( !IsValidObj( pUser ) )
		return;

	CTransformStuff stuff;
	stuff.Serialize( ar );	// ��Ḧ ����

	// ��ȯ ��ȣ�κ��� ��ȯ �Լ��� �����Ѵ�.
	ITransformer* pTransformer	= ITransformer::Transformer( stuff.GetTransform() );
	pTransformer->Transform( pUser, stuff );	// ��ȯ
}
#endif	// __PET_0519

#if __VER >= 12 // __MOD_TUTORIAL
void CDPSrvr::OnTutorialState( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( !IsValidObj( pUser ) )
		return;

	int nState;
	char szState[64]	= { 0,};
	char szOut[64]	= { 0,};
	ar >> nState;
	ar.ReadString( szState, 64 );
	MakeTutorialStateString( szOut, nState, pUser->GetName() );
	if( lstrcmp( szOut, szState ) == 0 )
	{
		pUser->SetTutorialState( nState );
		pUser->AddSetTutorialState();
	}
}
#endif	// __MOD_TUTORIAL

#if __VER >= 12 // __PET_0519
// �Ⱦ��� ���� ��� �޴� ���� �ڵ鷯
void CDPSrvr::OnPickupPetAwakeningCancel( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( !IsValidObj( pUser ) )
		return;
	if( !CNpcChecker::GetInstance()->IsCloseNpc( MMI_PET_AWAK_CANCEL, pUser->GetWorld(), pUser->GetPos() ) )
		return;

	DWORD dwItem;
	ar >> dwItem;
	CItemElem* pItem	= (CItemElem*)pUser->GetItemId( dwItem );
	if( !IsUsableItem( pItem ) )
		return;

	if( DoUseItemTarget_InitializeRandomOption( pUser, pItem, CRandomOptionProperty::eEatPet,
					TID_GAME_PICKUP_PET_AWAKENING_CANCEL_S001, TID_GAME_PICKUP_PET_AWAKENING_CANCEL_E001,
					"k", "PPAC" ) )
	{
		pUser->AddPlaySound( SND_INF_UPGRADESUCCESS );
		if( pUser->IsMode( TRANSPARENT_MODE ) == 0 )
			g_UserMng.AddCreateSfxObj( pUser, XI_INT_SUCCESS, pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z );
	}
}
#endif	// __PET_0519

#ifdef __AZRIA_1023
void CDPSrvr::OnDoUseItemInput( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long )
{
	DWORD dwData;
	ar >> dwData;
	char szInput[MAX_INPUT_LEN]		= { 0,};
	ar.ReadString( szInput, MAX_INPUT_LEN );
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( !IsValidObj( pUser ) )
		return;
	pUser->SetInput( szInput );
	pUser->OnDoUseItem( dwData, 0, -1 );
	pUser->ResetInput();
}
#endif	// __AZRIA_1023

#ifdef __PET_1024
void CDPSrvr::OnClearPetName( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		CPet* pPet	= pUser->GetPet();
		if( !pPet )
		{
			pUser->AddDefinedText( TID_GAME_NAME_PET_E00 );
			return;
		}
		pPet->SetName( "" );
		g_UserMng.AddSetPetName( pUser, pPet->GetName() );
	}
}
#endif	// __PET_1024

#if __VER >= 13 // __RAINBOW_RACE
void CDPSrvr::OnRainbowRacePrevRankingOpenWnd( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( !IsValidObj( pUser ) )
		return;

	pUser->AddRainbowRacePrevRankingOpenWnd();
}

void CDPSrvr::OnRainbowRaceApplicationOpenWnd( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( !IsValidObj( pUser ) )
		return;

	pUser->AddRainbowRaceApplicationOpenWnd();
}
void CDPSrvr::OnRainbowRaceApplication( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( !IsValidObj( pUser ) )
		return;
	
	CRainbowRaceMng::GetInstance()->SetApplicationUser( pUser );
}
void CDPSrvr::OnRainbowRaceMiniGamePacket( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( !IsValidObj( pUser ) )
		return;
	
	BOOL bExt;
	ar >> bExt;

	__MINIGAME_PACKET* pMiniGamePacket;
	if( bExt )	pMiniGamePacket = new __MINIGAME_EXT_PACKET;
	else		pMiniGamePacket = new __MINIGAME_PACKET;

	pMiniGamePacket->Serialize( ar );
	CRainbowRaceMng::GetInstance()->OnMiniGamePacket( pUser, pMiniGamePacket );
	SAFE_DELETE( pMiniGamePacket );
}

void CDPSrvr::OnRainbowRaceReqFinish( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( !IsValidObj( pUser ) )
		return;

	CRainbowRaceMng::GetInstance()->SetRanking( pUser );
}
#endif // __RAINBOW_RACE

#if __VER >= 13 // __HOUSING
void CDPSrvr::OnHousingSetupFurniture( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( !IsValidObj( pUser ) )
		return;

	HOUSINGINFO housingInfo;
	housingInfo.Serialize( ar );

	// �÷��̾ �濡 �־�� �ϰ� �ڽ��� ���̾ �� �ִ� ��츸 ����...
	CHousingMng::GetInstance()->ReqSetupFurniture( pUser, housingInfo );
}

void CDPSrvr::OnHousingSetVisitAllow( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( !IsValidObj( pUser ) )
		return;

	DWORD dwPlayerId;
	BOOL  bAllow;

	ar >> dwPlayerId >> bAllow;

	CHousingMng::GetInstance()->ReqSetAllowVisit( pUser, dwPlayerId, bAllow );
}

void CDPSrvr::OnHousingVisitRoom( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( !IsValidObj( pUser ) )
		return;

	DWORD dwPlayerId;
	ar >> dwPlayerId;

	CHousingMng::GetInstance()->SetVisitRoom( pUser, dwPlayerId );
}

void CDPSrvr::OnHousingVisitableList( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( !IsValidObj( pUser ) )
		return;

	CHousingMng::GetInstance()->OnReqVisitableList( pUser );
}

void CDPSrvr::OnHousingGoOut( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( !IsValidObj( pUser ) )
		return;

	CHousingMng::GetInstance()->GoOut( pUser );
}
#endif // __HOUSING

#if __VER >= 13 // __QUEST_HELPER
void CDPSrvr::OnReqQuestNPCPos( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long )
{
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( !IsValidObj( pUser ) )
		return;

	char szCharKey[64] = {0,};
	ar.ReadString( szCharKey, 64 );

	LPCHARACTER lpChar = prj.GetCharacter( szCharKey );
	if( lpChar )
	{
		if( pUser->GetWorld() && pUser->GetWorld()->GetID() == lpChar->m_dwWorldId )
			pUser->AddNPCPos( lpChar->m_vPos );
		else
			pUser->AddDefinedText( TID_GAME_QUESTINFO_FAIL );
	}
}
#endif // __QUEST_HELPER

#if __VER >= 13 // __COUPLE_1117
void CDPSrvr::OnPropose( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long )
{
	CUser* pUser	=	g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) ) 
	{
		char szPlayer[MAX_PLAYER]	= { 0,};
		ar.ReadString( szPlayer, MAX_PLAYER );
		CCoupleHelper::Instance()->OnPropose( pUser, szPlayer );
	}
}

void CDPSrvr::OnRefuse( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long )
{
	CUser* pUser	=	g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) ) 
		CCoupleHelper::Instance()->OnRefuse( pUser );
}

void CDPSrvr::OnCouple( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long )
{
	CUser* pUser	=	g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) ) 
		CCoupleHelper::Instance()->OnCouple( pUser );
}

void CDPSrvr::OnDecouple( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long )
{
	CUser* pUser	=	g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) ) 
		CCoupleHelper::Instance()->OnDecouple( pUser );
}
#endif	// __COUPLE_1117

#ifdef __MAP_SECURITY
void CDPSrvr::OnMapKey( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long )
{
	CUser* pUser	=	g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) ) 
	{
		char szFileName[33] = {0,}, szMapKey[33] = {0,};
		ar.ReadString( szFileName, 33 );
		ar.ReadString( szMapKey, 33 );
		g_WorldMng.CheckMapKey( pUser, szFileName, szMapKey );
	}
}
#endif // __MAP_SECURITY

#ifdef __QUIZ
void CDPSrvr::OnQuizEventEntrance( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );

	if( !IsValidObj( pUser ) )
		return;

	int nResult;
	nResult = CQuiz::GetInstance()->EntranceQuizEvent( pUser );
	if( nResult > 0 )
		pUser->AddDefinedText( nResult );
}
void CDPSrvr::OnQuizEventTeleport( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );

	if( !IsValidObj( pUser ) )
		return;

	int nZone;
	ar >> nZone;

	int nResult;
	nResult = CQuiz::GetInstance()->TeleportToQuizEvent( pUser, nZone );
	if( nResult > 0 )
		pUser->AddDefinedText( nResult );
}
#endif // __QUIZ

#if __VER >= 15 // __PETVIS
void CDPSrvr::OnRemoveVis( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	if( !IsValidObj( pUser ) )
		return;

	int nPos;
	ar >> nPos;

	CItemUpgrade::GetInstance()->RemovePetVisItem( pUser, nPos );
}

void CDPSrvr::OnSwapVis( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	if( !IsValidObj( pUser ) )
		return;

	int nPos1, nPos2;
	ar >> nPos1 >> nPos2;

	CItemUpgrade::GetInstance()->SwapVis( pUser, nPos1, nPos2 );
}
#endif // __PETVIS

#if __VER >= 15 // __GUILD_HOUSE
void CDPSrvr::OnBuyGuildHouse( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	if( !IsValidObj( pUser ) )
		return;

	GuildHouseMng->ReqBuyGuildHouse( pUser );
}

void CDPSrvr::OnGuildHousePacket( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	if( !IsValidObj( pUser ) )
		return;

	int nPacketType, nIndex;
	GH_Fntr_Info gfi;

	ar >> nPacketType >> nIndex;
	gfi.Serialize( ar );

	if( nPacketType == GUILDHOUSE_PCKTTYPE_LISTUP )
		return;

	GuildHouseMng->SendWorldToDatabase( pUser, nPacketType, gfi, nIndex );
}

void CDPSrvr::OnGuildHouseEnter( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	if( !IsValidObj( pUser ) )
		return;

	GuildHouseMng->EnteranceGuildHouse( pUser );
}

void CDPSrvr::OnGuildHouseGoOut( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	if( !IsValidObj( pUser ) )
		return;

	GuildHouseMng->GoOutGuildHouse( pUser );
}
#endif // __GUILD_HOUSE

#if __VER >= 15 // __TELEPORTER
void CDPSrvr::OnTeleporterReq( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long )
{
	CUser* pUser	=	g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) ) 
	{
		CHAR	m_szKey[64];
		int		nIndex;
		ar.ReadString(m_szKey, 64);
		ar >> nIndex;
		
		LPCHARACTER lpChar = prj.GetCharacter( m_szKey );
		if( lpChar )
		{
			if( !CNpcChecker::GetInstance()->IsCloseNpc( MMI_TELEPORTER, pUser->GetWorld(), pUser->GetPos() ) )
				return;

			if( (int)( lpChar->m_vecTeleportPos.size() ) <= nIndex )
				return;

			pUser->REPLACE( g_uIdofMulti, WI_WORLD_MADRIGAL, lpChar->m_vecTeleportPos[nIndex], REPLACE_NORMAL, nDefaultLayer );
		}
	}
}
#endif // __TELEPORTER

#if __VER >= 15 // __IMPROVE_QUEST_INTERFACE
void CDPSrvr::OnCheckedQuest( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		int nQuestid;
		BOOL bCheck;
		ar >> nQuestid >> bCheck;
		if( bCheck )
		{
			if( pUser->m_nCheckedQuestSize >= MAX_CHECKED_QUEST )
				return;

			for( int i = 0; i < pUser->m_nCheckedQuestSize; ++i )
			{
				if( pUser->m_aCheckedQuest[ i ] == nQuestid )
				{
					for( int j = i; j < pUser->m_nCheckedQuestSize -1; ++j )
						pUser->m_aCheckedQuest[ j ] = pUser->m_aCheckedQuest[ j+1 ];
					pUser->m_aCheckedQuest[ --pUser->m_nCheckedQuestSize ] = 0;
					break;
				}
			}
			pUser->m_aCheckedQuest[ pUser->m_nCheckedQuestSize++ ] = nQuestid;
		}
		else
		{
			if( pUser->m_nCheckedQuestSize <= 0 )
				return;

			for( int i = 0; i < pUser->m_nCheckedQuestSize; ++i )
			{
				if( pUser->m_aCheckedQuest[ i ] == nQuestid )
				{
					for( int j = i; j < pUser->m_nCheckedQuestSize -1; ++j )
						pUser->m_aCheckedQuest[ j ] = pUser->m_aCheckedQuest[ j+1 ];
					pUser->m_aCheckedQuest[ --pUser->m_nCheckedQuestSize ] = 0;
					break;
				}
			}
		}
		pUser->AddCheckedQuest();
	}
}
#endif // __IMPROVE_QUEST_INTERFACE

#if __VER >= 15 // __CAMPUS
void CDPSrvr::OnInviteCampusMember( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long )
{
	CUser* pRequest = g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pRequest ) )
	{
		u_long idTarget;
		ar >> idTarget;

		CUser* pTarget = g_UserMng.GetUserByPlayerID( idTarget );
		if( IsValidObj( pTarget ) )
			CCampusHelper::GetInstance()->OnInviteCampusMember( pRequest, pTarget );
	}
}

void CDPSrvr::OnAcceptCampusMember( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long )
{
	CUser* pTarget = g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pTarget ) )
	{
		u_long idRequest;
		ar >> idRequest;

		CUser* pRequest = g_UserMng.GetUserByPlayerID( idRequest );
		if( IsValidObj( pRequest ) )
			CCampusHelper::GetInstance()->OnAcceptCampusMember( pRequest, pTarget );
	}
}

void CDPSrvr::OnRefuseCampusMember( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long )
{
	CUser* pTarget = g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pTarget ) )
	{
		u_long idRequest;
		ar >> idRequest;

		CUser* pRequest = g_UserMng.GetUserByPlayerID( idRequest );
		if( IsValidObj( pRequest ) )
			pRequest->AddDefinedText( TID_GAME_TS_REFUSAL, "\"%s\"", pTarget->GetName() );
	}
}

void CDPSrvr::OnRemoveCampusMember( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long )
{
	CUser* pRequest = g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pRequest ) )
	{
		u_long idTarget;
		ar >> idTarget;

		if( idTarget > 0 )
			CCampusHelper::GetInstance()->OnRemoveCampusMember( pRequest, idTarget );
	}
}
#endif // __CAMPUS




//	mulcom	BEGIN100405	���� ��ȣ�� �η縶��
void	CDPSrvr::OnItemSelectAwakeningValue( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long )
{
	CUser* pRequest = g_UserMng.GetUser( dpidCache, dpidUser );

	if( IsValidObj( pRequest ) == TRUE )
	{
		DWORD	dwItemObjID		= 0;
		DWORD	dwSerialNumber	= 0;
		BYTE	bySelectFlag	= 0;

		ar >> dwItemObjID;
		ar >> dwSerialNumber;
		ar >> bySelectFlag;

		pRequest->SelectAwakeningValue( dwItemObjID, dwSerialNumber, bySelectFlag );
	}
	else
	{
		WriteLog( "pUser is invalid in OnItemSelectAwakeningValue function." );
	}
}
//	mulcom	END100405	���� ��ȣ�� �η縶��

#ifdef __GUILD_HOUSE_MIDDLE
void CDPSrvr::OnGuildHouseTenderMainWnd( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) == TRUE )
	{
		DWORD dwGHType;
		OBJID objNpcId;
		ar >> dwGHType >> objNpcId;

		GuildHouseMng->ReqTenderGuildHouseList( pUser, dwGHType, objNpcId );
	}
}

void CDPSrvr::OnGuildHouseTenderInfoWnd( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) == TRUE )
	{
		OBJID objGHId;
		ar >> objGHId;

		GuildHouseMng->ReqTenderGuildHouseInfo( pUser, objGHId );
	}
}

void CDPSrvr::OnGuildHouseTenderJoin( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long )
{
	CUser* pUser = g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) == TRUE )
	{
		OBJID objGHId;
		int nTenderPerin, nTenderPenya;
		ar >> objGHId >> nTenderPerin >> nTenderPenya;
		
		GuildHouseMng->OnGuildHouseTenderJoin( pUser, objGHId, nTenderPerin, nTenderPenya );
	}
}
#endif // __GUILD_HOUSE_MIDDLE
