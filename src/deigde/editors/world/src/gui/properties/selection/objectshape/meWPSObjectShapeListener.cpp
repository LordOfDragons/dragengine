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

#include "meWPSObjectShape.h"
#include "meWPSObjectShapeListener.h"
#include "../../../../world/meWorld.h"
#include "../../../../world/object/meObject.h"

#include <dragengine/common/exceptions.h>



// Class meWPSObjectShapeListener
///////////////////////////////////

// Constructor, destructor
////////////////////////////

meWPSObjectShapeListener::meWPSObjectShapeListener( meWPSObjectShape &panel ) :
pPanel( panel ){
}

meWPSObjectShapeListener::~meWPSObjectShapeListener(){
}



// Management
///////////////

void meWPSObjectShapeListener::ObjectSelectionChanged( meWorld* ){
	pPanel.UpdateListProperties( false );
	pPanel.UpdateSelection();
	pPanel.UpdateActiveShape();
}

void meWPSObjectShapeListener::ObjectClassChanged( meWorld*, meObject *object ){
	if( ! object->GetActive() ){
		return;
	}
	
	pPanel.UpdateListProperties( false );
	pPanel.UpdateSelection();
	pPanel.UpdateActiveShape();
}

void meWPSObjectShapeListener::ObjectGeometryChanged( meWorld*, meObject *object ){
	if( ! object->GetActive() ){
		return;
	}
	
	pPanel.UpdateObjectShapeShapes();
}

void meWPSObjectShapeListener::ObjectPropertiesChanged( meWorld*, meObject *object ){
	if( ! object->GetActive() ){
		return;
	}
	
	pPanel.UpdateListProperties( true );
	pPanel.UpdateSelection();
	pPanel.UpdateActiveShape();
}

void meWPSObjectShapeListener::ObjectActivePropertyChanged( meWorld*, meObject *object ){
	if( ! object->GetActive() ){
		return;
	}
	
	pPanel.UpdateListProperties( false );
	pPanel.UpdateSelection();
	pPanel.UpdateActiveShape();
}



void meWPSObjectShapeListener::ObjectShapeListChanged( meWorld* ){
	pPanel.UpdateSelection();
	pPanel.UpdateActiveShape();
}

void meWPSObjectShapeListener::ObjectShapeSelectionChanged( meWorld* ){
	pPanel.UpdateSelection();
	pPanel.UpdateActiveShape();
}
