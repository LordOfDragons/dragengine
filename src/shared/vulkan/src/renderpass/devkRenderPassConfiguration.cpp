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

#include "devkRenderPassConfiguration.h"

#include <dragengine/common/exceptions.h>


// Struct devkRenderPassConfiguration::sSubPass
/////////////////////////////////////////////////

bool devkRenderPassConfiguration::sSubPass::operator==(const sSubPass &other) const{
	return colorAttachments[0] == other.colorAttachments[0]
		&& colorAttachments[1] == other.colorAttachments[1]
		&& colorAttachments[2] == other.colorAttachments[2]
		&& colorAttachments[3] == other.colorAttachments[3]
		&& colorAttachments[4] == other.colorAttachments[4]
		&& colorAttachments[5] == other.colorAttachments[5]
		&& colorAttachments[6] == other.colorAttachments[6]
		&& colorAttachments[7] == other.colorAttachments[7]
		&& depthStencilAttachment == other.depthStencilAttachment;
}


// Class devkRenderPassConfiguration
//////////////////////////////////////

devkRenderPassConfiguration::devkRenderPassConfiguration() = default;

devkRenderPassConfiguration::devkRenderPassConfiguration(const devkRenderPassConfiguration &configuration){
	*this = configuration;
}

devkRenderPassConfiguration::~devkRenderPassConfiguration() = default;



// Management
///////////////

void devkRenderPassConfiguration::SetAttachmentCount(int count){
	DEASSERT_TRUE(count >= 0)
	
	pAttachments.SetAll(count, {});
	pClearValues.SetAll(count, {});
}

void devkRenderPassConfiguration::SetAttachmentAt(int index, const VkAttachmentDescription &attachment){
	pAttachments[index] = attachment;
}

void devkRenderPassConfiguration::SetColorAttachmentAt(int index, VkFormat format, pInType in, eOutType out){
	VkAttachmentDescription description{};
	description.format = format;
	description.samples = VK_SAMPLE_COUNT_1_BIT;
	description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	
	switch(in){
	case eitClear:
		description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		break;
		
	case eitLoad:
		description.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
		break;
		
	case eitDiscard:
	default:
		description.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	}
	
	switch(out){
	case eotRender:
		description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		description.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		break;
		
	case eotShader:
		description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		description.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		break;
		
	case eotReadBack:
		description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		description.finalLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		break;
		
	case eotDisplay:
		description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		description.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		break;
		
	case eotDiscard:
	default:
		description.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		description.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL; // ?
	}
	
	pAttachments[index] = description;
}

void devkRenderPassConfiguration::SetClearValueAt(int index, const VkClearValue &clearValue){
	pClearValues[index] = clearValue;
}

void devkRenderPassConfiguration::SetClearValueColorFloatAt(int index, float red, float green, float blue, float alpha){
	VkClearValue clear{};
	clear.color.float32[0] = red;
	clear.color.float32[1] = green;
	clear.color.float32[2] = blue;
	clear.color.float32[3] = alpha;
	
	pClearValues[index] = clear;
}

void devkRenderPassConfiguration::SetClearValueColorIntAt(int index, int red, int green, int blue, int alpha){
	VkClearValue clear{};
	clear.color.int32[0] = (int32_t)red;
	clear.color.int32[1] = (int32_t)green;
	clear.color.int32[2] = (int32_t)blue;
	clear.color.int32[3] = (int32_t)alpha;
	
	pClearValues[index] = clear;
}

void devkRenderPassConfiguration::SetClearValueColorUIntAt(int index, int red, int green, int blue, int alpha){
	VkClearValue clear{};
	clear.color.uint32[0] = (uint32_t)red;
	clear.color.uint32[1] = (uint32_t)green;
	clear.color.uint32[2] = (uint32_t)blue;
	clear.color.uint32[3] = (uint32_t)alpha;
	
	pClearValues[index] = clear;
}

void devkRenderPassConfiguration::SetClearValueDepthAt(int index, float depth, int stencil){
	VkClearValue clear{};
	clear.depthStencil.depth = depth;
	clear.depthStencil.stencil = (uint32_t)stencil;
	
	pClearValues[index] = clear;
}



void devkRenderPassConfiguration::SetSubPassCount(int count){
	DEASSERT_TRUE(count >= 0)
	
	pSubPasses.SetAll(count, {});
}

void devkRenderPassConfiguration::SetSubPassAt(int index, const sSubPass &subpass){
	pSubPasses[index] = subpass;
}

void devkRenderPassConfiguration::SetSubPassAt(int index, int depthStencil, int color1,
int color2, int color3, int color4, int color5, int color6, int color7, int color8){
	sSubPass subPass{};
	subPass.depthStencilAttachment = depthStencil;
	subPass.colorAttachments[0] = color1;
	subPass.colorAttachments[1] = color2;
	subPass.colorAttachments[2] = color3;
	subPass.colorAttachments[3] = color4;
	subPass.colorAttachments[4] = color5;
	subPass.colorAttachments[5] = color6;
	subPass.colorAttachments[6] = color7;
	subPass.colorAttachments[7] = color8;
	
	pSubPasses[index] = subPass;
}



// Operators
//////////////

static bool operator==(const VkAttachmentDescription &a, const VkAttachmentDescription &b){
	return a.format == b.format
		&& a.samples == b.samples
		&& a.loadOp == b.loadOp
		&& a.storeOp == b.storeOp
		&& a.stencilLoadOp == b.stencilLoadOp
		&& a.stencilStoreOp == b.stencilStoreOp
		&& a.initialLayout == b.initialLayout
		&& a.finalLayout == b.finalLayout;
}

static bool operator==(const VkClearColorValue &a, const VkClearColorValue &b){
	return a.uint32[0] == b.uint32[0]
		&& a.uint32[1] == b.uint32[1]
		&& a.uint32[2] == b.uint32[2]
		&& a.uint32[3] == b.uint32[3];
}

static bool operator==(const VkClearDepthStencilValue &a, const VkClearDepthStencilValue &b){
	return a.depth == b.depth
		&& a.stencil == b.stencil;
}

static bool operator==(const VkClearValue &a, const VkClearValue &b){
	return a.color == b.color
		&& a.depthStencil == b.depthStencil;
}

bool devkRenderPassConfiguration::operator==(const devkRenderPassConfiguration &configuration) const{
	return pAttachments == configuration.pAttachments
		&& pClearValues == configuration.pClearValues
		&& pSubPasses == configuration.pSubPasses;
}

devkRenderPassConfiguration &devkRenderPassConfiguration::operator=(const devkRenderPassConfiguration &configuration){
	pAttachments = configuration.pAttachments;
	pClearValues = configuration.pClearValues;
	pSubPasses = configuration.pSubPasses;
	
	return *this;
}
