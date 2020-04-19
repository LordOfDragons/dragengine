/* 
 * Drag[en]gine Game Engine
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
