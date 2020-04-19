/* 
 * Drag[en]gine IGDE Skin Editor
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

#include "seViewConstructed.h"
#include "seViewConstructedListener.h"
#include "../../skin/texture/seTexture.h"
#include "../../skin/property/seProperty.h"
#include "../../skin/property/node/sePropertyNodeGroup.h"

#include <deigde/environment/igdeEnvironment.h>



// Class seViewConstructedListener
////////////////////////////////////

// Constructor, destructor
////////////////////////////

seViewConstructedListener::seViewConstructedListener( seViewConstructed &view ) :
pView( view ){
}

seViewConstructedListener::~seViewConstructedListener(){
}



// Management
///////////////

void seViewConstructedListener::ActiveTextureChanged( seSkin *skin ){
	if( pView.GetSkin() != skin ){
		return;
	}
	
	pView.UpdateLayerRanges();
	pView.UpdateScrollbarRanges();
}

void seViewConstructedListener::PropertyChanged( seSkin *skin, seTexture *texture, seProperty *property ){
	if( pView.GetSkin() != skin || ! texture->GetActive() || ! property->GetActive() ){
		return;
	}
	
	pView.UpdateLayerRanges();
	pView.UpdateScrollbarRanges();
}

void seViewConstructedListener::ActivePropertyChanged( seSkin *skin, seTexture *texture ){
	if( pView.GetSkin() != skin || ! texture->GetActive() ){
		return;
	}
	
	pView.UpdateLayerRanges();
	pView.UpdateScrollbarRanges();
}

void seViewConstructedListener::PropertyNodeChanged( seSkin *skin, seTexture *texture,
seProperty *property, sePropertyNode *node ){
	if( pView.GetSkin() != skin || ! texture->GetActive()
	|| ! property->GetActive() || node != property->GetNodeGroup() ){
		return;
	}
	
	pView.UpdateLayerRanges();
	pView.UpdateScrollbarRanges();
}

void seViewConstructedListener::PropertyActiveNodeLayerChanged( seSkin *skin, seTexture *texture,
seProperty *property ){
	if( pView.GetSkin() != skin || ! texture->GetActive() || ! property->GetActive() ){
		return;
	}
	
	pView.UpdateLayerRanges();
	pView.UpdateScrollbarRanges();
}
