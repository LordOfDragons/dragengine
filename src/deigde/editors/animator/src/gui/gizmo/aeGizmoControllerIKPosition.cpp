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

#include "aeGizmoControllerIKPosition.h"
#include "../../animator/aeAnimator.h"
#include "../../animator/controller/aeController.h"
#include "../../visitors/aeElementVisitor.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/common/exceptions.h>



// Class aeGizmoControllerIKPosition
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeGizmoControllerIKPosition::aeGizmoControllerIKPosition(
	igdeEnvironment &environment, aeController &controller) :
igdeGizmoMove(environment),
pController(controller)
{
	SetScale(decVector(0.25f, 0.25f, 0.25f));
	
	decLayerMask layerMask;
	layerMask.SetBit(aeAnimator::eclGizmo);
	SetCollisionFilter(decCollisionFilter(layerMask));
	
	SetColliderUserPointer((aeElementVisitable*)this);
}

aeGizmoControllerIKPosition::~aeGizmoControllerIKPosition(){
}



// Management
///////////////

decDVector aeGizmoControllerIKPosition::GetObjectPosition(){
	return decDVector(pController.GetVector());
}

void aeGizmoControllerIKPosition::SetObjectPosition(const decDVector &position){
	pController.SetVector(position.ToVector());
}



// Visiting
/////////////

void aeGizmoControllerIKPosition::VisitElement(aeElementVisitor &visitor){
	visitor.VisitGizmo(this);
}
