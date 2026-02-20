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
gdeBaseAction(windowMain, "@GameDefinition.Menu.ObjectClassSubclass",
	windowMain.GetEnvironment().GetStockIcon(igdeEnvironment::esiStrongDown),
	"@GameDefinition.Menu.ObjectClassSubclass.ToolTip")
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
	
	while(igdeCommonDialogs::GetString(pWindowMain, "@GameDefinition.Dialog.ObjectClassSubclass.Title", "@GameDefinition.Dialog.ObjectClassSubclass.Name", name)){
		if(list.HasNamed(name)){
			igdeCommonDialogs::Error(pWindowMain, "@GameDefinition.Dialog.ObjectClassSubclass.Title", "@GameDefinition.Dialog.ObjectClassSubclass.ErrorExists");
			continue;
		}
		
		auto subclass(gdeObjectClass::Ref::New(name));
		
		subclass->SetCategory(objectClass->GetCategory());
		subclass->SetHideTags(objectClass->GetHideTags());
		subclass->SetIsGhost(objectClass->GetIsGhost());
		subclass->SetIsAttachableBehavior(objectClass->GetIsAttachableBehavior());
		subclass->SetCanInstantiate(objectClass->GetCanInstantiate());
		subclass->SetPartialHideTags(objectClass->GetPartialHideTags());
		subclass->SetScaleMode(objectClass->GetScaleMode());
		
		const gdeOCInherit::Ref inherit(gdeOCInherit::Ref::New(objectClass->GetName()));
		inherit->SetPropertyPrefix(objectClass->GetDefaultInheritPropertyPrefix());
		subclass->GetInherits().Add(inherit);
		
		return gdeUAddObjectClass::Ref::New(&gameDefinition, subclass);
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
