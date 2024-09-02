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

#include "deSky.h"
#include "deSkyManager.h"
#include "deSkyLayer.h"
#include "deSkyController.h"
#include "deSkyLink.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../systems/modules/graphic/deBaseGraphicSky.h"



// Class deSky
////////////////

// Constructor, destructor
////////////////////////////

deSky::deSky( deSkyManager *manager ) :
deResource( manager ),

pControllers( NULL ),
pControllerCount( 0 ),

pLinks( NULL ),
pLinkCount( 0 ),

pLayers( NULL ),
pLayerCount( 0 ),

pPeerGraphic( NULL ){
}

deSky::~deSky(){
	if( pPeerGraphic ){
		delete pPeerGraphic;
		pPeerGraphic = NULL;
	}
	
	if( pLayers ){
		delete [] pLayers;
	}
	if( pLinks ){
		delete [] pLinks;
	}
	if( pControllers ){
		delete [] pControllers;
	}
}



// Management
///////////////

void deSky::SetBgColor( const decColor &color ){
	pBgColor = color;
}



void deSky::SetControllerCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pControllers ){
		delete [] pControllers;
		pControllers = NULL;
		pControllerCount = 0;
	}
	
	if( count > 0 ){
		pControllers = new deSkyController[ count ];
		pControllerCount = count;
	}
}

deSkyController &deSky::GetControllerAt( int index ) const{
	if( index < 0 || index >= pControllerCount ){
		DETHROW( deeInvalidParam );
	}
	return pControllers[ index ];
}

int deSky::IndexOfControllerNamed( const char *name ) const{
	int i;
	for( i=0; i<pControllerCount; i++ ){
		if( pControllers[ i ].GetName() == name ){
			return i;
		}
	}
	
	return -1;
}



void deSky::SetLinkCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pLinks ){
		delete [] pLinks;
		pLinks = NULL;
		pLinkCount = 0;
	}
	
	if( count > 0 ){
		pLinks = new deSkyLink[ count ];
		pLinkCount = count;
	}
}

deSkyLink &deSky::GetLinkAt( int index ) const{
	if( index < 0 || index >= pLinkCount ){
		DETHROW( deeInvalidParam );
	}
	return pLinks[ index ];
}



void deSky::SetLayerCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pLayers ){
		delete [] pLayers;
		pLayers = NULL;
		pLayerCount = 0;
	}
	
	if( count > 0 ){
		pLayers = new deSkyLayer[ count ];
		pLayerCount = count;
	}
}

deSkyLayer &deSky::GetLayerAt( int index ) const{
	if( index < 0 || index >= pLayerCount ){
		DETHROW( deeInvalidParam );
	}
	return pLayers[ index ];
}



void deSky::NotifyParametersChanged(){
	if( pPeerGraphic ){
		pPeerGraphic->ParametersChanged();
	}
}



// System Peers
/////////////////

void deSky::SetPeerGraphic( deBaseGraphicSky *peer ){
	if( peer == pPeerGraphic ){
		return;
	}
	
	if( pPeerGraphic ){
		delete pPeerGraphic;
	}
	pPeerGraphic = peer;
}
