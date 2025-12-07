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

#include "debpPSimulatePropFields.h"
#include "../dePhysicsBullet.h"
#include "../debpConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class debpPSimulatePropFields
//////////////////////////////////

// Constructor, destructor
////////////////////////////

debpPSimulatePropFields::debpPSimulatePropFields(dePhysicsBullet &bullet) : debpParameter(bullet)
{
	SetName("simulatePropFields");
	SetType(deModuleParameter::eptBoolean);
	SetDescription("Enables simulation of prop fields under the influence of force fields and colliders");
	SetCategory(ecBasic);
	SetDisplayName("Simulate Prop Fields");
	SetDefaultValue("1");
}

debpPSimulatePropFields::~debpPSimulatePropFields(){
}



// Parameter Value
////////////////////

decString debpPSimulatePropFields::GetParameterValue(){
	return pBullet.GetConfiguration()->GetSimulatePropFields() ? "1" : "0";
}

void debpPSimulatePropFields::SetParameterValue(const char *value){
	pBullet.GetConfiguration()->SetSimulatePropFields(decString(value) == "1");
}
