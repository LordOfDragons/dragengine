#include "deMTGetStatsAndAchievements.h"
#include "../deMsgdkServiceMsgdk.h"
#include "../deMicrosoftGdk.h"
#include "../deMsgdkGameConfig.h"
#include "../convert/deMCCommon.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/service/deServiceManager.h>


// Class deMTGetStatsAndAchievements
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

deMTGetStatsAndAchievements::deMTGetStatsAndAchievements(deMsgdkServiceMsgdk &service,
	const decUniqueID &id, const deServiceObject &request) :
deMsgdkAsyncTask(service.GetInvalidator()),
pService(service),
pRequestId(id),
pStatNames(nullptr),
pResultData(deServiceObject::Ref::NewWith(),
pWaitAchievementsSynced(false)
{
	deServiceObject::Ref so;
	
	so = request.GetChildAt("stats");
	if(so){
		pStats = deMCCommon::StringList(so);
	}
	
	so = request.GetChildAt("achievements");
	if(so){
		pAchievements = deMCCommon::StringList(so);
	}
	
	pService.GetModule().LogInfo("deMTGetStatsAndAchievements: Get stats and achievements");

	pService.NewPendingRequest(pRequestId, "getStatsAndAchievements", pResultData);
	try
	{
		if(!service.GetUser() || !service.GetXblContext())
		{
			DETHROW_INFO(deeInvalidAction, "No user logged in");
		}

		if(!service.GetAchievementsSynced())
		{
			pWaitAchievementsSynced = true;
			service.AddFrameUpdateTask(this);
			return;
		}

		pGetAchievements();
		pGetStats();
	}
	catch(const deException &e)
	{
		pService.FailRequest(pRequestId, e);
		FreeReference();
	}
}

deMTGetStatsAndAchievements::~deMTGetStatsAndAchievements()
{
}


// Management
///////////////

void deMTGetStatsAndAchievements::OnFrameUpdate(float elapsed)
{
	if(!pWaitAchievementsSynced || !pService.GetAchievementsSynced())
	{
		return;
	}

	pWaitAchievementsSynced = false;
	pService.RemoveFrameUpdateTask(this);

	try
	{
		if(!pService.GetUser() || !pService.GetXblContext())
		{
			DETHROW_INFO(deeInvalidAction, "No user logged in");
		}

		pGetAchievements();
		pGetStats();
	}
	catch(const deException &e)
	{
		pService.FailRequest(pRequestId, e);
		FreeReference();
	}
}


// Protect Functions
//////////////////////

void deMTGetStatsAndAchievements::pGetAchievements()
{
	if(pAchievements.GetCount() == 0)
	{
		pResultData->SetChildAt("achievements", deServiceObject::Ref::NewWith());
		return;
	}
	
	XblAchievementsManagerResultHandle achievementResult;
	pService.AssertResult(XblAchievementsManagerGetAchievements(
		pService.GetUserId(), XblAchievementOrderBy::DefaultOrder,
		XblAchievementsManagerSortOrder::Unsorted, &achievementResult),
		"deMTGetStatsAndAchievements.XblAchievementsManagerGetAchievements");

	try
	{
		const XblAchievement *achievements;
		uint64_t i, achievementsCount;

		pService.AssertResult(XblAchievementsManagerResultGetAchievements(
			achievementResult, &achievements, &achievementsCount),
			"deMTGetStatsAndAchievements.pGetAchievements.XblAchievementsManagerResultGetAchievements");

		const deServiceObject::Ref so( deServiceObject::Ref::New( new deServiceObject ) );
		
		for(i=0; i<achievementsCount; i++){
			const char * const name = achievements[i].id;
			if(pAchievements.Has(name))
			{
				so->SetBoolChildAt(name, achievements[i].progressState
					== XblAchievementProgressState::Achieved);
			}
		}
		
		pResultData->SetChildAt("achievements", so);
	}
	catch(const deException &)
	{
		XblAchievementsManagerResultCloseHandle(achievementResult);
		throw;
	}
}

void deMTGetStatsAndAchievements::pGetStats()
{
	const int count = pStats.GetCount();
	if(count == 0)
	{
		const deMsgdkPendingRequest::Ref pr(pService.GetPendingRequestWithId(pRequestId));
		if(pr)
		{
			pResultData->SetChildAt("stats", deServiceObject::Ref::NewWith());
			pService.GetModule().GetGameEngine()->GetServiceManager()->
				QueueRequestResponse(pService.GetService(), pr->id, pr->data, true);
		}
		FreeReference();
		return;
	}
	
	int i;
	pStatNames = new const char*[count];
	for( i=0; i<count; i++ ){
		pStatNames[i] = pStats.GetAt(i).GetString();
	}

	pService.AssertResult(XblUserStatisticsGetSingleUserStatisticsAsync(
		pService.GetXblContext(), pService.GetUserId(),
		pService.GetModule().GetGameConfig().scid,
		pStatNames, (size_t)count, GetAsyncBlockPtr()),
		"deMTGetStatsAndAchievements.pGetStarts.XblUserStatisticsGetSingleUserStatisticsAsync");
}

void deMTGetStatsAndAchievements::OnFinished()
{
	const deMsgdkPendingRequest::Ref pr(pService.GetPendingRequestWithId(pRequestId));
	if(!pr)
	{
		return;
	}
	
	size_t bufferSize;
	HRESULT result = XblUserStatisticsGetSingleUserStatisticsResultSize(GetAsyncBlockPtr(), &bufferSize);
	if(FAILED(result))
	{
		pService.FailRequest(pr, result);
		return;
	}

	const int statCount = pStats.GetCount();
	bool *found = nullptr;
	uint8_t *buffer = new uint8_t[bufferSize];

	try
	{
		XblUserStatisticsResult *usr;
		size_t bufferUsed;
		HRESULT result = XblUserStatisticsGetSingleUserStatisticsResult(GetAsyncBlockPtr(),
			bufferSize, buffer, &usr, &bufferUsed);
		if(FAILED(result))
		{
			delete [] buffer;
			buffer = nullptr;
			pService.FailRequest(pr, result);
			return;
		}
		
		const deServiceObject::Ref so(deServiceObject::Ref::NewWith());
		const decString &scid = pService.GetModule().GetGameConfig().scid;
		uint32_t i, j;

		found = new bool[statCount];
		for(i=0; i<(uint32_t)statCount; i++)
		{
			found[i] = false;
		}

		for(i=0; i<usr->serviceConfigStatisticsCount; i++)
		{
			const XblServiceConfigurationStatistic &scs = usr->serviceConfigStatistics[i];
			if(scid != scs.serviceConfigurationId)
			{
				continue;
			}

			for(j=0; j<scs.statisticsCount; j++)
			{
				const char * const name = scs.statistics[j].statisticName;
				const int statIndex = pStats.IndexOf(name);

				if(statIndex == -1)
				{
					continue;
				}

				// statisticType values (undocumented, from observations):
				// - Double: for "Integer" format stats. Most probably also for "Decimal"
				//           and "Percentage" format stats. "Short timespan" and "Long timespan"
				//           are most probably also covered but this is unknown.
				// - String: for "String" format stats.
				//pService.GetModule().LogInfoFormat("DEBUG: '%s' -> '%s'",
				//	scs.statistics[j].statisticType, scs.statistics[j].value);
				const char * const type = scs.statistics[j].statisticType;
				if(strcmp(type, "Double") == 0)
				{
					so->SetFloatChildAt(name, decString(scs.statistics[j].value).ToFloatValid());
				}
				else if(strcmp(type, "String") == 0)
				{
					so->SetStringChildAt(name, scs.statistics[j].value);
				}
				else
				{
					decString message;
					message.Format("Invalid value for stat '%s': type='%s'", name, type);
					DETHROW_INFO(deeInvalidParam, message);
				}

				found[statIndex] = true;
			}
		}

		delete [] buffer;
		buffer = nullptr;

		for(i=0; i<(uint32_t)statCount; i++)
		{
			if(!found[i])
			{
				so->SetChildAt(pStats.GetAt(i), nullptr);
			}
		}

		delete [] found;
		found = nullptr;

		pResultData->SetChildAt("stats", so);

		pService.GetModule().LogInfo("deMTGetStatsAndAchievements.OnFinished: Stats received");

		pService.GetModule().GetGameEngine()->GetServiceManager()->
			QueueRequestResponse(pService.GetService(), pr->id, pr->data, true);
	}
	catch(const deException &e)
	{
		if(found)
		{
			delete [] found;
		}
		if(buffer)
		{
			delete [] buffer;
		}
		pService.FailRequest(pr, e);
		return;
	}
}
