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

#include "deoglRSky.h"
#include "deoglRSkyLink.h"
#include "deoglRSkyLayer.h"
#include "../configuration/deoglConfiguration.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../deGraphicOpenGl.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/sky/deSky.h>
#include <dragengine/resources/sky/deSkyLayer.h>



// Class deoglRSky
////////////////////

// Constructor, destructor
////////////////////////////

deoglRSky::deoglRSky(deoglRenderThread &renderThread) :
pRenderThread(renderThread),
pLinks(NULL),
pLinkCount(0),
pLayers(NULL),
pLayerCount(0)
{
	LEAK_CHECK_CREATE(renderThread, Sky);
}

deoglRSky::~deoglRSky(){
	if(pLayers){
		while(pLayerCount > 0){
			delete pLayers[--pLayerCount];
		}
		delete [] pLayers;
	}
	
	if(pLinks){
		while(pLinkCount > 0){
			delete pLinks[--pLinkCount];
		}
		delete [] pLinks;
	}
	
	LEAK_CHECK_FREE(pRenderThread, Sky);
}



// Management
///////////////

void deoglRSky::SetBgColor(const decColor &color){
	pBgColor = color;
}



const deoglRSkyLink &deoglRSky::GetLinkAt(int index) const{
	if(index < 0 || index >= pLinkCount){
		DETHROW(deeInvalidParam);
	}
	return *pLinks[index];
}

void deoglRSky::RebuildLinks(const deSky &sky){
	if(pLinks){
		while(pLinkCount > 0){
			delete pLinks[--pLinkCount];
		}
		delete [] pLinks;
		pLinks = NULL;
	}
	
	const int linkCount = sky.GetLinkCount();
	if(linkCount == 0){
		return;
	}
	
	pLinks = new deoglRSkyLink*[linkCount];
	for(pLinkCount=0; pLinkCount<linkCount; pLinkCount++){
		pLinks[pLinkCount] = new deoglRSkyLink(sky.GetLinkAt(pLinkCount));
	}
}



deoglRSkyLayer &deoglRSky::GetLayerAt(int index) const{
	if(index < 0 || index >= pLayerCount){
		DETHROW(deeInvalidParam);
	}
	return *pLayers[index];
}

void deoglRSky::RebuildLayers(const deSky &sky){
	if(pLayers){
		while(pLayerCount > 0){
			delete pLayers[--pLayerCount];
		}
		delete [] pLayers;
		pLayers = NULL;
	}
	
	const int layerCount = sky.GetLayerCount();
	if(layerCount == 0){
		return;
	}
	
	pLayers = new deoglRSkyLayer*[layerCount];
	for(pLayerCount=0; pLayerCount<layerCount; pLayerCount++){
		pLayers[pLayerCount] = new deoglRSkyLayer(sky.GetLayerAt(pLayerCount));
	}
}
