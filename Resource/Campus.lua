--------------------------------------------------------------------
-- �ʱ�ȭ ----------------------------------------------------------
--------------------------------------------------------------------
dofile( ".\\LuaFunc\\CampusFunc.lua" )
--------------------------------------------------------------------

RecoveryTime		=	MIN(60)		-- ���� ����Ʈ ȸ�� �ֱ�
RecoveryPoint		=	1		-- ȸ�� ����Ʈ


-- �Ϸ��ؾ� �� ���� ����Ʈ
SetQuest( "QUEST_SCE_MDRIGALTEACHER5" )


-- ������ ���������� ���� ������ ����
SetBuffLevel( 1, "II_TS_BUFF_POWER_LOVE01" )
SetBuffLevel( 2, "II_TS_BUFF_POWER_LOVE02" )
SetBuffLevel( 3, "II_TS_BUFF_POWER_LOVE03" )


-- ������ �ش� ������ �� ���� ���� ����Ʈ
SetCampusReward( 15, 1, 1 )
SetCampusReward( 40, 1, 2 )
SetCampusReward( 60, 1, 2 )
SetCampusReward( 75, 2, 5 ) -- ����


