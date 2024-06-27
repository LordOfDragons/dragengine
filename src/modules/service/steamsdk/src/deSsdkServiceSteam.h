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

#ifndef _DESSDKSERVICESTEAM_H_
#define _DESSDKSERVICESTEAM_H_

#include "deSsdkPendingRequest.h"

#include <steam_api.h>

#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/systems/modules/service/deBaseServiceService.h>
#include <dragengine/resources/service/deServiceObject.h>

class deSteamSdk;
class deService;


/**
 * Steam SDK Service.
 */
class deSsdkServiceSteam : public deBaseServiceService{
public:
	static const char * const serviceName;
	
	
private:
	deSteamSdk &pModule;
	deService * const pService;
	decObjectList pPendingRequests;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create module. */
	deSsdkServiceSteam( deSteamSdk &module, deService *service, const deServiceObject::Ref &data );
	
	/** Delete module. */
	~deSsdkServiceSteam() override;
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
	void StartRequest( const decUniqueID &id, const deServiceObject &request ) override;
	
	/**
	 * \brief Cancel service request if running.
	 */
	void CancelRequest( const decUniqueID &id ) override;
	/*@}*/
	
	
	
	/** \name Request */
	/*@{*/
	deSsdkPendingRequest *GetPendingRequestWithId( const decUniqueID &id ) const;
	deSsdkPendingRequest::Ref RemoveFirstPendingRequestWithId( const decUniqueID &id );
	deSsdkPendingRequest::Ref RemoveFirstPendingRequestWithFunction( const char *function );
	deSsdkPendingRequest::Ref NewPendingRequest( const decUniqueID &id,
		const decString &function, const deServiceObject::Ref &data = nullptr );
	
	void RequestCurrentStats( const decUniqueID &id );
	void GetStats( const decUniqueID &id, const deServiceObject& request );
	void SetStats( const decUniqueID &id, const deServiceObject& request );
	void ResetAllStats( const decUniqueID &id, const deServiceObject& request );
	void RequestEncryptedAppTicket( const decUniqueID &id, const deServiceObject& request );
	
	void FailRequest( const decUniqueID &id, const deException &e );
	void FailRequest( const deSsdkPendingRequest::Ref &request, const deException &e );
	/*@}*/
	
	
	
	/** \name Steam Callbacks */
	/*@{*/
	STEAM_CALLBACK( deSsdkServiceSteam, OnUserStatsReceived, UserStatsReceived_t );
	STEAM_CALLBACK( deSsdkServiceSteam, OnUserStatsStored, UserStatsStored_t );
	STEAM_CALLRESULT( deSsdkServiceSteam, OnEncryptedAppTicketResponse, EncryptedAppTicketResponse_t );
	/*@}*/
	
	
private:
	void pSetResultFields( EResult result, deServiceObject &so ) const;
};

#endif
