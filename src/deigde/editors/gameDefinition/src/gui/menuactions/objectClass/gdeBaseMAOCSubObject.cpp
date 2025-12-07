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

#include "gdeBaseMAOCSubObject.h"
#include "../../gdeWindowMain.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeBaseMAOCSubObject
///////////////////////////////

// Constructor
////////////////

gdeBaseMAOCSubObject::gdeBaseMAOCSubObject(gdeWindowMain &windowMain,
	const char *text, igdeIcon *icon, const char *description) :
gdeBaseAction(windowMain, text, icon, description){
}



// Management
///////////////

igdeUndo *gdeBaseMAOCSubObject::OnAction(gdeGameDefinition &gameDefinition){
	gdeObjectClass * const objectClass = gameDefinition.GetActiveObjectClass();
	if(!objectClass){
		return NULL;
	}
	
	return OnActionSubObject(gameDefinition, *objectClass);
}

void gdeBaseMAOCSubObject::Update(){
	SetEnabled(GetActiveObjectClass() != NULL);
}

gdeObjectClass *gdeBaseMAOCSubObject::GetActiveObjectClass() const{
	const gdeGameDefinition * const gameDefinition = pWindowMain.GetActiveGameDefinition();
	if(!gameDefinition){
		return NULL;
	}
	
	switch(gameDefinition->GetSelectedObjectType()){
	case gdeGameDefinition::eotObjectClass:
	case gdeGameDefinition::eotOCBillboard:
	case gdeGameDefinition::eotOCCamera:
	case gdeGameDefinition::eotOCComponent:
	case gdeGameDefinition::eotOCEnvMapProbe:
	case gdeGameDefinition::eotOCLight:
	case gdeGameDefinition::eotOCNavigationBlocker:
	case gdeGameDefinition::eotOCNavigationSpace:
	case gdeGameDefinition::eotOCParticleEmitter:
	case gdeGameDefinition::eotOCForceField:
	case gdeGameDefinition::eotOCSnapPoint:
	case gdeGameDefinition::eotOCSpeaker:
	case gdeGameDefinition::eotOCWorld:
		return gameDefinition->GetActiveObjectClass();
		
	default:
		return NULL;
	}
}
