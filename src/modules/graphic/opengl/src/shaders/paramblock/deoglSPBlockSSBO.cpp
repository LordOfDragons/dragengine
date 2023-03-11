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
pSSBOLocal( 0 ),
pBindingPoint( 0 ),
pBindingPointUBO( 0 ),
pBindingPointAtomic( 0 ),
pCompact( true ),
pAllocateBuffer( true ),
pWriteBuffer( nullptr ),
pWriteBufferCapacity( 0 ),
pPersistentMapped( nullptr ),
pFenceTransfer( nullptr ),
pMemoryGPUSSBO( 0 ){
}

deoglSPBlockSSBO::deoglSPBlockSSBO( const deoglSPBlockSSBO &paramBlock ) :
deoglShaderParameterBlock( paramBlock ),
pType( paramBlock.pType ),
pSSBO( 0 ),
pSSBOLocal( 0 ),
pBindingPoint( paramBlock.pBindingPoint ),
pBindingPointUBO( paramBlock.pBindingPointUBO ),
pBindingPointAtomic( paramBlock.pBindingPointAtomic ),
pCompact( paramBlock.pCompact ),
pAllocateBuffer( true ),
pWriteBuffer( nullptr ),
pWriteBufferCapacity( 0 ),
pPersistentMapped( nullptr ),
pFenceTransfer( nullptr ),
pMemoryGPUSSBO( paramBlock.pMemoryGPUSSBO ){
}

deoglSPBlockSSBO::deoglSPBlockSSBO( const deoglSPBlockSSBO &paramBlock, eType type ) :
deoglShaderParameterBlock( paramBlock ),
pType( type ),
pSSBO( 0 ),
pSSBOLocal( 0 ),
pBindingPoint( paramBlock.pBindingPoint ),
pBindingPointUBO( paramBlock.pBindingPointUBO ),
pBindingPointAtomic( paramBlock.pBindingPointAtomic ),
pCompact( paramBlock.pCompact ),
pAllocateBuffer( true ),
pWriteBuffer( nullptr ),
pWriteBufferCapacity( 0 ),
pPersistentMapped( nullptr ),
pFenceTransfer( nullptr ),
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
		OGL_CHECK( GetRenderThread(), pglUnmapNamedBuffer( pSSBOLocal ) );
	}
	if( pFenceTransfer ){
		pglDeleteSync( pFenceTransfer );
	}
	GetRenderThread().GetDelayedOperations().DeleteOpenGLBuffer( pSSBOLocal );
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
	if( ! GetRenderThread().GetChoices().GetUseDirectStateAccess() ){
		DEASSERT_FALSE( IsBufferMapped() )
	}
	
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
	if( ! GetRenderThread().GetChoices().GetUseDirectStateAccess() ){
		DEASSERT_FALSE( IsBufferMapped() )
	}
	
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
	if( ! GetRenderThread().GetChoices().GetUseDirectStateAccess() ){
		DEASSERT_FALSE( IsBufferMapped() )
	}
	
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
	if( ! GetRenderThread().GetChoices().GetUseDirectStateAccess() ){
		DEASSERT_FALSE( IsBufferMapped() )
	}
	
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
	
	EnsureBuffer();
	
	pGrowWriteBuffer( GetElementStride() * count );
	pSetMapped( pWriteBuffer, element, count );
}

void deoglSPBlockSSBO::UnmapBuffer(){
	DEASSERT_TRUE( pType == etStatic || pType == etStream )
	DEASSERT_TRUE( IsBufferMapped() )
	
	deoglRenderThread &renderThread = GetRenderThread();
	
	const int stride = GetElementStride();
	const int lower = pGetElementLower();
	const int upper = pGetElementUpper();
	
	const int offset = stride * lower;
	const int size = stride * ( upper - lower + 1 );
	
	if( renderThread.GetChoices().GetUseDirectStateAccess() ){
		if( pType == etStatic ){
			// EnsureBuffer does not create a buffer since we need to upload the data
			// while creating the buffer. writing it afterwards is an error
			OGL_CHECK( renderThread, pglNamedBufferStorage( pSSBO, GetBufferSize(), pWriteBuffer, 0 ) );
			
		}else{
			OGL_CHECK( renderThread, pglNamedBufferSubData( pSSBO, offset, size, pWriteBuffer ) );
		}
		
	}else{
		OGL_CHECK( renderThread, pglBindBufferRange( GL_SHADER_STORAGE_BUFFER, 0, pSSBO, offset, size ) );
		OGL_CHECK( renderThread, pglBufferSubData( GL_SHADER_STORAGE_BUFFER, offset, size, pWriteBuffer ) );
		OGL_CHECK( GetRenderThread(), pglBindBuffer( GL_SHADER_STORAGE_BUFFER, 0 ) );
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
	const int size = GetBufferSize();
	
	if( renderThread.GetChoices().GetUseDirectStateAccess() ){
		switch( pType ){
		case etStatic:
			// this is empty on purpose. data in static buffers can be only written during
			// buffer creation time. trying to do it afterwards is an error. for this reason
			// no buffer is created here. the creation will be done in UnmapBuffer()
			break;
			
		case etStream:
			// OGL_CHECK( renderThread, pglNamedBufferStorage( pSSBO, size, nullptr, GL_DYNAMIC_STORAGE_BIT ) );
			OGL_CHECK( renderThread, pglNamedBufferData( pSSBO, size, nullptr, GL_DYNAMIC_DRAW ) );
			break;
			
		case etRead:
			// storage buffers at 10x slower than regular buffers for shaders to write to
			//OGL_CHECK( renderThread, pglNamedBufferStorage( pSSBO, size, nullptr, GL_DYNAMIC_STORAGE_BIT ) );
			OGL_CHECK( renderThread, pglNamedBufferData( pSSBO, size, nullptr, GL_DYNAMIC_DRAW ) );
			
			OGL_CHECK( renderThread, pglNamedBufferStorage( pSSBOLocal, size, nullptr,
				GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_CLIENT_STORAGE_BIT ) );
			OGL_CHECK( renderThread, pPersistentMapped = ( char* )pglMapNamedBufferRange(
				pSSBOLocal, 0, size, GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT ) );
			// OGL_CHECK( renderThread, pglNamedBufferData( pSSBOLocal, size, nullptr, GL_DYNAMIC_READ ) );
			break;
			
		case etGpu:
			// storage buffers at 10x slower than regular buffers for shaders to write to
			// OGL_CHECK( renderThread, pglNamedBufferStorage( pSSBO, size, nullptr, GL_DYNAMIC_STORAGE_BIT ) );
			OGL_CHECK( renderThread, pglNamedBufferData( pSSBO, size, nullptr, GL_DYNAMIC_DRAW ) );
			break;
		}
		
	}else{
		OGL_CHECK( renderThread, pglBindBuffer( GL_SHADER_STORAGE_BUFFER, pSSBO ) );
		OGL_CHECK( renderThread, pglBufferData( GL_SHADER_STORAGE_BUFFER, size, nullptr, GL_DYNAMIC_DRAW ) );
		OGL_CHECK( GetRenderThread(), pglBindBuffer( GL_SHADER_STORAGE_BUFFER, 0 ) );
		
		if( pType == etRead ){
			if( ! pSSBOLocal ){
				OGL_CHECK( renderThread, pglGenBuffers( 1, &pSSBOLocal ) );
			}
			
			OGL_CHECK( renderThread, pglBindBuffer( GL_PIXEL_PACK_BUFFER, pSSBOLocal ) );
			OGL_CHECK( renderThread, pglBufferData( GL_PIXEL_PACK_BUFFER, size, nullptr, GL_DYNAMIC_READ ) );
			OGL_CHECK( GetRenderThread(), pglBindBuffer( GL_PIXEL_PACK_BUFFER, 0 ) );
		}
	}
	pAllocateBuffer = false;
}

void deoglSPBlockSSBO::ClearDataUInt( uint32_t r, uint32_t g, uint32_t b, uint32_t a ){
	ClearDataUInt( 0, GetElementCount(), r, g, b, a );
}

void deoglSPBlockSSBO::ClearDataUInt( int offset, int count, uint32_t r, uint32_t g, uint32_t b, uint32_t a ){
	DEASSERT_TRUE( offset >= 0 )
	DEASSERT_TRUE( offset * 16 <= GetBufferSize() )
	DEASSERT_TRUE( count >= 0 )
	DEASSERT_TRUE( ( offset + count ) * 16 <= GetBufferSize() )
	
	EnsureBuffer();
	
	const GLuint v[ 4 ] = { ( GLuint )r, ( GLuint )g, ( GLuint )b, ( GLuint )a };
	deoglRenderThread &renderThread = GetRenderThread();
	
	if( renderThread.GetChoices().GetUseDirectStateAccess() ){
		OGL_CHECK( renderThread, pglClearNamedBufferSubData( pSSBO, GL_RGBA32UI,
			offset * 16, count * 16, GL_RGBA_INTEGER, GL_UNSIGNED_INT, v ) );
		
	}else{
		OGL_CHECK( renderThread, pglBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, pSSBO ) );
		OGL_CHECK( renderThread, pglClearBufferSubData( GL_SHADER_STORAGE_BUFFER,
			GL_RGBA32UI, offset * 16, count * 16, GL_RGBA_INTEGER, GL_UNSIGNED_INT, &v[ 0 ] ) );
		OGL_CHECK( renderThread, pglBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, 0 ) );
	}
}

void deoglSPBlockSSBO::ClearDataFloat( float r, float g, float b, float a ){
	ClearDataFloat( 0, GetElementCount(), r, g, b, a );
}

void deoglSPBlockSSBO::ClearDataFloat( int offset, int count, float r, float g, float b, float a ){
	DEASSERT_TRUE( offset >= 0 )
	DEASSERT_TRUE( offset * 16 <= GetBufferSize() )
	DEASSERT_TRUE( count >= 0 )
	DEASSERT_TRUE( ( offset + count ) * 16 <= GetBufferSize() )
	
	EnsureBuffer();
	
	const GLfloat v[ 4 ] = { ( GLfloat )r, ( GLfloat )g, ( GLfloat )b, ( GLfloat )a };
	deoglRenderThread &renderThread = GetRenderThread();
	
	if( renderThread.GetChoices().GetUseDirectStateAccess() ){
		OGL_CHECK( renderThread, pglClearNamedBufferSubData( pSSBO, GL_RGBA32F,
			offset * 16, count * 16, GL_RGBA, GL_FLOAT, v ) );
		
	}else{
		OGL_CHECK( renderThread, pglBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, pSSBO ) );
		OGL_CHECK( renderThread, pglClearBufferSubData( GL_SHADER_STORAGE_BUFFER,
			GL_RGBA32F, offset * 16, count * 16, GL_RGBA, GL_FLOAT, &v[ 0 ] ) );
		OGL_CHECK( renderThread, pglBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, 0 ) );
	}
}

void deoglSPBlockSSBO::CopyData( const deoglSPBlockSSBO &ssbo, int offset, int count, int ssboOffset ){
	DEASSERT_TRUE( GetElementStride() == ssbo.GetElementStride() )
	DEASSERT_TRUE( offset >= 0 )
	DEASSERT_TRUE( count >= 0 )
	DEASSERT_TRUE( offset + count <= GetElementCount() )
	DEASSERT_TRUE( ssboOffset >= 0 )
	DEASSERT_TRUE( ssboOffset + count <= ssbo.GetElementCount() )
	DEASSERT_NOTNULL( ssbo.pSSBO )
	
	EnsureBuffer();
	
	deoglRenderThread &renderThread = GetRenderThread();
	const int stride = GetElementStride();
	
	if( renderThread.GetChoices().GetUseDirectStateAccess() ){
		OGL_CHECK( renderThread, pglCopyNamedBufferSubData( ssbo.pSSBO, pSSBO,
			stride * ssboOffset, stride * offset, stride * count ) );
		
	}else{
		OGL_CHECK( renderThread, pglBindBuffer( GL_COPY_READ_BUFFER, ssbo.pSSBO ) );
		OGL_CHECK( renderThread, pglBindBuffer( GL_COPY_WRITE_BUFFER, pSSBO ) );
		
		OGL_CHECK( renderThread, pglCopyBufferSubData( GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER,
			stride * ssboOffset, stride * offset, stride * count ) );
		
		OGL_CHECK( renderThread, pglBindBuffer( GL_COPY_WRITE_BUFFER, 0 ) );
		OGL_CHECK( renderThread, pglBindBuffer( GL_COPY_READ_BUFFER, 0 ) );
	}
}

void deoglSPBlockSSBO::GPUFinishedWriting(){
	DEASSERT_TRUE( pType == etRead )
	
	if( GetRenderThread().GetChoices().GetUseDirectStateAccess() && pFenceTransfer ){
		pglDeleteSync( pFenceTransfer );
		pFenceTransfer = nullptr;
	}
	
	// required to sync call to glCopy*BufferSubData after compute shader finished
	OGL_CHECK( GetRenderThread(), pglMemoryBarrier( GL_BUFFER_UPDATE_BARRIER_BIT ) );
}

void deoglSPBlockSSBO::GPUReadToCPU(){
	GPUReadToCPU( GetElementCount() );
}

void deoglSPBlockSSBO::GPUReadToCPU( int elementCount ){
	DEASSERT_TRUE( pType == etRead )
	DEASSERT_FALSE( IsBufferMapped() )
	DEASSERT_TRUE( elementCount >= 0 )
	DEASSERT_TRUE( elementCount <= GetElementCount() )
	DEASSERT_NOTNULL( pSSBO )
	DEASSERT_NOTNULL( pSSBOLocal )
	
	deoglRenderThread &renderThread = GetRenderThread();
	
	if( renderThread.GetChoices().GetUseDirectStateAccess() ){
		if( pFenceTransfer ){
			pglDeleteSync( pFenceTransfer );
			pFenceTransfer = nullptr;
		}
		
		OGL_CHECK( renderThread, pglCopyNamedBufferSubData(
			pSSBO, pSSBOLocal, 0, 0, GetElementStride() * elementCount ) );
		
		// required to make copied data visible in persistently mapped memory.
		// according to docs fence has to come after memory barrier
		OGL_CHECK( renderThread, pglMemoryBarrier( GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT ) );
		pFenceTransfer = pglFenceSync( GL_SYNC_GPU_COMMANDS_COMPLETE, 0 );
		
	}else{
		// the logic use here would be copy from GL_COPY_READ_BUFFER to GL_COPY_WRITE_BUFFER.
		// unfortunately this is slower than copying from GL_SHADER_STORAGE_BUFFER to
		// GL_PIXEL_PACK_BUFFER. for this reason GL_PIXEL_PACK_BUFFER is used
		
		OGL_CHECK( renderThread, pglBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, pSSBO ) );
		OGL_CHECK( renderThread, pglBindBuffer( GL_PIXEL_PACK_BUFFER, pSSBOLocal ) );
		
		OGL_CHECK( renderThread, pglCopyBufferSubData( GL_SHADER_STORAGE_BUFFER,
			GL_PIXEL_PACK_BUFFER, 0, 0, GetElementStride() * elementCount ) );
		
		OGL_CHECK( renderThread, pglBindBuffer( GL_PIXEL_PACK_BUFFER, 0 ) );
		OGL_CHECK( renderThread, pglBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, 0 ) );
	}
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
	DEASSERT_NOTNULL( pSSBOLocal )
	
	OGL_IF_CHECK( deoglRenderThread &renderThread = GetRenderThread(); )
	
	if( pPersistentMapped ){
		if( renderThread.GetChoices().GetUseDirectStateAccess() ){
			OGL_FENCE_WAIT( renderThread, pFenceTransfer );
		}
		
		pSetMapped( pPersistentMapped + GetElementStride() * element, element, count );
		
	}else{
		const int stride = GetElementStride();
		char *data;
		
		OGL_CHECK( renderThread, pglBindBuffer( GL_PIXEL_PACK_BUFFER, pSSBOLocal ) );
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
		deoglRenderThread &renderThread = GetRenderThread();
		OGL_CHECK( renderThread, pglBindBuffer( GL_PIXEL_PACK_BUFFER, pSSBOLocal ) );
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
	const bool dsa = renderThread.GetChoices().GetUseDirectStateAccess();
	
	if( dsa && pAllocateBuffer ){
		if( pSSBOLocal ){
			if( pPersistentMapped ){
				OGL_CHECK( renderThread, pglUnmapNamedBuffer( pSSBOLocal ) );
				pPersistentMapped = nullptr;
			}
			GetRenderThread().GetDelayedOperations().DeleteOpenGLBuffer( pSSBOLocal );
			pSSBOLocal = 0;
		}
		
		if( pSSBO ){
			GetRenderThread().GetDelayedOperations().DeleteOpenGLBuffer( pSSBO );
			pSSBO = 0;
		}
	}
	
	if( ! pSSBO ){
		if( dsa ){
			OGL_CHECK( renderThread, pglCreateBuffers( 1, &pSSBO ) );
			
		}else{
			OGL_CHECK( renderThread, pglGenBuffers( 1, &pSSBO ) );
		}
		pAllocateBuffer = true;
	}
	
	if( pType == etRead && ! pSSBOLocal ){
		if( dsa ){
			OGL_CHECK( renderThread, pglCreateBuffers( 1, &pSSBOLocal ) );
			
		}else{
			OGL_CHECK( renderThread, pglGenBuffers( 1, &pSSBOLocal ) );
		}
		
		pAllocateBuffer = true;
	}
}
