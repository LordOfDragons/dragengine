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
#include "deoglGITraceRays.h"
#include "../capabilities/deoglCapabilities.h"
#include "../configuration/deoglConfiguration.h"
#include "../framebuffer/deoglRestoreFramebuffer.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTFramebuffer.h"

#include <dragengine/common/exceptions.h>


// Class deoglGITraceRays
//////////////////////

// Constructor, destructor
////////////////////////////

deoglGITraceRays::deoglGITraceRays(deoglRenderThread &renderThread) :
pRenderThread(renderThread),
pRaysPerProbe(ConfigRaysPerProbe(renderThread.GetConfiguration())),
pProbesPerLine(8),
pProbeCount(ConfigProbeCount(renderThread.GetConfiguration())),
pTexPosition(renderThread),
pTexNormal(renderThread),
pTexDiffuse(renderThread),
pTexReflectivity(renderThread),
pTexLight(renderThread),
pFBOLight(deoglFramebuffer::Ref::NewWith(renderThread, false))
{
	try{
		pCreateFBORay();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoglGITraceRays::~deoglGITraceRays(){
	pCleanUp();
}



// Management
///////////////

int deoglGITraceRays::ConfigRaysPerProbe(const deoglConfiguration &config){
	switch(config.GetGIQuality()){
	case deoglConfiguration::egiqVeryHigh:
		return 256;
		
	case deoglConfiguration::egiqHigh:
	default:
		return 128;
		// return 192;
		
	case deoglConfiguration::egiqMedium:
		return 64;
		// return 192;
		
	case deoglConfiguration::egiqLow:
		return 32;
		// return 128;
		
	case deoglConfiguration::egiqVeryLow:
		return 16;
		// return 128;
	}
}

int deoglGITraceRays::ConfigProbeCount(const deoglConfiguration &config){
	switch(config.GetGIUpdateSpeed()){
	case deoglConfiguration::egiusVeryHigh:
		return 2048;
		
	case deoglConfiguration::egiusHigh:
		return 1024;
		
	case deoglConfiguration::egiusMedium:
	default:
		return 512;
		
	case deoglConfiguration::egiusLow:
		return 256;
		
	case deoglConfiguration::egiusVeryLow:
		return 128;
	}
}

void deoglGITraceRays::UpdateFromConfig(){
	const int raysPerProbe = ConfigRaysPerProbe(pRenderThread.GetConfiguration());
	const int probeCount = ConfigProbeCount(pRenderThread.GetConfiguration());
	if(raysPerProbe == pRaysPerProbe && probeCount == pProbeCount){
		return;
	}
	
	pRaysPerProbe = raysPerProbe;
	pProbeCount = probeCount;
	
	pCreateFBORay();
}



// Private Functions
//////////////////////

void deoglGITraceRays::pCleanUp(){
}

void deoglGITraceRays::pCreateFBORay(){
	// memory consumption:
	// case 1: 64 rays per probe => 512x1024
	// case 2: 256 rays per probe => 2048x1024
	// 
	// position, normal and light: (36M, 9M) [35651584, 8912896]
	// diffuse and reflectivity: (15M, 4M) [14680064, 3670016]
	// 
	// total: (63M, 16M) [62914560, 15728640]
	//
	const deoglRestoreFramebuffer restoreFbo(pRenderThread);
	const GLenum buffers[5] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4};
	
	const int width = pProbesPerLine * pRaysPerProbe;
	const int height = GI_MAX_PROBE_COUNT / pProbesPerLine;
		// ^== can not use pProbeCount since GI cascades are allowed to temporarily use the
		//     maximum update probe count no matter what config option is set
	
	pTexPosition.SetFBOFormat(4, true);
	pTexPosition.SetSize(width, height);
	pTexPosition.CreateTexture();
	
	pTexNormal.SetFBOFormatSNorm(4, 8); // image load/store supports only 1, 2 and 4 not 3
	pTexNormal.SetSize(width, height);
	pTexNormal.CreateTexture();
	
	pTexDiffuse.SetFBOFormat(4, false); // image load/store supports only 1, 2 and 4 not 3
	pTexDiffuse.SetSize(width, height);
	pTexDiffuse.CreateTexture();
	
	pTexReflectivity.SetFBOFormat(4, false);
	pTexReflectivity.SetSize(width, height);
	pTexReflectivity.CreateTexture();
	
	pTexLight.SetFBOFormat(4, true); // image load/store supports only 1, 2 and 4 not 3
	pTexLight.SetSize(width, height);
	pTexLight.CreateTexture();
	
	pRenderThread.GetFramebuffer().Activate(pFBOLight);
	pFBOLight->DetachAllImages();
	pFBOLight->AttachColorTexture(0, &pTexLight);
	OGL_CHECK(pRenderThread, pglDrawBuffers(1, buffers));
	OGL_CHECK(pRenderThread, glReadBuffer(GL_COLOR_ATTACHMENT0));
	pFBOLight->Verify();
}
