/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#include "igdeGDCWorld.h"

#include <dragengine/common/exceptions.h>


// Class igdeGDCWorld
///////////////////////

// Constructor, destructor
////////////////////////////

igdeGDCWorld::igdeGDCWorld(){
}

igdeGDCWorld::igdeGDCWorld(const igdeGDCWorld &world) :
pPath(world.pPath),
pPosition(world.pPosition),
pOrientation(world.pOrientation)
{
	int i;
	for(i=0; i<PropertyNameCount; i++){
		pPropertyNames[i] = world.pPropertyNames[i];
	}
}

igdeGDCWorld::~igdeGDCWorld(){
}



// Management
///////////////

void igdeGDCWorld::SetPath(const char *path){
	pPath = path;
}

void igdeGDCWorld::SetPosition(const decDVector &position){
	pPosition = position;
}

void igdeGDCWorld::SetOrientation(const decQuaternion &orientation){
	pOrientation = orientation;
}


bool igdeGDCWorld::IsPropertySet(eProperties property) const{
	return !pPropertyNames[property].IsEmpty();
}

const decString &igdeGDCWorld::GetPropertyName(eProperties property) const{
	return pPropertyNames[property];
}

void igdeGDCWorld::SetPropertyName(eProperties property, const char *name){
	pPropertyNames[property] = name;
}

bool igdeGDCWorld::HasPropertyWithName(const char *name) const{
	int i;
	for(i=0; i<PropertyNameCount; i++){
		if(pPropertyNames[i] == name){
			return true;
		}
	}
	return false;
}
