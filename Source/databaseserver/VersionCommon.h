#ifndef __VERSION_COMMON__
#define __VERSION_COMMON__
#define		__VER	15	// 15��
#define __MAINSERVER
#if !defined( __TESTSERVER ) && !defined( __MAINSERVER )
	#define __INTERNALSERVER
#endif

#define		__SERVER					// Ŭ���̾�Ʈ �����ڵ带 �������� �ʱ� ���� define
#define		__ITEM_REMOVE_LIST			// ������ ���� 
#define		__S1108_BACK_END_SYSTEM		// ��ص�ý��� Trans, World, Neuz

#define		__EVENT_1101				// ��ý �̺�Ʈ

// �ؿ� 6�� ����
//	#define		__VERIFY_0201

#define		__LANG_1013
//	mulcom	BEGIN100415	DB_OVERLAPPED_PLUS �޸� Ǯ ���.
//#define		__TRANS_0413				// Ʈ���� ���� �޷θ� ����ȭ
//	mulcom	END100415	DB_OVERLAPPED_PLUS �޸� Ǯ ���.

#define		__SKILL_0205				// ��ų ���� �κи� ����
#define		__STL_0402					// stl

#define		__ITEMDROPRATE				// ������ ��ӷ� ����(�̺�Ʈ)
#define		__EVENT_1101_2				// ��ý �̺�Ʈ 64��Ʈ ����
#define		__GETMAILREALTIME			// �ǽð� ���� ������ ���� Trans
#define		__S_RECOMMEND_EVE			// ��õ �̺�Ʈ Neuz, Trans, World
#define		__EXP_ANGELEXP_LOG			// ����ġ, ���� ����ġ �α� ���� CHARACTER_TBL �÷� �߰�

#define		__RULE_0615					// ��� ��Ģ ����
#define		__S_BUG_GC					// ������ ��û�� ���� �ݾ��� ��û�ϸ� �����̵�� ��Ʈ �Ǽ� ������ �ٲ�� ���� ����( map -> vector ) 

#define		__PROP_0827					// ������ ������Ƽ ���� �Ķ���� Ȯ��

//11th
//#define		__MA_VER11_04				// ��� â�� �α� ��� world,database
//#define		__MA_VER11_05				// �ɸ��� ���� �ŷ� ��� world,database,neuz
//#define		__CSC_VER11_5				// �½�ũ�� Ȯ��
//#define		__GUILD_COMBAT_1TO1			// �ϴ��� ������

#define		__EVENTLUA_COUPON			// ���� �̺�Ʈ
#define		__LOG_PLAYERCOUNT_CHANNEL	// ä�κ� ���� �α�

//	#define		__SYS_POCKET				// �ָӴ�
//	#define		__SYS_COLLECTING			//
//	#define		__SYS_IDENTIFY				// ����, �ູ
//	#define		__SYS_PLAYER_DATA			// ĳ���� ���� ����
#define		__RT_1025					// �޽���
#define		__CONV_0906
#define		__INVALID_LOGIN_0320		// ������ �Ϸ� ���� ���� ������� ���� ����(���� ����)
#define		__INVALID_LOGIN_0612		// ����
#define		__VENDOR_1106				// ���λ��� ���� ��� ����

#define		__JEFF_9_20		// ���� �ð��� ħ�� �ð����� ����
#define		__JEFF_FIX_0

#define	__REMOVE_PLAYER_0221		// ĳ���� ���� ��Ŷ ����

#define		__SEND_ITEM_ULTIMATE	// ���͸� ������ ITEM_SEND_TBL���� ���ް����ϰ�...

#if (_MSC_VER > 1200)
#define		__VS2003		// �����Ϸ�����.net
#endif

#define		__AUTO_NOTICE		// �ڵ� ����

// 12��
//	#define		__SECRET_ROOM
//	#define		__TAX
//	#define		__LORD
//	#define		__EXT_PIERCING				// ���� �Ǿ��
//	#define		__MOD_TUTORIAL	// Ʃ�丮�� ����

// 13��
//	#define		__RAINBOW_RACE		// ���κ��� ���̽�
//	#define		__HOUSING			// �Ͽ�¡ �ý���

#define		__PET_1024
#define		__BUFF_1107	
//	#define		__HONORABLE_TITLE			// ����
//	#define		__COUPLE_1117
//	#define		__COUPLE_1202		// Ŀ�� ����
//	#define		__POST_1204		// ���� ���� ����ȭ

#define		__OCCUPATION_SHOPITEM	// ���ɱ�� ���� ���� ���� ������

#define		__POST_DUP_1209		// ���� �ε� �� ���Ἲ �˻�

#define		__LAYER_1015	// ���̾� ����	

#define		__FUNNY_COIN			// �۴� ����

#define		__REMOVEITEM_POCKET		// ���� ���� ���̺� �޴밡�� ����..

#define		__PERIN_BUY_BUG				// ��� �ݺ����� ���� Ȯ�ο� �ڵ�

// 14��
//	#define		__INSTANCE_DUNGEON			// 14�� �ν��Ͻ� ���� ���
//	#define		__PCBANG				// PC�� ����

#define		__QUIZ					// ���� �̺�Ʈ �ý���

#define		__ERROR_LOG_TO_DB		// ���� �α� �ý���


// 15��
//	#define		__USING_CONTINENT_DATA		// ��� ��赥���� �ܺο��� �ܾ��! 

//	#define		__PETVIS					// 15�� ����
//	#define		__GUILD_HOUSE				// 15�� ����Ͽ콺

//	#define		__HERO129_VER15				// 15�� ����� ����Ȯ��

//	#define		__IMPROVE_QUEST_INTERFACE	// 15�� ����Ʈ �ý���
//	#define		__CAMPUS					// 15�� ���� �ý���
//	#define		__2ND_PASSWORD_SYSTEM		// �α��� �� 2�� ��й�ȣ �Է�

#if	  defined(__INTERNALSERVER)	// ���� �繫�� �׽�Ʈ���� 
//	#define	__LANG_1013
//	#define	__RULE_0615

	#define		__SKILL_0205
	#define		__GUILDVOTE				// ��� ��ǥ 
	#define		__VERIFY_MEMPOOL

//	#define		__S_NEW_SKILL_2			// ��ų ���� ��Ŷ ���� Neuz, World, Trans
	#define		__Y_MAX_LEVEL_8			// ���� 120���� ����... Neuz, World, Trans
//	#define		__INVALID_LOGIN_0320	// ������ �Ϸ� ���� ���� ������� ���� ����(���� ����)
//	#define		__INVALID_LOGIN_0612	// ����

//	#define		__LEGEND				//	9�� ���½ý���	Neuz, World, Trans
//	#define		__ULTIMATE				// ���͸� ����

//	#define		__PET_0410				// 9, 10�� ��
//	#define		__AI_0509				// ���� �ΰ�����

//	#define		__LUASCRIPT				// ��� ��ũ��Ʈ ��� (World, Trans, Neuz)
//	#define		__EVENTLUA				// �̺�Ʈ (��� ��ũ��Ʈ ����) - World, Trans, Neuz
//	#define		__SKILL_0706			// ���ο� AddSkillProp �÷� �߰� �� ����

//	#define		__CONV_0906

//	#define		__SYS_POCKET			// �ָӴ�
//	#define		__SYS_COLLECTING	//

//	#define		__SYS_IDENTIFY			// ����, �ູ
//	#define		__SYS_PLAYER_DATA		// ĳ���� ���� ����
//	#define		__RT_1025				// �޽���

//	#define		__VENDOR_1106			// ���λ��� ��� ����

	#define		__JEFF_11_2		// ������ ���� ���� �ε� �� �ݼ�

	#define		__VTN_TIMELIMIT

	#define		 __GUILD_HOUSE_MIDDLE	// ����Ͽ콺 ����



#elif defined(__TESTSERVER)		// �ܺ� ���� �׽�Ʈ���� 

//	#define		__NOTIFYSTART0706		// ���Ŀ��� ���� �˸� ����� �̺�Ʈ��  
	#define		__GUILDVOTE				// ��� ��ǥ 
	#define		__Y_MAX_LEVEL_8			// ���� 120���� ����... Neuz, World, Trans
//	#define		__PET_0410				// 9, 10�� ��
//	#define		__AI_0509				// ���� �ΰ�����
//	#define		__LEGEND				//	9�� ���½ý���	Neuz, World, Trans
//	#define		__ULTIMATE				// ���͸� ����
	
//	#define		__LUASCRIPT				// ��� ��ũ��Ʈ ��� (World, Trans, Neuz)
//	#undef	__VER
//	#define	__VER	11					// 11��

//	#define		__SYS_POCKET			// �ָӴ�
//	#define		__SYS_COLLECTING	//
//	#define		__SYS_IDENTIFY			// ����, �ູ
//	#define		__SYS_PLAYER_DATA		// ĳ���� ���� ����
//	#define		__RT_1025				// �޽���
//	#define		__CONV_0906
//	#define		__INVALID_LOGIN_0320	// ������ �Ϸ� ���� ���� ������� ���� ����(���� ����)
//	#define		__INVALID_LOGIN_0612	// ����
//	#define		__VENDOR_1106			// ���λ��� ��� ����

	#define	__JEFF_11_2		// ������ ���� ���� �ε� �� �ݼ�

	#define __GUILD_HOUSE_MIDDLE	// ����Ͽ콺 ����


#elif defined(__MAINSERVER)		// �ܺ� ���� 

	#define		__IDC
//	#define		__TOOMANY_PENDINGLOG1115

#endif	// end - ���������� define 

#endif