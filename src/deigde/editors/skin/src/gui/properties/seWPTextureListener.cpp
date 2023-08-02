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

#include "seWPTexture.h"
#include "seWPTextureListener.h"
#include "../../skin/seSkin.h"
#include "../../skin/texture/seTexture.h"
#include "../../skin/property/seProperty.h"
#include "../../skin/property/node/sePropertyNodeGroup.h"

#include <dragengine/common/exceptions.h>



// Class seWPTextureListener
//////////////////////////////

// Constructor, destructor
////////////////////////////

seWPTextureListener::seWPTextureListener( seWPTexture &panel ) :
pPanel( panel ){
}

seWPTextureListener::~seWPTextureListener(){
}



// Management
///////////////

void seWPTextureListener::MappedStructureChanged( seSkin *skin ){
	if( skin != pPanel.GetSkin() ){
		return;
	}
	
	pPanel.UpdatePropertyMappedTargetList();
}

void seWPTextureListener::MappedNameChanged( seSkin *skin, seMapped* ){
	MappedStructureChanged( skin );
}

void seWPTextureListener::TextureStructureChanged( seSkin *skin ){
	if( skin != pPanel.GetSkin() ){
		return;
	}
	
	pPanel.UpdateTextureList();
}

void seWPTextureListener::TextureChanged( seSkin *skin, seTexture *texture ){
	if( skin != pPanel.GetSkin() || ! texture->GetActive() ){
		return;
	}
	
	pPanel.UpdateTexture();
}

void seWPTextureListener::TextureNameChanged( seSkin *skin, seTexture* ){
	if( skin != pPanel.GetSkin() ){
		return;
	}
	
	pPanel.UpdateTextureList();
}

void seWPTextureListener::ActiveTextureChanged( seSkin *skin ){
	if( skin != pPanel.GetSkin() ){
		return;
	}
	
	pPanel.UpdateTexture();
}



void seWPTextureListener::PropertyStructureChanged( seSkin *skin, seTexture *texture ){
	if( skin != pPanel.GetSkin() || ! texture->GetActive() ){
		return;
	}
	
	pPanel.UpdatePropertyList();
}

void seWPTextureListener::PropertyChanged( seSkin *skin, seTexture *texture, seProperty *property ){
	if( skin != pPanel.GetSkin() || ! texture->GetActive() || ! property->GetActive() ){
		return;
	}
	
	pPanel.ShowPropertyPanel();
	pPanel.UpdateProperty();
}

void seWPTextureListener::ActivePropertyChanged( seSkin *skin, seTexture *texture ){
	if( skin != pPanel.GetSkin() || ! texture->GetActive() ){
		return;
	}
	
	pPanel.ShowPropertyPanel();
	pPanel.SelectActiveProperty();
}

void seWPTextureListener::PropertyNodeChanged( seSkin* skin, seTexture* texture,
seProperty* property, sePropertyNode* node ){
	if( skin != pPanel.GetSkin() || ! texture->GetActive() || ! property->GetActive()
	|| node != property->GetNodeGroup() ){
		return;
	}
	
	pPanel.UpdateProperty();
}
