#ifndef __COMMON_DEFINE__
#define __COMMON_DEFINE__

#include <windows.h>

//////////////////////////////////////////////////////////////////////////////
// Ŭ���̾�Ʈ �� �������� �������� ����ϴ� ��(constant), ���� �����ϴ� ����
//////////////////////////////////////////////////////////////////////////////
const int MAX_SHORTCUT_STRING = 128;
const int MAX_GOLD_COIN = 32767;
const int MAX_GOLD_REWARD = INT_MAX;
const int MAX_INPUT_REWARD = 200000000;
const int MIN_INPUT_REWARD = 1000;
const int REQ_WANTED_GOLD = 1000;			// ����� ��ġ���� ���� �� 

const unsigned char	REMOVE_VOTE	= 0;		// ��ǥ���� 
const unsigned char	CLOSE_VOTE	= 1;		// ��ǥ���� 
const unsigned char	CAST_VOTE	= 2;		// ��ǥ�ϱ� 
const int			TIMEWAIT_CLOSE = 10;	// �� ��ȭ���� ������ �ð�  
const	int		TIMEWAIT_RELEASE	= 3600;
const	int		TIMEWAIT_INSTANT_BANKER		= 180000;


const int MAX_CHARGE_LEVEL = 4;

extern const int OLD_MPU;
extern int g_MPU;

enum ATK_TYPE 
{
	ATK_FORCE,					// �̸����� �������� ������ �� ��� 
	ATK_MELEESKILL,				// �и���ų 
	ATK_MAGICSKILL,				// ���� ��ų 
	ATK_MAGIC,					// �ϵ� ���� 
	ATK_GENERIC,				// �Ϲ� ���� 
};

class CMover;

struct ATTACK_INFO 
{
	CMover*		pAttacker;			// ������ 
	CMover*		pDefender;			// ����� 
	DWORD		dwAtkFlags;			// ���� �÷���
	int			nParam;				// ���� �߰� �÷��� 
	int			nParts;				// �������ΰ� �޼��ΰ� 
	int			nDEFFactor;			// ���� ���� 

	ATK_TYPE	GetAtkType() const;
	BOOL		CanIgnoreDEF() const;
	int			GetChargeLevel() const;
	int			GetAttackCount() const;
	int			GetSkill() const;
	BOOL		IsRangeAttack() const;
	int			GetSkillLevel() const; 
};

#ifdef __CLIENT
	inline int ATTACK_INFO::GetSkill() const
	{
		return 0;
	}
#endif

#endif	// __COMMON_DEFINE__
