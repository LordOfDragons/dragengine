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

#include <stdlib.h>
#include <string.h>

#include "devkFramebufferConfiguration.h"

#include <dragengine/common/exceptions.h>


// class devkFramebufferConfiguration
//////////////////////////////////////

devkFramebufferConfiguration::devkFramebufferConfiguration() :
pAttachmentCount(0),
pAttachments(nullptr),
pSize(1, 1),
pLayerCount(1){
}

devkFramebufferConfiguration::devkFramebufferConfiguration(const devkFramebufferConfiguration &configuration) :
pAttachmentCount(0),
pAttachments(nullptr),
pSize(1, 1),
pLayerCount(1)
{
	*this = configuration;
}

devkFramebufferConfiguration::~devkFramebufferConfiguration(){
	if(pAttachments){
		delete [] pAttachments;
	}
}



// Management
///////////////

void devkFramebufferConfiguration::SetAttachmentCount(int count){
	if(count < 0){
		DETHROW_INFO(deeInvalidParam, "count < 0");
	}
	
	if(pAttachments){
		delete [] pAttachments;
		pAttachments = nullptr;
		pAttachmentCount = 0;
	}
	
	if(count == 0){
		return;
	}
	
	pAttachments = new devkImageView::Ref[count];
	pAttachmentCount = count;
}

devkImageView *devkFramebufferConfiguration::GetAttachmentAt(int index) const{
	if(index < 0){
		DETHROW_INFO(deeInvalidParam, "index < 0");
	}
	if(index >= pAttachmentCount){
		DETHROW_INFO(deeInvalidParam, "index >= attachmentCount");
	}
	
	return pAttachments[index];
}

void devkFramebufferConfiguration::SetAttachmentAt(int index, devkImageView *attachment){
	if(index < 0){
		DETHROW_INFO(deeInvalidParam, "index < 0");
	}
	if(index >= pAttachmentCount){
		DETHROW_INFO(deeInvalidParam, "index >= attachmentCount");
	}
	
	pAttachments[index] = attachment;
}

void devkFramebufferConfiguration::SetSize(const decPoint &size){
	if(!(size >= decPoint(1, 1))){
		DETHROW_INFO(deeInvalidParam, "!(size >= (1,1))");
	}
	
	pSize = size;
}

void devkFramebufferConfiguration::SetLayerCount(int count){
	if(count < 1){
		DETHROW_INFO(deeInvalidParam, "count < ");
	}
	
	pLayerCount = count;
}



// Operators
//////////////

bool devkFramebufferConfiguration::operator==(const devkFramebufferConfiguration &configuration) const{
	if(pAttachmentCount != configuration.pAttachmentCount
	|| pSize != configuration.pSize
	|| pLayerCount != configuration.pLayerCount){
		return false;
	}
	
	int i;
	for(i=0; i<pAttachmentCount; i++){
		if(pAttachments[i] != configuration.pAttachments[i]){
			return false;
		}
	}
	
	return true;
}

devkFramebufferConfiguration &devkFramebufferConfiguration::operator=(const devkFramebufferConfiguration &configuration){
	SetAttachmentCount(configuration.pAttachmentCount);
	int i;
	for(i=0; i<pAttachmentCount; i++){
		pAttachments[i] = configuration.pAttachments[i];
	}
	
	pSize = configuration.pSize;
	pLayerCount = configuration.pLayerCount;
	return *this;
}
