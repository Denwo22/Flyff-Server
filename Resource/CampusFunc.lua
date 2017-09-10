function SEC( a ) return a * 1000 end
function MIN( a ) return SEC( a ) * 60 end
--------------------------------------------------------------------------

tCampusQuest = {}
tCampusBuff = {}
tCampusReward = {}


function SetQuest( strQuestId )
	local nIndex = #( tCampusQuest ) + 1
	tCampusQuest[nIndex] = {}
	tCampusQuest[nIndex].strQuestId = strQuestId
end


function SetBuffLevel( nLevel, strBuffId )
	local nIndex = #( tCampusBuff ) + 1
	tCampusBuff[nIndex] = {}
	tCampusBuff[nIndex].nLevel = nLevel
	tCampusBuff[nIndex].strBuffId = strBuffId
end


function SetCampusReward( nLevel, nMasterReward, nPupilReward )
	local nIndex = #( tCampusReward ) + 1
	tCampusReward[nIndex] = {}
	tCampusReward[nIndex].nLevel = nLevel
	tCampusReward[nIndex].nMasterReward = nMasterReward
	tCampusReward[nIndex].nPupilReward = nPupilReward
end
