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
