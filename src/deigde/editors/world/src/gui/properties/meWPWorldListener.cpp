/* 
 * Drag[en]gine IGDE World Editor
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

#include "meWPWorld.h"
#include "meWPWorldListener.h"
#include "../../world/meWorld.h"

#include "dragengine/common/exceptions.h"



// Class meWPWorldListener
////////////////////////////

// Constructor, destructor
////////////////////////////

meWPWorldListener::meWPWorldListener( meWPWorld &panel ) :
pPanel( panel ){
}

meWPWorldListener::~meWPWorldListener(){
}



// Notifications
//////////////////

void meWPWorldListener::WorldParametersChanged( meWorld* ){
	pPanel.UpdateWorldParameters();
}

void meWPWorldListener::LightingChanged( meWorld* ){
	pPanel.UpdateWorld();
}

void meWPWorldListener::PathFindTestChanged( meWorld* ){
	pPanel.UpdatePathFindTest();
}

void meWPWorldListener::SectorCountChanged( meWorld* ){
	pPanel.UpdateWorld();
}

void meWPWorldListener::SectorSizeChanged( meWorld* ){
	pPanel.UpdateWorld();
}

void meWPWorldListener::PropertiesChanged( meWorld* ){
	pPanel.UpdateProperties();
}

void meWPWorldListener::ActivePropertyChanged( meWorld* ){
	pPanel.SelectActiveProperty();
}

void meWPWorldListener::ObjectPropertiesChanged( meWorld*, meObject* ){
	pPanel.UpdateIdentifierLists();
}

void meWPWorldListener::ObjectTexturePropertiesChanged( meWorld*, meObject*, meObjectTexture* ){
	pPanel.UpdateIdentifierLists();
}
