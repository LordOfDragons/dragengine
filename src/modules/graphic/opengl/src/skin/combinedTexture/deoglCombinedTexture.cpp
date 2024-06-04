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

#include "deoglCombinedTexture.h"
#include "deoglCombinedTextureList.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTTexture.h"
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
	}
}

deoglCombinedTexture::~deoglCombinedTexture(){
	if( pTexture ){
		delete pTexture;
	}
}



// Management
///////////////

const deoglRImage::Ref &deoglCombinedTexture::GetImageAt( int component ) const{
	DEASSERT_TRUE( component >= 0 )
	DEASSERT_TRUE( component <= 3 )
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

unsigned int deoglCombinedTexture::CalcHashCodeFor( const decColor &color, const deoglRImage::Ref *images ){
	unsigned int hashCode = 0;
	
	hashCode += ( unsigned int )( color.r * 255.0 );
	hashCode += ( unsigned int )( color.g * 255.0 );
	hashCode += ( unsigned int )( color.b * 255.0 );
	hashCode += ( unsigned int )( color.a * 255.0 );
	
	hashCode += ( unsigned int )( ( intptr_t )( deoglRImage* )images[ 0 ] & 0xffff );
	hashCode += ( unsigned int )( ( intptr_t )( deoglRImage* )images[ 1 ] & 0xffff );
	hashCode += ( unsigned int )( ( intptr_t )( deoglRImage* )images[ 2 ] & 0xffff );
	hashCode += ( unsigned int )( ( intptr_t )( deoglRImage* )images[ 3 ] & 0xffff );
	
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
			pImages[ i ] = nullptr;
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
