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

#ifndef _DEEOSSDKFLOWAUTHLOGOUT_H_
#define _DEEOSSDKFLOWAUTHLOGOUT_H_

#include <eos_auth.h>
#include <eos_connect.h>

#include "deEosSdkFlow.h"


/**
 * EOS SDK flow auth logout.
 */
class deEosSdkFlowAuthLogout : public deEosSdkFlow{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create flow. */
	deEosSdkFlowAuthLogout(deEosSdkServiceEos &service, const decUniqueID &id);
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	void ConnectLogout();
	void AuthLogout();
	void DeletePersistentAuth();
	
	void OnConnectLogoutCallback(const EOS_Connect_LogoutCallbackInfo &data);
	void OnAuthLogoutCallback(const EOS_Auth_LogoutCallbackInfo &data);
	void OnDeletePersistentAuthCallback(const EOS_Auth_DeletePersistentAuthCallbackInfo &data);
	/*@}*/
};

#endif
