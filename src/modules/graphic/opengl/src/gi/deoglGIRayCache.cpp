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

#include "deoglGI.h"
#include "deoglGIRayCache.h"
#include "../capabilities/deoglCapabilities.h"
#include "../renderthread/deoglRenderThread.h"
#include "../texture/pixelbuffer/deoglPixelBuffer.h"
#include "../utils/deoglConvertFloatHalf.h"

#include <dragengine/common/exceptions.h>


// Class deoglGIRayCache
//////////////////////

// Constructor, destructor
////////////////////////////

deoglGIRayCache::deoglGIRayCache(deoglRenderThread &renderThread,
	int raysPerProbe, int probeCount, int layerCount) :
pRenderThread(renderThread),
pRaysPerProbe(raysPerProbe),
pProbesPerLine(8),
pProbeCount(probeCount),
pLayerCount(layerCount),
pRayMapScale(1.0f, 1.0f),
pTexDistance(renderThread),
pTexNormal(renderThread),
pTexDiffuse(renderThread),
pTexReflectivity(renderThread),
pTexLight(renderThread)
{
	if(raysPerProbe < 16 || probeCount < 64 || layerCount < 1){
		DETHROW(deeInvalidParam);
	}
	
	try{
		pCreateFBO();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoglGIRayCache::~deoglGIRayCache(){
	pCleanUp();
}



// Management
///////////////

void deoglGIRayCache::SetRaysPerProbe(int raysPerProbe){
	if(raysPerProbe == pRaysPerProbe){
		return;
	}
	if(raysPerProbe < 16){
		DETHROW(deeInvalidParam);
	}
	
	pRaysPerProbe = raysPerProbe;
	
	pCreateFBO();
}

void deoglGIRayCache::SetProbeCount(int count){
	if(count == pProbeCount){
		return;
	}
	if(count < 64){
		DETHROW(deeInvalidParam);
	}
	
	pProbeCount = count;
	
	pCreateFBO();
}

void deoglGIRayCache::SetLayerCount(int count){
	if(count == pLayerCount){
		return;
	}
	if(count < 1){
		DETHROW(deeInvalidParam);
	}
	
	pLayerCount = count;
	
	pCreateFBO();
}



// Private Functions
//////////////////////

void deoglGIRayCache::pCleanUp(){
}

void deoglGIRayCache::pCreateFBO(){
	// memory consumption (assuming 8k probes):
	// case 1: 64 rays per probe => 512x1024
	// case 2: 256 rays per probe => 2048x1024
	// case 2: 512 rays per probe => 4096x1024
	// 
	// distance limit: (4MB, 1MB) [4194304, 1048576]
	// 
	// distance, normal and light: (23M, 6M) [23068672, 5767168]
	// diffuse and reflectivity: (15M, 4M) [14680064, 3670016]
	// total: (37M, 9M) [37748736, 9437184]
	// 
	const int width = pProbesPerLine * pRaysPerProbe;
	const int height = pProbeCount / pProbesPerLine;
	
	pRayMapScale.x = 1.0f / (float)width;
	pRayMapScale.y = 1.0f / (float)height;
	
	// create/resize textures
	#ifdef GI_USE_RAY_CACHE
		if(!pTexDistance.GetTexture()){
			if(pRenderThread.GetCapabilities().GetRestrictedImageBufferFormats()){
				//pTexDistance.SetFBOFormatIntegral(1, 16, true);
				pTexDistance.SetFBOFormat(4, true);
				
			}else{
				pTexDistance.SetFBOFormat(1, true);
			}
		}
		pTexDistance.SetSize(width, height, pLayerCount);
		pTexDistance.CreateTexture();
		
		if(! pTexNormal.GetTexture()){
			pTexNormal.SetFBOFormatSNorm(4, 8); // image load/store supports only 1, 2 and 4 not 3
		}
		pTexNormal.SetSize(width, height, pLayerCount);
		pTexNormal.CreateTexture();
		
		if(! pTexDiffuse.GetTexture()){
			pTexDiffuse.SetFBOFormat(4, false); // image load/store supports only 1, 2 and 4 not 3
		}
		pTexDiffuse.SetSize(width, height, pLayerCount);
		pTexDiffuse.CreateTexture();
		
		if(! pTexReflectivity.GetTexture()){
			pTexReflectivity.SetFBOFormat(4, false);
		}
		pTexReflectivity.SetSize(width, height, pLayerCount);
		pTexReflectivity.CreateTexture();
		
		if(! pTexLight.GetTexture()){
			pTexLight.SetFBOFormat(4, true); // image load/store supports only 1, 2 and 4 not 3
		}
		pTexLight.SetSize(width, height, pLayerCount);
		pTexLight.CreateTexture();
	#endif
	
	// clear textures
	#ifdef GI_USE_RAY_CACHE
		deoglPixelBuffer::Ref pixbuf;
		
		if(pRenderThread.GetCapabilities().GetRestrictedImageBufferFormats()){
			/*
			pixbuf.TakeOver(new deoglPixelBuffer(deoglPixelBuffer::epfInt1, width, height, pLayerCount));
			const HALF_FLOAT hfvalue = convertFloatToHalf(10000.0f);
			pixbuf->SetToUIntColor(hfvalue, 0, 0, 0);
			*/
			pixbuf.TakeOver(new deoglPixelBuffer(deoglPixelBuffer::epfFloat4, width, height, pLayerCount));
			pixbuf->SetToFloatColor(10000.0f, 10000.0f, 10000.0f, 10000.0f);
			
		}else{
			pixbuf.TakeOver(new deoglPixelBuffer(deoglPixelBuffer::epfFloat1, width, height, pLayerCount));
			pixbuf->SetToFloatColor(10000.0f, 10000.0f, 10000.0f, 10000.0f);
		}
		pTexDistance.SetPixels(pixbuf);
		
		pixbuf.TakeOver(new deoglPixelBuffer(deoglPixelBuffer::epfByte4, width, height, pLayerCount));
		pixbuf->SetToFloatColor(0.0f, 0.0f, 1.0f, 0.0f);
		pTexNormal.SetPixels(pixbuf);
		
		pixbuf->SetToFloatColor(1.0f, 1.0f, 1.0f, 0.0f);
		pTexDiffuse.SetPixels(pixbuf);
		
		pixbuf->SetToFloatColor(0.0f, 0.0f, 0.0f, 1.0f);
		pTexReflectivity.SetPixels(pixbuf);
		
		pixbuf.TakeOver(new deoglPixelBuffer(deoglPixelBuffer::epfFloat4, width, height, pLayerCount));
		pixbuf->SetToFloatColor(0.0f, 0.0f, 0.0f, 0.0f);
		pTexLight.SetPixels(pixbuf);
	#endif
}
