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

#ifdef OS_W32
#ifdef CreateService
#undef CreateService
#endif
#endif

#include "deService.h"
#include "../common/collection/decObjectList.h"
#include "../common/string/decStringSet.h"
#include "../threading/deMutex.h"

class deEngine;


/**
 * \brief Service Manager.
 * \version 1.23
 */
class DE_DLL_EXPORT deServiceManager{
private:
	deEngine &pEngine;
	deMutex pMutex;
	decObjectList pEventQueue;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create service manager linked to the given engine. */
	deServiceManager( deEngine &engine );
	
	/** \brief Clean up service manager. */
	~deServiceManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief List of all service names supported by all service modules.
	 */
	decStringSet GetAllSupportedSerices() const;
	
	/**
	 * \brief Create named service.
	 * \throw deeInvalidParam Named service not supported by any loaded service module.
	 */
	deService::Ref CreateService( const char *name );
	
	/**
	 * \brief Queue request response event.
	 * \note Can be called from any thread except the main thread.
	 */
	void QueueRequestResponse( const deService::Ref &service, const decUniqueID &id,
	const deServiceObject::Ref &response, bool finished );
	
	/**
	 * \brief Queue request failed event.
	 * \note Can be called from any thread except the main thread.
	 */
	void QueueRequestFailed( const deService::Ref &service, const decUniqueID &id,
	const deServiceObject::Ref &error );
	
	/**
	 * \brief Queue event received event.
	 * \note Can be called from any thread except the main thread.
	 */
	void QueueEventReceived( const deService::Ref &service, const deServiceObject::Ref &event );
	
	/**
	 * \brief Process all queued events sending them to the respective service.
	 * 
	 * Processed events are removed from the queue.
	 * 
	 * \note Has to be called only from the main thread.
	 */
	void ProcessQueuedEvents();
	/*@}*/
};

#endif
