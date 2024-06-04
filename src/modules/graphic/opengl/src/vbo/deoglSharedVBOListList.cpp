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

#include "deoglSharedVBOList.h"
#include "deoglSharedVBOListList.h"
#include "deoglVBOAttribute.h"
#include "../renderthread/deoglRenderThread.h"

#include <dragengine/common/exceptions.h>



// Class deoglSharedVBOListList
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSharedVBOListList::deoglSharedVBOListList( deoglRenderThread &renderThread,
int maxSize, int maxIndexSize ) :
pRenderThread( renderThread ),
pMaxSize( maxSize ),
pMaxIndexSize( maxIndexSize )
{
	if( maxSize < 0 || maxIndexSize < 0 ){
		DETHROW( deeInvalidParam );
	}
}

deoglSharedVBOListList::~deoglSharedVBOListList(){
	const int count = pLists.GetCount();
	int i;
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
		if( ( ( deoglSharedVBOList* )pLists.GetAt( i ) )->Matches( layout, drawType ) ){
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
		list = new deoglSharedVBOList( pRenderThread, layout, drawType, pMaxSize, pMaxIndexSize );
		pLists.Add( list );
		
	}catch( const deException & ){
		if( list ){
			delete list;
		}
		throw;
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
