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
	pPanel.UpdateAttachBehaviors();
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

void meWPSObjectListener::ObjectAttachBehaviorsChanged(meWorld*, meObject *object){
	if(!object->GetActive()){
		return;
	}
	
	pPanel.UpdatePropertyKeys();
	pPanel.UpdateProperties();
	pPanel.UpdateAttachBehaviors();
}

void meWPSObjectListener::ObjectActiveAttachBehaviorChanged(meWorld*, meObject *object){
	if(!object->GetActive()){
		return;
	}
	
	pPanel.SelectActiveAttachBehavior();
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
