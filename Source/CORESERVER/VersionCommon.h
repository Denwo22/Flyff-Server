#ifndef __VERSION_COMMON_H__
#define __VERSION_COMMON_H__
#define __MAINSERVER
#define		__VER 15	// 15��

#if !defined( __TESTSERVER ) && !defined( __MAINSERVER )
	#define __INTERNALSERVER
#endif

#if (_MSC_VER > 1200)
#define		__VS2003		// �����Ϸ�����.net
#endif

#define		__SERVER				// Ŭ���̾�Ʈ �����ڵ带 �������� �ʱ� ���� define
#define		__MAP_SIZE						
#define		__S8_SERVER_PORT		// 2006�� 11�� 7�� ������Ʈ - World, Cache, CoreServer
#define		__EVE_NEWYEAR			// �ų� ȿ��
#define		__STL_0402				// stl

//
//#define		__HACK_0516 // �̱� ��ŷ 2��
#define		__JEFF_9_20		// ���� �ð��� ħ�� �ð����� ����

//
#define		__MA_VER11_01			// �شܷ���������Ʈ����
//#define		__SYS_PLAYER_DATA
#define		__RT_1025				// �޽���


#define		__AUTO_NOTICE		// �ڵ� ����

// 12��
//#define		__JHMA_VER12_1	//12�� �ش����������  world,core
//#define		__PARSKILL1001	//12�� �Ľ�ų ������ ����  world,core,neuz


// 13��
#define		__LAYER_1015		// ���� ��ü ��

// 14��
//	#define		__INSTANCE_DUNGEON			// 14�� �ν��Ͻ� ���� ���
//	#define		__PARTY_DUNGEON				// 14�� �ش� ���� �ν��Ͻ� ����
	#define		__QUIZ						// ���� �̺�Ʈ �ý���

	#define		__EVENTLUA_RAIN				// �帶 �̺�Ʈ -> ��Ʒ� ����

	#define		__EVENTLUA_SNOW				// ���� �̺�Ʈ

// 15��
//	#define		__HERO129_VER15				// 15�� ����� ����Ȯ��

	#define		__ENVIRONMENT_EFFECT


#if	  defined(__INTERNALSERVER)	// ���� �繫�� �׽�Ʈ���� 
	
	#define		__GUILDVOTE			// ��� ��ǥ 

	#define		__HACK_0516			// �̱� ��ŷ 2��
//	#define		__SYS_PLAYER_DATA
//	#define		__RT_1025			// �޽���


#elif defined(__TESTSERVER)		// �ܺ� ���� �׽�Ʈ���� 

//	#define		__NOTIFYSTART0706	// ���Ŀ��� ���� �˸� ����� �̺�Ʈ��  
	#define		__GUILDVOTE			// ��� ��ǥ 

	#define		__HACK_0516			// �̱� ��ŷ 2��

//	#define		__SYS_PLAYER_DATA
//	#define		__RT_1025			// �޽���

//	#define		__JAPAN_SAKURA				// ���� �̺�Ʈ


#elif defined(__MAINSERVER)		// �ܺ� ����
//	#define		__RAIN_EVENT		// �帶 �̺�Ʈ(����� ���� ����ġ 2��)

//	#define		__JAPAN_SAKURA				// ���� �̺�Ʈ
#endif	// end - ���������� define 


#endif


