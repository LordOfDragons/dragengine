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

#ifndef _DEEOSSDKFLOW_H_
#define _DEEOSSDKFLOW_H_

#include "../deEosSdkPendingRequest.h"

#include <dragengine/common/string/decString.h>
#include <dragengine/common/utils/decUniqueID.h>
#include <dragengine/resources/service/deServiceObject.h>

#include <eos_sdk.h>

class deEosSdk;
class deEosSdkServiceEos;


/**
 * EOS SDK flow.
 */
class deEosSdkFlow{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create flow. */
	deEosSdkFlow( deEosSdkServiceEos &service, const decUniqueID &id );
	
	/** Delete flow. */
	virtual ~deEosSdkFlow();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	deEosSdk &GetModule() const;
	
	void Fail( const deException &e );
	void Fail( EOS_EResult res );
	void Fail( const deEosSdkPendingRequest::Ref &request, const deException &e );
	void Fail( const deEosSdkPendingRequest::Ref &request, EOS_EResult res );
	void Finish();
	void Abandon();
	
	inline bool HasFailed() const{ return pHasFailed; }
	/*@}*/
	
	
	
protected:
	deEosSdkServiceEos &pService;
	const decUniqueID pId;
	bool pHasFailed;
};

#endif
