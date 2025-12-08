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

#ifndef _DEGDKSERVICEGDK_H_
#define _DEGDKSERVICEGDK_H_

#include <memory>

#include "gdk_include.h"
#include "deMsgdkPendingRequest.h"
#include "deMsgdkAsyncTask.h"

#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/systems/modules/service/deBaseServiceService.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/service/deServiceObject.h>

class deMicrosoftGdk;
class deService;


/**
 * Microsoft GDK Service.
 */
class deMsgdkServiceMsgdk : public deBaseServiceService
{
public:
	static const char * const serviceName;
	

private:
	deMicrosoftGdk &pModule;
	deService * const pService;
	decObjectList pPendingRequests;
	bool pIsInitialized;
	const deMsgdkAsyncTask::Invalidator::Ref pInvalidator;
	
	XUserHandle pUser;
	uint64_t pUserId;
	XUserLocalId pUserLocalId;

	XblContextHandle pXblContext;
	bool pAchievementsSynced;
	decObjectList pFrameUpdateTasks;

	deImage *pAuthProviderIcon, pAuthProviderImage;

	static deMsgdkServiceMsgdk *pGlobalService;


public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create module. */
	deMsgdkServiceMsgdk(deMicrosoftGdk &module, deService *service,
		const deServiceObject::Ref &data);
	
	/** Delete module. */
	~deMsgdkServiceMsgdk() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Start service request.
	 * 
	 * Script module peer is called with the received answer. Depending on the service the
	 * answer can be received continuously or as individual responses or in smaller chunks.
	 * The script module has to use a unique id for each request. The same id is returned
	 * in responses to allow matching them to requests. The id can also be used to cancel
	 * a request at any time.
	 */
	void StartRequest(const decUniqueID &id, const deServiceObject &request) override;
	
	/**
	 * \brief Cancel service request if running.
	 */
	void CancelRequest(const decUniqueID &id) override;
	
	/**
	 * \brief Run action returning result immediately.
	 */
	deServiceObject::Ref RunAction(const deServiceObject &action) override;
	
	/**
	 * \brief Frame update.
	 */
	void OnFrameUpdate(float elapsed);
	/*@}*/
	
	
	
	/** \name Request */
	/*@{*/
	static deMsgdkServiceMsgdk *GlobalService(){return pGlobalService;}

	inline deMicrosoftGdk &GetModule() const{ return pModule; }
	inline deService *GetService() const{ return pService; }
	inline const deMsgdkAsyncTask::Invalidator::Ref &GetInvalidator() const{ return pInvalidator; }
	
	inline XUserHandle GetUser() const{ return pUser; }
	inline uint64_t GetUserId() const{ return pUserId; }
	inline const XUserLocalId &GetUserLocalId() const{ return pUserLocalId; }
	void SetUser(XUserHandle user);
	
	inline XblContextHandle GetXblContext() const{ return pXblContext; }
	
	deMsgdkPendingRequest *GetPendingRequestWithId(const decUniqueID &id) const;
	deMsgdkPendingRequest::Ref RemoveFirstPendingRequestWithId(const decUniqueID &id);
	deMsgdkPendingRequest::Ref RemoveFirstPendingRequestWithFunction(const char *function);
	deMsgdkPendingRequest::Ref NewPendingRequest(const decUniqueID &id,
		const decString &function, const deServiceObject::Ref &data = nullptr);
	
	deServiceObject::Ref GetUserFeatures();
	deServiceObject::Ref GetUserInfo();
	deServiceObject::Ref IsUserLoggedIn();
	
	void FailRequest(const decUniqueID &id, HRESULT result);
	void FailRequest(const decUniqueID &id, const deException &e);
	void FailRequest(const deMsgdkPendingRequest::Ref &request, HRESULT result);
	void FailRequest(const deMsgdkPendingRequest::Ref &request, const deException &e);
	void AssertResult(HRESULT result, const char *source) const;

	void UpdateAchievementManager();
	inline bool GetAchievementsSynced() const{ return pAchievementsSynced; }

	void AddFrameUpdateTask(deMsgdkAsyncTask *task);
	void RemoveFrameUpdateTask(deMsgdkAsyncTask *task);
	void FrameUpdateTasks(float elapsed);

	decString UriEncode(const char *url) const;
	/*@}*/
	
	
	
	/** \name Callbacks */
	/*@{*/
	void SetResultFields(HRESULT result, deServiceObject &so) const;
	void SetResultFields(const deException &exception, deServiceObject &so) const;
	/*@}*/
};

#endif
