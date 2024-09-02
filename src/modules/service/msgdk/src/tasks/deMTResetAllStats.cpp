#include "deMTResetAllStats.h"
#include "../deMsgdkServiceMsgdk.h"
#include "../deMicrosoftGdk.h"
#include "../deMsgdkGameConfig.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/service/deServiceManager.h>


// Class deMTResetAchievements
////////////////////////////////

// Constructor, destructor
////////////////////////////

deMTResetAllStats::deMTResetAllStats(deMsgdkServiceMsgdk &service, const deServiceObject &request) :
deMsgdkAsyncTask(service.GetInvalidator()),
pService(service),
pWithAchievements(false),
pWaitAchievementsSynced(false)
{
	pService.GetModule().LogInfo("deMTResetAchievements: Reset achievements");

	try
	{
		deServiceObject::Ref so(request.GetChildAt("withAchievements"));
		if(so)
		{
			pWithAchievements = so->GetBoolean();
		}

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

		pResetStats();
		if(pWithAchievements)
		{
			pResetAchievements();
		}
		else
		{
			FreeReference();
		}
	}
	catch(const deException &e)
	{
		service.GetModule().LogException(e);
		FreeReference();
	}
}

deMTResetAllStats::~deMTResetAllStats()
{
}


// Management
///////////////

void deMTResetAllStats::OnFrameUpdate(float elapsed)
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

		pResetStats();
		if(pWithAchievements)
		{
			pResetAchievements();
		}
		else
		{
			FreeReference();
		}
	}
	catch(const deException &e)
	{
		pService.GetModule().LogException(e);
		FreeReference();
	}
}


// Protect Functions
//////////////////////

void deMTResetAllStats::pResetStats()
{
	XblTitleManagedStatistic dummy{};
	dummy.statisticName = "_does_not_exist_";
	dummy.statisticType = XblTitleManagedStatType::Number;
	dummy.numberValue = 0.0;
	pService.AssertResult(XblTitleManagedStatsWriteAsync(pService.GetXblContext(),
		pService.GetUserId(), &dummy, 1, GetAsyncBlockPtr()),
		"deMTResetAchievements.pResetStats.XblTitleManagedStatsWriteAsync");
}

void deMTResetAllStats::pResetAchievements()
{
	// not supported
}

void deMTResetAllStats::OnFinished()
{
	HRESULT result = XAsyncGetStatus(GetAsyncBlockPtr(), false);
	if(FAILED(result))
	{
		pService.GetModule().LogErrorFormat("Failed: %s",
			pService.GetModule().GetErrorCodeString(result).GetString());
		return;
	}
	
	pService.GetModule().LogInfo("deMTResetAchievements.OnFinished: Stats reset");
}
