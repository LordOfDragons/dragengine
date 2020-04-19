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

#include "gdeWPSOCComponent.h"
#include "gdeWPSOCComponentListener.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/objectClass/component/gdeOCComponent.h"

#include <dragengine/common/exceptions.h>



// Class gdeWPSOCComponentListener
////////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeWPSOCComponentListener::gdeWPSOCComponentListener( gdeWPSOCComponent &panel ) :
pPanel( panel ){
}

gdeWPSOCComponentListener::~gdeWPSOCComponentListener(){
}



// Management
///////////////

void gdeWPSOCComponentListener::BasePathChanged( gdeGameDefinition *gameDefinition ){
	if( pPanel.GetGameDefinition() != gameDefinition ){
		return;
	}
	
	if( gameDefinition->GetIsProjectGameDef() ){
		return;
	}
	
	pPanel.UpdateComponent();
	pPanel.UpdateTextureList();
	pPanel.UpdatePropertyList();
	pPanel.UpdateTexturePropertyList();
}

void gdeWPSOCComponentListener::BaseGameDefinitionsChanged( gdeGameDefinition *gameDefinition ){
	if( pPanel.GetGameDefinition() != gameDefinition ){
		return;
	}
	
	pPanel.UpdateComponent();
	pPanel.UpdateTextureList();
	pPanel.UpdatePropertyList();
	pPanel.UpdateTexturePropertyList();
}



void gdeWPSOCComponentListener::OCPropertyNameChanged( gdeGameDefinition*,
gdeObjectClass *objectClass, gdeProperty *property ){
	if( pPanel.GetObjectClass() != objectClass ){
		return;
	}
	
	pPanel.UpdatePropertyList();
}

void gdeWPSOCComponentListener::OCPropertiesChanged( gdeGameDefinition*, gdeObjectClass *objectClass ){
	if( pPanel.GetObjectClass() != objectClass ){
		return;
	}
	
	pPanel.UpdatePropertyList();
}

void gdeWPSOCComponentListener::OCTexturePropertyNameChanged( gdeGameDefinition*,
gdeObjectClass *objectClass, gdeProperty* ){
	if( pPanel.GetObjectClass() != objectClass ){
		return;
	}
	
	pPanel.UpdateTexturePropertyList();
}

void gdeWPSOCComponentListener::OCTexturePropertiesChanged( gdeGameDefinition*,
gdeObjectClass *objectClass ){
	if( pPanel.GetObjectClass() != objectClass ){
		return;
	}
	
	pPanel.UpdateTexturePropertyList();
}

void gdeWPSOCComponentListener::ActiveObjectClassChanged( gdeGameDefinition* ){
	pPanel.UpdatePropertyList();
}



void gdeWPSOCComponentListener::OCComponentsChanged( gdeGameDefinition*, gdeObjectClass *objectClass ){
	if( pPanel.GetObjectClass() != objectClass ){
		return;
	}
	
	pPanel.UpdateComponent();
	pPanel.UpdateTextureList();
}

void gdeWPSOCComponentListener::OCComponentChanged( gdeGameDefinition*,
gdeObjectClass *objectClass, gdeOCComponent *component ){
	if( pPanel.GetObjectClass() != objectClass ){
		return;
	}
	if( pPanel.GetComponent() != component ){
		return;
	}
	
	pPanel.UpdateComponent();
	pPanel.UpdateTextureList();
}

void gdeWPSOCComponentListener::OCComponentActiveTextureChanged( gdeGameDefinition*,
gdeObjectClass *objectClass, gdeOCComponent *component ){
	if( pPanel.GetObjectClass() != objectClass ){
		return;
	}
	if( pPanel.GetComponent() != component ){
		return;
	}
	
	pPanel.SelectActiveTexture();
	pPanel.UpdateTexture();
}

void gdeWPSOCComponentListener::OCComponentTextureChanged( gdeGameDefinition*,
gdeObjectClass *objectClass, gdeOCComponent *component, gdeOCComponentTexture *texture ){
	if( pPanel.GetObjectClass() != objectClass ){
		return;
	}
	if( pPanel.GetComponent() != component ){
		return;
	}
	if( component->GetActiveTexture() != texture ){
		return;
	}
	
	pPanel.UpdateTexture();
}

void gdeWPSOCComponentListener::OCComponentTextureNameChanged( gdeGameDefinition*,
gdeObjectClass *objectClass, gdeOCComponent *component, gdeOCComponentTexture* ){
	if( pPanel.GetObjectClass() != objectClass ){
		return;
	}
	if( pPanel.GetComponent() != component ){
		return;
	}
	
	pPanel.UpdateTextureList();
}

void gdeWPSOCComponentListener::OCComponentTexturePropertiesChanged( gdeGameDefinition*,
gdeObjectClass *objectClass, gdeOCComponent *component, gdeOCComponentTexture* ){
	if( pPanel.GetObjectClass() != objectClass ){
		return;
	}
	if( pPanel.GetComponent() != component ){
		return;
	}
	
	pPanel.UpdateTextureProperties();
}

void gdeWPSOCComponentListener::ActiveOCComponentChanged( gdeGameDefinition* ){
	pPanel.UpdateComponent();
	pPanel.UpdateTextureList();
}
