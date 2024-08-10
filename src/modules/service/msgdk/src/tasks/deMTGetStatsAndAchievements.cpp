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
pResultData(deServiceObject::Ref::New(new deServiceObject))
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

		pGetAchievements();
		pGetStarts();
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


// Protect Functions
//////////////////////

void deMTGetStatsAndAchievements::pGetAchievements()
{
	if(pAchievements.GetCount() == 0)
	{
		pResultData->SetChildAt("achievements", deServiceObject::Ref::New( new deServiceObject ));
		return;
	}
	
	XblAchievementsManagerResultHandle achievementResult;
	pService.AssertResult(XblAchievementsManagerGetAchievements(
		pService.GetUserId(), XblAchievementOrderBy::DefaultOrder,
		XblAchievementsManagerSortOrder::Ascending, &achievementResult),
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

void deMTGetStatsAndAchievements::pGetStarts()
{
	const int count = pStats.GetCount();
	if(count == 0)
	{
		const deMsgdkPendingRequest::Ref pr(pService.GetPendingRequestWithId(pRequestId));
		if(pr)
		{
			pResultData->SetChildAt("stats", deServiceObject::Ref::New(new deServiceObject));
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
			pService.FailRequest(pr, result);
			return;
		}
		
		const deServiceObject::Ref so(deServiceObject::Ref::New(new deServiceObject));
		const decString &scid = pService.GetModule().GetGameConfig().scid;
		uint32_t i, j;

		for(i=0; i<usr->serviceConfigStatisticsCount; i++)
		{
			const XblServiceConfigurationStatistic &scs = usr->serviceConfigStatistics[i];
			if(scs.serviceConfigurationId != scid)
			{
				continue;
			}

			for(j=0; j<scs.statisticsCount; j++)
			{
				const char * const name = scs.statistics[j].statisticName;
				if(pStats.Has(name))
				{
					so->SetIntChildAt(name, decString(scs.statistics[j].value).ToIntValid());
				}
			}
		}

		pResultData->SetChildAt("stats", so);

		pService.GetModule().LogInfo("deMTGetStatsAndAchievements.OnFinished: Stats received");

		pService.GetModule().GetGameEngine()->GetServiceManager()->
			QueueRequestResponse(pService.GetService(), pr->id, pr->data, true);
	}
	catch(const deException &e)
	{
		delete [] buffer;
		pService.FailRequest(pr, e);
		return;
	}
}
