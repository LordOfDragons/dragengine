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

#include "aeWPLink.h"
#include "aeWPLinkListener.h"
#include "../../animator/aeAnimator.h"

#include "dragengine/common/exceptions.h"



// Class aeWPLinkListener
///////////////////////////

// Constructor, destructor
////////////////////////////

aeWPLinkListener::aeWPLinkListener( aeWPLink &panel ) :
pPanel( panel ){
}

aeWPLinkListener::~aeWPLinkListener(){
}



// Notifications
//////////////////

void aeWPLinkListener::RigChanged( aeAnimator *animator ){
	if( animator != pPanel.GetAnimator() ){
		return;
	}
	
	pPanel.UpdateRigBoneList();
}

void aeWPLinkListener::ControllerNameChanged( aeAnimator *animator, aeController* ){
	if( animator != pPanel.GetAnimator() ){
		return;
	}
	
	pPanel.UpdateControllerList();
}

void aeWPLinkListener::ControllerStructureChanged( aeAnimator *animator ){
	if( animator != pPanel.GetAnimator() ){
		return;
	}
	
	pPanel.UpdateControllerList();
}

void aeWPLinkListener::ActiveLinkChanged( aeAnimator *animator, aeLink* ){
	if( animator != pPanel.GetAnimator() ){
		return;
	}
	
	pPanel.SelectActiveLink();
	pPanel.UpdateLink();
}

void aeWPLinkListener::LinkChanged( aeAnimator*, aeLink *link ){
	if( link != pPanel.GetLink() ){
		return;
	}
	
	pPanel.UpdateLink();
}

void aeWPLinkListener::LinkNameChanged( aeAnimator*, aeLink *link ){
	if( link != pPanel.GetLink() ){
		return;
	}
	
	pPanel.UpdateLinkList();
	pPanel.UpdateLink();
}

void aeWPLinkListener::LinkStructureChanged( aeAnimator *animator ){
	if( animator != pPanel.GetAnimator() ){
		return;
	}
	
	pPanel.UpdateLinkList();
}
