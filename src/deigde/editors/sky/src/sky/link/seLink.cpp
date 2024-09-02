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
