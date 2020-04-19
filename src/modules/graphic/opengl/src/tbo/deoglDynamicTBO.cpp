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

#include "deoglDynamicTBO.h"
#include "../memory/deoglMemoryManager.h"
#include "../texture/deoglTextureStageManager.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTTexture.h"

#include <dragengine/common/exceptions.h>



// Class deoglDynamicTBO
//////////////////////////

// Constructor, destructor
////////////////////////////

deoglDynamicTBO::deoglDynamicTBO( deoglRenderThread &renderThread ) :
pRenderThread( renderThread )
{
	deoglMemoryConsumptionVBO &consumption = renderThread.GetMemoryManager().GetConsumption().GetVBO();
	
	pVBO = 0;
	pTBO = 0;
	
	pDataFloat = NULL;
	pDataSize = 0;
	pDataCount = 0;
	
	pMemoryGPU = 0;
	
	try{
		OGL_CHECK( pRenderThread, pglGenBuffers( 1, &pVBO ) );
		if( ! pVBO ){
			DETHROW( deeOutOfMemory );
		}
		
		consumption.IncrementCount();
		consumption.IncrementTBOCount();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglDynamicTBO::~deoglDynamicTBO(){
	pCleanUp();
}



// Management
///////////////

void deoglDynamicTBO::Clear(){
	pDataCount = 0;
}

void deoglDynamicTBO::AddBool( bool value ){
	if( value ){
		AddFloat( 1.0f );
		
	}else{
		AddFloat( 0.0f );
	}
}

void deoglDynamicTBO::AddFloat( float value ){
	pEnlarge( 1 );
	*pDataFloat++ = value;
}

void deoglDynamicTBO::AddVec2( float value1, float value2 ){
	pEnlarge( 2 );
	*pDataFloat++ = value1;
	*pDataFloat++ = value2;
}

void deoglDynamicTBO::AddVec2( const decVector2 &value ){
	pEnlarge( 2 );
	*pDataFloat++ = value.x;
	*pDataFloat++ = value.y;
}

void deoglDynamicTBO::AddVec3( float value1, float value2, float value3 ){
	pEnlarge( 3 );
	*pDataFloat++ = value1;
	*pDataFloat++ = value2;
	*pDataFloat++ = value3;
}

void deoglDynamicTBO::AddVec3( const decVector &value ){
	pEnlarge( 3 );
	*pDataFloat++ = value.x;
	*pDataFloat++ = value.y;
	*pDataFloat++ = value.z;
}

void deoglDynamicTBO::AddVec4( float value1, float value2, float value3, float value4 ){
	pEnlarge( 4 );
	*pDataFloat++ = value1;
	*pDataFloat++ = value2;
	*pDataFloat++ = value3;
	*pDataFloat++ = value4;
}

void deoglDynamicTBO::AddVec4( const decVector4 &value ){
	pEnlarge( 4 );
	*pDataFloat++ = value.x;
	*pDataFloat++ = value.y;
	*pDataFloat++ = value.z;
	*pDataFloat++ = value.w;
}

void deoglDynamicTBO::AddMat4x3( const decMatrix &value ){
	pEnlarge( 12 );
	*pDataFloat++ = value.a11; *pDataFloat++ = value.a21; *pDataFloat++ = value.a31;
	*pDataFloat++ = value.a12; *pDataFloat++ = value.a22; *pDataFloat++ = value.a32;
	*pDataFloat++ = value.a13; *pDataFloat++ = value.a23; *pDataFloat++ = value.a33;
	*pDataFloat++ = value.a14; *pDataFloat++ = value.a24; *pDataFloat++ = value.a34;
}

void deoglDynamicTBO::AddMat3x3( const decMatrix &value ){
	pEnlarge( 9 );
	*pDataFloat++ = value.a11; *pDataFloat++ = value.a21; *pDataFloat++ = value.a31;
	*pDataFloat++ = value.a12; *pDataFloat++ = value.a22; *pDataFloat++ = value.a32;
	*pDataFloat++ = value.a13; *pDataFloat++ = value.a23; *pDataFloat++ = value.a33;
}

void deoglDynamicTBO::AddMat3x2( const decMatrix &value ){
	pEnlarge( 6 );
	*pDataFloat++ = value.a11; *pDataFloat++ = value.a21;
	*pDataFloat++ = value.a12; *pDataFloat++ = value.a22;
	*pDataFloat++ = value.a13; *pDataFloat++ = value.a23;
}

void deoglDynamicTBO::Update(){
	if( pDataCount > 0 ){
		deoglMemoryConsumptionVBO &consumption = pRenderThread.GetMemoryManager().GetConsumption().GetVBO();
		
		while( ( pDataCount & 0x3 ) > 0 ){
			AddFloat( 0.0f ); // pad up to size of 4
		}
		
		consumption.DecrementTBOGPU( pMemoryGPU );
		consumption.DecrementGPU( pMemoryGPU );
		
		OGL_CHECK( pRenderThread, pglBindBuffer( GL_TEXTURE_BUFFER, pVBO ) );
		OGL_CHECK( pRenderThread, pglBufferData( GL_TEXTURE_BUFFER, sizeof( GLfloat ) * pDataCount, NULL, GL_STREAM_DRAW ) );
		OGL_CHECK( pRenderThread, pglBufferData( GL_TEXTURE_BUFFER, sizeof( GLfloat ) * pDataCount, pDataFloat, GL_STREAM_DRAW ) );
		
		if( ! pTBO ){
			deoglTextureStageManager &tsmgr = pRenderThread.GetTexture().GetStages();
			
			OGL_CHECK( pRenderThread, pglGenBuffers( 1, &pTBO ) );
			if( ! pTBO ){
				DETHROW( deeOutOfMemory );
			}
			
			tsmgr.EnableBareTBO( 0, pTBO );
			OGL_CHECK( pRenderThread, pglTexBuffer( GL_TEXTURE_BUFFER, GL_RGBA16F, pVBO ) );
			tsmgr.DisableStage( 0 );
		}
		
		OGL_CHECK( pRenderThread, pglBindBuffer( GL_TEXTURE_BUFFER, 0 ) );
		
		pMemoryGPU = sizeof( GLfloat ) * pDataCount;
		
		consumption.IncrementTBOGPU( pMemoryGPU );
		consumption.IncrementGPU( pMemoryGPU );
	}
}



// Private Functions
//////////////////////

void deoglDynamicTBO::pCleanUp(){
	deoglMemoryConsumptionVBO &consumption = pRenderThread.GetMemoryManager().GetConsumption().GetVBO();
	
	consumption.DecrementTBOGPU( pMemoryGPU );
	consumption.DecrementTBOCount();
	consumption.DecrementGPU( pMemoryGPU );
	consumption.DecrementCount();
	
	if( pTBO ){
		pglDeleteBuffers( 1, &pTBO );
	}
	if( pVBO ){
		pglDeleteBuffers( 1, &pVBO );
	}
}

void deoglDynamicTBO::pEnlarge( int count ){
	if( pDataCount + count >= pDataSize ){
		const int newSize = pDataCount + count + 50;
		float * const newArray = new float[ newSize ];
		
		if( pDataFloat ){
			memcpy( newArray, pDataFloat, sizeof( float ) * pDataCount );
			delete [] pDataFloat;
		}
		
		pDataFloat = newArray;
		pDataSize = newSize;
	}
}
