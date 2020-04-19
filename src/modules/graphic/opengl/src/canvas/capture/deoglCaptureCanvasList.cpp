/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
