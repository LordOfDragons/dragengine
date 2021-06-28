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
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"

#include <dragengine/common/exceptions.h>



// Class deoglDynamicTBOFloat32
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDynamicTBOFloat32::deoglDynamicTBOFloat32( deoglRenderThread &renderThread, int componentCount ) :
deoglDynamicTBO( renderThread, componentCount, sizeof( float ) ){
}

deoglDynamicTBOFloat32::~deoglDynamicTBOFloat32(){
}



// Management
///////////////

void deoglDynamicTBOFloat32::AddBool( bool value ){
	if( value ){
		AddFloat( 1.0f );
		
	}else{
		AddFloat( 0.0f );
	}
}

void deoglDynamicTBOFloat32::AddFloat( float value ){
	pEnlarge( 1 );
	
	float * const dataFloat = ( float* )pData;
	dataFloat[ pDataCount++ ] = value;
}

void deoglDynamicTBOFloat32::AddVec2( float value1, float value2 ){
	pEnlarge( 2 );
	
	float * const dataFloat = ( float* )pData;
	dataFloat[ pDataCount++ ] = value1;
	dataFloat[ pDataCount++ ] = value2;
}

void deoglDynamicTBOFloat32::AddVec2( const decVector2 &value ){
	pEnlarge( 2 );
	
	float * const dataFloat = ( float* )pData;
	dataFloat[ pDataCount++ ] = value.x;
	dataFloat[ pDataCount++ ] = value.y;
}

void deoglDynamicTBOFloat32::AddVec3( float value1, float value2, float value3 ){
	pEnlarge( 3 );
	
	float * const dataFloat = ( float* )pData;
	dataFloat[ pDataCount++ ] = value1;
	dataFloat[ pDataCount++ ] = value2;
	dataFloat[ pDataCount++ ] = value3;
}

void deoglDynamicTBOFloat32::AddVec3( const decVector &value ){
	pEnlarge( 3 );
	
	float * const dataFloat = ( float* )pData;
	dataFloat[ pDataCount++ ] = value.x;
	dataFloat[ pDataCount++ ] = value.y;
	dataFloat[ pDataCount++ ] = value.z;
}

void deoglDynamicTBOFloat32::AddVec4( float value1, float value2, float value3, float value4 ){
	pEnlarge( 4 );
	
	float * const dataFloat = ( float* )pData;
	dataFloat[ pDataCount++ ] = value1;
	dataFloat[ pDataCount++ ] = value2;
	dataFloat[ pDataCount++ ] = value3;
	dataFloat[ pDataCount++ ] = value4;
}

void deoglDynamicTBOFloat32::AddVec4( const decVector &value, float value4 ){
	pEnlarge( 4 );
	
	float * const dataFloat = ( float* )pData;
	dataFloat[ pDataCount++ ] = value.x;
	dataFloat[ pDataCount++ ] = value.y;
	dataFloat[ pDataCount++ ] = value.z;
	dataFloat[ pDataCount++ ] = value4;
}

void deoglDynamicTBOFloat32::AddVec4( const decVector4 &value ){
	pEnlarge( 4 );
	
	float * const dataFloat = ( float* )pData;
	dataFloat[ pDataCount++ ] = value.x;
	dataFloat[ pDataCount++ ] = value.y;
	dataFloat[ pDataCount++ ] = value.z;
	dataFloat[ pDataCount++ ] = value.w;
}

void deoglDynamicTBOFloat32::AddMat4x3( const decMatrix &value ){
	pEnlarge( 12 );
	
	float * const dataFloat = ( float* )pData;
	dataFloat[ pDataCount++ ] = value.a11;
	dataFloat[ pDataCount++ ] = value.a21;
	dataFloat[ pDataCount++ ] = value.a31;
	
	dataFloat[ pDataCount++ ] = value.a12;
	dataFloat[ pDataCount++ ] = value.a22;
	dataFloat[ pDataCount++ ] = value.a32;
	
	dataFloat[ pDataCount++ ] = value.a13;
	dataFloat[ pDataCount++ ] = value.a23;
	dataFloat[ pDataCount++ ] = value.a33;
	
	dataFloat[ pDataCount++ ] = value.a14;
	dataFloat[ pDataCount++ ] = value.a24;
	dataFloat[ pDataCount++ ] = value.a34;
}

void deoglDynamicTBOFloat32::AddMat3x4( const decMatrix &value ){
	pEnlarge( 12 );
	
	float * const dataFloat = ( float* )pData;
	dataFloat[ pDataCount++ ] = value.a11;
	dataFloat[ pDataCount++ ] = value.a12;
	dataFloat[ pDataCount++ ] = value.a13;
	dataFloat[ pDataCount++ ] = value.a14;
	
	dataFloat[ pDataCount++ ] = value.a21;
	dataFloat[ pDataCount++ ] = value.a22;
	dataFloat[ pDataCount++ ] = value.a23;
	dataFloat[ pDataCount++ ] = value.a24;
	
	dataFloat[ pDataCount++ ] = value.a31;
	dataFloat[ pDataCount++ ] = value.a32;
	dataFloat[ pDataCount++ ] = value.a33;
	dataFloat[ pDataCount++ ] = value.a34;
}

void deoglDynamicTBOFloat32::AddMat3x3( const decMatrix &value ){
	pEnlarge( 9 );
	
	float * const dataFloat = ( float* )pData;
	dataFloat[ pDataCount++ ] = value.a11;
	dataFloat[ pDataCount++ ] = value.a21;
	dataFloat[ pDataCount++ ] = value.a31;
	
	dataFloat[ pDataCount++ ] = value.a12;
	dataFloat[ pDataCount++ ] = value.a22;
	dataFloat[ pDataCount++ ] = value.a32;
	
	dataFloat[ pDataCount++ ] = value.a13;
	dataFloat[ pDataCount++ ] = value.a23;
	dataFloat[ pDataCount++ ] = value.a33;
}

void deoglDynamicTBOFloat32::AddMat3x2( const decMatrix &value ){
	pEnlarge( 6 );
	
	float * const dataFloat = ( float* )pData;
	dataFloat[ pDataCount++ ] = value.a11;
	dataFloat[ pDataCount++ ] = value.a21;
	
	dataFloat[ pDataCount++ ] = value.a12;
	dataFloat[ pDataCount++ ] = value.a22;
	
	dataFloat[ pDataCount++ ] = value.a13;
	dataFloat[ pDataCount++ ] = value.a23;
}

void deoglDynamicTBOFloat32::SetBoolAt( int offset, bool value ){
	SetFloatAt( offset, value ? 1.0f : 0.0f );
}

void deoglDynamicTBOFloat32::SetFloatAt( int offset, float value ){
	if( offset < 0 || offset >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	float * const dataFloat = ( float* )pData;
	dataFloat[ offset ] = value;
}

void deoglDynamicTBOFloat32::SetVec2At( int offset, float value1, float value2 ){
	if( offset < 0 || offset + 1 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	float * const dataFloat = ( float* )pData;
	dataFloat[ offset ] = value1;
	dataFloat[ offset + 1 ] = value2;
}

void deoglDynamicTBOFloat32::SetVec2At( int offset, const decVector2 &value ){
	if( offset < 0 || offset + 1 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	float * const dataFloat = ( float* )pData;
	dataFloat[ offset ] = value.x;
	dataFloat[ offset + 1 ] = value.y;
}

void deoglDynamicTBOFloat32::SetVec3At( int offset, float value1, float value2, float value3 ){
	if( offset < 0 || offset + 2 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	float * const dataFloat = ( float* )pData;
	dataFloat[ offset ] = value1;
	dataFloat[ offset + 1 ] = value2;
	dataFloat[ offset + 2 ] = value3;
}

void deoglDynamicTBOFloat32::SetVec3At( int offset, const decVector &value ){
	if( offset < 0 || offset + 2 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	float * const dataFloat = ( float* )pData;
	dataFloat[ offset ] = value.x;
	dataFloat[ offset + 1 ] = value.y;
	dataFloat[ offset + 2 ] = value.z;
}

void deoglDynamicTBOFloat32::SetVec4At( int offset, float value1, float value2,
float value3, float value4 ){
	if( offset < 0 || offset + 3 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	float * const dataFloat = ( float* )pData;
	dataFloat[ offset ] = value1;
	dataFloat[ offset + 1 ] = value2;
	dataFloat[ offset + 2 ] = value3;
	dataFloat[ offset + 3 ] = value4;
}

void deoglDynamicTBOFloat32::SetVec4At( int offset, const decVector &value, float value4 ){
	if( offset < 0 || offset + 3 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	float * const dataFloat = ( float* )pData;
	dataFloat[ offset ] = value.x;
	dataFloat[ offset + 1 ] = value.y;
	dataFloat[ offset + 2 ] = value.z;
	dataFloat[ offset + 3 ] = value4;
}

void deoglDynamicTBOFloat32::SetVec4At( int offset, const decVector4 &value ){
	if( offset < 0 || offset + 3 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	float * const dataFloat = ( float* )pData;
	dataFloat[ offset ] = value.x;
	dataFloat[ offset + 1 ] = value.y;
	dataFloat[ offset + 2 ] = value.z;
	dataFloat[ offset + 3 ] = value.w;
}

void deoglDynamicTBOFloat32::SetMat4x3At( int offset, const decMatrix &value ){
	if( offset < 0 || offset + 11 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	float * const dataFloat = ( float* )pData;
	dataFloat[ offset ] = value.a11;
	dataFloat[ offset + 1 ] = value.a21;
	dataFloat[ offset + 2 ] = value.a31;
	
	dataFloat[ offset + 3 ] = value.a12;
	dataFloat[ offset + 4 ] = value.a22;
	dataFloat[ offset + 5 ] = value.a32;
	
	dataFloat[ offset + 6 ] = value.a13;
	dataFloat[ offset + 7 ] = value.a23;
	dataFloat[ offset + 8 ] = value.a33;
	
	dataFloat[ offset + 9 ] = value.a14;
	dataFloat[ offset + 10 ] = value.a24;
	dataFloat[ offset + 11 ] = value.a34;
}

void deoglDynamicTBOFloat32::SetMat3x4At( int offset, const decMatrix &value ){
	if( offset < 0 || offset + 11 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	float * const dataFloat = ( float* )pData;
	dataFloat[ offset ] = value.a11;
	dataFloat[ offset + 1 ] = value.a12;
	dataFloat[ offset + 2 ] = value.a13;
	dataFloat[ offset + 3 ] = value.a14;
	
	dataFloat[ offset + 4 ] = value.a21;
	dataFloat[ offset + 5 ] = value.a22;
	dataFloat[ offset + 6 ] = value.a23;
	dataFloat[ offset + 7] = value.a24;
	
	dataFloat[ offset + 8 ] = value.a31;
	dataFloat[ offset + 9 ] = value.a32;
	dataFloat[ offset + 10 ] = value.a33;
	dataFloat[ offset + 11 ] = value.a34;
}

void deoglDynamicTBOFloat32::SetMat3x3At( int offset, const decMatrix &value ){
	if( offset < 0 || offset + 8 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	float * const dataFloat = ( float* )pData;
	dataFloat[ offset ] = value.a11;
	dataFloat[ offset + 1 ] = value.a21;
	dataFloat[ offset + 2 ] = value.a31;
	
	dataFloat[ offset + 3 ] = value.a12;
	dataFloat[ offset + 4 ] = value.a22;
	dataFloat[ offset + 5 ] = value.a32;
	
	dataFloat[ offset + 6 ] = value.a13;
	dataFloat[ offset + 7 ] = value.a23;
	dataFloat[ offset + 8 ] = value.a33;
}

void deoglDynamicTBOFloat32::SetMat3x2At( int offset, const decMatrix &value ){
	if( offset < 0 || offset + 5 >= pDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	float * const dataFloat = ( float* )pData;
	dataFloat[ offset ] = value.a11;
	dataFloat[ offset + 1 ] = value.a21;
	
	dataFloat[ offset + 2 ] = value.a12;
	dataFloat[ offset + 3 ] = value.a22;
	
	dataFloat[ offset + 4 ] = value.a13;
	dataFloat[ offset + 5 ] = value.a23;
}

void deoglDynamicTBOFloat32::DebugPrint(){
	deoglRTLogger &logger = pRenderThread.GetLogger();
	logger.LogInfoFormat( "TBO %d-Float32:", pComponentCount );
	float *data = ( float* )pData;
	int i, pixel = 0;
	
	pEnsurePadding();
	
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

GLenum deoglDynamicTBOFloat32::GetTBOFormat(){
	switch( pComponentCount ){
	case 1:
		return GL_R32F;
		
	case 2:
		return GL_RG32F;
		
	case 3:
		return GL_RGB32F;
		
	case 4:
		return GL_RGBA32F;
		
	default:
		DETHROW( deeInvalidParam );
	}
}
