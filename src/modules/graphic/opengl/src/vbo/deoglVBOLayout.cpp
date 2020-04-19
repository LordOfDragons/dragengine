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

#include "deoglVBOLayout.h"
#include "deoglVBOAttribute.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"

#include <dragengine/common/exceptions.h>



// Class deoglVBOLayout
/////////////////////////

// Constructor, destructor
////////////////////////////

deoglVBOLayout::deoglVBOLayout(){
	pSize = 0;
	pStride = 0;
	pAttributes = NULL;
	pAttributeCount = 0;
	pIndexType = eitNone;
	pIndexSize = 0;
	pIndexGLType = GL_NONE;
}

deoglVBOLayout::~deoglVBOLayout(){
	if( pAttributes ){
		delete [] pAttributes;
	}
}



// Management
///////////////

void deoglVBOLayout::SetSize( int size ){
	if( size < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pSize = size;
}

void deoglVBOLayout::SetStride( int stride ){
	if( stride < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pStride = stride;
}

void deoglVBOLayout::SetIndexType( eIndexTypes indexType ){
	if( indexType < eitNone || indexType >= EIT_COUNT ){
		DETHROW( deeInvalidParam );
	}
	
	pIndexType = indexType;
	
	if( indexType == eitUnsignedInt ){
		pIndexSize = 4;
		pIndexGLType = GL_UNSIGNED_INT;
		
	}else if( indexType == eitUnsignedShort ){
		pIndexSize = 2;
		pIndexGLType = GL_UNSIGNED_SHORT;
		
	}else if( indexType == eitUnsignedByte ){
		pIndexSize = 1;
		pIndexGLType = GL_UNSIGNED_BYTE;
		
	}else{
		// the values here are chosen this way to prevent a crash if by mistake indices are used although
		// eitNone has been specified in the layout. an index size of 0 causes allocation to fail should
		// a check be missing for pIndexSize equal to 0. also GL_NONE causes rendering to fail with an
		// opengl error instead of crashing
		pIndexSize = 0;
		pIndexGLType = GL_NONE;
	}
}



// Attributes
///////////////

void deoglVBOLayout::SetAttributeCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count != pAttributeCount ){
		if( pAttributes ){
			delete [] pAttributes;
			pAttributes = NULL;
			pAttributeCount = 0;
		}
		
		if( count > 0 ){
			pAttributes = new deoglVBOAttribute[ count ];
			pAttributeCount = count;
		}
	}
}

deoglVBOAttribute& deoglVBOLayout::GetAttributeAt( int index ) const{
	if( index < 0 || index >= pAttributeCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pAttributes[ index ];
}

void deoglVBOLayout::SetVAOAttributeAt( deoglRenderThread &renderThread, int attribute, int target ) const{
	GetAttributeAt( attribute ).SetVAOAttributeAt( renderThread, target, pStride );
}

void deoglVBOLayout::SetVAOAttributeAt( deoglRenderThread &renderThread, int attribute, int target, int offset ) const{
	GetAttributeAt( attribute ).SetVAOAttributeAt( renderThread, target, pStride, offset );
}



// Operators
//////////////

bool deoglVBOLayout::operator==( const deoglVBOLayout &layout ) const{
	if( layout.GetStride() != pStride || layout.GetAttributeCount() != pAttributeCount ){
		return false;
	}
	
	int i;
	
	for( i=0; i<pAttributeCount; i++ ){
		if( layout.GetAttributeAt( i ) != pAttributes[ i ] ){
			return false;
		}
	}
	
	return true;
}

bool deoglVBOLayout::operator!=( const deoglVBOLayout &layout ) const{
	if( layout.GetStride() != pStride || layout.GetAttributeCount() != pAttributeCount ){
		return true;
	}
	
	int i;
	
	for( i=0; i<pAttributeCount; i++ ){
		if( layout.GetAttributeAt( i ) != pAttributes[ i ] ){
			return true;
		}
	}
	
	return false;
}



// Debugging
//////////////

void deoglVBOLayout::PrintToConsole( deoglRenderThread &renderThread, const char* name ){
	const char * const typeNames[] = {
		"float", "byte", "ubyte", "short", "ushort", "int", "uint",
		"ibyte", "iubyte", "ishort", "iushort", "iint", "iuint" };
	const char * const indexTypeNames[] = { "none", "uint", "ushort", "ubyte" };
	int a;
	
	renderThread.GetLogger().LogInfoFormat( "%s VBO Layout: size=%i stride=%i", name, pSize, pStride );
	
	for( a=0; a<pAttributeCount; a++ ){
		renderThread.GetLogger().LogInfoFormat( "- attribute %i: components=%i type=%s offset=%i indexType=%s indexSize=%i",
			a, pAttributes[ a ].GetComponentCount(), typeNames[ pAttributes[ a ].GetDataType() ],
			pAttributes[ a ].GetOffset(), indexTypeNames[ pIndexType ], pIndexSize );
	}
}
