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

#include "debpShapeBox.h"
#include "../dePhysicsBullet.h"

#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/exceptions.h>



// Class debpShapeBox
///////////////////////

// Constructor, destructor
////////////////////////////

debpShapeBox::debpShapeBox(decShapeBox *shape) : debpShape(estBox, shape){
	pSBox = shape;
	SetCollisionVolume(&pCBox);
}

debpShapeBox::~debpShapeBox(){
}



// Management
///////////////

void debpShapeBox::UpdateWithMatrix(const decDMatrix &transformation, const decDVector &scale){
	pCBox.SetCenter(transformation * pSBox->GetPosition());
	pCBox.SetOrientation(pSBox->GetOrientation() * transformation.ToQuaternion());
	pCBox.SetHalfSize(pSBox->GetHalfExtends().Multiply(scale));
}

void debpShapeBox::PrintDebug(dePhysicsBullet &module){
	const decDVector &c = pCBox.GetCenter();
	const decDVector &h = pCBox.GetHalfSize();
	const decQuaternion &o = pCBox.GetOrientation();
	
	module.LogInfoFormat("box: c=(%.4g,%.4g,%.4g) h=(%.4g,%.4g,%.4g) o=(%.4g,%.4g,%.4g,%.4g)", c.x, c.y, c.z, h.x, h.y, h.z, o.x, o.y, o.z, o.w);
}
