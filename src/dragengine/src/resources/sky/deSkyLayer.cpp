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
#include <string.h>

#include "deSkyLayer.h"
#include "deSkyLayerBody.h"
#include "deSkyControllerTarget.h"
#include "../skin/deSkin.h"
#include "../../common/exceptions.h"



// Class deSkyLayer
/////////////////////

// Constructor, destructor
////////////////////////////

deSkyLayer::deSkyLayer() :
pColor( 1.0f, 1.0f, 1.0f ),
pIntensity( 1.0f ),
pTransparency( 1.0f ),

pLightColor( 1.0f, 1.0f, 1.0f ),
pLightIntensity( 0.0f ),
pAmbientIntensity( 0.0f ),

pBodies( NULL ),
pBodyCount( 0 ),

pMulBySkyLight( false ),
pMulBySkyColor( false ){
}

deSkyLayer::~deSkyLayer(){
	if( pBodies ){
		delete [] pBodies;
	}
}



// Management
///////////////

void deSkyLayer::SetOffset( const decVector &offset ){
	pOffset = offset;
}

void deSkyLayer::SetOrientation( const decVector &orientation ){
	pOrientation = orientation;
}

void deSkyLayer::SetColor( const decColor &color ){
	pColor = color;
}

void deSkyLayer::SetIntensity( float intensity ){
	pIntensity = decMath::max( intensity, 0.0f );
}

void deSkyLayer::SetTransparency( float transparency ){
	pTransparency = decMath::clamp( transparency, 0.0f, 1.0f );
}

void deSkyLayer::SetSkin( deSkin *skin ){
	pSkin = skin;
}



void deSkyLayer::SetLightOrientation( const decQuaternion &orientation ){
	pLightOrientation = orientation;
}

void deSkyLayer::SetLightColor( const decColor &color ){
	pLightColor = color;
}

void deSkyLayer::SetLightIntensity( float intensity ){
	pLightIntensity = decMath::max( intensity, 0.0f );
}

void deSkyLayer::SetAmbientIntensity( float intensity ){
	pAmbientIntensity = decMath::max( intensity, 0.0f );
}



void deSkyLayer::SetBodyCount( int count ){
	if( pBodies ){
		delete [] pBodies;
		pBodies = NULL;
		pBodyCount = 0;
	}
	
	if( count > 0 ){
		pBodies = new deSkyLayerBody[ count ];
		pBodyCount = count;
	}
}

deSkyLayerBody &deSkyLayer::GetBodyAt( int index ) const{
	if( index < 0 || index >= pBodyCount ){
		DETHROW( deeInvalidParam );
	}
	return pBodies[ index ];
}



const deSkyControllerTarget &deSkyLayer::GetTarget( eTargets target ) const{
	if( target < etOffsetX || target > etAmbientIntensity ){
		DETHROW( deeInvalidParam );
	}
	return pTargets[ target ];
}

deSkyControllerTarget &deSkyLayer::GetTarget( eTargets target ){
	if( target < etOffsetX || target > etAmbientIntensity ){
		DETHROW( deeInvalidParam );
	}
	return pTargets[ target ];
}



void deSkyLayer::SetMultiplyBySkyLight( bool multiply ){
	pMulBySkyLight = multiply;
}

void deSkyLayer::SetMultiplyBySkyColor( bool multiply ){
	pMulBySkyColor = multiply;
}
