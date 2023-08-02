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

#include <stdlib.h>
#include <string.h>

#include "seWPNode.h"
#include "seWPNodeListener.h"
#include "../../skin/seSkin.h"
#include "../../skin/texture/seTexture.h"
#include "../../skin/property/seProperty.h"
#include "../../skin/property/node/sePropertyNode.h"

#include <dragengine/common/exceptions.h>



// Class seWPNodeListener
///////////////////////////

// Constructor, destructor
////////////////////////////

seWPNodeListener::seWPNodeListener( seWPNode &panel ) :
pPanel( panel ){
}

seWPNodeListener::~seWPNodeListener(){
}



// Management
///////////////

void seWPNodeListener::MappedStructureChanged( seSkin *skin ){
	if( skin != pPanel.GetSkin() ){
		return;
	}
	
	pPanel.UpdateMappedTargetList();
}

void seWPNodeListener::MappedNameChanged( seSkin *skin, seMapped* ){
	MappedStructureChanged( skin );
}



void seWPNodeListener::ActiveTextureChanged( seSkin *skin ){
	if( skin != pPanel.GetSkin() ){
		return;
	}
	
	pPanel.UpdateMappedTypeList();
	pPanel.UpdateNode();
}

void seWPNodeListener::PropertyChanged( seSkin *skin, seTexture *texture, seProperty *property ){
	if( skin != pPanel.GetSkin() || ! texture->GetActive() || ! property->GetActive() ){
		return;
	}
	
	pPanel.ShowNodePanel();
	pPanel.UpdateNode();
}

void seWPNodeListener::ActivePropertyChanged( seSkin *skin, seTexture *texture ){
	if( skin != pPanel.GetSkin() || ! texture->GetActive() ){
		return;
	}
	
	pPanel.UpdateOutline();
	pPanel.ShowNodePanel();
	pPanel.UpdateMappedTypeList();
	pPanel.UpdateNode();
}

void seWPNodeListener::PropertyNodeChanged( seSkin *skin, seTexture *texture,
seProperty *property, sePropertyNode* ){
	if( skin != pPanel.GetSkin() || ! texture->GetActive() || ! property->GetActive() ){
		return;
	}
	
	pPanel.UpdateNode();
}

void seWPNodeListener::PropertyNodeStructureChanged( seSkin *skin, seTexture *texture,
seProperty *property ){
	if( skin != pPanel.GetSkin() || ! texture->GetActive() || ! property->GetActive() ){
		return;
	}
	
	pPanel.UpdateOutline();
	pPanel.OutlinerSelectActive();
}

void seWPNodeListener::PropertyNodeSelectionChanged( seSkin *skin, seTexture *texture,
seProperty *property ){
	if( skin != pPanel.GetSkin() || ! texture->GetActive() || ! property->GetActive() ){
		return;
	}
	
	pPanel.UpdateNode();
	pPanel.ShowNodePanel();
}

void seWPNodeListener::PropertyActiveNodeChanged( seSkin *skin, seTexture *texture,
seProperty *property ){
	if( skin != pPanel.GetSkin() || ! texture->GetActive() || ! property->GetActive() ){
		return;
	}
	
	pPanel.OutlinerSelectActive();
	pPanel.UpdateMappedTypeList();
	pPanel.UpdateNode();
	pPanel.ShowNodePanel();
}
