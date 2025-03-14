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

#include "devkImageConfiguration.h"

#include <dragengine/common/exceptions.h>


// class devkImageConfiguration
/////////////////////////////////

devkImageConfiguration::devkImageConfiguration() :
pType(VK_IMAGE_TYPE_2D),
pSize(1, 1, 1),
pLayerCount(1),
pMipMapCount(1),
pFormat(VK_FORMAT_R8G8B8A8_UINT),
pFlags(0),
pSamples(VK_SAMPLE_COUNT_1_BIT),
pUsage(VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT){
}

devkImageConfiguration::devkImageConfiguration(const devkImageConfiguration &configuration){
	*this = configuration;
}

devkImageConfiguration::~devkImageConfiguration(){
}



// Management
///////////////

void devkImageConfiguration::SetType(VkImageType type){
	pType = type;
}

void devkImageConfiguration::SetSize(const decPoint3 &size){
	if(! (size >= decPoint3(1, 1, 1))){
		DETHROW(deeInvalidParam);
	}
	
	pSize = size;
}

void devkImageConfiguration::SetLayerCount(int count){
	if(count < 1){
		DETHROW(deeInvalidParam);
	}
	
	pLayerCount = count;
}

void devkImageConfiguration::SetMipMapCount(int count){
	if(count < 1){
		DETHROW(deeInvalidParam);
	}
	
	pMipMapCount = count;
}

void devkImageConfiguration::SetFormat(VkFormat format){
	pFormat = format;
}

void devkImageConfiguration::SetFlags(VkImageCreateFlags flags){
	pFlags = flags;
}

void devkImageConfiguration::SetSamples(VkSampleCountFlagBits samples){
	pSamples = samples;
}

void devkImageConfiguration::SetUsage(VkImageUsageFlags usage){
	pUsage = usage;
}



void devkImageConfiguration::Set2D(const decPoint &size, VkFormat format){
	SetSize(decPoint3(size.x, size.y, 1));
	SetFormat(format);
	
	pType = VK_IMAGE_TYPE_2D;
	pLayerCount = 1;
	pMipMapCount = 1;
	pFlags = 0;
	pSamples = VK_SAMPLE_COUNT_1_BIT;
	pUsage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
}

void devkImageConfiguration::Set3D(const decPoint3 &size, VkFormat format){
	SetSize(size);
	SetFormat(format);
	
	pType = VK_IMAGE_TYPE_3D;
	pLayerCount = 1;
	pMipMapCount = 1;
	pFlags = 0;
	pSamples = VK_SAMPLE_COUNT_1_BIT;
	pUsage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
}

void devkImageConfiguration::SetCube(int size, VkFormat format){
	SetSize(decPoint3(size, size, 1));
	SetFormat(format);
	
	pType = VK_IMAGE_TYPE_2D;
	pLayerCount = 6;
	pMipMapCount = 1;
	pFlags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
	pSamples = VK_SAMPLE_COUNT_1_BIT;
	pUsage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
}

void devkImageConfiguration::Set2DArray(const decPoint &size, int layerCount, VkFormat format){
	SetSize(decPoint3(size.x, size.y, 1));
	SetLayerCount(layerCount);
	SetFormat(format);
	
	pType = VK_IMAGE_TYPE_2D;
	pMipMapCount = 1;
	pFlags = 0;
	pSamples = VK_SAMPLE_COUNT_1_BIT;
	pUsage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
}

void devkImageConfiguration::SetCubeArray(int size, int layerCount, VkFormat format){
	SetSize(decPoint3(size, size, 1));
	SetLayerCount(6 * layerCount);
	SetFormat(format);
	
	pType = VK_IMAGE_TYPE_2D;
	pMipMapCount = 1;
	pFlags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
	pSamples = VK_SAMPLE_COUNT_1_BIT;
	pUsage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
}



void devkImageConfiguration::DisableAll(){
	pUsage = 0;
}

void devkImageConfiguration::EnableTransfer(bool enable){
	if(enable){
		pUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		
	}else{
		pUsage &= ~(VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT);
	}
}

void devkImageConfiguration::EnableTransferSource(bool enable){
	if(enable){
		pUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		
	}else{
		pUsage &= ~VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
	}
}

void devkImageConfiguration::EnableTransferDestination(bool enable){
	if(enable){
		pUsage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		
	}else{
		pUsage &= ~VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	}
}

void devkImageConfiguration::EnableSampled(bool enable){
	if(enable){
		pUsage |= VK_IMAGE_USAGE_SAMPLED_BIT;
		
	}else{
		pUsage &= ~VK_IMAGE_USAGE_SAMPLED_BIT;
	}
}

void devkImageConfiguration::EnableStorage(bool enable){
	if(enable){
		pUsage |= VK_IMAGE_USAGE_STORAGE_BIT;
		
	}else{
		pUsage &= ~VK_IMAGE_USAGE_STORAGE_BIT;
	}
}

void devkImageConfiguration::EnableColorAttachment(bool enable){
	if(enable){
		pUsage &= ~VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		pUsage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		
	}else{
		pUsage &= ~VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	}
}

void devkImageConfiguration::EnableDepthStencilAttachment(bool enable){
	if(enable){
		pUsage &= ~VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		pUsage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		
	}else{
		pUsage &= ~VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	}
}



// Operators
//////////////

bool devkImageConfiguration::operator==(const devkImageConfiguration &configuration) const{
	return pType == configuration.pType
		&& pSize == configuration.pSize
		&& pLayerCount == configuration.pLayerCount
		&& pMipMapCount == configuration.pMipMapCount
		&& pFormat == configuration.pFormat
		&& pFlags == configuration.pFlags
		&& pSamples == configuration.pSamples
		&& pUsage == configuration.pUsage;
}

devkImageConfiguration &devkImageConfiguration::operator=(const devkImageConfiguration &configuration){
	pType = configuration.pType;
	pSize = configuration.pSize;
	pLayerCount = configuration.pLayerCount;
	pMipMapCount = configuration.pMipMapCount;
	pFormat = configuration.pFormat;
	pFlags = configuration.pFlags;
	pSamples = configuration.pSamples;
	pUsage = configuration.pUsage;
	return *this;
}
