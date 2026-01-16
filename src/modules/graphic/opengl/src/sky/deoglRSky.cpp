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
pRenderThread(renderThread)
{
	LEAK_CHECK_CREATE(renderThread, Sky);
}

deoglRSky::~deoglRSky(){
	pLayers.RemoveAll();
	pLinks.RemoveAll();
	
	LEAK_CHECK_FREE(pRenderThread, Sky);
}



// Management
///////////////

void deoglRSky::SetBgColor(const decColor &color){
	pBgColor = color;
}



const deoglRSkyLink &deoglRSky::GetLinkAt(int index) const{
	return pLinks.GetAt(index);
}

void deoglRSky::RebuildLinks(const deSky &sky){
	pLinks.RemoveAll();
	
	sky.GetLinks().Visit([&](const deSkyLink &l){
		pLinks.Add(deoglRSkyLink::Ref::New(l));
	});
}



deoglRSkyLayer &deoglRSky::GetLayerAt(int index) const{
	return pLayers.GetAt(index);
}

void deoglRSky::RebuildLayers(const deSky &sky){
	pLayers.RemoveAll();
	
	sky.GetLayers().Visit([&](const deSkyLayer &l){
		pLayers.Add(deoglRSkyLayer::Ref::New(l));
	});
}
