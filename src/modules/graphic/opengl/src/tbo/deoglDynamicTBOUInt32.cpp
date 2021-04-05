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

#include "deoglDynamicTBOUInt32.h"
#include "../memory/deoglMemoryManager.h"
#include "../texture/deoglTextureStageManager.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTTexture.h"

#include <dragengine/common/exceptions.h>



// Class deoglDynamicTBOUInt32
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDynamicTBOUInt32::deoglDynamicTBOUInt32( deoglRenderThread &renderThread, int componentCount ) :
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

deoglDynamicTBOUInt32::~deoglDynamicTBOUInt32(){
	pCleanUp();
}



// Management
///////////////

int deoglDynamicTBOUInt32::GetPixelCount() const{
	int count = pDataCount / pComponentCount;
	if( pDataCount % pComponentCount != 0 ){
		count++;
	}
	return count;
}

int deoglDynamicTBOUInt32::GetPixelOffset( int pixel ) const{
	if( pixel < 0 ){
		DETHROW( deeInvalidParam );
	}
	return pixel * pComponentCount;
}

void deoglDynamicTBOUInt32::Clear(){
	pDataCount = 0;
}

void deoglDynamicTBOUInt32::AddBool( bool value ){
	AddInt( value ? ( uint32_t )~1 : 0 );
}

void deoglDynamicTBOUInt32::AddInt( uint32_t value ){
	pEnlarge( 1 );
	pDataUInt[ pDataCount++ ] = value;
}

void deoglDynamicTBOUInt32::AddVec2( uint32_t value1, uint32_t value2 ){
	pEnlarge( 2 );
	pDataUInt[ pDataCount++ ] = value1;
	pDataUInt[ pDataCount++ ] = value2;
}

void deoglDynamicTBOUInt32::AddVec2( const decPoint &value ){
	pEnlarge( 2 );
	pDataUInt[ pDataCount++ ] = ( uint32_t )value.x;
	pDataUInt[ pDataCount++ ] = ( uint32_t )value.y;
}

void deoglDynamicTBOUInt32::AddVec3( uint32_t value1, uint32_t value2, uint32_t value3 ){
	pEnlarge( 3 );
	pDataUInt[ pDataCount++ ] = value1;
	pDataUInt[ pDataCount++ ] = value2;
	pDataUInt[ pDataCount++ ] = value3;
}

void deoglDynamicTBOUInt32::AddVec3( const decPoint3 &value ){
	pEnlarge( 3 );
	pDataUInt[ pDataCount++ ] = ( uint32_t )value.x;
	pDataUInt[ pDataCount++ ] = ( uint32_t )value.y;
	pDataUInt[ pDataCount++ ] = ( uint32_t )value.z;
}

void deoglDynamicTBOUInt32::AddVec4( uint32_t value1, uint32_t value2, uint32_t value3, uint32_t value4 ){
	pEnlarge( 4 );
	pDataUInt[ pDataCount++ ] = value1;
	pDataUInt[ pDataCount++ ] = value2;
	pDataUInt[ pDataCount++ ] = value3;
	pDataUInt[ pDataCount++ ] = value4;
}

void deoglDynamicTBOUInt32::AddTBO( const deoglDynamicTBOUInt32 &tbo ){
	if( tbo.pDataCount == 0 ){
		return;
	}
	
	pEnlarge( tbo.pDataCount );
	memcpy( pDataUInt + pDataCount, tbo.pDataUInt, sizeof( uint32_t ) * tbo.pDataCount );
	pDataCount += tbo.pDataCount;
}

void deoglDynamicTBOUInt32::SetBoolAt( int offset, bool value ){
	SetIntAt( offset, value ? ( uint32_t )~1 : 0 );
}

void deoglDynamicTBOUInt32::SetIntAt( int offset, uint32_t value ){
	if( offset < 0 || offset >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	pDataUInt[ offset ] = value;
}

void deoglDynamicTBOUInt32::SetVec2At( int offset, uint32_t value1, uint32_t value2 ){
	if( offset < 0 || offset + 1 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	pDataUInt[ offset ] = value1;
	pDataUInt[ offset + 1 ] = value2;
}

void deoglDynamicTBOUInt32::SetVec2At( int offset, const decPoint &value ){
	if( offset < 0 || offset + 1 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	pDataUInt[ offset ] = ( uint32_t )value.x;
	pDataUInt[ offset + 1 ] = ( uint32_t )value.y;
}

void deoglDynamicTBOUInt32::SetVec3At( int offset, uint32_t value1, uint32_t value2, uint32_t value3 ){
	if( offset < 0 || offset + 2 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	pDataUInt[ offset ] = value1;
	pDataUInt[ offset + 1 ] = value2;
	pDataUInt[ offset + 2 ] = value3;
}

void deoglDynamicTBOUInt32::SetVec3At( int offset, const decPoint3 &value ){
	if( offset < 0 || offset + 2 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	pDataUInt[ offset ] = ( uint32_t )value.x;
	pDataUInt[ offset + 1 ] = ( uint32_t )value.y;
	pDataUInt[ offset + 2 ] = ( uint32_t )value.z;
}

void deoglDynamicTBOUInt32::SetVec4At( int offset, uint32_t value1, uint32_t value2,
uint32_t value3, uint32_t value4 ){
	if( offset < 0 || offset + 3 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	pDataUInt[ offset ] = value1;
	pDataUInt[ offset + 1 ] = value2;
	pDataUInt[ offset + 2 ] = value3;
	pDataUInt[ offset + 3 ] = value4;
}

void deoglDynamicTBOUInt32::Update(){
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
	
	OGL_CHECK( pRenderThread, pglBufferData( GL_TEXTURE_BUFFER,
		sizeof( uint32_t ) * pDataCount, NULL, GL_STREAM_DRAW ) );
	
	OGL_CHECK( pRenderThread, pglBufferData( GL_TEXTURE_BUFFER,
		sizeof( uint32_t ) * pDataCount, pDataUInt, GL_STREAM_DRAW ) );
	
	if( ! pTBO ){
		OGL_CHECK( pRenderThread, glGenTextures( 1, &pTBO ) );
		if( ! pTBO ){
			DETHROW( deeOutOfMemory );
		}
		
		deoglTextureStageManager &tsmgr = pRenderThread.GetTexture().GetStages();
		tsmgr.EnableBareTBO( 0, pTBO );
		
		switch( pComponentCount ){
		case 1:
			OGL_CHECK( pRenderThread, pglTexBuffer( GL_TEXTURE_BUFFER, GL_R32UI, pVBO ) );
			break;
			
		case 2:
			OGL_CHECK( pRenderThread, pglTexBuffer( GL_TEXTURE_BUFFER, GL_RG32UI, pVBO ) );
			break;
			
		case 3:
			OGL_CHECK( pRenderThread, pglTexBuffer( GL_TEXTURE_BUFFER, GL_RGB32UI, pVBO ) );
			break;
			
		case 4:
			OGL_CHECK( pRenderThread, pglTexBuffer( GL_TEXTURE_BUFFER, GL_RGBA32UI, pVBO ) );
			break;
		}
		
		tsmgr.DisableStage( 0 );
	}
	
	OGL_CHECK( pRenderThread, pglBindBuffer( GL_TEXTURE_BUFFER, 0 ) );
	
	pMemoryGPU = sizeof( uint32_t ) * pDataCount;
	
	consumption.IncrementTBOGPU( pMemoryGPU );
	consumption.IncrementGPU( pMemoryGPU );
}

void deoglDynamicTBOUInt32::DebugPrint(){
	deoglRTLogger &logger = pRenderThread.GetLogger();
	logger.LogInfoFormat( "TBO %d-UInt32:", pComponentCount );
	uint32_t *data = pDataUInt;
	int i, pixel = 0;
	
	while( pDataCount % pComponentCount != 0 ){
		AddInt( 0 ); // pad up to size of component count
	}
	
	switch( pComponentCount ){
	case 1:
		for( i=0; i<pDataCount; i++ ){
			const uint32_t v1 = *(data++);
			logger.LogInfoFormat( "%5d: [%11d]", pixel++, v1 );
		}
		break;
		
	case 2:
		for( i=0; i<pDataCount; i+=2 ){
			const uint32_t v1 = *(data++);
			const uint32_t v2 = *(data++);
			logger.LogInfoFormat( "%5d: [%11d | %11d]", pixel++, v1, v2 );
		}
		break;
		
	case 3:
		for( i=0; i<pDataCount; i+=3 ){
			const uint32_t v1 = *(data++);
			const uint32_t v2 = *(data++);
			const uint32_t v3 = *(data++);
			logger.LogInfoFormat( "%5d: [%11d | %11d | %11d]", pixel++, v1, v2, v3 );
		}
		break;
		
	case 4:
		for( i=0; i<pDataCount; i+=4 ){
			const uint32_t v1 = *(data++);
			const uint32_t v2 = *(data++);
			const uint32_t v3 = *(data++);
			const uint32_t v4 = *(data++);
			logger.LogInfoFormat( "%5d: [%11d | %11d | %11d | %11d]", pixel++, v1, v2, v3, v4 );
		}
		break;
	}
}



// Private Functions
//////////////////////

void deoglDynamicTBOUInt32::pCleanUp(){
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

void deoglDynamicTBOUInt32::pEnlarge( int count ){
	if( pDataCount + count <= pDataSize ){
		return;
	}
	
	//const int newSize = pDataCount + count + 50;
	const int newSize = ( pDataCount + count ) * 3 / 2 + 1;
	uint32_t * const newArray = new uint32_t[ newSize ];
	
	if( pDataUInt ){
		memcpy( newArray, pDataUInt, sizeof( uint32_t ) * pDataCount );
		delete [] pDataUInt;
	}
	
	pDataUInt = newArray;
	pDataSize = newSize;
}
