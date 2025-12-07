#include "deMTLoadUserResource.h"
#include "../deMsgdkServiceMsgdk.h"
#include "../deMicrosoftGdk.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/file/decMemoryFile.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/filesystem/deVFSMemoryFiles.h>
#include <dragengine/resources/deFileResource.h>
#include <dragengine/resources/service/deServiceManager.h>
#include <dragengine/resources/loader/tasks/deResourceLoaderTask.h>
#include <dragengine/parallel/deParallelTask.h>
#include <dragengine/parallel/deParallelProcessing.h>
#include <dragengine/threading/deThreadSafeObjectReference.h>


// class cLoadResourceTask
////////////////////////////

class cLoadResourceTask : public deParallelTask
{
private:
	deMsgdkServiceMsgdk &pService;
	const deMsgdkAsyncTask::Invalidator::Ref pInvalidator;
	const deMsgdkResourceUrl pUrl;
	const decUniqueID pRequestId;
	const deVirtualFileSystem::Ref pVFS;
	deThreadSafeObjectReference pTaskLoadResource;
		
public:
	cLoadResourceTask(deMsgdkServiceMsgdk &service, const decString &url,
		const decUniqueID &requestId, const decMemoryFile::Ref &memoryFile) :
	deParallelTask(&service.GetModule()),
	pService(service),
	pInvalidator(service.GetInvalidator()),
	pUrl(url),
	pRequestId(requestId),
	pVFS(deVirtualFileSystem::Ref::NewWith()
	{
		SetEmptyRun(true);

		const deVFSMemoryFiles::Ref container(deVFSMemoryFiles::Ref::NewWith(decPath::CreatePathUnix("/")));
		container->AddMemoryFile(memoryFile);
		pVFS->AddContainer(container);

		pTaskLoadResource = service.GetModule().GetGameEngine()->GetResourceLoader()->
			AddLoadRequest(pVFS, memoryFile->GetFilename(), deResourceLoader::ertImage);
		deResourceLoaderTask * const rlt = (deResourceLoaderTask*)(deThreadSafeObject*)pTaskLoadResource;
		
		switch(rlt->GetState())
		{
		case deResourceLoaderTask::esPending:
			if(!DoesDependOn(rlt))
			{
				AddDependsOn(rlt);
			}
			break;
			
		case deResourceLoaderTask::esSucceeded:
			break;
			
		case deResourceLoaderTask::esFailed:
			Cancel();
			break;
		}
	}

	void Run() override
	{
	}
	
	void Finished() override
	{
		if(!pInvalidator->IsValid())
		{
			Cancel();
			return;
		}
		
		const deResourceLoaderTask * const rlt = (deResourceLoaderTask*)(deThreadSafeObject*)pTaskLoadResource;
		
		if(!IsCancelled() && rlt->GetState() == deResourceLoaderTask::esSucceeded)
		{
			FinishRequest(rlt->GetResource());
		}
		else
		{
			FinishRequest(nullptr);
		}
	}
	
	decString GetDebugName() const override
	{
		return "MssdkService-MTLoadUserResource";
	}

	void FinishRequest(deFileResource *resource)
	{
		if(!resource)
		{
			pService.FailRequest(pRequestId, deeInvalidAction(__FILE__, __LINE__, "Failed loading image"));
			return;
		}
		
		pService.GetModule().LogInfoFormat("deMTLoadUserResource.FinishRequest: url=%s resource=%s",
			pUrl.url.GetString(), resource ? "valid" : "null");
		
		const deMsgdkPendingRequest::Ref pr(pService.RemoveFirstPendingRequestWithId(pRequestId));
		if(!pr)
		{
			return;
		}
		
		try
		{
			pr->data->SetResourceChildAt("resource", resource);
			
			pService.GetModule().GetGameEngine()->GetServiceManager()->
				QueueRequestResponse(pService.GetService(), pr->id, pr->data, true);
		}
		catch(const deException &e)
		{
			pService.FailRequest(pr, e);
			return;
		}
	}
};


// Class deMTLoadUserResource
///////////////////////////////

// Constructor, destructor
////////////////////////////

deMTLoadUserResource::deMTLoadUserResource(deMsgdkServiceMsgdk &service,
	const decUniqueID &id, const deServiceObject &request) :
deMsgdkAsyncTask(service.GetInvalidator()),
pService(service),
pUrl(request.GetChildAt("url")->GetString()),
pRequestId(id)
{
	GetGamerPicture();
}

deMTLoadUserResource::~deMTLoadUserResource()
{
}


// Protect Functions
//////////////////////

void deMTLoadUserResource::GetGamerPicture()
{
	if(pUrl.type != "res")
	{
		DETHROW_INFO(deeInvalidParam, "url");
	}
	
	pService.GetModule().LogInfoFormat("deMTLoadUserResource.GetGamerPicture: url=%s", pUrl.url.GetString());
	
	const deServiceObject::Ref data(deServiceObject::Ref::NewWith());
	data->SetStringChildAt("url", pUrl.url);
	data->SetStringChildAt("resourceType", "image");
	
	if(pUrl.getComponentAt(0) == "user")
	{
		const uint64_t uid = deMCCommon::UInt64(pUrl.getComponentAt(1));
		XUserHandle user = nullptr;

		if(uid == pService.GetUserId())
		{
			user = pService.GetUser();
		}

		if(pUrl.getComponentAt(2) == "gamerPicture")
		{
			XUserGamerPictureSize size;

			if(pUrl.getComponentAt(3) == "extraLarge")
			{
				size = XUserGamerPictureSize::ExtraLarge;
			}
			else if(pUrl.getComponentAt(3) == "large")
			{
				size = XUserGamerPictureSize::Large;
			}
			else if(pUrl.getComponentAt(3) == "medium")
			{
				size = XUserGamerPictureSize::Medium;
			}
			else if(pUrl.getComponentAt(3) == "small")
			{
				size = XUserGamerPictureSize::Small;
			}
			else
			{
				DETHROW_INFO(deeInvalidParam, "url");
			}

			pService.NewPendingRequest(pRequestId, "loadUserResource", data);
			try
			{
				if(!user)
				{
					pService.AssertResult(XUserFindUserById(uid, &user),
						"deMTLoadUserResource.GetGamerPicture.XUserFindUserById");
				}

				pService.AssertResult(XUserGetGamerPictureAsync(user, size, GetAsyncBlockPtr()),
					"deMTLoadUserResource.GetGamerPicture.XUserGetGamerPictureAsync");
			}
			catch(const deException &e)
			{
				pService.FailRequest(pRequestId, e);
				FreeReference();
			}
		}
		else
		{
			DETHROW_INFO(deeInvalidParam, "url");
		}
	}
	else
	{
		DETHROW_INFO(deeInvalidParam, "url");
	}
}

void deMTLoadUserResource::OnFinished()
{
	const deMsgdkPendingRequest::Ref pr(pService.GetPendingRequestWithId(pRequestId));
	if(!pr)
	{
		return;
	}
	
	size_t bufferSize;
	HRESULT result = XUserGetGamerPictureResultSize(GetAsyncBlockPtr(), &bufferSize);
	pService.GetModule().LogInfoFormat("deMTLoadUserResource.OnFinished: res(%d)[%s]",
		result, pService.GetModule().GetErrorCodeString(result));
	
	if(FAILED(result))
	{
		pService.FailRequest(pr, result);
		return;
	}
	
	const decMemoryFile::Ref memoryFile(decMemoryFile::Ref::NewWith("/image.png"));
	memoryFile->Resize((int)bufferSize);
	size_t bufferUsed;
	result = XUserGetGamerPictureResult(GetAsyncBlockPtr(),
		bufferSize, memoryFile->GetPointer(), &bufferUsed);

	if(FAILED(result))
	{
		pService.FailRequest(pr, result);
		return;
	}
	
	pService.GetModule().LogInfoFormat("deMTLoadUserResource.OnFinished: Load image from data for url '%s'",
		pUrl.url.GetString());

	deThreadSafeObjectReference task;
	task.TakeOver(new cLoadResourceTask(pService, pUrl.url, pRequestId, memoryFile));
	pService.GetModule().GetGameEngine()->GetParallelProcessing().AddTaskAsync(
		(deParallelTask*)(deThreadSafeObject*)task);
}
