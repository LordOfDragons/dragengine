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
