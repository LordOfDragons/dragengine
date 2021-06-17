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

#include "gdeWPSObjectClass.h"
#include "gdeWPSObjectClassListener.h"
#include "../../../gamedef/gdeGameDefinition.h"

#include <dragengine/common/exceptions.h>



// Class gdeWPSObjectClassListener
////////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeWPSObjectClassListener::gdeWPSObjectClassListener( gdeWPSObjectClass &panel ) :
pPanel( panel ){
}

gdeWPSObjectClassListener::~gdeWPSObjectClassListener(){
}



// Management
///////////////

void gdeWPSObjectClassListener::ObjectClassUsedTagsChanged( gdeGameDefinition* ){
	pPanel.UpdateUsedTagsList();
}

void gdeWPSObjectClassListener::ObjectClassCategoriesChanged( gdeGameDefinition* ){
	pPanel.UpdateCategoryList();
}

void gdeWPSObjectClassListener::BasePathChanged( gdeGameDefinition *gameDefinition ){
	if( pPanel.GetGameDefinition() != gameDefinition ){
		return;
	}
	
	if( gameDefinition->GetIsProjectGameDef() ){
		return;
	}
	
	pPanel.UpdateUsedTagsList();
	pPanel.UpdateCategoryList();
	pPanel.UpdateClassLists();
	
	pPanel.UpdateObjectClass();
}

void gdeWPSObjectClassListener::BaseGameDefinitionsChanged( gdeGameDefinition *gameDefinition ){
	if( pPanel.GetGameDefinition() != gameDefinition ){
		return;
	}
	
	pPanel.UpdateUsedTagsList();
	pPanel.UpdateCategoryList();
	pPanel.UpdateClassLists();
	
	pPanel.UpdateObjectClass();
}

void gdeWPSObjectClassListener::DefinedUsedIDsChanged( gdeGameDefinition *gameDefinition ){
	if( pPanel.GetGameDefinition() != gameDefinition ){
		return;
	}
	
	pPanel.UpdateIdentifierLists();
}



void gdeWPSObjectClassListener::ObjectClassStructureChanged( gdeGameDefinition* ){
	pPanel.UpdateClassLists();
}

void gdeWPSObjectClassListener::ObjectClassChanged( gdeGameDefinition*, gdeObjectClass *objectClass ){
	if( pPanel.GetObjectClass() == objectClass ){
		pPanel.UpdateObjectClass();
	}
}

void gdeWPSObjectClassListener::ObjectClassNameChanged( gdeGameDefinition*, gdeObjectClass* ){
	pPanel.UpdateClassLists();
}

void gdeWPSObjectClassListener::OCPropertyChanged( gdeGameDefinition*, gdeObjectClass*,
gdeProperty *property ){
	if( pPanel.GetProperty() == property ){
		pPanel.UpdateProperty();
	}
}

void gdeWPSObjectClassListener::OCPropertyNameChanged( gdeGameDefinition*,
gdeObjectClass *objectClass, gdeProperty* ){
	if( pPanel.GetObjectClass() == objectClass ){
		pPanel.UpdateProperties();
		pPanel.UpdatePropertyValueKeys();
	}
}

void gdeWPSObjectClassListener::OCPropertiesChanged( gdeGameDefinition*,
gdeObjectClass *objectClass ){
	if( pPanel.GetObjectClass() == objectClass ){
		pPanel.UpdateProperties();
		pPanel.UpdatePropertyValueKeys();
	}
}

void gdeWPSObjectClassListener::OCPropertyValuesChanged( gdeGameDefinition*,
gdeObjectClass *objectClass ){
	if( pPanel.GetObjectClass() == objectClass ){
		pPanel.UpdatePropertyValues();
	}
}

void gdeWPSObjectClassListener::OCTexturePropertyChanged( gdeGameDefinition*,
gdeObjectClass*, gdeProperty *property ){
	if( pPanel.GetTextureProperty() == property ){
		pPanel.UpdateTextureProperty();
	}
}

void gdeWPSObjectClassListener::OCTexturePropertyNameChanged( gdeGameDefinition*,
gdeObjectClass *objectClass, gdeProperty* ){
	if( pPanel.GetObjectClass() == objectClass ){
		pPanel.UpdateTextureProperties();
	}
}

void gdeWPSObjectClassListener::OCTexturePropertiesChanged( gdeGameDefinition*,
gdeObjectClass *objectClass ){
	if( pPanel.GetObjectClass() == objectClass ){
		pPanel.UpdateTextureProperties();
	}
}

void gdeWPSObjectClassListener::ActiveObjectClassChanged( gdeGameDefinition* ){
	pPanel.UpdateObjectClass();
}

void gdeWPSObjectClassListener::OCInheritsChanged( gdeGameDefinition*, gdeObjectClass* objectClass){
	if( pPanel.GetObjectClass() != objectClass ){
		return;
	}
	
	pPanel.UpdateInherits();
	pPanel.UpdatePropertyValueKeys();
}

void gdeWPSObjectClassListener::OCInheritChanged( gdeGameDefinition*, gdeObjectClass *objectClass, gdeOCInherit* ){
	if( pPanel.GetObjectClass() != objectClass ){
		return;
	}
	
	pPanel.UpdateInherits();
	pPanel.UpdatePropertyValueKeys();
}

void gdeWPSObjectClassListener::OCTexturesChanged( gdeGameDefinition*, gdeObjectClass *objectClass ){
	if( pPanel.GetObjectClass() != objectClass ){
		return;
	}
	
	pPanel.UpdateTextureList();
}

void gdeWPSObjectClassListener::OCTextureChanged( gdeGameDefinition*, gdeObjectClass *objectClass, gdeOCComponentTexture *texture ){
	if( pPanel.GetObjectClass() != objectClass || pPanel.GetTexture() != texture ){
		return;
	}
	
	pPanel.UpdateTexture();
}

void gdeWPSObjectClassListener::OCActiveTextureChanged( gdeGameDefinition*, gdeObjectClass *objectClass ){
	if( pPanel.GetObjectClass() != objectClass ){
		return;
	}
	
	pPanel.SelectActiveTexture();
	pPanel.UpdateTexture();
}
