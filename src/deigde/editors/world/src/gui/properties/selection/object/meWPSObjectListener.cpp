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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "meWPSObject.h"
#include "meWPSObjectListener.h"
#include "../../../../world/meWorld.h"
#include "../../../../world/object/meObject.h"
#include "../../../../world/object/meObjectSelection.h"
#include "../../../../world/object/texture/meObjectTexture.h"
#include "../../../../world/object/texture/meObjectTextureList.h"

#include <dragengine/common/exceptions.h>



// Class meWPSObjectListener
//////////////////////////////

// Constructor, destructor
////////////////////////////

meWPSObjectListener::meWPSObjectListener( meWPSObject &panel ) :
pPanel( panel ){
}

meWPSObjectListener::~meWPSObjectListener(){
}



// Management
///////////////

void meWPSObjectListener::TriggerTableChanged( meWorld* ){
	pPanel.UpdateTriggerTargetLists();
}



void meWPSObjectListener::ObjectSelectionChanged( meWorld *world ){
	meObject * const object = world->GetSelectionObject().GetActive();
	
	pPanel.UpdateSelection();
	pPanel.UpdateEnabled();
	pPanel.UpdateObject();
	pPanel.UpdateGeometry();
	pPanel.UpdatePropertyKeys();
	pPanel.UpdateProperties();
	pPanel.UpdateTextureList();
	pPanel.UpdateIdentifierLists();
	pPanel.UpdateLight();
	
	if( object && ! object->GetActiveTexture() && object->GetTextureCount() > 0 ){
		// combo box displays textures in sorted order. using first texture from object would
		// not necessarily select the top most texture. ensure this is the case
		decStringList names;
		object->GetTextureNameList( names );
		names.SortAscending();
		object->SetActiveTexture( object->GetTextureNamed( names.GetAt( 0 ) ) );
		// the above call has already caused an update so no need to do it again
		return;
	}
	
	pPanel.UpdateTextureEnabled();
	pPanel.UpdateTexture();
	pPanel.UpdateTexPropertyKeys();
	pPanel.UpdateTexProperties();
}

void meWPSObjectListener::ObjectChanged( meWorld*, meObject *object ){
	if( ! object->GetActive() ){
		return;
	}
	
	pPanel.UpdateObject();
}

void meWPSObjectListener::ObjectClassChanged( meWorld*, meObject *object ){
	if( ! object->GetActive() ){
		return;
	}
	
	pPanel.UpdateObject();
	pPanel.UpdatePropertyKeys();
	pPanel.UpdateTexPropertyKeys();
}

void meWPSObjectListener::ObjectGeometryChanged( meWorld*, meObject *object ){
	if( ! object->GetActive() ){
		return;
	}
	
	pPanel.UpdateGeometry();
}

void meWPSObjectListener::ObjectPropertiesChanged( meWorld*, meObject *object ){
	if( ! object->GetActive() ){
		return;
	}
	
	pPanel.UpdateProperties();
	pPanel.UpdateIdentifierLists();
	pPanel.UpdateLight();
}

void meWPSObjectListener::ObjectActivePropertyChanged( meWorld*, meObject *object ){
	if( ! object->GetActive() ){
		return;
	}
	
	pPanel.SelectActiveProperty();
}

void meWPSObjectListener::ObjectTextureCountChanged( meWorld*, meObject *object ){
	if( ! object->GetActive() ){
		return;
	}
	
	pPanel.UpdateTextureList();
	
	if( ! object->GetActiveTexture() && object->GetTextureCount() > 0 ){
		// combo box displays textures in sorted order. using first texture from object would
		// not necessarily select the top most texture. ensure this is the case
		decStringList names;
		object->GetTextureNameList( names );
		names.SortAscending();
		object->SetActiveTexture( object->GetTextureNamed( names.GetAt( 0 ) ) );
		// the above call has already caused an update so no need to do it again
		return;
	}
	
	pPanel.UpdateTextureEnabled();
	pPanel.UpdateTexture();
}

void meWPSObjectListener::ObjectActiveTextureChanged( meWorld*, meObject *object ){
	if( ! object->GetActive() ){
		return;
	}
	
	pPanel.SelectActiveTexture();
	pPanel.UpdateTextureEnabled();
	pPanel.UpdateTexture();
	pPanel.UpdateTexProperties();
}

void meWPSObjectListener::ObjectTextureChanged( meWorld*, meObject *object, meObjectTexture *texture ){
	if( ! object->GetActive() || texture != object->GetActiveTexture() ){
		return;
	}
	
	pPanel.UpdateTexture();
}

void meWPSObjectListener::ObjectTexturePropertiesChanged( meWorld*, meObject *object, meObjectTexture *texture ){
	if( ! object->GetActive() || texture != object->GetActiveTexture() ){
		return;
	}
	
	pPanel.UpdateTexProperties();
	pPanel.UpdateIdentifierLists();
}

void meWPSObjectListener::ObjectTextureActivePropertyChanged( meWorld*, meObject *object, meObjectTexture *texture ){
	if( ! object->GetActive() ){
		return;
	}
	if( texture != object->GetActiveTexture() ){
		return;
	}
	
	pPanel.SelectTexActiveProperty();
}
