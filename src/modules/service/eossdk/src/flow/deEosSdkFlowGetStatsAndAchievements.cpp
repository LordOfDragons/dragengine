/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "deEosSdkFlowGetStatsAndAchievements.h"
#include "../deEosSdkServiceEos.h"
#include "../deEosSdk.h"
#include "../convert/deECCommon.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/service/deServiceManager.h>


// Class deEosSdkFlowGetStatsAndAchievements
//////////////////////////////////////////////

static void fQueryStatsCallback(const EOS_Stats_OnQueryStatsCompleteCallbackInfo *data){
	((deEosSdkFlowGetStatsAndAchievements*)data->ClientData)->OnQueryStatsCompleted(*data);
}

static void fQueryPlayerAchievementsCallback(
const EOS_Achievements_OnQueryPlayerAchievementsCompleteCallbackInfo *data){
	((deEosSdkFlowGetStatsAndAchievements*)data->ClientData)->
		OnQueryPlayerAchievementsCompleted(*data);
}



// Constructor, destructor
////////////////////////////

deEosSdkFlowGetStatsAndAchievements::deEosSdkFlowGetStatsAndAchievements(deEosSdkServiceEos &service,
	const decUniqueID &id, const deServiceObject &request) :
deEosSdkFlow(service, id),
pStatNames(nullptr),
pAchievementNames(nullptr),
pStatsReceived(false),
pAchievementsReceived(false),
pResultData(deServiceObject::Ref::NewWith())
{
	deServiceObject::Ref so;
	
	so = request.GetChildAt("stats");
	if(so){
		pStats = deECCommon::StringList(so);
	}
	
	so = request.GetChildAt("achievements");
	if(so){
		pAchievements = deECCommon::StringList(so);
	}
	
	service.NewPendingRequest(id, "getStatsAndAchievements", pResultData);
	
	try{
		if(! service.productUserId){
			DETHROW_INFO(deeInvalidAction, "No user logged in");
		}
		QueryStats();
		
	}catch(const deException &e){
		Fail(e);
		Finish();
		return;
	}
	
	try{
		QueryPlayerAchievements();
		
	}catch(const deException &e){
		Fail(e);
	}
	CheckFinished();
}

deEosSdkFlowGetStatsAndAchievements::~deEosSdkFlowGetStatsAndAchievements(){
	if(pAchievementNames){
		delete [] pAchievementNames;
	}
	if(pStatNames){
		delete [] pStatNames;
	}
}



// Management
///////////////

void deEosSdkFlowGetStatsAndAchievements::QueryStats(){
	const int count = pStats.GetCount();
	if(count == 0){
		pResultData->SetChildAt("stats", deServiceObject::Ref::NewWith());
		pStatsReceived = true;
		return;
	}
	
	int i;
	pStatNames = new const char*[count];
	for(i=0; i<count; i++){
		pStatNames[i] = pStats.GetAt(i).GetString();
	}
	
	EOS_Stats_QueryStatsOptions options = {};
	options.ApiVersion = EOS_STATS_QUERYSTATS_API_LATEST;
	options.LocalUserId = pService.productUserId;
	options.StartTime = EOS_STATS_TIME_UNDEFINED;
	options.EndTime = EOS_STATS_TIME_UNDEFINED;
	options.TargetUserId = pService.productUserId;
	options.StatNames = pStatNames;
	options.StatNamesCount = count;
	
	GetModule().LogInfo("deEosSdkFlowGetStatsAndAchievements.QueryStats");
	EOS_Stats_QueryStats(pService.GetHandleStats(), &options, this, fQueryStatsCallback);
}

void deEosSdkFlowGetStatsAndAchievements::QueryPlayerAchievements(){
	const int count = pAchievements.GetCount();
	if(count == 0){
		pResultData->SetChildAt("achievements", deServiceObject::Ref::NewWith());
		pAchievementsReceived = true;
		return;
	}
	
	int i;
	pAchievementNames = new const char*[count];
	for(i=0; i<count; i++){
		pAchievementNames[i] = pAchievements.GetAt(i).GetString();
	}
	
	EOS_Achievements_QueryPlayerAchievementsOptions options = {};
	options.ApiVersion = EOS_ACHIEVEMENTS_QUERYPLAYERACHIEVEMENTS_API_LATEST;
	options.LocalUserId = pService.productUserId;
	options.TargetUserId = pService.productUserId;
	
	GetModule().LogInfo("deEosSdkFlowGetStatsAndAchievements.QueryPlayerAchievements");
	EOS_Achievements_QueryPlayerAchievements(pService.GetHandleAchievements(), &options,
		this, fQueryPlayerAchievementsCallback);
}



void deEosSdkFlowGetStatsAndAchievements::OnQueryStatsCompleted(
const EOS_Stats_OnQueryStatsCompleteCallbackInfo &data){
	GetModule().LogInfoFormat(
		"deEosSdkFlowGetStatsAndAchievements.OnQueryStatsCompleted: res=%d",
		(int)data.ResultCode);
	pStatsReceived = true;
	
	if(data.ResultCode != EOS_EResult::EOS_Success){
		Fail(data.ResultCode);
		CheckFinished();
		return;
	}
	
	EOS_Stats_Stat *stat = nullptr;
	
	try{
		DEASSERT_NOTNULL(pService.productUserId)
		
		EOS_Stats_CopyStatByNameOptions options = {};
		options.ApiVersion = EOS_STATS_COPYSTATBYNAME_API_LATEST;
		options.TargetUserId = pService.productUserId;
		
		const int count = pStats.GetCount();
		EOS_EResult result;
		int i;
		
		const deServiceObject::Ref so(deServiceObject::Ref::NewWith());
		
		for(i=0; i<count; i++){
			const char * const name = pStatNames[i];
			options.Name = name;
			result = EOS_Stats_CopyStatByName(pService.GetHandleStats(), &options, &stat);
			
			switch(result){
			case EOS_EResult::EOS_Success:
				so->SetIntChildAt(name, (int)stat->Value);
				EOS_Stats_Stat_Release(stat);
				stat = nullptr;
				break;
				
			case EOS_EResult::EOS_NotFound:
				// either the stat really does not exist or it has not been set yet.
				// we assume the stat has not been set yet so we set it to nullptr.
				so->SetChildAt(name, nullptr);
				break;
				
			default:
				Fail(result);
				CheckFinished();
				return;
			}
		}
		
		pResultData->SetChildAt("stats", so);
		CheckFinished();
		
	}catch(const deException &e){
		if(stat){
			EOS_Stats_Stat_Release(stat);
		}
		Fail(e);
		CheckFinished();
	}
}

void deEosSdkFlowGetStatsAndAchievements::OnQueryPlayerAchievementsCompleted(
const EOS_Achievements_OnQueryPlayerAchievementsCompleteCallbackInfo &data){
	GetModule().LogInfoFormat(
		"deEosSdkFlowGetStatsAndAchievements.OnQueryPlayerAchievementsCompleted: res=%d",
		(int)data.ResultCode);
	pAchievementsReceived = true;
	
	if(data.ResultCode != EOS_EResult::EOS_Success){
		Fail(data.ResultCode);
		CheckFinished();
		return;
	}
	
	EOS_Achievements_PlayerAchievement *achievement = nullptr;
	
	try{
		DEASSERT_NOTNULL(pService.productUserId)
		
		EOS_Achievements_CopyPlayerAchievementByAchievementIdOptions options = {};
		options.ApiVersion = EOS_ACHIEVEMENTS_COPYPLAYERACHIEVEMENTBYACHIEVEMENTID_API_LATEST;
		options.LocalUserId = pService.productUserId;
		options.TargetUserId = pService.productUserId;
		
		const int count = pAchievements.GetCount();
		EOS_EResult result;
		int i;
		
		const deServiceObject::Ref so(deServiceObject::Ref::NewWith());
		
		for(i=0; i<count; i++){
			const char * const name = pAchievementNames[i];
			options.AchievementId = name;
			result = EOS_Achievements_CopyPlayerAchievementByAchievementId(
				pService.GetHandleAchievements(), &options, &achievement);
			
			switch(result){
			case EOS_EResult::EOS_Success:
				so->SetBoolChildAt(name, (int)achievement->UnlockTime
					!= EOS_ACHIEVEMENTS_ACHIEVEMENT_UNLOCKTIME_UNDEFINED);
				
				EOS_Achievements_PlayerAchievement_Release(achievement);
				achievement = nullptr;
				break;
				
			case EOS_EResult::EOS_NotFound:
				// either the achievement really does not exist or it has not been set yet.
				// we assume the achievement has not been set yet so we use the value false
				so->SetBoolChildAt(name, false);
				break;
				
			default:
				Fail(result);
				CheckFinished();
				return;
			}
		}
		
		pResultData->SetChildAt("achievements", so);
		CheckFinished();
		
	}catch(const deException &e){
		if(achievement){
			EOS_Achievements_PlayerAchievement_Release(achievement);
		}
		Fail(e);
		CheckFinished();
	}
}



void deEosSdkFlowGetStatsAndAchievements::CheckFinished(){
	if(pStatsReceived && pAchievementsReceived){
		Finish();
	}
}
