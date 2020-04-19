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

#include "seWPDynamicSkin.h"
#include "seWPDynamicSkinListener.h"
#include "../../../skin/seSkin.h"
#include "../../../skin/dynamicskin/seDynamicSkin.h"
#include "../../../skin/dynamicskin/seDynamicSkinRenderable.h"

#include <dragengine/common/exceptions.h>



// Class seWPDynamicSkinListener
//////////////////////////////////

// Constructor, destructor
////////////////////////////

seWPDynamicSkinListener::seWPDynamicSkinListener( seWPDynamicSkin &panel ) :
pPanel( panel ){
}

seWPDynamicSkinListener::~seWPDynamicSkinListener(){
}



// Management
///////////////

void seWPDynamicSkinListener::DynamicSkinRenderableStructureChanged( seSkin *skin ){
	if( skin != pPanel.GetSkin() ){
		return;
	}
	
	pPanel.UpdateRenderableList();
}

void seWPDynamicSkinListener::DynamicSkinRenderableChanged( seSkin *skin,
seDynamicSkinRenderable *renderable ){
	if( skin != pPanel.GetSkin() || ! renderable->GetActive() ){
		return;
	}
	
	pPanel.ShowValuePanel();
	pPanel.UpdateRenderable();
}

void seWPDynamicSkinListener::DynamicSkinRenderableNameChanged( seSkin *skin,
seDynamicSkinRenderable *renderable ){
	if( skin != pPanel.GetSkin() || ! renderable->GetActive() ){
		return;
	}
	
	pPanel.UpdateRenderableList();
}

void seWPDynamicSkinListener::DynamicSkinActiveRenderableChanged( seSkin *skin ){
	if( skin != pPanel.GetSkin() ){
		return;
	}
	
	pPanel.SelectActiveRenderable();
	pPanel.ShowValuePanel();
}
