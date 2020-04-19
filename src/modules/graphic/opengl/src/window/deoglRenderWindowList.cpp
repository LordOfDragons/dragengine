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
