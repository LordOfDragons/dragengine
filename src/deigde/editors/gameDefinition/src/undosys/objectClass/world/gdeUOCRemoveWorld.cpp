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
#include <string.h>
#include <stdlib.h>

#include "gdeUOCRemoveWorld.h"
#include "../../../gamedef/gdeGameDefinition.h"

#include <dragengine/common/exceptions.h>


// Class gdeUOCRemoveWorld
////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCRemoveWorld::gdeUOCRemoveWorld(gdeObjectClass *objectClass, gdeOCWorld *world) :
pObjectClass(objectClass),
pWorld(world)
{
	DEASSERT_NOTNULL(objectClass)
	DEASSERT_NOTNULL(world)
	DEASSERT_TRUE(objectClass->GetWorlds().Has(world))
	
	SetShortInfo("Remove world");
}


// Management
///////////////

void gdeUOCRemoveWorld::Undo(){
	pObjectClass->GetWorlds().Add(pWorld);
	pObjectClass->NotifyWorldsChanged();
}

void gdeUOCRemoveWorld::Redo(){
	gdeGameDefinition * const gameDefinition = pObjectClass->GetGameDefinition();
	if(gameDefinition && gameDefinition->GetActiveOCWorld()){
		if(gameDefinition->GetSelectedObjectType() == gdeGameDefinition::eotOCWorld){
			gameDefinition->SetSelectedObjectType(gdeGameDefinition::eotObjectClass);
		}
		gameDefinition->SetActiveOCWorld(nullptr);
	}
	
	pObjectClass->GetWorlds().Remove(pWorld);
	pObjectClass->NotifyWorldsChanged();
}
