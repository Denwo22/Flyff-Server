#ifndef	__MOVERMSG_H__
#define	__MOVERMSG_H__


#define		OBJSTA_NONE					0x00000000	// �ƹ��͵� �ƴѻ���.

// stop/move
#define		OBJSTA_STAND				0x00000001	// ������
#define		OBJSTA_STAND2				0x00000002	// ������EX
#define		OBJSTA_SIT					0x00000003	// �ɱ�
#define		OBJSTA_FMOVE				0x00000004	// ������ �ȱ�
#define		OBJSTA_BMOVE				0x00000005	// �ڷ� �ȱ�
#define		OBJSTA_LEFT					0x00000006	// ������ �ȱ�(�����̵�)
#define		OBJSTA_RIGHT				0x00000007	// 
#define		OBJSTA_PICKUP				0x00000008	// (���ǵ���)�ݴ�.

#define		OBJSTA_LMOVE				0x00000009	// ����������� �ȱ�
#define		OBJSTA_RMOVE				0x0000000a	// ������������� �ȱ�
#define		OBJSTA_STOP_RUN				0x0000000b	// ���ڸ� �ٱ�

#define		OBJSTA_MOVE_ALL				0x000000FF
#define		OBJSTA_NOT_STAND			(OBJSTA_MOVE_ALL & (~OBJSTA_STAND))		// STAND�̿ܰ� �ִ°�.

// turn
#define		OBJSTA_LTURN				0x00000100	// �� ��
#define		OBJSTA_RTURN				0x00000200	// �� ��
#define		OBJSTA_TURN_ALL				0x00000300	//
// look up/down
#define		OBJSTA_LOOKUP				0x00000400	//
#define		OBJSTA_LOOKDOWN				0x00000800	//
#define		OBJSTA_LOOK_ALL				0x00000C00

// jump
#define		OBJSTA_FJUMP_READY			0x00001000	// ������ ���� - ������
#define		OBJSTA_FJUMP				0x00002000	// ������
#define		OBJSTA_FFALL				0x00003000	// ����������
#define		OBJSTA_FLAND				0x00004000	// ����
#define		OBJSTA_SJUMP1				0x00005000	// ������ ���� - ���ڸ�����
#define		OBJSTA_SJUMP2				0x00006000	// ������
#define		OBJSTA_SJUMP3				0x00007000	// ����������
#define		OBJSTA_SJUMP4				0x00008000	// ����
#define		OBJSTA_BJUMP_READY			0x00009000	// ������ ���� - ������
#define		OBJSTA_BJUMP				0x0000a000	// ������
#define		OBJSTA_BFALL				0x0000b000	// ����������
#define		OBJSTA_BLAND				0x0000c000	// ����
#define		OBJSTA_JUMP_ALL				0x0000F000
// attack
#define		OBJSTA_ATK1					0x00010000	// 1����Ÿ ���ݵ�����
#define		OBJSTA_ATK2					0x00020000	// 2����Ÿ ���ݵ�����
#define		OBJSTA_ATK3					0x00030000	// ...
#define		OBJSTA_ATK4					0x00040000	// ...				// ���߿� "������"�̶�� �������� ���յ����� �𸣰ڴ�....
// wand attack
#define		OBJSTA_ATK_MAGIC1    		0x00050000	// �ϵ���ݵ�����
#define		OBJSTA_ATK_RANGE1    		0x00060000	// ���Ÿ����ݵ�����
#define		OBJSTA_RANGE3				0x00070000	// 
#define		OBJSTA_RANGE4				0x00080000	// 
// melee skill
#define		OBJSTA_ATK_MELEESKILL		0x00090000	// ���� ������ų���� ��
#define		OBJSTA_ATK_RANGESKILL		0x000a0000	// ��Ÿ� ������ų���� ��
#define		OBJSTA_ATK_CASTING1			0x000b0000	// ���� ĳ���� 1�� (����)
#define		OBJSTA_ATK_CASTING2			0x000c0000	// ���� ĳ���� 2�� (�ݺ�.)
#define		OBJSTA_ATK_MAGICSKILL		0x000d0000	// ���� �߻� ����.
#define		OBJSTA_SP_ATK1				0x00100000	// Ư������ : ���Ͱ� ���.
#define		OBJSTA_SP_ATK2				0x00200000	// Ư������2 : ���Ͱ� ���.
#define		OBJSTA_ATK_ALL				0x00FF0000
//#define		OBJSTA_RANGE_ALL			0x0f000000
// damage
#define		OBJSTA_DMG					0x01000000	// �ǰ�
#define		OBJSTA_DMG_RESERVED1		0x04000000	
#define		OBJSTA_DMG_RESERVED2		0x05000000	

#define		OBJSTA_DMG_FLY				0x02000000	// �� �ǰ� - ���ư���
#define		OBJSTA_DMG_DIE				0x03000000	// �� �ǰ��� ���� �н�!
#define		OBJSTA_DMG_LIVE				0x06000000	// ������ ���¿��� �ٽ� �Ͼ��.
#define		OBJSTA_DMG_FLY_ALL			0x02000000	// ���ư��� ������ ���. �ٸ������� 2��° ��Ʈ�� ���� �ʵ�������.

#define		OBJSTA_DEAD					0x08000000	// �׾� �κη��� ����(��������)
#define		OBJSTA_DISAPPEAR			0x09000000	// �׾� �������� ����
#define		OBJSTA_RESURRECTION			0x0C000000	// ��Ȱ 0xd = 1101
#define		OBJSTA_DEAD_RESERVED1		0x0D000000	
#define		OBJSTA_DIE_ALL				0x08000000	// ���� - 0x00800000 ��Ʈ�� ���̸� ���� ����.
#define		OBJSTA_DMG_ALL				0x0F000000	// �ǰ� & ���� ���

// �Ϲ� action - �ٸ� � ��ɵ� �� �׼��߿��� ������ ����ģ��.
#define		OBJSTA_COLLECT				0x10000000	// ä��.
#define		OBJSTA_APPEAR				0x20000000	// �����.
#define		OBJSTA_APPEAR2				0x30000000  // ����� 2
#define		OBJSTA_STUN					0x40000000	// ���� ����.
#define		OBJSTA_ACTION_ALL			0xF0000000	// 

#define		OBJSTA_ALL	(OBJSTA_MOVE_ALL | OBJSTA_TURN_ALL | OBJSTA_LOOK_ALL | OBJSTA_JUMP_ALL | OBJSTA_ATK_ALL | OBJSTA_DMG_ALL | OBJSTA_ACTION_ALL )


// State Flag
#define		OBJSTAF_COMBAT				0x00000001	// ���� ���
#define		OBJSTAF_WALK				0x00000002	// �ȴ� ���
#define		OBJSTAF_SIT					0x00000004	// �ɱ� ����
#define		OBJSTAF_FLY					0x00000008	// ���� ���
#define		OBJSTAF_ACC					0x00000010	// ���� ����
#define		OBJSTAF_ETC					0x00000020	// Ŀ���� ���
#define		OBJSTAF_ACCTURN				0x00000040	// �� ��ȸ.
#define		OBJSTAF_TURBO				0x00000080	// �ͺ� ���.

// Act Message
enum OBJMSG
{
	OBJMSG_NONE,
	OBJMSG_FORWARD,		// ���� �ض�
	OBJMSG_BACKWARD,	// ���� �ض�
	OBJMSG_STAND,		// ���ڸ��� ���־��
	OBJMSG_SITDOWN,		// �ɾƶ�
	OBJMSG_STANDUP,		// ����.(�ɾ�������)
	OBJMSG_STOP,		// �����
	OBJMSG_ASTOP,		// �����
	OBJMSG_LTURN,		// �������� ���ƶ�
	OBJMSG_RTURN,		// ���������� ���ƶ�
	OBJMSG_STOP_TURN,		// ���°� �����
	OBJMSG_PICKUP,		// �ݴ�.
	OBJMSG_RESURRECTION,	// ��Ȱ.
	OBJMSG_COLLECT,		// ä��.
	OBJMSG_APPEAR,		// ����
	OBJMSG_APPEAR2,		// ����2
	OBJMSG_STUN,		// ���ϻ���
	OBJMSG_JUMP,		// ����
	OBJMSG_MODE_COMBAT,		// ������尡 �Ǿ��
	OBJMSG_MODE_PEACE,		// ��ȭ��尡 �Ǿ��,
	OBJMSG_MODE_WALK,		// �ȱ��尡 �Ǿ��
	OBJMSG_MODE_RUN,		// �ٱ��尡 �Ǿ��
	OBJMSG_MODE_FLY,		// �����尡 �Ǿ��
	OBJMSG_MODE_GROUND,		// �����尡 �Ǿ��
	OBJMSG_MODE_TURBO_ON,	// ���� ���� ���.
	OBJMSG_MODE_TURBO_OFF,	// ���Ӹ�� ����.
	OBJMSG_MODE_TURN,
	OBJMSG_ACC_START,	// ���� ����
	OBJMSG_ACC_STOP,	// ���� ����
	OBJMSG_ATK1,		// ��Ÿ���� 1����
	OBJMSG_ATK2,		// ��Ÿ���� 2����..
	OBJMSG_ATK3,
	OBJMSG_ATK4,
	OBJMSG_SP_ATK1,		// Ư������ : ���Ͱ� ���.
	OBJMSG_SP_ATK2,
	OBJMSG_ATK_RANGE1,	// ���Ÿ����� 1���� - Ȱ����...
	OBJMSG_ATK_MAGIC1,	// �ϵ����
	OBJMSG_MELEESKILL,
	OBJMSG_MAGICSKILL,
	OBJMSG_MAGICCASTING,
	OBJMSG_DIE,		// �׾��!
	OBJMSG_DAMAGE,			// ����������.
	OBJMSG_DAMAGE_FORCE,	// �������� - ������ ���ư�.
	OBJMSG_LOOKUP,
	OBJMSG_LOOKDOWN,
	OBJMSG_STOP_LOOK,
	OBJMSG_MOTION,
	OBJMSG_TURNMOVE,
	OBJMSG_TURNMOVE2,
	OBJMSG_BASE_RADY,		// �⺻ ���� ����
	OBJMSG_TEMP,
	OBJMSG_TEMP2,	// ������ ���ݸ޽���
	OBJMSG_TEMP3,
	OBJMSG_DESTPOS,
	OBJMSG_DESTOBJ,
	OBJMSG_FALL,
	OBJMSG_LFORWARD,
	OBJMSG_RFORWARD,
	OBJMSG_STOP_RUN,
};

// ������Ʈ �ൿ
enum OBJACT
{
	OBJACT_NONE,
//	OBJACT_MOVETO_OBJ,	
//	OBJACT_MOVETO_POS,
	OBJACT_MELEE_ATTACK,
	OBJACT_MAGIC_ATTACK,
	OBJACT_RANGE_ATTACK,
	OBJACT_SP_ATTACK,
	OBJACT_USESKILL,
	OBJACT_USEITEM,
	OBJACT_COLLECT,

};

#define		MVRF_NONE			0x00000000
#define		MVRF_SKILL			0x00000001		// ���� ��ų �����.
#define		MVRF_CRITICAL		0x00000002		// ũ��Ƽ�� ���. 1ȸ��.
#define		MVRF_TRACKING		0x00000004		// ������ �ڵ����������.
#define		MVRF_WARMARK		0x00000008		// ������ũ�� �Ӹ����� ������. 0�̸� CreateSfx�� ���ְ� 1�� ����� �ش�.
#define		MVRF_COLLECT		0x00000010		// ä�� ����Ʈ.
#define		MVRF_HITCONT		0x00000020		// �Ϲݰ��� Ÿ�����ĺ��� �ִϳ��������� ��� ON
#define		MVRF_LASERCHARGE	0x00000040		// ������ ������.
#define		MVRF_EFFECT1		0x00000080		// ������ �ϳ��� �ް�ٴҼ� �ְ� �ϴ� ����Ʈ(������������..)
#define		MVRF_NOMOVE			0x00000100		// �̵�����. ���ڸ����� �̵�����.
#define		MVRF_NOATTACK		0x00000200		// ���ݱ���. ����/�ϵ�/��ų�� ���� �� ��ų�� ��� ����
#define		MVRF_NOUSERITEM		0x00000400		// �����ۻ�����. 
#define		MVRF_NOANI			0x00000800		// �ִϸ��̼� ����.
#define		MVRF_NOACTION		(MVRF_NOMOVE | MVRF_NOATTACK | MVRF_NOUSERITEM)		// �׼Ǳ���( �̵�/����/��ų/������ ��� ���Ѱ� )
#define		MVRF_FREEZE			(MVRF_NOANI | MVRF_NOACTION)	// �ִϸ��̼ǵ� ���߰� ���۵� �� �����.
#define		MVRF_MEDITATION_SIT	0x00001000		// �޵����̼ǻ���� �ɱ� �÷��� - ���� �ѹ��� �ɾƾ� �ϱ⶧��.
#define		MVRF_SETITEMSFX		0x00002000		// ��Ʈ ������ ȿ�� ����Ʈ


#define		ENEMY_PVP_PARTY		0		// ��Ƽ ���
#define		ENEMY_WAR			1		// �����
#define		ENEMY_PK			2		// ������
#define		ENEMY_PVP			3		// ���

#endif	// __MOVERMSG_H__
