#if !defined(AFX_PROJECT_H__3C837668_F3CC_430F_87E6_792AF43D7626__INCLUDED_)
#define AFX_PROJECT_H__3C837668_F3CC_430F_87E6_792AF43D7626__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "defineNeuz.h"
#include "ModelMng.h"
#include "TerrainMng.h"
#include "ObjMap.h" 
#include "ProjectCmn.h"
#include "Script.h" 
#include "guildquest.h"

#include "partyquest.h"
#include <set>
using	namespace	std;

#ifdef __EVE_MINIGAME
#include "MiniGame.h"
#endif // __EVE_MINIGAME

#if __VER >= 9 // __ULTIMATE
#include "UltimateWeapon.h"
#endif // __ULTIMATE

#ifdef __TRADESYS
#include "Exchange.h"
#endif // __TRADESYS

#if __VER >= 9 && defined(__WORLDSERVER) // __EVENTLUA && __WORLDSERVER
#include "EventLua.h"
#endif // __EVENTLUA && __WORLDSERVER

#ifdef __IMPROVE_MAP_SYSTEM
#ifdef __CLIENT
#include "MapInformationManager.h"
#endif // __CLIENT
#endif // __IMPROVE_MAP_SYSTEM

////////////////////////////////////////////////////////////////////////////////////////////////////
// define  
////////////////////////////////////////////////////////////////////////////////////////////////////

#define		MAX_RESPAWN			1536
#define		MAX_MONSTER_PROP	1024
#define		MAX_GUILDAPPELL		5

////////////////////////////////////////////////////////////////////////////////////////////////////
// extern 
////////////////////////////////////////////////////////////////////////////////////////////////////

extern CString GetLangFileName( int nLang, int nType );

////////////////////////////////////////////////////////////////////////////////////////////////////
// struct
////////////////////////////////////////////////////////////////////////////////////////////////////

#define MAX_QUESTPROPITEM  4
#define MAX_QUESTCONDITEM  64
#define MAX_QUESTREMOVE    12

struct QuestState
{
	TCHAR	m_szDesc[ 512 ];
};

#if __VER >= 15 // __IMPROVE_QUEST_INTERFACE
struct QuestGoalData
{
	QuestGoalData( void );

	DWORD m_dwGoalIndex;
	FLOAT m_fGoalPositionX;
	FLOAT m_fGoalPositionZ;
	DWORD m_dwGoalTextID;
};
#endif // __IMPROVE_QUEST_INTERFACE

struct QuestPropItem 
{
	char    m_nSex;
	char	m_nType; 
	int     m_nJobOrItem;
	int		m_nItemIdx;
	int		m_nItemNum;
#ifdef __JEFF_11
	int		m_nAbilityOption;
#endif	//__JEFF_11
#if __VER >= 13 // __CHIPI_QUESTITEM_FLAG
	BYTE	m_byFlag;
#endif // __CHIPI_QUESTITEM_FLAG
#if __VER >= 15 // __IMPROVE_QUEST_INTERFACE
	QuestGoalData m_ItemGoalData;
#endif // __IMPROVE_QUEST_INTERFACE
};

struct QuestProp
{
	WORD	m_wId;
	TCHAR	m_szTitle[ 64 ];
	TCHAR	m_szNpcName[ 32 ];
	WORD	m_nHeadQuest;
	CHAR	m_nQuestType;
	bool	m_bNoRemove; // TRUE�� ��� ���� �Ұ� 

	// ���� - ���� 
	int		m_nParam[ 4 ];
	char    m_nBeginCondPreviousQuestType;
	WORD	m_anBeginCondPreviousQuest[ 6 ];
	WORD	m_anBeginCondExclusiveQuest[ 6 ];
	char	m_nBeginCondJob[ MAX_JOB ];
	char	m_nBeginCondJobNum;
	BYTE	m_nBeginCondLevelMax;
	BYTE	m_nBeginCondLevelMin;
	char	m_nBeginCondParty; 
	char 	m_nBeginCondPartyNumComp; 
	char	m_nBeginCondPartyNum; 
	char	m_nBeginCondPartyLeader; 
	char	m_nBeginCondGuild; 
	char 	m_nBeginCondGuildNumComp; 
	WORD	m_nBeginCondGuildNum; 
	char	m_nBeginCondGuildLeader; 
	QuestPropItem* m_paBeginCondItem; 
	char	m_nBeginCondItemNum;
	char	m_nBeginCondSex; 
	int		m_nBeginCondSkillIdx;
	char	m_nBeginCondSkillLvl;
#if __VER >= 8 // __S8_PK
	QuestPropItem* m_paBeginCondNotItem; 
	char	m_nBeginCondNotItemNum;
	int		m_nBeginCondPKValue;
#else // __VER >= 8 // __S8_PK
	char	m_nBeginCondKarmaComp;
	int		m_nBeginCondKarmaPoint;
	char	m_nBeginCondChaotic; 
#endif // __VER >= 8 // __S8_PK
	int		m_nBeginCondDisguiseMoverIndex;
	// ���� - �߰� 
	int  	m_nBeginSetAddItemIdx[ 4 ];
	char	m_nBeginSetAddItemNum[ 4 ];
	int		m_nBeginSetAddGold;
	int     m_nBeginSetDisguiseMoverIndex;
#if __VER >= 9	// __PET_0410
	int		m_nBeginCondPetLevel;
	int		m_nBeginCondPetExp;
#endif	// __PET_0410
#if __VER >= 12 // __MOD_TUTORIAL
	int		m_nBeginCondTutorialState;
#endif	// __MOD_TUTORIAL
#if __VER >= 15 // __CAMPUS
	int		m_nBeginCondTSP;
#endif // __CAMPUS
	// ���� - ����  
	int     m_nEndCondLimitTime; //  ����Ʈ ���� ���� �ð� 
	QuestPropItem* m_paEndCondItem; 
	char	m_nEndCondItemNum;
	int     m_nEndCondKillNPCIdx[ 2 ]; // �׿����� NPC �ε��� - �� 2�� 
	int     m_nEndCondKillNPCNum[ 2 ]; // �׿����� NPC ���� - �� 2��    
#if __VER >= 15 // __IMPROVE_QUEST_INTERFACE
	QuestGoalData m_KillNPCGoalData[ 2 ]; // �׿��� �� NPC ��ǥ ������
#endif // __IMPROVE_QUEST_INTERFACE
	DWORD   m_dwEndCondPatrolWorld; // �����ؾߵ� �� 
#if __VER >= 15 // __IMPROVE_QUEST_INTERFACE
	DWORD	m_dwPatrolDestinationID;	// ���� ������ �̸��� ã�� ���� ID
	QuestGoalData m_PatrolWorldGoalData; // �����ؾ� �� ���� ��ǥ ������
#endif // __IMPROVE_QUEST_INTERFACE
	CRect   m_rectEndCondPatrol; // �����ؾߵ� ��� ���� 
	CHAR    m_szEndCondCharacter[64]; // ����Ʈ�� �ϼ��� �Ǵ��� ĳ���� Ű(NULL�̸� �ڽ�)
#if __VER >= 15 // __IMPROVE_QUEST_INTERFACE
	QuestGoalData m_MeetCharacterGoalData; // ����Ʈ �ϼ��� �Ǵ��� ĳ���� ��ǥ ������
#endif // __IMPROVE_QUEST_INTERFACE
	CHAR*   m_lpszEndCondMultiCharacter; 
	int		m_nEndCondSkillIdx;
	char	m_nEndCondSkillLvl;
#if __VER >= 8 // __S8_PK
	QuestPropItem* m_paEndCondOneItem;
	char	m_nEndCondOneItemNum;
	int		m_nEndCondGold;
	BYTE	m_nEndCondLevelMin;	
	BYTE	m_nEndCondLevelMax;
	#if __VER >= 10 // __LEGEND	//	10�� ���½ý���	Neuz, World, Trans
	int		m_nEndCondExpPercentMin;	
	int		m_nEndCondExpPercentMax;
	#endif	//__LEGEND	//	10�� ���½ý���	Neuz, World, Trans
#else // __VER >= 8 // __S8_PK
	char	m_nEndCondKarmaComp;
	int		m_nEndCondKarmaPoint;
	char	m_nEndCondChaotic; 
#endif // __VER >= 8 // __S8_PK

#if __VER >= 9	// __PET_0410
	int		m_nEndCondPetLevel;
	int		m_nEndCondPetExp;
#endif	// __PET_0410

	int		m_nEndCondDisguiseMoverIndex; // ���� 
	char	m_nEndCondParty; // ��Ƽ ���� (�ַ�,��Ƽ,���)  
	char 	m_nEndCondPartyNumComp; 
	WORD 	m_nEndCondPartyNum; 
	char	m_nEndCondPartyLeader;
	char	m_nEndCondGuild; 
	char 	m_nEndCondGuildNumComp; 
	WORD	m_nEndCondGuildNum; 
	char	m_nEndCondGuildLeader; 
	BYTE	m_nEndCondState; // ������Ʈ ����  
	BYTE    m_nEndCondCompleteQuestOper; // 0 = or, 1 = and
	WORD	m_nEndCondCompleteQuest[ 6 ]; // �Ϸ� ����Ʈ ����. �ó����� ����Ʈ�� �ʿ� 
	CHAR    m_szEndCondDlgCharKey[ 64 ];
#if __VER >= 15 // __IMPROVE_QUEST_INTERFACE
	QuestGoalData m_DialogCharacterGoalData; // ��ȭ�ؾ� �� ĳ���� ��ǥ ������
#endif // __IMPROVE_QUEST_INTERFACE
	CHAR    m_szEndCondDlgAddKey[ 64 ];
	CHAR    m_szPatrolZoneName[ 64 ];
#if __VER >= 15 // __CAMPUS
	int		m_nEndCondTSP;
#endif // __CAMPUS
	// EndDialog ��ȭ�� ������ ������(������ ����)
	int		m_nDlgRewardItemIdx[ 4 ];
	int		m_nDlgRewardItemNum[ 4 ];
	// ���� - ���� 
	int		m_nEndRemoveItemIdx[ 8 ];
	int		m_nEndRemoveItemNum[ 8 ];
	int		m_nEndRemoveGold;
	int		m_anEndRemoveQuest[ MAX_QUESTREMOVE ];
#if __VER >= 15 // __CAMPUS
	int		m_nEndRemoveTSP;
#endif // __CAMPUS
	// ���� 
	QuestPropItem* m_paEndRewardItem; 
	int		m_nEndRewardItemNum;
	int		m_nEndRewardGoldMin;
	int		m_nEndRewardGoldMax;
	int		m_nEndRewardExpMin;
	int		m_nEndRewardExpMax;
#if __VER >= 8 // __S8_PK
	int		m_nEndRewardPKValueMin;
	int		m_nEndRewardPKValueMax;
#else // __VER >= 8 // __S8_PK
	int     m_nEndRewardKarmaPoint;
	char	m_nEndRewardKarmaStyle; // 0�� ���� ����, 1�� ����, 2�� �߰� 
#endif // __VER >= 8 // __S8_PK
#if __VER >= 9 // __S_9_ADD
	int		m_nEndRewardTeleport;
	D3DXVECTOR3		m_nEndRewardTeleportPos;
#endif // __S_9_ADD
#if __VER >= 9	// __PET_0410
	BOOL	m_bEndRewardPetLevelup;
#endif	// __PET_0410
	int		m_nEndRewardSkillPoint;
	bool    m_bEndRewardHide; // ���� ����� ������ ����
	bool    m_bRepeat;
#if __VER >= 15 // __CAMPUS
	int		m_nEndRewardTSP;
#endif // __CAMPUS

#if defined( __WORLDSERVER ) 
	CHAR*		m_apQuestDialog[ 32 ];
#endif
	QuestState*	m_questState[ 16 ];
};

typedef struct 
{
	TCHAR	m_szSrc[ 64 ];
	TCHAR	m_szDst[ 64 ];

} FILTER,* LPFILTER;

typedef struct tagJOB
{
	TCHAR	szName[ 32 ];
	TCHAR	szEName[ 32 ];
	DWORD	dwJobBase;
	DWORD	dwJobType;

} JOB,* LPJOB;

typedef struct tagSTRUCTURE
{
	TCHAR	szName[ 32 ];
	TCHAR	szEName[ 32 ];
	
} STRUCTURE,* LPSTRUCTURE;


typedef struct tagGUILD_APPELL
{
	TCHAR	szName[ 32 ];
	TCHAR	szEName[ 32 ];
	
} GUILD_APPELL,* LPGUILD_APPELL;

typedef struct tagFxpCharacter
{
	DWORD	dwFxp;
	DWORD	dwFxp2;
} FXPCHARACTER,* LPFXPCHARACTER;

#ifndef __S_ADD_RESTATE
typedef struct tagJobAbility
{
	DWORD	dwStr, dwStrAdd;
	DWORD	dwSta, dwStaAdd;
	DWORD	dwDex, dwDexAdd;
	DWORD	dwInt, dwIntAdd;

} JOBABILITY,* LPJOBABILITY;
#endif // __S_ADD_RESTATE

typedef struct tagATKSTYLE
{
	DWORD	dwParam1, dwParam2;

} ATKSTYLE,* LPATKSTYLE;

typedef struct tagADDEXPPARTY
{
	DWORD	Exp, Level;	//  ���� �߰� ����ġ / ��������
} ADDEXPPARTY,* LPADDEXPPARTY;



struct MotionProp
{
	DWORD	dwID;   
	DWORD	dwAction;
	DWORD	dwMotion;
	DWORD	dwPlay;				// 0:1play    1:cont		2:loop
	DWORD	dwRequireLv;		// ���� ���� 
	DWORD	dwRequireExp;		// �䱸 ����ġ 
	TCHAR	szName[ 32 ];		// ǥ�� �ܾ� 
	TCHAR	szRoot[ 32 ];		// ��Ʈ ��Ī 
	TCHAR	szLink[ 128];		// ��ũ �ܾ� 
	TCHAR	szIconName[ 32 ];	// ������ ���� ���� 
	TCHAR	szDesc[ 128 ];		// ���� 
	CTexture*	pTexture;
};

typedef struct _VENDOR_ITEM 
{
#if __VER >= 11 // __CSC_VER11_3
	DWORD	m_dwItemId;
#endif //__CSC_VER11_3
	int		m_nItemkind3;
	int		m_nItemJob;
	int		m_nUniqueMin;
	int		m_nUniqueMax;
	int		m_nTotalNum ;
	int		m_nMaterialCount;
} VENDOR_ITEM,* LPVENDOR_ITEM;

#ifdef __NPC_BUFF
typedef struct _NPC_BUFF_SKILL
{
	DWORD	dwSkillID;
	DWORD	dwSkillLV;
	int		nMinPlayerLV;
	int		nMaxPlayerLV;
	DWORD	dwSkillTime;
} NPC_BUFF_SKILL;
#endif // __NPC_BUFF

typedef struct tagCHARACTER
{
	CHAR			m_szKey[ 64 ];
	CString			m_strName;
	CHAR			m_szChar[ 64 ];
	CHAR			m_szDialog[ 64 ];
	CHAR			m_szDlgQuest[ 64 ];		// TODO_raiders: ������� �ʴ´�. ���� 
	int				m_nStructure;
	int				m_nEquipNum;
	DWORD			m_dwMoverIdx;
	DWORD			m_dwMusicId;
	DWORD			m_adwEquip[ MAX_HUMAN_PARTS ];
	DWORD			m_dwHairMesh;
	DWORD			m_dwHairColor;
	DWORD			m_dwHeadMesh;
	RANDOM_ITEM		m_randomItem;
	BOOL			m_abMoverMenu[ MAX_MOVER_MENU ];
	CString			m_venderSlot[ 4 ];
	CPtrArray		m_venderItemAry[ 4 ];

	CWordArray		m_awSrcQuest; // �� ĳ���Ͱ� ������ ����Ʈ ���̵� ��� 
	CWordArray		m_awDstQuest; // �� ĳ���Ͱ� ������ ����Ʈ ���̵� ��� 
	CUIntArray		m_anSrcQuestItem; 
	CUIntArray		m_anDstQuestItem; 
	void Clear();

#if __VER >= 11 // __CSC_VER11_3
	int				m_nVenderType;
	CPtrArray		m_venderItemAry2[ 4 ];
#endif //__CSC_VER11_3
#ifdef __NPC_BUFF
	vector<NPC_BUFF_SKILL> m_vecNPCBuffSkill;
#endif // __NPC_BUFF
#ifdef __CHIPI_DYO
	vector<DWORD>	m_vecdwLanguage;
	BOOL			bOutput;
#endif // __CHIPI_DYO
#if __VER >= 13 // __QUEST_HELPER
	DWORD			m_dwWorldId;
	D3DXVECTOR3		m_vPos;
#endif // __QUEST_HELPER
#if __VER >= 15 // __TELEPORTER
	vector<D3DXVECTOR3> m_vecTeleportPos;
#endif // __TELEPORTER
} CHARACTER,* LPCHARACTER;

#ifdef __S1108_BACK_END_SYSTEM

typedef struct	_MONSTER_RESPAWN
{
	char		szMonsterName[32];
	int			nRespawnIndex;
	D3DXVECTOR3	vPos;
	int			nQuantity;
	int			nAggressive;
	int			nRange;
	int			nTime;

	_MONSTER_RESPAWN()
	{	
		szMonsterName[0] = '\0';
		vPos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		nQuantity = 0;
		nAggressive = 0;
		nRange = 0;
		nTime = 0;
		nRespawnIndex = 0;
	}
}
MONSTER_RESPAWN, *PMONSTER_RESPAWN;

typedef struct _MONSTER_PROP
{
	char	szMonsterName[32];
	int		nHitPoint;
	int		nAttackPower;
	int		nDefence;
	int		nExp;
	int		nItemDrop;
	int		nPenya;
	_MONSTER_PROP()
	{
		szMonsterName[0] = '\0';
		nHitPoint = 0;
		nAttackPower = 0;
		nDefence = 0;
		nExp = 0;
		nItemDrop = 0;
		nPenya = 0;
	}
}
MONSTER_PROP, *PMONSTER_PROP;
#endif // __S1108_BACK_END_SYSTEM

#if __VER >= 8  
typedef struct _DIE_PENALTY
{
	int		nLevel;
	int		nValue;
	_DIE_PENALTY()
	{
		nLevel = 0;
		nValue = 0;
	}
} DIE_PENALTY, *PDIE_PENALTY;
#endif //  __VER >= 8  

#if __VER >= 8 // __S8_PK
typedef struct _CHAO_PROPENSITY
{
	DWORD	dwPropensityMin;		/// ���� ��ġ Min
	DWORD	dwPropensityMax;		/// ���� ��ġ Max
	int		nEquipDorpMin;			/// ��������� ��� ���� Min
	int		nEquipDorpMax;			/// ��������� ��� ���� Max
	int		nInvenDorpMin;			/// �κ������� ��� ���� Min
	int		nInvenDorpMax;			/// �κ������� ��� ���� Max
	int		nShop;					/// ���� �̿� ��/��
	int		nBank;					/// â�� �̿� ��/��
	int		nFly;					/// ���� �̿� ��/��
	int		nGuardAttack;			/// ��� ���� ��/��
	int		nVendor;				/// ���λ��� �̿� �� ���� ��/��
	_CHAO_PROPENSITY()
	{
		dwPropensityMin = 0;
		dwPropensityMax = 0;
		nEquipDorpMin = 0;
		nEquipDorpMax = 0;
		nInvenDorpMin = 0;
		nInvenDorpMax = 0;
		nShop = 1;
		nBank = 1;
		nFly = 1;
		nGuardAttack = 1;
		nVendor = 1;
	}
} CHAO_PROPENSITY, *PCHAO_PROPENSITY;
typedef struct _PK_SETTING
{
	DWORD	dwGeneralColor;			/// �� : �Ϲ�(���)
	DWORD	dwReadyColor;			/// �� : ��ī��(��ũ��)
	DWORD	dwChaoColor;			/// �� : ī��(�����)
	int		nGeneralAttack;			/// �Ϲ����� ���������� ��ī���� �Ǵ� �ð�
	int		nReadyAttack;			/// ��ī���� ���������� ��ī���� �Ǵ� �ð�
	int     nLimitLevel;			/// ī���� �Ҽ� �ִ� ����
	int		nDiePropensityMin;		/// ������ ����ġ ���̴� Min
	int		nDiePropensityMax;		/// ������ ����ġ ���̴� Max
	map<int, CHAO_PROPENSITY> mapPropensityPenalty;	/// �����ġ �ܰ躰 Penalty
	map<int, DWORD> mapLevelExp;					/// ������ ���� ����ġ
	_PK_SETTING()
	{
		dwGeneralColor = 0;
		dwReadyColor = 0;
		dwChaoColor = 0;
		nGeneralAttack = 0;
		nReadyAttack = 0; 
		nLimitLevel = 0;
		nDiePropensityMin = 0;
		nDiePropensityMax = 0;
		mapPropensityPenalty.clear();
		mapLevelExp.clear();
	}
} PK_SETTING, *PPK_SETTING;
#endif // __VER >= 8 // __S8_PK

////////////////////////////////////////////////////////////////////////////////////////////////////
// enum 
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
// macro and inline 
////////////////////////////////////////////////////////////////////////////////////////////////////

inline void LOG_RANGE( const char* szMsg, int nMin, int nMax, int nIndex )
{
	LPCTSTR szErr = Error( _T( "%s min:%d, max:%d, index:%d" ), szMsg, nMin, nMax, nIndex ); 
	//ADDERRORMSG( szErr ); 
}

inline void LOG_CALLSTACK()
{
#ifdef _DEBUG
	__asm int 3
#endif

#if defined(__INTERNALSERVER)
	int *p = NULL;		// �ݽ����� �����ϱ� ���� ���δ�
	*p = 1;
#endif
}

#define VERIFY_RANGE( nIndex, nMin, nMax, szMsg, result )  \
	do { \
		if( nIndex < (nMin) || nIndex >= (nMax) ) \
		{  \
			LOG_RANGE( szMsg, nMin, nMax, nIndex ); \
			return (result); \
		}  \
	} while (0)

#define VERIFY_RANGE_ASSERT( nIndex, nMin, nMax, szMsg, result )  \
	do { \
		if( nIndex < (nMin) || nIndex >= (nMax) ) \
		{  \
			LOG_RANGE( szMsg, nMin, nMax, nIndex ); \
			LOG_CALLSTACK(); \
			return (result); \
		}  \
	} while (0)


////////////////////////////////////////////////////////////////////////////////////////////////////
// KARMAPROP 
////////////////////////////////////////////////////////////////////////////////////////////////////

#if __VER < 8 // __S8_PK
const int MAX_KARMAPROP = 13;

enum SLAUGHTER_GRADE
{
	SLAUGHTER_NORMAL,			// �Ϲ����� 
	SLAUGHTER_SEMI_CHAOTIC,		// �� ī�� 
	SLAUGHTER_CHAOTIC,			// ī�� 
};

// ī������ ������Ƽ 
struct KarmaProp
{
	int		nGrade;					// ���
	TCHAR	szName[64];				// ��Ī 
	DWORD   dwKarmaRecoverPoint;	// ī���� ȸ�� ��ġ (�� ����)
	int		nKarmaPoint;			// ����Ʈ
	DWORD	dwKarmaGrade;			// ���� ��� ( 0 - �Ϲ�, 1 - ��ī��, 2 - ī�� )
	DWORD   dwColor;				// ��Ī ����
	FLOAT   fDiscountRate;			// ���� ��ǰ ���� ����
	FLOAT   fSellPenaltyRate;		// ���� ��ǰ �Ǹ� �г�Ƽ
	BOOL    bGuardReaction;			// ��� ����
	int		nSubtractExpRate;		// ����ġ �϶��� 
	int		nDropGoldPercent;		// ���� ����� ����� ���� 
	int		nDropItem;				// ����� ������ ���� 
	int		nDropPercent;			// ������ ����� Ȯ�� 
	int		nKarmaRecoverNum;		// ī���� ȸ����ġ
	DWORD	dwStatLimitTime;		// ���� ���� �ð�
	int		nStatLimitNum;			// ���� ���� ����
	int		nStatLimitRate;			// ���� ���� ����
	TCHAR	szCommand [128];		// ����
};
#endif // __VER < 8 // __S8_PK

#ifdef __WORLDSERVER
#define	MAX_GIFTBOX_ITEM	128
#ifndef __STL_GIFTBOX_VECTOR
#define	MAX_GIFTBOX			256
#endif // __STL_GIFTBOX_VECTOR
typedef	struct	_GIFTBOX
{
	DWORD	dwGiftbox;
	int		nSum;
	int		nSize;
	DWORD	adwItem[MAX_GIFTBOX_ITEM];
	DWORD	adwProbability[MAX_GIFTBOX_ITEM];
	int		anNum[MAX_GIFTBOX_ITEM];
	BYTE	anFlag[MAX_GIFTBOX_ITEM];
	int		anSpan[MAX_GIFTBOX_ITEM];
	int		anAbilityOption[MAX_GIFTBOX_ITEM];
	/*
#ifdef __GIFTBOX0213
	BOOL	bGlobal;
	int		anTotal[MAX_GIFTBOX_ITEM];
#endif	// __GIFTBOX0213
	*/
}	GIFTBOX,	*PGIFTBOX;

typedef struct	_GIFTBOXRESULT
{
	DWORD	dwItem;
	int		nNum;
	BYTE nFlag;
	int nSpan;
	int	nAbilityOption;
	_GIFTBOXRESULT()
		{
			dwItem	= 0;
			nNum	= 0;
			nFlag	= 0;
			nSpan	= 0;
			nAbilityOption	= 0;
		}
}	GIFTBOXRESULT,	*PGIFTBOXRESULT;

/*
#ifdef __GIFTBOX0213
class CItemElem;
class CDPMng;
#endif	// __GIFTBOX0213
*/
class	CGiftboxMan
{
private:
#ifdef __STL_GIFTBOX_VECTOR
	vector<GIFTBOX> m_vGiftBox;
#else // __STL_GIFTBOX_VECTOR
	int		m_nSize;
	GIFTBOX	m_giftbox[MAX_GIFTBOX];
#endif // __STL_GIFTBOX_VECTOR
	map<DWORD, int>	m_mapIdx;
	int	m_nQuery;

public:
	CGiftboxMan();
	virtual	~CGiftboxMan()	{}
	
	static	CGiftboxMan* GetInstance( void );

	/*
#ifdef __GIFTBOX0213
	BOOL	AddItem( DWORD dwGiftbox, DWORD dwItem, DWORD dwProbability, int nNum, BYTE nFlag = 0, int nTotal = 0 );
	void	Upload( CDPMng* pdp );
	void	Query( CDPMng* pdp, DWORD dwGiftbox, DWORD dwItem, int nNum, u_long idPlayer, CItemElem* pItemElem );
	void	Restore( CDPMng* pdp, DWORD dwGiftbox, DWORD dwItem );
	BOOL	Open( DWORD dwGiftbox, LPDWORD pdwItem, int* pnNum, u_long idPlayer, CItemElem* pItemElem );
	BOOL	OpenLowest( DWORD dwGiftbox, LPDWORD pdwItem, int* pnNum );
#else	// __GIFTBOX0213
	*/
	BOOL	AddItem( DWORD dwGiftbox, DWORD dwItem, DWORD dwProbability, int nNum, BYTE nFlag = 0, int nSpan	= 0, int nAbilityOption = 0 );
	BOOL	Open( DWORD dwGiftBox, PGIFTBOXRESULT pGiftboxResult );
//#endif	// __GIFTBOX0213
	void	Verify( void );
};
#endif	// __WORLDSERVER

#define	MAX_ITEM_PER_PACK	16
#define	MAX_PACKITEM		512

typedef	struct	_PACKITEMELEM
{
	DWORD dwPackItem;
	int		nSize;
	int		nSpan;
	DWORD	adwItem[MAX_ITEM_PER_PACK];
	int		anAbilityOption[MAX_ITEM_PER_PACK];
	int		anNum[MAX_ITEM_PER_PACK];
}	PACKITEMELEM,	*PPACKITEMELEM;

class CPackItem
{
private:
	int		m_nSize;
	PACKITEMELEM	m_packitem[MAX_PACKITEM];
	map<DWORD, int>	m_mapIdx;

public:
	CPackItem();
	virtual	~CPackItem()	{}

	static	CPackItem*	GetInstance( void );

	BOOL	AddItem( DWORD dwPackItem, DWORD dwItem, int nAbilityOption, int nNum );
	PPACKITEMELEM	Open( DWORD dwPackItem );
};



#define	MAX_ITEMAVAIL	32
typedef struct	__ITEMAVAIL
{
	int		nSize;
	int		anDstParam[MAX_ITEMAVAIL];
	int		anAdjParam[MAX_ITEMAVAIL];
	__ITEMAVAIL()
	{
		nSize	= 0;
	}
}	ITEMAVAIL, *PITEMAVAIL;

typedef	struct	__NEWSETITEMAVAIL : public ITEMAVAIL
{
	int		anEquiped[MAX_ITEMAVAIL];
}	NEWSETITEMAVAIL, *PNEWSETITEMAVAIL;

#define	MAX_SETITEM_STRING		64
#define	MAX_SETITEM_ELEM		8
class CSetItem
{
public:
	int		m_nId;
	char	m_pszString[MAX_SETITEM_STRING];
	int		m_nElemSize;
	int		m_anParts[MAX_SETITEM_ELEM];
	DWORD	m_adwItemId[MAX_SETITEM_ELEM];
	NEWSETITEMAVAIL		m_avail;

public:
//	Constructions
	CSetItem( int nId, const char* pszString );
	virtual	~CSetItem()		{}

	BOOL	AddSetItemElem( DWORD dwItemId, int nParts );
	BOOL	AddItemAvail( int nDstParam, int nAdjParam, int nEquiped );
	void	SortItemAvail( void );

	void	GetItemAvail( PITEMAVAIL pItemAvail, int nEquiped, BOOL bAll );
	char* GetString( void )		{	return m_pszString;		}
};

class CSetItemFinder
{
private:
	map<DWORD, CSetItem*>		m_mapItemId;
	map<int, CSetItem*>		m_mapSetId;
public:
	CSetItemFinder()	{}
	virtual	~CSetItemFinder()	{	Free();	}

	void	AddSetItem( CSetItem* pSetItem );
	CSetItem*	GetSetItem( int nSetItemId );
	CSetItem*	GetSetItemByItemId( DWORD dwItemId );
	void	Free();
	static	CSetItemFinder*	GetInstance( void );
};

typedef struct	_SETITEMAVAIL
{
	int		nHitRate;
	int		nBlock;
	int		nMaxHitPointRate;
	int		nAddMagic;
	int		nAdded;
}	SETITEMAVAIL, *PSETITEMAVAIL;

#define	MAX_PIERCING_DSTPARAM	32
typedef struct _PIERCINGAVAIL
{
	DWORD	dwItemId;
	int		nSize;
	int		anDstParam[MAX_PIERCING_DSTPARAM];
	int		anAdjParam[MAX_PIERCING_DSTPARAM];
} PIERCINGAVAIL, *PPIERCINGAVAIL;

#define	MAX_PIERCING_MATERIAL	128
class CPiercingAvail
{
private:
	int		m_nSize;
	map<DWORD, int>		m_mapIdx;
	PIERCINGAVAIL	m_pPiercingAvail[MAX_PIERCING_MATERIAL];

public:
	CPiercingAvail();
	~CPiercingAvail();
	BOOL	AddPiercingAvail( DWORD dwItemId, int nDstParam, int nAdjParam );
	PPIERCINGAVAIL	GetPiercingAvail( DWORD dwItemId );

	static	CPiercingAvail*	GetInstance( void );
};



#define	MAX_RANDOMOPTITEMSTRING		32
typedef	struct	_RANDOMOPTITEM
{
	int	nId;
	int nLevel;
	char pszString[MAX_RANDOMOPTITEMSTRING];
	DWORD	dwProbability;
	ITEMAVAIL	ia;
	_RANDOMOPTITEM()
		{	nId	= 0;	nLevel	= 0;	*pszString	= '\0';	dwProbability	= 0;	}
}	RANDOMOPTITEM, *PRANDOMOPTITEM;

#if __VER >= 11 // __SYS_IDENTIFY
#define	MAX_RANDOMOPTITEM		256
#else	// __SYS_IDENTIFY
#define	MAX_RANDOMOPTITEM		1024
#endif	// __SYS_IDENTIFY

class CRandomOptItemGen
{
	int		m_nSize;
	RANDOMOPTITEM	m_aRandomOptItem[MAX_RANDOMOPTITEM];
	map<int, int>	m_mapid;

#if __VER >= 10 // __LEGEND	//	10�� ���½ý���	Neuz, World, Trans
#if __VER >= 15 // __HERO129_VER15				// 15�� ����� ����Ȯ��
	int		m_anIndex[MAX_MONSTER_LEVEL];
	#else	// 15�� ����� ����Ȯ��
	int		m_anIndex[MAX_LEGEND_LEVEL];
	#endif	// 15�� ����� ����Ȯ��
#else //__LEGEND	//	10�� ���½ý���	Neuz, World, Trans
	int		m_anIndex[MAX_LEVEL];
#endif	//__LEGEND	//	10�� ���½ý���	Neuz, World, Trans

public:
//	Constructions
	CRandomOptItemGen();
	virtual	~CRandomOptItemGen()	{	Free();	}

	BOOL	AddRandomOption( PRANDOMOPTITEM pRandomOptItem );
	PRANDOMOPTITEM	GetRandomOptItem( int nId );
	const char*	GetRandomOptItemString( int nId );
	int		GenRandomOptItem( int nLevel, FLOAT fPenalty, ItemProp* pItemProp, DWORD dwClass );
	void	Arrange( void );
	void	Free( void );

	static	CRandomOptItemGen*	GetInstance();
};


////////////////////////////////////////////////////////////////////////////////////////////////////
// typedef 
////////////////////////////////////////////////////////////////////////////////////////////////////

typedef map<u_long, string>	ULONG2STRING;
typedef map<string, u_long>	STRING2ULONG;

////////////////////////////////////////////////////////////////////////////////////////////////////
// CProject
////////////////////////////////////////////////////////////////////////////////////////////////////

class CObj;
class CCtrl;
class CSfx;
class CItem;
class CMover;
class CShip;
#ifdef __WORLDSERVER
class CUser;
#endif

class CProject  
{ 
public:
	CProject();
	virtual ~CProject();

private:
#if __VER < 11 // __SYS_PLAYER_DATA
	ULONG2STRING				m_ulong2str;
	STRING2ULONG				m_str2ulong;
#endif	// __SYS_PLAYER_DATA
	map<string, DWORD>			m_mapII;
	map<string, DWORD>			m_mapMVI;
	map<string, DWORD>			m_mapCtrl;
#ifdef __CLIENT
	CDWordArray					m_aStateQuest; // ������Ʈ ��� ���θ� üũ�� �迭 
#endif

#if __VER >= 15 /* __IMPROVE_QUEST_INTERFACE */ && defined( __CLIENT )
	map< int, CString >			m_mapQuestDestination;		// ����Ʈ ������ ����
	map< int, CString >			m_mapPatrolDestination;		// ���� ������ �̸�
#endif // defined( __IMPROVE_QUEST_INTERFACE ) && defined( __CLIENT )

public:
	static FLOAT				m_fItemDropRate;			// ���� ������ ��ӷ�
	static FLOAT				m_fGoldDropRate;			// ���� ��� ��ӷ�
	static FLOAT				m_fMonsterExpRate;			// ���� ����ġ��
	static FLOAT				m_fMonsterHitRate;			// ���� ���ݷ�
	static FLOAT				m_fShopCost;				// ��������
	static FLOAT				m_fSkillExpRate;		
	static DWORD				m_dwVagSP;					// ����� ��ų�� �������� �ʿ��� SP����Ʈ
	static DWORD				m_dwExpertSP;				// 1������ ��ų�� �������� �ʿ��� SP����Ʈ
	static DWORD				m_dwProSP;					// 2������ ��ų�� �������� �ʿ��� SP����Ʈ

	map<u_long, CMover*>		m_idPlayerToUserPtr;
	CObjMap						m_objmap;
	CModelMng					m_modelMng;
	int							m_nMoverPropSize;
	MoverProp*					m_pPropMover;				// m_aPropMover�迭�� �޸� ħ���� �־ ������.04.10.14
	CFixedArray< ItemProp >		m_aPartySkill;
	CFixedArray< CtrlProp >		m_aPropCtrl;
	CFixedArray< MotionProp >	m_aPropMotion;
	CFixedArray< ItemProp >		m_aPropItem;
	CFixedArray< ItemProp >		m_aPropSkill;
	CFixedArray< AddSkillProp > m_aPropAddSkill;
	CFixedArray< tagColorText >	m_colorText;
#ifdef __FILTER_0705
	map<string, string>	m_mapAlphaFilter;
	map<string, string>	m_mapNonalphaFilter;
#else	// __FILTER_0705
	CFixedArray< FILTER >		m_aWordFilter;
#endif	// __FILTER_0705
	CFixedArray< QuestProp >	m_aPropQuest ;
	CFixedArray<GUILDQUESTPROP>	m_aPropGuildQuest;
	CMapStringToPtr				m_mapCharacter;
	JobProp						m_aPropJob[MAX_JOB];
	ItemProp*					m_aJobSkill[ MAX_JOB ][ 40 ];
	DWORD						m_aJobSkillNum[ MAX_JOB ];
	JOB							m_aJob[ MAX_JOB ];
	JOBITEM						m_jobItem[ MAX_JOBITEM ];
	STRUCTURE					m_aStructure[ MAX_STRUCTURE ];
	GUILD_APPELL				m_aGuildAppell[ MAX_GUILDAPPELL ];
	EXPCHARACTER				m_aExpCharacter[ MAX_EXPCHARACTER ];
	FXPCHARACTER				m_aFxpCharacter[ MAX_FXPCHARACTER ];
#ifndef __S_ADD_RESTATE
	JOBABILITY					m_aJobAbillity[ MAX_JOB + 1 ];
#endif // __S_ADD_RESTATE
	DWORD						m_aExpLPPoint[ MAX_EXPLPPOINT ];
	DWORD						m_aExpSkill[ MAX_EXPSKILL ];
	EXPPARTY					m_aExpParty[MAX_PARTYLEVEL];
	ADDEXPPARTY					m_aAddExpParty[ MAX_ADDEXPPARTY ];
	CPtrArray					m_itemKindAry[ MAX_ITEM_KIND3 ];
	int							m_aExpUpItem[6][11];	// +0 ~ +10������ �߰��ɷ�ġ.
	DWORD						m_adwExpDropLuck[122][11];
	SETITEMAVAIL				m_aSetItemAvail[11];
	SIZE						m_minMaxIdxAry[MAX_ITEM_KIND3][MAX_UNIQUE_SIZE];
	map<int, PARTYQUESTPROP>	m_propPartyQuest;
#if __VER >= 8  
	vector< DIE_PENALTY >		m_vecRevivalPenalty;
	vector< DIE_PENALTY >		m_vecDecExpPenalty;
	vector< DIE_PENALTY >		m_vecLevelDownPenalty;
#endif //  __VER >= 8  
#if __VER >= 8 // __S8_PK
	PK_SETTING					m_PKSetting;
#else // __VER >= 8 // __S8_PK
	KarmaProp					m_aKarmaProp[ MAX_KARMAPROP ];
#endif // __VER >= 8 // __S8_PK
	set<DWORD>					m_setExcept;

#ifdef __CLIENT
	CTerrainMng					m_terrainMng;
	CMapStringToString			m_mapHelp;
	CMapStringToString			m_mapWordToolTip;	
#endif
	
#ifdef __S1108_BACK_END_SYSTEM
	FLOAT						m_fMonsterRebirthRate;	// ���� ��������(�ð�)
	FLOAT						m_fMonsterHitpointRate;	// ���� ����·�
	FLOAT						m_fMonsterAggressiveRate;	// �������ͷ�
	FLOAT						m_fMonsterRespawnRate;	// ���� ��������(���ͼ���)
	MONSTER_PROP				m_aMonsterProp[MAX_MONSTER_PROP];	// Monster Prop
	MONSTER_PROP				m_aAddProp[MAX_MONSTER_PROP];	// Monster Prop
	char						m_aRemoveProp[MAX_MONSTER_PROP][32];	// Monster Prop
	int							m_nAddMonsterPropSize;
	int							m_nRemoveMonsterPropSize;
	int							m_nMonsterPropSize;
	char						m_chGMChat[10][256];
#endif // __S1108_BACK_END_SYSTEM
	
#if __VER >= 9 // __Y_ADV_ENCHANT_EFFECT
	int				m_nEnchantLimitLevel[3];
	float			m_fEnchantLevelScal[2][10];
#endif //__Y_ADV_ENCHANT_EFFECT

#ifdef __EVE_MINIGAME
	CMiniGame		m_MiniGame;			// �̴ϰ���
#endif // __EVE_MINIGAEM

#if __VER >= 9 // __ULTIMATE
	CUltimateWeapon	m_UltimateWeapon;
#endif //__ULTIMATE
	
#ifdef __TRADESYS
	CExchange m_Exchange;
#endif // __TRADESYS

#if __VER >= 9 && defined(__WORLDSERVER) // __EVENTLUA && __WORLDSERVER
	CEventLua m_EventLua;
#endif // __EVENTLUA && __WORLDSERVER
	
#ifdef __YS_CHATTING_BLOCKING_SYSTEM
#ifdef __CLIENT
	enum { BLOCKING_NUMBER_MAX = 1000 };
	set< CString > m_setBlockedUserID;
#endif // __CLIENT
#endif // __YS_CHATTING_BLOCKING_SYSTEM

#ifdef __CLIENT
#ifdef __SHOP_COST_RATE
public:
	float	m_fShopBuyRate;
	float	m_fShopSellRate;
#endif // __SHOP_COST_RATE
#endif // __CLIENT

#ifdef __IMPROVE_MAP_SYSTEM
#ifdef __CLIENT
	CMapInformationManager m_MapInformationManager;
	BOOL m_bMapTransparent;
#endif // __CLIENT
#endif // __IMPROVE_MAP_SYSTEM

public:
#if __VER >= 9 // __Y_ADV_ENCHANT_EFFECT	
	BOOL			LoadPropEnchant( LPCTSTR lpszFileName );
#endif //__Y_ADV_ENCHANT_EFFECT
	static void		ReadConstant( CScript& script );
	static BOOL		LoadConstant( LPCTSTR lpszFileName );
	static void		SetGlobal( UINT type, float fValue );

	PSETITEMAVAIL	GetSetItemAvail( int nAbilityOption );
	int				GetMinIdx( int nItemKind3, DWORD dwItemRare );
	int				GetMaxIdx( int nItemKind3, DWORD dwItemRare );
	ObjProp*		GetProp( int nType, int nIndex );
	JobProp*		GetJobProp( int nIndex );
	GUILDQUESTPROP*	GetGuildQuestProp( int nQuestId );
	PARTYQUESTPROP*	GetPartyQuestProp( int nQuestId );
	BOOL			IsGuildQuestRegion( const D3DXVECTOR3 & vPos );
	CtrlProp*		GetCtrlProp( int nIndex );
	ItemProp*		GetItemProp( int nIndex ); 
	ItemProp*		GetSkillProp( int nIndex ); 
	CPtrArray*		GetItemKindAry( int nKind );
	MoverProp*		GetMoverProp( int nIndex ) ;
	MoverProp*		GetMoverPropEx( int nIndex );
	DWORD			GetSkillPoint( ItemProp* pSkillProp );
	ItemProp*		GetItemProp( LPCTSTR lpszItem );
	MoverProp*		GetMoverProp( LPCTSTR lpszMover ); 
	CtrlProp*		GetCtrlProp( LPCTSTR lpszMover );
	MotionProp*		GetMotionProp ( int nIndex ) { return m_aPropMotion.GetAt( nIndex ); }
//	LPATKSTYLE		GetAttackStyle( int nIndex ) { return &m_aAttackStyle[ nIndex ]; }
	int				GetExpUpItem( DWORD dwItemKind3, int nOption ); 
	AddSkillProp*	GetAddSkillProp( DWORD dwSubDefine, DWORD dwLevel );
	AddSkillProp*	GetAddSkillProp( DWORD dwSubDefine );
	ItemProp*		GetPartySkill ( int nIndex ) { return m_aPartySkill.GetAt( nIndex ); }
	void			LoadDefines();
	void			LoadPreFiles();
	void			LoadStrings();
	BOOL			OpenProject( LPCTSTR lpszFileName );
	BOOL			LoadPropJob( LPCTSTR lpszFileName );
	BOOL			LoadPropMover( LPCTSTR lpszFileName );
	BOOL			LoadPropItem( LPCTSTR lpszFileName, CFixedArray< ItemProp >* apObjProp );
	void			OnAfterLoadPropItem();
	BOOL			LoadPropSfx( LPCTSTR lpszFileName );
	BOOL			LoadPropCtrl( LPCTSTR lpszFileName, CFixedArray<CtrlProp>*	apObjProp);
	BOOL			LoadMotionProp( LPCTSTR lpszFileName );
	BOOL			LoadText( LPCTSTR lpszFileName );
	BOOL			LoadExpTable( LPCTSTR lpszFileName );
	BOOL			LoadPropMoverEx_AI_SCAN( LPCTSTR szFileName, CScript &script, int nVal );
	BOOL			LoadPropMoverEx_AI_BATTLE( LPCTSTR szFileName, CScript &script, int nVal );
	BOOL			LoadPropMoverEx_AI_MOVE( LPCTSTR szFileName, CScript &script, int nVal );
	BOOL			LoadPropMoverEx_AI( LPCTSTR szFileName, CScript &script, int nVal );
	BOOL			LoadPropMoverEx( LPCTSTR szFileName );
	BOOL			LoadJobItem( LPCTSTR szFileName );
	CString			GetLangScript( CScript& script );
	BOOL			LoadCharacter( LPCTSTR szFileName );
	BOOL			LoadEtc( LPCTSTR szFileName );
	BOOL			LoadPropAddSkill( LPCTSTR lpszFileName );
	void			InterpretRandomItem( LPRANDOM_ITEM pRandomItem, CScript& script );
#if __VER >= 8  
	BOOL			LoadScriptDiePenalty( LPCTSTR lpszFileName );
#endif //  __VER >= 8  
#if __VER >= 8 // __S8_PK
	BOOL			LoadScriptPK( LPCTSTR lpszFileName );
#else // __VER >= 8 // __S8_PK
	BOOL			LoadPropKarma( LPCTSTR lpszFileName );
#endif // __VER >= 8 // __S8_PK
	BOOL			LoadPropQuest( LPCTSTR szFileName, BOOL bOptimize = TRUE );
	BOOL			LoadPropGuildQuest( LPCTSTR szFilename );
	BOOL			LoadPropPartyQuest( LPCTSTR szFilename );
	BOOL			LoadDropEvent( LPCTSTR lpszFileName );
	BOOL			LoadGiftbox( LPCTSTR lpszFileName );
	BOOL			LoadPackItem( LPCTSTR lpszFileName );
	BOOL			LoadPiercingAvail( LPCTSTR lpszFileName );
	DWORD			GetTextColor( DWORD dwIndex ); 
	LPCTSTR			GetText( DWORD dwIndex );
	LPCTSTR			GetGuildAppell( int nAppell );
	CCtrl*			GetCtrl( OBJID objid );
	CItem*			GetItem( OBJID objid );
	CMover*			GetMover( OBJID objid );
	CShip*			GetShip( OBJID objid );
	CMover*			GetUserByID( u_long idPlayer );
	LPCHARACTER		GetCharacter( LPCTSTR lpStrKey );
	void			ProtectPropMover();
#if __VER < 11 // __SYS_PLAYER_DATA
	u_long			GetPlayerID( const CHAR* lpszPlayer );
	LPCSTR			GetPlayerString( u_long idPlayer );
	BOOL			SetPlayerID( u_long idPlayer, const CHAR* lpszPlayer );
	BOOL			RemovePlayerID( u_long idPlayer );
#endif	// __SYS_PLAYER_DATA
#if __VER >= 8 // __S8_PK
	DWORD			GetLevelExp( int nLevel );
	CHAO_PROPENSITY GetPropensityPenalty( DWORD dwPropensity );
#else // __VER >= 8 // __S8_PK
	KarmaProp*		GetKarmaProp( int nSlaughter ); 
	KarmaProp*		GetKarmaPropByGrade( int nGrade ); 
	SLAUGHTER_GRADE GetSlaughterGrade( int nSlaughter );
#endif // __VER >= 8 // __S8_PK
	BOOL			LoadExcept( LPCTSTR lpszFilename );
	void			LoadSkill();


#ifdef __EVE_MINIGAME
	BOOL			LoadMiniGame();
#endif // __EVE_MINIGAME

#if __VER >= 9 // __ULTIMATE
	BOOL			LoadUltimateWeapon();
#endif // __ULTIMATE

#ifdef __WORLDSERVER
	CUser*			GetUser( OBJID objid );
	BOOL			SortDropItem( void );
#if __VER >= 12 // __MOD_TUTORIAL
private:
	int			m_nMaxSequence;
public:
	int		GetGuildMaxSeq( void )	{	return m_nMaxSequence;	}
	BOOL		LoadGuideMaxSeq();
#endif	// __MOD_TUTORIAL

#ifdef __JEFF_11_3
	BOOL	LoadServerScript( const char* sFile );
#endif	// __JEFF_11_3
#endif	// __WORLDSERVER

#ifdef __CLIENT
	BOOL			LoadFilter( LPCTSTR lpszFileName );
	BOOL			LoadWordToolTip( LPCTSTR lpszFileName );
	BOOL			LoadHelp( LPCTSTR lpszFileName ); 
	CString			GetHelp( LPCTSTR lpStr );
	CString			GetWordToolTip( LPCTSTR lpStr );
	CSfx*			GetSfx( OBJID objid );
#ifndef __RULE_0615
	// ˬ
	set<string>		m_sInvalidNames;
	BOOL	LoadInvalidName( void );
	BOOL	IsInvalidName( LPCSTR szName );
#endif	// __RULE_0615
#endif	// __CLIENT

#ifdef __RULE_0615
	set<string>		m_sInvalidNames;
	BOOL	LoadInvalidName( void );
	BOOL	IsInvalidName( LPCSTR szName );

	set<char>	m_sAllowedLetter;
#ifdef __VENDOR_1106
	set<char>	m_sAllowedLetter2;
	BOOL	LoadAllowedLetter( BOOL bVendor = FALSE );
#else	// __VENDOR_1106
	BOOL	LoadAllowedLetter( void );
#endif	// __VENDOR_1106
	BOOL	IsAllowedLetter( LPCSTR szName, BOOL bVendor = FALSE );
	void	Formalize( LPSTR szName );
#endif	// __RULE_0615

#ifdef __OUTPUT_INFO_0803
	void			OutputStore( const char* lpStrKey, CMover* pMover  );
#endif	// __OUTPUT_INFO_0803
	void			OutputDropItem( void );

#ifdef __ADDSKILLPROP0811
	void			OutputSkill( void );
#endif	

#ifdef __S1108_BACK_END_SYSTEM
	void			AddMonsterProp( MONSTER_PROP MonsterProp );
	void			RemoveMonsterProp( char* lpszMonsterName );
#endif 

#if __VER >= 15 /* __IMPROVE_QUEST_INTERFACE */ && defined( __CLIENT )
	BOOL LoadQuestDestination( void );
	const CString& GetQuestDestination( DWORD dwKey ) const;
	BOOL LoadPatrolDestination( void );
	const CString& GetPatrolDestination( DWORD dwKey ) const;
#endif // defined( __IMPROVE_QUEST_INTERFACE ) && defined( __CLIENT )

#ifdef __IMPROVE_MAP_SYSTEM
#ifdef __CLIENT
	BOOL LoadPropMapComboBoxData( const CString& strFileName );
#endif // __CLIENT
#endif // __IMPROVE_MAP_SYSTEM
};

#if __VER >= 8 // __S8_PK
inline DWORD CProject::GetLevelExp( int nLevel )
{
	map<int, DWORD>::iterator it = m_PKSetting.mapLevelExp.find( nLevel );
	if( it != m_PKSetting.mapLevelExp.end() )
		return it->second;
	return 4400000;
}
inline CHAO_PROPENSITY CProject::GetPropensityPenalty( DWORD dwPropensity )
{
	map<int, CHAO_PROPENSITY >::iterator it = m_PKSetting.mapPropensityPenalty.begin();
	for( ; it != m_PKSetting.mapPropensityPenalty.end() ; it++ )
	{
		CHAO_PROPENSITY Propensity = it->second;
		if( Propensity.dwPropensityMin <= dwPropensity && dwPropensity <= Propensity.dwPropensityMax )
			return Propensity;
	}
	
	it = m_PKSetting.mapPropensityPenalty.find( 8 );	// ��ã���� ������ 8�ܰ�� ���� // �� �־����
	if( it != m_PKSetting.mapPropensityPenalty.end() )
		return it->second;

	CHAO_PROPENSITY Propensity;
	return Propensity;
}
#else // __VER >= 8 // __S8_PK
inline SLAUGHTER_GRADE CProject::GetSlaughterGrade( int nSlaughter )
{
	KarmaProp* pProp = GetKarmaProp( nSlaughter );

	switch( pProp->dwKarmaGrade )
	{
	case 0: return SLAUGHTER_NORMAL;
	case 1: return SLAUGHTER_SEMI_CHAOTIC;
	case 2: return SLAUGHTER_CHAOTIC;
	default:	ASSERT( FALSE ); break;
	}
	return SLAUGHTER_NORMAL;
}

inline KarmaProp* CProject::GetKarmaProp( int nSlaughter ) 
{ 
	for( int i=0; i<MAX_KARMAPROP-1; i++ )
	{
		if( nSlaughter >= m_aKarmaProp[i].nKarmaPoint )
			return &m_aKarmaProp[i];
	}

	return &m_aKarmaProp[MAX_KARMAPROP-1];
}

inline KarmaProp* CProject::GetKarmaPropByGrade( int nGrade ) 
{ 
	for( int i=0; i<MAX_KARMAPROP; i++ )
	{
		if( nGrade == m_aKarmaProp[i].nGrade )
			return &m_aKarmaProp[i];
	}

	ASSERT( FALSE );
	return NULL;
}
#endif // __VER >= 8 // __S8_PK

inline ItemProp* CProject::GetItemProp( LPCTSTR lpszItem )
{
	map<string, DWORD>::iterator i	= m_mapII.find( lpszItem );
	if( i != m_mapII.end() )
		return GetItemProp( i->second );
	return NULL;
}

inline CtrlProp* CProject::GetCtrlProp( LPCTSTR lpszCtrl )
{
	map<string, DWORD>::iterator i	= m_mapCtrl.find( lpszCtrl );
	if( i != m_mapCtrl.end() )
		return (CtrlProp*)GetCtrlProp( i->second );
	return NULL;
}

inline MoverProp* CProject::GetMoverProp( LPCTSTR lpszMover )
{
	map<string, DWORD>::iterator i	= m_mapMVI.find( lpszMover );
	if( i != m_mapMVI.end() )
		return GetMoverProp( i->second );
	return NULL;
}

inline CtrlProp* CProject::GetCtrlProp( int nIndex ) 
{ 
	VERIFY_RANGE_ASSERT( nIndex, 0, m_aPropCtrl.GetSize(), "GetCtrlProp range_error", NULL );
	return m_aPropCtrl.GetAt( nIndex ); 
}

inline ItemProp* CProject::GetItemProp( int nIndex ) 
{ 
	VERIFY_RANGE_ASSERT( nIndex, 0, m_aPropItem.GetSize(), "GetItemProp range_error", NULL );
	return m_aPropItem.GetAt( nIndex ); 
}

inline ItemProp* CProject::GetSkillProp( int nIndex ) 
{ 
	VERIFY_RANGE( nIndex, 0, m_aPropSkill.GetSize(), "GetSkillProp range_error", NULL );
	return m_aPropSkill.GetAt( nIndex ); 
}

inline CPtrArray* CProject::GetItemKindAry( int nKind )  
{ 
	return &m_itemKindAry[nKind]; 
}

inline MoverProp* CProject::GetMoverProp( int nIndex ) 
{ 
	VERIFY_RANGE( nIndex, 0, m_nMoverPropSize, "GetMoverProp range_error", NULL );
	return m_pPropMover + nIndex;
}

inline MoverProp* CProject::GetMoverPropEx( int nIndex )
{
	if( nIndex < 0 || nIndex >= m_nMoverPropSize )
		return NULL;
	return GetMoverProp( nIndex );
}

inline DWORD CProject::GetTextColor( DWORD dwIndex ) 
{ 
	VERIFY_RANGE( (int)( dwIndex ), 0, m_colorText.GetSize(), "GetTextColor range_error", 0xFFFFFFFF );
	if( m_colorText.GetAt( dwIndex ) == NULL )
		return 0;
	return m_colorText.GetAt( dwIndex )->dwColor; 
}

inline LPCTSTR CProject::GetText( DWORD dwIndex ) 
{ 
	VERIFY_RANGE( (int)( dwIndex ), 0, m_colorText.GetSize(), "GetText range_error", "error" );
	if( m_colorText.GetAt( dwIndex ) == NULL )
		return "";
	return m_colorText.GetAt( dwIndex )->lpszData; 
}

inline CMover* CProject::GetUserByID( u_long idPlayer )
{
	map<u_long, CMover*>::iterator i = m_idPlayerToUserPtr.find( idPlayer );
	if( i != m_idPlayerToUserPtr.end() )
		return i->second;
	return NULL;
}

inline LPCHARACTER CProject::GetCharacter( LPCTSTR lpStrKey )
{ 
	if( lpStrKey[0] == 0 )	return NULL;
	LPCHARACTER lpCharacter = NULL; 
	TCHAR szStrKeyLwr[ 64 ];
	_tcscpy( szStrKeyLwr, lpStrKey );
	_tcslwr( szStrKeyLwr );
	m_mapCharacter.Lookup( szStrKeyLwr, (void*&)lpCharacter ); 
	return lpCharacter; 
}

extern CProject prj;

#define GETTEXTCOLOR( x )	prj.GetTextColor( x )
#define GETTEXT( x )		prj.GetText( x )
#define GETANIM( x )		prj.GetAnim( x )
#define GETITEM( x )		prj.GetItem( x )
#define GETMOVER( x )		prj.GetMover( x )

#ifdef __PERF_0226
class CPartsItem
{
private:
	vector<ItemProp*>		m_items[3][MAX_HUMAN_PARTS];
	CProject*	m_pProject;
private:
	CPartsItem();
public:
	~CPartsItem();
	static	CPartsItem*	GetInstance( void );
	void	Init( CProject* pProject );
	ItemProp*	GetItemProp( int nSex, int nParts );
};
#endif	// __PERF_0226

#endif // !defined(AFX_PROJECT_H__3C837668_F3CC_430F_87E6_792AF43D7626__INCLUDED_)
