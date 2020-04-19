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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglSharedVBOList.h"
#include "deoglSharedVBOListList.h"
#include "deoglVBOAttribute.h"
#include "../renderthread/deoglRenderThread.h"

#include <dragengine/common/exceptions.h>



// Class deoglSharedVBOListList
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSharedVBOListList::deoglSharedVBOListList( deoglRenderThread &renderThread, int maxSize ) :
pRenderThread( renderThread ),
pMaxSize( maxSize ){
	if( maxSize < 0 ){
		DETHROW( deeInvalidParam );
	}
}

deoglSharedVBOListList::~deoglSharedVBOListList(){
	int i, count;
	
	count = pLists.GetCount();
	for( i=0; i<count; i++ ){
		delete ( ( deoglSharedVBOList* )pLists.GetAt( i ) );
	}
	pLists.RemoveAll();
}



// Management
///////////////

int deoglSharedVBOListList::GetCount() const{
	return pLists.GetCount();
}

deoglSharedVBOList *deoglSharedVBOListList::GetAt( int index ) const{
	return ( deoglSharedVBOList* )pLists.GetAt( index );
}

bool deoglSharedVBOListList::HasWith( const deoglVBOLayout &layout, GLenum drawType ) const{
	const int count = pLists.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const deoglSharedVBOList &list = *( ( deoglSharedVBOList* )pLists.GetAt( i ) );
		
		if( list.Matches( layout, drawType ) ){
			return true;
		}
	}
	
	return false;
}

deoglSharedVBOList *deoglSharedVBOListList::GetWith( const deoglVBOLayout &layout, GLenum drawType ){
	const int count = pLists.GetCount();
	deoglSharedVBOList *list;
	int i;
	
	for( i=0; i<count; i++ ){
		list = ( deoglSharedVBOList* )pLists.GetAt( i );
		
		if( list->Matches( layout, drawType ) ){
			return list;
		}
	}
	
	list = NULL;
	
	try{
		list = new deoglSharedVBOList( pRenderThread, layout, drawType, pMaxSize );
		pLists.Add( list );
		
	}catch( const deException & ){
		if( list ){
			delete list;
		}
	}
	
	return list;
}



void deoglSharedVBOListList::PrepareAllLists(){
	const int count = pLists.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( deoglSharedVBOList* )pLists.GetAt( i ) )->PrepareVBOs();
	}
}
