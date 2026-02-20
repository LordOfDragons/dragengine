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
#include "reWPConstraint.h"
#include "reWPConstraintListener.h"
#include "../../rig/reRig.h"
#include "../../rig/constraint/reRigConstraint.h"
#include "../../rig/constraint/reSelectionConstraints.h"



// Class reWPConstraintListener
/////////////////////////////////

// Constructor, destructor
////////////////////////////

reWPConstraintListener::reWPConstraintListener(reWPConstraint &panel) :
pPanel(panel){
}

reWPConstraintListener::~reWPConstraintListener(){
}



// Notifications
//////////////////

void reWPConstraintListener::BoneCountChanged(reRig *rig){
	pPanel.UpdateBoneLists();
	pPanel.UpdateConstraint();
}

void reWPConstraintListener::BoneChanged(reRig *rig, reRigBone *bone){
	pPanel.UpdateBoneLists();
	pPanel.UpdateConstraint();
}



void reWPConstraintListener::ConstraintChanged(reRig *rig, reRigConstraint *constraint){
	if(constraint == pPanel.GetConstraint()){
		pPanel.UpdateBoneLists(); // because constraint bone could have changed
		pPanel.UpdateConstraint();
	}
}

void reWPConstraintListener::ConstraintDofChanged(reRig *rig, reRigConstraint *constraint,
deColliderConstraint::eDegreesOfFreedom dof){
	if(constraint == pPanel.GetConstraint()){
		pPanel.UpdateConstraint();
	}
}

void reWPConstraintListener::ActiveConstraintChanged(reRig *rig){
	if(rig == pPanel.GetRig()){
		pPanel.SetConstraint(rig->GetSelectionConstraints()->GetActiveConstraint());
	}
}
