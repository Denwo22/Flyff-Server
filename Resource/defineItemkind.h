#ifndef __DEFINE_ITEMKIND
#define __DEFINE_ITEMKIND

// IK_2�� ���� �ǹ����� ����, IK_3�� ������ ������ ���

////////////////////////////////////////////////////////////////////////////
// 1�� ���� 
//////////////////////////////////////////////////////////////////////////
#define IK1_GOLD                   0 // ��<-
#define IK1_WEAPON                 1 // ������<-
#define IK1_ARMOR                  2 // ������<-
#define IK1_GENERAL                3 // ������<-
#define IK1_RIDE                   4 // ������<-
#define IK1_SYSTEM                 5 // <-
#define IK1_CHARGED				   6 // ���ȭ ������ ���� �Ǵ�.
#define IK1_HOUSING					7	// �Ͽ�¡

////////////////////////////////////////////////////////////////////////////
// 2�� ���� 
//////////////////////////////////////////////////////////////////////////
#define IK2_GOLD                   0 // ��<-
#define IK2_WEAPON_HAND            1 // �ܰŸ�<-
#define IK2_WEAPON_DIRECT          2 // �ܰŸ�<-
#define IK2_WEAPON_MAGIC           3 // ����<-
#define IK2_ARMOR                  7 // ��(����, ����)<-
#define IK2_ARMORETC               8 // �ֺ� ��(��Ʋ�� ���)<-
#define IK2_CLOTH                  9 // �ǻ�(�Ϲ� �ǻ��)<-
#define IK2_CLOTHETC              10 // �ֺ� �ǻ�(��Ʋ�� ���)<-
#define IK2_REFRESHER             11 // ��� ����<-
#define IK2_POTION                12 // ���� ����<-
#define IK2_JEWELRY               13 // ��ű�<-
#define IK2_FOOD                  14 // ����<-
#define IK2_MAGIC                 15 // ������ǰ<-
#define IK2_GEM                   16 // ����<-
#define IK2_MATERIAL              17 // ���� ���<-
#define IK2_TOOLS                 18 // ����<-
#define IK2_SYSTEM                19 // �ý��ۿ� ���<-
#define IK2_RIDING                20 // ���� ����ü<-
#define IK2_MOB                   21 // <-���Ϳ��� ���Ǵ� ������
#define IK2_BLINKWING             22 //<-
#define IK2_AIRFUEL               23 // ����ü �����<-
#define IK2_CHARM                 24 // ������
#define IK2_BULLET                25 // �߻�ü
#define IK2_TEXT                  26 // ����
#define IK2_GMTEXT				  27 // GM���
#define IK2_GENERAL				  28 // �屺������...�� �ƴϰ� �� �����Ұ� ������ �̰� �� -_-;;;;
#define	IK2_BUFF					29	// ���� ������
#define	IK2_WARP				30
#define IK2_SKILL				31
#define IK2_CLOTHWIG			32	// �Ӹ��ǻ�(����)
#define	IK2_BUFF2		33	// ���� ������ - ���� �ð�
#define IK2_FURNITURE		34	// �Ͽ�¡ - ����
#define IK2_PAPERING		35	// �Ͽ�¡ - ����
#define	IK2_TOCASH		36	// �۴� ����
#define	IK2_BUFF_TOGIFT		37	// ���� �Ϸ� �� ������ ����
#define	IK2_GUILDHOUSE_FURNITURE		38 //����Ͽ콺 ����
#define	IK2_GUILDHOUSE_NPC		39 //����Ͽ콺 NPC
#define IK2_GUILDHOUSE_PAPERING 40 //����Ͽ콺 ����
#define	IK2_GUILDHOUES_COMEBACK	41 //����Ͽ콺 ��ȯ �ֹ���


////////////////////////////////////////////////////////////////////////////
// 3�� ���� 
//////////////////////////////////////////////////////////////////////////
#define IK3_GOLD                   0 //<-
// ���⿡ ���õ� �� (IK_WEAPON �Ҽ�)
#define IK3_HAND                   1 //<-
#define IK3_SWD                    2 //<-
#define IK3_AXE                    3 //<-
#define IK3_CHEERSTICK             4 // <-
#define IK3_KNUCKLEHAMMER          5 //<-
#define IK3_WAND                   6 //<-
#define IK3_STAFF                  7 //<-
#define IK3_THSWD                  8 // ��� �ҵ�
#define IK3_THAXE                  9 // ��� ����
#define IK3_VIRTUAL               10 //<-���Ͱ� ����ϴ� ���� ������
#define IK3_YOYO                  11 //���
#define IK3_BOW                   12 //Ȱ
#define IK3_YOBO				  13 //���� Ȱ�� ���ÿ� ���

// ���� ���õ� �� (IK_ARMOR �Ҽ�)
#define IK3_SHIELD                16 // ����<-
#define IK3_HELMET                17 // �Ӹ� ��ȣ ��<-
#define IK3_SUIT                  18 // ��ü(��¥) ��ȣ ��<-
#define IK3_GAUNTLET              19 // �� ��ȣ ��<-
#define IK3_BOOTS                 20 // �� ��ȣ ��<-

// �ǻ� ���õ� �� (IK2_CLOTH �Ҽ�)
#define IK3_HAT                   21 // ���� �ǻ�<-
#define IK3_MASK                  22 // �Ź� �ǻ�<-
#define IK3_SHOES                 23 // �Ź� �ǻ�<-
#define IK3_CLOAK                 24 // ���� �ǻ�<-
#define IK3_CLOTH                 57 // ��
#define IK3_GLOVE                 58 // �尩

// �Ϲ� �����۵�
#define IK3_REFRESHER             25 // ���ŷ� ġ��<-
#define IK3_POTION                26 // ���� ����<-
#define IK3_EARRING               27 // �Ͱ���<-
#define IK3_NECKLACE              28 // �����<-
#define IK3_RING                  29 // ����<-
#define IK3_INSTANT               30 // �Ｎ���� ������ �ְ� ������� ����<-
#define IK3_COOKING               31 // �丮�� ����(����)<-
#define IK3_ICECEARM              32 // ������� ���̽�ũ��2<-
#define IK3_PILL                  59 // ���� ���� �˾�
#define IK3_MAGICTRICK            33 // ���� ������ǰ<-
#define IK3_GEM                   34 // �׳� ����<-
#define IK3_DRINK                 35 // Ȱ����<-
#define IK3_COLLECTER		      36 // �ݷ���<-
#define IK3_ELECARD		          37 // �Ӽ� ī��<-
#define IK3_DICE		          38 // �Ŀ� �ֻ���<-
#define IK3_SUPSTONE		      39 // ������<-
 
// Ż��
#define IK3_BOARD                 40 // ���� ������ Ż ��(�ջ�밡��)<-
#define IK3_STICK                 41 // �ö� Ÿ�� ��(�ջ��Ҵ�)<-
#define IK3_EVENTMAIN             42 // <-�̺�Ʈ�� ������  �����ڵ�, �Ǹ�, Ʈ���̵�, Ÿ���ݱ� �Ұ�
#define IK3_QUEST                 43 // ����Ʈ�� ������<-
#define IK3_MAP                   44 // ����Ʈ�� ������<-
#define IK3_BLINKWING             45 // ����Ʈ�� ������<-
#define IK3_EVENTSUB              46 // <-�̺�Ʈ�� ������  �����ڵ�, �Ϲݾ����۰� ����
#define IK3_TOWNBLINKWING         47 // <-�����θ� ���ư��� �Ǵ� ��ũ��

//����ü ������2
#define IK3_ACCEL		          48 // ���� ���� ������<-
#define IK3_DELETE		          49 // <-dwItemkind3�� �̰����� �����Ǹ� ĳ���Ͱ� �ε��� �� �ڵ����� �����.

//���ȭ ������
#define IK3_SCROLL		        50 //<-
#define IK3_ENCHANTWEAPON		51
#define IK3_CFLIGHT		        52
#define IK3_BFLIGHT		        53
#define IK3_MAGICBOTH			54
#define IK3_BCHARM		        55
#define IK3_RCHARM		        56 
#define IK3_ARROW		        60

//���� ī�� �� �Ǿ�� �ֻ���
#define IK3_PIERDICE			61
#define IK3_SOCKETCARD			62
#define IK3_SOCKETCARD2			63

//���� ����
#define IK3_TEXT_BOOK           	70 // ����
#define IK3_TEXT_SCROLL	        	71 // ����
#define IK3_TEXT_LETTER         	72 // ����

//��� ��� ������ȭ
#define	IK3_TEXT_UNDYING		80
#define	IK3_TEXT_DISGUISE		81
#define IK3_TEXT_INVISIBLE		82
#define IK3_TEXT_GM			83

// BINDS
#define	IK3_BINDS			84

// ���� ��ȯ
#define IK3_CREATE_MONSTER		85

#define IK3_POTION_BUFF_STR		90		// ���� ����
#define IK3_POTION_BUFF_DEX		91		// ���� ����
#define IK3_POTION_BUFF_INT		92		// ���� ����
#define IK3_POTION_BUFF_STA		93		// ���� ����
#define IK3_POTION_BUFF_DEFENSE	94		// ���� ����

#define IK3_ANGEL_BUFF			95		// ���� ����

// PET
#define IK3_PET				100		// ���ȯ ������
#define IK3_RANDOM_SCROLL		101		// ���� ��Ŭ��

#define IK3_ULTIMATE			102		// ���͸� ���� ����

#define	IK3_LINK	104
// General
#define IK3_GENERAL			118		// �ƹ� ������ ���� �Ϲ�.

#define IK3_ENCHANT			119

#define IK3_EGG		120		// ���� ��
#define	IK3_FEED	121

#define	IK3_TICKET	122		// �����
#define	IK3_POCKET	123		// �ָӴ�

#define IK3_BED			124		// �Ͽ�¡ - ���� - ħ��
#define IK3_SOFA		125		// �Ͽ�¡ - ���� - ����
#define IK3_WARDROBE	126		// �Ͽ�¡ - ���� - ����
#define IK3_CLOSET		127		// �Ͽ�¡ - ���� - ȭ���
#define IK3_TABLE		128		// �Ͽ�¡ - ���� - Ź��
#define IK3_CABINET		129		// �Ͽ�¡ - ���� - �����
#define IK3_PROPS		130		// �Ͽ�¡ - ���� - ��ǰ
#define IK3_WALLPAPER	131		// �Ͽ�¡ - ���� - ����
#define IK3_CARPET		132		// �Ͽ�¡ - ���� - ����
#define	IK3_COUPLE_BUFF	133		// Ŀ�� ȿ��
#define	IK3_FUNNYCOIN	134		// �۴� ����
#define	IK3_FLOWER	135		// ��ħ ������ ���� ������. ���� ���� ��� �ȵ�
#define IK3_BALLOON	136		// ǳ��
#define IK3_WING	137		// ����

#define IK3_VIS		138		// ��
#define	IK3_TS_BUFF	139

#define IK3_TELEPORTER		140 //����Ͽ콺 �ڷ�����
#define IK3_REST			141	//�޽��� ���
#define	IK3_DESK			142	// �Ͽ�¡ - ���� - å��
#define	IK3_CHAIR			143	// �Ͽ�¡ - ���� - ����
#define	IK3_CASE			144	// �Ͽ�¡ - ���� - å��
#define	IK3_BATH			145	// �Ͽ�¡ - ���� - ����
#define	IK3_DRAWER			146	// �Ͽ�¡ - ���� - ��Ź

#define	IK3_CRYSTAL			147

#define IK3_KEY				148

#define MAX_ITEM_KIND3      149	// ItemKind3 �� ���� 
#define MAX_UNIQUE_SIZE	    400

// ��Ʈ�� ������Ʈ Kind
#define CK1_CHEST				0    // <-��������
#define CK1_DOOR				1    // <-��
#define CK1_TRIGGER				2    // ���� ������Ʈ
#define CK1_HOUSING				3    // �Ͽ�¡ ���� ��Ʈ��
#define	CK1_GUILD_HOUSE				4    // ��� ���� ��Ʈ��

#define CK2_FADE				1    // ������� ��Ʈ��
#define CK2_KEEP				2    // �������� �ʴ� ��Ʈ��

#define CK3_FULL				1    // Ǯ���ϸ��̼�
#define CK3_HALF				2    // 1/2 ���ϸ��̼�

#define WEAPON_GENERAL			0	// �Ϲ� ����
#define WEAPON_UNIQUE			1	// ����ũ ����
#define WEAPON_ULTIMATE			2	// ���͸� ����

#define ARMOR_SET				3	// ��Ʈ ��

#define PET_VIS					1	// VisPet
#endif