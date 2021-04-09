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
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"

#include <dragengine/common/exceptions.h>



// Class deoglDynamicTBOUInt32
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDynamicTBOUInt32::deoglDynamicTBOUInt32( deoglRenderThread &renderThread, int componentCount ) :
deoglDynamicTBO( renderThread, componentCount, sizeof( uint32_t ) ){
}

deoglDynamicTBOUInt32::~deoglDynamicTBOUInt32(){
}



// Management
///////////////

void deoglDynamicTBOUInt32::AddBool( bool value ){
	AddInt( value ? ( uint32_t )~1 : 0 );
}

void deoglDynamicTBOUInt32::AddInt( uint32_t value ){
	pEnlarge( 1 );
	
	uint32_t * const dataUInt = ( uint32_t* )pData;
	dataUInt[ pDataCount++ ] = value;
}

void deoglDynamicTBOUInt32::AddVec2( uint32_t value1, uint32_t value2 ){
	pEnlarge( 2 );
	
	uint32_t * const dataUInt = ( uint32_t* )pData;
	dataUInt[ pDataCount++ ] = value1;
	dataUInt[ pDataCount++ ] = value2;
}

void deoglDynamicTBOUInt32::AddVec2( const decPoint &value ){
	pEnlarge( 2 );
	
	uint32_t * const dataUInt = ( uint32_t* )pData;
	dataUInt[ pDataCount++ ] = ( uint32_t )value.x;
	dataUInt[ pDataCount++ ] = ( uint32_t )value.y;
}

void deoglDynamicTBOUInt32::AddVec3( uint32_t value1, uint32_t value2, uint32_t value3 ){
	pEnlarge( 3 );
	
	uint32_t * const dataUInt = ( uint32_t* )pData;
	dataUInt[ pDataCount++ ] = value1;
	dataUInt[ pDataCount++ ] = value2;
	dataUInt[ pDataCount++ ] = value3;
}

void deoglDynamicTBOUInt32::AddVec3( const decPoint3 &value ){
	pEnlarge( 3 );
	
	uint32_t * const dataUInt = ( uint32_t* )pData;
	dataUInt[ pDataCount++ ] = ( uint32_t )value.x;
	dataUInt[ pDataCount++ ] = ( uint32_t )value.y;
	dataUInt[ pDataCount++ ] = ( uint32_t )value.z;
}

void deoglDynamicTBOUInt32::AddVec4( uint32_t value1, uint32_t value2, uint32_t value3, uint32_t value4 ){
	pEnlarge( 4 );
	
	uint32_t * const dataUInt = ( uint32_t* )pData;
	dataUInt[ pDataCount++ ] = value1;
	dataUInt[ pDataCount++ ] = value2;
	dataUInt[ pDataCount++ ] = value3;
	dataUInt[ pDataCount++ ] = value4;
}

void deoglDynamicTBOUInt32::SetBoolAt( int offset, bool value ){
	SetIntAt( offset, value ? ( uint32_t )~1 : 0 );
}

void deoglDynamicTBOUInt32::SetIntAt( int offset, uint32_t value ){
	if( offset < 0 || offset >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	uint32_t * const dataUInt = ( uint32_t* )pData;
	dataUInt[ offset ] = value;
}

void deoglDynamicTBOUInt32::SetVec2At( int offset, uint32_t value1, uint32_t value2 ){
	if( offset < 0 || offset + 1 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	uint32_t * const dataUInt = ( uint32_t* )pData;
	dataUInt[ offset ] = value1;
	dataUInt[ offset + 1 ] = value2;
}

void deoglDynamicTBOUInt32::SetVec2At( int offset, const decPoint &value ){
	if( offset < 0 || offset + 1 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	uint32_t * const dataUInt = ( uint32_t* )pData;
	dataUInt[ offset ] = ( uint32_t )value.x;
	dataUInt[ offset + 1 ] = ( uint32_t )value.y;
}

void deoglDynamicTBOUInt32::SetVec3At( int offset, uint32_t value1, uint32_t value2, uint32_t value3 ){
	if( offset < 0 || offset + 2 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	uint32_t * const dataUInt = ( uint32_t* )pData;
	dataUInt[ offset ] = value1;
	dataUInt[ offset + 1 ] = value2;
	dataUInt[ offset + 2 ] = value3;
}

void deoglDynamicTBOUInt32::SetVec3At( int offset, const decPoint3 &value ){
	if( offset < 0 || offset + 2 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	uint32_t * const dataUInt = ( uint32_t* )pData;
	dataUInt[ offset ] = ( uint32_t )value.x;
	dataUInt[ offset + 1 ] = ( uint32_t )value.y;
	dataUInt[ offset + 2 ] = ( uint32_t )value.z;
}

void deoglDynamicTBOUInt32::SetVec4At( int offset, uint32_t value1, uint32_t value2,
uint32_t value3, uint32_t value4 ){
	if( offset < 0 || offset + 3 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	uint32_t * const dataUInt = ( uint32_t* )pData;
	dataUInt[ offset ] = value1;
	dataUInt[ offset + 1 ] = value2;
	dataUInt[ offset + 2 ] = value3;
	dataUInt[ offset + 3 ] = value4;
}

void deoglDynamicTBOUInt32::DebugPrint(){
	deoglRTLogger &logger = pRenderThread.GetLogger();
	logger.LogInfoFormat( "TBO %d-UInt32:", pComponentCount );
	uint32_t *data = ( uint32_t* )pData;
	int i, pixel = 0;
	
	pEnsurePadding();
	
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

GLenum deoglDynamicTBOUInt32::GetTBOFormat(){
	switch( pComponentCount ){
	case 1:
		return GL_R32UI;
		
	case 2:
		return GL_RG32UI;
		
	case 3:
		return GL_RGB32UI;
		
	case 4:
		return GL_RGBA32UI;
		
	default:
		DETHROW( deeInvalidParam );
	}
}
