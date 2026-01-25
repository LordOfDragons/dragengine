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

#include "devkFramebufferConfiguration.h"

#include <dragengine/common/exceptions.h>


// class devkFramebufferConfiguration
//////////////////////////////////////

devkFramebufferConfiguration::devkFramebufferConfiguration() :
pSize(1, 1),
pLayerCount(1){
}

devkFramebufferConfiguration::devkFramebufferConfiguration(const devkFramebufferConfiguration &configuration) :
pSize(1, 1),
pLayerCount(1)
{
	*this = configuration;
}

devkFramebufferConfiguration::~devkFramebufferConfiguration() = default;



// Management
///////////////

void devkFramebufferConfiguration::SetAttachmentCount(int count){
	DEASSERT_TRUE(count >= 0)
	
	pAttachments.SetAll(count, {});
}

void devkFramebufferConfiguration::SetAttachmentAt(int index, devkImageView *attachment){
	pAttachments[index] = attachment;
}

void devkFramebufferConfiguration::SetSize(const decPoint &size){
	DEASSERT_TRUE(size >= decPoint(1, 1))
	
	pSize = size;
}

void devkFramebufferConfiguration::SetLayerCount(int count){
	DEASSERT_TRUE(count > 0)
	
	pLayerCount = count;
}



// Operators
//////////////

bool devkFramebufferConfiguration::operator==(const devkFramebufferConfiguration &configuration) const{
	return pSize == configuration.pSize
		&& pLayerCount == configuration.pLayerCount
		&& pAttachments == configuration.pAttachments;
}

devkFramebufferConfiguration &devkFramebufferConfiguration::operator=(const devkFramebufferConfiguration &configuration){
	pSize = configuration.pSize;
	pLayerCount = configuration.pLayerCount;
	pAttachments = configuration.pAttachments;
	return *this;
}
