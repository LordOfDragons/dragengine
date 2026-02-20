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

#include "gdeWPSOCComponent.h"
#include "gdeWPSOCComponentListener.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/objectClass/component/gdeOCComponent.h"

#include <dragengine/common/exceptions.h>



// Class gdeWPSOCComponentListener
////////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeWPSOCComponentListener::gdeWPSOCComponentListener(gdeWPSOCComponent &panel) :
pPanel(panel){
}

gdeWPSOCComponentListener::~gdeWPSOCComponentListener(){
}



// Management
///////////////

void gdeWPSOCComponentListener::BasePathChanged(gdeGameDefinition *gameDefinition){
	if(pPanel.GetGameDefinition() != gameDefinition){
		return;
	}
	
	if(gameDefinition->GetIsProjectGameDef()){
		return;
	}
	
	pPanel.UpdateComponent();
	pPanel.UpdateTextureList();
	pPanel.UpdatePropertyList();
	pPanel.UpdateTexturePropertyList();
}

void gdeWPSOCComponentListener::BaseGameDefinitionsChanged(gdeGameDefinition *gameDefinition){
	if(pPanel.GetGameDefinition() != gameDefinition){
		return;
	}
	
	pPanel.UpdateComponent();
	pPanel.UpdateTextureList();
	pPanel.UpdatePropertyList();
	pPanel.UpdateTexturePropertyList();
}



void gdeWPSOCComponentListener::OCPropertyNameChanged(gdeGameDefinition*,
gdeObjectClass *objectClass, gdeProperty *property){
	if(pPanel.GetObjectClass() != objectClass){
		return;
	}
	
	pPanel.UpdatePropertyList();
}

void gdeWPSOCComponentListener::OCPropertiesChanged(gdeGameDefinition*, gdeObjectClass *objectClass){
	if(pPanel.GetObjectClass() != objectClass){
		return;
	}
	
	pPanel.UpdatePropertyList();
}

void gdeWPSOCComponentListener::OCTexturePropertyNameChanged(gdeGameDefinition*,
gdeObjectClass *objectClass, gdeProperty*){
	if(pPanel.GetObjectClass() != objectClass){
		return;
	}
	
	pPanel.UpdateTexturePropertyList();
}

void gdeWPSOCComponentListener::OCTexturePropertiesChanged(gdeGameDefinition*,
gdeObjectClass *objectClass){
	if(pPanel.GetObjectClass() != objectClass){
		return;
	}
	
	pPanel.UpdateTexturePropertyList();
}

void gdeWPSOCComponentListener::ActiveObjectClassChanged(gdeGameDefinition*){
	pPanel.UpdatePropertyList();
}



void gdeWPSOCComponentListener::OCComponentsChanged(gdeGameDefinition*, gdeObjectClass *objectClass){
	if(pPanel.GetObjectClass() != objectClass){
		return;
	}
	
	pPanel.UpdateComponent();
	pPanel.UpdateTextureList();
}

void gdeWPSOCComponentListener::OCComponentChanged(gdeGameDefinition*,
gdeObjectClass *objectClass, gdeOCComponent *component){
	if(pPanel.GetObjectClass() != objectClass){
		return;
	}
	if(pPanel.GetComponent() != component){
		return;
	}
	
	pPanel.UpdateComponent();
	pPanel.UpdateTextureList();
}

void gdeWPSOCComponentListener::OCComponentActiveTextureChanged(gdeGameDefinition*,
gdeObjectClass *objectClass, gdeOCComponent *component){
	if(pPanel.GetObjectClass() != objectClass){
		return;
	}
	if(pPanel.GetComponent() != component){
		return;
	}
	
	pPanel.SelectActiveTexture();
	pPanel.UpdateTexture();
}

void gdeWPSOCComponentListener::OCComponentTextureChanged(gdeGameDefinition*,
gdeObjectClass *objectClass, gdeOCComponent *component, gdeOCComponentTexture *texture){
	if(pPanel.GetObjectClass() != objectClass){
		return;
	}
	if(pPanel.GetComponent() != component){
		return;
	}
	if(component->GetActiveTexture() != texture){
		return;
	}
	
	pPanel.UpdateTexture();
}

void gdeWPSOCComponentListener::OCComponentTextureNameChanged(gdeGameDefinition*,
gdeObjectClass *objectClass, gdeOCComponent *component, gdeOCComponentTexture*){
	if(pPanel.GetObjectClass() != objectClass){
		return;
	}
	if(pPanel.GetComponent() != component){
		return;
	}
	
	pPanel.UpdateTextureList();
}

void gdeWPSOCComponentListener::OCComponentTexturePropertiesChanged(gdeGameDefinition*,
gdeObjectClass *objectClass, gdeOCComponent *component, gdeOCComponentTexture*){
	if(pPanel.GetObjectClass() != objectClass){
		return;
	}
	if(pPanel.GetComponent() != component){
		return;
	}
	
	pPanel.UpdateTextureProperties();
}

void gdeWPSOCComponentListener::ActiveOCComponentChanged(gdeGameDefinition*){
	pPanel.UpdateComponent();
	pPanel.UpdateTextureList();
}
