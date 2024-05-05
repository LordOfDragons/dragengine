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

#include "deoglRenderWindow.h"
#include "deoglRenderWindowList.h"
#include "deoglRRenderWindow.h"
#include "../deGraphicOpenGl.h"
#include "../renderthread/deoglRenderThread.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderWindowList
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderWindowList::deoglRenderWindowList( deGraphicOpenGl &ogl ) :
pOgl( ogl ),
pDirty( false ){
}

deoglRenderWindowList::~deoglRenderWindowList(){
}



// Management
///////////////

int deoglRenderWindowList::GetCount() const{
	return pWindows.GetCount();
}

deoglRenderWindow *deoglRenderWindowList::GetAt( int index ) const{
	return ( deoglRenderWindow* )pWindows.GetAt( index );
}

void deoglRenderWindowList::Add( deoglRenderWindow *window ){
	pWindows.Add( window );
	pDirty = true;
}

void deoglRenderWindowList::Remove( deoglRenderWindow *window ){
	pWindows.Remove( window );
	pDirty = true;
}

void deoglRenderWindowList::RemoveAll(){
	pWindows.RemoveAll();
	pDirty = true;
}



void deoglRenderWindowList::SyncToRender(){
	const int count = pWindows.GetCount();
	int i;
	
	// if dirty synchronize list of windows with render thread. this happens only a few times
	if( pDirty ){
		decObjectOrderedSet &list = pOgl.GetRenderThread().GetRRenderWindowList();
		
		list.RemoveAll();
		
		for( i=0; i<count; i++ ){
			list.Add( ( ( deoglRenderWindow* )pWindows.GetAt( i ) )->GetRRenderWindow() );
		}
		
		pDirty = false;
	}
	
	// sync to render all windows if required
	for( i=0; i<count; i++ ){
		( ( deoglRenderWindow* )pWindows.GetAt( i ) )->SyncToRender();
	}
}
