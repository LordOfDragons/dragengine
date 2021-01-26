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

#include "deoglDynamicTBOFloat32.h"
#include "../memory/deoglMemoryManager.h"
#include "../texture/deoglTextureStageManager.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTTexture.h"

#include <dragengine/common/exceptions.h>



// Class deoglDynamicTBOFloat32
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDynamicTBOFloat32::deoglDynamicTBOFloat32( deoglRenderThread &renderThread, int componentCount ) :
pRenderThread( renderThread ),
pComponentCount( componentCount ),
pVBO( 0 ),
pTBO( 0 ),
pDataFloat( NULL ),
pDataSize( 0 ),
pDataCount( 0 ),
pMemoryGPU( 0 )
{
	if( componentCount < 1 || componentCount > 4 ){
		DETHROW( deeInvalidParam );
	}
	
	try{
		OGL_CHECK( pRenderThread, pglGenBuffers( 1, &pVBO ) );
		if( ! pVBO ){
			DETHROW( deeOutOfMemory );
		}
		
		deoglMemoryConsumptionVBO &consumption = renderThread.GetMemoryManager().GetConsumption().GetVBO();
		consumption.IncrementCount();
		consumption.IncrementTBOCount();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglDynamicTBOFloat32::~deoglDynamicTBOFloat32(){
	pCleanUp();
}



// Management
///////////////

int deoglDynamicTBOFloat32::GetPixelCount() const{
	int count = pDataCount / pComponentCount;
	if( pDataCount % pComponentCount != 0 ){
		count++;
	}
	return count;
}

void deoglDynamicTBOFloat32::Clear(){
	pDataCount = 0;
}

void deoglDynamicTBOFloat32::AddBool( bool value ){
	if( value ){
		AddFloat( 1.0f );
		
	}else{
		AddFloat( 0.0f );
	}
}

void deoglDynamicTBOFloat32::AddFloat( float value ){
	pEnlarge( 1 );
	pDataFloat[ pDataCount++ ] = value;
}

void deoglDynamicTBOFloat32::AddVec2( float value1, float value2 ){
	pEnlarge( 2 );
	pDataFloat[ pDataCount++ ] = value1;
	pDataFloat[ pDataCount++ ] = value2;
}

void deoglDynamicTBOFloat32::AddVec2( const decVector2 &value ){
	pEnlarge( 2 );
	pDataFloat[ pDataCount++ ] = value.x;
	pDataFloat[ pDataCount++ ] = value.y;
}

void deoglDynamicTBOFloat32::AddVec3( float value1, float value2, float value3 ){
	pEnlarge( 3 );
	pDataFloat[ pDataCount++ ] = value1;
	pDataFloat[ pDataCount++ ] = value2;
	pDataFloat[ pDataCount++ ] = value3;
}

void deoglDynamicTBOFloat32::AddVec3( const decVector &value ){
	pEnlarge( 3 );
	pDataFloat[ pDataCount++ ] = value.x;
	pDataFloat[ pDataCount++ ] = value.y;
	pDataFloat[ pDataCount++ ] = value.z;
}

void deoglDynamicTBOFloat32::AddVec4( float value1, float value2, float value3, float value4 ){
	pEnlarge( 4 );
	pDataFloat[ pDataCount++ ] = value1;
	pDataFloat[ pDataCount++ ] = value2;
	pDataFloat[ pDataCount++ ] = value3;
	pDataFloat[ pDataCount++ ] = value4;
}

void deoglDynamicTBOFloat32::AddVec4( const decVector4 &value ){
	pEnlarge( 4 );
	pDataFloat[ pDataCount++ ] = value.x;
	pDataFloat[ pDataCount++ ] = value.y;
	pDataFloat[ pDataCount++ ] = value.z;
	pDataFloat[ pDataCount++ ] = value.w;
}

void deoglDynamicTBOFloat32::AddMat4x3( const decMatrix &value ){
	pEnlarge( 12 );
	
	pDataFloat[ pDataCount++ ] = value.a11;
	pDataFloat[ pDataCount++ ] = value.a21;
	pDataFloat[ pDataCount++ ] = value.a31;
	
	pDataFloat[ pDataCount++ ] = value.a12;
	pDataFloat[ pDataCount++ ] = value.a22;
	pDataFloat[ pDataCount++ ] = value.a32;
	
	pDataFloat[ pDataCount++ ] = value.a13;
	pDataFloat[ pDataCount++ ] = value.a23;
	pDataFloat[ pDataCount++ ] = value.a33;
	
	pDataFloat[ pDataCount++ ] = value.a14;
	pDataFloat[ pDataCount++ ] = value.a24;
	pDataFloat[ pDataCount++ ] = value.a34;
}

void deoglDynamicTBOFloat32::AddMat3x3( const decMatrix &value ){
	pEnlarge( 9 );
	
	pDataFloat[ pDataCount++ ] = value.a11;
	pDataFloat[ pDataCount++ ] = value.a21;
	pDataFloat[ pDataCount++ ] = value.a31;
	
	pDataFloat[ pDataCount++ ] = value.a12;
	pDataFloat[ pDataCount++ ] = value.a22;
	pDataFloat[ pDataCount++ ] = value.a32;
	
	pDataFloat[ pDataCount++ ] = value.a13;
	pDataFloat[ pDataCount++ ] = value.a23;
	pDataFloat[ pDataCount++ ] = value.a33;
}

void deoglDynamicTBOFloat32::AddMat3x2( const decMatrix &value ){
	pEnlarge( 6 );
	
	pDataFloat[ pDataCount++ ] = value.a11;
	pDataFloat[ pDataCount++ ] = value.a21;
	
	pDataFloat[ pDataCount++ ] = value.a12;
	pDataFloat[ pDataCount++ ] = value.a22;
	
	pDataFloat[ pDataCount++ ] = value.a13;
	pDataFloat[ pDataCount++ ] = value.a23;
}

void deoglDynamicTBOFloat32::Update(){
	if( pDataCount == 0 ){
		return;
	}
	
	while( pDataCount % pComponentCount != 0 ){
		AddFloat( 0.0f ); // pad up to size of component count
	}
	
	deoglMemoryConsumptionVBO &consumption = pRenderThread.GetMemoryManager().GetConsumption().GetVBO();
	consumption.DecrementTBOGPU( pMemoryGPU );
	consumption.DecrementGPU( pMemoryGPU );
	
	OGL_CHECK( pRenderThread, pglBindBuffer( GL_TEXTURE_BUFFER, pVBO ) );
	OGL_CHECK( pRenderThread, pglBufferData( GL_TEXTURE_BUFFER, sizeof( GLfloat ) * pDataCount, NULL, GL_STREAM_DRAW ) );
	OGL_CHECK( pRenderThread, pglBufferData( GL_TEXTURE_BUFFER, sizeof( GLfloat ) * pDataCount, pDataFloat, GL_STREAM_DRAW ) );
	
	if( ! pTBO ){
		OGL_CHECK( pRenderThread, glGenTextures( 1, &pTBO ) );
		if( ! pTBO ){
			DETHROW( deeOutOfMemory );
		}
		
		deoglTextureStageManager &tsmgr = pRenderThread.GetTexture().GetStages();
		tsmgr.EnableBareTBO( 0, pTBO );
		
		switch( pComponentCount ){
		case 1:
			OGL_CHECK( pRenderThread, pglTexBuffer( GL_TEXTURE_BUFFER, GL_R32F, pVBO ) );
			break;
			
		case 2:
			OGL_CHECK( pRenderThread, pglTexBuffer( GL_TEXTURE_BUFFER, GL_RG32F, pVBO ) );
			break;
			
		case 3:
			OGL_CHECK( pRenderThread, pglTexBuffer( GL_TEXTURE_BUFFER, GL_RGB32F, pVBO ) );
			break;
			
		case 4:
			OGL_CHECK( pRenderThread, pglTexBuffer( GL_TEXTURE_BUFFER, GL_RGBA32F, pVBO ) );
			break;
		}
		
		/*
		to use GL_RGBA16F use HALF_FLOAT and convertFloatToHalf from utils/deoglConvertFloatToHalf.h
		
		const int vboDataSize = sizeof( HALF_FLOAT ) * 4
		HALF_FLOAT * const vboData = ( HALF_FLOAT* )renderThread.GetBufferObject().GetTemporaryVBOData( vboDataSize );
		HALF_FLOAT *vboDataPtr = vboData;
		...
		*( vboDataPtr++ ) = CONVERT_FLOAT_TO_HALF( value );
		...
		*/
		
		tsmgr.DisableStage( 0 );
	}
	
	OGL_CHECK( pRenderThread, pglBindBuffer( GL_TEXTURE_BUFFER, 0 ) );
	
	pMemoryGPU = sizeof( GLfloat ) * pDataCount;
	
	consumption.IncrementTBOGPU( pMemoryGPU );
	consumption.IncrementGPU( pMemoryGPU );
}

void deoglDynamicTBOFloat32::DebugPrint(){
	deoglRTLogger &logger = pRenderThread.GetLogger();
	logger.LogInfoFormat( "TBO %d-Float32:", pComponentCount );
	float *data = pDataFloat;
	int i, pixel = 0;
	
	while( pDataCount % pComponentCount != 0 ){
		AddFloat( 0.0f ); // pad up to size of component count
	}
	
	switch( pComponentCount ){
	case 1:
		for( i=0; i<pDataCount; i++ ){
			const float v1 = *(data++);
			logger.LogInfoFormat( "%5d: [%11.4f]", pixel++, v1 );
		}
		break;
		
	case 2:
		for( i=0; i<pDataCount; i+=2 ){
			const float v1 = *(data++);
			const float v2 = *(data++);
			logger.LogInfoFormat( "%5d: [%11.4f | %11.4f]", pixel++, v1, v2 );
		}
		break;
		
	case 3:
		for( i=0; i<pDataCount; i+=3 ){
			const float v1 = *(data++);
			const float v2 = *(data++);
			const float v3 = *(data++);
			logger.LogInfoFormat( "%5d: [%11.4f | %11.4f | %11.4f]", pixel++, v1, v2, v3 );
		}
		break;
		
	case 4:
		for( i=0; i<pDataCount; i+=4 ){
			const float v1 = *(data++);
			const float v2 = *(data++);
			const float v3 = *(data++);
			const float v4 = *(data++);
			logger.LogInfoFormat( "%5d: [%11.4f | %11.4f | %11.4f | %11.4f]", pixel++, v1, v2, v3, v4 );
		}
		break;
	}
}



// Private Functions
//////////////////////

void deoglDynamicTBOFloat32::pCleanUp(){
	deoglMemoryConsumptionVBO &consumption = pRenderThread.GetMemoryManager().GetConsumption().GetVBO();
	consumption.DecrementTBOGPU( pMemoryGPU );
	consumption.DecrementTBOCount();
	consumption.DecrementGPU( pMemoryGPU );
	consumption.DecrementCount();
	
	if( pTBO ){
		glDeleteTextures( 1, &pTBO );
	}
	if( pVBO ){
		pglDeleteBuffers( 1, &pVBO );
	}
	if( pDataFloat ){
		delete [] pDataFloat;
	}
}

void deoglDynamicTBOFloat32::pEnlarge( int count ){
	if( pDataCount + count <= pDataSize ){
		return;
	}
	
	const int newSize = pDataCount + count + 50;
	float * const newArray = new float[ newSize ];
	
	if( pDataFloat ){
		memcpy( newArray, pDataFloat, sizeof( float ) * pDataCount );
		delete [] pDataFloat;
	}
	
	pDataFloat = newArray;
	pDataSize = newSize;
}
