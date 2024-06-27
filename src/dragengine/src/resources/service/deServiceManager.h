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

#ifndef _DESERVICEMANAGER_H_
#define _DESERVICEMANAGER_H_ 

#include "deService.h"
#include "deServiceObject.h"
#include "../deResourceManager.h"
#include "../deResourceList.h"
#include "../../common/collection/decObjectList.h"
#include "../../common/collection/decPointerOrderedSet.h"
#include "../../common/string/decStringSet.h"
#include "../../threading/deMutex.h"

class deEngine;


/**
 * \brief Service Manager.
 * \version 1.23
 */
class DE_DLL_EXPORT deServiceManager : public deResourceManager{
private:
	deMutex pMutex;
	decObjectList pEventQueue;
	deResourceList pServices;
	decPointerOrderedSet pModules;
	bool pDirtyModules;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create service manager linked to the given engine. */
	deServiceManager( deEngine *engine );
	
	/** \brief Clean up service manager. */
	~deServiceManager() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of service resource. */
	int GetServiceCount() const;
	
	/** \brief Root service resource for iteration purpose. */
	deService *GetRootService() const;
	
	/** \brief Release leaking resources and report them. */
	void ReleaseLeakingResources() override;
	
	
	
	/**
	 * \brief List of all service names supported by all service modules.
	 */
	decStringSet GetAllSupportedSerices() const;
	
	/**
	 * \brief Create named service.
	 * \throw deeInvalidParam Named service not supported by any loaded service module.
	 * 
	 * Caller takes over reference. Use deService::Ref::New() or deService::Ref:TakeOver
	 * to acquire the returned reference correctly.
	 */
	deService *CreateService( const char *name, const deServiceObject::Ref &data );
	
	/**
	 * \brief Queue request response event.
	 * \note Can be called from any thread.
	 */
	void QueueRequestResponse( deService *service, const decUniqueID &id,
	const deServiceObject::Ref &response, bool finished );
	
	/**
	 * \brief Queue request failed event.
	 * \note Can be called from any thread.
	 */
	void QueueRequestFailed( deService *service, const decUniqueID &id,
	const deServiceObject::Ref &error );
	
	/**
	 * \brief Queue event received event.
	 * \note Can be called from any thread.
	 */
	void QueueEventReceived( deService *service, const deServiceObject::Ref &event );
	
	/**
	 * \brief Remove all events matching service.
	 * \note Can be called from any thread.
	 */
	void RemoveAllMatchingEvents( deService *service );
	
	
	
	/**
	 * \brief Update services and process all queued events.
	 * 
	 * Queued events are sent to the respective service then removed from the queue.
	 * 
	 * \note Has to be called only from the main thread.
	 */
	void FrameUpdate();
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	void SystemScriptingLoad() override;
	void SystemScriptingUnload() override;
	/*@}*/
	
	
	
	/**
	 * \name Resource only Functions
	 * \brief Those functions are only for resource objects and should never be called directly from an application.
	 */
	/*@{*/
	void RemoveResource( deResource *resource ) override;
	/*@}*/
	
	
	
private:
	void pUpdateModuleList();
	void pUpdateModules();
	void pProcessEvents();
};

#endif
