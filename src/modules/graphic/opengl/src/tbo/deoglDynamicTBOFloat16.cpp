/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include "deoglDynamicTBOFloat16.h"
#include "../memory/deoglMemoryManager.h"
#include "../texture/deoglTextureStageManager.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTTexture.h"

#include <dragengine/common/exceptions.h>



// Class deoglDynamicTBOFloat16
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDynamicTBOFloat16::deoglDynamicTBOFloat16( deoglRenderThread &renderThread, int componentCount ) :
pRenderThread( renderThread ),
pComponentCount( componentCount ),
pVBO( 0 ),
pTBO( 0 ),
pDataFloat( NULL ),
pDataSize( 0 ),
pDataCount( 0 ),
pMemoryGPU( 0 )
{
	if( componentCount < 1 || componentCount > 4 || componentCount == 3 ){
		DETHROW( deeInvalidParam );
	}
}

deoglDynamicTBOFloat16::~deoglDynamicTBOFloat16(){
	pCleanUp();
}



// Management
///////////////

void deoglDynamicTBOFloat16::IncreaseDataCount( int byAmount ){
	if( byAmount < 0 ){
		DETHROW( deeInvalidParam );
	}
	if( byAmount == 0 ){
		return;
	}
	
	pEnlarge( byAmount );
	pDataCount += byAmount;
}

int deoglDynamicTBOFloat16::GetPixelCount() const{
	int count = pDataCount / pComponentCount;
	if( pDataCount % pComponentCount != 0 ){
		count++;
	}
	return count;
}

void deoglDynamicTBOFloat16::IncreasePixelCount( int byAmount ){
	pEnlarge( byAmount * pComponentCount );
}

int deoglDynamicTBOFloat16::GetPixelOffset( int pixel ) const{
	if( pixel < 0 ){
		DETHROW( deeInvalidParam );
	}
	return pixel * pComponentCount;
}

void deoglDynamicTBOFloat16::Clear(){
	pDataCount = 0;
}

void deoglDynamicTBOFloat16::AddBool( bool value ){
	AddFloat( value ? 1.0f : 0.0f );
}

void deoglDynamicTBOFloat16::AddFloat( float value ){
	pEnlarge( 1 );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value );
}

void deoglDynamicTBOFloat16::AddVec2( float value1, float value2 ){
	pEnlarge( 2 );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value1 );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value2 );
}

void deoglDynamicTBOFloat16::AddVec2( const decVector2 &value ){
	pEnlarge( 2 );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.x );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.y );
}

void deoglDynamicTBOFloat16::AddVec3( float value1, float value2, float value3 ){
	pEnlarge( 3 );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value1 );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value2 );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value3 );
}

void deoglDynamicTBOFloat16::AddVec3( const decVector &value ){
	pEnlarge( 3 );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.x );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.y );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.z );
}

void deoglDynamicTBOFloat16::AddVec4( float value1, float value2, float value3, float value4 ){
	pEnlarge( 4 );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value1 );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value2 );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value3 );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value4 );
}

void deoglDynamicTBOFloat16::AddVec4( const decVector &value, float value4 ){
	pEnlarge( 4 );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.x );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.y );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.z );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value4 );
}

void deoglDynamicTBOFloat16::AddVec4( const decVector4 &value ){
	pEnlarge( 4 );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.x );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.y );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.z );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.w );
}

void deoglDynamicTBOFloat16::AddMat4x3( const decMatrix &value ){
	pEnlarge( 12 );
	
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.a11 );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.a21 );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.a31 );
	
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.a12 );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.a22 );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.a32 );
	
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.a13 );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.a23 );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.a33 );
	
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.a14 );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.a24 );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.a34 );
}

void deoglDynamicTBOFloat16::AddMat3x4( const decMatrix &value ){
	pEnlarge( 12 );
	
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.a11 );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.a12 );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.a13 );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.a14 );
	
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.a21 );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.a22 );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.a23 );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.a24 );
	
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.a31 );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.a32 );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.a33 );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.a34 );
}

void deoglDynamicTBOFloat16::AddMat3x3( const decMatrix &value ){
	pEnlarge( 9 );
	
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.a11 );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.a21 );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.a31 );
	
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.a12 );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.a22 );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.a32 );
	
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.a13 );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.a23 );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.a33 );
}

void deoglDynamicTBOFloat16::AddMat3x2( const decMatrix &value ){
	pEnlarge( 6 );
	
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.a11 );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.a21 );
	
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.a12 );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.a22 );
	
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.a13 );
	pDataFloat[ pDataCount++ ] = convertFloatToHalf( value.a23 );
}

void deoglDynamicTBOFloat16::AddTBO( const deoglDynamicTBOFloat16 &tbo ){
	if( tbo.pDataCount == 0 ){
		return;
	}
	
	pEnlarge( tbo.pDataCount );
	memcpy( pDataFloat + pDataCount, tbo.pDataFloat, sizeof( HALF_FLOAT ) * tbo.pDataCount );
	pDataCount += tbo.pDataCount;
}

void deoglDynamicTBOFloat16::SetBoolAt( int offset, bool value ){
	SetFloatAt( offset, value ? 1.0f : 0.0f );
}

void deoglDynamicTBOFloat16::SetFloatAt( int offset, float value ){
	if( offset < 0 || offset >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	pDataFloat[ offset ] = convertFloatToHalf( value );
}

void deoglDynamicTBOFloat16::SetVec2At( int offset, float value1, float value2 ){
	if( offset < 0 || offset + 1 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	pDataFloat[ offset ] = convertFloatToHalf( value1 );
	pDataFloat[ offset + 1 ] = convertFloatToHalf( value2 );
}

void deoglDynamicTBOFloat16::SetVec2At( int offset, const decVector2 &value ){
	if( offset < 0 || offset + 1 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	pDataFloat[ offset ] = convertFloatToHalf( value.x );
	pDataFloat[ offset + 1 ] = convertFloatToHalf( value.y );
}

void deoglDynamicTBOFloat16::SetVec3At( int offset, float value1, float value2, float value3 ){
	if( offset < 0 || offset + 2 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	pDataFloat[ offset ] = convertFloatToHalf( value1 );
	pDataFloat[ offset + 1 ] = convertFloatToHalf( value2 );
	pDataFloat[ offset + 2 ] = convertFloatToHalf( value3 );
}

void deoglDynamicTBOFloat16::SetVec3At( int offset, const decVector &value ){
	if( offset < 0 || offset + 2 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	pDataFloat[ offset ] = convertFloatToHalf( value.x );
	pDataFloat[ offset + 1 ] = convertFloatToHalf( value.y );
	pDataFloat[ offset + 2 ] = convertFloatToHalf( value.z );
}

void deoglDynamicTBOFloat16::SetVec4At( int offset, float value1, float value2,
float value3, float value4 ){
	if( offset < 0 || offset + 3 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	pDataFloat[ offset ] = convertFloatToHalf( value1 );
	pDataFloat[ offset + 1 ] = convertFloatToHalf( value2 );
	pDataFloat[ offset + 2 ] = convertFloatToHalf( value3 );
	pDataFloat[ offset + 3 ] = convertFloatToHalf( value4 );
}

void deoglDynamicTBOFloat16::SetVec4At( int offset, const decVector &value, float value4 ){
	if( offset < 0 || offset + 3 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	pDataFloat[ offset ] = convertFloatToHalf( value.x );
	pDataFloat[ offset + 1 ] = convertFloatToHalf( value.y );
	pDataFloat[ offset + 2 ] = convertFloatToHalf( value.z );
	pDataFloat[ offset + 3 ] = convertFloatToHalf( value4 );
}

void deoglDynamicTBOFloat16::SetVec4At( int offset, const decVector4 &value ){
	if( offset < 0 || offset + 3 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	pDataFloat[ offset ] = convertFloatToHalf( value.x );
	pDataFloat[ offset + 1 ] = convertFloatToHalf( value.y );
	pDataFloat[ offset + 2 ] = convertFloatToHalf( value.z );
	pDataFloat[ offset + 3 ] = convertFloatToHalf( value.w );
}

void deoglDynamicTBOFloat16::SetMat4x3At( int offset, const decMatrix &value ){
	if( offset < 0 || offset + 11 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	pDataFloat[ offset ] = convertFloatToHalf( value.a11 );
	pDataFloat[ offset + 1 ] = convertFloatToHalf( value.a21 );
	pDataFloat[ offset + 2 ] = convertFloatToHalf( value.a31 );
	
	pDataFloat[ offset + 3 ] = convertFloatToHalf( value.a12 );
	pDataFloat[ offset + 4 ] = convertFloatToHalf( value.a22 );
	pDataFloat[ offset + 5 ] = convertFloatToHalf( value.a32 );
	
	pDataFloat[ offset + 6 ] = convertFloatToHalf( value.a13 );
	pDataFloat[ offset + 7 ] = convertFloatToHalf( value.a23 );
	pDataFloat[ offset + 8 ] = convertFloatToHalf( value.a33 );
	
	pDataFloat[ offset + 9 ] = convertFloatToHalf( value.a14 );
	pDataFloat[ offset + 10 ] = convertFloatToHalf( value.a24 );
	pDataFloat[ offset + 11 ] = convertFloatToHalf( value.a34 );
}

void deoglDynamicTBOFloat16::SetMat3x4At( int offset, const decMatrix &value ){
	if( offset < 0 || offset + 11 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	pDataFloat[ offset ] = convertFloatToHalf( value.a11 );
	pDataFloat[ offset + 1 ] = convertFloatToHalf( value.a12 );
	pDataFloat[ offset + 2 ] = convertFloatToHalf( value.a13 );
	pDataFloat[ offset + 3 ] = convertFloatToHalf( value.a14 );
	
	pDataFloat[ offset + 4 ] = convertFloatToHalf( value.a21 );
	pDataFloat[ offset + 5 ] = convertFloatToHalf( value.a22 );
	pDataFloat[ offset + 6 ] = convertFloatToHalf( value.a23 );
	pDataFloat[ offset + 7] = convertFloatToHalf( value.a24 );
	
	pDataFloat[ offset + 8 ] = convertFloatToHalf( value.a31 );
	pDataFloat[ offset + 9 ] = convertFloatToHalf( value.a32 );
	pDataFloat[ offset + 10 ] = convertFloatToHalf( value.a33 );
	pDataFloat[ offset + 11 ] = convertFloatToHalf( value.a34 );
}

void deoglDynamicTBOFloat16::SetMat3x3At( int offset, const decMatrix &value ){
	if( offset < 0 || offset + 8 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	pDataFloat[ offset ] = convertFloatToHalf( value.a11 );
	pDataFloat[ offset + 1 ] = convertFloatToHalf( value.a21 );
	pDataFloat[ offset + 2 ] = convertFloatToHalf( value.a31 );
	
	pDataFloat[ offset + 3 ] = convertFloatToHalf( value.a12 );
	pDataFloat[ offset + 4 ] = convertFloatToHalf( value.a22 );
	pDataFloat[ offset + 5 ] = convertFloatToHalf( value.a32 );
	
	pDataFloat[ offset + 6 ] = convertFloatToHalf( value.a13 );
	pDataFloat[ offset + 7 ] = convertFloatToHalf( value.a23 );
	pDataFloat[ offset + 8 ] = convertFloatToHalf( value.a33 );
}

void deoglDynamicTBOFloat16::SetMat3x2At( int offset, const decMatrix &value ){
	if( offset < 0 || offset + 5 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	pDataFloat[ offset ] = convertFloatToHalf( value.a11 );
	pDataFloat[ offset + 1 ] = convertFloatToHalf( value.a21 );
	
	pDataFloat[ offset + 2 ] = convertFloatToHalf( value.a12 );
	pDataFloat[ offset + 3 ] = convertFloatToHalf( value.a22 );
	
	pDataFloat[ offset + 4 ] = convertFloatToHalf( value.a13 );
	pDataFloat[ offset + 5 ] = convertFloatToHalf( value.a23 );
}

void deoglDynamicTBOFloat16::Update(){
	if( pDataCount == 0 ){
		return;
	}
	
	pEnsurePadding();
	pEnsureVBO();
	
	OGL_CHECK( pRenderThread, pglBindBuffer( GL_TEXTURE_BUFFER, pVBO ) );
	
	OGL_CHECK( pRenderThread, pglBufferData( GL_TEXTURE_BUFFER,
		sizeof( HALF_FLOAT ) * pDataCount, NULL, GL_STREAM_DRAW ) );
	
	OGL_CHECK( pRenderThread, pglBufferData( GL_TEXTURE_BUFFER,
		sizeof( HALF_FLOAT ) * pDataCount, pDataFloat, GL_STREAM_DRAW ) );
	
	pEnsureTBO();
	
	OGL_CHECK( pRenderThread, pglBindBuffer( GL_TEXTURE_BUFFER, 0 ) );
}

void deoglDynamicTBOFloat16::Update( int offset, int count ){
	if( count == 0 ){
		return;
	}
	
	pEnsurePadding();
	
	if( offset < 0 || count < 0 || offset + count > GetPixelCount() ){
		DETHROW( deeInvalidParam );
	}
	
	pEnsureVBO();
	
	OGL_CHECK( pRenderThread, pglBindBuffer( GL_TEXTURE_BUFFER, pVBO ) );
	
	OGL_CHECK( pRenderThread, pglBufferSubData( GL_TEXTURE_BUFFER,
		sizeof( HALF_FLOAT ) * ( offset * pComponentCount ),
		sizeof( HALF_FLOAT ) * ( count * pComponentCount ),
		pDataFloat + ( offset * pComponentCount ) ) );
	
	pEnsureTBO();
	
	OGL_CHECK( pRenderThread, pglBindBuffer( GL_TEXTURE_BUFFER, 0 ) );
}

void deoglDynamicTBOFloat16::DebugPrint(){
	deoglRTLogger &logger = pRenderThread.GetLogger();
	logger.LogInfoFormat( "TBO %d-Float16:", pComponentCount );
	HALF_FLOAT *data = pDataFloat;
	int i, pixel = 0;
	
	pEnsurePadding();
	
	switch( pComponentCount ){
	case 1:
		for( i=0; i<pDataCount; i++ ){
			const float v1 = convertHalfToFloat( *(data++) );
			logger.LogInfoFormat( "%5d: [%11.4f]", pixel++, v1 );
		}
		break;
		
	case 2:
		for( i=0; i<pDataCount; i+=2 ){
			const float v1 = convertHalfToFloat( *(data++) );
			const float v2 = convertHalfToFloat( *(data++) );
			logger.LogInfoFormat( "%5d: [%11.4f | %11.4f]", pixel++, v1, v2 );
		}
		break;
		
	case 3:
		for( i=0; i<pDataCount; i+=3 ){
			const float v1 = convertHalfToFloat( *(data++) );
			const float v2 = convertHalfToFloat( *(data++) );
			const float v3 = convertHalfToFloat( *(data++) );
			logger.LogInfoFormat( "%5d: [%11.4f | %11.4f | %11.4f]", pixel++, v1, v2, v3 );
		}
		break;
		
	case 4:
		for( i=0; i<pDataCount; i+=4 ){
			const float v1 = convertHalfToFloat( *(data++) );
			const float v2 = convertHalfToFloat( *(data++) );
			const float v3 = convertHalfToFloat( *(data++) );
			const float v4 = convertHalfToFloat( *(data++) );
			logger.LogInfoFormat( "%5d: [%11.4f | %11.4f | %11.4f | %11.4f]", pixel++, v1, v2, v3, v4 );
		}
		break;
	}
}



// Private Functions
//////////////////////

void deoglDynamicTBOFloat16::pCleanUp(){
	if( pTBO ){
		glDeleteTextures( 1, &pTBO );
	}
	
	if( pVBO ){
		deoglMemoryConsumptionVBO &consumption = pRenderThread.GetMemoryManager().GetConsumption().GetVBO();
		consumption.DecrementTBOGPU( pMemoryGPU );
		consumption.DecrementTBOCount();
		consumption.DecrementGPU( pMemoryGPU );
		consumption.DecrementCount();
		
		pglDeleteBuffers( 1, &pVBO );
	}
	
	if( pDataFloat ){
		delete [] pDataFloat;
	}
}

void deoglDynamicTBOFloat16::pEnlarge( int count ){
	if( pDataCount + count <= pDataSize ){
		return;
	}
	
	//const int newSize = pDataCount + count + 50;
	const int newSize = ( pDataCount + count ) * 3 / 2 + 1;
	HALF_FLOAT * const newArray = new HALF_FLOAT[ newSize ];
	
	if( pDataFloat ){
		memcpy( newArray, pDataFloat, sizeof( HALF_FLOAT ) * pDataCount );
		delete [] pDataFloat;
	}
	
	pDataFloat = newArray;
	pDataSize = newSize;
}

void deoglDynamicTBOFloat16::pEnsureVBO(){
	deoglMemoryConsumptionVBO &consumption = pRenderThread.GetMemoryManager().GetConsumption().GetVBO();
	
	if( pVBO ){
		consumption.DecrementTBOGPU( pMemoryGPU );
		consumption.DecrementGPU( pMemoryGPU );
		
	}else{
		OGL_CHECK( pRenderThread, pglGenBuffers( 1, &pVBO ) );
		if( ! pVBO ){
			DETHROW( deeOutOfMemory );
		}
		
		consumption.IncrementCount();
		consumption.IncrementTBOCount();
	}
	
	pMemoryGPU = sizeof( HALF_FLOAT ) * pDataCount;
	
	consumption.IncrementTBOGPU( pMemoryGPU );
	consumption.IncrementGPU( pMemoryGPU );
}

void deoglDynamicTBOFloat16::pEnsureTBO(){
	if( pTBO ){
		return;
	}
	
	OGL_CHECK( pRenderThread, glGenTextures( 1, &pTBO ) );
	if( ! pTBO ){
		DETHROW( deeOutOfMemory );
	}
	
	deoglTextureStageManager &tsmgr = pRenderThread.GetTexture().GetStages();
	tsmgr.EnableBareTBO( 0, pTBO );
	
	switch( pComponentCount ){
	case 1:
		OGL_CHECK( pRenderThread, pglTexBuffer( GL_TEXTURE_BUFFER, GL_R16F, pVBO ) );
		break;
		
	case 2:
		OGL_CHECK( pRenderThread, pglTexBuffer( GL_TEXTURE_BUFFER, GL_RG16F, pVBO ) );
		break;
		
	case 4:
		OGL_CHECK( pRenderThread, pglTexBuffer( GL_TEXTURE_BUFFER, GL_RGBA16F, pVBO ) );
		break;
	}
	
	tsmgr.DisableStage( 0 );
}

void deoglDynamicTBOFloat16::pEnsurePadding(){
	while( pDataCount % pComponentCount != 0 ){
		AddFloat( 0.0f ); // pad up to size of component count
	}
}
