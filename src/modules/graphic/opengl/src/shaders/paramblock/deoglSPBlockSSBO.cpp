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
#include "../../renderthread/deoglRTChoices.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>



// Class deoglSPBlockSSBO
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglSPBlockSSBO::deoglSPBlockSSBO( deoglRenderThread &renderThread, eType type ) :
deoglShaderParameterBlock( renderThread ),
pType( type ),
pSSBO( 0 ),
pSSBORead( 0 ),
pBindingPoint( 0 ),
pBindingPointUBO( 0 ),
pBindingPointAtomic( 0 ),
pCompact( true ),
pAllocateBuffer( true ),
pWriteBuffer( nullptr ),
pWriteBufferCapacity( 0 ),
pPersistentMapped( nullptr ),
pFenceTransfer( 0 ),
pMemoryGPUSSBO( 0 ){
}

deoglSPBlockSSBO::deoglSPBlockSSBO( const deoglSPBlockSSBO &paramBlock ) :
deoglShaderParameterBlock( paramBlock ),
pType( paramBlock.pType ),
pSSBO( 0 ),
pSSBORead( 0 ),
pBindingPoint( paramBlock.pBindingPoint ),
pBindingPointUBO( paramBlock.pBindingPointUBO ),
pBindingPointAtomic( paramBlock.pBindingPointAtomic ),
pCompact( paramBlock.pCompact ),
pAllocateBuffer( true ),
pWriteBuffer( nullptr ),
pWriteBufferCapacity( 0 ),
pPersistentMapped( nullptr ),
pFenceTransfer( 0 ),
pMemoryGPUSSBO( paramBlock.pMemoryGPUSSBO ){
}

deoglSPBlockSSBO::~deoglSPBlockSSBO(){
	if( IsBufferMapped() ){
		pClearMapped();
	}
	if( pWriteBuffer ){
		delete [] pWriteBuffer;
	}
	if( pPersistentMapped ){
		OGL_CHECK( GetRenderThread(), pglUnmapNamedBuffer( pSSBO ) );
	}
	GetRenderThread().GetDelayedOperations().DeleteOpenGLBuffer( pSSBORead );
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
	Activate( pBindingPoint );
}

void deoglSPBlockSSBO::Activate( int bindingPoint ) const{
	DEASSERT_NOTNULL( pSSBO )
	DEASSERT_FALSE( IsBufferMapped() )
	
	OGL_CHECK( GetRenderThread(), pglBindBufferBase( GL_SHADER_STORAGE_BUFFER, bindingPoint, pSSBO ) );
}

void deoglSPBlockSSBO::Deactivate() const{
	Deactivate( pBindingPoint );
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
	ActivateAtomic( pBindingPointAtomic );
}

void deoglSPBlockSSBO::ActivateAtomic( int bindingPoint ) const{
	DEASSERT_NOTNULL( pSSBO )
	DEASSERT_FALSE( IsBufferMapped() )
	
	OGL_CHECK( GetRenderThread(), pglBindBufferBase( GL_ATOMIC_COUNTER_BUFFER, bindingPoint, pSSBO ) );
}

void deoglSPBlockSSBO::DeactivateAtomic() const{
	DeactivateAtomic( pBindingPointAtomic );
}

void deoglSPBlockSSBO::DeactivateAtomic( int bindingPoint ) const{
	OGL_CHECK( GetRenderThread(), pglBindBufferBase( GL_ATOMIC_COUNTER_BUFFER, bindingPoint, 0 ) );
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
	MapBuffer( 0, GetElementCount() );
}

void deoglSPBlockSSBO::MapBuffer( int element ){
	MapBuffer( element, 1 );
}

void deoglSPBlockSSBO::MapBuffer( int element, int count ){
	DEASSERT_TRUE( pType == etStatic || pType == etStream )
	DEASSERT_FALSE( IsBufferMapped()  )
	DEASSERT_TRUE( GetBufferSize() > 0 )
	if( pType == etStatic ){
		DEASSERT_TRUE( element == 0 )
		DEASSERT_TRUE( count == GetElementCount() )
		
	}else{
		DEASSERT_TRUE( element >= 0 )
		DEASSERT_TRUE( count > 0 )
		DEASSERT_TRUE( element + count <= GetElementCount() )
	}
	
	if( pType == etStream && GetRenderThread().GetChoices().GetUseDirectStateAccess() ){
		pEnsurePersistMapped( GL_MAP_WRITE_BIT );
	}
	
	if( pPersistentMapped ){
		pSetMapped( pPersistentMapped + GetElementStride() * element, element, count );
		
	}else{
		pGrowWriteBuffer( GetElementStride() * count );
		pSetMapped( pWriteBuffer, element, count );
	}
}

void deoglSPBlockSSBO::UnmapBuffer(){
	DEASSERT_TRUE( pType == etStatic || pType == etStream )
	DEASSERT_TRUE( IsBufferMapped() )
	
	deoglRenderThread &renderThread = GetRenderThread();
	
	if( pPersistentMapped ){
		OGL_CHECK( renderThread, pglMemoryBarrier( GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT ) );
		
	}else{
		const int stride = GetElementStride();
		const int lower = pGetElementLower();
		const int upper = pGetElementUpper();
		
		const int offset = stride * lower;
		const int size = stride * ( upper - lower + 1 );
		
		if( renderThread.GetChoices().GetUseDirectStateAccess() ){
			if( pType == etStatic ){
				pEnsureSSBO();
				OGL_CHECK( renderThread, pglNamedBufferStorage( pSSBO, GetBufferSize(), pWriteBuffer, 0 ) );
			}
			
		}else{
			pEnsureSSBO();
			
			if( pAllocateBuffer ){
				OGL_CHECK( renderThread, pglBindBuffer( GL_SHADER_STORAGE_BUFFER, pSSBO ) );
				OGL_CHECK( renderThread, pglBufferData( GL_SHADER_STORAGE_BUFFER,
					GetBufferSize(), nullptr, GL_DYNAMIC_DRAW ) );
				pAllocateBuffer = false;
				
			}else{
				OGL_CHECK( renderThread, pglBindBufferRange( GL_SHADER_STORAGE_BUFFER, 0, pSSBO, offset, size ) );
			}
			
			OGL_CHECK( renderThread, pglBufferSubData( GL_SHADER_STORAGE_BUFFER, offset, size, pWriteBuffer ) );
			OGL_CHECK( GetRenderThread(), pglBindBuffer( GL_SHADER_STORAGE_BUFFER, 0 ) );
		}
	}
	
	pClearMapped();
}

void deoglSPBlockSSBO::EnsureBuffer(){
	DEASSERT_FALSE( IsBufferMapped() )
	
	pEnsureSSBO();
	
	if( ! pAllocateBuffer ){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	
	if( renderThread.GetChoices().GetUseDirectStateAccess() ){
		switch( pType ){
		case etStatic:
			break;
			
		case etStream:
			OGL_CHECK( renderThread, pglNamedBufferStorage( pSSBO, GetBufferSize(), nullptr,
				GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT ) );
			OGL_CHECK( renderThread, pPersistentMapped = ( char* )pglMapNamedBufferRange(
				pSSBO, 0, GetBufferSize(), GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT ) );
			break;
			
		case etRead:
			OGL_CHECK( renderThread, pglNamedBufferStorage( pSSBO, GetBufferSize(), nullptr,
				GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_CLIENT_STORAGE_BIT ) );
			OGL_CHECK( renderThread, pPersistentMapped = ( char* )pglMapNamedBufferRange(
				pSSBO, 0, GetBufferSize(), GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT ) );
			break;
			
		case etGpu:
			OGL_CHECK( renderThread, pglNamedBufferStorage( pSSBO, GetBufferSize(), nullptr, 0 ) );
			break;
		}
		
	}else{
		OGL_CHECK( renderThread, pglBindBuffer( GL_SHADER_STORAGE_BUFFER, pSSBO ) );
		OGL_CHECK( renderThread, pglBufferData( GL_SHADER_STORAGE_BUFFER,
			GetBufferSize(), nullptr, GL_DYNAMIC_DRAW ) );
		OGL_CHECK( GetRenderThread(), pglBindBuffer( GL_SHADER_STORAGE_BUFFER, 0 ) );
		
		if( pType == etRead ){
			if( ! pSSBORead ){
				OGL_CHECK( renderThread, pglGenBuffers( 1, &pSSBORead ) );
			}
			
			OGL_CHECK( renderThread, pglBindBuffer( GL_PIXEL_PACK_BUFFER, pSSBORead ) );
			OGL_CHECK( renderThread, pglBufferData( GL_PIXEL_PACK_BUFFER,
				GetBufferSize(), nullptr, GL_DYNAMIC_READ ) );
			OGL_CHECK( GetRenderThread(), pglBindBuffer( GL_PIXEL_PACK_BUFFER, 0 ) );
		}
	}
	pAllocateBuffer = false;
}

void deoglSPBlockSSBO::ClearDataUInt( int count, uint32_t r, uint32_t g, uint32_t b, uint32_t a ){
	DEASSERT_FALSE( IsBufferMapped() )
	DEASSERT_TRUE( count >= 0 )
	DEASSERT_TRUE( count * 16 <= GetBufferSize() )
	
	EnsureBuffer();
	
	const GLuint v[ 4 ] = { ( GLuint )r, ( GLuint )g, ( GLuint )b, ( GLuint )a };
	deoglRenderThread &renderThread = GetRenderThread();
	
	if( renderThread.GetChoices().GetUseDirectStateAccess() ){
		OGL_CHECK( renderThread, pglClearNamedBufferSubData( pSSBO, GL_RGBA32UI,
			0, count * 16, GL_RGBA_INTEGER, GL_UNSIGNED_INT, v ) );
		
	}else{
		OGL_CHECK( renderThread, pglBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, pSSBO ) );
		OGL_CHECK( renderThread, pglClearBufferSubData( GL_SHADER_STORAGE_BUFFER,
			GL_RGBA32UI, 0, count * 16, GL_RGBA_INTEGER, GL_UNSIGNED_INT, &v[ 0 ] ) );
		OGL_CHECK( renderThread, pglBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, 0 ) );
	}
}

void deoglSPBlockSSBO::ClearDataFloat( int count, float r, float g, float b, float a ){
	DEASSERT_FALSE( IsBufferMapped() )
	DEASSERT_TRUE( count >= 0 )
	DEASSERT_TRUE( count * 16 <= GetBufferSize() )
	
	EnsureBuffer();
	
	const GLfloat v[ 4 ] = { ( GLfloat )r, ( GLfloat )g, ( GLfloat )b, ( GLfloat )a };
	deoglRenderThread &renderThread = GetRenderThread();
	
	if( renderThread.GetChoices().GetUseDirectStateAccess() ){
		OGL_CHECK( renderThread, pglClearNamedBufferSubData( pSSBO, GL_RGBA32F,
			0, count * 16, GL_RGBA, GL_FLOAT, v ) );
		
	}else{
		OGL_CHECK( renderThread, pglBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, pSSBO ) );
		OGL_CHECK( renderThread, pglClearBufferSubData( GL_SHADER_STORAGE_BUFFER,
			GL_RGBA32F, 0, count * 16, GL_RGBA, GL_FLOAT, &v[ 0 ] ) );
		OGL_CHECK( renderThread, pglBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, 0 ) );
	}
}

void deoglSPBlockSSBO::GPUFinishedWriting(){
	DEASSERT_TRUE( pType == etRead )
	
	deoglRenderThread &renderThread = GetRenderThread();
	
	if( renderThread.GetChoices().GetUseDirectStateAccess() ){
		pFenceTransfer = pglFenceSync( GL_SYNC_GPU_COMMANDS_COMPLETE, 0 );
		OGL_CHECK( renderThread, pglMemoryBarrier( GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT ) );
		
	}else{
		OGL_CHECK( renderThread, pglMemoryBarrier( GL_BUFFER_UPDATE_BARRIER_BIT ) );
	}
}

void deoglSPBlockSSBO::GPUReadToCPU(){
	GPUReadToCPU( GetElementCount() );
}

void deoglSPBlockSSBO::GPUReadToCPU( int elementCount ){
	DEASSERT_TRUE( pType == etRead )
	DEASSERT_FALSE( IsBufferMapped() )
	DEASSERT_TRUE( elementCount >= 0 )
	DEASSERT_TRUE( elementCount <= GetElementCount() )
	
	deoglRenderThread &renderThread = GetRenderThread();
	if( renderThread.GetChoices().GetUseDirectStateAccess() ){
		return;
	}
	
	EnsureBuffer();
	
	// NOTE the logic use here would be copy from GL_COPY_READ_BUFFER to GL_COPY_WRITE_BUFFER.
	//      unfortunately this is slower than copying from GL_SHADER_STORAGE_BUFFER to
	//      GL_PIXEL_PACK_BUFFER. for this reason GL_PIXEL_PACK_BUFFER is used
	
	OGL_CHECK( renderThread, pglBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, pSSBO ) );
	OGL_CHECK( renderThread, pglBindBuffer( GL_PIXEL_PACK_BUFFER, pSSBORead ) );
	
	OGL_CHECK( renderThread, pglCopyBufferSubData( GL_SHADER_STORAGE_BUFFER,
		GL_PIXEL_PACK_BUFFER, 0, 0, GetElementStride() * elementCount ) );
	
	OGL_CHECK( renderThread, pglBindBuffer( GL_PIXEL_PACK_BUFFER, 0 ) );
	OGL_CHECK( renderThread, pglBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, 0 ) );
}

void deoglSPBlockSSBO::MapBufferRead(){
	MapBufferRead( 0, GetElementCount() );
}

void deoglSPBlockSSBO::MapBufferRead( int element ){
	MapBufferRead( element, 1 );
}

void deoglSPBlockSSBO::MapBufferRead( int element, int count ){
	DEASSERT_TRUE( pType == etRead )
	DEASSERT_FALSE( IsBufferMapped()  )
	DEASSERT_TRUE( GetBufferSize() > 0 )
	DEASSERT_TRUE( element >= 0 )
	DEASSERT_TRUE( count > 0 )
	DEASSERT_TRUE( element + count <= GetElementCount() )
	
	OGL_IF_CHECK( deoglRenderThread &renderThread = GetRenderThread(); )
	
	if( pPersistentMapped ){
		OGL_FENCE_WAIT( renderThread, pFenceTransfer );
		pSetMapped( pPersistentMapped + GetElementStride() * element, element, count );
		
	}else{
		DEASSERT_NOTNULL( pSSBORead )
		
		const int stride = GetElementStride();
		char *data;
		
		OGL_CHECK( renderThread, pglBindBuffer( GL_PIXEL_PACK_BUFFER, pSSBORead ) );
		OGL_CHECK( renderThread, data = ( char* )pglMapBufferRange(
			GL_PIXEL_PACK_BUFFER, stride * element, stride * count, GL_MAP_READ_BIT ) );
		OGL_CHECK( renderThread, pglBindBuffer( GL_PIXEL_PACK_BUFFER, 0 ) );
		DEASSERT_NOTNULL( data )
		
		pSetMapped( data, element, count );
	}
}

void deoglSPBlockSSBO::UnmapBufferRead(){
	DEASSERT_TRUE( pType == etRead )
	DEASSERT_TRUE( IsBufferMapped() )
	
	if( ! pPersistentMapped ){
		OGL_IF_CHECK( deoglRenderThread &renderThread = GetRenderThread(); )
		OGL_CHECK( renderThread, pglBindBuffer( GL_PIXEL_PACK_BUFFER, pSSBORead ) );
		OGL_CHECK( renderThread, pglUnmapBuffer( GL_PIXEL_PACK_BUFFER ) );
		OGL_CHECK( renderThread, pglBindBuffer( GL_PIXEL_PACK_BUFFER, 0 ) );
	}
	
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
		pWriteBuffer = nullptr;
		pWriteBufferCapacity = 0;
	}
	
	pWriteBuffer = new char[ size ];
	pWriteBufferCapacity = size;
}

void deoglSPBlockSSBO::pEnsureSSBO(){
	deoglRenderThread &renderThread = GetRenderThread();
	
	if( pSSBO && renderThread.GetChoices().GetUseDirectStateAccess() && pAllocateBuffer ){
		if( pPersistentMapped ){
			OGL_CHECK( renderThread, pglUnmapNamedBuffer( pSSBO ) );
			pPersistentMapped = nullptr;
		}
		GetRenderThread().GetDelayedOperations().DeleteOpenGLBuffer( pSSBO );
		pSSBO = 0;
	}
	
	if( ! pSSBO ){
		if( renderThread.GetChoices().GetUseDirectStateAccess() ){
			OGL_CHECK( renderThread, pglCreateBuffers( 1, &pSSBO ) );
			
		}else{
			OGL_CHECK( renderThread, pglGenBuffers( 1, &pSSBO ) );
		}
		pAllocateBuffer = true;
	}
}

void deoglSPBlockSSBO::pEnsurePersistMapped( GLenum flagsStorage ){
	pEnsurePersistMapped( flagsStorage, flagsStorage );
}

void deoglSPBlockSSBO::pEnsurePersistMapped( GLenum flagsStorage, GLenum flagsMap ){
	pEnsureSSBO();
	
	if( ! pAllocateBuffer ){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	
	OGL_CHECK( renderThread, pglNamedBufferStorage(
		pSSBO, GetBufferSize(), nullptr, flagsStorage | GL_MAP_PERSISTENT_BIT ) );
	OGL_CHECK( renderThread, pPersistentMapped = ( char* )pglMapNamedBufferRange(
		pSSBO, 0, GetBufferSize(), flagsMap | GL_MAP_PERSISTENT_BIT ) );
	
	pAllocateBuffer = false;
}
