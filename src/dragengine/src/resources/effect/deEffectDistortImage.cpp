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

// includes
#include <stdio.h>
#include <stdlib.h>
#include "deEffectChain.h"
#include "deEffectDistortImage.h"
#include "deEffectVisitor.h"
#include "../image/deImage.h"
#include "../../systems/modules/graphic/deBaseGraphicEffect.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"



// class deEffectDistortImage
///////////////////////////////

// constructor, destructor
////////////////////////////

deEffectDistortImage::deEffectDistortImage( deEffectManager *manager ) : deEffect( manager ){
	pTexCoords[ 0 ].Set( 0.0f, 0.0f );
	pTexCoords[ 1 ].Set( 1.0f, 0.0f );
	pTexCoords[ 2 ].Set( 1.0f, 1.0f );
	pTexCoords[ 3 ].Set( 0.0f, 1.0f );
	pStrengthU = 0.01f;
	pStrengthV = 0.01f;
}

deEffectDistortImage::~deEffectDistortImage(){
}



// Management
///////////////

void deEffectDistortImage::SetImage( deImage *image ){
	if( image == pImage ){
		return;
	}
	
	pImage = image;
	
	deBaseGraphicEffect *graEff = GetPeerGraphic();
	if( graEff ){
		graEff->ImageChanged();
	}
}

const decVector2 &deEffectDistortImage::GetTextureCoordinatesFor( int corner ) const{
	if( corner < 0 || corner > 3 ) DETHROW( deeInvalidParam );
	return pTexCoords[ corner ];
}

void deEffectDistortImage::SetTextureCoordinatesFor( int corner, const decVector2 &textureCoordinates ){
	if( corner < 0 || corner > 3 ) DETHROW( deeInvalidParam );
	
	if( ! textureCoordinates.IsEqualTo( pTexCoords[ corner ] ) ){
		pTexCoords[ corner ] = textureCoordinates;
		
		deBaseGraphicEffect *graEffect = GetPeerGraphic();
		if( graEffect ) graEffect->TextureCoordinatesChanged();
	}
}

void deEffectDistortImage::SetStrengthU( float strength ){
	if( fabs( strength - pStrengthU ) > 1e-5 ){
		pStrengthU = strength;
		
		deBaseGraphicEffect *graEffect = GetPeerGraphic();
		if( graEffect ) graEffect->StrengthChanged();
	}
}

void deEffectDistortImage::SetStrengthV( float strength ){
	if( fabs( strength - pStrengthV ) > 1e-5 ){
		pStrengthV = strength;
		
		deBaseGraphicEffect *graEffect = GetPeerGraphic();
		if( graEffect ) graEffect->StrengthChanged();
	}
}



// Visiting
/////////////

void deEffectDistortImage::Visit( deEffectVisitor &visitor ){
	visitor.VisitDistortImage( *this );
}
