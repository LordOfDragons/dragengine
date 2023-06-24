/* 
 * Drag[en]gine IGDE
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#include "igdeGizmoMouseKeyListener.h"
#include "igdeGizmoManager.h"

#include <dragengine/common/exceptions.h>



// Class igdeGizmoMouseKeyListener
////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGizmoMouseKeyListener::igdeGizmoMouseKeyListener(
	igdeGizmoManager &gizmoManager, igdeViewRenderWindow &view ) :
pGizmoManager( gizmoManager ),
pView( view ),
pCamera( nullptr ){
}

igdeGizmoMouseKeyListener::~igdeGizmoMouseKeyListener(){
}



// Management
///////////////

void igdeGizmoMouseKeyListener::SetCamera( igdeCamera *camera ){
	pCamera = camera;
}



void igdeGizmoMouseKeyListener::OnButtonPress( igdeWidget*, int button,
const decPoint &position, int modifiers ){
	if( pCamera ){
		pGizmoManager.OnButtonPress( pView, *pCamera, button, position, modifiers );
	}
}

void igdeGizmoMouseKeyListener::OnButtonRelease( igdeWidget*, int button,
const decPoint &position, int modifiers ){
	if( pCamera ){
		pGizmoManager.OnButtonRelease( pView, *pCamera, button, position, modifiers );
	}
}

void igdeGizmoMouseKeyListener::OnMouseMoved( igdeWidget*, const decPoint &position, int modifiers ){
	if( pCamera ){
		pGizmoManager.OnMouseMoved( pView, *pCamera, position, modifiers );
	}
}

void igdeGizmoMouseKeyListener::OnMouseWheeled( igdeWidget*, const decPoint &position,
const decPoint &change, int modifiers ){
	if( pCamera ){
		pGizmoManager.OnMouseWheeled( pView, *pCamera, position, change, modifiers );
	}
}

void igdeGizmoMouseKeyListener::OnKeyPress( igdeWidget*, deInputEvent::eKeyCodes keyCode, int key ){
	pGizmoManager.OnKeyPress( keyCode, key );
}

void igdeGizmoMouseKeyListener::OnKeyRelease( igdeWidget*, deInputEvent::eKeyCodes keyCode, int key ){
	pGizmoManager.OnKeyRelease( keyCode, key );
}
