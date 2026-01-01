#include "deMTSetStatsAndAchievements.h"
#include "../deMsgdkServiceMsgdk.h"
#include "../deMicrosoftGdk.h"
#include "../deMsgdkGameConfig.h"
#include "../convert/deMCCommon.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/service/deServiceManager.h>


// Class deMTSetStatsAndAchievements
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

deMTSetStatsAndAchievements::deMTSetStatsAndAchievements(deMsgdkServiceMsgdk &service,
	const decUniqueID &id, const deServiceObject &request) :
deMsgdkAsyncTask(service.GetInvalidator()),
pService(service),
pRequestId(id),
pResultData(deServiceObject::Ref::New()),
pWaitAchievementsSynced(false)
{
	pService.GetModule().LogInfo("deMTSetStatsAndAchievements: Set stats and achievements");

	deServiceObject::Ref so;
	
	so = request.GetChildAt("stats");
	if(so){
		pResultData->SetChildAt("stats", deServiceObject::Ref::New(so, true));
	}
	else
	{
		pResultData->SetChildAt("stats", deServiceObject::Ref::New());
	}
	
	so = request.GetChildAt("achievements");
	if(so){
		pResultData->SetChildAt("achievements", deServiceObject::Ref::New(so, true));
	}
	else
	{
		pResultData->SetChildAt("achievements", deServiceObject::Ref::New());
	}

	pService.NewPendingRequest(pRequestId, "setStatsAndAchievements", pResultData);
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

		pSetAchievements();
		pSetStats();
	}
	catch(const deException &e)
	{
		pService.FailRequest(pRequestId, e);
		FreeReference();
	}
}

deMTSetStatsAndAchievements::~deMTSetStatsAndAchievements()
{
}


// Management
///////////////

void deMTSetStatsAndAchievements::OnFrameUpdate(float elapsed)
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

		pSetAchievements();
		pSetStats();
	}
	catch(const deException &e)
	{
		pService.FailRequest(pRequestId, e);
		FreeReference();
	}
}


// Protect Functions
//////////////////////

void deMTSetStatsAndAchievements::pSetAchievements()
{
	const deServiceObject::Ref &so = pResultData->GetChildAt("achievements");
	const decStringList names(so->GetChildrenKeys());
	const int count = names.GetCount();

	if(count == 0)
	{
		return;
	}
	
	const uint64_t userId = pService.GetUserId();
	uint8_t value;
	int i;

	for(i=0; i<count; i++)
	{
		const decString &name = names.GetAt(i);

		if(so->GetChildAt(name)->GetBoolean())
		{
			value = 100;
		}
		else
		{
			value = 1;
		}

		// this call can fail with E_NOT_VALID_STATE if the achievement has been already unlocked
		// or E_INVALIDARG if the progress value is equal or less than the cached progress.
		// for this reason no error checking is done
		XblAchievementsManagerUpdateAchievement(userId, name, value);
	}
}

void deMTSetStatsAndAchievements::pSetStats()
{
	const deServiceObject::Ref &so = pResultData->GetChildAt("stats");
	const decStringList names(so->GetChildrenKeys());
	const int count = names.GetCount();

	if(count == 0)
	{
		const deMsgdkPendingRequest::Ref pr(pService.GetPendingRequestWithId(pRequestId));
		if(pr)
		{
			pService.GetModule().GetGameEngine()->GetServiceManager()->
				QueueRequestResponse(pService.GetService(), pr->id, pr->data, true);
		}
		FreeReference();
		return;
	}
	
	decStringList values;
	decString value;

	XblTitleManagedStatistic * const xblstats = new XblTitleManagedStatistic[count];
	try
	{
		int i;
		for(i=0; i<count; i++)
		{
			const decString &name = names.GetAt(i);
			const deServiceObject::Ref &soChild = so->GetChildAt(name);

			xblstats[i] = {};
			xblstats[i].statisticName = name;

			switch(soChild->GetValueType())
			{
			case deServiceObject::evtInteger:
				value.Format("%d", soChild->GetInteger());
				values.Add(value);
				xblstats[i].stringValue = values.GetAt(values.GetCount() - 1);
				xblstats[i].statisticType = XblTitleManagedStatType::String;
				break;

			case deServiceObject::evtFloat:
				xblstats[i].numberValue = (double)soChild->GetFloat();
				xblstats[i].statisticType = XblTitleManagedStatType::Number;
				break;

			case deServiceObject::evtString:
				xblstats[i].stringValue = soChild->GetString();
				xblstats[i].statisticType = XblTitleManagedStatType::String;
				break;

			default:
				value.Format("Invalid value for stat '%s': type %d",
					name.GetString(), soChild->GetValueType());
				DETHROW_INFO(deeInvalidParam, value);
			}
		}

		pService.AssertResult(XblTitleManagedStatsUpdateStatsAsync(
			pService.GetXblContext(), xblstats, count, GetAsyncBlockPtr()),
			"deMTSetStatsAndAchievements.pSetStats.XblTitleManagedStatsUpdateStatsAsync");

		delete [] xblstats;
	}
	catch(const deException &)
	{
		delete [] xblstats;
		throw;
	}
}

void deMTSetStatsAndAchievements::OnFinished()
{
	const deMsgdkPendingRequest::Ref pr(pService.GetPendingRequestWithId(pRequestId));
	if(!pr)
	{
		return;
	}
	
	HRESULT result = XAsyncGetStatus(GetAsyncBlockPtr(), false);
	if(FAILED(result))
	{
		pService.FailRequest(pr, result);
		return;
	}
	
	pService.GetModule().LogInfo("deMTSetStatsAndAchievements.OnFinished: Stats updated");
	
	pService.GetModule().GetGameEngine()->GetServiceManager()->
		QueueRequestResponse(pService.GetService(), pr->id, pr->data, true);
}
