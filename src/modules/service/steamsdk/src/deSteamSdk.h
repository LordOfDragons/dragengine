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

#ifndef _DESTEAMSDK_H_
#define _DESTEAMSDK_H_

#include "steam_api.h"

#include <dragengine/systems/modules/service/deBaseServiceModule.h>


/** Macro to define the CCallResult system SteamSDK does not provide a macro for. */
#define STEAM_CALLRESULT(cls,method,resptype) \
	struct sCR##method{\
		CCallResult<cls,resptype> callresult;\
		void Run(cls *myself, SteamAPICall_t handle){\
			callresult.Set(handle, myself, &cls::method);\
		}\
	} pCR##method;\
	void method(resptype *response, bool iofailure);


/**
 * Steam SDK Service Module.
 */
class deSteamSdk : public deBaseServiceModule{
private:
	bool pSdkInited;
	
	
public:
	static deSteamSdk *globalModule;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create module. */
	deSteamSdk(deLoadableModule &loadableModule);
	
	/** Delete module. */
	~deSteamSdk() override;
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** Set of supported service names. */
	decStringSet GetSupportedServices() override;
	
	/** Initialize Steam SDK if not initialized yet. */
	void InitSdk(const deServiceObject::Ref &data);
	
	/**
	 * Create service peer.
	 * 
	 * If service name is not supported nullptr is returned.
	 */
	deBaseServiceService::Ref CreateService(deService *service,
		const char *name, const deServiceObject::Ref &data) override;
	
	/** Frame update. */
	void FrameUpdate(float elapsed) override;
	
	/** Get message matching result code. */
	const char *GetResultMessage(EResult result) const;
	/*@}*/
};

#endif
