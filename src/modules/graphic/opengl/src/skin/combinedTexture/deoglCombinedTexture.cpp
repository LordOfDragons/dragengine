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

#include "deoglCombinedTexture.h"
#include "deoglCombinedTextureList.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTTexture.h"
#include "../../texture/deoglRImage.h"
#include "../../texture/texture2d/deoglTexture.h"

#include <dragengine/common/exceptions.h>



// Class deoglCombinedTexture
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoglCombinedTexture::deoglCombinedTexture( deoglRenderThread &renderThread,
const decColor &color, deoglRImage *images[ 4 ] ) :
pRenderThread( renderThread ),
pColor( color ),
pTexture( NULL ),
pUsageCount( 1 ),
pHashCode( 0 ),
pLLPrev( NULL ),
pLLNext( NULL )
{
	int i;
	
	for( i=0; i<4; i++ ){
		pImages[ i ] = images[ i ];
		if( images[ i ] ){
			images[ i ]->AddReference();
		}
	}
}

deoglCombinedTexture::~deoglCombinedTexture(){
	int i;
	
	if( pTexture ){
		delete pTexture;
	}
	
	for( i=0; i<4; i++ ){
		if( pImages[ i ] ){
			pImages[ i ]->FreeReference();
		}
	}
}



// Management
///////////////

deoglRImage *deoglCombinedTexture::GetImageAt( int component ) const{
	if( component < 0 || component > 3 ){
		DETHROW( deeInvalidParam );
	}
	
	return pImages[ component ];
}



void deoglCombinedTexture::SetTexture( deoglTexture *texture ){
	if( texture == pTexture ){
		return;
	}
	
	if( pTexture ){
		delete pTexture;
	}
	
	pTexture = texture;
}



bool deoglCombinedTexture::Equals( const deoglCombinedTexture &combinedTexture ) const{
	return pHashCode == combinedTexture.pHashCode
		&& pColor.IsEqualTo( combinedTexture.pColor )
		&& pImages[ 0 ] == combinedTexture.pImages[ 0 ]
		&& pImages[ 1 ] == combinedTexture.pImages[ 1 ]
		&& pImages[ 2 ] == combinedTexture.pImages[ 2 ]
		&& pImages[ 3 ] == combinedTexture.pImages[ 3 ];
}

bool deoglCombinedTexture::Equals( const decColor &color, deoglRImage *images[ 4 ] ) const{
	return pColor.IsEqualTo( color )
		&& pImages[ 0 ] == images[ 0 ]
		&& pImages[ 1 ] == images[ 1 ]
		&& pImages[ 2 ] == images[ 2 ]
		&& pImages[ 3 ] == images[ 3 ];
}



void deoglCombinedTexture::CalcHashCode(){
	pHashCode = CalcHashCodeFor( pColor, pImages );
}

unsigned int deoglCombinedTexture::CalcHashCodeFor( const decColor &color, deoglRImage *images[ 4 ] ){
	unsigned int hashCode = 0;
	
	hashCode += ( unsigned int )( color.r * 255.0 );
	hashCode += ( unsigned int )( color.g * 255.0 );
	hashCode += ( unsigned int )( color.b * 255.0 );
	hashCode += ( unsigned int )( color.a * 255.0 );
	
	hashCode += ( unsigned int )( ( intptr_t )images[ 0 ] & 0xffff );
	hashCode += ( unsigned int )( ( intptr_t )images[ 1 ] & 0xffff );
	hashCode += ( unsigned int )( ( intptr_t )images[ 2 ] & 0xffff );
	hashCode += ( unsigned int )( ( intptr_t )images[ 3 ] & 0xffff );
	
	return hashCode;
}

void deoglCombinedTexture::AddUsage(){
	pUsageCount++;
}

void deoglCombinedTexture::RemoveUsage(){
	pUsageCount--;
	
	if( pUsageCount == 0 ){
		int i;
		
		for( i=0; i<4; i++ ){
			if( pImages[ i ] ){
				pImages[ i ]->FreeReference();
				pImages[ i ] = NULL;
			}
		}
		
		// WARNING remove usage typically happens during main thread. if the combined texture
		//         would be removed from it can be potentially deleted. combined texture
		//         though holds references to render images (which can cause deletion loop)
		//         as well as opengl objects (which would incorrectly try to delete them in
		//         the opengl context free main thread). if removing is desired the combined
		//         texture list has to take care of delaying deletion of the opengl objects
		//         until a later time. this makes this call main thread safe
		//pRenderThread.GetTexture().GetCombinedTexture().Remove( this );
	}
}

void deoglCombinedTexture::SetLLPrev( deoglCombinedTexture *entry ){
	pLLPrev = entry;
}

void deoglCombinedTexture::SetLLNext( deoglCombinedTexture *entry ){
	pLLNext = entry;
}
