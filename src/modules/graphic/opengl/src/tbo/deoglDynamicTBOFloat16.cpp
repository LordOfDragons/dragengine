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

#include "deoglDynamicTBOFloat16.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"

#include <dragengine/common/exceptions.h>



// Class deoglDynamicTBOFloat16
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDynamicTBOFloat16::deoglDynamicTBOFloat16( deoglRenderThread &renderThread, int componentCount ) :
deoglDynamicTBO( renderThread, componentCount, sizeof( HALF_FLOAT ) ){
}

deoglDynamicTBOFloat16::~deoglDynamicTBOFloat16(){
}



// Management
///////////////

void deoglDynamicTBOFloat16::AddBool( bool value ){
	AddFloat( value ? 1.0f : 0.0f );
}

void deoglDynamicTBOFloat16::AddFloat( float value ){
	pEnlarge( 1 );
	
	HALF_FLOAT * const dataFloat = ( HALF_FLOAT* )pData;
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value );
}

void deoglDynamicTBOFloat16::AddVec2( float value1, float value2 ){
	pEnlarge( 2 );
	
	HALF_FLOAT * const dataFloat = ( HALF_FLOAT* )pData;
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value1 );
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value2 );
}

void deoglDynamicTBOFloat16::AddVec2( const decVector2 &value ){
	pEnlarge( 2 );
	
	HALF_FLOAT * const dataFloat = ( HALF_FLOAT* )pData;
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.x );
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.y );
}

void deoglDynamicTBOFloat16::AddVec3( float value1, float value2, float value3 ){
	pEnlarge( 3 );
	
	HALF_FLOAT * const dataFloat = ( HALF_FLOAT* )pData;
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value1 );
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value2 );
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value3 );
}

void deoglDynamicTBOFloat16::AddVec3( const decVector &value ){
	pEnlarge( 3 );
	
	HALF_FLOAT * const dataFloat = ( HALF_FLOAT* )pData;
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.x );
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.y );
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.z );
}

void deoglDynamicTBOFloat16::AddVec4( float value1, float value2, float value3, float value4 ){
	pEnlarge( 4 );
	
	HALF_FLOAT * const dataFloat = ( HALF_FLOAT* )pData;
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value1 );
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value2 );
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value3 );
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value4 );
}

void deoglDynamicTBOFloat16::AddVec4( const decVector &value, float value4 ){
	pEnlarge( 4 );
	
	HALF_FLOAT * const dataFloat = ( HALF_FLOAT* )pData;
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.x );
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.y );
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.z );
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value4 );
}

void deoglDynamicTBOFloat16::AddVec4( const decVector4 &value ){
	pEnlarge( 4 );
	
	HALF_FLOAT * const dataFloat = ( HALF_FLOAT* )pData;
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.x );
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.y );
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.z );
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.w );
}

void deoglDynamicTBOFloat16::AddMat4x3( const decMatrix &value ){
	pEnlarge( 12 );
	
	HALF_FLOAT * const dataFloat = ( HALF_FLOAT* )pData;
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.a11 );
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.a21 );
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.a31 );
	
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.a12 );
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.a22 );
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.a32 );
	
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.a13 );
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.a23 );
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.a33 );
	
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.a14 );
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.a24 );
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.a34 );
}

void deoglDynamicTBOFloat16::AddMat3x4( const decMatrix &value ){
	pEnlarge( 12 );
	
	HALF_FLOAT * const dataFloat = ( HALF_FLOAT* )pData;
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.a11 );
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.a12 );
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.a13 );
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.a14 );
	
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.a21 );
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.a22 );
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.a23 );
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.a24 );
	
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.a31 );
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.a32 );
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.a33 );
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.a34 );
}

void deoglDynamicTBOFloat16::AddMat3x3( const decMatrix &value ){
	pEnlarge( 9 );
	
	HALF_FLOAT * const dataFloat = ( HALF_FLOAT* )pData;
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.a11 );
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.a21 );
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.a31 );
	
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.a12 );
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.a22 );
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.a32 );
	
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.a13 );
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.a23 );
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.a33 );
}

void deoglDynamicTBOFloat16::AddMat3x2( const decMatrix &value ){
	pEnlarge( 6 );
	
	HALF_FLOAT * const dataFloat = ( HALF_FLOAT* )pData;
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.a11 );
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.a21 );
	
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.a12 );
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.a22 );
	
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.a13 );
	dataFloat[ pDataCount++ ] = convertFloatToHalf( value.a23 );
}

void deoglDynamicTBOFloat16::SetBoolAt( int offset, bool value ){
	SetFloatAt( offset, value ? 1.0f : 0.0f );
}

void deoglDynamicTBOFloat16::SetFloatAt( int offset, float value ){
	if( offset < 0 || offset >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	HALF_FLOAT * const dataFloat = ( HALF_FLOAT* )pData;
	dataFloat[ offset ] = convertFloatToHalf( value );
}

void deoglDynamicTBOFloat16::SetVec2At( int offset, float value1, float value2 ){
	if( offset < 0 || offset + 1 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	HALF_FLOAT * const dataFloat = ( HALF_FLOAT* )pData;
	dataFloat[ offset ] = convertFloatToHalf( value1 );
	dataFloat[ offset + 1 ] = convertFloatToHalf( value2 );
}

void deoglDynamicTBOFloat16::SetVec2At( int offset, const decVector2 &value ){
	if( offset < 0 || offset + 1 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	HALF_FLOAT * const dataFloat = ( HALF_FLOAT* )pData;
	dataFloat[ offset ] = convertFloatToHalf( value.x );
	dataFloat[ offset + 1 ] = convertFloatToHalf( value.y );
}

void deoglDynamicTBOFloat16::SetVec3At( int offset, float value1, float value2, float value3 ){
	if( offset < 0 || offset + 2 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	HALF_FLOAT * const dataFloat = ( HALF_FLOAT* )pData;
	dataFloat[ offset ] = convertFloatToHalf( value1 );
	dataFloat[ offset + 1 ] = convertFloatToHalf( value2 );
	dataFloat[ offset + 2 ] = convertFloatToHalf( value3 );
}

void deoglDynamicTBOFloat16::SetVec3At( int offset, const decVector &value ){
	if( offset < 0 || offset + 2 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	HALF_FLOAT * const dataFloat = ( HALF_FLOAT* )pData;
	dataFloat[ offset ] = convertFloatToHalf( value.x );
	dataFloat[ offset + 1 ] = convertFloatToHalf( value.y );
	dataFloat[ offset + 2 ] = convertFloatToHalf( value.z );
}

void deoglDynamicTBOFloat16::SetVec4At( int offset, float value1, float value2,
float value3, float value4 ){
	if( offset < 0 || offset + 3 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	HALF_FLOAT * const dataFloat = ( HALF_FLOAT* )pData;
	dataFloat[ offset ] = convertFloatToHalf( value1 );
	dataFloat[ offset + 1 ] = convertFloatToHalf( value2 );
	dataFloat[ offset + 2 ] = convertFloatToHalf( value3 );
	dataFloat[ offset + 3 ] = convertFloatToHalf( value4 );
}

void deoglDynamicTBOFloat16::SetVec4At( int offset, const decVector &value, float value4 ){
	if( offset < 0 || offset + 3 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	HALF_FLOAT * const dataFloat = ( HALF_FLOAT* )pData;
	dataFloat[ offset ] = convertFloatToHalf( value.x );
	dataFloat[ offset + 1 ] = convertFloatToHalf( value.y );
	dataFloat[ offset + 2 ] = convertFloatToHalf( value.z );
	dataFloat[ offset + 3 ] = convertFloatToHalf( value4 );
}

void deoglDynamicTBOFloat16::SetVec4At( int offset, const decVector4 &value ){
	if( offset < 0 || offset + 3 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	HALF_FLOAT * const dataFloat = ( HALF_FLOAT* )pData;
	dataFloat[ offset ] = convertFloatToHalf( value.x );
	dataFloat[ offset + 1 ] = convertFloatToHalf( value.y );
	dataFloat[ offset + 2 ] = convertFloatToHalf( value.z );
	dataFloat[ offset + 3 ] = convertFloatToHalf( value.w );
}

void deoglDynamicTBOFloat16::SetMat4x3At( int offset, const decMatrix &value ){
	if( offset < 0 || offset + 11 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	HALF_FLOAT * const dataFloat = ( HALF_FLOAT* )pData;
	dataFloat[ offset ] = convertFloatToHalf( value.a11 );
	dataFloat[ offset + 1 ] = convertFloatToHalf( value.a21 );
	dataFloat[ offset + 2 ] = convertFloatToHalf( value.a31 );
	
	dataFloat[ offset + 3 ] = convertFloatToHalf( value.a12 );
	dataFloat[ offset + 4 ] = convertFloatToHalf( value.a22 );
	dataFloat[ offset + 5 ] = convertFloatToHalf( value.a32 );
	
	dataFloat[ offset + 6 ] = convertFloatToHalf( value.a13 );
	dataFloat[ offset + 7 ] = convertFloatToHalf( value.a23 );
	dataFloat[ offset + 8 ] = convertFloatToHalf( value.a33 );
	
	dataFloat[ offset + 9 ] = convertFloatToHalf( value.a14 );
	dataFloat[ offset + 10 ] = convertFloatToHalf( value.a24 );
	dataFloat[ offset + 11 ] = convertFloatToHalf( value.a34 );
}

void deoglDynamicTBOFloat16::SetMat3x4At( int offset, const decMatrix &value ){
	if( offset < 0 || offset + 11 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	HALF_FLOAT * const dataFloat = ( HALF_FLOAT* )pData;
	dataFloat[ offset ] = convertFloatToHalf( value.a11 );
	dataFloat[ offset + 1 ] = convertFloatToHalf( value.a12 );
	dataFloat[ offset + 2 ] = convertFloatToHalf( value.a13 );
	dataFloat[ offset + 3 ] = convertFloatToHalf( value.a14 );
	
	dataFloat[ offset + 4 ] = convertFloatToHalf( value.a21 );
	dataFloat[ offset + 5 ] = convertFloatToHalf( value.a22 );
	dataFloat[ offset + 6 ] = convertFloatToHalf( value.a23 );
	dataFloat[ offset + 7] = convertFloatToHalf( value.a24 );
	
	dataFloat[ offset + 8 ] = convertFloatToHalf( value.a31 );
	dataFloat[ offset + 9 ] = convertFloatToHalf( value.a32 );
	dataFloat[ offset + 10 ] = convertFloatToHalf( value.a33 );
	dataFloat[ offset + 11 ] = convertFloatToHalf( value.a34 );
}

void deoglDynamicTBOFloat16::SetMat3x3At( int offset, const decMatrix &value ){
	if( offset < 0 || offset + 8 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	HALF_FLOAT * const dataFloat = ( HALF_FLOAT* )pData;
	dataFloat[ offset ] = convertFloatToHalf( value.a11 );
	dataFloat[ offset + 1 ] = convertFloatToHalf( value.a21 );
	dataFloat[ offset + 2 ] = convertFloatToHalf( value.a31 );
	
	dataFloat[ offset + 3 ] = convertFloatToHalf( value.a12 );
	dataFloat[ offset + 4 ] = convertFloatToHalf( value.a22 );
	dataFloat[ offset + 5 ] = convertFloatToHalf( value.a32 );
	
	dataFloat[ offset + 6 ] = convertFloatToHalf( value.a13 );
	dataFloat[ offset + 7 ] = convertFloatToHalf( value.a23 );
	dataFloat[ offset + 8 ] = convertFloatToHalf( value.a33 );
}

void deoglDynamicTBOFloat16::SetMat3x2At( int offset, const decMatrix &value ){
	if( offset < 0 || offset + 5 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	HALF_FLOAT * const dataFloat = ( HALF_FLOAT* )pData;
	dataFloat[ offset ] = convertFloatToHalf( value.a11 );
	dataFloat[ offset + 1 ] = convertFloatToHalf( value.a21 );
	
	dataFloat[ offset + 2 ] = convertFloatToHalf( value.a12 );
	dataFloat[ offset + 3 ] = convertFloatToHalf( value.a22 );
	
	dataFloat[ offset + 4 ] = convertFloatToHalf( value.a13 );
	dataFloat[ offset + 5 ] = convertFloatToHalf( value.a23 );
}

void deoglDynamicTBOFloat16::DebugPrint(){
	deoglRTLogger &logger = pRenderThread.GetLogger();
	logger.LogInfoFormat( "TBO %d-Float16:", pComponentCount );
	HALF_FLOAT *data = ( HALF_FLOAT* )pData;
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

GLenum deoglDynamicTBOFloat16::GetTBOFormat(){
	switch( pComponentCount ){
	case 1:
		return GL_R16F;
		
	case 2:
		return GL_RG16F;
		
	case 3:
		return GL_RGB16F;
		
	case 4:
		return GL_RGBA16F;
		
	default:
		DETHROW( deeInvalidParam );
	}
}
