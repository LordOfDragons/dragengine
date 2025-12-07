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
#include <stdlib.h>
#include <string.h>

#include "deoglImposterBillboard.h"
#include "../skin/channel/deoglSkinChannel.h"
#include "../texture/arraytexture/deoglArrayTexture.h"

#include <dragengine/common/exceptions.h>



// Class deoglImposterBillboard
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglImposterBillboard::deoglImposterBillboard(deoglRenderThread &renderThread) :
pRenderThread(renderThread),
pChannelTextures(NULL)
{
	int i;
	pChannelTextures = new deoglArrayTexture*[deoglSkinChannel::CHANNEL_COUNT];
	for(i=0; i<deoglSkinChannel::CHANNEL_COUNT; i++){
		pChannelTextures[i] = NULL;
	}
}

deoglImposterBillboard::~deoglImposterBillboard(){
	if(pChannelTextures){
		int i;
		
		for(i=0; i<deoglSkinChannel::CHANNEL_COUNT; i++){
			if(pChannelTextures[i]){
				delete pChannelTextures[i];
			}
		}
		
		delete [] pChannelTextures;
	}
}



// Management
///////////////

deoglArrayTexture *deoglImposterBillboard::GetChannelTextureAt(int channel) const{
	if(channel < 0 || channel >= deoglSkinChannel::CHANNEL_COUNT){
		DETHROW(deeInvalidParam);
	}
	
	return pChannelTextures[channel];
}

void deoglImposterBillboard::EnableChannelTexture(int channel, bool enable){
	if(channel < 0 || channel >= deoglSkinChannel::CHANNEL_COUNT){
		DETHROW(deeInvalidParam);
	}
	
	if(enable){
		if(pChannelTextures[channel]){
			return;
		}
		
		pChannelTextures[channel] = new deoglArrayTexture(pRenderThread);
		
	}else{
		if(!pChannelTextures[channel]){
			return;
		}
		
		delete pChannelTextures[channel];
		pChannelTextures[channel] = NULL;
	}
}

bool deoglImposterBillboard::IsChannelTextureEnabled(int channel) const{
	if(channel < 0 || channel >= deoglSkinChannel::CHANNEL_COUNT){
		DETHROW(deeInvalidParam);
	}
	
	return pChannelTextures[channel] != NULL;
}



void deoglImposterBillboard::SetExtends(const decVector2 &minExtend, const decVector2 &maxExtend){
	pMinExtend = minExtend;
	pMaxExtend = maxExtend;
}
