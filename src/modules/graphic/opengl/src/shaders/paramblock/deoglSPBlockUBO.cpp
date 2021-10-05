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

#include "deoglSPBParameter.h"
#include "deoglSPBlockUBO.h"
#include "../deoglShaderCompiled.h"
#include "../../delayedoperation/deoglDelayedOperations.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>



// Class deoglSPBlockUBO
//////////////////////////

// Constructor, destructor
////////////////////////////

deoglSPBlockUBO::deoglSPBlockUBO( deoglRenderThread &renderThread ) :
deoglShaderParameterBlock( renderThread ),
pUBO( 0 ),
pBindingPoint( 0 ),
pAllocateBuffer( true ),
pWriteBuffer( NULL ),
pWriteBufferCapacity( 0 ),
pWriteBufferUsed( false ){
}

deoglSPBlockUBO::deoglSPBlockUBO( const deoglSPBlockUBO &paramBlock ) :
deoglShaderParameterBlock( paramBlock ),
pUBO( 0 ),
pBindingPoint( paramBlock.pBindingPoint ),
pAllocateBuffer( true ),
pWriteBuffer( NULL ),
pWriteBufferCapacity( 0 ),
pWriteBufferUsed( false ){
}

deoglSPBlockUBO::~deoglSPBlockUBO(){
	if( IsBufferMapped() ){
		pClearMapped(); // done by delete
	}
	GetRenderThread().GetDelayedOperations().DeleteOpenGLBuffer( pUBO );
	if( pWriteBuffer ){
		delete [] pWriteBuffer;
	}
}



// Management
///////////////

void deoglSPBlockUBO::SetBindingPoint( int bindingPoint ){
	pBindingPoint = bindingPoint;
}

void deoglSPBlockUBO::Activate() const{
	if( ! pUBO || IsBufferMapped() ){
		DETHROW( deeInvalidParam );
	}
	
	OGL_CHECK( GetRenderThread(), pglBindBufferBase( GL_UNIFORM_BUFFER, pBindingPoint, pUBO ) );
}

void deoglSPBlockUBO::Activate( int bindingPoint ) const{
	if( ! pUBO || IsBufferMapped() ){
		DETHROW( deeInvalidParam );
	}
	
	OGL_CHECK( GetRenderThread(), pglBindBufferBase( GL_UNIFORM_BUFFER, bindingPoint, pUBO ) );
}

void deoglSPBlockUBO::Deactivate() const{
	OGL_CHECK( GetRenderThread(), pglBindBufferBase( GL_UNIFORM_BUFFER, pBindingPoint, 0 ) );
}

void deoglSPBlockUBO::Deactivate( int bindingPoint ) const{
	OGL_CHECK( GetRenderThread(), pglBindBufferBase( GL_UNIFORM_BUFFER, bindingPoint, 0 ) );
}

void deoglSPBlockUBO::MapBuffer(){
	if( IsBufferMapped() || GetBufferSize() == 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( false ){ // use mapped
		if( ! pUBO ){
			OGL_CHECK( GetRenderThread(), pglGenBuffers( 1, &pUBO ) );
			if( ! pUBO ){
				DETHROW( deeOutOfMemory );
			}
		}
		
		OGL_CHECK( GetRenderThread(), pglBindBuffer( GL_UNIFORM_BUFFER, pUBO ) );
		OGL_CHECK( GetRenderThread(), pglBufferData( GL_UNIFORM_BUFFER,
			GetBufferSize(), NULL, GL_DYNAMIC_DRAW ) );
				// clear using old clear method
		pAllocateBuffer = false;
		
		// NOTE glMapBuffer() does not exist on EGL. glMapBufferRange() though fails to run if
		//      used from here. no idea how this is supposed to work with glMapBuffer() not
		//      working. furthermore glBufferData() seems faster than glMapBuffer*()
		try{
			char *data;
			
			if( pglMapBuffer ){
				OGL_CHECK( GetRenderThread(), data = ( char* )pglMapBuffer(
					GL_UNIFORM_BUFFER, GL_WRITE_ONLY ) );
				
			}else{
				OGL_CHECK( GetRenderThread(), data = ( char* )pglMapBufferRange( GL_UNIFORM_BUFFER,
					0, GetBufferSize(), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT ) );
			}
			
			if( ! data ){
				DETHROW( deeInvalidParam );
			}
			pSetMapped( data );
			
		}catch( const deException & ){
			OGL_CHECK( GetRenderThread(), pglBindBuffer( GL_UNIFORM_BUFFER, 0 ) );
			throw;
		}
		
	}else{
		pGrowWriteBuffer( GetBufferSize() );
		pWriteBufferUsed = true;
		pSetMapped( pWriteBuffer );
	}
}

void deoglSPBlockUBO::MapBuffer( int element ){
	if( IsBufferMapped() || GetBufferSize() == 0 || element < 0 || element >= GetElementCount() ){
		DETHROW( deeInvalidParam );
	}
	
	if( false ){ // use mapped
		if( ! pUBO ){
			OGL_CHECK( GetRenderThread(), pglGenBuffers( 1, &pUBO ) );
			if( ! pUBO ){
				DETHROW( deeOutOfMemory );
			}
			pAllocateBuffer = true;
		}
		
		OGL_CHECK( GetRenderThread(), pglBindBuffer( GL_UNIFORM_BUFFER, pUBO ) );
		
		if( pAllocateBuffer ){
			OGL_CHECK( GetRenderThread(), pglBufferData( GL_UNIFORM_BUFFER,
				GetBufferSize(), NULL, GL_DYNAMIC_DRAW ) );
			pAllocateBuffer = false;
		}
		
		try{
			char *data;
			
			OGL_CHECK( GetRenderThread(), data = ( char* )pglMapBufferRange( GL_UNIFORM_BUFFER,
				GetElementStride() * element, GetElementStride(),
				GL_WRITE_ONLY | GL_MAP_INVALIDATE_RANGE_BIT ) );
			
			if( ! data ){
				DETHROW( deeInvalidParam );
			}
			pSetMapped( data, element );
			
		}catch( const deException & ){
			OGL_CHECK( GetRenderThread(), pglBindBuffer( GL_UNIFORM_BUFFER, 0 ) );
			throw;
		}
		
	}else{
		pGrowWriteBuffer( GetElementStride() );
		pWriteBufferUsed = true;
		pSetMapped( pWriteBuffer, element );
	}
}

void deoglSPBlockUBO::UnmapBuffer(){
	if( ! IsBufferMapped() ){
		DETHROW( deeInvalidParam );
	}
	
	if( pWriteBufferUsed ){
		OGL_IF_CHECK( deoglRenderThread &renderThread = GetRenderThread(); )
		
		if( ! pUBO ){
			OGL_CHECK( renderThread, pglGenBuffers( 1, &pUBO ) );
			if( ! pUBO ){
				DETHROW( deeOutOfMemory );
			}
			pAllocateBuffer = true;
		}
		
		const int stride = GetElementStride();
		const int lower = pGetElementLower();
		const int upper = pGetElementUpper();
		
		if( pAllocateBuffer ){
			OGL_CHECK( renderThread, pglBindBuffer( GL_UNIFORM_BUFFER, pUBO ) );
			OGL_CHECK( renderThread, pglBufferData( GL_UNIFORM_BUFFER,
				GetBufferSize(), NULL, GL_DYNAMIC_DRAW ) );
			pAllocateBuffer = false;
			
		}else{
			if( pglBindBufferRange ){
				OGL_CHECK( renderThread, pglBindBufferRange( GL_UNIFORM_BUFFER, 0, pUBO,
					stride * lower, stride * ( upper - lower + 1 ) ) );
				
			}else{
				OGL_CHECK( renderThread, pglBindBuffer( GL_UNIFORM_BUFFER, pUBO ) );
			}
		}
		
		OGL_CHECK( renderThread, pglBufferSubData( GL_UNIFORM_BUFFER,
			stride * lower, stride * ( upper - lower + 1 ), pWriteBuffer ) );
		
		pWriteBufferUsed = false;
		
	}else{
		OGL_CHECK( GetRenderThread(), pglBindBuffer( GL_UNIFORM_BUFFER, pUBO ) );
		OGL_CHECK( GetRenderThread(), pglUnmapBuffer( GL_UNIFORM_BUFFER ) );
	}
	
	OGL_CHECK( GetRenderThread(), pglBindBuffer( GL_UNIFORM_BUFFER, 0 ) );
	
	pClearMapped();
}



void deoglSPBlockUBO::DebugPrintConfig( const char *name ){
	const char * const valueTypeNames[ 3 ] = { "float", "int", "bool" };
	int i;
	
	GetRenderThread().GetLogger().LogInfoFormat( "Shader Parameter Block '%s' (%p):"
		" parameters=%d ubo=%u binding=%u elementCount=%d elementStride=%d"
		" offsetPadding=%d size=%d %s", name, this, GetParameterCount(), pUBO, pBindingPoint,
		GetElementCount(), GetElementStride(), GetOffsetPadding(), GetBufferSize(),
		GetRowMajor() ? "rowMajor" : "colMajor" );
	
	for( i=0; i<GetParameterCount(); i++ ){
		const deoglSPBParameter &parameter = GetParameterAt( i );
		GetRenderThread().GetLogger().LogInfoFormat( "- Parameter %i: %s%ix%i[%i]"
			" offset=%u stride=%u arraystride=%i size=%u", i,
				valueTypeNames[ parameter.GetValueType() ],
				parameter.GetComponentCount(), parameter.GetVectorCount(),
				parameter.GetArrayCount(), parameter.GetOffset(),
				parameter.GetStride(), parameter.GetArrayStride(),
				parameter.GetDataSize() );
	}
}



// Protected Functions
////////////////////////

void deoglSPBlockUBO::pUpdateBufferSize(){
	deoglShaderParameterBlock::pUpdateBufferSize();
	pAllocateBuffer = true;
}



// Private Functions
//////////////////////

void deoglSPBlockUBO::pGrowWriteBuffer( int size ){
	if( size <= pWriteBufferCapacity ){
		return;
	}
	
	char * const newBuffer = new char[ size ];
	
	if( pWriteBuffer ){
		if( pWriteBufferCapacity > 0 ){
			memcpy( newBuffer, pWriteBuffer, pWriteBufferCapacity );
		}
		
		delete [] pWriteBuffer;
		pWriteBuffer = NULL;
		pWriteBufferCapacity = 0;
	}
	
	pWriteBuffer = newBuffer;
	pWriteBufferCapacity = size;
}
