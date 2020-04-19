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

#include "seViewConstructedView.h"
#include "seViewConstructedViewListener.h"
#include "../../skin/texture/seTexture.h"
#include "../../skin/property/seProperty.h"
#include "../../skin/property/node/sePropertyNodeGroup.h"



// Class seViewConstructedViewListener
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

seViewConstructedViewListener::seViewConstructedViewListener( seViewConstructedView &view ) :
pView( view ){
}

seViewConstructedViewListener::~seViewConstructedViewListener(){
}



// Management
///////////////

void seViewConstructedViewListener::ActiveTextureChanged( seSkin *skin ){
	if( pView.GetSkin() != skin ){
		return;
	}
	
	pView.UpdateConstructedCanvas();
	pView.RecreateContentCanvas();
	pView.UpdateMarkers();
}

void seViewConstructedViewListener::PropertyChanged( seSkin *skin, seTexture *texture, seProperty *property ){
	if( pView.GetSkin() != skin || ! texture->GetActive() || ! property->GetActive()
	|| property->GetValueType() != seProperty::evtConstructed ){
		return;
	}
	
	pView.UpdateConstructedCanvas();
	pView.UpdateMarkers();
}

void seViewConstructedViewListener::ActivePropertyChanged( seSkin *skin, seTexture *texture ){
	if( pView.GetSkin() != skin || ! texture->GetActive() ){
		return;
	}
	
	pView.UpdateConstructedCanvas();
	pView.RecreateContentCanvas();
	pView.UpdateMarkers();
}

void seViewConstructedViewListener::PropertyNodeStructureChanged( seSkin *skin,
seTexture *texture, seProperty *property ){
	if( pView.GetSkin() != skin || ! texture->GetActive() || ! property->GetActive() ){
		return;
	}
	
	pView.RecreateContentCanvas();
	pView.UpdateGroupDarkening();
	pView.UpdateMarkers();
}

void seViewConstructedViewListener::PropertyNodeChanged( seSkin *skin, seTexture *texture,
seProperty *property, sePropertyNode *node ){
	if( pView.GetSkin() != skin || ! texture->GetActive() || ! property->GetActive() ){
		return;
	}
	
	if( node == property->GetNodeGroup() ){
		pView.UpdateConstructedCanvas();
		
	}else{
		pView.UpdateContentCanvasParams();
	}
	
	pView.UpdateGroupDarkening();
	pView.UpdateMarkers();
}

void seViewConstructedViewListener::PropertyActiveNodeChanged( seSkin *skin,
seTexture *texture, seProperty *property ){
	if( pView.GetSkin() != skin || ! texture->GetActive() || ! property->GetActive() ){
		return;
	}
	
	pView.UpdateGroupDarkening();
	pView.UpdateMarkers();
}

void seViewConstructedViewListener::PropertyNodeSelectionChanged( seSkin *skin,
seTexture *texture, seProperty *property ){
	if( pView.GetSkin() != skin || ! texture->GetActive() || ! property->GetActive() ){
		return;
	}
	
	pView.UpdateGroupDarkening();
	pView.UpdateMarkers();
}

void seViewConstructedViewListener::PropertyActiveNodeGroupChanged( seSkin *skin,
seTexture *texture, seProperty *property ){
	if( pView.GetSkin() != skin || ! texture->GetActive() || ! property->GetActive() ){
		return;
	}
	
	pView.UpdateGroupDarkening();
	pView.UpdateMarkers();
}

void seViewConstructedViewListener::PropertyActiveNodeLayerChanged( seSkin *skin,
seTexture *texture, seProperty *property ){
	if( pView.GetSkin() != skin || ! texture->GetActive() || ! property->GetActive() ){
		return;
	}
	
	pView.UpdateConstructedCanvas();
	pView.RecreateContentCanvas();
	pView.UpdateMarkers();
}
