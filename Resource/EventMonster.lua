-- Init -------------------------------------
-- tEventMonster �ʱ�ȭ, �Լ� �ε�
dofile( ".\\LuaFunc\\EventMonsterFunc.lua" ) -- �������� ����!!!
---------------------------------------------
-- AddMonster( "Monster_ID", nLevel, nLootTime, fItemDropRate, bPet, bGiftBox )
-- 		"Monster_ID"	-> ���� ID
-- 		nLevel			-> ĳ���ͷ��� - ���ͷ��� >= nLevel �϶� ���ݺҰ�
-- 		nLootTime		-> ������ ���� �ð�(sec)
--		fItemDropRate	-> ������ ��� ����(�Ҽ� ����)
-- 		bPet			-> ���� ���氡�� ����
-- 		bGiftBox		-> ����Ʈ �ڽ� ���� ����
--
-- ** �̺�Ʈ������ ���۵� �ű� ���͸� �߰��� ��!!!
--------------------------------------------------

--AddMonster( "MI_AIBATT1", 15, 0, false, false )
--AddMonster( "MI_AIBATT2", 15, 0, false, false )
AddMonster( "MI_EVENT01", 20, 3, 10, false, false )
AddMonster( "MI_EVENT02", 20, 3, 10, false, false )
AddMonster( "MI_EVENT03", 20, 3, 10, false, false )
AddMonster( "MI_EVENT04", 20, 3, 10, false, false )