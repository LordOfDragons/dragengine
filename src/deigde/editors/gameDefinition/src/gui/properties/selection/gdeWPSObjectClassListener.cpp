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
