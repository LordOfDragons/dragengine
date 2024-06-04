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

deoglCaptureCanvasList::deoglCaptureCanvasList( deGraphicOpenGl &ogl ) :
pOgl( ogl ),
pCaptureCanvas( false ),
pDirty( false ){
}

deoglCaptureCanvasList::~deoglCaptureCanvasList(){
}



// Management
///////////////

int deoglCaptureCanvasList::GetCount() const{
	return pCaptureCanvas.GetCount();
}

deoglCaptureCanvas *deoglCaptureCanvasList::GetAt( int index ) const{
	return ( deoglCaptureCanvas* )pCaptureCanvas.GetAt( index );
}

void deoglCaptureCanvasList::Add( deoglCaptureCanvas *captureCanvas ){
	pCaptureCanvas.Add( captureCanvas );
	pDirty = true;
}

void deoglCaptureCanvasList::Remove( deoglCaptureCanvas *captureCanvas ){
	pCaptureCanvas.Remove( captureCanvas );
	pDirty = true;
}

void deoglCaptureCanvasList::RemoveAll(){
	pCaptureCanvas.RemoveAll();
	pDirty = true;
}



void deoglCaptureCanvasList::SyncToRender(){
	const int count = pCaptureCanvas.GetCount();
	int i;
	
	// sync to render
	for( i=0; i<count; i++ ){
		( ( deoglCaptureCanvas* )pCaptureCanvas.GetAt( i ) )->SyncToRender();
	}
	
	// if dirty synchronize list of capture canvas with render thread. this happens only a few times
	if( pDirty ){
		decObjectOrderedSet &list = pOgl.GetRenderThread().GetRCaptureCanvasList();
		
		list.RemoveAll();
		
		for( i=0; i<count; i++ ){
			list.Add( ( ( deoglCaptureCanvas* )pCaptureCanvas.GetAt( i ) )->GetRCaptureCanvas() );
		}
		
		pDirty = false;
	}
}
