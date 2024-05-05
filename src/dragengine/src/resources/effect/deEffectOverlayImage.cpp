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

#include "deEffectChain.h"
#include "deEffectOverlayImage.h"
#include "deEffectVisitor.h"
#include "../image/deImage.h"
#include "../../deEngine.h"
#include "../../systems/modules/graphic/deBaseGraphicEffect.h"
#include "../../common/exceptions.h"



// Class deEffectOverlayImage
///////////////////////////////

// Constructor, destructor
////////////////////////////

deEffectOverlayImage::deEffectOverlayImage( deEffectManager *manager ) :
deEffect( manager ),
pTransparency( 1.0f )
{
	pTexCoords[ 0 ].Set( 0.0f, 0.0f );
	pTexCoords[ 1 ].Set( 1.0f, 0.0f );
	pTexCoords[ 2 ].Set( 1.0f, 1.0f );
	pTexCoords[ 3 ].Set( 0.0f, 1.0f );
}

deEffectOverlayImage::~deEffectOverlayImage(){
}



// Management
///////////////

void deEffectOverlayImage::SetImage( deImage *image ){
	if( image == pImage ){
		return;
	}
	
	pImage = image;
	
	deBaseGraphicEffect * const peer = GetPeerGraphic();
	if( peer ){
		peer->ImageChanged();
	}
}

const decVector2 &deEffectOverlayImage::GetTextureCoordinatesFor( int corner ) const{
	if( corner < 0 || corner > 3 ){
		DETHROW( deeOutOfBoundary );
	}
	
	return pTexCoords[ corner ];
}

void deEffectOverlayImage::SetTextureCoordinatesFor( int corner, const decVector2 &textureCoordinates ){
	if( corner < 0 || corner > 3 ){
		DETHROW( deeOutOfBoundary );
	}
	
	if( textureCoordinates.IsEqualTo( pTexCoords[ corner ] ) ){
		return;
	}
	
	pTexCoords[ corner ] = textureCoordinates;
	
	deBaseGraphicEffect * const peer = GetPeerGraphic();
	if( peer ){
		peer->TextureCoordinatesChanged();
	}
}

void deEffectOverlayImage::SetTransparency( float transparency ){
	transparency = decMath::clamp( transparency, 0.0f, 1.0f );
	if( fabsf( transparency - pTransparency ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pTransparency = transparency;
	
	deBaseGraphicEffect * const peer = GetPeerGraphic();
	if( peer ){
		peer->TransparencyChanged();
	}
}



// visiting
/////////////

void deEffectOverlayImage::Visit( deEffectVisitor &visitor ){
	visitor.VisitOverlayImage( *this );
}
