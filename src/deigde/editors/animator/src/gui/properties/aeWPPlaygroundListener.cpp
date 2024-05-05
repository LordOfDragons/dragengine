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

#include <stdlib.h>
#include <string.h>

#include "aeWPPlayground.h"
#include "aeWPPlaygroundListener.h"
#include "../../animator/aeAnimator.h"

#include "dragengine/common/exceptions.h"



// Class aeWPPlaygroundListener
/////////////////////////////////

// Constructor, destructor
////////////////////////////

aeWPPlaygroundListener::aeWPPlaygroundListener( aeWPPlayground &panel ) :
pPanel( panel ){
}

aeWPPlaygroundListener::~aeWPPlaygroundListener(){
}



// Notifications
//////////////////

void aeWPPlaygroundListener::LocomotionChanged( aeAnimator *animator ){
	if( pPanel.GetAnimator() != animator ){
		return;
	}
	
	pPanel.UpdateLocomotion();
}



void aeWPPlaygroundListener::ControllerChanged( aeAnimator *animator, aeController *controller ){
	if( pPanel.GetAnimator() != animator ){
		return;
	}
	
	pPanel.RebuildControllers();
}

void aeWPPlaygroundListener::ControllerNameChanged( aeAnimator *animator, aeController *controller ){
	if( pPanel.GetAnimator() != animator ){
		return;
	}
	
	pPanel.RebuildControllers();
}

void aeWPPlaygroundListener::ControllerValueChanged( aeAnimator *animator, aeController *controller ){
	if( pPanel.GetAnimator() != animator ){
		return;
	}
	
	pPanel.UpdateControllerValue( controller );
}

void aeWPPlaygroundListener::ControllerStructureChanged( aeAnimator *animator ){
	if( pPanel.GetAnimator() != animator ){
		return;
	}
	
	pPanel.RebuildControllers();
}
