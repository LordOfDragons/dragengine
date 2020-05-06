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
#include <stdint.h>

#include "deoglVBOAttribute.h"
#include "../deoglBasics.h"

#include <dragengine/common/exceptions.h>



// Class deoglVBOAttribute
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglVBOAttribute::deoglVBOAttribute() :
pComponentCount( 0 ),
pDataType( edtFloat ),
pOffset( 0 ){
}

deoglVBOAttribute::~deoglVBOAttribute(){
}



// Management
///////////////

void deoglVBOAttribute::SetComponentCount( int componentCount ){
	if( componentCount < 0 || componentCount > 4 ){
		DETHROW( deeInvalidParam );
	}
	pComponentCount = componentCount;
}

void deoglVBOAttribute::SetDataType( eDataTypes dataType ){
	pDataType = dataType;
}

void deoglVBOAttribute::SetOffset( int offset ){
	if( offset < 0 ){
		DETHROW( deeInvalidParam );
	}
	pOffset = offset;
}



void deoglVBOAttribute::SetVAOAttributeAt( deoglRenderThread &renderThread, int attribute, int stride ) const{
	SetVAOAttributeAt( renderThread, attribute, stride, 0 );
}

void deoglVBOAttribute::SetVAOAttributeAt( deoglRenderThread &renderThread, int attribute, int stride, int offset ) const{
	if( attribute < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pComponentCount == 0 ){
		return;
	}
	
	const GLvoid * const pointer = ( const GLvoid * )( intptr_t )( offset + ( int )pOffset );
	
	OGL_CHECK( renderThread, pglEnableVertexAttribArray( attribute ) );
	
	switch( pDataType ){
	case edtFloat:
		OGL_CHECK( renderThread, pglVertexAttribPointer( attribute,
			pComponentCount, GL_FLOAT, GL_FALSE, stride, pointer ) );
		break;
		
	case edtByte:
		OGL_CHECK( renderThread, pglVertexAttribPointer( attribute,
			pComponentCount, GL_BYTE, GL_TRUE, stride, pointer ) );
		break;
		
	case edtUByte:
		OGL_CHECK( renderThread, pglVertexAttribPointer( attribute,
			pComponentCount, GL_UNSIGNED_BYTE, GL_TRUE, stride, pointer ) );
		break;
		
	case edtShort:
		OGL_CHECK( renderThread, pglVertexAttribPointer( attribute,
			pComponentCount, GL_SHORT, GL_TRUE, stride, pointer ) );
		break;
		
	case edtUShort:
		OGL_CHECK( renderThread, pglVertexAttribPointer( attribute,
			pComponentCount, GL_UNSIGNED_SHORT, GL_TRUE, stride, pointer ) );
		break;
		
	case edtInt:
		OGL_CHECK( renderThread, pglVertexAttribPointer( attribute,
			pComponentCount, GL_INT, GL_TRUE, stride, pointer ) );
		break;
		
	case edtUInt:
		OGL_CHECK( renderThread, pglVertexAttribPointer( attribute,
			pComponentCount, GL_UNSIGNED_INT, GL_TRUE, stride, pointer ) );
		break;
		
	case edtIByte:
		OGL_CHECK( renderThread, pglVertexAttribIPointer( attribute,
			pComponentCount, GL_BYTE, stride, pointer ) );
		break;
		
	case edtIUByte:
		OGL_CHECK( renderThread, pglVertexAttribIPointer( attribute,
			pComponentCount, GL_UNSIGNED_BYTE, stride, pointer ) );
		break;
		
	case edtIShort:
		OGL_CHECK( renderThread, pglVertexAttribIPointer( attribute,
			pComponentCount, GL_SHORT, stride, pointer ) );
		break;
		
	case edtIUShort:
		OGL_CHECK( renderThread, pglVertexAttribIPointer( attribute,
			pComponentCount, GL_UNSIGNED_SHORT, stride, pointer ) );
		break;
		
	case edtIInt:
		OGL_CHECK( renderThread, pglVertexAttribIPointer( attribute,
			pComponentCount, GL_INT, stride, pointer ) );
		break;
		
	case edtIUInt:
		OGL_CHECK( renderThread, pglVertexAttribIPointer( attribute,
			pComponentCount, GL_UNSIGNED_INT, stride, pointer ) );
		break;
		
	default:
		DETHROW( deeInvalidParam );
	}
}



// Operators
//////////////

deoglVBOAttribute &deoglVBOAttribute::operator=( const deoglVBOAttribute &attribute ){
	pComponentCount = attribute.pComponentCount;
	pDataType = attribute.pDataType;
	pOffset = attribute.pOffset;
	return *this;
}

bool deoglVBOAttribute::operator==( const deoglVBOAttribute &attribute ) const{
	return pComponentCount == attribute.pComponentCount
		&& pDataType == attribute.pDataType
		&& pOffset == attribute.pOffset;
}

bool deoglVBOAttribute::operator!=( const deoglVBOAttribute &attribute ) const{
	return pComponentCount != attribute.pComponentCount
		|| pDataType != attribute.pDataType
		|| pOffset != attribute.pOffset;
}
