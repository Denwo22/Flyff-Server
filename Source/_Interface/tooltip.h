#ifndef _TOOLTIP_H_
#define _TOOLTIP_H_

enum 
{
	FIRST_TT,
//	SECOND_TT,
//	THIRD_TT,
	MAX_TT
};

class CToolTip
{
	BOOL        m_bEnable       ;
	BOOL        m_bPutToolTip   ;
	BOOL        m_bReadyToopTip ;
	DWORD       m_dwToolTipId   ;
	CEditString m_strToolTip    ;
	CTimer      m_timerToopTip  ;
	CPoint      m_ptToolTip     ;
	CPoint      m_ptOldToolTip  ;
	CRect       m_rect          ;
	CRect       m_rectRender    ;
	int         m_nPosition     ;
	BOOL        m_bToolTip      ;
	int         m_nAlpha        ;
	CTexture	m_apTextureToolTip[ MAX_TT * 9 ];
#if __VER >= 9 // __CSC_VER9_1
	int			m_nAdded;
	int			m_nAddedJewel[5];
	int			m_nSlot;
	CItemBase*	m_pUltimateItemBase;
#if __VER >= 12 // __EXT_PIERCING
	CTexture*	m_pUltimateTexture;
	CTexture*	m_pJewelBgTexture;
#endif //__EXT_PIERCING
#endif //__CSC_VER9_1

#ifndef __IMPROVE_MAP_SYSTEM
#if __VER >= 13 // __CSC_VER13_1
	int			m_nMonInfoCnt;
	DWORD		m_pDwMonId[5];
#endif //__CSC_VER13_1
#endif // __IMPROVE_MAP_SYSTEM
#if __VER >= 15 // __IMPROVE_SYSTEM_VER15
	int			m_nSubToolTipFlag;
	CRect		m_nRevisedRect;
	int			m_nSubToolTipNumber;
#endif // __IMPROVE_SYSTEM_VER15
#ifdef __IMPROVE_MAP_SYSTEM
	vector< DWORD > m_vecMapMonsterID;
#endif // __IMPROVE_MAP_SYSTEM
public:
	CToolTip();
	~CToolTip();
	void SetPosition( int nPos ) { m_nPosition = nPos; }
	void SetEnable( BOOL bEnable ) { m_bEnable = bEnable; }
	BOOL GetEnable() { return m_bEnable; }
	void CancelToolTip();

	void PutToolTip( DWORD dwToolTipId, CEditString& string, CRect rect, CPoint pt, int nToolTipPos = 0 );
	void PutToolTip( DWORD dwToolTipId, CString& string, CRect rect, CPoint pt, int nToolTipPos = 0 );
	void PutToolTip( DWORD dwToolTipId, LPCTSTR lpszString, CRect rect, CPoint pt, int nToolTipPos = 0 );	
#if __VER >= 15 // __IMPROVE_SYSTEM_VER15
	void PutToolTipEx( DWORD dwToolTipId, CEditString& string, CRect rect, CPoint pt, int nToolTipPos = 0, int nSubToolTipFlag = 0 );
#endif // __IMPROVE_SYSTEM_VER15
	void Process( CPoint pt, C2DRender* p2DRender );
	void Paint( C2DRender* p2DRender );
	void Delete();
	void InitTexture();
#if __VER >= 9 // __CSC_VER9_1
	void SetUltimateToolTip(CItemBase* pItemBase);
#endif //__CSC_VER9_1
#ifndef __IMPROVE_MAP_SYSTEM
#if __VER >= 13 // __CSC_VER13_1
	void SetWorldMapMonsterInfo(int nMonCnt, DWORD* pDwMonId);
#endif //__CSC_VER13_1
#endif // __IMPROVE_MAP_SYSTEM
#ifdef __IMPROVE_MAP_SYSTEM
	void ResizeMapMonsterToolTip( void );
	void InsertMonsterID( DWORD dwMonsterID );
#endif // __IMPROVE_MAP_SYSTEM
#if __VER >= 15 // __IMPROVE_SYSTEM_VER15
	const CPoint& GetPointToolTip( void ) const;
	const CRect& GetRect( void ) const;
	void SetRenderRect( const CRect& rectRender );
	const CRect& GetRenderRect( void ) const;
	const CRect& GetRevisedRect( void ) const;
	void SetSubToolTipNumber( int nSubToolTipNumber );
	int GetSubToolTipNumber( void ) const;
	BOOL GetReadyToolTipSwitch( void ) const;
#endif // __IMPROVE_SYSTEM_VER15
};
/*
#define TOOLTIP_COMBATMODE   1  // "����/��ȭ\n����Ű[C]"
#define TOOLTIP_INVENTORY    2  // "����ǰ\n����Ű[I]"
#define TOOLTIP_OPTION       3  // "����\n����Ű[ALT-X]"
#define TOOLTIP_STATUS       4  // "�ɷ�ġ����\n����Ű[S]"

#define TOOLTIP_EQUIPMENT    5  // "�������\n����Ű[E]"
#define TOOLTIP_KNOWLEDGE    6  // "���Ļ��\n����Ű[K]"
#define TOOLTIP_LOOK         7  // "�ڼ�������\n����Ű[L]"

#define TOOLTIP_SLOT         8  // "������ ����"  
#define TOOLTIP_SLOT_F1      9  // "1�� ���� ����\n����Ű[F1]"
#define TOOLTIP_SLOT_F2      10 // "2�� ���� ����\n����Ű[F2]"
#define TOOLTIP_SLOT_F3      11 // "3�� ���� ����\n����Ű[F3]"

#define TOOLTIP_CHAT         12 // "ä�� �Է�â\n����Ű[ENTER]"
#define TOOLTIP_CHAT1        14 // "�Ϲ� ä��"
#define TOOLTIP_CHAT2        15 // "�ӼӸ�"
#define TOOLTIP_CHAT3        16 // 

#define TOOLTIP_MESSAGE      17 // "�޽��� ���â"
*/
#endif