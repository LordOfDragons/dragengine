/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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
#include "deoglSPBlockReadBackSSBO.h"
#include "../deoglShaderCompiled.h"
#include "../../capabilities/deoglCapabilities.h"
#include "../../delayedoperation/deoglDelayedOperations.h"
#include "../../extensions/deoglExtensions.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>



// Class deoglSPBlockReadBackSSBO
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSPBlockReadBackSSBO::deoglSPBlockReadBackSSBO( deoglRenderThread &renderThread ) :
deoglShaderParameterBlock( renderThread ),
pSSBO( 0 ),
pCompact( true ),
pAllocateBuffer( true )
{
	DEASSERT_NOTNULL( pglMapBuffer )
}

deoglSPBlockReadBackSSBO::deoglSPBlockReadBackSSBO( const deoglSPBlockReadBackSSBO &paramBlock ) :
deoglShaderParameterBlock( paramBlock ),
pSSBO( 0 ),
pCompact( paramBlock.pCompact ),
pAllocateBuffer( true ){
}

deoglSPBlockReadBackSSBO::deoglSPBlockReadBackSSBO( const deoglSPBlockSSBO &paramBlock ) :
deoglShaderParameterBlock( paramBlock ),
pSSBO( 0 ),
pCompact( paramBlock.GetCompact() ),
pAllocateBuffer( true ){
}

deoglSPBlockReadBackSSBO::~deoglSPBlockReadBackSSBO(){
	if( IsBufferMapped() ){
		pClearMapped();
	}
	GetRenderThread().GetDelayedOperations().DeleteOpenGLBuffer( pSSBO );
}



// Management
///////////////

void deoglSPBlockReadBackSSBO::SetCompact( bool compact ){
	DEASSERT_FALSE( IsBufferMapped() )
	pCompact = compact;
}

void deoglSPBlockReadBackSSBO::Activate() const{
	DEASSERT_NOTNULL( pSSBO )
	DEASSERT_FALSE( IsBufferMapped() )
	
	OGL_CHECK( GetRenderThread(), pglBindBuffer( GL_PIXEL_PACK_BUFFER, pSSBO ) );
}

void deoglSPBlockReadBackSSBO::Activate( int ) const{
	DETHROW( deeInvalidAction );
}

void deoglSPBlockReadBackSSBO::Deactivate() const{
	OGL_CHECK( GetRenderThread(), pglBindBuffer( GL_PIXEL_PACK_BUFFER, 0 ) );
}

void deoglSPBlockReadBackSSBO::Deactivate( int ) const{
	DETHROW( deeInvalidAction );
}

void deoglSPBlockReadBackSSBO::MapBuffer(){
	DEASSERT_FALSE( IsBufferMapped()  )
	DEASSERT_TRUE( GetBufferSize() > 0 )
	DEASSERT_NOTNULL( pSSBO )
	
	OGL_IF_CHECK( deoglRenderThread &renderThread = GetRenderThread(); )
	char *data;
	
	OGL_CHECK( renderThread, pglBindBuffer( GL_PIXEL_PACK_BUFFER, pSSBO ) );
	OGL_CHECK( renderThread, data = ( char* )pglMapBufferRange(
		GL_PIXEL_PACK_BUFFER, 0, GetBufferSize(), GL_MAP_READ_BIT ) );
	OGL_CHECK( renderThread, pglBindBuffer( GL_PIXEL_PACK_BUFFER, 0 ) );
	DEASSERT_NOTNULL( data )
	
	pSetMapped( data );
}

void deoglSPBlockReadBackSSBO::MapBuffer( int element ){
	MapBuffer( element, 1 );
}

void deoglSPBlockReadBackSSBO::MapBuffer( int element, int count ){
	DEASSERT_FALSE( IsBufferMapped()  )
	DEASSERT_TRUE( GetBufferSize() > 0 )
	DEASSERT_TRUE( element >= 0 )
	DEASSERT_TRUE( count > 0 )
	DEASSERT_TRUE( element + count <= GetElementCount() )
	
	OGL_IF_CHECK( deoglRenderThread &renderThread = GetRenderThread(); )
	const int stride = GetElementStride();
	char *data;
	
	OGL_CHECK( renderThread, pglBindBuffer( GL_PIXEL_PACK_BUFFER, pSSBO ) );
	OGL_CHECK( renderThread, data = ( char* )pglMapBufferRange(
		GL_PIXEL_PACK_BUFFER, stride * element, stride * count, GL_MAP_READ_BIT ) );
	OGL_CHECK( renderThread, pglBindBuffer( GL_PIXEL_PACK_BUFFER, 0 ) );
	DEASSERT_NOTNULL( data )
	
	pSetMapped( data, element, count );
}

void deoglSPBlockReadBackSSBO::UnmapBuffer(){
	DEASSERT_TRUE( IsBufferMapped() )
	
	OGL_IF_CHECK( deoglRenderThread &renderThread = GetRenderThread(); )
	
	OGL_CHECK( renderThread, pglBindBuffer( GL_PIXEL_PACK_BUFFER, pSSBO ) );
	OGL_CHECK( renderThread, pglUnmapBuffer( GL_PIXEL_PACK_BUFFER ) );
	OGL_CHECK( renderThread, pglBindBuffer( GL_PIXEL_PACK_BUFFER, 0 ) );
	
	pClearMapped();
}

void deoglSPBlockReadBackSSBO::EnsureBuffer(){
	DEASSERT_FALSE( IsBufferMapped() )
	
	OGL_IF_CHECK( deoglRenderThread &renderThread = GetRenderThread(); )
	
	if( ! pSSBO ){
		OGL_CHECK( renderThread, pglGenBuffers( 1, &pSSBO ) );
		pAllocateBuffer = true;
	}
	
	if( pAllocateBuffer ){
		OGL_CHECK( renderThread, pglBindBuffer( GL_PIXEL_PACK_BUFFER, pSSBO ) );
		OGL_CHECK( renderThread, pglBufferData( GL_PIXEL_PACK_BUFFER,
			GetBufferSize(), nullptr, GL_DYNAMIC_READ ) );
		OGL_CHECK( GetRenderThread(), pglBindBuffer( GL_PIXEL_PACK_BUFFER, 0 ) );
		pAllocateBuffer = false;
	}
}

int deoglSPBlockReadBackSSBO::GetAlignmentRequirements() const{
	return pCompact ? 0 : GetRenderThread().GetCapabilities().GetUBOOffsetAlignment();
}

deoglShaderParameterBlock *deoglSPBlockReadBackSSBO::Copy() const{
	return new deoglSPBlockReadBackSSBO( *this );
}

void deoglSPBlockReadBackSSBO::TransferFrom( const deoglSPBlockSSBO &ssbo ){
	TransferFrom( ssbo, GetElementCount() );
}

void deoglSPBlockReadBackSSBO::TransferFrom( const deoglSPBlockSSBO &ssbo, int elementCount ){
	DEASSERT_FALSE( IsBufferMapped() )
	DEASSERT_TRUE( elementCount > 0 )
	
	OGL_IF_CHECK( deoglRenderThread &renderThread = GetRenderThread(); )
	
	EnsureBuffer();
	
	ssbo.Activate( 0 );
	OGL_CHECK( renderThread, pglBindBuffer( GL_PIXEL_PACK_BUFFER, pSSBO ) );
	
	OGL_CHECK( renderThread, pglCopyBufferSubData( GL_SHADER_STORAGE_BUFFER,
		GL_PIXEL_PACK_BUFFER, 0, 0, GetElementStride() * elementCount ) );
	
	OGL_CHECK( GetRenderThread(), pglBindBuffer( GL_PIXEL_PACK_BUFFER, 0 ) );
	ssbo.Deactivate( 0 );
}



void deoglSPBlockReadBackSSBO::DebugPrintConfig( const char *name ){
	const char * const valueTypeNames[ 3 ] = { "float", "int", "bool" };
	int i;
	
	GetRenderThread().GetLogger().LogInfoFormat( "Shader Parameter Block '%s' (%p):"
		" parameters=%d ssbo=%u elementCount=%d elementStride=%d"
		" offsetPadding=%d size=%d %s", name, this, GetParameterCount(), pSSBO,
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

void deoglSPBlockReadBackSSBO::pUpdateBufferSize(){
	deoglShaderParameterBlock::pUpdateBufferSize();
	pAllocateBuffer = true;
}
