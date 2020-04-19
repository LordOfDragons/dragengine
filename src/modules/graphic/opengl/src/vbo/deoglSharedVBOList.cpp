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

#include "deoglSharedVBO.h"
#include "deoglSharedVBOList.h"
#include "deoglSharedVBOBlock.h"
#include "deoglVBOAttribute.h"

#include <dragengine/common/exceptions.h>



// Class deoglSharedVBOList
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSharedVBOList::deoglSharedVBOList( deoglRenderThread &renderThread,
const deoglVBOLayout &layout, GLenum drawType, int maxSize ) :
pRenderThread( renderThread ),
pDrawType( drawType )
{
	if( maxSize < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = layout.GetAttributeCount();
	int i;
	
	pMaxSize = maxSize - maxSize % layout.GetStride();
	pMaxPointCount = pMaxSize / layout.GetStride();
	
	pLayout.SetSize( 0 );
	pLayout.SetStride( layout.GetStride() );
	pLayout.SetAttributeCount( count );
	pLayout.SetIndexType( layout.GetIndexType() );
	
	for( i=0; i<count; i++ ){
		const deoglVBOAttribute &attrSrc = layout.GetAttributeAt( i );
		deoglVBOAttribute &attrDest = pLayout.GetAttributeAt( i );
		
		attrDest.SetComponentCount( attrSrc.GetComponentCount() );
		attrDest.SetDataType( attrSrc.GetDataType() );
		attrDest.SetOffset( attrSrc.GetOffset() );
	}
}

deoglSharedVBOList::~deoglSharedVBOList(){
	pVBOs.RemoveAll();
}



// Management
///////////////

bool deoglSharedVBOList::Matches( const deoglVBOLayout &layout, GLenum drawType ) const{
	return drawType == pDrawType && layout == pLayout;
}

int deoglSharedVBOList::GetCount() const{
	return pVBOs.GetCount();
}

deoglSharedVBO *deoglSharedVBOList::GetAt( int index ) const{
	return ( deoglSharedVBO* )pVBOs.GetAt( index );
}

deoglSharedVBOBlock *deoglSharedVBOList::AddData( int size ){
	if( size < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pVBOs.GetCount();
	deoglSharedVBOBlock *block;
	deoglSharedVBO *vbo;
	int i;
	
	// if there is a vbo able to host the data add it there
	for( i=0; i<count; i++ ){
		block = ( ( deoglSharedVBO* )pVBOs.GetAt( i ) )->AddBlock( size );
		
		if( block ){
			return block;
		}
	}
	
	// otherwise create a new vbo to add the data there
	vbo = NULL;
	
	try{
		if( size > pMaxPointCount ){
			// a little hack to deal with very large models. often they do not fit into the VBOs
			// we have so for this case we create an oversized VBO for the time being. has to be
			// made better later on
			vbo = new deoglSharedVBO( this, size );
			
		}else{
			vbo = new deoglSharedVBO( this, pMaxPointCount );
		}
		
		pVBOs.Add( vbo );
		vbo->FreeReference();
		
	}catch( const deException & ){
		if( vbo ){
			vbo->FreeReference();
		}
		throw;
	}
	
	block = ( ( deoglSharedVBO* )pVBOs.GetAt( pVBOs.GetCount() - 1 ) )->AddBlock( size );
	if( ! block ){
		DETHROW( deeInvalidAction );
	}
	
	return block;
}

deoglSharedVBOBlock *deoglSharedVBOList::AddData( int size, int indexCount ){
	if( size < 1 || indexCount < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pVBOs.GetCount();
	deoglSharedVBOBlock *block;
	deoglSharedVBO *vbo;
	int i;
	
	// if there is a vbo able to host the data add it there
	for( i=0; i<count; i++ ){
		block = ( ( deoglSharedVBO* )pVBOs.GetAt( i ) )->AddBlock( size, indexCount );
		
		if( block ){
			return block;
		}
	}
	
	// otherwise create a new vbo to add the data there
	vbo = NULL;
	
	try{
		if( size > pMaxPointCount ){
			// a little hack to deal with very large models. often they do not fit into the VBOs
			// we have so for this case we create an oversized VBO for the time being. has to be
			// made better later on
			vbo = new deoglSharedVBO( this, size );
			
		}else{
			vbo = new deoglSharedVBO( this, pMaxPointCount );
		}
		
		pVBOs.Add( vbo );
		vbo->FreeReference();
		
	}catch( const deException & ){
		if( vbo ){
			vbo->FreeReference();
		}
		throw;
	}
	
	block = ( ( deoglSharedVBO* )pVBOs.GetAt( pVBOs.GetCount() - 1 ) )->AddBlock( size, indexCount );
	if( ! block ){
		DETHROW( deeInvalidAction );
	}
	
	return block;
}



void deoglSharedVBOList::PrepareVBOs(){
	const int count = pVBOs.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( deoglSharedVBO* )pVBOs.GetAt( i ) )->Prepare();
	}
}
