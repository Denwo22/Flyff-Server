#ifndef __VERSION_COMMON_H__
#define __VERSION_COMMON_H__
#define __MAINSERVER
#define		__VER	15		// 15��

#if !defined( __TESTSERVER ) && !defined( __MAINSERVER )
	#define __INTERNALSERVER
#endif

#if (_MSC_VER > 1200)
#define		__VS2003		// �����Ϸ�����.net
#endif

#define		__TRANSFER_ERROR_TEXT
#define		__CRC
#define		__PROTOCOL0910
#define		__PROTOCOL1021
#define		__INFO_SOCKLIB0516			// ��Ʈ��ũ ������� 
#define		__S1108_BACK_END_SYSTEM		// Trans, World, Neuz : ��ص�ý��� 
#define		__THROUGHPORTAL0810			// ���� ���� ����
#define		__TWN_LOGIN0816				// �븸 �α��� �������� ����	// Ŭ��/����
#define		__FIX_WND_1109				// â ���� �� �θ�â ���� ���� ����
#define		__EVENT_1101				// ��ý �̺�Ʈ
#define		__EVE_NEWYEAR				// �ų� ȿ��

#define		__S_SERVER_UNIFY			// ���� ���� Neuz, World
#define		__LANG_IME_0327				// ������ imeǥ�� â ��� ����
#define		__STL_0402					// stl

#define		__NEWYEARDAY_EVENT_COUPON	// �� �̺�Ʈ - ����, ���� �����̿�� ����.
// �ؿ� 6�� ����
#define		__LANG_1013					// �ؿ� ���� �ű� �߰� ��ũ��Ʈ

#define		__ITEMDROPRATE				// �̺�Ʈ ������ ��ӷ� ����


// 10
#define		__TRADESYS					// ��ȯ ������ ��ũ��Ʈ(�̺�Ʈ, ����Ʈ, ... ) Neuz, World
#define		__EVE_BALLOON				// ǳ�� �̺�Ʈ ����
#define		__CSC_GAME_GRADE			// ���ӹ���ް��� ä��â �˸� �� �̹��� ����.

#define		__PK_PVP_SKILL_REGION		// PK������������ ���� ��ų�� PVP��뿡�Ը� �����ϵ���..
#define		__EVENT_1101_2				// ��ý �̺�Ʈ 64��Ʈ ����
#define		__S_RECOMMEND_EVE			// ��õ �̺�Ʈ Neuz, Trans, World
#define		__EVE_MINIGAME				// Mini Game 4�� �̺�Ʈ �߰� ����.
#define		__S_ADD_EXP					// EXP_S ������ ����

#define		__RULE_0615					// ��� ��Ģ ����
#define		__FILTER_0705				// ���͸� ��Ģ ����
#define		__S_BUG_GC					// ������ ��û�� ���� �ݾ��� ��û�ϸ� �����̵�� ��Ʈ �Ǽ� ������ �ٲ�� ���� ����( map -> vector ) 
#define		__S_ADD_RESTATE				// ���ȭ ������ ����( ������Ʈ ��, ��ø, ü��, ���� )

#define		__PROP_0827					// ������ ������Ƽ ���� �Ķ���� Ȯ��

#define		__PKSERVER_USE_ANGEL		// PK���������� �������� ������,���� ���
#define		__CSC_ENCHANT_EFFECT_2		// ���� ����Ʈ + ���� ����Ʈ

//11��
//	#define		__MA_VER11_02				// ��ǥ ��� �丰 ����
//	#define		__MA_VER11_03				// neuz�ش�â(B)���� �شܿ��� �ָ� ������ �־ ������ ǥ�� �ǵ���
//	#define		__MA_VER11_04				// ��� â�� �α� ��� world,database,neuz
//	#define		__MA_VER11_05				// �ɸ��� ���� �ŷ� ��� world,database,neuz
//	#define		__MA_VER11_06				// Ȯ����ų ȿ������ world,neuz
//	#define		__CSC_VER11_1				// <����??> ����, ä��â ����, ��İŷ� ��ҹ�ư ����
//	#define		__CSC_VER11_2				// Ÿ�� ǥ�� ��� ����
//	#define		__CSC_VER11_3				// Ĩ���� ���� �̿� �ϱ� (��� ���� ����)
//	#define		__CSC_VER11_4				// �޽���â ����
//	#define		__CSC_VER11_5				// �½�ũ�� Ȯ��
//	#define		__GUILDCOMBATCHIP			// ������ Ĩ���� �� Ĩ�� ���� ���� �̿�
//	#define		__GUILD_COMBAT_1TO1			// �ϴ��� ��� ����
//	#define		__GUILD_BANK_LOG			// ����ũ �α�â �߰�
//	#define		__EXPITEM_TOOLTIP_CHANGE	// ����ġ ������ ǥ�� ��� ����
//	#define		__REMOVE_ENDURANCE			// ������ ������ ���� ����
//	#define		__PIERCING_REMOVE			// �Ǿ�� �ɼ� ����
//	#define		__CHIPI_071210				// �Ⱓ�� ������ ���ӽð� �ʴ��� ǥ��

#define		__EVENTLUA_COUPON			// ���� �̺�Ʈ
#define		__NOLIMIT_RIDE_ITEM			// ���� ���� ���� ����ü ������
#define		__NPC_BUFF					// NPC�� ���� �����ޱ�

//	#define		__SYS_POCKET				// �ָӴ�
//	#define		__SYS_COLLECTING			// ä�� �ý���
//	#define		__SYS_IDENTIFY				// ����, �ູ
//	#define		__FIX_ROTATE				// ��Ʈ���� �߰ߵ� ���� ����
//	#define		__ADD_ZOOMOPT				// �ܿɼ� �߰� 
//	#define		__MOD_VENDOR				// ���λ��� ����
//	#define		__FIX_COLLISION				// �浹 ���� ����
//	#define		__FIX_PICKING				// ��ŷ ���� ����
//	#define		__CAPTURE_JPG				// JPG�������� ĸ��

#define		__SYS_TICKET				// �����
//	#define		__SYS_PLAYER_DATA			// ĳ���� ���� ����
#define		__HACK_1023					// ���ҽ� ���� // ���� �ӵ�, ���� ���� �ӵ�
#define		__RT_1025					// �޽���
#define		__VENDOR_1106				// ���λ��� ��� ����
#define		__JEFF_11

#define		__JEFF_9_20					// ���� �ð��� ħ�� �ð����� ����
#define		__JEFF_11_4					// �Ʒ���
#define		__JEFF_11_5					// �޸� ����
#define		__JEFF_11_6					// �ٺ� ����
#define		__DST_GIFTBOX				// ���� �����ۿ� ����Ʈ �ڽ� ��� �߰� 

#define		__SFX_OPT					// ����Ʈ(��ƼŬ) ����ȭ ����
//#define		__PERF_0229

#define		__HELP_BUG_FIX				// ���򸻰��� ���� ���� (���� �Ѱ��� ���� �ν��Ͻ� ����)

#define		__CHIPI_DYO					// NPC�� Ư������������ ����� �� �ְ� character.inc�� ���� 

// 12��

//	#define		__SECRET_ROOM				// ����� ��
//	#define		__TAX						// ����
//	#define		__CSC_VER12_1				// ���λ��� �� �÷��̾� ���캸�� �޴� �����ϵ��� ���� & ���캸�� â �����ϱ�
//	#define		__CSC_VER12_2				// ��� â�� ��� �ܿ� �׸� ������ �����ϵ��� �߰�
//	#define		__CSC_VER12_3				// ����Ʈ�� ���� ��� ����Ʈ �׸��� �߸��� ���� ������ ��ȭâ �ø�
//	#define		__CSC_VER12_4				// �Ǿ�� ���� â ���� �� ���͸� ���� ���� â �߰�
//	#define		__CSC_VER12_5				// �� �� ��ȯ ��� �߰�
//	#define		__HEAVEN_TOWER				// �ɿ��� ž
//	#define		__EXT_PIERCING				// ���� �Ǿ��
//	#define		__NEW_SUMMON_RULE			// ���� ��ȯ ��Ģ ����
//	#define		__MOD_TUTORIAL				// Ʃ�丮�� ����
//	#define		__LORD						// ����
//	#define		__PET_0519	// �� ����
//	#define		__J12_0		// ����, �ູ ���� ���� �߰�
//	#define		__UPDATE_OPT				// �ɼ�â ����
//	#define		__CAM_FAST_RECOVER			// ī�޶� �����ӵ� ����
//	#define		__JHMA_VER12_1	//12�� �ش����������
//	#define		__ITEMCREATEMON_S0602		// ���� ���� Neuz, World
//	#define		__PARSKILL1001	//12�� �Ľ�ų ������ ����  world,core,neuz
//	#define		__LEAK_0827


// 13��
//	#define		__EXT_ENCHANT				// ���� Ȯ��(�Ӽ�, �Ϲ�)
//	#define		__RAINBOW_RACE				// ���κ��� ���̽�
	#define		__HOUSING					// �Ͽ�¡ �ý���
//	#define		__QUEST_HELPER				// ����Ʈ NPC��ġ �˸�.
//	#define		__CHIPI_QUESTITEM_FLAG
//	#define		__RENEW_CHARINFO			// ĳ��������â ������
//	#define		__HONORABLE_TITLE			// ����
	#define		__MAX_BUY_ITEM9999			// ȭ�������ͱ��԰���9999��
//	#define		__REMOVE_JOINPARTYMEMBER_ANI			// ��Ƽ�ɹ����ν� �ִ� ����
//	#define		__CSC_VER13_1				// WorldMap ���� ǥ��
//	#define		__CSC_VER13_2				// Ŀ�� �ý���
//	#define		__COUPLE_1117				// Ŀ�� �ý���
//	#define		__COUPLE_1202				// Ŀ�� ����

#define		__LAYER_1020				// __LAYER_1015 �׽�Ʈ
#define		__AZRIA_1023				// ����� ���� ����
#define		__PET_1024					// �� �۸�
#define		__BUFF_1107
#define		__SYNC_1217					// �񵿱� ����(�ʸ��� ����)
#define		__SYS_ITEMTRANSY			// NPC�޴� ������ Ʈ���� ���

#define		__SPEED_SYNC_0108		// ResetDestParam speed ���� �������� ���̴°� ���� Ÿ������ �ʱ⽺�ǵ�񵿱⵵ ���� ���� 
#define		__PARTY_DEBUG_0129		// �ش��� ƨ��� ���� ����� neuz

#define		__FUNNY_COIN			// �۴� ����

#define		__GPAUTH
#define		__GPAUTH_01
#define		__GPAUTH_02
#define		__EUROPE_0514
#define		__WINDOW_INTERFACE_BUG		// ���� �̵� ��Ȳ���� ������ �ɸ� ������ â ����� ������ Ŭ���̾�Ʈ ũ�����Ǵ� ���� ����

#define		__MAP_SECURITY				// ������ ������ �� �˻�

// 14��
	#define		__NEW_CONTINENT				// 14�� �űԴ�� �ϸ���� �߰�
	#define		__SMELT_SAFETY				// 14�� ��������
	#define		__INSTANCE_DUNGEON			// 14�� �ν��Ͻ� ����
	#define		__EQUIP_BIND				// ���������� ����� �ͼ�
	#define		__EXT_ATTRIBUTE				// �Ӽ����� ���� Ȯ�� �� ����
	#define		__NEW_ITEM_LIMIT_LEVEL		// ������ ���뷹�� ��Ģ ����(MASTER, HERO�� ���� ������ �ް� ���� Ŭ���� ����� ������ ���� ����)
	#define		__WATER_EXT					// �� ǥ�� Ȯ�� (�� �ؽ����� ���� �� �����Ӽӵ� ���� �߰�)
	#define		__WND_EDIT_NUMBER_MODE		// ����Ʈ ��Ʈ�� ���ڸ� �Է¹ް� �ϴ� ���
#define		__SHIFT_KEY_CORRECTION		// ����Ʈ Ű �Է� ���� ����
	#define		__STATIC_ALIGN				// ����ƽ ��Ʈ�ѿ� ������ ���� ��� �߰�
	#define		__ITEM_DROP_SOUND			// ������ ��� �� ȿ���� ��ũ��Ʈ�� ó��
	#define		__JOB_TEXT					// ������, ����� ���� �����ۿ� '�ʿ� ����' �ؽ�Ʈ ��µǵ��� ����
	#define		__RESTATE_CONFIRM			// ������Ʈ ��� ��, Ȯ�� â ���
	#define		__PREVENTION_TOOLTIP_BUG	// �������� ������ �ǹ� ���� �ؽ�Ʈ ��µǴ� ���� ����
	#define		__DROP_CONFIRM_BUG			// ������ ���� Ȯ�� â�� ���� ���¿��� �Ͼ�� ���� ���� ����
	#define		__CLOUD_ANIMATION_BUG		// ���� �ִϸ��̼��� Ư�� ���������� ����Ǵ� ���� ����
	#define		__BUFF_CRASH				// ���� ������ �����Ͽ� Ŭ���̾�Ʈ ũ�����Ǵ� ���� ����
	#define		__BS_FIX_SHADOW_ONOBJECT	// ������Ʈ �׸��� ���ù� ���ǰ˻� ���� ( ���ΰ� ����, �δ� õ�� �׸��� ���� )
	#define		__BS_FIX_HAIR_AMBIENT		// ĳ���� ��� ambient ���� ( �δ����� �Ӹ�ī�� ���� ��İ� ������ ���� ���� )
	#define		__BALLOON_CODE_IMPROVEMENT	// ǳ�� ���� �ڵ� ����
	#define		__WING_ITEM					// ���� ������
	#define		__USE_SOUND_LIB_FMOD		// use FMod sound lib
	#define		__PCBANG					// PC�� ����
//	end 14th

// 15�� ZU TEST ZWECKEN AUSDEKLARIERUNG ENTFERNT!
	#define		__DYNAMIC_MPU				// ���氡���� MPU !!
	#define		__BOUND_BOX_COLLISION		// �ٿ�� �ڽ� �浹 ��ƾ ����
	#define		__BS_CHANGING_ENVIR			// ����� ȯ�� ���� ( light, fog, sky, weather, 24hours light ... ) <<< ONLY CLIENT!!!! >>>
	#define		__USING_CONTINENT_DATA		// ��� ��赥���� �ܺο��� �ܾ��! 
	#define		__IMPROVE_QUEST_INTERFACE	// 15�� ���� ����Ʈ �������̽� �ý���
	#define		__IMPROVE_SYSTEM_VER15		// 15�� ���� ����
	#define		__15TH_INSTANCE_DUNGEON		// 15�� �ν��Ͻ� ����
	#define		__PETVIS					// 15�� ����
	#define		__GUILD_HOUSE				// 15�� ����Ͽ콺
	#define		__TELEPORTER				// 15�� �ڷ�����
	#define		__FIND_OBJ_INSIGHT			// �þ߰Ÿ� ���� ����������Ʈ ���( �̴ϸ� �����ʹ�ư Ŭ�� )
	#define		__HERO129_VER15				// 15�� ����� ����Ȯ��
	#define		__MUSIC2					// ������ 7.2g ver ������Ʈ ( OGG ��� ���� ����  ) ���� Lib������Ʈ�ÿ� ���� Ǯ�������.
#define		__NEW_CONTINENT15			// �߰� ��� ( ���� )
	#define		__BS_BBOX_ABS_EXTENT		// BBOX ���� ���밪���� ����
	#define		__CAMPUS					// 15�� ���� �ý���
	#define		__BS_FIXED_KNOCKBACK		// �˹�� ���� �������� ���� 
	#define		__BS_FIXED_EQUIPMOTION		// ������ ��/Ż ���� ��������� �����Ǵ����� ���� 
	#define		__15_5TH_ELEMENTAL_SMELT_SAFETY	// 15.5�� �Ӽ� ���� ���� �߰�
	#define		__2ND_PASSWORD_SYSTEM		// 2�� ��й�ȣ �ý���
 // end 15th

	#define		__QUIZ						// ���� �̺�Ʈ �ý���

	#define		__EVENTLUA_RAIN				// �帶 �̺�Ʈ -> ��Ʒ� ����

	#define		__EVENTLUA_SNOW				// ���� �̺�Ʈ

	#define		__ADD_RESTATE_LOW			// ���ȭ ������ ����( ������Ʈ �ϱ� ��, ��ø, ü��, ���� )

	#define		__YS_CHATTING_BLOCKING_SYSTEM	// ĳ���� ä�� ���� �ý���
	#define		__BAN_CHATTING_SYSTEM			// ���� ä�� ���� �ý���


	#define __ENCRYPT_PASSWORD	//	mulcom	BEGIN100218	�н����� ��ȣȭ

//	#define		__CERTIFIER_COLLECTING_SYSTEM	// ���� ���� ���� �ý���

	#define		__DELETE_CHAR_CHANGE_KEY_VALUE	// ������� i_PIN �������� ���� ĳ���ͻ��� Ű�� ����(����, �ֹι�ȣ -> 2�����)
	#define		__BS_ADJUST_SYNC			// mover�� ����ȭ ���� 

	#define		__SHOP_COST_RATE			// ���� ���� ����

//	#define		__NEW_WEB_BOX				// ���ο� �� ǥ�� â
	#define		__PROTECT_AWAKE				//������ȣ
	#define		__MAIL_REQUESTING_BOX		// ���� ��û �ڽ�
	#define     __BS_SAFE_WORLD_DELETE		// CWorld::Process���� safe delete obj( sfx ���� Ȯ�δ� )
//	#define		__GAME_GRADE_SYSTEM			// ���ӹ� ��� ǥ�� �ý���
	#define		__BS_FIX_ARRIVEPOS_ALGO		// ���콺 �̵��� ���� �˻��ƾ ���� ( ������ ������ �Ѱ��� ���������� ���ߴ� �������־��� )
	#define		__BS_ITEM_UNLIMITEDTIME		// ������ �����ð� ( 999999999 >= �ϰ�� ������ ǥ�� )
	
	#define		__ENVIRONMENT_EFFECT

// 8.5��
#if	  defined(__INTERNALSERVER)	// ���� �繫�� �׽�Ʈ���� 
//	#define	__RULE_0615

	#define		NO_GAMEGUARD
	#undef		__TRANSFER_ERROR_TEXT

	#define		__CPU_UTILDOWN_060502		// CPU ���� �����۾� 
	#define		__SLIDE_060502				// ������ ������ ���� ���� 
	#define		__GUILDVOTE					// ��� ��ǥ 
	#define		__IAOBJ0622					// ��� ��ǥ ��ǥ ����	// ����, ����
	#define		__YNOTICE_UNI1026			// �������� �����ڵ� ����
//	#define		__YENV
//	#define		__YENV_WITHOUT_BUMP
	#define		__SKILL0517					// ��ų ���� �Ķ����
	#define		__YAIMONSTER_EX				// AI��� �߰� - Neuz, World
//	#define		__Y_INTERFACE_VER3			// �������̽� ���� 3.0 - Neuz
	#define		__VERIFY_MEMPOOL
//	#define		__S_NEW_SKILL_2				// ��ų ���� ��Ŷ ���� Neuz, World, Trans
	#define		__Y_ROTXZ					// ȸ�� �߰� Neuz, World
	#define		__Y_PATROL					// ����...Neuz. World
	#define		__Y_CHARACTER_TEXT			// ĳ���� ����â ���α׷� �ؽ�Ʈ ���..Neuz
	#define		__Y_RENDER_NAME_OPTION		// �̸� ��� ���� UI.Neuz
	#define		__V060721_TEXTDRAG
	#define		__Y_MAPVIEW_EX
	#define		__Y_BEAUTY_SHOP_CHARGE		// ��Ƽ��, ���λ��� ���������ȭ..Neuz, World
//	#define		__EVENT_FALL				// ��ǳ �̺�Ʈ - �𵨱�ü Neuz, World
//	#define		__LANG_1013					// �ؿ� ���� �߰� ��ũ��Ʈ
//	#define		__KWCSC_UPDATE				// ��ų �ߵ� ���� ���� ���� Neuz
//	#define		__CSC_VER8_1				// 8�� 'T'Ű ���, ���� ���� �Ⱥ��̱�, ȭ�� ��� â On/Off Neuz
//	#define		__CSC_VER8_2				// 8�� ��Ƽâ ���� ���� Neuz
//	#define		__JEFF_VER_8				// 8�� �۾�
//	#define		__Y_FLAG_SKILL_BUFF			// ��, ���� ������ ��۹��� ����... Neuz, World
//	#define		__Y_GAMMA_CONTROL_8			// ���, ����, ��� ���� Neuz
//	#define		__Y_CHAT_SYSTEM_8			// �ý��� �޼��� ������ȭ... Neuz
//	#define		__Y_EYE_FLASH_8				// �����ڰŸ���... Neuz
//	#define		__Y_CAMERA_SLOW_8			// 8�� Ű/���콺 X�� ȸ���� �������� ȸ���ϱ�..Neuz
//	#define		__Y_MAX_LEVEL_8				// ���� 120���� ����... Neuz, World, Trans
//	#define		__CSC_VER8_3				// 8�� ����â ���� ����, Key Down�� ��ų�ߵ� 1.5�ʰ� �����̷� ���ӵǰ� ���� Neuz
//	#define		__CSC_VER8_4				// 8�� ������ ���� �� �������� ��� �߰�.
//	#define		__JHMA_VER_8_1				// 8�� ���ӳ�����ӱ���	Neuz, World
//	#define		__JHMA_VER_8_2				// 8�� ���ӳ��������ǸŰ�������Ǯ��	Neuz, World
	#define		__FIX_WND_1109				// �ڽ� â �ʱ�ȭ ���� ����
//	#define		__JHMA_VER_8_3				// 8�� ctrl �ڵ����ݱ�� ����	Neuz
//	#define		__JHMA_VER_8_4				// 8�� Ÿ��Ŭ���ڵ����� �̸�Ƽ��	Neuz
//	#define		__JHMA_VER_8_5				// 8�� ��ų����ġ�ٿ��	Neuz, World
//	#define		__JHMA_VER_8_7				// 8�� ������� ������� PVP�����ϰ���   Neuz, World


//	#define		__CSC_VER8_5				// 8�� ���� ��ȯ Neuz, World, Trans

	#define		__TRAFIC_1218				// �������� ���� ��Ŷ ����
	#define		__Y_HAIR_BUG_FIX
	#define		__FOR_PROLOGUE_UPDATE		// �±�,�븸,�̱�,���� Prologue�߰�
//	#define		__NEWYEARDAY_EVENT_COUPON	// �� �̺�Ʈ - ����, ���� �����̿�� ����.
	#define		__TRAFIC_1215				// ���۷� ����

//	#define     __Y_NEW_ENCHANT				// ���� ������ ����, Neuz, World

//	#define		__ULTIMATE					// 9,10�� ����
//	#define		__LEGEND					// 10�� ���½ý���	Neuz, World, Trans
//	#define		__CSC_VER9_1				// 9�� ���°��� Clienet - Neuz

//	#define		__PET_0410					// 9, 10�� ��
	#define		__HACK_0516					// �̱� ��ŷ 2��
//	#define		__AI_0509					// ���� �ΰ�����

//	#define		__CSC_VER9_RESOLUTION		// Wide�� ���ػ� �߰�
//	#define		__Y_ADV_ENCHANT_EFFECT		// ���� ����Ʈ ��� ����
//	#define		__S_9_ADD					// 9�� �߰���
//	#define		__RECOVERY10				// 9�� �߰� (Ÿ��, �ǰ� 10�� �� �ڵ� ȸ��)

//	#define		__CSC_VER9_2				// 9�� ����â ���� ���� Neuz, World
//	#define		__CSC_VER9_3				// 9�� ä�û�Ȳ���� Function Key ��� ����
//	#define		__CSC_VER9_4				// Ŭ���̾�Ʈ ���� �� ����ð��� �̹��� �߰�.
//	#define		__CSC_VER9_5				// 9�� ���ð��� (�߰� �˱� ���)

//	#define		__Y_DRAGON_FIRE				// ���׿���Ŀ ���̾� �߻�!!!

//	#define		__LUASCRIPT					// ��� ��ũ��Ʈ ��� (World, Trans, Neuz)
//	#define		__EVENTLUA					// �̺�Ʈ (��� ��ũ��Ʈ ����) - World, Trans, Neuz

//	#define		__METEONYKER_0608
//	#define		__CSC_UPDATE_WORLD3D		// World3D Object Culling�κ� ������Ʈ
//	#define		__CSC_VER9_REMOVE_PKSETTING	// �ɼ� ���� â���� PK���� ���� (������ CtrlŰ ���� ���¿����� ����)

	#define		__SECURITY_0628				// ���ҽ� ���� ���� not contained

//	#define		__INSERT_MAP				// ��ŸƮ�޴��� ��ü���� �޴� �߰� 

	#define		__GLOBAL_COUNT_0705			// CTime::GetTimer
	
//	#define		__SKILL_0706				// ���ο� AddSkillProp �÷� �߰� �� ����

//	#define		__AI_0711					// ���׿���Ŀ AI ����

//	#define		__REMOVE_ATTRIBUTE			// �Ӽ����� ����(10���� ����)
//	#define		__CSC_ENCHANT_EFFECT_2		// ���� ����Ʈ + ���� ����Ʈ
	
//	#define		__SYS_TICKET				// �����
//	#define		__SYS_PLAYER_DATA			// ĳ���� ���� ����
//	#define		__HACK_1023					// ���ҽ� ���� // ���� �ӵ�, ���� ���� �ӵ�

//	#define		__RT_1025					// �޽���

//  #define		__LINK_PORTAL				// �ܺ� ��Ż ���� �� ������ ó��

//	#define		__VENDOR_1106				// ���λ��� ��� ����
//	#define		__JEFF_11

	#define		__JEFF_11_1

//	#define		__JEFF_11_4					// �Ʒ���
//	#define		__JEFF_11_5					// �޸� ����

//	#define		__DST_GIFTBOX				// ���� �����ۿ� ����Ʈ �ڽ� ��� �߰� 
	#define		__DISABLE_GAMMA_WND			// ������ ��忡�� ���� ���� �Ұ��ϰ� ����

	#define		__ATTACH_MODEL				// �𵨿� �ٸ� �� ���̱� (����...)
	#define		__NEW_PROFILE

	#define		__VTN_TIMELIMIT				// ��Ʈ�� �÷��� �ð� ����

//	#define		__FLYFF_INITPAGE_EXT

	#define		__BS_CHECKLEAK				// �޸� ���� Ž���� ���� Ȯ�� 2009/07/14
	#define		__BS_ADJUST_COLLISION		// �浹 ��ƾ ���� ( 2009. 07. 28 )
//	#define		__BS_PUTNAME_QUESTARROW		// ����Ʈ Ŭ���� ���� NPC ȭ��ǥ ǥ���� �̸� �߰�			( 09_1228 ���� )

	#define		__JAPAN_AUTH				// �Ϻ� ���� ����(������)
	#define		__IMPROVE_MAP_SYSTEM		// ���� ���� �ý���

//	#define		__BS_NO_CREATION_POST		// �ӽ� : ��ü�� ���� ����


	#undef		__VER
	#define		__VER 15

	#define		__GUILD_HOUSE_MIDDLE		// ����Ͽ콺 ����
	#define		__BS_ADDOBJATTR_INVISIBLE	// CObj Invisible �� ���� �Ӽ��߰�   --> 16�� ���� 
	#define		__BS_DEATH_ACTION			// die ���·� ���Խ� ���� ȿ�� ( client only )
	#define		__BS_EFFECT_LUA				// ������Ʈ ���º� ȿ������ ( Lua base )
	#define		__BS_ADD_CONTINENT_WEATHER	// ��� ���� �߰� ( �³�ȭ�� ���� �¾糯��, �ٸ� ������ε� ������ ���� )
	#define		__BS_CHANGEABLE_WORLD_SEACLOUD	// ���氡���� ���� �ٴٱ��� 

	
#elif defined(__TESTSERVER)  // �ܺ� ���� �׽�Ʈ���� 

	#define		NO_GAMEGUARD

	#define		__CPU_UTILDOWN_060502		// CPU ���� �����۾� 
	#define		__SLIDE_060502				// ������ ������ ���� ���� 
	#define     __GUILDVOTE					// ��� ��ǥ
//	#define		__SKILL0517					// ��ų ���� �Ķ����
	#define		__Y_CHARACTER_TEXT			// ĳ���� ����â ���α׷� �ؽ�Ʈ ���..Neuz
	#define		__V060721_TEXTDRAG			// ���ڿ� ȿ���ֱ�
	#define		__Y_ROTXZ					// ȸ�� �߰� Neuz, World
	#define		__Y_PATROL					// ����...Neuz. World
	#define		__Y_BEAUTY_SHOP_CHARGE		// ��Ƽ��, ���λ��� ���������ȭ..Neuz, World
	#define		__TRAFIC_1215				// ���۷� ����

//	#define		__ULTIMATE					// 9,10�� ����
//	#define		__LEGEND					// 10�� ���½ý���	Neuz, World, Trans
//	#define		__CSC_VER9_1				// 9�� ���°��� Clienet - Neuz

//	#define		__CSC_VER9_RESOLUTION		// Wide�� ���ػ� �߰�
//	#define		__Y_ADV_ENCHANT_EFFECT		// ���� ����Ʈ ��� ����
//	#define		__S_9_ADD					// 9�� �߰���
//	#define		__RECOVERY10				// 9�� �߰� (Ÿ��, �ǰ� 10�� �� �ڵ� ȸ��)

//	#define		__CSC_VER9_2				// 9�� ����â ���� ���� Neuz, World
//	#define		__CSC_VER9_3				// 9�� ä�û�Ȳ���� Function Key ��� ����
//	#define		__CSC_VER9_4				// Ŭ���̾�Ʈ ���� �� ����ð��� �̹��� �߰�.
//	#define		__CSC_VER9_5				// 9�� ���ð��� (�߰� �˱� ���)

//	#define		__Y_DRAGON_FIRE				// ���׿���Ŀ ���̾� �߻�!!!
	#define		__HACK_0516					// �̱� ��ŷ 2��

//	#define		__LUASCRIPT					// ��� ��ũ��Ʈ ��� (World, Trans, Neuz)
//	#define		__EVENTLUA					// �̺�Ʈ (��� ��ũ��Ʈ ����) - World, Trans, Neuz
//	#define		__CSC_VER9_REMOVE_PKSETTING	// �ɼ� ���� â���� PK���� ���� (������ CtrlŰ ���� ���¿����� ����)

//	#define		__INSERT_MAP				// ��ŸƮ�޴��� ��ü���� �޴� �߰� 

//	#define		__PET_0410					// 9, 10�� ��
//	#define		__AI_0509					// ���� �ΰ�����
//	#define		__METEONYKER_0608
//	#define		__SKILL_0706				// ���ο� AddSkillProp �÷� �߰� �� ����
//	#define		__AI_0711					// ���׿���Ŀ AI ����
	#define		__GLOBAL_COUNT_0705			// CTime::GetTimer

//	#undef		__VER
//	#define		__VER	11					// 11��

	#define		__JEFF_11_1
//	#define		__JEFF_11_4					// �Ʒ���
//	#define		__JEFF_11_5					// �޸� ����

//	#define		__JAPAN_SAKURA				// ���� �̺�Ʈ


//   VER 16 __TESTSERVER begin
	#define		__GUILD_HOUSE_MIDDLE		// ����Ͽ콺 ����
	#define		__BS_ADDOBJATTR_INVISIBLE	// CObj Invisible �� ���� �Ӽ��߰�   --> 16�� ���� 
	#define		__BS_DEATH_ACTION			// die ���·� ���Խ� ���� ȿ�� ( client only )
//	#define		__BS_EFFECT_LUA				// ������Ʈ ���º� ȿ������ ( Lua base )
	#define		__BS_ADD_CONTINENT_WEATHER	// ��� ���� �߰� ( �³�ȭ�� ���� �¾糯��, �ٸ� ������ε� ������ ���� )
	#define		__BS_CHANGEABLE_WORLD_SEACLOUD	// ���氡���� ���� �ٴٱ��� 
//   VER 16 __TESTSERVER end

	
#elif defined(__MAINSERVER)  // �ܺ� ����
//	#define		__TMP_POCKET				// �޴� ���� �Ͻ������� ����
	#define		__Y_BEAUTY_SHOP_CHARGE		// ��Ƽ��, ���λ��� ���������ȭ..Neuz, World
	#define		__TRAFIC_1215
//	#define     __Y_NEW_ENCHANT				// ���� ������ ����, Neuz, World
//	#define		__EVENT_FALL				// ��ǳ
//	#define		__JAPAN_SAKURA				// ���� �̺�Ʈ

	#define		__JEFF_11_1
//	#define		__RAIN_EVENT				// �帶 �̺�Ʈ(����� ���� ����ġ 2��)

#endif	// end - ���������� define 

#ifndef NO_GAMEGUARD 
	#define	__NPROTECT_VER	4	
#endif	

#endif // VERSION_COMMON_H
