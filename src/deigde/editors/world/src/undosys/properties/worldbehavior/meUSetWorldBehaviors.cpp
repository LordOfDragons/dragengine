/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

<<<<<<<< HEAD:src/modules/graphic/opengl/src/parameters/vr/deoglPVRSubmitDepth.cpp
#include "deoglPVRSubmitDepth.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"
========
#include "meUSetWorldBehaviors.h"
#include "../../../world/meWorld.h"
>>>>>>>> origin/master:src/deigde/editors/world/src/undosys/properties/worldbehavior/meUSetWorldBehaviors.cpp

#include <dragengine/common/exceptions.h>


<<<<<<<< HEAD:src/modules/graphic/opengl/src/parameters/vr/deoglPVRSubmitDepth.cpp
// Class deoglPVRSubmitDepth
//////////////////////////////
========
// Class meUSetWorldBehaviors
///////////////////////////////
>>>>>>>> origin/master:src/deigde/editors/world/src/undosys/properties/worldbehavior/meUSetWorldBehaviors.cpp

// Constructor, destructor
////////////////////////////

<<<<<<<< HEAD:src/modules/graphic/opengl/src/parameters/vr/deoglPVRSubmitDepth.cpp
deoglPVRSubmitDepth::deoglPVRSubmitDepth(deGraphicOpenGl &ogl) :
deoglParameterBool(ogl)
{
	SetName("vrSubmitDepth");
	SetDescription("Submit depth images to VR headset if supported.");
	SetCategory(ecAdvanced);
	SetDisplayName("VR Submit Depth");
}

deoglPVRSubmitDepth::~deoglPVRSubmitDepth() = default;


// Parameter Value
////////////////////

bool deoglPVRSubmitDepth::GetParameterBool(){
	return pOgl.GetConfiguration().GetVRSubmitDepth();
}

void deoglPVRSubmitDepth::SetParameterBool(bool value){
	pOgl.GetConfiguration().SetVRSubmitDepth(value);
========
meUSetWorldBehaviors::meUSetWorldBehaviors(meWorld *world, const meWorldBehavior::List &newValue) :
pWorld(world),
pNewValue(newValue)
{
	DEASSERT_NOTNULL(world)
	
	pOldValue = world->GetWorldBehaviors();
	
	SetShortInfo("@World.USetWorldBehaviors");
	SetLongInfo("");
}

meUSetWorldBehaviors::~meUSetWorldBehaviors() = default;


// Management
///////////////

void meUSetWorldBehaviors::Undo(){
	pWorld->SetWorldBehaviors(pOldValue);
}

void meUSetWorldBehaviors::Redo(){
	pWorld->SetWorldBehaviors(pNewValue);
>>>>>>>> origin/master:src/deigde/editors/world/src/undosys/properties/worldbehavior/meUSetWorldBehaviors.cpp
}
