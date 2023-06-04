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

#include "seBody.h"
#include "../seSky.h"
#include "../layer/seLayer.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>



// Class seBody
////////////////////

// Constructor, destructor
////////////////////////////

seBody::seBody( deEngine *engine ) :
pEngine( engine ),

pLayer( NULL ),

pSize( 5.0f, 5.0f ),
pColor( 1.0f, 1.0f, 1.0f ),

pSelected( false ),
pActive( false )
{
	if( ! engine ){
		DETHROW( deeInvalidParam );
	}
}

seBody::~seBody(){
}



// Management
///////////////

void seBody::SetLayer( seLayer *layer ){
	if( layer == pLayer ){
		return;
	}
	
	pLayer = layer;
	UpdateRelativeResources();
}

void seBody::SetOrientation( const decVector &orientation ){
	if( orientation.IsEqualTo( pOrientation ) ){
		return;
	}
	
	pOrientation = orientation;
	NotifyBodyChanged();
}

void seBody::SetSize( const decVector2 &size ){
	if( size.IsEqualTo( pSize ) ){
		return;
	}
	
	pSize = size;
	NotifyBodyChanged();
}

void seBody::SetColor( const decColor &color ){
	if( color.IsEqualTo( pColor ) ){
		return;
	}
	
	pColor = color;
	NotifyBodyChanged();
}

void seBody::SetSkinPath( const char *skinPath ){
	if( pPathSkin == skinPath ){
		return;
	}
	
	pPathSkin = skinPath;
	pUpdateSkin();
	NotifyBodyChanged();
}



void seBody::SetSelected( bool selected ){
	pSelected = selected;
}

void seBody::SetActive( bool active ){
	pActive = active;
}

void seBody::UpdateRelativeResources(){
	pUpdateSkin();
}



void seBody::NotifyBodyChanged(){
	if( ! pLayer ){
		return;
	}
	
	seSky * const sky = pLayer->GetSky();
	if( ! sky ){
		return;
	}
	
	sky->NotifyBodyChanged( pLayer, this );
}



// Private Functions
//////////////////////

void seBody::pUpdateSkin(){
	if( pPathSkin.IsEmpty() ){
		pEngSkin = NULL;
		return;
	}
	
	deSkinManager &skinMgr = *pEngine->GetSkinManager();
	const char *basePath = "/";
	if( pLayer && pLayer->GetSky() ){
		basePath = pLayer->GetSky()->GetDirectoryPath();
	}
	
	try{
		pEngSkin.TakeOver( skinMgr.LoadSkin( pPathSkin, basePath ) );
		
	}catch( const deException & ){
		if( pLayer && pLayer->GetSky() ){
			pEngSkin = pLayer->GetSky()->GetEnvironment()->GetStockSkin( igdeEnvironment::essError );
		}
	}
}
