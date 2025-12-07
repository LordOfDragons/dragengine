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

#ifndef DEMTLOADUSERRESOURCE_H
#define DEMTLOADUSERRESOURCE_H

#include "../deMsgdkAsyncTask.h"
#include "../deMsgdkResourceUrl.h"
#include <dragengine/common/utils/decUniqueID.h>

class deMsgdkServiceMsgdk;


/**
 * Load user resource task.
 */
class deMTLoadUserResource : public deMsgdkAsyncTask{
private:
	deMsgdkServiceMsgdk &pService;
	const deMsgdkResourceUrl pUrl;
	const decUniqueID pRequestId;


public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create asynchronous task. Object destroys itself automatically once finished. */
	deMTLoadUserResource(deMsgdkServiceMsgdk &service, const decUniqueID &id, const deServiceObject &request);
	
protected:
	/** Delete asynchronous task. */
	~deMTLoadUserResource() override;
	/*@}*/


public:
	inline deMsgdkServiceMsgdk &GetService() const{return pService;}


protected:
	void GetGamerPicture();
	void OnFinished() override;
};

#endif
