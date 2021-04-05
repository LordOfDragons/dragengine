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

#include "deoglDynamicTBOUInt8.h"
#include "../memory/deoglMemoryManager.h"
#include "../texture/deoglTextureStageManager.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTTexture.h"

#include <dragengine/common/exceptions.h>



// Class deoglDynamicTBOUInt8
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDynamicTBOUInt8::deoglDynamicTBOUInt8( deoglRenderThread &renderThread, int componentCount ) :
pRenderThread( renderThread ),
pComponentCount( componentCount ),
pVBO( 0 ),
pTBO( 0 ),
pDataUInt( NULL ),
pDataSize( 0 ),
pDataCount( 0 ),
pMemoryGPU( 0 )
{
	if( componentCount < 1 || componentCount > 4 ){
		DETHROW( deeInvalidParam );
	}
}

deoglDynamicTBOUInt8::~deoglDynamicTBOUInt8(){
	pCleanUp();
}



// Management
///////////////

void deoglDynamicTBOUInt8::IncreaseDataCount( int byAmount ){
	if( byAmount < 0 ){
		DETHROW( deeInvalidParam );
	}
	if( byAmount == 0 ){
		return;
	}
	
	pEnlarge( byAmount );
	pDataCount += byAmount;
}

int deoglDynamicTBOUInt8::GetPixelCount() const{
	int count = pDataCount / pComponentCount;
	if( pDataCount % pComponentCount != 0 ){
		count++;
	}
	return count;
}

void deoglDynamicTBOUInt8::IncreasePixelCount( int byAmount ){
	pEnlarge( byAmount * pComponentCount );
}

int deoglDynamicTBOUInt8::GetPixelOffset( int pixel ) const{
	if( pixel < 0 ){
		DETHROW( deeInvalidParam );
	}
	return pixel * pComponentCount;
}

void deoglDynamicTBOUInt8::Clear(){
	pDataCount = 0;
}

void deoglDynamicTBOUInt8::AddBool( bool value ){
	if( value ){
		AddInt( ( uint8_t )~1 );
		
	}else{
		AddInt( 0 );
	}
}

void deoglDynamicTBOUInt8::AddInt( uint8_t value ){
	pEnlarge( 1 );
	pDataUInt[ pDataCount++ ] = value;
}

void deoglDynamicTBOUInt8::AddVec2( uint8_t value1, uint8_t value2 ){
	pEnlarge( 2 );
	pDataUInt[ pDataCount++ ] = value1;
	pDataUInt[ pDataCount++ ] = value2;
}

void deoglDynamicTBOUInt8::AddVec2( const decPoint &value ){
	pEnlarge( 2 );
	pDataUInt[ pDataCount++ ] = ( uint8_t )value.x;
	pDataUInt[ pDataCount++ ] = ( uint8_t )value.y;
}

void deoglDynamicTBOUInt8::AddVec3( uint8_t value1, uint8_t value2, uint8_t value3 ){
	pEnlarge( 3 );
	pDataUInt[ pDataCount++ ] = value1;
	pDataUInt[ pDataCount++ ] = value2;
	pDataUInt[ pDataCount++ ] = value3;
}

void deoglDynamicTBOUInt8::AddVec3( const decPoint3 &value ){
	pEnlarge( 3 );
	pDataUInt[ pDataCount++ ] = ( uint8_t )value.x;
	pDataUInt[ pDataCount++ ] = ( uint8_t )value.y;
	pDataUInt[ pDataCount++ ] = ( uint8_t )value.z;
}

void deoglDynamicTBOUInt8::AddVec4( uint8_t value1, uint8_t value2, uint8_t value3, uint8_t value4 ){
	pEnlarge( 4 );
	pDataUInt[ pDataCount++ ] = value1;
	pDataUInt[ pDataCount++ ] = value2;
	pDataUInt[ pDataCount++ ] = value3;
	pDataUInt[ pDataCount++ ] = value4;
}

void deoglDynamicTBOUInt8::AddTBO( const deoglDynamicTBOUInt8 &tbo ){
	if( tbo.pDataCount == 0 ){
		return;
	}
	
	pEnlarge( tbo.pDataCount );
	memcpy( pDataUInt + pDataCount, tbo.pDataUInt, sizeof( uint8_t ) * tbo.pDataCount );
	pDataCount += tbo.pDataCount;
}

void deoglDynamicTBOUInt8::SetBoolAt( int offset, bool value ){
	SetIntAt( offset, value ? ( uint8_t )~1 : 0 );
}

void deoglDynamicTBOUInt8::SetIntAt( int offset, uint8_t value ){
	if( offset < 0 || offset >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	pDataUInt[ offset ] = value;
}

void deoglDynamicTBOUInt8::SetVec2At( int offset, uint8_t value1, uint8_t value2 ){
	if( offset < 0 || offset + 1 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	pDataUInt[ offset ] = value1;
	pDataUInt[ offset + 1 ] = value2;
}

void deoglDynamicTBOUInt8::SetVec2At( int offset, const decPoint &value ){
	if( offset < 0 || offset + 1 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	pDataUInt[ offset ] = ( uint8_t )value.x;
	pDataUInt[ offset + 1 ] = ( uint8_t )value.y;
}

void deoglDynamicTBOUInt8::SetVec3At( int offset, uint8_t value1, uint8_t value2, uint8_t value3 ){
	if( offset < 0 || offset + 2 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	pDataUInt[ offset ] = value1;
	pDataUInt[ offset + 1 ] = value2;
	pDataUInt[ offset + 2 ] = value3;
}

void deoglDynamicTBOUInt8::SetVec3At( int offset, const decPoint3 &value ){
	if( offset < 0 || offset + 2 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	pDataUInt[ offset ] = ( uint8_t )value.x;
	pDataUInt[ offset + 1 ] = ( uint8_t )value.y;
	pDataUInt[ offset + 2 ] = ( uint8_t )value.z;
}

void deoglDynamicTBOUInt8::SetVec4At( int offset, uint8_t value1, uint8_t value2,
uint8_t value3, uint8_t value4 ){
	if( offset < 0 || offset + 3 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	pDataUInt[ offset ] = value1;
	pDataUInt[ offset + 1 ] = value2;
	pDataUInt[ offset + 2 ] = value3;
	pDataUInt[ offset + 3 ] = value4;
}

void deoglDynamicTBOUInt8::Update(){
	if( pDataCount == 0 ){
		return;
	}
	
	while( pDataCount % pComponentCount != 0 ){
		AddInt( 0 ); // pad up to size of component count
	}
	
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
	
	OGL_CHECK( pRenderThread, pglBindBuffer( GL_TEXTURE_BUFFER, pVBO ) );
	OGL_CHECK( pRenderThread, pglBufferData( GL_TEXTURE_BUFFER, sizeof( uint8_t ) * pDataCount, NULL, GL_STREAM_DRAW ) );
	OGL_CHECK( pRenderThread, pglBufferData( GL_TEXTURE_BUFFER, sizeof( uint8_t ) * pDataCount, pDataUInt, GL_STREAM_DRAW ) );
	
	if( ! pTBO ){
		OGL_CHECK( pRenderThread, glGenTextures( 1, &pTBO ) );
		if( ! pTBO ){
			DETHROW( deeOutOfMemory );
		}
		
		deoglTextureStageManager &tsmgr = pRenderThread.GetTexture().GetStages();
		tsmgr.EnableBareTBO( 0, pTBO );
		
		switch( pComponentCount ){
		case 1:
			OGL_CHECK( pRenderThread, pglTexBuffer( GL_TEXTURE_BUFFER, GL_R8UI, pVBO ) );
			break;
			
		case 2:
			OGL_CHECK( pRenderThread, pglTexBuffer( GL_TEXTURE_BUFFER, GL_RG8UI, pVBO ) );
			break;
			
		case 3:
			OGL_CHECK( pRenderThread, pglTexBuffer( GL_TEXTURE_BUFFER, GL_RGB8UI, pVBO ) );
			break;
			
		case 4:
			OGL_CHECK( pRenderThread, pglTexBuffer( GL_TEXTURE_BUFFER, GL_RGBA8UI, pVBO ) );
			break;
		}
		
		tsmgr.DisableStage( 0 );
	}
	
	OGL_CHECK( pRenderThread, pglBindBuffer( GL_TEXTURE_BUFFER, 0 ) );
	
	pMemoryGPU = sizeof( uint8_t ) * pDataCount;
	
	consumption.IncrementTBOGPU( pMemoryGPU );
	consumption.IncrementGPU( pMemoryGPU );
}

void deoglDynamicTBOUInt8::DebugPrint(){
	deoglRTLogger &logger = pRenderThread.GetLogger();
	logger.LogInfoFormat( "TBO %d-UInt8:", pComponentCount );
	uint8_t *data = pDataUInt;
	int i, pixel = 0;
	
	while( pDataCount % pComponentCount != 0 ){
		AddInt( 0 ); // pad up to size of component count
	}
	
	switch( pComponentCount ){
	case 1:
		for( i=0; i<pDataCount; i++ ){
			const uint8_t v1 = *(data++);
			logger.LogInfoFormat( "%5d: [%3d]", pixel++, v1 );
		}
		break;
		
	case 2:
		for( i=0; i<pDataCount; i+=2 ){
			const uint8_t v1 = *(data++);
			const uint8_t v2 = *(data++);
			logger.LogInfoFormat( "%5d: [%3d | %3d]", pixel++, v1, v2 );
		}
		break;
		
	case 3:
		for( i=0; i<pDataCount; i+=3 ){
			const uint8_t v1 = *(data++);
			const uint8_t v2 = *(data++);
			const uint8_t v3 = *(data++);
			logger.LogInfoFormat( "%5d: [%3d | %3d | %3d]", pixel++, v1, v2, v3 );
		}
		break;
		
	case 4:
		for( i=0; i<pDataCount; i+=4 ){
			const uint8_t v1 = *(data++);
			const uint8_t v2 = *(data++);
			const uint8_t v3 = *(data++);
			const uint8_t v4 = *(data++);
			logger.LogInfoFormat( "%5d: [%3d | %3d | %3d | %3d]", pixel++, v1, v2, v3, v4 );
		}
		break;
	}
}



// Private Functions
//////////////////////

void deoglDynamicTBOUInt8::pCleanUp(){
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
	
	if( pDataUInt ){
		delete [] pDataUInt;
	}
}

void deoglDynamicTBOUInt8::pEnlarge( int count ){
	if( pDataCount + count <= pDataSize ){
		return;
	}
	
	//const int newSize = pDataCount + count + 50;
	const int newSize = ( pDataCount + count ) * 3 / 2 + 1;
	uint8_t * const newArray = new uint8_t[ newSize ];
	
	if( pDataUInt ){
		memcpy( newArray, pDataUInt, sizeof( uint8_t ) * pDataCount );
		delete [] pDataUInt;
	}
	
	pDataUInt = newArray;
	pDataSize = newSize;
}
