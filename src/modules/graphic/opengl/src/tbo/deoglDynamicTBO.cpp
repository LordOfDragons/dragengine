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

#include "deoglDynamicTBO.h"
#include "../memory/deoglMemoryManager.h"
#include "../texture/deoglTextureStageManager.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTTexture.h"

#include <dragengine/common/exceptions.h>



// Class deoglDynamicTBO
//////////////////////////

// Constructor, destructor
////////////////////////////

deoglDynamicTBO::deoglDynamicTBO( deoglRenderThread &renderThread, int componentCount, int dataTypeSize ) :
pRenderThread( renderThread ),
pComponentCount( componentCount ),
pDataTypeSize( dataTypeSize ),
pVBO( 0 ),
pTBO( 0 ),
pData( NULL ),
pDataSize( 0 ),
pDataCount( 0 ),
pMemoryGPU( 0 )
{
	if( componentCount < 1 || componentCount > 4 || dataTypeSize < 1 || dataTypeSize > 4 ){
		DETHROW( deeInvalidParam );
	}
}

deoglDynamicTBO::~deoglDynamicTBO(){
	pCleanUp();
}



// Management
///////////////

void deoglDynamicTBO::IncreaseDataCount( int byAmount ){
	if( byAmount < 0 ){
		DETHROW( deeInvalidParam );
	}
	if( byAmount == 0 ){
		return;
	}
	
	pEnlarge( byAmount );
	pDataCount += byAmount;
}

void deoglDynamicTBO::SetDataCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	if( count > pDataCount ){
		pEnlarge( count - pDataCount );
	}
	pDataCount = count;
}

int deoglDynamicTBO::GetPixelCount() const{
	int count = pDataCount / pComponentCount;
	if( pDataCount % pComponentCount != 0 ){
		count++;
	}
	return count;
}

void deoglDynamicTBO::IncreasePixelCount( int byAmount ){
	IncreaseDataCount( byAmount * pComponentCount );
}

void deoglDynamicTBO::SetPixelCount( int count ){
	SetDataCount( count * pComponentCount );
}

int deoglDynamicTBO::GetPixelOffset( int pixel ) const{
	if( pixel < 0 ){
		DETHROW( deeInvalidParam );
	}
	return pixel * pComponentCount;
}

void deoglDynamicTBO::Clear(){
	pDataCount = 0;
}

void deoglDynamicTBO::AddTBO( const deoglDynamicTBO &tbo ){
	if( tbo.pDataTypeSize != pDataTypeSize ){
		DETHROW( deeInvalidParam );
	}
	if( tbo.pDataCount == 0 ){
		return;
	}
	
	pEnlarge( tbo.pDataCount );
	memcpy( pData + pDataCount * pDataTypeSize, tbo.pData, tbo.pDataCount * tbo.pDataTypeSize );
	pDataCount += tbo.pDataCount;
}

void deoglDynamicTBO::SetTBO( int offset, const deoglDynamicTBO &tbo ){
	if( tbo.pDataTypeSize != pDataTypeSize ){
		DETHROW( deeInvalidParam );
	}
	if( tbo.pDataCount == 0 ){
		return;
	}
	if( offset < 0 || offset + tbo.pDataCount - 1 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	memcpy( pData + offset * pDataTypeSize, tbo.pData, tbo.pDataCount * tbo.pDataTypeSize );
}

void deoglDynamicTBO::Update(){
	if( pDataCount == 0 ){
		return;
	}
	
	pEnsurePadding();
	pEnsureVBO();
	
	OGL_CHECK( pRenderThread, pglBindBuffer( GL_TEXTURE_BUFFER, pVBO ) );
	
	const int size = pDataCount * pDataTypeSize;
	OGL_CHECK( pRenderThread, pglBufferData( GL_TEXTURE_BUFFER, size, NULL, GL_STREAM_DRAW ) );
	OGL_CHECK( pRenderThread, pglBufferData( GL_TEXTURE_BUFFER, size, pData, GL_STREAM_DRAW ) );
	
	pEnsureTBO();
	
	OGL_CHECK( pRenderThread, pglBindBuffer( GL_TEXTURE_BUFFER, 0 ) );
}

void deoglDynamicTBO::Update( int offset, int count ){
	if( count == 0 ){
		return;
	}
	
	pEnsurePadding();
	
	if( offset < 0 || count < 0 || offset + count > GetPixelCount() ){
		DETHROW( deeInvalidParam );
	}
	
	pEnsureVBO();
	
	OGL_CHECK( pRenderThread, pglBindBuffer( GL_TEXTURE_BUFFER, pVBO ) );
	
	const int byteOffset = offset * pComponentCount * pDataTypeSize;
	OGL_CHECK( pRenderThread, pglBufferSubData( GL_TEXTURE_BUFFER, byteOffset,
		count * pComponentCount * pDataTypeSize, pData + byteOffset ) );
	
	pEnsureTBO();
	
	OGL_CHECK( pRenderThread, pglBindBuffer( GL_TEXTURE_BUFFER, 0 ) );
}



// Private Functions
//////////////////////

void deoglDynamicTBO::pCleanUp(){
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
	
	if( pData ){
		delete [] pData;
	}
}

void deoglDynamicTBO::pEnlarge( int count ){
	if( pDataCount + count <= pDataSize ){
		return;
	}
	
	const int newSize = ( pDataCount + count ) * 3 / 2 + 1;
	uint8_t * const newArray = new uint8_t[ newSize * pDataTypeSize ];
	
	if( pData ){
		memcpy( newArray, pData, pDataCount * pDataTypeSize );
		delete [] pData;
	}
	
	pData = newArray;
	pDataSize = newSize;
}

void deoglDynamicTBO::pEnsureVBO(){
	deoglMemoryConsumptionVBO &consumption = pRenderThread.GetMemoryManager().GetConsumption().GetVBO();
	
	if( ! pVBO ){
		OGL_CHECK( pRenderThread, pglGenBuffers( 1, &pVBO ) );
		if( ! pVBO ){
			DETHROW( deeOutOfMemory );
		}
		
		consumption.IncrementCount();
		consumption.IncrementTBOCount();
	}
	
	int incrMemoryGPU = -pMemoryGPU;
	pMemoryGPU = pDataCount * pDataTypeSize;
	incrMemoryGPU += pMemoryGPU;
	
	consumption.IncrementTBOGPU( incrMemoryGPU );
	consumption.IncrementGPU( incrMemoryGPU );
}

void deoglDynamicTBO::pEnsureTBO(){
	if( pTBO ){
		return;
	}
	
	OGL_CHECK( pRenderThread, glGenTextures( 1, &pTBO ) );
	if( ! pTBO ){
		DETHROW( deeOutOfMemory );
	}
	
	deoglTextureStageManager &tsmgr = pRenderThread.GetTexture().GetStages();
	tsmgr.EnableBareTBO( 0, pTBO );
	OGL_CHECK( pRenderThread, pglTexBuffer( GL_TEXTURE_BUFFER, GetTBOFormat(), pVBO ) );
	tsmgr.DisableStage( 0 );
}

void deoglDynamicTBO::pEnsurePadding(){
	const int remainder = pDataCount % pComponentCount;
	if( remainder > 0 ){
		pEnlarge( pComponentCount - remainder );
	}
}
