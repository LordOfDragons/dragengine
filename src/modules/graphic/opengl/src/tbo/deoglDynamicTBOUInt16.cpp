/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglDynamicTBOUInt16.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"

#include <dragengine/common/exceptions.h>



// Class deoglDynamicTBOUInt16
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDynamicTBOUInt16::deoglDynamicTBOUInt16( deoglRenderThread &renderThread, int componentCount ) :
deoglDynamicTBO( renderThread, componentCount, sizeof( uint16_t ) ){
}

deoglDynamicTBOUInt16::~deoglDynamicTBOUInt16(){
}



// Management
///////////////

void deoglDynamicTBOUInt16::AddBool( bool value ){
	if( value ){
		AddInt( ( uint16_t )~1 );
		
	}else{
		AddInt( 0 );
	}
}

void deoglDynamicTBOUInt16::AddInt( uint16_t value ){
	pEnlarge( 1 );
	
	uint16_t * const dataUInt = ( uint16_t* )pData;
	dataUInt[ pDataCount++ ] = value;
}

void deoglDynamicTBOUInt16::AddVec2( uint16_t value1, uint16_t value2 ){
	pEnlarge( 2 );
	
	uint16_t * const dataUInt = ( uint16_t* )pData;
	dataUInt[ pDataCount++ ] = value1;
	dataUInt[ pDataCount++ ] = value2;
}

void deoglDynamicTBOUInt16::AddVec2( const decPoint &value ){
	pEnlarge( 2 );
	
	uint16_t * const dataUInt = ( uint16_t* )pData;
	dataUInt[ pDataCount++ ] = ( uint16_t )value.x;
	dataUInt[ pDataCount++ ] = ( uint16_t )value.y;
}

void deoglDynamicTBOUInt16::AddVec3( uint16_t value1, uint16_t value2, uint16_t value3 ){
	pEnlarge( 3 );
	
	uint16_t * const dataUInt = ( uint16_t* )pData;
	dataUInt[ pDataCount++ ] = value1;
	dataUInt[ pDataCount++ ] = value2;
	dataUInt[ pDataCount++ ] = value3;
}

void deoglDynamicTBOUInt16::AddVec3( const decPoint3 &value ){
	pEnlarge( 3 );
	
	uint16_t * const dataUInt = ( uint16_t* )pData;
	dataUInt[ pDataCount++ ] = ( uint16_t )value.x;
	dataUInt[ pDataCount++ ] = ( uint16_t )value.y;
	dataUInt[ pDataCount++ ] = ( uint16_t )value.z;
}

void deoglDynamicTBOUInt16::AddVec4( uint16_t value1, uint16_t value2, uint16_t value3, uint16_t value4 ){
	pEnlarge( 4 );
	
	uint16_t * const dataUInt = ( uint16_t* )pData;
	dataUInt[ pDataCount++ ] = value1;
	dataUInt[ pDataCount++ ] = value2;
	dataUInt[ pDataCount++ ] = value3;
	dataUInt[ pDataCount++ ] = value4;
}

void deoglDynamicTBOUInt16::SetBoolAt( int offset, bool value ){
	SetIntAt( offset, value ? ( uint16_t )~1 : 0 );
}

void deoglDynamicTBOUInt16::SetIntAt( int offset, uint16_t value ){
	if( offset < 0 || offset >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	uint16_t * const dataUInt = ( uint16_t* )pData;
	dataUInt[ offset ] = value;
}

void deoglDynamicTBOUInt16::SetVec2At( int offset, uint16_t value1, uint16_t value2 ){
	if( offset < 0 || offset + 1 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	uint16_t * const dataUInt = ( uint16_t* )pData;
	dataUInt[ offset ] = value1;
	dataUInt[ offset + 1 ] = value2;
}

void deoglDynamicTBOUInt16::SetVec2At( int offset, const decPoint &value ){
	if( offset < 0 || offset + 1 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	uint16_t * const dataUInt = ( uint16_t* )pData;
	dataUInt[ offset ] = ( uint16_t )value.x;
	dataUInt[ offset + 1 ] = ( uint16_t )value.y;
}

void deoglDynamicTBOUInt16::SetVec3At( int offset, uint16_t value1, uint16_t value2, uint16_t value3 ){
	if( offset < 0 || offset + 2 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	uint16_t * const dataUInt = ( uint16_t* )pData;
	dataUInt[ offset ] = value1;
	dataUInt[ offset + 1 ] = value2;
	dataUInt[ offset + 2 ] = value3;
}

void deoglDynamicTBOUInt16::SetVec3At( int offset, const decPoint3 &value ){
	if( offset < 0 || offset + 2 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	uint16_t * const dataUInt = ( uint16_t* )pData;
	dataUInt[ offset ] = ( uint16_t )value.x;
	dataUInt[ offset + 1 ] = ( uint16_t )value.y;
	dataUInt[ offset + 2 ] = ( uint16_t )value.z;
}

void deoglDynamicTBOUInt16::SetVec4At( int offset, uint16_t value1, uint16_t value2,
uint16_t value3, uint16_t value4 ){
	if( offset < 0 || offset + 3 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	uint16_t * const dataUInt = ( uint16_t* )pData;
	dataUInt[ offset ] = value1;
	dataUInt[ offset + 1 ] = value2;
	dataUInt[ offset + 2 ] = value3;
	dataUInt[ offset + 3 ] = value4;
}

void deoglDynamicTBOUInt16::DebugPrint(){
	deoglRTLogger &logger = pRenderThread.GetLogger();
	logger.LogInfoFormat( "TBO %d-UInt16:", pComponentCount );
	uint16_t *data = ( uint16_t* )pData;
	int i, pixel = 0;
	
	pEnsurePadding();
	
	switch( pComponentCount ){
	case 1:
		for( i=0; i<pDataCount; i++ ){
			const uint16_t v1 = *(data++);
			logger.LogInfoFormat( "%5d: [%5d]", pixel++, v1 );
		}
		break;
		
	case 2:
		for( i=0; i<pDataCount; i+=2 ){
			const uint16_t v1 = *(data++);
			const uint16_t v2 = *(data++);
			logger.LogInfoFormat( "%5d: [%5d | %5d]", pixel++, v1, v2 );
		}
		break;
		
	case 3:
		for( i=0; i<pDataCount; i+=3 ){
			const uint16_t v1 = *(data++);
			const uint16_t v2 = *(data++);
			const uint16_t v3 = *(data++);
			logger.LogInfoFormat( "%5d: [%5d | %5d | %5d]", pixel++, v1, v2, v3 );
		}
		break;
		
	case 4:
		for( i=0; i<pDataCount; i+=4 ){
			const uint16_t v1 = *(data++);
			const uint16_t v2 = *(data++);
			const uint16_t v3 = *(data++);
			const uint16_t v4 = *(data++);
			logger.LogInfoFormat( "%5d: [%5d | %5d | %5d | %5d]", pixel++, v1, v2, v3, v4 );
		}
		break;
	}
}

GLenum deoglDynamicTBOUInt16::GetTBOFormat(){
	switch( pComponentCount ){
	case 1:
		return GL_R16UI;
		
	case 2:
		return GL_RG16UI;
		
	case 3:
		return GL_RGB16UI;
		
	case 4:
		return GL_RGBA16UI;
		
	default:
		DETHROW( deeInvalidParam );
	}
}
