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

#include "deEosSdkFlowSetStatsAndAchievements.h"
#include "../deEosSdkServiceEos.h"
#include "../deEosSdk.h"
#include "../convert/deECCommon.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/collection/decTList.h>
#include <dragengine/resources/service/deServiceManager.h>


// Class deEosSdkFlowSetStatsAndAchievements
//////////////////////////////////////////////

static void fIngestStatCallback(const EOS_Stats_IngestStatCompleteCallbackInfo *data){
	((deEosSdkFlowSetStatsAndAchievements*)data->ClientData)->OnIngestStatCompleted(*data);
}

static void fUnlockAchievementsCallback(
const EOS_Achievements_OnUnlockAchievementsCompleteCallbackInfo *data){
	((deEosSdkFlowSetStatsAndAchievements*)data->ClientData)->
		OnUnlockAchievementsCompleted(*data);
}



// Constructor, destructor
////////////////////////////

deEosSdkFlowSetStatsAndAchievements::deEosSdkFlowSetStatsAndAchievements(deEosSdkServiceEos &service,
	const decUniqueID &id, const deServiceObject &request) :
deEosSdkFlow(service, id),
pStatsCompleted(false),
pAchievementsCompleted(false),
pResultData(deServiceObject::Ref::New())
{
	service.NewPendingRequest(id, "setStatsAndAchievements", pResultData);
	
	try{
		if(!service.productUserId){
			DETHROW_INFO(deeInvalidAction, "No user logged in");
		}
		IngestStat(request);
		
	}catch(const deException &e){
		Fail(e);
		Finish();
		return;
	}
	
	try{
		UnlockAchievements(request);
		
	}catch(const deException &e){
		Fail(e);
	}
	
	CheckFinished();
}



// Management
///////////////

void deEosSdkFlowSetStatsAndAchievements::IngestStat(const deServiceObject &request){
	const deServiceObject::Ref soResp(deServiceObject::Ref::New());
	pResultData->SetChildAt("stats", soResp);
	
	const deServiceObject::Ref soIn(request.GetChildAt("stats"));
	if(!soIn){
		pStatsCompleted = true;
		return;
	}
	
	const decStringList keys(soIn->GetChildrenKeys());
	const int count = keys.GetCount();
	if(count == 0){
		pStatsCompleted = true;
		return;
	}
	
	decTList<EOS_Stats_IngestData> ingestData(count, EOS_Stats_IngestData{});
	int i;
	
	
	for(i=0; i<count; i++){
		const decString &apiName = keys.GetAt(i);
		const deServiceObject::Ref soValue(soIn->GetChildAt(apiName));
		
		if(soValue->IsInteger()){
			const int value = soValue->GetInteger();
			soResp->SetIntChildAt(apiName, value);
			
			ingestData[i].ApiVersion = EOS_STATS_INGESTDATA_API_LATEST;
			ingestData[i].StatName = apiName.GetString();
			ingestData[i].IngestAmount = (int32_t)value;
			
		}else{
			DETHROW_INFO(deeInvalidParam, apiName);
		}
	}
	
	EOS_Stats_IngestStatOptions options = {};
	options.ApiVersion = EOS_STATS_INGESTSTAT_API_LATEST;
	options.LocalUserId = pService.productUserId;
	options.TargetUserId = pService.productUserId;
	options.Stats = ingestData.GetArrayPointer();
	options.StatsCount = count;
	
	GetModule().LogInfo("deEosSdkFlowSetStatsAndAchievements.IngestStat");
	EOS_Stats_IngestStat(pService.GetHandleStats(), &options, this, fIngestStatCallback);
}

void deEosSdkFlowSetStatsAndAchievements::UnlockAchievements(const deServiceObject &request){
	const deServiceObject::Ref soResp(deServiceObject::Ref::New());
	pResultData->SetChildAt("achievements", soResp);
	
	const deServiceObject::Ref soIn(request.GetChildAt("achievements"));
	if(!soIn){
		pAchievementsCompleted = true;
		return;
	}
	
	const decStringList keys(soIn->GetChildrenKeys());
	const int count = keys.GetCount();
	if(count == 0){
		pAchievementsCompleted = true;
		return;
	}
	
	decTList<const char*> ids;
	ids.SetCountDiscard(count);
	
	int i;
	for(i=0; i<count; i++){
		const decString &name = keys.GetAt(i);
		const bool achieved = soIn->GetChildAt(name)->GetBoolean();
		
		if(achieved){
			soResp->SetBoolChildAt(name, true);
			ids[i] = name.GetString();
			
		}else{
			DETHROW_INFO(deeInvalidParam, "Reverting unlocking achievements is not supported");
		}
	}
	
	EOS_Achievements_UnlockAchievementsOptions options = {};
	options.ApiVersion = EOS_ACHIEVEMENTS_UNLOCKACHIEVEMENTS_API_LATEST;
	options.UserId = pService.productUserId;
	options.AchievementIds = ids.GetArrayPointer();
	options.AchievementsCount = count;
	
	GetModule().LogInfo("deEosSdkFlowSetStatsAndAchievements.UnlockAchievements");
	EOS_Achievements_UnlockAchievements(pService.GetHandleAchievements(), &options,
		this, fUnlockAchievementsCallback);
}



void deEosSdkFlowSetStatsAndAchievements::OnIngestStatCompleted(
const EOS_Stats_IngestStatCompleteCallbackInfo &data){
	GetModule().LogInfoFormat(
		"deEosSdkFlowSetStatsAndAchievements.OnIngestStatCompleted: res=%d",
		(int)data.ResultCode);
	pStatsCompleted = true;
	
	if(data.ResultCode != EOS_EResult::EOS_Success){
		Fail(data.ResultCode);
	}
	
	CheckFinished();
}

void deEosSdkFlowSetStatsAndAchievements::OnUnlockAchievementsCompleted(
const EOS_Achievements_OnUnlockAchievementsCompleteCallbackInfo &data){
	GetModule().LogInfoFormat(
		"deEosSdkFlowSetStatsAndAchievements.OnUnlockAchievementsCompleted: res=%d",
		(int)data.ResultCode);
	pAchievementsCompleted = true;
	
	if(data.ResultCode != EOS_EResult::EOS_Success){
		Fail(data.ResultCode);
	}
	
	CheckFinished();
}



void deEosSdkFlowSetStatsAndAchievements::CheckFinished(){
	if(pStatsCompleted && pAchievementsCompleted){
		Finish();
	}
}
