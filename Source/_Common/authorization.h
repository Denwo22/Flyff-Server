#ifndef __AUTHORIZATION_H
#define __AUTHORIZATION_H

// AUTHORAZATION ���ǵ�. ADMINISTRATOR�� ����� ���� ���� �����̴�.

#define AUTH_OBSERVER      'D' // ������ ���� ; ��ۿ�, ����͸�, ���ÿ�
#define AUTH_GENERAL       'F' // �Ϲ� ����� ����
#define AUTH_LOGCHATTING   'G' // ä�÷α׸� ���� �Ϲ����� 
#define AUTH_JOURNALIST    'H' // ������ �����ϱ� ���� ���� ����
#define AUTH_HELPER        'J' // �� ��� ���� ; ����ڸ� ���� ���� ���� ������
#define AUTH_GAMEMASTER    'L' // ���� ������ ; ���� ��ü�� ������ ������ ���� GM_LEVEL_1
#define AUTH_GAMEMASTER2   'M' // ���� ������ ; ���� ��ü�� ������ ������ ���� GM_LEVEL_2
#define AUTH_GAMEMASTER3   'N' // ���� ������ ; ���� ��ü�� ������ ������ ���� GM_LEVEL_3
#define AUTH_OPERATOR      'O' // ���� ���� ; ����� AUTH_ADMINISTRATOR�� ���� ������� ���� GM_LEVEL_4
#define AUTH_ADMINISTRATOR 'P' // ������ ���� ; ��� ����� ����� �� ����. GM_LEVEL_4


// ���(����, ����ȭ ���)CMover::m_dwMode�� �����. �����ο� ���Ӹ��� ������ ���ڳ�. -xuzhu-
// 1, 2, 4, 8, 10, 20, 40...
#define MATCHLESS_MODE			(DWORD)0x00000001	// ���� ����
#define TRANSPARENT_MODE		(DWORD)0x00000002	// ���� ����
#define ONEKILL_MODE			(DWORD)0x00000004	// ���� ����
#define DONMOVE_MODE			(DWORD)0x00000008	// �������� ���ϴ� ����
#define SAYTALK_MODE			(DWORD)0x00000010	// �ӼӸ� ���ϴ� ����
#define MATCHLESS2_MODE			(DWORD)0x00000020	// ���� ����2 (�������� ������ ������ �ʴ´�.)
#define NO_ATTACK_MODE			(DWORD)0x00000040	// ���� ���ϴ� ����
#define ITEM_MODE				(DWORD)0x00000080	// ������ �������� ������ ���
#define COMMUNITY_MODE			(DWORD)0x00000100	// ���, ��Ƽ, ģ��, ���ΰ� �ŷ�, ���λ��� �Ҽ� ����
#define TALK_MODE				(DWORD)0x00000200	// �Ϲ� �� ���ϴ� ����
#define SHOUTTALK_MODE			(DWORD)0x00000400	// ��ġ�� ���ϴ� ����
#define RECOVERCHAO_MODE		(DWORD)0x00000800	// ī�� �غ� ���
#define	FREEPK_MODE				(DWORD)0x00001000	// CTRLŰ ���� ���� pk����
#define	PVPCONFIRM_MODE			(DWORD)0x00002000	// PVP���� ����
#define	QUERYSETPLAYERNAME_MODE	(DWORD)0x00004000	// ĳ���� �� ���� ���� ���
#define	MODE_MAILBOX			(DWORD)0x00008000	// ���� ���� ����

#define	EVENT_OLDBOY_MODE		(DWORD)0x00010000
#define	EQUIP_DENIAL_MODE		(DWORD)0x00020000

#define MODE_EXPUP_STOP			(DWORD)0x00040000		// ����ġ ��� �������.

#define GCWAR_NOT_CLICK_MODE	(DWORD)0x00080000		// Ÿ���� �ڽ��� Ŭ�� ���Ѵ�...(��ŷ �н�)
#define GCWAR_RENDER_SKIP_MODE	(DWORD)0x00100000		// ������ ��ŵ

#define	MODE_OPTION_DONT_RENDER_MASK	(DWORD)0x01000000	// ����ũ ���� ����


#define FRESH_MODE			(DWORD)0x10000000	// ó�� �����ϴ� �ɸ��ͳ�
#define NOTFRESH_MODE		(DWORD)0x20000000	// �� �ٲٱ⿡ �ʿ�
#define NOTFRESH_MODE2		(DWORD)0x40000000	// �� �ٲٱ⿡ �ʿ�2

#define	MODE_OUTOF_PARTYQUESTRGN	(DWORD)0x80000000	// ��Ƽ ����Ʈ ������ 

#define DONTALK_MODE		(DWORD)0x00000610	// �� ���ϴ� ���� TALK_MODE, SAYTALK_MODE, SHOUTTALK_MODE ���Ѱ�
#define OBSERVE_MODE		(DWORD)0x000000c0	// COMMUNITY_MODE, ITEM_MODE, ATACK_MODE, SHOUTTALK_MODE, SAYTALK_MODE ���Ѱ�
#define	ALL_MODE			(DWORD)0x000005d0

#define	QUIZ_RENDER_SKIP_MODE		(DWORD)0x00200000	// �����̺�Ʈ ���������� ������ ��ŵ


// PK, PVP, ����CMover::m_dwStateMode���.
#define STATE_PK_MODE			(DWORD)0x00000001	// PK ����
#define STATE_PVP_MODE			(DWORD)0x00000002	// PVP ����
#define STATE_BASEMOTION_MODE	(DWORD)0x00000004	// �������� ����

#define STATE_BASEMOTION		(DWORD)0x0000000c	// STATE_BASEMOTION_MODE + STATE_BASEMOTION_END_MODE

#define STATEMODE_BASEMOTION_ON			(BYTE)0x00
#define STATEMODE_BASEMOTION_OFF		(BYTE)0x01
#define STATEMODE_BASEMOTION_CANCEL		(BYTE)0x02


#endif