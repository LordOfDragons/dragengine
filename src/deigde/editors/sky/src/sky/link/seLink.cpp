/* 
 * Drag[en]gine IGDE Sky Editor
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

#include "seLink.h"
#include "../seSky.h"
#include "../controller/seController.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/curve/decCurveBezierPoint.h>



// Class seLink
/////////////////

// Constructor, destructor
////////////////////////////

seLink::seLink() :
pSky( NULL ),
pIndex( -1 ),

pName( "Link" ),

pController( NULL ),
pRepeat( 1 ),

pSelected( false ),
pActive( false ){
}

seLink::~seLink(){
	if( pController ){
		pController->FreeReference();
	}
}



// Management
///////////////

void seLink::SetSky( seSky *sky ){
	pSky = sky;
}

void seLink::SetIndex( int index ){
	pIndex = index;
}

void seLink::SetName( const char *name ){
	if( pName == name ){
		return;
	}
	
	pName = name;
	if( pSky ){
		pSky->NotifyLinkNameChanged( this );
	}
}

void seLink::NotifyCurveChanged(){
	if( pSky ){
		pSky->NotifyLinkChanged( this );
	}
}

void seLink::SetController( seController *controller ){
	if( controller == pController ){
		return;
	}
	
	if( pController ){
		pController->FreeReference();
	}
	
	pController = controller;
	
	if( controller ){
		controller->AddReference();
	}
	
	if( pSky ){
		pSky->NotifyLinkChanged( this );
	}
}

void seLink::SetRepeat( int repeat ){
	if( repeat == pRepeat ){
		return;
	}
	
	pRepeat = repeat;
	
	if( pSky ){
		pSky->NotifyLinkChanged( this );
	}
}



void seLink::SetSelected( bool selected ){
	pSelected = selected;
}

void seLink::SetActive( bool active ){
	pActive = active;
}
