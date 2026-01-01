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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "deoglFramebuffer.h"
#include "deoglFramebufferManager.h"
#include "../configuration/deoglConfiguration.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"

#include <dragengine/common/exceptions.h>


// Class deoglFramebufferManager::Usage
/////////////////////////////////////////

deoglFramebufferManager::Usage::Usage() :
pFBO(nullptr){
}

deoglFramebufferManager::Usage::Usage(deoglFramebuffer *fbo) :
pFBO(fbo)
{
	if(fbo){
		fbo->IncreaseUsageCount();
	}
}

deoglFramebufferManager::Usage::Usage(const Usage &usage) :
pFBO(usage.pFBO)
{
	if(usage.pFBO){
		usage.pFBO->IncreaseUsageCount();
	}
}

deoglFramebufferManager::Usage::~Usage(){
	if(pFBO){
		pFBO->DecreaseUsageCount();
	}
}

deoglFramebufferManager::Usage &deoglFramebufferManager::Usage::operator=(const Usage &usage){
	if(pFBO){
		pFBO->DecreaseUsageCount();
	}
	pFBO = usage.pFBO;
	if(usage.pFBO){
		usage.pFBO->IncreaseUsageCount();
	}
	return *this;
}

void deoglFramebufferManager::Usage::Clear(){
	if(pFBO){
		pFBO->DecreaseUsageCount();
		pFBO = nullptr;
	}
}


// Class deoglFramebufferManager
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglFramebufferManager::deoglFramebufferManager(deoglRenderThread &renderThread) :
pRenderThread(renderThread){
}


// Management
///////////////

int deoglFramebufferManager::GetFBOCount() const{
	return pFBOs.GetCount();
}

const deoglFramebuffer &deoglFramebufferManager::GetFBOAt(int index) const{
	return *((deoglFramebuffer*)pFBOs[index]);
}

deoglFramebufferManager::Usage deoglFramebufferManager::GetFBOWithResolution(int width, int height){
	// if we have to hand out only one fbo use the first one
	if(pRenderThread.GetConfiguration().GetUseOneFBO()){
		if(pFBOs.GetCount() == 0){
			pFBOs.Add(deoglFramebuffer::Ref::New(pRenderThread, false));
		}
		
		deoglFramebuffer * const fbo = (deoglFramebuffer*)pFBOs[0];
		fbo->SetUsageResolution(width, height);
		return Usage(fbo);
		
	// otherwise hand out an fbo for each resolution
	}else{
		//deoglFramebuffer *reusableFBO = NULL;
		int i;
		
		// try to find a framebuffer with the same dimensions or one that is reusable
		const int count = pFBOs.GetCount();
		for(i=0; i<count; i++){
			deoglFramebuffer * const fbo = (deoglFramebuffer*)pFBOs[i];
			if(fbo->GetUsageWidth() == width && fbo->GetUsageHeight() == height){
				return Usage(fbo);
			//	
			//}else if(!reusableFBO && fbo->GetUsageCount() == 0){
			//	reusableFBO = fbo;
			}
		}
		
		// if we have a reusable fbo change the size and hand it out
		//if(reusableFBO){
		//	pRenderThread.GetLogger().LogInfoFormat("FBO Manager: Resize FBO (%d x %d)", width, height);
		//	reusableFBO->SetUsageResolution(width, height);
		//	return Usage(reusableFBO);
		//}
		
		// otherwise create a new fbo with the given size and hand it out
		pFBOs.Add(deoglFramebuffer::Ref::New(pRenderThread, false));
		
		deoglFramebuffer * const fbo = (deoglFramebuffer*)pFBOs[pFBOs.GetCount() - 1];
		fbo->SetUsageResolution(width, height);
		pRenderThread.GetLogger().LogInfoFormat("FBO Manager: Create FBO (%d x %d)", width, height);
		return Usage(fbo);
	}
}
