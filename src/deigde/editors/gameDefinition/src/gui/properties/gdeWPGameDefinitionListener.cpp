/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#include "gdeWPGameDefinition.h"
#include "gdeWPGameDefinitionListener.h"
#include "../../gamedef/gdeGameDefinition.h"

#include <dragengine/common/exceptions.h>



// Class gdeWPGameDefinitionListener
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeWPGameDefinitionListener::gdeWPGameDefinitionListener( gdeWPGameDefinition &panel ) :
pPanel( panel ){
}

gdeWPGameDefinitionListener::~gdeWPGameDefinitionListener(){
}



// Management
///////////////

void gdeWPGameDefinitionListener::GameDefinitionChanged( gdeGameDefinition *gameDefinition ){
	if( gameDefinition != pPanel.GetGameDefinition() ){
		return;
	}
	
	pPanel.UpdateWorld();
}

void gdeWPGameDefinitionListener::BasePathChanged( gdeGameDefinition *gameDefinition ){
	if( gameDefinition != pPanel.GetGameDefinition() ){
		return;
	}
	
	pPanel.UpdateWorld();
}

void gdeWPGameDefinitionListener::BaseGameDefinitionsChanged( gdeGameDefinition *gameDefinition ){
	if( gameDefinition != pPanel.GetGameDefinition() ){
		return;
	}
	
	pPanel.UpdateWorld();
}



void gdeWPGameDefinitionListener::WorldPropertiesChanged( gdeGameDefinition *gameDefinition ){
	if( gameDefinition != pPanel.GetGameDefinition() ){
		return;
	}
	
	pPanel.UpdateWorldProperties();
}

void gdeWPGameDefinitionListener::WorldPropertyChanged( gdeGameDefinition*, gdeProperty *property ){
	if( pPanel.GetActiveWorldProperty() != property ){
		return;
	}
	
	pPanel.UpdateWorldProperty();
}

void gdeWPGameDefinitionListener::WorldPropertyNameChanged( gdeGameDefinition*, gdeProperty *property ){
	if( pPanel.GetActiveWorldProperty() != property ){
		return;
	}
	
	pPanel.UpdateWorldProperties();
}



void gdeWPGameDefinitionListener::DecalPropertiesChanged( gdeGameDefinition *gameDefinition ){
	if( gameDefinition != pPanel.GetGameDefinition() ){
		return;
	}
	
	pPanel.UpdateDecalProperties();
}

void gdeWPGameDefinitionListener::DecalPropertyChanged( gdeGameDefinition*, gdeProperty *property ){
	if( pPanel.GetActiveDecalProperty() != property ){
		return;
	}
	
	pPanel.UpdateDecalProperty();
}

void gdeWPGameDefinitionListener::DecalPropertyNameChanged( gdeGameDefinition*, gdeProperty *property ){
	if( pPanel.GetActiveDecalProperty() != property ){
		return;
	}
	
	pPanel.UpdateDecalProperties();
}



void gdeWPGameDefinitionListener::AutoFindPathObjectClassesChanged( gdeGameDefinition* ){
	pPanel.UpdateAutoFindPathObjectClasses();
}

void gdeWPGameDefinitionListener::AutoFindPathSkinsChanged( gdeGameDefinition* ){
	pPanel.UpdateAutoFindPathSkins();
}

void gdeWPGameDefinitionListener::AutoFindPathSkiesChanged( gdeGameDefinition* ){
	pPanel.UpdateAutoFindPathSkies();
}
