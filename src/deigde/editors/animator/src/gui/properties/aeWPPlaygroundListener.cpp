/* 
 * Drag[en]gine IGDE Animator Editor
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
