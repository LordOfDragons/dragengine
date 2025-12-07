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

#include <dragengine/common/math/decMath.h>

#include "deoglMemoryConsumptionTexture.h"
#include "deoglMemoryConsumptionTextureUse.h"
#ifdef BACKEND_OPENGL
#include "../../capabilities/deoglCapsTextureFormat.h"
#elif defined BACKEND_VULKAN
#include <devkFormat.h>
#endif


// Class deoglMemoryConsumptionTextureUse
///////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglMemoryConsumptionTextureUse::deoglMemoryConsumptionTextureUse(deoglMemoryConsumptionTexture &tracker) :
colorCompressed(tracker.colorCompressed),
colorUncompressed(tracker.colorUncompressed),
depthCompressed(tracker.depthCompressed),
depthUncompressed(tracker.depthUncompressed){
}

deoglMemoryConsumptionTextureUse::~deoglMemoryConsumptionTextureUse(){
}



// Management
///////////////

unsigned long long deoglMemoryConsumptionTextureUse::Total() const{
	return TotalCompressed() + TotalUncompressed();
}

unsigned long long deoglMemoryConsumptionTextureUse::TotalCompressed() const{
	return colorCompressed.GetConsumption() + depthCompressed.GetConsumption();
}

unsigned long long deoglMemoryConsumptionTextureUse::TotalUncompressed() const{
	return colorUncompressed.GetConsumption() + depthUncompressed.GetConsumption();
}

void deoglMemoryConsumptionTextureUse::Clear(){
	colorCompressed = 0;
	colorUncompressed = 0;
	depthCompressed = 0;
	depthUncompressed = 0;
}

void deoglMemoryConsumptionTextureUse::Set(unsigned long long consumption, bool depth, bool compressed){
	Clear();
	
	if(depth){
		if(compressed){
			depthCompressed = consumption;
			
		}else{
			depthUncompressed = consumption;
		}
		
	}else{
		if(compressed){
			colorCompressed = consumption;
			
		}else{
			colorUncompressed = consumption;
		}
	}
}

#ifdef BACKEND_OPENGL
void deoglMemoryConsumptionTextureUse::SetCompressed(unsigned long long consumption,
const deoglCapsTextureFormat& format){
	Set(consumption, format.GetIsDepth() || format.GetIsDepthFloat(), true);
}

void deoglMemoryConsumptionTextureUse::SetUncompressed(const deoglCapsTextureFormat &format,
int width, int height, int depth, int mipMapLevels){
	unsigned long long consumption = BaseConsumption(format, width, height, depth);
	if(mipMapLevels > 0){
		consumption = MipMappedConsumption(mipMapLevels, width, height, consumption);
	}
	Set(consumption, format.GetIsDepth() || format.GetIsDepthFloat(), false);
}

unsigned long long deoglMemoryConsumptionTextureUse::BaseConsumption(
const deoglCapsTextureFormat &format, int width, int height, int depth) const{
	unsigned long long consumption = (unsigned long long)width
		* ( unsigned long long )height
		* ( unsigned long long )depth;
	
	consumption *= (unsigned long long)(format.GetBitsPerPixel() >> 3);
	if((format.GetBitsPerPixel() & 0x7) > 0){
		consumption >>= 1ull;
	}
	
	return consumption;
}

#elif defined BACKEND_VULKAN
void deoglMemoryConsumptionTextureUse::SetCompressed(
unsigned long long consumption, const devkFormat &format){
	Set(consumption, format.GetIsDepth() || format.GetIsDepthFloat(), true);
}

void deoglMemoryConsumptionTextureUse::SetUncompressed(const devkFormat &format,
int width, int height, int depth, int mipMapLevels){
	unsigned long long consumption = BaseConsumption(format, width, height, depth);
	if(mipMapLevels > 0){
		consumption = MipMappedConsumption(mipMapLevels, width, height, consumption);
	}
	Set(consumption, format.GetIsDepth() || format.GetIsDepthFloat(), false);
}

unsigned long long deoglMemoryConsumptionTextureUse::BaseConsumption(const devkFormat &format,
int width, int height, int depth) const{
	unsigned long long consumption = (unsigned long long)width
		* (unsigned long long)height * (unsigned long long)depth;
	
	consumption *= (unsigned long long)(format.GetBitsPerPixel() >> 3);
	if((format.GetBitsPerPixel() & 0x7) > 0){
		consumption >>= 1ull;
	}
	
	return consumption;
}
#endif

unsigned long long deoglMemoryConsumptionTextureUse::MipMappedConsumption(
int levels, int width, int height, unsigned long long baseConsumption) const{
	int i;
	
	for(i=0; i<levels; i++){
		width = decMath::max(width >> 1, 1);
		height = decMath::max(height >> 1, 1);
		
		baseConsumption += (unsigned long long)width * (unsigned long long)height;
	}
	
	return baseConsumption;
}
