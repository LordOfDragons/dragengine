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

#include "deoglSky.h"
#include "deoglRSky.h"
#include "../deGraphicOpenGl.h"
#include "../renderthread/deoglRenderThread.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/sky/deSky.h>
#include <dragengine/resources/sky/deSkyLayer.h>



// Class deoglSky
///////////////////

// Constructor, destructor
////////////////////////////

deoglSky::deoglSky(deGraphicOpenGl &ogl, const deSky &sky) :
pOgl(ogl),
pSky(sky),
pRSky(NULL),
pDirtyParameters(true),
pUpdateTracker(0)
{
	pRSky.TakeOverWith(pOgl.GetRenderThread());
}

deoglSky::~deoglSky(){
		pRSky = NULL;
	}
}



// Management
///////////////

void deoglSky::SyncToRender(){
	if(pDirtyParameters){
		pDirtyParameters = false;
		
		pRSky->SetBgColor(pSky.GetBgColor());
		pRSky->RebuildLinks(pSky);
		pRSky->RebuildLayers(pSky);
	}
}



// Notifications
//////////////////

void deoglSky::ParametersChanged(){
	if(pDirtyParameters){
		return;
	}
	
	pDirtyParameters = true;
	pUpdateTracker++;
}
