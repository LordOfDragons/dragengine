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

#ifndef _DEEOSSDKSERVICEEOS_H_
#define _DEEOSSDKSERVICEEOS_H_

#include "deEosSdk.h"
#include "deEosSdkPendingRequest.h"

#include <eos_sdk.h>
#include <eos_auth.h>
#include <eos_userinfo.h>
#include <eos_stats.h>
#include <eos_achievements.h>
#include <eos_connect.h>
#include <eos_ui.h>

#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/systems/modules/service/deBaseServiceService.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/service/deServiceObject.h>

class deService;


/**
 * Steam SDK Service.
 */
class deEosSdkServiceEos : public deBaseServiceService, deEosSdk::cFrameUpdater{
public:
	static const char * const serviceName;
	
	
private:
	deEosSdk &pModule;
	deService * const pService;
	decObjectList pPendingRequests;
	decString pEncAppTicket;
	
	EOS_HPlatform pHandlePlatform;
	EOS_HAuth pHandleAuth;
	EOS_HUserInfo pHandleUserInfo;
	EOS_HAchievements pHandleAchievements;
	EOS_HStats pHandleStats;
	EOS_HConnect pHandleConnect;
	EOS_HUI pHandleUI;
	
	deImage::Ref pAuthProviderIcon, pAuthProviderImage;
	
	EOS_NotificationId pNotifyIdUi;
	bool pUiExclusiveModeEnabled;
	
	
public:
	EOS_EpicAccountId localUserId;
	EOS_EpicAccountId selectedAccountId;
	EOS_ProductUserId productUserId;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create module. */
	deEosSdkServiceEos( deEosSdk &module, deService *service, const deServiceObject::Ref &data );
	
	/** Delete module. */
	~deEosSdkServiceEos() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Get module. */
	inline deEosSdk &GetModule() const{ return pModule; }
	
	/** Service. */
	inline deService *GetService() const{ return pService; }
	
	/** Get handles. */
	EOS_HAuth GetHandleAuth();
	EOS_HUserInfo GetHandleUserInfo();
	EOS_HAchievements GetHandleAchievements();
	EOS_HStats GetHandleStats();
	EOS_HConnect GetHandleConnect();
	EOS_HUI GetHandleUI();
	
	/**
	 * Start service request.
	 * 
	 * Script module peer is called with the received answer. Depending on the service the
	 * answer can be received continuously or as individual responses or in smaller chunks.
	 * The script module has to use a unique id for each request. The same id is returned
	 * in responses to allow matching them to requests. The id can also be used to cancel
	 * a request at any time.
	 */
	void StartRequest( const decUniqueID &id, const deServiceObject &request ) override;
	
	/** Cancel service request if running. */
	void CancelRequest( const decUniqueID &id ) override;
	
	/** Run action returning result immediately. */
	deServiceObject::Ref RunAction( const deServiceObject &action ) override;
	/*@}*/
	
	
	
	/** \name Request */
	/*@{*/
	deEosSdkPendingRequest *GetPendingRequestWithId( const decUniqueID &id ) const;
	deEosSdkPendingRequest::Ref RemoveFirstPendingRequestWithId( const decUniqueID &id );
	deEosSdkPendingRequest::Ref RemoveFirstPendingRequestWithFunction( const char *function );
	deEosSdkPendingRequest::Ref NewPendingRequest( const decUniqueID &id,
		const decString &function, const deServiceObject::Ref &data = nullptr );
	
	void QueryUserInfo( const decUniqueID &id, const deServiceObject& request );
	
	deServiceObject::Ref CopyIdToken( const deServiceObject& action );
	deServiceObject::Ref IsUserLoggedIn( const deServiceObject& action );
	deServiceObject::Ref GetUserFeatures();
	
	void FailRequest( const decUniqueID &id, const deException &e );
	void FailRequest( const decUniqueID &id, EOS_EResult res );
	void FailRequest( const deEosSdkPendingRequest::Ref &request, const deException &e );
	void FailRequest( const deEosSdkPendingRequest::Ref &request, EOS_EResult res );
	/*@}*/
	
	
	
	/** \name EOS Callbacks */
	/*@{*/
	void OnQueryUserInfoCallback( const decUniqueID &id,
		const EOS_UserInfo_QueryUserInfoCallbackInfo &data );

	void OnDisplaySettingsUpdatedCallback(
		const EOS_UI_OnDisplaySettingsUpdatedCallbackInfo &data );
	/*@}*/
	
	
	
	/** \name deEosSdk::cFrameUpdater */
	/*@{*/
	/** Frame update. */
	void FrameUpdate( float elapsed ) override;
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pAddListenUIUpdate();
	void pRemoveListenUIUpdate();
};

#endif
