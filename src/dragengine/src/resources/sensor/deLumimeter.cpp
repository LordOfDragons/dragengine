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
#include "deLumimeter.h"
#include "deLumimeterManager.h"
#include "../world/deWorld.h"
#include "../../deEngine.h"
#include "../../systems/modules/graphic/deBaseGraphicLumimeter.h"
#include "../../common/exceptions.h"



// Class deLumimeter
//////////////////////

// Constructor, destructor
////////////////////////////

deLumimeter::deLumimeter( deLumimeterManager *manager ) : deResource( manager ){
	pDirection.Set( 0.0f, 0.0f, 1.0f );
	pConeInnerAngle = PI;
	pConeOuterAngle = PI;
	pConeExponent = 1.0f;
	
	pPeerGraphic = NULL;
	
	pParentWorld = NULL;
	pLLWorldNext = NULL;
	pLLWorldPrev = NULL;
}

deLumimeter::~deLumimeter(){
	if( pPeerGraphic ) delete pPeerGraphic;
}



// Management
///////////////

void deLumimeter::SetPosition( const decDVector &position ){
	if( ! position.IsEqualTo( pPosition ) ){
		pPosition = position;
		
		if( pPeerGraphic ) pPeerGraphic->PositionChanged();
	}
}

void deLumimeter::SetDirection( const decVector &direction ){
	if( ! direction.IsEqualTo( pDirection ) ){
		pDirection = direction;
		
		if( pPeerGraphic ) pPeerGraphic->DirectionChanged();
	}
}

void deLumimeter::SetConeInnerAngle( float angle ){
	if( angle < 0.0f ) angle = 0.0f;
	if( angle > PI ) angle = PI;
	
	if( fabs( angle - pConeInnerAngle ) > 1e-5 ){
		pConeInnerAngle = angle;
		
		if( pPeerGraphic ) pPeerGraphic->ConeParameterChanged();
	}
}

void deLumimeter::SetConeOuterAngle( float angle ){
	if( angle < 0.0f ) angle = 0.0f;
	if( angle > PI ) angle = PI;
	
	if( fabs( angle - pConeOuterAngle ) > 1e-5 ){
		pConeOuterAngle = angle;
		
		if( pPeerGraphic ) pPeerGraphic->ConeParameterChanged();
	}
}

void deLumimeter::SetConeExponent( float exponent ){
	if( exponent < 0.0f ) exponent = 0.0f;
	
	if( fabs( exponent - pConeExponent ) > 1e-5 ){
		pConeExponent = exponent;
		
		if( pPeerGraphic ) pPeerGraphic->ConeParameterChanged();
	}
}



// Measuring
//////////////

float deLumimeter::MeasureLuminance(){
	if( pPeerGraphic ){
		return pPeerGraphic->MeasureLuminance();
		
	}else{
		return 0.0f;
	}
}

decColor deLumimeter::MeasureColor(){
	if( pPeerGraphic ){
		return pPeerGraphic->MeasureColor();
		
	}else{
		return decColor();
	}
}



// Linked List
////////////////

void deLumimeter::SetParentWorld( deWorld *world ){
	pParentWorld = world;
}

void deLumimeter::SetLLWorldPrev( deLumimeter *lumimeter ){
	pLLWorldPrev = lumimeter;
}

void deLumimeter::SetLLWorldNext( deLumimeter *lumimeter ){
	pLLWorldNext = lumimeter;
}



// System Peers
/////////////////

void deLumimeter::SetPeerGraphic( deBaseGraphicLumimeter *peer ){
	if( peer == pPeerGraphic ){
		return;
	}
	
	if( pPeerGraphic ){
		delete pPeerGraphic;
	}
	
	pPeerGraphic = peer;
}
