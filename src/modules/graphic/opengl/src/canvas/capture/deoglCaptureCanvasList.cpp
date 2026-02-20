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

#include "deoglCaptureCanvas.h"
#include "deoglCaptureCanvasList.h"
#include "deoglRCaptureCanvas.h"
#include "../../deGraphicOpenGl.h"
#include "../../renderthread/deoglRenderThread.h"

#include <dragengine/common/exceptions.h>



// Class deoglCaptureCanvasList
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglCaptureCanvasList::deoglCaptureCanvasList(deGraphicOpenGl &ogl) :
pOgl(ogl),
pDirty(false){
}

deoglCaptureCanvasList::~deoglCaptureCanvasList(){
}



// Management
///////////////

void deoglCaptureCanvasList::Add(deoglCaptureCanvas *captureCanvas){
	pCaptureCanvas.AddOrThrow(captureCanvas);
	pDirty = true;
}

void deoglCaptureCanvasList::Remove(deoglCaptureCanvas *captureCanvas){
	pCaptureCanvas.RemoveOrThrow(captureCanvas);
	pDirty = true;
}

void deoglCaptureCanvasList::RemoveAll(){
	if(pCaptureCanvas.IsEmpty()){
		return;
	}
	
	pCaptureCanvas.RemoveAll();
	pDirty = true;
}



void deoglCaptureCanvasList::SyncToRender(){
	// sync to render
	pCaptureCanvas.Visit([](deoglCaptureCanvas *captureCanvas){
		captureCanvas->SyncToRender();
	});
	
	// if dirty synchronize list of capture canvas with render thread. this happens only a few times
	if(pDirty){
		decTObjectOrderedSet<deoglRCaptureCanvas> &list = pOgl.GetRenderThread().GetRCaptureCanvasList();
		
		list.RemoveAll();
		pCaptureCanvas.Visit([&](const deoglCaptureCanvas *captureCanvas){
			list.Add(captureCanvas->GetRCaptureCanvas());
		});
		
		pDirty = false;
	}
}
