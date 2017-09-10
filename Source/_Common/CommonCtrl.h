#ifndef __COMMONCTRL_H__
#define __COMMONCTRL_H__

#include "Ctrl.h"
//#include "Respawn.h"
#include "ProjectCmn.h"


#define			MAX_CTRLDROPITEM		4
#define			MAX_CTRLDROPMOB			3
#define			MAX_TRAP                3

#define			MAX_KEY					64

#define         OPEN_COOLTIME			SEC(10)

typedef struct CCtrlElem
{
	DWORD		m_dwSet;				    //-�������� ���� UA_ITEM, UA_LEVEL...
	DWORD 		m_dwSetItem;				//-�ڽ� ���� ����(������)								- UA_ITEM
	DWORD		m_dwSetLevel;				//-�ڽ� ���� ����(����)									- UA_LEVEL
	DWORD		m_dwSetQuestNum;			//-�ڽ� ���� ����(����Ʈ �ѹ�,����Ʈ �÷��� �ѹ�)       - UA_QUEST
	DWORD		m_dwSetFlagNum;
	DWORD		m_dwSetGender;				//-�ڽ� ���� ����(����)									- UA_GENDER
	BOOL		m_bSetJob[MAX_JOB];			//-�ڽ� ���� ����(Ŭ����)								- UA_CLASS
	
	DWORD		m_dwSetEndu;				//-�ڽ� ������ ? hp�� ���� ����	
	 
	DWORD		m_dwMinItemNum;				//-�ּ� �߻� ������ �� 
	DWORD		m_dwMaxiItemNum;			//-�ִ� �߻� ������ ��
	DWORD		m_dwInsideItemKind[MAX_CTRLDROPITEM];		//-���� ������ ���� �� !!!����!!! ������ MAX_CTRLDROPITEM-1 �� ���� ���۰���
	DWORD		m_dwInsideItemPer [MAX_CTRLDROPITEM];		//-���� ������ �� �߻� Ȯ��( 3000000000 )
	
	DWORD       m_dwMonResKind  [MAX_CTRLDROPMOB];          //-����1 �߻� ����(���� ���̵� �Է�)
	DWORD		m_dwMonResNum   [MAX_CTRLDROPMOB];			//-����1 �߻� ����
	DWORD		m_dwMonActAttack[MAX_CTRLDROPMOB];			//-���� Ÿ��1���� ���� ���� �߻� ����(���� �ʵ� ���� �ʰ��ؼ��� �ʵ�)

	// Ʈ������
	DWORD		m_dwTrapOperType;              // Ʈ�� �ߵ� Ÿ�� TOT_RANDOM, TOT_NOENDU
	DWORD		m_dwTrapRandomPer;             // Ʈ�� �ߵ� Ȯ��(TOT_RANDOM�ϰ��)
	DWORD		m_dwTrapDelay;				   // Ʈ�� �ߵ� ������ 0�̸� ��� �߻�
	
	DWORD		m_dwTrapKind	[MAX_TRAP];    // Ʈ�� ���� - ���� ��ų�� ����� ��� Ʈ���� ��ų�� (��ų������Ƽ�� ID��)
 	DWORD		m_dwTrapLevel	[MAX_TRAP];
	TCHAR		m_strLinkCtrlKey[MAX_KEY];		// ���� ��Ʈ���� Ű �̸�
	TCHAR		m_strCtrlKey[MAX_KEY];			// ��Ʈ���� Ű �̸�
	DWORD		m_dwSetQuestNum1;			//-�ڽ� ������ ����(����Ʈ �ѹ�,����Ʈ �÷��� �ѹ�)       - UA_QUEST
	DWORD		m_dwSetFlagNum1;
	DWORD		m_dwSetQuestNum2;			//-�ڽ� ������ ����(����Ʈ �ѹ�,����Ʈ �÷��� �ѹ�)       - UA_QUEST
	DWORD		m_dwSetFlagNum2;
	DWORD 		m_dwSetItemCount;
	DWORD		m_dwTeleWorldId;
	DWORD		m_dwTeleX;
	DWORD		m_dwTeleY;
	DWORD		m_dwTeleZ;
	
} CCtrlElem, *LPCtrlElem;

/// ���� ���� ����, Ʈ��  
class CCommonCtrl : public CCtrl
{
public:
	CCommonCtrl();
	virtual ~CCommonCtrl();

public:
	OBJID		m_nMoverID;
	CCtrlElem	m_CtrlElem;

	BOOL		 m_bTrap;
	BOOL		 m_bAniPlay;
	BOOL		 m_bAlpha;
	int			 m_nBlendFactorLocal;
	DWORD		 m_dwTrapProcessTime;
	
	float		 m_fFrameCount;
	BOOL		 m_bAniDelay;
	DWORD		 m_dwAniDelay;
	
public:
	void Init();
	ItemProp *m_pSkillProp;
	AddSkillProp *m_pAddSkillProp;
#ifdef __CLIENT
	CSfxModel *m_pSfxModel;
	CSfxModel *m_pSfxModel2;
#endif // __CLIENT
	OBJID	m_idAttacker;			// ����ű �� ������.
	BOOL	m_bControl;
	int		m_nCount;
	int		m_nEvent;
	DWORD	m_tmStart;
	int		m_nLife;				// ��� �ε����� ��������.

	BOOL	m_bAction;
	DWORD   m_dwCtrlReadyTime;

	DWORD		m_dwDelete;
	EXPINTEGER	m_nExpBox;
	u_long      m_idExpPlayer;		
	
	void DestroyWall( void );
	void _ProcessWall( void );
	void _CreateTrapSkill();
	void _ProcessTrap();
	void _ProcessAction();
	void DropNPC();
	
	void			DropItem();
	void			SetActionPlay();
//#ifdef __WORLDSERVER
//	void			SendActionError();
//#endif //__WORLDSERVER
	CtrlProp*		GetProp() { return prj.GetCtrlProp( GetIndex() ); }

	virtual void Process();
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
	virtual void RenderName( LPDIRECT3DDEVICE9 pd3dDevice, CD3DFont* pFont, DWORD dwColor = 0xffffffff );
	virtual BOOL Read( CFileIO* pFile );
};

#endif //__COMMONCTRL_H__
