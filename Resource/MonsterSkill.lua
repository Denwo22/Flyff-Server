--------------------------------------------------
-- �ʱ�ȭ ----------------------------------------
dofile( ".\\LuaFunc\\MonsterSkillFunc.lua" )
--------------------------------------------------
--[[

AddMonster( "MI_LUCIFER01" ) -- �߰��� ���� ID �ݵ�� ""�ȿ� ��� �Ѵ�.
--{
	-- ����� ��ų
	-- AddSkill( ����Method, "��ųID", ��ų����, n��°Ÿ��, ��ų����, �ߵ�Ȯ��, ��ų���ӽð�(s), ��ų��üȮ������ )
	AddSkill( ATK_MELEE, "SI_MAG_ELECTRICITY_LIGHTINGPARM",	1, 1, 10, 80, 10, true )
	AddSkill( ATK_RANGE, "SI_MAG_EARTH_ROCKCRASH",		1, 1, 10, 20, 10, true )
--}
������ 0�� ��� Ÿ�ٸ� ����
Ȯ���� ������ ��� ��ų �ߵ� �ȵ�
--]]


AddMonster( "MI_LUCIFER01" )
--{
	AddSkill( ATK_MELEE, "SI_BIL_PST_ASALRAALAIKUM", 	2, 1, 0, 30, 0, true )
	AddSkill( ATK_MELEE, "SI_ELE_EARTH_EARTHQUAKE",		5, 1, 0, 30, 0, true )
	AddSkill( ATK_MELEE, "SI_ELE_ELECTRICITY_ELETRICSHOCK", 10, 1, 0, 15, 0, true )
	AddSkill( ATK_MELEE, "SI_MAG_EARTH_SPIKESTONE",		1, 1, 0, 5, 20, true ) -- ����

	AddSkill( ATK_RANGE, "SI_ELE_WATER_POISONCLOUD",	10, 1, 12, 4, 30, true ) -- ������ ƽ��100,�⺻ ��ų ������
	AddSkill( ATK_RANGE, "SI_PSY_PSY_PSYCHICBOMB",		10, 1, 5, 5, 5, true ) -- ����
	AddSkill( ATK_RANGE, "SI_MAG_EARTH_SPIKESTONE",		1, 1, 20, 15, 10, true ) -- ����
	AddSkill( ATK_RANGE, "SI_ACR_YOYO_DEADLYSWING", 	20, 1, 30, 10, 20, true ) -- ���� ƽ��120
	AddSkill( ATK_RANGE, "SI_ACR_YOYO_PULLING",		20, 1, 30, 20, 0, true ) --Ǯ��
	AddSkill( ATK_RANGE, "SI_JST_HERO_SILENCE", 		1, 1, 30, 23, 6, true )  -- ħ��
	AddSkill( ATK_RANGE, "SI_MAG_EARTH_LOOTING", 		20, 1, 30, 23, 10, true ) -- ����
--}

AddMonster( "MI_ASURA04" )
--{
	AddSkill( ATK_MELEE, "SI_MAG_EARTH_SPIKESTONE",		1, 1, 20, 15, 10, true )
--}



AddMonster( "MI_VEMPAIN01" )
--{
	AddSkill( ATK_MELEE, "SI_BIL_HERO_DISENCHANT", 		5, 1, 0, 10, 0, true )
	AddSkill( ATK_MELEE, "SI_ELE_EARTH_EARTHQUAKE",		5, 1, 0, 30, 0, true )
	AddSkill( ATK_MELEE, "SI_ELE_ELECTRICITY_ELETRICSHOCK", 10, 1, 0, 15, 0, true )
	AddSkill( ATK_MELEE, "SI_MAG_EARTH_SPIKESTONE",		1, 1, 0, 5, 20, true ) -- ����

	AddSkill( ATK_RANGE, "SI_ELE_WATER_POISONCLOUD",	10, 1, 12, 4, 30, true ) -- ������ ƽ��100,�⺻ ��ų ������
	AddSkill( ATK_RANGE, "SI_PSY_PSY_PSYCHICBOMB",		10, 1, 5, 5, 5, true ) -- ����
	AddSkill( ATK_RANGE, "SI_MAG_EARTH_SPIKESTONE",		1, 1, 20, 15, 10, true ) -- ����
	AddSkill( ATK_RANGE, "SI_ACR_YOYO_DEADLYSWING", 	20, 1, 30, 10, 20, true ) -- ���� ƽ��120
	AddSkill( ATK_RANGE, "SI_ACR_YOYO_PULLING",		20, 1, 30, 20, 0, true ) --Ǯ��
	AddSkill( ATK_RANGE, "SI_JST_HERO_SILENCE", 		1, 1, 30, 23, 6, true )  -- ħ��
	AddSkill( ATK_RANGE, "SI_MAG_EARTH_LOOTING", 		20, 1, 30, 23, 10, true ) -- ����
--}

AddMonster( "MI_VEMPAIN01_1" )
--{
	AddSkill( ATK_MELEE, "SI_BIL_HERO_DISENCHANT", 		5, 1, 0, 15, 0, true )
	AddSkill( ATK_MELEE, "SI_ELE_EARTH_EARTHQUAKE",		5, 1, 0, 30, 0, true )
	AddSkill( ATK_MELEE, "SI_ELE_ELECTRICITY_ELETRICSHOCK", 10, 1, 0, 15, 0, true )
	AddSkill( ATK_MELEE, "SI_MAG_EARTH_SPIKESTONE",		1, 1, 0, 5, 20, true ) -- ����

	AddSkill( ATK_RANGE, "SI_ELE_WATER_POISONCLOUD",	10, 1, 12, 4, 30, true ) -- ������ ƽ��100,�⺻ ��ų ������
	AddSkill( ATK_RANGE, "SI_PSY_PSY_PSYCHICBOMB",		5, 1, 5, 5, 5, true ) -- ����
	AddSkill( ATK_RANGE, "SI_MAG_EARTH_SPIKESTONE",		1, 1, 20, 15, 10, true ) -- ����
	AddSkill( ATK_RANGE, "SI_ACR_YOYO_DEADLYSWING", 	20, 1, 30, 10, 20, true ) -- ���� ƽ��120
	AddSkill( ATK_RANGE, "SI_ACR_YOYO_PULLING",		20, 1, 30, 20, 0, true ) --Ǯ��
	AddSkill( ATK_RANGE, "SI_JST_HERO_SILENCE", 		1, 1, 30, 23, 6, true )  -- ħ��
	AddSkill( ATK_RANGE, "SI_MAG_EARTH_LOOTING", 		20, 1, 30, 23, 10, true ) -- ����
--}

AddMonster( "MI_SKELGENERAL" )
--{
	AddSkill( ATK_MELEE, "SI_BIL_HERO_DISENCHANT", 		5, 1, 0, 10, 0, true )

	AddSkill( ATK_RANGE, "SI_MAG_EARTH_SPIKESTONE",		20, 1, 15, 100, 7, true ) -- ������ũ����
--}

AddMonster( "MI_SKELDEVIL" )
--{
	AddSkill( ATK_MELEE, "SI_BIL_HERO_DISENCHANT", 		5, 1, 0, 10, 0, true )

	AddSkill( ATK_RANGE, "SI_MAG_EARTH_SPIKESTONE",		20, 1, 15, 57, 7, true ) -- ������ũ����
	AddSkill( ATK_RANGE, "SI_ACR_YOYO_PULLING",		20, 1, 30, 20, 0, true ) --Ǯ��
	AddSkill( ATK_RANGE, "SI_JST_HERO_SILENCE", 		1, 1, 30, 23, 6, true )  -- ħ��
--}