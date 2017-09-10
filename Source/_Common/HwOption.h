#ifndef __HWOPTION_H__
#define	__HWOPTION_H__

class COption
{
public:
	TCHAR	m_szFileName[MAX_PATH];
//	int		m_nSoundEffect;
	int		m_nResWidth;
	int		m_nResHeight;
	BOOL	m_bStartFullScreen;
//	float	m_fMusicVolume;
	int		m_nTextureQuality;		// �ؽ��� ǰ�� ����0,�߰�1,����2
	int		m_nViewArea;			// �þ߹��� �ָ�0,�߰�1,������2
	int		m_nObjectDetail;		// ������Ʈ ǥ������ ����0,�߰�1,����2
	int     m_nObjectDistant;       // ������Ʈ �Ÿ�  ����0,�߰�1,����2
	int     m_nShadow; // �׸��� 
	int		m_nToggleScreen;		// ��Ʈ���� ����/������.  �̰� �������� �𸥴�.
	BOOL    m_bOperator;
	DWORD   m_dwChatFilter[5];
	DWORD   m_dwChatFilterEtc;
	int		m_nBloom;				// �ǻ��� ȿ��.	
	int		m_nMonName;		// ���� �̸� On / Off
	int		m_nPlayerName;		    // �ڽ� �̸� On / Off
	int		m_nOtherPlayerName;		// �ٸ� �÷��̾� �̸� On / Off
	int		m_nToolTipTexture;		// ���� �������̽� �ٲٱ�
	int		m_nToolTipText;
#ifdef __SFX_OPT
	int		m_nSfxLevel;	
#endif
	BOOL	m_nWeatherEffect;
//#ifdef __YADDSOUND
	BOOL    m_bVoice;
	float	m_fEffectVolume;
	float	m_fBGMVolume;
//#endif //__YADDSOUND

	// ���� �ɼ�
	int     m_nWindowAlpha; // 0 ~ 255
	int     m_nWindowEffect; // 0(�Ϲ�), 1(������), 2(�簢)

	// �Ӿ� �ܾ� ����
	int     m_nSlangWord; // 0(��ü), 1(����)

	// ä�� ��ɾ�
	int     m_nChatCommand; // 0(�ѱ� Ǯ����),1(�ѱ�����),2(����Ǯ����),3(��������)
	
	// �ʺ��� ����(ä��â�� ������ ��)
	int     m_nInstantHelp; // 
	
	// �������̽� ���
	int		m_nInterface;	// 0 : ������ 1 : �Ź���(��Ŭ������ ī�޶� ȸ���ϴ�..)
	BOOL	m_bRollEffect;	// ����� �Ѹ� ȿ��.
	
	BOOL	m_bSay;			// �ͼӸ� ���� / �ź�
	BOOL	m_bTrade;		// �ŷ� ���� / �ź�
	BOOL	m_bParty;		// ��Ƽ ���� / �ź�
	BOOL	m_bShout;		// ��ġ�� ǥ�� ON / OFF
	BOOL	m_bMessengerJoin; // Messenger �˸� ǥ�� On / Off
	BOOL	m_bMessenger;	// Messenger ���� / �ź�
#if __VER >= 15 // __IMPROVE_SYSTEM_VER15
	BOOL	m_bBattleBGM;	// ���� ���� ON / OFF
#endif // __IMPROVE_SYSTEM_VER15
#ifdef __GAME_GRADE_SYSTEM
	BOOL	m_bGameGradeRendering;
#endif // __GAME_GRADE_SYSTEM

	int		m_nSer;		// ������ �����ߴ� ����
	int		m_nMSer;	// ������ �����ߴ� ��Ƽ ����
	
	BOOL    m_bSaveAccount;
	TCHAR   m_szAccount[ 64 ];
	BOOL    m_bNotice;
	BOOL    m_bTip;
	time_t  m_tNoticeTime;
	
	int		m_nGuide1;	
	int		m_nGuide2;	
	int		m_nGuide3;	
#if __VER >= 12 // __MOD_TUTORIAL
	int		m_nTutorialLv;
#endif
#if __VER >= 12 // __UPDATE_OPT
	BOOL	m_bViewMask;
#endif
	BOOL    m_bIsShowReSkillMessage;

#ifdef __YENV
	BOOL	m_bExitChange;
	BOOL	m_bSpecBump;
#endif //__YENV

	BOOL	m_bDamageRender;

	BOOL	m_bVisibleBuffTimeRender;
	
	int		m_MouseSpeed;
	BOOL	m_bSFXRenderOff;	
	BOOL	m_bCameraLock;
	BOOL	m_bAutoAttack;
	int*	m_pGuide;
	
#if __VER >= 8 //__Y_GAMMA_CONTROL_8
	int     m_nOverBright;
	float	m_fGamma;
	float   m_fContrast;
#endif //__Y_GAMMA_CONTROL_8

#if __VER >= 8 //__CSC_VER8_3
	int m_BuffStatusMode;
#endif //__CSC_VER8_3

#if __VER >= 11 // __ADD_ZOOMOPT
	BOOL	m_bZoomLimit;		// �� ���� ����
#endif
	COption();
	~COption();
	
	void Init();
	int Load( LPCTSTR szFileName );
	int Save( LPCTSTR szFileName );
#ifdef _DEBUG
	int LoadToolTip( LPCTSTR szFileName );
#endif // _DEBUG
	
#if __VER >= 9 // __CSC_VER9_RESOLUTION
	void CheckResolution();
#endif //__CSC_VER9_RESOLUTION
};

extern COption		g_Option;

#endif
