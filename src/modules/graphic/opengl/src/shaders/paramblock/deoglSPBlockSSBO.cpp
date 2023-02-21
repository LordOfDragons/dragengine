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

#include "deoglSPBlockSSBO.h"
#include "../deoglShaderCompiled.h"
#include "../../capabilities/deoglCapabilities.h"
#include "../../delayedoperation/deoglDelayedOperations.h"
#include "../../extensions/deoglExtensions.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>



// Class deoglSPBlockSSBO
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglSPBlockSSBO::deoglSPBlockSSBO( deoglRenderThread &renderThread ) :
deoglShaderParameterBlock( renderThread ),
pSSBO( 0 ),
pBindingPoint( 0 ),
pBindingPointUBO( 0 ),
pBindingPointAtomic( 0 ),
pCompact( true ),
pAllocateBuffer( true ),
pWriteBuffer( NULL ),
pWriteBufferCapacity( 0 ),
pWriteBufferUsed( false ),
pMemoryGPUSSBO( 0 )
{
	if( ! renderThread.GetExtensions().GetHasExtension(
		deoglExtensions::ext_ARB_shader_storage_buffer_object ) ){
			DETHROW( deeInvalidParam );
	}
}

deoglSPBlockSSBO::deoglSPBlockSSBO( const deoglSPBlockSSBO &paramBlock ) :
deoglShaderParameterBlock( paramBlock ),
pSSBO( 0 ),
pBindingPoint( paramBlock.pBindingPoint ),
pBindingPointUBO( paramBlock.pBindingPointUBO ),
pBindingPointAtomic( paramBlock.pBindingPointAtomic ),
pCompact( paramBlock.pCompact ),
pAllocateBuffer( true ),
pWriteBuffer( NULL ),
pWriteBufferCapacity( 0 ),
pWriteBufferUsed( false ),
pMemoryGPUSSBO( paramBlock.pMemoryGPUSSBO ){
}

deoglSPBlockSSBO::~deoglSPBlockSSBO(){
	if( IsBufferMapped() ){
		pClearMapped();
	}
	if( pWriteBuffer ){
		delete [] pWriteBuffer;
	}
	GetRenderThread().GetDelayedOperations().DeleteOpenGLBuffer( pSSBO );
}



// Management
///////////////

void deoglSPBlockSSBO::SetBindingPoint( int bindingPoint ){
	pBindingPoint = bindingPoint;
}

void deoglSPBlockSSBO::SetBindingPointUBO( int bindingPoint ){
	pBindingPointUBO = bindingPoint;
}

void deoglSPBlockSSBO::SetBindingPointAtomic( int bindingPoint ){
	pBindingPointAtomic = bindingPoint;
}

void deoglSPBlockSSBO::SetCompact( bool compact ){
	DEASSERT_FALSE( IsBufferMapped() )
	
	pCompact = compact;
}

void deoglSPBlockSSBO::Activate() const{
	DEASSERT_NOTNULL( pSSBO )
	DEASSERT_FALSE( IsBufferMapped() )
	
	OGL_CHECK( GetRenderThread(), pglBindBufferBase( GL_SHADER_STORAGE_BUFFER, pBindingPoint, pSSBO ) );
}

void deoglSPBlockSSBO::Activate( int bindingPoint ) const{
	DEASSERT_NOTNULL( pSSBO )
	DEASSERT_FALSE( IsBufferMapped() )
	
	OGL_CHECK( GetRenderThread(), pglBindBufferBase( GL_SHADER_STORAGE_BUFFER, bindingPoint, pSSBO ) );
}

void deoglSPBlockSSBO::Deactivate() const{
	OGL_CHECK( GetRenderThread(), pglBindBufferBase( GL_SHADER_STORAGE_BUFFER, pBindingPoint, 0 ) );
}

void deoglSPBlockSSBO::Deactivate( int bindingPoint ) const{
	OGL_CHECK( GetRenderThread(), pglBindBufferBase( GL_SHADER_STORAGE_BUFFER, bindingPoint, 0 ) );
}

void deoglSPBlockSSBO::ActivateUBO() const{
	DEASSERT_NOTNULL( pSSBO )
	DEASSERT_FALSE( IsBufferMapped() )
	
	OGL_CHECK( GetRenderThread(), pglBindBufferBase( GL_UNIFORM_BUFFER, pBindingPointUBO, pSSBO ) );
}

void deoglSPBlockSSBO::DeactivateUBO() const{
	OGL_CHECK( GetRenderThread(), pglBindBufferBase( GL_UNIFORM_BUFFER, pBindingPointUBO, 0 ) );
}

void deoglSPBlockSSBO::ActivateAtomic() const{
	DEASSERT_NOTNULL( pSSBO )
	DEASSERT_FALSE( IsBufferMapped() )
	
	OGL_CHECK( GetRenderThread(), pglBindBufferBase( GL_ATOMIC_COUNTER_BUFFER, pBindingPointAtomic, pSSBO ) );
}

void deoglSPBlockSSBO::DeactivateAtomic() const{
	OGL_CHECK( GetRenderThread(), pglBindBufferBase( GL_ATOMIC_COUNTER_BUFFER, pBindingPointUBO, 0 ) );
}

void deoglSPBlockSSBO::ActivateDispatchIndirect() const{
	DEASSERT_NOTNULL( pSSBO )
	DEASSERT_FALSE( IsBufferMapped() )
	
	OGL_CHECK( GetRenderThread(), pglBindBuffer( GL_DISPATCH_INDIRECT_BUFFER, pSSBO ) );
}

void deoglSPBlockSSBO::DeactivateDispatchIndirect() const{
	OGL_CHECK( GetRenderThread(), pglBindBuffer( GL_DISPATCH_INDIRECT_BUFFER, 0 ) );
}

void deoglSPBlockSSBO::MapBuffer(){
	DEASSERT_FALSE( IsBufferMapped()  )
	DEASSERT_TRUE( GetBufferSize() > 0 )
	
	if( false ){ // use mapped
		if( ! pSSBO ){
			OGL_CHECK( GetRenderThread(), pglGenBuffers( 1, &pSSBO ) );
			if( ! pSSBO ){
				DETHROW( deeOutOfMemory );
			}
		}
		
		OGL_CHECK( GetRenderThread(), pglBindBuffer( GL_SHADER_STORAGE_BUFFER, pSSBO ) );
		OGL_CHECK( GetRenderThread(), pglBufferData( GL_SHADER_STORAGE_BUFFER,
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
					GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY ) );
				
			}else{
				OGL_CHECK( GetRenderThread(), data = ( char* )pglMapBufferRange(
					GL_SHADER_STORAGE_BUFFER, 0, GetBufferSize(),
					GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT) );
			}
			
			if( ! data ){
				DETHROW( deeInvalidParam );
			}
			pSetMapped( data );
			
		}catch( const deException & ){
			OGL_CHECK( GetRenderThread(), pglBindBuffer( GL_SHADER_STORAGE_BUFFER, 0 ) );
			throw;
		}
		
	}else{
		pGrowWriteBuffer( GetBufferSize() );
		pWriteBufferUsed = true;
		pSetMapped( pWriteBuffer );
	}
}

void deoglSPBlockSSBO::MapBuffer( int element ){
	MapBuffer( element, 1 );
}

void deoglSPBlockSSBO::MapBuffer( int element, int count ){
	DEASSERT_FALSE( IsBufferMapped()  )
	DEASSERT_TRUE( GetBufferSize() > 0 )
	DEASSERT_TRUE( element >= 0 )
	DEASSERT_TRUE( count > 0 )
	DEASSERT_TRUE( element + count <= GetElementCount() )
	
	if( false ){ // use mapped
		if( ! pSSBO ){
			OGL_CHECK( GetRenderThread(), pglGenBuffers( 1, &pSSBO ) );
			if( ! pSSBO ){
				DETHROW( deeOutOfMemory );
			}
			pAllocateBuffer = true;
		}
		
		OGL_CHECK( GetRenderThread(), pglBindBuffer( GL_SHADER_STORAGE_BUFFER, pSSBO ) );
		
		if( pAllocateBuffer ){
			OGL_CHECK( GetRenderThread(), pglBufferData( GL_SHADER_STORAGE_BUFFER,
				GetBufferSize(), NULL, GL_DYNAMIC_DRAW ) );
			pAllocateBuffer = false;
		}
		
		try{
			char *data;
			OGL_CHECK( GetRenderThread(), data = ( char* )pglMapBufferRange(
				GL_SHADER_STORAGE_BUFFER, GetElementStride() * element, GetElementStride() * count,
				GL_WRITE_ONLY | GL_MAP_INVALIDATE_RANGE_BIT ) );
			if( ! data ){
				DETHROW( deeInvalidParam );
			}
			pSetMapped( data, element, count );
			
		}catch( const deException & ){
			OGL_CHECK( GetRenderThread(), pglBindBuffer( GL_SHADER_STORAGE_BUFFER, 0 ) );
			throw;
		}
		
	}else{
		pGrowWriteBuffer( GetElementStride() * count );
		pWriteBufferUsed = true;
		pSetMapped( pWriteBuffer, element, count );
	}
}

void deoglSPBlockSSBO::UnmapBuffer(){
	DEASSERT_TRUE( IsBufferMapped() )
	
	if( pWriteBufferUsed ){
		OGL_IF_CHECK( deoglRenderThread &renderThread = GetRenderThread(); )
		
		if( ! pSSBO ){
			OGL_CHECK( renderThread, pglGenBuffers( 1, &pSSBO ) );
			pAllocateBuffer = true;
		}
		
		const int stride = GetElementStride();
		const int lower = pGetElementLower();
		const int upper = pGetElementUpper();
		
		const int offset = stride * lower;
		const int size = stride * ( upper - lower + 1 );
		
		if( pAllocateBuffer ){
			OGL_CHECK( renderThread, pglBindBuffer( GL_SHADER_STORAGE_BUFFER, pSSBO ) );
			OGL_CHECK( renderThread, pglBufferData( GL_SHADER_STORAGE_BUFFER,
				GetBufferSize(), NULL, GL_DYNAMIC_DRAW ) );
			pAllocateBuffer = false;
			
		}else{
			OGL_CHECK( renderThread, pglBindBufferRange( GL_SHADER_STORAGE_BUFFER, 0, pSSBO, offset, size ) );
		}
		
		OGL_CHECK( renderThread, pglBufferSubData( GL_SHADER_STORAGE_BUFFER, offset, size, pWriteBuffer ) );
		pWriteBufferUsed = false;
		
	}else{
		OGL_CHECK( GetRenderThread(), pglBindBuffer( GL_SHADER_STORAGE_BUFFER, pSSBO ) );
		OGL_CHECK( GetRenderThread(), pglUnmapBuffer( GL_SHADER_STORAGE_BUFFER ) );
	}
	
	OGL_CHECK( GetRenderThread(), pglBindBuffer( GL_SHADER_STORAGE_BUFFER, 0 ) );
	pClearMapped();
}

int deoglSPBlockSSBO::GetAlignmentRequirements() const{
	return pCompact ? 0 : GetRenderThread().GetCapabilities().GetUBOOffsetAlignment();
}

deoglShaderParameterBlock *deoglSPBlockSSBO::Copy() const{
	return new deoglSPBlockSSBO( *this );
}

void deoglSPBlockSSBO::MapToStd430(){
	DETHROW( deeInvalidParam );
	// TODO same as std130 but arrays of continuous floats are better packed
}

char *deoglSPBlockSSBO::ReadBuffer(){
	return ReadBuffer( GetElementCount() );
}

char *deoglSPBlockSSBO::ReadBuffer( int elementCount ){
	DEASSERT_TRUE( elementCount >= 0 )
	DEASSERT_TRUE( elementCount <= GetElementCount() )
	DEASSERT_FALSE( IsBufferMapped()  )
	DEASSERT_TRUE( GetBufferSize() > 0 )
	DEASSERT_NOTNULL( pSSBO )
	
	pGrowWriteBuffer( GetBufferSize() );
	
	OGL_CHECK( GetRenderThread(), pglBindBuffer( GL_SHADER_STORAGE_BUFFER, pSSBO ) );
	OGL_CHECK( GetRenderThread(), pglGetBufferSubData( GL_SHADER_STORAGE_BUFFER, 0,
		GetElementStride() * elementCount, pWriteBuffer ) );
	OGL_CHECK( GetRenderThread(), pglBindBuffer( GL_SHADER_STORAGE_BUFFER, 0 ) );
	return pWriteBuffer;
}



void deoglSPBlockSSBO::DebugPrintConfig( const char *name ){
	const char * const valueTypeNames[ 3 ] = { "float", "int", "bool" };
	int i;
	
	GetRenderThread().GetLogger().LogInfoFormat( "Shader Parameter Block '%s' (%p):"
		" parameters=%d ssbo=%u binding=%u elementCount=%d elementStride=%d"
		" offsetPadding=%d size=%d %s", name, this, GetParameterCount(), pSSBO, pBindingPoint,
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

void deoglSPBlockSSBO::pUpdateBufferSize(){
	deoglShaderParameterBlock::pUpdateBufferSize();
	pAllocateBuffer = true;
}



// Private Functions
//////////////////////

void deoglSPBlockSSBO::pGrowWriteBuffer( int size ){
	if( size <= pWriteBufferCapacity ){
		return;
	}
	
	if( pWriteBuffer ){
		delete [] pWriteBuffer;
		pWriteBuffer = NULL;
		pWriteBufferCapacity = 0;
	}
	
	pWriteBuffer = new char[ size ];
	pWriteBufferCapacity = size;
}
