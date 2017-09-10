tOccupationShopItem = {}
-------------------------------------------------------------
--- ���� ���� �ð� üũ �Լ� --------------------------------
-------------------------------------------------------------
function CheckChangeTime( strDate )	-- strDate �� ���� �ֱٿ� ������ ����� �ð�
	local strNowDate = os.date("%a %H:%M")
	local nNowDate = tonumber( GetNowDate() )	-- ���� ��¥ �� �ð� 
	local nNextDate = tonumber( GetNextDate( strDate ) ) -- ���� ����� ��¥ �� �ð� 
			
	if( ChangeTaxTime == strNowDate ) then	-- ChangeTaxTime�� ������ �ð��̸� ����
		return true, tostring( nNowDate )
	elseif( nNowDate >= nNextDate  ) then	-- ���� ������ �ð��� �������� ����
		return true, tostring( nNowDate )	-- (ChangeTaxTime�� ���ְ� ���� �������� ���� ���)
	end
	
	return false, tostring( nNowDate )
end

function CheckPayTime()	-- ���� ���޽ð�
	if( PayTime == os.date("%H:%M") ) then
		return true
	end
	return false
end

function GetNowDate()
	return os.date("%Y%m%d%H%M")
end

----------------------------------------------------------------
-- ������ʹ� lua������ ����ϴ� �Լ� ------------------
----------------------------------------------------------------
function GetNextDate( strDate )
	local nYear = tonumber( string.sub( strDate, 1, 4 ) )
	local nMonth = tonumber( string.sub( strDate, 5, 6 ) )
	local nDay = tonumber( string.sub( strDate, 7, 8 ) )
	
	local nWDay, nHour, nMin = GetCheckTime()
	
	-- ���� ����� ��¥�� ���Ѵ�.(strDate�� ���� ���� ���� �ð�)
	local temp = os.date( "*t", os.time{year=nYear, month=nMonth, day=nDay, hour=nHour, min=nMin} )
	if( temp.wday >= nWDay ) then
		temp.day = temp.day + ( 7 - ( temp.wday - nWDay ) )
	elseif( temp.wday < nWDay ) then
		temp.day = temp.day + ( nWDay - temp.wday )
	end	

	return os.date( "%Y%m%d%H%M", os.time(temp) )
end

function GetCheckTime()
	local strWDay = string.sub( ChangeTaxTime, 1, 3 )
	local nHour = string.sub( ChangeTaxTime, 5, 6 )
	local nMin = string.sub( ChangeTaxTime, 8, 9 )
	local nWDay = 2
	
	if( strWDay == "Sun" ) then nWDay = 1
	elseif( strWDay == "Mon" ) then nWDay = 2
	elseif( strWDay == "Tue" ) then nWDay = 3
	elseif( strWDay == "Wed" ) then nWDay = 4
	elseif( strWDay == "Thu" ) then nWDay = 5
	elseif( strWDay == "Fri" ) then nWDay = 6
	elseif( strWDay == "Sat" ) then nWDay = 7
	end
	
	return nWDay, nHour, nMin
end


function AddOccupationShopItem( strItemId )
	local nIndex = #( tOccupationShopItem ) + 1
	
	tOccupationShopItem[nIndex] = strItemId
end
