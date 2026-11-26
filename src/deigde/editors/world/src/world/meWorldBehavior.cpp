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

#include "meWorldBehavior.h"
#include "meWorld.h"

#include <dragengine/common/exceptions.h>


// Definitions
////////////////

#define LOGSOURCE "World Editor"


// Class meWorldBehavior
//////////////////////////

// Constructor, destructor
////////////////////////////

meWorldBehavior::meWorldBehavior(igdeEnvironment &environment) :
pEnvironment(environment),
pWorld(nullptr){
}

meWorldBehavior::meWorldBehavior(igdeEnvironment &environment, const decString &name, const decString &id) :
pEnvironment(environment),
pWorld(nullptr),
pName(name),
pId(id){
}

meWorldBehavior::~meWorldBehavior() = default;


// Management
///////////////

void meWorldBehavior::SetWorld(meWorld *world){
	pWorld = world;
}

void meWorldBehavior::SetName(const decString &name){
	if(pName == name){
		return;
	}
	
	pName = name;
	NotifyChanged();
}

void meWorldBehavior::SetId(const decString &id){
	if(pId == id){
		return;
	}
	
	pId = id;
	NotifyChanged();
}


void meWorldBehavior::NotifyChanged(){
	if(pWorld){
		pWorld->NotifyWorldBehaviorChanged(this);
	}
}
