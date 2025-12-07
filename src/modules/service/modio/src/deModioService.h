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

#ifndef _deModioSERVICE_H_
#define _deModioSERVICE_H_

#include <stdint.h>

#include "deModioPendingRequest.h"
#include "modio.h"

#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/collection/decObjectDictionary.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/resources/service/deServiceObject.h>
#include <dragengine/systems/modules/service/deBaseServiceService.h>

class deModio;
class deService;


/**
 * Mod.io mods service.
 */
class deModioService : public deBaseServiceService{
public:
	static const char * const serviceName;
	
	class cInvalidator : public deObject{
	public:
		bool invalidated;
		typedef deTObjectReference<cInvalidator> Ref;
		cInvalidator();
	};
	
	
private:
	deModio &pModule;
	deService * const pService;
	decObjectList pPendingRequests;
	decString pApiKey;
	uint64_t pGameId;
	decString pUserId;
	Modio::Portal pPortal;
	bool pIsInitialized;
	int pRequiresEventHandlingCount;
	decObjectDictionary pResources;
	cInvalidator::Ref pInvalidator;
	deVirtualFileSystem::Ref pVFS;
	decStringList pBaseDirPathStr;
	decPath *pBaseDirPath;
	int pBaseDirPathCount;
	bool pPauseModManagement;
	bool pModManagementEnabled;
	float pElapsedUpdateProgress;
	float pUpdateProgressInterval;
	Modio::Optional<Modio::GameInfo> pGameInfo;
	
	static deModioService *pGlobalService;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create module. */
	deModioService(deModio &module, deService *service, const deServiceObject::Ref &data);
	
	/** Delete module. */
	~deModioService() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	inline deModio &GetModule(){return pModule;}
	inline const cInvalidator::Ref &GetInvalidator() const{return pInvalidator;}
	inline const deVirtualFileSystem::Ref &GetVFS() const{return pVFS;}
	
	static inline deModioService *Global(){return pGlobalService;}
	
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
	
	/** Frame update. */
	void FrameUpdate(float elapsed);
	/*@}*/
	
	
	
	/** \name Request */
	/*@{*/
	deModioPendingRequest *GetPendingRequestWithId(const decUniqueID &id) const;
	deModioPendingRequest::Ref RemoveFirstPendingRequestWithId(const decUniqueID &id);
	deModioPendingRequest::Ref RemoveFirstPendingRequestWithFunction(const char *function);
	deModioPendingRequest::Ref NewPendingRequest(const decUniqueID &id,
		const decString &function, const deServiceObject::Ref &data = nullptr);
	
	void ListAllMods(const decUniqueID &id, const deServiceObject &request);
	void LoadModResource(const decUniqueID &id, const deServiceObject &request);
	void PauseModManagement(const decUniqueID &id, const deServiceObject &request);
	void AuthenticateUserExternal(const decUniqueID &id, const deServiceObject &request);
	void ClearUserData(const decUniqueID &id, const deServiceObject &request);
	void SubscribeToMod(const decUniqueID &id, const deServiceObject &request);
	void UnsubscribeFromMod(const decUniqueID &id, const deServiceObject &request);
	void GetModInfo(const decUniqueID &id, const deServiceObject &request);
	void SubmitModRating(const decUniqueID &id, const deServiceObject &request);
	void RevokeModRating(const decUniqueID &id, const deServiceObject &request);
	void ReportMod(const decUniqueID &id, const deServiceObject &request);
	void GetModTagOptions(const decUniqueID &id, const deServiceObject &request);
	void LoadUserResource(const decUniqueID &id, const deServiceObject &request);
	void GetUserWalletBalance(const decUniqueID &id, const deServiceObject &request);
	void ReportUser(const decUniqueID &id, const deServiceObject &request);
	void GetUserPurchasedMods(const decUniqueID &id, const deServiceObject &request);
	void PurchaseMod(const decUniqueID &id, const deServiceObject &request);
	
	void ActivateMods();
	deServiceObject::Ref IsAuthenticated();
	deServiceObject::Ref QueryCurrentModUpdate();
	deServiceObject::Ref QueryUserSubscriptions();
	deServiceObject::Ref QuerySystemInstallations();
	deServiceObject::Ref QueryUserProfile();
	void SetModDisabled(const deServiceObject &action);
	deServiceObject::Ref ModHasMatchingFiles(const deServiceObject &action);
	deServiceObject::Ref GetActiveMods();
	deServiceObject::Ref GetModsFeatures();
	deServiceObject::Ref GetUserFeatures();
	
	void FailRequest(const decUniqueID &id, const deException &e);
	void FailRequest(const decUniqueID &id, const Modio::ErrorCode &ec);
	void FailRequest(const deModioPendingRequest::Ref &request, const deException &e);
	void FailRequest(const deModioPendingRequest::Ref &request, const Modio::ErrorCode &ec);
	
	void OnFinishedLoadResource(const decUniqueID &id, const decString &path, deResource *resource);
	
	void AddRequiresEventHandlingCount();
	void RemoveRequiresEventHandlingCount();
	/*@}*/
	
	
	
private:
	/** \name Callbacks */
	/*@{*/
	void pOnInitialize(Modio::ErrorCode ec);
	void pOnInitializeGetGameInfo(Modio::ErrorCode ec, Modio::Optional<Modio::GameInfo> info);
	void pOnInitializeFetchUpdates(Modio::ErrorCode ec);
	void pOnRequestFinished(const decUniqueID &id, Modio::ErrorCode ec);
	
	void pOnListAllModsFinished(const decUniqueID &id, Modio::ErrorCode ec,
		Modio::Optional<Modio::ModInfoList> results);
	
	void pOnLoadResourceFinished(const decUniqueID &id, Modio::ErrorCode ec,
		Modio::Optional<std::string> filename);
	
	void pOnGetModInfo(const decUniqueID &id, Modio::ErrorCode ec,
		Modio::Optional<Modio::ModInfo> info);
	
	void pOnAuthenticateUserExternal(const decUniqueID &id, Modio::ErrorCode ec);
	void pOnAuthenticateUserExternalFetchUpdates(const decUniqueID &id, Modio::ErrorCode ec);
	void pOnClearUserData(const decUniqueID &id, Modio::ErrorCode ec);
	
	void pOnSubscribeToMod(const decUniqueID &id, Modio::ErrorCode ec);
	void pOnUnsubscribeFromMod(const decUniqueID &id, Modio::ErrorCode ec);
	
	void pOnGetModTagOptions(const decUniqueID &id, Modio::ErrorCode ec,
		Modio::Optional<Modio::ModTagOptions> tagOptions);
	
	void pOnGetUserWalletBalance(const decUniqueID &id, Modio::ErrorCode ec,
		Modio::Optional<uint64_t> amount);
	
	void pOnGetUserPurchasedMods(const decUniqueID &id, Modio::ErrorCode ec);
	
	void pOnPurchaseMod(const decUniqueID &id, Modio::ErrorCode ec,
		Modio::Optional<Modio::TransactionRecord> record);
	
	void pOnLogCallback(Modio::LogLevel level, const std::string &message);
	void pOnModManagement(Modio::ModManagementEvent event);
	/*@}*/
	
	void pPrintBaseInfos();
	void pInitVFS();
	void pUpdateModManagementEnabled();
	void pCheckProgressUpdate(float elapsed);
	void pActivateMods();
	
	deModioPendingRequest::Ref pOnBaseResponseInit(const decUniqueID &id, Modio::ErrorCode ec,
		bool peekPendingRequest = false);
	
	bool pOnBaseResponseExit(const deModioPendingRequest::Ref &pr);
};

#endif
