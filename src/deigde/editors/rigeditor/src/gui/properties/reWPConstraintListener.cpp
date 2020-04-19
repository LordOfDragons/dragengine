/* 
 * Drag[en]gine IGDE Rig Editor
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
#include "reWPConstraint.h"
#include "reWPConstraintListener.h"
#include "../../rig/reRig.h"
#include "../../rig/constraint/reRigConstraint.h"
#include "../../rig/constraint/reSelectionConstraints.h"



// Class reWPConstraintListener
/////////////////////////////////

// Constructor, destructor
////////////////////////////

reWPConstraintListener::reWPConstraintListener( reWPConstraint &panel ) :
pPanel( panel ){
}

reWPConstraintListener::~reWPConstraintListener(){
}



// Notifications
//////////////////

void reWPConstraintListener::BoneCountChanged( reRig *rig ){
	pPanel.UpdateBoneLists();
	pPanel.UpdateConstraint();
}

void reWPConstraintListener::BoneChanged( reRig *rig, reRigBone *bone ){
	pPanel.UpdateBoneLists();
	pPanel.UpdateConstraint();
}



void reWPConstraintListener::ConstraintChanged( reRig *rig, reRigConstraint *constraint ){
	if( constraint == pPanel.GetConstraint() ){
		pPanel.UpdateBoneLists(); // because constraint bone could have changed
		pPanel.UpdateConstraint();
	}
}

void reWPConstraintListener::ConstraintDofChanged( reRig *rig, reRigConstraint *constraint,
deColliderConstraint::eDegreesOfFreedom dof ){
	if( constraint == pPanel.GetConstraint() ){
		pPanel.UpdateConstraint();
	}
}

void reWPConstraintListener::ActiveConstraintChanged( reRig *rig ){
	if( rig == pPanel.GetRig() ){
		pPanel.SetConstraint( rig->GetSelectionConstraints()->GetActiveConstraint() );
	}
}
