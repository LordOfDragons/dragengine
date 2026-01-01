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

#include "gdeMAObjectClassSubclass.h"
#include "../../gdeWindowMain.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../gamedef/objectClass/inherit/gdeOCInherit.h"
#include "../../../undosys/objectClass/gdeUAddObjectClass.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeMAObjectClassSubclass
///////////////////////////////////

// Constructor
////////////////

gdeMAObjectClassSubclass::gdeMAObjectClassSubclass(gdeWindowMain &windowMain) :
gdeBaseAction(windowMain, "Subclass Object Class",
	windowMain.GetEnvironment().GetStockIcon(igdeEnvironment::esiStrongDown),
	"Create new Object Class as Subclass of the selected Object Class")
{
}



// Management
///////////////

igdeUndo::Ref gdeMAObjectClassSubclass::OnAction(gdeGameDefinition &gameDefinition){
	gdeObjectClass * const category = gameDefinition.GetActiveObjectClass();
	if(!category || gameDefinition.GetSelectedObjectType() != gdeGameDefinition::eotObjectClass){
		return {};
	}
	
	const gdeObjectClass * const objectClass = gameDefinition.GetActiveObjectClass();
	if(!objectClass){
		return {};
	}
	
	const gdeObjectClass::List &list = gameDefinition.GetObjectClasses();
	decString name(objectClass->GetName());
	
	while(igdeCommonDialogs::GetString(&pWindowMain, "Subclass Object Class", "Name:", name)){
		if(list.HasMatching([&](const gdeObjectClass &oc){
			return oc.GetName() == name;
		})){
			igdeCommonDialogs::Error(&pWindowMain, "Subclass Object Class", "Object Class exists already.");
			continue;
		}
		
		const gdeObjectClass::Ref subclass(gdeObjectClass::Ref::New(name));
		gdeObjectClass &soc = (gdeObjectClass&)(deObject&)subclass;
		
		soc.SetCategory(objectClass->GetCategory());
		soc.SetHideTags(objectClass->GetHideTags());
		soc.SetIsGhost(objectClass->GetIsGhost());
		soc.SetIsAttachableBehavior(objectClass->GetIsAttachableBehavior());
		soc.SetCanInstantiate(objectClass->GetCanInstantiate());
		soc.SetPartialHideTags(objectClass->GetPartialHideTags());
		soc.SetScaleMode(objectClass->GetScaleMode());
		
		const gdeOCInherit::Ref objRefInherit(gdeOCInherit::Ref::New(objectClass->GetName()));
		gdeOCInherit& inherit = (gdeOCInherit&)(deObject&)objRefInherit;
		inherit.SetPropertyPrefix(objectClass->GetDefaultInheritPropertyPrefix());
		soc.GetInherits().Add(&inherit);
		
		return gdeUAddObjectClass::Ref::New(&gameDefinition, &soc);
	}
	return {};
}

void gdeMAObjectClassSubclass::Update(){
	gdeGameDefinition * const gameDefinition = pWindowMain.GetActiveGameDefinition();
	if(!gameDefinition){
		SetEnabled(false);
		return;
	}
	
	SetEnabled(gameDefinition->GetSelectedObjectType() == gdeGameDefinition::eotObjectClass 
		&& gameDefinition->GetActiveObjectClass() != nullptr);
}
