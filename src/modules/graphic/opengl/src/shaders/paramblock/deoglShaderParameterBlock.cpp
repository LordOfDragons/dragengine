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

#include "deoglShaderParameterBlock.h"
#include "../deoglShaderCompiled.h"
#include "../../capabilities/deoglCapabilities.h"
#include "../../extensions/deoglExtensions.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>



// Helper Functions
/////////////////////

static inline void fSet( GLfloat *d, float v ){
	d[ 0 ] = ( GLfloat )v;
}
static inline void fSet( GLfloat *d, float v1, float v2 ){
	d[ 0 ] = ( GLfloat )v1;  d[ 1 ] = ( GLfloat )v2;
}
static inline void fSet( GLfloat *d, float v1, float v2, float v3 ){
	d[ 0 ] = ( GLfloat )v1;  d[ 1 ] = ( GLfloat )v2;
	d[ 2 ] = ( GLfloat )v3;
}
static inline void fSet( GLfloat *d, float v1, float v2, float v3, float v4 ){
	d[ 0 ] = ( GLfloat )v1;  d[ 1 ] = ( GLfloat )v2;
	d[ 2 ] = ( GLfloat )v3;  d[ 3 ] = ( GLfloat )v4;
}

// static inline void fSet( GLfloat *d, double v ){
// 	fSet( d, ( GLfloat )v );
// }
static inline void fSet( GLfloat *d, double v1, double v2 ){
	fSet( d, ( GLfloat )v1, ( GLfloat )v2 );
}
static inline void fSet( GLfloat *d, double v1, double v2, double v3 ){
	fSet( d, ( GLfloat )v1, ( GLfloat )v2, ( GLfloat )v3 );
}
static inline void fSet( GLfloat *d, double v1, double v2, double v3, double v4 ){
	fSet( d, ( GLfloat )v1, ( GLfloat )v2, ( GLfloat )v3, ( GLfloat )v4 );
}

static inline void fSet( GLint *d, int v ){
	d[ 0 ] = ( GLint )v;
}
static inline void fSet( GLint *d, int v1, int v2 ){
	d[ 0 ] = ( GLint )v1;  d[ 1 ] = ( GLint )v2;
}
static inline void fSet( GLint *d, int v1, int v2, int v3 ){
	d[ 0 ] = ( GLint )v1;  d[ 1 ] = ( GLint )v2;
	d[ 2 ] = ( GLint )v3;
}
static inline void fSet( GLint *d, int v1, int v2, int v3, int v4 ){
	d[ 0 ] = ( GLint )v1;  d[ 1 ] = ( GLint )v2;
	d[ 2 ] = ( GLint )v3;  d[ 3 ] = ( GLint )v4;
}

static inline void fSet( GLuint *d, bool v ){
	d[ 0 ] = ( GLuint )v;
}
static inline void fSet( GLuint *d, bool v1, bool v2 ){
	d[ 0 ] = ( GLuint )v1;  d[ 1 ] = ( GLuint )v2;
}
static inline void fSet( GLuint *d, bool v1, bool v2, bool v3 ){
	d[ 0 ] = ( GLuint )v1;  d[ 1 ] = ( GLuint )v2;
	d[ 2 ] = ( GLuint )v3;
}
static inline void fSet( GLuint *d, bool v1, bool v2, bool v3, bool v4 ){
	d[ 0 ] = ( GLuint )v1;  d[ 1 ] = ( GLuint )v2;
	d[ 2 ] = ( GLuint )v3;  d[ 3 ] = ( GLuint )v4;
}

static inline void fSet( GLuint *d, unsigned int v ){
	d[ 0 ] = ( GLuint )v;
}
static inline void fSet( GLuint *d, unsigned int v1, unsigned int v2 ){
	d[ 0 ] = ( GLuint )v1;  d[ 1 ] = ( GLuint )v2;
}
static inline void fSet( GLuint *d, unsigned int v1, unsigned int v2, unsigned int v3 ){
	d[ 0 ] = ( GLuint )v1;  d[ 1 ] = ( GLuint )v2;
	d[ 2 ] = ( GLuint )v3;
}
static inline void fSet( GLuint *d, unsigned int v1, unsigned int v2, unsigned int v3, unsigned int v4 ){
	d[ 0 ] = ( GLuint )v1;  d[ 1 ] = ( GLuint )v2;
	d[ 2 ] = ( GLuint )v3;  d[ 3 ] = ( GLuint )v4;
}



// Class deoglShaderParameterBlock
////////////////////

// Constructor, destructor
////////////////////////////

deoglShaderParameterBlock::deoglShaderParameterBlock( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),

pParameters( NULL ),
pParameterCount( 0 ),
pRowMajor( true ),

pElementCount( 1 ),
pElementStride( 0 ),
pOffsetPadding( 0 ),

pBufferSize( 0 ),
pMapped( NULL ),
pElementLower( 0 ),
pElementUpper( 0 ){
}

deoglShaderParameterBlock::deoglShaderParameterBlock( const deoglShaderParameterBlock &paramBlock ) :
pRenderThread( paramBlock.pRenderThread ),

pParameters( NULL ),
pParameterCount( 0 ),
pRowMajor( paramBlock.pRowMajor ),

pElementCount( paramBlock.pElementCount ),
pElementStride( paramBlock.pElementStride ),
pOffsetPadding( paramBlock.pOffsetPadding ),

pBufferSize( paramBlock.pBufferSize ),
pMapped( NULL ),
pElementLower( 0 ),
pElementUpper( 0 )
{
	if( paramBlock.pParameterCount == 0 ){
		return;
	}
	
	pParameters = new deoglSPBParameter[ paramBlock.pParameterCount ];
	pParameterCount = paramBlock.pParameterCount;
	
	int i;
	for( i=0; i<pParameterCount; i++ ){
		const deoglSPBParameter &src = paramBlock.pParameters[ i ];
		deoglSPBParameter &dest = pParameters[ i ];
		
		dest.SetAll( src.GetValueType(), src.GetComponentCount(),
			src.GetVectorCount(), src.GetArrayCount() );
		dest.SetOffset( src.GetOffset() );
		dest.SetStride( src.GetStride() );
		dest.SetArrayStride( src.GetArrayStride() );
		dest.SetDataSize( src.GetDataSize() );
	}
}

deoglShaderParameterBlock::~deoglShaderParameterBlock(){
	if( pParameters ){
		delete [] pParameters;
	}
}



// Management
///////////////

void deoglShaderParameterBlock::SetParameterCount( int count ){
	if( count < 0 || pMapped ){
		DETHROW( deeInvalidParam );
	}
	
	if( pParameters ){
		delete [] pParameters;
		pParameters = NULL;
		pParameterCount = 0;
	}
	
	if( count > 0 ){
		pParameters = new deoglSPBParameter[ count ];
		pParameterCount = count;
	}
	
	pUpdateBufferSize();
}

deoglSPBParameter &deoglShaderParameterBlock::GetParameterAt( int index ) const{
	if( index < 0 || index >= pParameterCount ){
		DETHROW( deeInvalidParam );
	}
	return pParameters[ index ];
}

void deoglShaderParameterBlock::SetRowMajor( bool rowMajor ){
	if( pMapped ){
		DETHROW( deeInvalidParam );
	}
	pRowMajor = rowMajor;
}

void deoglShaderParameterBlock::SetElementCount( int count ){
	if( count < 0 || pMapped ){
		DETHROW( deeInvalidParam );
	}
	
	pElementCount = count;
	pUpdateBufferSize();
}

void deoglShaderParameterBlock::CalculateOffsetPadding(){
	const int alignment = GetRenderThread().GetCapabilities().GetUBOOffsetAlignment();
	pOffsetPadding = ( alignment - ( pElementStride % alignment ) ) % alignment;
	pSetElementStride( pElementStride + pOffsetPadding );
}

void deoglShaderParameterBlock::MapToStd140(){
	if( pMapped ){
		DETHROW( deeInvalidParam );
	}
	
	int i, alignment, stride, adjust, chunkOffset = 0;
	int componentCount, vectorCount, elementStride = 0;
	
	for( i=0; i<pParameterCount; i++ ){
		deoglSPBParameter &parameter = pParameters[ i ];
		
		if( parameter.GetVectorCount() == 1 || pRowMajor ){
			componentCount = parameter.GetComponentCount();
			vectorCount = parameter.GetVectorCount();
			
		}else{
			componentCount = parameter.GetVectorCount();
			vectorCount = parameter.GetComponentCount();
		}
		
		stride = componentCount;
		alignment = stride;
		if( stride == 3 ){ // 3-component requires 4-component alignment
			alignment = 4;
		}
		if( vectorCount > 1 ){ // matrices require 4-component alignment
			alignment = 4;
			stride = 4;
		}
		if( parameter.GetArrayCount() > 1 ){ // arrays require 4-component alignment
			alignment = 4;
			stride = 4;
		}
		
		adjust = ( alignment - ( chunkOffset % alignment ) ) % alignment;
		elementStride += adjust * 4;
		chunkOffset += adjust;
		
		if( chunkOffset + stride > 4 ){
			elementStride += ( 4 - chunkOffset ) * 4;
			chunkOffset = 0;
		}
		
		parameter.SetOffset( elementStride );
		parameter.SetStride( stride * 4 );
		parameter.SetArrayStride( parameter.GetStride() * vectorCount );
		parameter.SetDataSize( parameter.GetArrayStride() * parameter.GetArrayCount() );
		
		chunkOffset += stride;
		elementStride += parameter.GetDataSize();
	}
	
	// element stride is aligned like arrays to 16-byte boundary
	adjust = ( 16 - ( elementStride % 16 ) ) % 16;
	pSetElementStride( elementStride + adjust );
}



bool deoglShaderParameterBlock::IsBufferMapped() const{
	return pMapped != NULL;
}

void deoglShaderParameterBlock::SetParameterDataFloat( int p, float v ){
	SetParameterDataFloat( p, pElementLower, v );
}

void deoglShaderParameterBlock::SetParameterDataVec2( int p, float v1, float v2 ){
	SetParameterDataVec2( p, pElementLower, v1, v2 );
}
void deoglShaderParameterBlock::SetParameterDataVec2( int p, const decVector2 &v ){
	SetParameterDataVec2( p, pElementLower, v );
}

void deoglShaderParameterBlock::SetParameterDataVec3( int p, float v1, float v2, float v3 ){
	SetParameterDataVec3( p, pElementLower, v1, v2, v3 );
}
void deoglShaderParameterBlock::SetParameterDataVec3( int p, const decColor &c ){
	SetParameterDataVec3( p, pElementLower, c );
}
void deoglShaderParameterBlock::SetParameterDataVec3( int p, const decVector &v ){
	SetParameterDataVec3( p, pElementLower, v );
}
void deoglShaderParameterBlock::SetParameterDataVec3( int p, const decDVector &v ){
	SetParameterDataVec3( p, pElementLower, v );
}

void deoglShaderParameterBlock::SetParameterDataVec4( int p, float v1, float v2, float v3, float v4 ){
	SetParameterDataVec4( p, pElementLower, v1, v2, v3, v4 );
}
void deoglShaderParameterBlock::SetParameterDataVec4( int p, const decColor &c ){
	SetParameterDataVec4( p, pElementLower, c );
}
void deoglShaderParameterBlock::SetParameterDataVec4( int p, const decColor &c, float a ){
	SetParameterDataVec4( p, pElementLower, c, a );
}
void deoglShaderParameterBlock::SetParameterDataVec4( int p, const decVector4 &v ){
	SetParameterDataVec4( p, pElementLower, v );
}
void deoglShaderParameterBlock::SetParameterDataVec4( int p, const decDVector4 &v ){
	SetParameterDataVec4( p, pElementLower, v );
}
void deoglShaderParameterBlock::SetParameterDataVec4( int p, const decVector &v, float w ){
	SetParameterDataVec4( p, pElementLower, v, w );
}
void deoglShaderParameterBlock::SetParameterDataVec4( int p, const decDVector4 &v, double w ){
	SetParameterDataVec4( p, pElementLower, v, w );
}

void deoglShaderParameterBlock::SetParameterDataMat3x2( int p, const float *v ){
	SetParameterDataMat3x2( p, pElementLower, v );
}
void deoglShaderParameterBlock::SetParameterDataMat3x2( int p, const decMatrix &m ){
	SetParameterDataMat3x2( p, pElementLower, m );
}
void deoglShaderParameterBlock::SetParameterDataMat3x2( int p, const decDMatrix &m ){
	SetParameterDataMat3x2( p, pElementLower, m );
}
void deoglShaderParameterBlock::SetParameterDataMat3x2( int p, const decTexMatrix &m ){
	SetParameterDataMat3x2( p, pElementLower, m );
}
void deoglShaderParameterBlock::SetParameterDataMat3x2( int p, const decTexMatrix2 &m ){
	SetParameterDataMat3x2( p, pElementLower, m );
}

void deoglShaderParameterBlock::SetParameterDataMat3x3( int p, const float *v ){
	SetParameterDataMat3x3( p, pElementLower, v );
}
void deoglShaderParameterBlock::SetParameterDataMat3x3( int p, const decMatrix &m ){
	SetParameterDataMat3x3( p, pElementLower, m );
}
void deoglShaderParameterBlock::SetParameterDataMat3x3( int p, const decDMatrix &m ){
	SetParameterDataMat3x3( p, pElementLower, m );
}
void deoglShaderParameterBlock::SetParameterDataMat3x3( int p, const decTexMatrix &m ){
	SetParameterDataMat3x3( p, pElementLower, m );
}

void deoglShaderParameterBlock::SetParameterDataMat4x3( int p, const float *v ){
	SetParameterDataMat4x3( p, pElementLower, v );
}
void deoglShaderParameterBlock::SetParameterDataMat4x3( int p, const decMatrix &m ){
	SetParameterDataMat4x3( p, pElementLower, m );
}
void deoglShaderParameterBlock::SetParameterDataMat4x3( int p, const decDMatrix &m ){
	SetParameterDataMat4x3( p, pElementLower, m );
}

void deoglShaderParameterBlock::SetParameterDataMat4x4( int p, const float *v ){
	SetParameterDataMat4x4( p, pElementLower, v );
}
void deoglShaderParameterBlock::SetParameterDataMat4x4( int p, const decMatrix &m ){
	SetParameterDataMat4x4( p, pElementLower, m );
}
void deoglShaderParameterBlock::SetParameterDataMat4x4( int p, const decDMatrix &m ){
	SetParameterDataMat4x4( p, pElementLower, m );
}

void deoglShaderParameterBlock::SetParameterDataInt( int p, int v ){
	SetParameterDataInt( p, pElementLower, v );
}
void deoglShaderParameterBlock::SetParameterDataIVec2( int p, int v1, int v2 ){
	SetParameterDataIVec2( p, pElementLower, v1, v2 );
}
void deoglShaderParameterBlock::SetParameterDataIVec2( int p, const decPoint &pt ){
	SetParameterDataIVec2( p, pElementLower, pt.x, pt.y );
}
void deoglShaderParameterBlock::SetParameterDataIVec3( int p, int v1, int v2, int v3 ){
	SetParameterDataIVec3( p, pElementLower, v1, v2, v3 );
}
void deoglShaderParameterBlock::SetParameterDataIVec3( int p, const decPoint3 &v ){
	SetParameterDataIVec3( p, pElementLower, v.x, v.y, v.z );
}

void deoglShaderParameterBlock::SetParameterDataIVec4( int p, int v1, int v2, int v3, int v4 ){
	SetParameterDataIVec4( p, pElementLower, v1, v2, v3, v4 );
}

void deoglShaderParameterBlock::SetParameterDataUInt( int p, unsigned int v ){
	SetParameterDataUInt( p, pElementLower, v );
}
void deoglShaderParameterBlock::SetParameterDataUVec2( int p, unsigned int v1, unsigned int v2 ){
	SetParameterDataUVec2( p, pElementLower, v1, v2 );
}
void deoglShaderParameterBlock::SetParameterDataUVec3( int p, unsigned int v1, unsigned int v2, unsigned int v3 ){
	SetParameterDataUVec3( p, pElementLower, v1, v2, v3 );
}
void deoglShaderParameterBlock::SetParameterDataUVec4( int p, unsigned int v1, unsigned int v2, unsigned int v3, unsigned int v4 ){
	SetParameterDataUVec4( p, pElementLower, v1, v2, v3, v4 );
}

void deoglShaderParameterBlock::SetParameterDataBool( int p, bool v ){
	SetParameterDataBool( p, pElementLower, v );
}
void deoglShaderParameterBlock::SetParameterDataBVec2( int p, bool v1, bool v2 ){
	SetParameterDataBVec2( p, pElementLower, v1, v2 );
}
void deoglShaderParameterBlock::SetParameterDataBVec3( int p, bool v1, bool v2, bool v3 ){
	SetParameterDataBVec3( p, pElementLower, v1, v2, v3 );
}
void deoglShaderParameterBlock::SetParameterDataBVec4( int p, bool v1, bool v2, bool v3, bool v4 ){
	SetParameterDataBVec4( p, pElementLower, v1, v2, v3, v4 );
}

void deoglShaderParameterBlock::SetParameterDataArrayFloat( int p, int a, float v ){
	SetParameterDataArrayFloat( p, pElementLower, a, v );
}

void deoglShaderParameterBlock::SetParameterDataArrayVec2( int p, int a, float v1, float v2 ){
	SetParameterDataArrayVec2( p, pElementLower, a, v1, v2 );
}
void deoglShaderParameterBlock::SetParameterDataArrayVec2( int p, int a, const decVector2 &v ) {
	SetParameterDataArrayVec2( p, pElementLower, a, v );
}

void deoglShaderParameterBlock::SetParameterDataArrayVec3( int p, int a, float v1, float v2, float v3 ){
	SetParameterDataArrayVec3( p, pElementLower, a, v1, v2, v3 );
}
void deoglShaderParameterBlock::SetParameterDataArrayVec3( int p, int a, const decColor &c ){
	SetParameterDataArrayVec3( p, pElementLower, a, c );
}
void deoglShaderParameterBlock::SetParameterDataArrayVec3( int p, int a, const decVector &v ){
	SetParameterDataArrayVec3( p, pElementLower, a, v );
}
void deoglShaderParameterBlock::SetParameterDataArrayVec3( int p, int a, const decDVector &v ){
	SetParameterDataArrayVec3( p, pElementLower, a, v );
}

void deoglShaderParameterBlock::SetParameterDataArrayVec4( int p, int a, float v1, float v2, float v3, float v4 ){
	SetParameterDataArrayVec4( p, pElementLower, a, v1, v2, v3, v4 );
}
void deoglShaderParameterBlock::SetParameterDataArrayVec4( int p, int a, const decColor &c ){
	SetParameterDataArrayVec4( p, pElementLower, a, c );
}
void deoglShaderParameterBlock::SetParameterDataArrayVec4( int p, int a, const decColor &c, float al ){
	SetParameterDataArrayVec4( p, pElementLower, a, c, al );
}
void deoglShaderParameterBlock::SetParameterDataArrayVec4( int p, int a, const decVector4 &v ){
	SetParameterDataArrayVec4( p, pElementLower, a, v );
}
void deoglShaderParameterBlock::SetParameterDataArrayVec4( int p, int a, const decDVector4 &v ){
	SetParameterDataArrayVec4( p, pElementLower, a, v );
}
void deoglShaderParameterBlock::SetParameterDataArrayVec4( int p, int a, const decVector &v, float w ){
	SetParameterDataArrayVec4( p, pElementLower, a, v, w );
}
void deoglShaderParameterBlock::SetParameterDataArrayVec4( int p, int a, const decDVector &v, double w ){
	SetParameterDataArrayVec4( p, pElementLower, a, v, w );
}

void deoglShaderParameterBlock::SetParameterDataArrayMat3x2( int p, int a, const float *v ){
	SetParameterDataArrayMat3x2( p, pElementLower, a, v );
}
void deoglShaderParameterBlock::SetParameterDataArrayMat3x2( int p, int a, const decMatrix &m ){
	SetParameterDataArrayMat3x2( p, pElementLower, a, m );
}
void deoglShaderParameterBlock::SetParameterDataArrayMat3x2( int p, int a, const decDMatrix &m ){
	SetParameterDataArrayMat3x2( p, pElementLower, a, m );
}
void deoglShaderParameterBlock::SetParameterDataArrayMat3x2( int p, int a, const decTexMatrix &m ){
	SetParameterDataArrayMat3x2( p, pElementLower, a, m );
}
void deoglShaderParameterBlock::SetParameterDataArrayMat3x2( int p, int a, const decTexMatrix2 &m ){
	SetParameterDataArrayMat3x2( p, pElementLower, a, m );
}

void deoglShaderParameterBlock::SetParameterDataArrayMat3x3( int p, int a, const float *v ){
	SetParameterDataArrayMat3x3( p, pElementLower, a, v );
}
void deoglShaderParameterBlock::SetParameterDataArrayMat3x3( int p, int a, const decMatrix &m ){
	SetParameterDataArrayMat3x3( p, pElementLower, a, m );
}
void deoglShaderParameterBlock::SetParameterDataArrayMat3x3( int p, int a, const decDMatrix &m ){
	SetParameterDataArrayMat3x3( p, pElementLower, a, m );
}
void deoglShaderParameterBlock::SetParameterDataArrayMat3x3( int p, int a, const decTexMatrix &m ){
	SetParameterDataArrayMat3x3( p, pElementLower, a, m );
}

void deoglShaderParameterBlock::SetParameterDataArrayMat4x3( int p, int a, const float *v ){
	SetParameterDataArrayMat4x3( p, pElementLower, a, v );
}
void deoglShaderParameterBlock::SetParameterDataArrayMat4x3( int p, int a, const decMatrix &m ){
	SetParameterDataArrayMat4x3( p, pElementLower, a, m );
}
void deoglShaderParameterBlock::SetParameterDataArrayMat4x3( int p, int a, const decDMatrix &m ){
	SetParameterDataArrayMat4x3( p, pElementLower, a, m );
}

void deoglShaderParameterBlock::SetParameterDataArrayMat4x4( int p, int a, const float *v ){
	SetParameterDataArrayMat4x4( p, pElementLower, a, v );
}
void deoglShaderParameterBlock::SetParameterDataArrayMat4x4( int p, int a, const decMatrix &m ){
	SetParameterDataArrayMat4x4( p, pElementLower, a, m );
}
void deoglShaderParameterBlock::SetParameterDataArrayMat4x4( int p, int a, const decDMatrix &m ){
	SetParameterDataArrayMat4x4( p, pElementLower, a, m );
}

void deoglShaderParameterBlock::SetParameterDataArrayFloat( int p, float *values, int count ){
	SetParameterDataArrayFloat( p, pElementLower, values, count );
}

void deoglShaderParameterBlock::SetParameterDataArrayInt( int p, int a, int v ){
	SetParameterDataArrayInt( p, pElementLower, a, v );
}
void deoglShaderParameterBlock::SetParameterDataArrayIVec2( int p, int a, int v1, int v2 ){
	SetParameterDataArrayIVec2( p, pElementLower, a, v1, v2 );
}
void deoglShaderParameterBlock::SetParameterDataArrayIVec3( int p, int a, int v1, int v2, int v3 ){
	SetParameterDataArrayIVec3( p, pElementLower, a, v1, v2, v3 );
}
void deoglShaderParameterBlock::SetParameterDataArrayIVec4( int p, int a, int v1, int v2, int v3, int v4 ){
	SetParameterDataArrayIVec4( p, pElementLower, a, v1, v2, v3, v4 );
}
void deoglShaderParameterBlock::SetParameterDataArrayInt( int p, int *values, int count ){
	SetParameterDataArrayInt( p, pElementLower, values, count );
}

void deoglShaderParameterBlock::SetParameterDataArrayUInt( int p, int a, unsigned int v ){
	SetParameterDataArrayUInt( p, pElementLower, a, v );
}
void deoglShaderParameterBlock::SetParameterDataArrayUVec2( int p, int a, unsigned int v1, unsigned int v2 ){
	SetParameterDataArrayUVec2( p, pElementLower, a, v1, v2 );
}
void deoglShaderParameterBlock::SetParameterDataArrayUVec3( int p, int a, unsigned int v1, unsigned int v2, unsigned int v3 ){
	SetParameterDataArrayUVec3( p, pElementLower, a, v1, v2, v3 );
}
void deoglShaderParameterBlock::SetParameterDataArrayUVec4( int p, int a, unsigned int v1, unsigned int v2, unsigned int v3, unsigned int v4 ){
	SetParameterDataArrayUVec4( p, pElementLower, a, v1, v2, v3, v4 );
}
void deoglShaderParameterBlock::SetParameterDataArrayUInt( int p, unsigned int *values, int count ){
	SetParameterDataArrayUInt( p, pElementLower, values, count );
}

void deoglShaderParameterBlock::SetParameterDataArrayBool( int p, int a, bool v ){
	SetParameterDataArrayBool( p, pElementLower, a, v );
}
void deoglShaderParameterBlock::SetParameterDataArrayBVec2( int p, int a, bool v1, bool v2 ){
	SetParameterDataArrayBVec2( p, pElementLower, a, v1, v2 );
}
void deoglShaderParameterBlock::SetParameterDataArrayBVec3( int p, int a, bool v1, bool v2, bool v3 ){
	SetParameterDataArrayBVec3( p, pElementLower, a, v1, v2, v3 );
}
void deoglShaderParameterBlock::SetParameterDataArrayBVec4( int p, int a, bool v1, bool v2, bool v3, bool v4 ){
	SetParameterDataArrayBVec4( p, pElementLower, a, v1, v2, v3, v4 );
}
void deoglShaderParameterBlock::SetParameterDataArrayBool( int p, bool *values, int count ){
	SetParameterDataArrayBool( p, pElementLower, values, count );
}



void deoglShaderParameterBlock::SetParameterDataFloat( int p, int e, float v ){
	fSet( pDataFloat( pParamFloat( p, 1 ), e ), v );
}

void deoglShaderParameterBlock::SetParameterDataVec2( int p, int e, float v1, float v2 ){
	fSet( pDataFloat( pParamFloat( p, 2 ), e ), v1, v2 );
}
void deoglShaderParameterBlock::SetParameterDataVec2( int p, int e, const decVector2 &v ){
	fSet( pDataFloat( pParamFloat( p, 2 ), e ), v.x, v.y );
}

void deoglShaderParameterBlock::SetParameterDataVec3( int p, int e, float v1, float v2, float v3 ){
	fSet( pDataFloat( pParamFloat( p, 3 ), e ), v1, v2, v3 );
}
void deoglShaderParameterBlock::SetParameterDataVec3( int p, int e, const decColor &c ){
	fSet( pDataFloat( pParamFloat( p, 3 ), e ), c.r, c.g, c.b );
}
void deoglShaderParameterBlock::SetParameterDataVec3( int p, int e, const decVector &v ){
	fSet( pDataFloat( pParamFloat( p, 3 ), e ), v.x, v.y, v.z );
}
void deoglShaderParameterBlock::SetParameterDataVec3( int p, int e, const decDVector &v ){
	fSet( pDataFloat( pParamFloat( p, 3 ), e ), v.x, v.y, v.z );
}

void deoglShaderParameterBlock::SetParameterDataVec4( int p, int e, float v1, float v2, float v3, float v4 ){
	fSet( pDataFloat( pParamFloat( p, 4 ), e ), v1, v2, v3, v4 );
}
void deoglShaderParameterBlock::SetParameterDataVec4( int p, int e, const decColor &c ){
	fSet( pDataFloat( pParamFloat( p, 4 ), e ), c.r, c.g, c.b, c.a );
}
void deoglShaderParameterBlock::SetParameterDataVec4( int p, int e, const decColor &c, float a ){
	fSet( pDataFloat( pParamFloat( p, 4 ), e ), c.r, c.g, c.b, a );
}
void deoglShaderParameterBlock::SetParameterDataVec4( int p, int e, const decVector4 &v ){
	fSet( pDataFloat( pParamFloat( p, 4 ), e ), v.x, v.y, v.z, v.w );
}
void deoglShaderParameterBlock::SetParameterDataVec4( int p, int e, const decDVector4 &v ){
	fSet( pDataFloat( pParamFloat( p, 4 ), e ), v.x, v.y, v.z, v.w );
}
void deoglShaderParameterBlock::SetParameterDataVec4( int p, int e, const decVector &v, float w ){
	fSet( pDataFloat( pParamFloat( p, 4 ), e ), v.x, v.y, v.z, w );
}
void deoglShaderParameterBlock::SetParameterDataVec4( int p, int e, const decDVector4 &v, double w ){
	fSet( pDataFloat( pParamFloat( p, 4 ), e ), v.x, v.y, v.z, w );
}


void deoglShaderParameterBlock::SetParameterDataMat3x2( int p, int e, const float *v ){
	GLfloat *d[ 3 ];
	if( pRowMajor ){
		pDataMatrix2( pParamFloat( p, 3, 2 ), e, d );
		fSet( d[ 0 ], v[ 0 ], v[ 1 ], v[ 2 ] );
		fSet( d[ 1 ], v[ 3 ], v[ 4 ], v[ 5 ] );
		
	}else{
		pDataMatrix3( pParamFloat( p, 3, 2 ), e, d );
		fSet( d[ 0 ], v[ 0 ], v[ 3 ] );
		fSet( d[ 1 ], v[ 1 ], v[ 4 ] );
		fSet( d[ 2 ], v[ 2 ], v[ 5 ] );
	}
}

#define IMPL_SET_PARAM_DATA_MAT_3x2(p,e,m) \
	GLfloat *d[ 3 ]; \
	if( pRowMajor ){ \
		pDataMatrix2( pParamFloat( p, 3, 2 ), e, d ); \
		fSet( d[ 0 ], m.a11, m.a12, m.a13 ); \
		fSet( d[ 1 ], m.a21, m.a22, m.a23 ); \
	}else{ \
		pDataMatrix3( pParamFloat( p, 3, 2 ), e, d ); \
		fSet( d[ 0 ], m.a11, m.a21 ); \
		fSet( d[ 1 ], m.a12, m.a22 ); \
		fSet( d[ 2 ], m.a13, m.a23 ); \
	}

void deoglShaderParameterBlock::SetParameterDataMat3x2( int p, int e, const decMatrix &m ){
	IMPL_SET_PARAM_DATA_MAT_3x2( p, e, m )
}
void deoglShaderParameterBlock::SetParameterDataMat3x2( int p, int e, const decDMatrix &m ){
	IMPL_SET_PARAM_DATA_MAT_3x2( p, e, m )
}
void deoglShaderParameterBlock::SetParameterDataMat3x2( int p, int e, const decTexMatrix &m ){
	IMPL_SET_PARAM_DATA_MAT_3x2( p, e, m )
}
void deoglShaderParameterBlock::SetParameterDataMat3x2( int p, int e, const decTexMatrix2 &m ){
	IMPL_SET_PARAM_DATA_MAT_3x2( p, e, m )
}


void deoglShaderParameterBlock::SetParameterDataMat3x3( int p, int e, const float *v ){
	GLfloat *d[ 3 ];
	pDataMatrix3( pParamFloat( p, 3, 3 ), e, d );
	
	if( pRowMajor ){
		fSet( d[ 0 ], v[ 0 ], v[ 1 ], v[ 2 ] );
		fSet( d[ 1 ], v[ 3 ], v[ 4 ], v[ 5 ] );
		fSet( d[ 2 ], v[ 6 ], v[ 7 ], v[ 8 ] );
		
	}else{
		fSet( d[ 0 ], v[ 0 ], v[ 3 ], v[ 6 ] );
		fSet( d[ 1 ], v[ 1 ], v[ 4 ], v[ 7 ] );
		fSet( d[ 2 ], v[ 2 ], v[ 5 ], v[ 8 ] );
	}
}

#define IMPL_SET_PARAM_DATA_MAT_3x3(p,e,m) \
	GLfloat *d[ 3 ]; \
	pDataMatrix3( pParamFloat( p, 3, 3 ), e, d ); \
	if( pRowMajor ){ \
		fSet( d[ 0 ], m.a11, m.a12, m.a13 ); \
		fSet( d[ 1 ], m.a21, m.a22, m.a23 ); \
		fSet( d[ 2 ], m.a31, m.a32, m.a33 ); \
	}else{ \
		fSet( d[ 0 ], m.a11, m.a21, m.a31 ); \
		fSet( d[ 1 ], m.a12, m.a22, m.a32 ); \
		fSet( d[ 2 ], m.a13, m.a23, m.a33 ); \
	}

void deoglShaderParameterBlock::SetParameterDataMat3x3( int p, int e, const decMatrix &m ){
	IMPL_SET_PARAM_DATA_MAT_3x3( p, e, m )
}
void deoglShaderParameterBlock::SetParameterDataMat3x3( int p, int e, const decDMatrix &m ){
	IMPL_SET_PARAM_DATA_MAT_3x3( p, e, m )
}
void deoglShaderParameterBlock::SetParameterDataMat3x3( int p, int e, const decTexMatrix &m ){
	IMPL_SET_PARAM_DATA_MAT_3x3( p, e, m )
}

void deoglShaderParameterBlock::SetParameterDataMat4x3( int p, int e, const float *v ){
	GLfloat *d[ 4 ];
	if( pRowMajor ){
		pDataMatrix3( pParamFloat( p, 4, 3 ), e, d );
		fSet( d[ 0 ], v[ 0 ], v[ 1 ], v[ 2 ], v[ 3 ] );
		fSet( d[ 1 ], v[ 4 ], v[ 5 ], v[ 6 ], v[ 7 ] );
		fSet( d[ 2 ], v[ 8 ], v[ 9 ], v[ 10 ], v[ 11 ] );
		
	}else{
		pDataMatrix4( pParamFloat( p, 4, 3 ), e, d );
		fSet( d[ 0 ], v[ 0 ], v[ 4 ], v[ 8 ] );
		fSet( d[ 1 ], v[ 1 ], v[ 5 ], v[ 9 ] );
		fSet( d[ 2 ], v[ 2 ], v[ 6 ], v[ 10 ] );
		fSet( d[ 3 ], v[ 3 ], v[ 7 ], v[ 11 ] );
	}
}

#define IMPL_SET_PARAM_DATA_MAT_4x3(p,e,m) \
	GLfloat *d[ 4 ]; \
	if( pRowMajor ){ \
		pDataMatrix3( pParamFloat( p, 4, 3 ), e, d ); \
		fSet( d[ 0 ], m.a11, m.a12, m.a13, m.a14 ); \
		fSet( d[ 1 ], m.a21, m.a22, m.a23, m.a24 ); \
		fSet( d[ 2 ], m.a31, m.a32, m.a33, m.a34 ); \
	}else{ \
		pDataMatrix4( pParamFloat( p, 4, 3 ), e, d ); \
		fSet( d[ 0 ], m.a11, m.a21, m.a31 ); \
		fSet( d[ 1 ], m.a12, m.a22, m.a32 ); \
		fSet( d[ 2 ], m.a13, m.a23, m.a33 ); \
		fSet( d[ 3 ], m.a14, m.a24, m.a34 ); \
	}

void deoglShaderParameterBlock::SetParameterDataMat4x3( int p, int e, const decMatrix &m ){
	IMPL_SET_PARAM_DATA_MAT_4x3( p, e, m )
}
void deoglShaderParameterBlock::SetParameterDataMat4x3( int p, int e, const decDMatrix &m ){
	IMPL_SET_PARAM_DATA_MAT_4x3( p, e, m )
}


void deoglShaderParameterBlock::SetParameterDataMat4x4( int p, int e, const float *v ){
	GLfloat *d[ 4 ];
	pDataMatrix4( pParamFloat( p, 4, 4 ), e, d );
	
	if( pRowMajor ){
		fSet( d[ 0 ], v[ 0 ], v[ 1 ], v[ 2 ], v[ 3 ] );
		fSet( d[ 1 ], v[ 4 ], v[ 5 ], v[ 6 ], v[ 7 ] );
		fSet( d[ 2 ], v[ 8 ], v[ 9 ], v[ 10 ], v[ 11 ] );
		fSet( d[ 3 ], v[ 12 ], v[ 13 ], v[ 14 ], v[ 15 ] );
		
	}else{
		fSet( d[ 0 ], v[ 0 ], v[ 4 ], v[ 8 ], v[ 12 ] );
		fSet( d[ 1 ], v[ 1 ], v[ 5 ], v[ 9 ], v[ 13 ] );
		fSet( d[ 2 ], v[ 2 ], v[ 6 ], v[ 10 ], v[ 14 ] );
		fSet( d[ 3 ], v[ 3 ], v[ 7 ], v[ 11 ], v[ 15 ] );
	}
}

#define IMPL_SET_PARAM_DATA_MAT_4x4(p,e,m) \
	GLfloat *d[ 4 ]; \
	pDataMatrix4( pParamFloat( p, 4, 4 ), e, d ); \
	if( pRowMajor ){ \
		fSet( d[ 0 ], m.a11, m.a12, m.a13, m.a14 ); \
		fSet( d[ 1 ], m.a21, m.a22, m.a23, m.a24 ); \
		fSet( d[ 2 ], m.a31, m.a32, m.a33, m.a34 ); \
		fSet( d[ 3 ], m.a41, m.a42, m.a43, m.a44 ); \
	}else{ \
		fSet( d[ 0 ], m.a11, m.a21, m.a31, m.a41 ); \
		fSet( d[ 1 ], m.a12, m.a22, m.a32, m.a42 ); \
		fSet( d[ 2 ], m.a13, m.a23, m.a33, m.a43 ); \
		fSet( d[ 3 ], m.a14, m.a24, m.a34, m.a44 ); \
	}

void deoglShaderParameterBlock::SetParameterDataMat4x4( int p, int e, const decMatrix &m ){
	IMPL_SET_PARAM_DATA_MAT_4x4( p, e, m )
}
void deoglShaderParameterBlock::SetParameterDataMat4x4( int p, int e, const decDMatrix &m ){
	IMPL_SET_PARAM_DATA_MAT_4x4( p, e, m )
}

void deoglShaderParameterBlock::SetParameterDataInt( int p, int e, int v ){
	fSet( pDataInt( pParamInt( p, 1 ), e ), v );
}
void deoglShaderParameterBlock::SetParameterDataIVec2( int p, int e, int v1, int v2 ){
	fSet( pDataInt( pParamInt( p, 2 ), e ), v1, v2 );
}
void deoglShaderParameterBlock::SetParameterDataIVec3( int p, int e, int v1, int v2, int v3 ){
	fSet( pDataInt( pParamInt( p, 3 ), e ), v1, v2, v3 );
}
void deoglShaderParameterBlock::SetParameterDataIVec4( int p, int e, int v1, int v2, int v3, int v4 ){
	fSet( pDataInt( pParamInt( p, 4 ), e ), v1, v2, v3, v4 );
}

void deoglShaderParameterBlock::SetParameterDataUInt( int p, int e, unsigned int v ){
	fSet( pDataUInt( pParamInt( p, 1 ), e ), v );
}
void deoglShaderParameterBlock::SetParameterDataUVec2( int p, int e, unsigned int v1, unsigned int v2 ){
	fSet( pDataUInt( pParamInt( p, 2 ), e ), v1, v2 );
}
void deoglShaderParameterBlock::SetParameterDataUVec3( int p, int e, unsigned int v1, unsigned int v2, unsigned int v3 ){
	fSet( pDataUInt( pParamInt( p, 3 ), e ), v1, v2, v3 );
}
void deoglShaderParameterBlock::SetParameterDataUVec4( int p, int e, unsigned int v1, unsigned int v2, unsigned int v3, unsigned int v4 ){
	fSet( pDataUInt( pParamInt( p, 4 ), e ), v1, v2, v3, v4 );
}

void deoglShaderParameterBlock::SetParameterDataBool( int p, int e, bool v ){
	fSet( pDataUInt( pParamBool( p, 1 ), e ), v );
}
void deoglShaderParameterBlock::SetParameterDataBVec2( int p, int e, bool v1, bool v2 ){
	fSet( pDataUInt( pParamBool( p, 2 ), e ), v1, v2 );
}
void deoglShaderParameterBlock::SetParameterDataBVec3( int p, int e, bool v1, bool v2, bool v3 ){
	fSet( pDataUInt( pParamBool( p, 3 ), e ), v1, v2, v3 );
}
void deoglShaderParameterBlock::SetParameterDataBVec4( int p, int e, bool v1, bool v2, bool v3, bool v4 ){
	fSet( pDataUInt( pParamBool( p, 4 ), e ), v1, v2, v3, v4 );
}

void deoglShaderParameterBlock::SetParameterDataArrayFloat( int p, int e, int a, float v ){
	fSet( pDataFloat( pParamFloat( p, 1, 1, a ), e, a ), v );
}

void deoglShaderParameterBlock::SetParameterDataArrayVec2( int p, int e, int a, float v1, float v2 ){
	fSet( pDataFloat( pParamFloat( p, 2, 1, a ), e, a ), v1, v2 );
}
void deoglShaderParameterBlock::SetParameterDataArrayVec2( int p, int e, int a, const decVector2& v ) {
	fSet( pDataFloat( pParamFloat( p, 2, 1, a ), e, a ), v.x, v.y );
}

void deoglShaderParameterBlock::SetParameterDataArrayVec3( int p, int e, int a, float v1, float v2, float v3 ){
	fSet( pDataFloat( pParamFloat( p, 3, 1, a ), e, a ), v1, v2, v3 );
}
void deoglShaderParameterBlock::SetParameterDataArrayVec3( int p, int e, int a, const decColor &c ){
	fSet( pDataFloat( pParamFloat( p, 3, 1, a ), e, a ), c.r, c.g, c.b );
}
void deoglShaderParameterBlock::SetParameterDataArrayVec3( int p, int e, int a, const decVector &v ){
	fSet( pDataFloat( pParamFloat( p, 3, 1, a ), e, a ), v.x, v.y, v.z );
}
void deoglShaderParameterBlock::SetParameterDataArrayVec3( int p, int e, int a, const decDVector &v ){
	fSet( pDataFloat( pParamFloat( p, 3, 1, a ), e, a ), v.x, v.y, v.z );
}

void deoglShaderParameterBlock::SetParameterDataArrayVec4( int p, int e, int a, float v1, float v2, float v3, float v4 ){
	fSet( pDataFloat( pParamFloat( p, 4, 1, a ), e, a ), v1, v2, v3, v4 );
}
void deoglShaderParameterBlock::SetParameterDataArrayVec4( int p, int e, int a, const decColor &c ){
	fSet( pDataFloat( pParamFloat( p, 4, 1, a ), e, a ), c.r, c.g, c.b, c.a );
}
void deoglShaderParameterBlock::SetParameterDataArrayVec4( int p, int e, int a, const decColor &c, float al ){
	fSet( pDataFloat( pParamFloat( p, 4, 1, a ), e, a ), c.r, c.g, c.b, al );
}
void deoglShaderParameterBlock::SetParameterDataArrayVec4( int p, int e, int a, const decVector4 &v ){
	fSet( pDataFloat( pParamFloat( p, 4, 1, a ), e, a ), v.x, v.y, v.z, v.w );
}
void deoglShaderParameterBlock::SetParameterDataArrayVec4( int p, int e, int a, const decDVector4 &v ){
	fSet( pDataFloat( pParamFloat( p, 4, 1, a ), e, a ), v.x, v.y, v.z, v.w );
}
void deoglShaderParameterBlock::SetParameterDataArrayVec4( int p, int e, int a, const decVector &v, float w ){
	fSet( pDataFloat( pParamFloat( p, 4, 1, a ), e, a ), v.x, v.y, v.z, w );
}
void deoglShaderParameterBlock::SetParameterDataArrayVec4( int p, int e, int a, const decDVector &v, double w ){
	fSet( pDataFloat( pParamFloat( p, 4, 1, a ), e, a ), v.x, v.y, v.z, w );
}

void deoglShaderParameterBlock::SetParameterDataArrayMat3x2( int p, int e, int a, const float *v ){
	GLfloat *d[ 3 ];
	if( pRowMajor ){
		pDataMatrix2( pParamFloat( p, 3, 2, a ), e, d, a );
		fSet( d[ 0 ], v[ 0 ], v[ 1 ], v[ 2 ] );
		fSet( d[ 1 ], v[ 3 ], v[ 4 ], v[ 5 ] );
		
	}else{
		pDataMatrix3( pParamFloat( p, 3, 2, a ), e, d, a );
		fSet( d[ 0 ], v[ 0 ], v[ 3 ] );
		fSet( d[ 1 ], v[ 1 ], v[ 4 ] );
		fSet( d[ 2 ], v[ 2 ], v[ 5 ] );
	}
}

#define IMPL_SET_PARAM_DATA_ARRAY_MAT_3x2(p,e,a,m) \
	GLfloat *d[ 3 ]; \
	if( pRowMajor ){ \
		pDataMatrix2( pParamFloat( p, 3, 2, a ), e, d, a ); \
		fSet( d[ 0 ], m.a11, m.a12, m.a13 ); \
		fSet( d[ 1 ], m.a21, m.a22, m.a23 ); \
	}else{ \
		pDataMatrix3( pParamFloat( p, 3, 2, a ), e, d, a ); \
		fSet( d[ 0 ], m.a11, m.a21 ); \
		fSet( d[ 1 ], m.a12, m.a22 ); \
		fSet( d[ 2 ], m.a13, m.a23 ); \
	}

void deoglShaderParameterBlock::SetParameterDataArrayMat3x2( int p, int e, int a, const decMatrix &m ){
	IMPL_SET_PARAM_DATA_ARRAY_MAT_3x2( p, e, a, m )
}
void deoglShaderParameterBlock::SetParameterDataArrayMat3x2( int p, int e, int a, const decDMatrix &m ){
	IMPL_SET_PARAM_DATA_ARRAY_MAT_3x2( p, e, a, m )
}
void deoglShaderParameterBlock::SetParameterDataArrayMat3x2( int p, int e, int a, const decTexMatrix &m ){
	IMPL_SET_PARAM_DATA_ARRAY_MAT_3x2( p, e, a, m )
}
void deoglShaderParameterBlock::SetParameterDataArrayMat3x2( int p, int e, int a, const decTexMatrix2 &m ){
	IMPL_SET_PARAM_DATA_ARRAY_MAT_3x2( p, e, a, m )
}

void deoglShaderParameterBlock::SetParameterDataArrayMat3x3( int p, int e, int a, const float *v ){
	GLfloat *d[ 3 ];
	pDataMatrix3( pParamFloat( p, 3, 3, a ), e, d, a );
	
	if( pRowMajor ){
		fSet( d[ 0 ], v[ 0 ], v[ 1 ], v[ 2 ] );
		fSet( d[ 1 ], v[ 3 ], v[ 4 ], v[ 5 ] );
		fSet( d[ 2 ], v[ 6 ], v[ 7 ], v[ 8 ] );
		
	}else{
		fSet( d[ 0 ], v[ 0 ], v[ 3 ], v[ 6 ] );
		fSet( d[ 1 ], v[ 1 ], v[ 4 ], v[ 7 ] );
		fSet( d[ 2 ], v[ 2 ], v[ 5 ], v[ 8 ] );
	}
}

#define IMPL_SET_PARAM_DATA_ARRAY_MAT_3x3(p,e,a,m) \
	GLfloat *d[ 3 ]; \
	pDataMatrix3( pParamFloat( p, 3, 3, a ), e, d, a ); \
	if( pRowMajor ){ \
		fSet( d[ 0 ], m.a11, m.a12, m.a13 ); \
		fSet( d[ 1 ], m.a21, m.a22, m.a23 ); \
		fSet( d[ 2 ], m.a31, m.a32, m.a33 ); \
	}else{ \
		fSet( d[ 0 ], m.a11, m.a21, m.a31 ); \
		fSet( d[ 1 ], m.a12, m.a22, m.a32 ); \
		fSet( d[ 2 ], m.a13, m.a23, m.a33 ); \
	}

void deoglShaderParameterBlock::SetParameterDataArrayMat3x3( int p, int e, int a, const decMatrix &m ){
	IMPL_SET_PARAM_DATA_ARRAY_MAT_3x3( p, e, a, m )
}
void deoglShaderParameterBlock::SetParameterDataArrayMat3x3( int p, int e, int a, const decDMatrix &m ){
	IMPL_SET_PARAM_DATA_ARRAY_MAT_3x3( p, e, a, m )
}
void deoglShaderParameterBlock::SetParameterDataArrayMat3x3( int p, int e, int a, const decTexMatrix &m ){
	IMPL_SET_PARAM_DATA_ARRAY_MAT_3x3( p, e, a, m )
}

void deoglShaderParameterBlock::SetParameterDataArrayMat4x3( int p, int e, int a, const float *v ){
	GLfloat *d[ 4 ];
	if( pRowMajor ){
		pDataMatrix3( pParamFloat( p, 4, 3, a ), e, d, a );
		fSet( d[ 0 ], v[ 0 ], v[ 1 ], v[ 2 ], v[ 3 ] );
		fSet( d[ 1 ], v[ 4 ], v[ 5 ], v[ 6 ], v[ 7 ] );
		fSet( d[ 2 ], v[ 8 ], v[ 9 ], v[ 10 ], v[ 11 ] );
		
	}else{
		pDataMatrix4( pParamFloat( p, 4, 3, a ), e, d, a );
		fSet( d[ 0 ], v[ 0 ], v[ 4 ], v[ 8 ] );
		fSet( d[ 1 ], v[ 1 ], v[ 5 ], v[ 9 ] );
		fSet( d[ 2 ], v[ 2 ], v[ 6 ], v[ 10 ] );
		fSet( d[ 3 ], v[ 3 ], v[ 7 ], v[ 11 ] );
	}
}

#define IMPL_SET_PARAM_DATA_ARRAY_MAT_4x3(p,e,a,m) \
	GLfloat *d[ 4 ]; \
	if( pRowMajor ){ \
		pDataMatrix3( pParamFloat( p, 4, 3, a ), e, d, a ); \
		fSet( d[ 0 ], m.a11, m.a12, m.a13, m.a14 ); \
		fSet( d[ 1 ], m.a21, m.a22, m.a23, m.a24 ); \
		fSet( d[ 2 ], m.a31, m.a32, m.a33, m.a34 ); \
	}else{ \
		pDataMatrix4( pParamFloat( p, 4, 3, a ), e, d, a ); \
		fSet( d[ 0 ], m.a11, m.a21, m.a31 ); \
		fSet( d[ 1 ], m.a12, m.a22, m.a32 ); \
		fSet( d[ 2 ], m.a13, m.a23, m.a33 ); \
		fSet( d[ 3 ], m.a14, m.a24, m.a34 ); \
	}

void deoglShaderParameterBlock::SetParameterDataArrayMat4x3( int p, int e, int a, const decMatrix &m ){
	IMPL_SET_PARAM_DATA_ARRAY_MAT_4x3( p, e, a, m )
}
void deoglShaderParameterBlock::SetParameterDataArrayMat4x3( int p, int e, int a, const decDMatrix &m ){
	IMPL_SET_PARAM_DATA_ARRAY_MAT_4x3( p, e, a, m )
}

void deoglShaderParameterBlock::SetParameterDataArrayMat4x4( int p, int e, int a, const float *v ){
	GLfloat *d[ 4 ];
	pDataMatrix4( pParamFloat( p, 4, 4, a ), e, d, a );
	
	if( pRowMajor ){
		fSet( d[ 0 ], v[ 0 ], v[ 1 ], v[ 2 ], v[ 3 ] );
		fSet( d[ 1 ], v[ 4 ], v[ 5 ], v[ 6 ], v[ 7 ] );
		fSet( d[ 2 ], v[ 8 ], v[ 9 ], v[ 10 ], v[ 11 ] );
		fSet( d[ 3 ], v[ 12 ], v[ 13 ], v[ 14 ], v[ 15 ] );
		
	}else{
		fSet( d[ 0 ], v[ 0 ], v[ 4 ], v[ 8 ], v[ 12 ] );
		fSet( d[ 1 ], v[ 1 ], v[ 5 ], v[ 9 ], v[ 13 ] );
		fSet( d[ 2 ], v[ 2 ], v[ 6 ], v[ 10 ], v[ 14 ] );
		fSet( d[ 3 ], v[ 3 ], v[ 7 ], v[ 11 ], v[ 15 ] );
	}
}

#define IMPL_SET_PARAM_DATA_ARRAY_MAT_4x4(p,e,a,m) \
	GLfloat *d[ 4 ]; \
	pDataMatrix4( pParamFloat( p, 4, 4, a ), e, d, a ); \
	if( pRowMajor ){ \
		fSet( d[ 0 ], m.a11, m.a12, m.a13, m.a14 ); \
		fSet( d[ 1 ], m.a21, m.a22, m.a23, m.a24 ); \
		fSet( d[ 2 ], m.a31, m.a32, m.a33, m.a34 ); \
		fSet( d[ 3 ], m.a41, m.a42, m.a43, m.a44 ); \
	}else{ \
		fSet( d[ 0 ], m.a11, m.a21, m.a31, m.a41 ); \
		fSet( d[ 1 ], m.a12, m.a22, m.a32, m.a42 ); \
		fSet( d[ 2 ], m.a13, m.a23, m.a33, m.a43 ); \
		fSet( d[ 3 ], m.a14, m.a24, m.a34, m.a44 ); \
	}

void deoglShaderParameterBlock::SetParameterDataArrayMat4x4( int p, int e, int a, const decMatrix &m ){
	IMPL_SET_PARAM_DATA_ARRAY_MAT_4x4( p, e, a, m )
}
void deoglShaderParameterBlock::SetParameterDataArrayMat4x4( int p, int e, int a, const decDMatrix &m ){
	IMPL_SET_PARAM_DATA_ARRAY_MAT_4x4( p, e, a, m )
}

void deoglShaderParameterBlock::SetParameterDataArrayFloat( int p, int e, float *values, int count ){
	if( count == 0 ){
		return;
	}
	if( ! values || count < 0 ){
		DETHROW( deeInvalidParam );
	}
	const deoglSPBParameter &parameter = pParamFloat( p, 1, 1, count - 1 );
	char * const mapped = pData( parameter, e, 0 );
	
	const int stride = parameter.GetArrayStride();
	int i;
	
	for( i=0; i<count; i++ ){
		fSet( ( GLfloat* )( mapped + stride * i ), values[ i ] );
	}
}

void deoglShaderParameterBlock::SetParameterDataArrayInt( int p, int e, int a, int v ){
	fSet( pDataInt( pParamInt( p, 1, 1, a ), e, a ), v );
}
void deoglShaderParameterBlock::SetParameterDataArrayIVec2( int p, int e, int a, int v1, int v2 ){
	fSet( pDataInt( pParamInt( p, 2, 1, a ), e, a ), v1, v2 );
}
void deoglShaderParameterBlock::SetParameterDataArrayIVec3( int p, int e, int a, int v1, int v2, int v3 ){
	fSet( pDataInt( pParamInt( p, 3, 1, a ), e, a ), v1, v2, v3 );
}
void deoglShaderParameterBlock::SetParameterDataArrayIVec4( int p, int e, int a, int v1, int v2, int v3, int v4 ){
	fSet( pDataInt( pParamInt( p, 4, 1, a ), e, a ), v1, v2, v3, v4 );
}

void deoglShaderParameterBlock::SetParameterDataArrayInt( int p, int e, int *values, int count ){
	if( count == 0 ){
		return;
	}
	if( ! values || count < 0 ){
		DETHROW( deeInvalidParam );
	}
	const deoglSPBParameter &parameter = pParamInt( p, 1, 1, count - 1 );
	char * const mapped = pData( parameter, e, 0 );
	
	const int stride = parameter.GetArrayStride();
	int i;
	
	for( i=0; i<count; i++ ){
		fSet( ( GLint* )( mapped + stride * i ), values[ i ] );
	}
}

void deoglShaderParameterBlock::SetParameterDataArrayUInt( int p, int e, int a, unsigned int v ){
	fSet( pDataUInt( pParamInt( p, 1, 1, a ), e, a ), v );
}
void deoglShaderParameterBlock::SetParameterDataArrayUVec2( int p, int e, int a,
unsigned int v1, unsigned int v2 ){
	fSet( pDataUInt( pParamInt( p, 2, 1, a ), e, a ), v1, v2 );
}
void deoglShaderParameterBlock::SetParameterDataArrayUVec3( int p, int e, int a,
unsigned int v1, unsigned int v2, unsigned int v3 ){
	fSet( pDataUInt( pParamInt( p, 3, 1, a ), e, a ), v1, v2, v3 );
}
void deoglShaderParameterBlock::SetParameterDataArrayUVec4( int p, int e, int a,
unsigned int v1, unsigned int v2, unsigned int v3, unsigned int v4 ){
	fSet( pDataUInt( pParamInt( p, 4, 1, a ), e, a ), v1, v2, v3, v4 );
}

void deoglShaderParameterBlock::SetParameterDataArrayUInt( int p, int e, unsigned int *values, int count ){
	if( count == 0 ){
		return;
	}
	if( ! values || count < 0 ){
		DETHROW( deeInvalidParam );
	}
	const deoglSPBParameter &parameter = pParamInt( p, 1, 1, count - 1 );
	char * const mapped = pData( parameter, e, 0 );
	
	const int stride = parameter.GetArrayStride();
	int i;
	
	for( i=0; i<count; i++ ){
		fSet( ( GLuint* )( mapped + stride * i ), values[ i ] );
	}
}

void deoglShaderParameterBlock::SetParameterDataArrayBool( int p, int e, int a, bool v ){
	fSet( pDataUInt( pParamBool( p, 1, 1, a ), e, a ), v );
}
void deoglShaderParameterBlock::SetParameterDataArrayBVec2( int p, int e, int a, bool v1, bool v2 ){
	fSet( pDataUInt( pParamBool( p, 2, 1, a ), e, a ), v1, v2 );
}
void deoglShaderParameterBlock::SetParameterDataArrayBVec3( int p, int e, int a, bool v1, bool v2, bool v3 ){
	fSet( pDataUInt( pParamBool( p, 3, 1, a ), e, a ), v1, v2, v3 );
}
void deoglShaderParameterBlock::SetParameterDataArrayBVec4( int p, int e, int a, bool v1, bool v2, bool v3, bool v4 ){
	fSet( pDataUInt( pParamBool( p, 4, 1, a ), e, a ), v1, v2, v3, v4 );
}

void deoglShaderParameterBlock::SetParameterDataArrayBool( int p, int e, bool *values, int count ){
	if( count == 0 ){
		return;
	}
	if( ! values || count < 0 ){
		DETHROW( deeInvalidParam );
	}
	const deoglSPBParameter &parameter = pParamBool( p, 1, 1, count - 1 );
	char * const mapped = pData( parameter, e, 0 );
	
	const int stride = parameter.GetArrayStride();
	int i;
	
	for( i=0; i<count; i++ ){
		fSet( ( GLuint* )( mapped + stride * i ), values[ i ] );
	}
}



// Protected Functions
////////////////////////

void deoglShaderParameterBlock::pSetMapped( char *data ){
	if( pMapped || pBufferSize == 0 ){
		DETHROW( deeInvalidParam );
	}
	if( ! data ){
		DETHROW( deeInvalidParam );
	}
	
	pMapped = data;
	pElementLower = 0;
	pElementUpper = pElementCount - 1;
}

void deoglShaderParameterBlock::pSetMapped( char *data, int element ){
	if( pMapped || pBufferSize == 0 ){
		DETHROW( deeInvalidParam );
	}
	if( ! data || element < 0 || element >= pElementCount ){
		DETHROW( deeInvalidParam );
	}
	
	pMapped = data;
	pElementLower = element;
	pElementUpper = element;
}

void deoglShaderParameterBlock::pClearMapped(){
	if( ! pMapped ){
		DETHROW( deeInvalidParam );
	}
	
	pMapped = NULL;
}

void deoglShaderParameterBlock::pSetElementStride( int stride ){
	pElementStride = stride;
	pUpdateBufferSize();
}

void deoglShaderParameterBlock::pUpdateBufferSize(){
	pBufferSize = pElementStride * pElementCount;
}



// Private Functions
//////////////////////

const deoglSPBParameter &deoglShaderParameterBlock::pParamFloat( int index, int componentCount,
int vectorCount, int arrayIndex ) const{
	return pParam( index, deoglSPBParameter::evtFloat, componentCount, vectorCount, arrayIndex );
}

const deoglSPBParameter &deoglShaderParameterBlock::pParamInt( int index, int componentCount,
int vectorCount, int arrayIndex ) const{
	return pParam( index, deoglSPBParameter::evtInt, componentCount, vectorCount, arrayIndex );
}

const deoglSPBParameter &deoglShaderParameterBlock::pParamBool( int index, int componentCount,
int vectorCount, int arrayIndex ) const{
	return pParam( index, deoglSPBParameter::evtBool, componentCount, vectorCount, arrayIndex );
}

const deoglSPBParameter &deoglShaderParameterBlock::pParam( int index,
deoglSPBParameter::eValueTypes valueType, int componentCount,
int vectorCount, int arrayIndex ) const{
	const deoglSPBParameter &parameter = GetParameterAt( index );
	
	if( parameter.GetValueType() != valueType
	|| parameter.GetComponentCount() != componentCount
	|| parameter.GetVectorCount() != vectorCount
	|| arrayIndex < 0 || arrayIndex >= parameter.GetArrayCount() ){
		DETHROW( deeInvalidParam );
	}
	
	return parameter;
}


GLfloat *deoglShaderParameterBlock::pDataFloat( const deoglSPBParameter &parameter,
int element, int arrayIndex ) const{
	return ( GLfloat* )pData( parameter, element, arrayIndex );
}

GLint *deoglShaderParameterBlock::pDataInt( const deoglSPBParameter &parameter,
int element, int arrayIndex ) const{
	return ( GLint* )pData( parameter, element, arrayIndex );
}

GLuint *deoglShaderParameterBlock::pDataUInt( const deoglSPBParameter &parameter,
int element, int arrayIndex ) const{
	return ( GLuint* )pData( parameter, element, arrayIndex );
}

char *deoglShaderParameterBlock::pData( const deoglSPBParameter &parameter,
int element, int arrayIndex ) const{
	if( ! pMapped || element < pElementLower || element > pElementUpper ){
		DETHROW( deeInvalidParam );
	}
	return pMapped + ( pElementStride * ( element - pElementLower )
		+ parameter.GetArrayStride() * arrayIndex + parameter.GetOffset() );
}


void deoglShaderParameterBlock::pDataMatrix2( const deoglSPBParameter &parameter, int element,
GLfloat **data, int arrayIndex ) const{
	char * const mapped = pData( parameter, element, arrayIndex );
	data[ 0 ] = ( GLfloat* )mapped;
	data[ 1 ] = ( GLfloat* )( mapped + parameter.GetStride() );
}

void deoglShaderParameterBlock::pDataMatrix3( const deoglSPBParameter &parameter, int element,
GLfloat **data, int arrayIndex ) const{
	char * const mapped = pData( parameter, element, arrayIndex );
	data[ 0 ] = ( GLfloat* )mapped;
	data[ 1 ] = ( GLfloat* )( mapped + parameter.GetStride() );
	data[ 2 ] = ( GLfloat* )( mapped + parameter.GetStride() * 2 );
}

void deoglShaderParameterBlock::pDataMatrix4( const deoglSPBParameter &parameter, int element,
GLfloat **data, int arrayIndex ) const{
	char * const mapped = pData( parameter, element, arrayIndex );
	data[ 0 ] = ( GLfloat* )mapped;
	data[ 1 ] = ( GLfloat* )( mapped + parameter.GetStride() );
	data[ 2 ] = ( GLfloat* )( mapped + parameter.GetStride() * 2 );
	data[ 3 ] = ( GLfloat* )( mapped + parameter.GetStride() * 3 );
}
