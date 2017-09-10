#ifndef __DEFINE_JOB
#define __DEFINE_JOB

#define JTYPE_BASE   0
#define JTYPE_EXPERT 1
#define JTYPE_PRO    2
#define JTYPE_TROUPE 3
#define JTYPE_COMMON 4
#define JTYPE_MASTER	 5
#define JTYPE_HERO	 6

#define MAX_JOB_SKILL			3
#define MAX_EXPERT_SKILL		20
#define MAX_PRO_SKILL			20
#define MAX_TROUPE_SKILL		9
#define MAX_MASTER_SKILL		1
#define MAX_HERO_SKILL			1

#define MAX_JOB_LEVEL			15
#define MAX_EXP_LEVEL			45
#define MAX_PRO_LEVEL			30
#define MAX_TROUPE_LEVEL		1

#if __VER >= 15 // __HERO129_VER15				// 15�� ����� ����Ȯ��
#define MAX_LEGEND_LEVEL			129	
#define MAX_MONSTER_LEVEL			160	
#else	// 15�� ����� ����Ȯ��
#define MAX_LEGEND_LEVEL			121
#endif	// 15�� ����� ����Ȯ��

#if __VER >= 7 //__Y_MAX_LEVEL_8
#define MAX_LEVEL				120
#else //__Y_MAX_LEVEL_8
#define MAX_LEVEL				150
#endif //__Y_MAX_LEVEL_8
//-------------------------------------------------------
// ���� ��ȣ�� ������� �ٲٸ� �ȵ˴ϴ�.
// �ٲٰ� �����ø� seghope �����.. ����
//-------------------------------------------------------
// Job
#define JOB_VAGRANT                 0 
#define MAX_JOBBASE                 1
  
// Expert
#define JOB_MERCENARY               1 // �⺻ ������
#define JOB_ACROBAT                 2
#define JOB_ASSIST                  3
#define JOB_MAGICIAN                4
#define JOB_PUPPETEER               5
#define MAX_EXPERT                  6

// Professional
#define JOB_KNIGHT      		6
#define JOB_BLADE			7
#define JOB_JESTER		        8
#define JOB_RANGER                   9
#define JOB_RINGMASTER               10
#define JOB_BILLPOSTER               11
#define JOB_PSYCHIKEEPER		12
#define JOB_ELEMENTOR			13
#define JOB_GATEKEEPER               14
#define JOB_DOPPLER                  15
#define MAX_PROFESSIONAL             16

// Master
#define JOB_KNIGHT_MASTER      		16
#define JOB_BLADE_MASTER			17
#define JOB_JESTER_MASTER		        18
#define JOB_RANGER_MASTER                   19
#define JOB_RINGMASTER_MASTER               20
#define JOB_BILLPOSTER_MASTER               21
#define JOB_PSYCHIKEEPER_MASTER		22
#define JOB_ELEMENTOR_MASTER		23
#define MAX_MASTER	             24

// Hero
#define JOB_KNIGHT_HERO      		24
#define JOB_BLADE_HERO			25
#define JOB_JESTER_HERO		        26
#define JOB_RANGER_HERO                   27
#define JOB_RINGMASTER_HERO               28
#define JOB_BILLPOSTER_HERO               29
#define JOB_PSYCHIKEEPER_HERO		30
#define JOB_ELEMENTOR_HERO		31
#define MAX_HERO	             32

//

#if __VER >= 10    //#ifdef 	__LEGEND	//	9�� ���½ý���	Neuz, World, Trans
#define MAX_JOB                      32
#else //__LEGEND	//	9�� ���½ý���	Neuz, World, Trans
#define MAX_JOB                      16
#endif	//__LEGEND	//	9�� ���½ý���	Neuz, World, Trans

#define JOB_ALL					MAX_JOB
//-------------------------------------------------------

// SkillGroup    (Disciple)
#define DIS_VAGRANT                 0 //
#define DIS_SWORD                   1 //
#define DIS_DOUBLE                  2 //
#define DIS_CASE                    3 //
#define DIS_JUGGLING                4 //
#define DIS_YOYO                    5 //
#define DIS_RIFLE                   6 //
#define DIS_MARIONETTE              7 //
#define DIS_BOW			    32 // ���� �߰��� ��
// �������
#define DIS_SHIELD                  8 //
#define DIS_DANCE                   9 //
#define DIS_ACROBATIC              10 //
#define DIS_SUPPORT                23 //�Ӽ��ʸ��� ������ų
// �������� �����   
#define DIS_HEAL                   11 //
#define DIS_CHEER                  12 //
#define DIS_ACTING                 13 //
#define DIS_POSTER                 14 //
#define DIS_FIRE                   15 //
#define DIS_WIND                   16 //
#define DIS_WATER                  17 //
#define DIS_EARTH                  18 //
#define DIS_ELECTRICITY            24 //
// Ư�� ���    
#define DIS_STRINGDANCE            19 //
#define DIS_GIGAPUPPET             20 //
#define DIS_KNUCKLE                21 //
#define DIS_MAGIC                  22 //

#define DIS_MULTY		23
#define DIS_PSYCHIC		24
#define DIS_CURSE		25
#define DIS_HOLY		26
#define DIS_TWOHANDWEAPON	27
#define DIS_TWOHANDSWORD	28
#define DIS_TWOHANDAXE		29
#define DIS_DOUBLESWORD		30
#define DIS_DOUBLEAXE		31



// �ش� �Ҽ�
#define TRO_MASTER                  0 // ����
#define TRO_MEMBERE                 1 // ���

// ���Ҽ�
#define GUD_MASTER                  0 // ������
#define GUD_KINGPIN                 1 // ŷ��
#define GUD_CAPTAIN                 2 // ĸƾ
#define GUD_SUPPORTER               3 // ������
#define GUD_ROOKIE                  4 // ��Ű

// �Ϲ� ��� ����.... ĳ���� ���ο� ������� ����
#endif
/*
#ifndef __DEFINE_JOB
#define __DEFINE_JOB

#define JTYPE_BASE   0
#define JTYPE_EXPERT 1
#define JTYPE_PRO    2

#define MAX_JOB_SKILL    3
#define MAX_EXPERT_SKILL 14

#define MAX_JOB_LEVEL               10
#define MAX_EXP_LEVEL               40
#define MAX_PRO_LEVEL               30

// Job
#define JOB_MERCENARY               0 // �⺻ ������
#define JOB_ACROBAT                 1
#define JOB_ASSIST                  2
#define JOB_MAGICIAN                3
#define JOB_ENGINEER                4
#define JOB_VAGRANT                 5 // ���� ����
#define MAX_JOBBASE                 5
  
// Expert
#define JOB_GUARD                   5
#define JOB_STRONGMAN               6
#define JOB_MARKMAN                 7
#define JOB_PIERROT                 8
#define JOB_HEALER                  9
#define JOB_BILLPOSTER              10
#define JOB_PYRON                   11
#define JOB_RAINMAKER               12
#define JOB_PUPPETEER               13
#define JOB_GATEKEEPER              14
#define MAX_EXPERT                  15

// Professional
#define JOB_KNIGHT                  16 // ������ ������
#define JOB_BABARIAN                17
#define JOB_DANCER                  18
#define JOB_JESTER                  19 // ������ ������
#define JOB_STARGAZER               20
#define JOB_ACCOUNTER               21
#define JOB_RAIDEN                  22 // ���ͼ��� ������
#define JOB_OVERCASTER              23
#define JOB_DOPPLER                 24
#define JOB_STRINGMASTER            25 // ��ڽ� ������
#define MAX_PROFESSIONAL            26 
#define MAX_JOB                     26 


// SkillGroup
#define DIS_SWORD                   0 //
#define DIS_AXE                     1 //
#define DIS_MACE                    2 //
#define DIS_TOOL                    3 //
#define DIS_CASE                    4 //
#define DIS_SPEAR                   5 //
#define DIS_WHIP                    6 //
#define DIS_BOW                     7 //
#define DIS_THROWING                8 //
#define DIS_PISTOL                  9 //
#define DIS_RIFLE                  10 //
// �������
#define DIS_SHIELD                 11 //
#define DIS_PARRY                  12 //
#define DIS_ACROBAT                13 //

// �������� �����   
#define DIS_DIVINEPOWER            14 //
#define DIS_HEAL                   15 //
#define DIS_ASTROLOGY              16 //
#define DIS_FIRE                   17 //
#define DIS_WIND                   18 //
#define DIS_ELECTRICITY            19 //
#define DIS_DARK                   20 //
#define DIS_WATER                  21 //
#define DIS_EARTH                  22 //
// Ư�� ���    
#define DIS_CHEER                  23 //
#define DIS_PROPERTIES             24 //
#define DIS_TAMING                 25 //
#define DIS_TALISMANS              26 //
#define DIS_HERCULEAN              27 //
#define DIS_PUPPET                 28 //
#define DIS_MARIONETTE             29 //
#define DIS_STRINGDANCE            30 //

// �Ϲ� ��� ����.... ĳ���� ���ο� ������� ����
#endif
*/
