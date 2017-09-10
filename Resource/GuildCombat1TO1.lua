-----------------------------------------------------------------------------------------
-- Begin Script -------------------------------------------------------------------------
-----------------------------------------------------------------------------------------
-- ��� ��ũ��Ʈ�� "//" ��� "--"�� ����Ͽ� �ּ�ó�� �Ѵ�.
-- �� �ּ��� "/*, */"��� "--[[, --]]"�� ����Ѵ�.
-----------------------------------------------------------------------------------------

-- �ּ� ���� PENYA ����
MinJoinPenya = 			110000000

-- �ּ� ���� ��� ���� ����
MinGuildLevel =			20

-- �ּ� ���� ��� ����
MinJoinGuild =			2

-- �ִ� ������ �Ҽ� �ִ� ��� ����
MaxJoinGuild =			8

-- �ּ� ���� �ο�
MinJoinPlayer = 		6

-- �ִ� ���� �ο�
MaxJoinPlayer =			11

-- �ּ� ���� ����
MinJoinPlayerLevel =	30

-- ������ �����ڵ��� �ִ� �����
PlayerLife =			1

-- ���� ��� �� �����ִ� ��� %
CancelReturnRate =		80

-- ���� ���� �� �����ִ� ��� %
FailReturnRate =		98

-- ������ �ۼ� �ð�(msec)
MemberLineUpTime =		3600000

-- ������ð� (msec)
EntranceWaitTime = 		600000

-- ���� ������ ���ð�(msec)
WarWaitTime = 			60000

-- ���� �ð�(msec)
WarTime =			600000

-- ���� ���� �� ��� �ð�(msec)
WarCloseWaitTime = 	30000

-- ���� �¸��� ��� Ĩ ���� ����
WinChipNum = MinJoinPenya * 0.9 * 0.00001 / MaxJoinPlayer

OPENTIME = {} -- �ʱ�ȭ(���� ����)
-- �ڵ� ���� �ð� ����( ���� ������, index�� 1���� ����, �ð��� 2�ڸ���[�� - 07:08] )
-- ���� ���� - Sun, Mon, Tue, Wed, Thu, Fri, Sat : ��ҹ��� ������, ���� ���� ������ index 1�� ����.
OPENTIME[1] = "Sat 17:00"





------------------------------------------------------------------------------
-- Function ------------------------------------------------------------------
------------------------------------------------------------------------------

-- __DBSERVER
function CheckOpenTime()
	local strNowTime = os.date("%a %H:%M")
	
	for i in pairs(OPENTIME) do
		if( OPENTIME[i] == strNowTime ) then
			return 1
		end
	end
	return 0
end

-- ���� �ð� __WORLDSERVER
function GetRemainNextTime()
	if( #(OPENTIME) == 0 ) then 
		return 0
	end
	
	local nNowDay = tonumber( os.date("%w") )
	local nNowHour = tonumber( os.date("%H") )
	local nNowMin = tonumber( os.date("%M") )
	local nNowSec = tonumber( os.date("%S") )
			
	for i in pairs(OPENTIME) do
		local a, b, c = GetWeekDayStrToNum( OPENTIME[i] )
		if( a > nNowDay ) then
			return ((a-nNowDay)*24*3600)+((b-nNowHour)*3600)+((c-nNowMin)*60)-nNowSec
		elseif( (a == nNowDay) and (b > nNowHour) ) then
			return ((b-nNowHour)*3600)+((c-nNowMin)*60)-nNowSec
		elseif( (a == nNowDay) and (b == nNowHour) and (c > nNowMin) ) then 
			return ((c-nNowMin)*60)-nNowSec
		elseif( (a == nNowDay) and (b == nNowHour) and (c == nNowMin) ) then 
			return nNowSec
		end
	end
	
	-- ������ ������...
	local a, b, c = GetWeekDayStrToNum( OPENTIME[1] )
	
	return ((7-nNowDay+a)*24*3600)+((b-nNowHour)*3600)+((c-nNowMin)*60)-nNowSec
end

-- ������ ��ġȭ
function GetWeekDayStrToNum( strTime )
	local a = 0
	local nTemp1, nTemp2 = string.find(strTime, "%d%d:")
	local b = tonumber( string.sub(strTime, nTemp1, nTemp2-1) )
	nTemp1, nTemp2 = string.find(strTime, ":%d%d")
	local c = tonumber( string.sub(strTime, nTemp1+1, nTemp2) )

	local strDay = string.sub(strTime, string.find(strTime, "%a+"))
	if( strDay == "Sun" ) then a=0
	elseif( strDay == "Mon" ) then a=1
	elseif( strDay == "Tue" ) then a=2
	elseif( strDay == "Wed" ) then a=3
	elseif( strDay == "Thu" ) then a=4
	elseif( strDay == "Fri" ) then a=5
	elseif( strDay == "Sat" ) then a=6
	end
	
	return a, b, c
end
