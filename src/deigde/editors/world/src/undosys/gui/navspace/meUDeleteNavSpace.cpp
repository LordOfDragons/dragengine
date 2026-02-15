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

#include "meUDeleteNavSpace.h"
#include "meUndoDataNavSpace.h"
#include "../../../world/meWorld.h"
#include "../../../world/navspace/meNavigationSpace.h"
#include "../../../world/navspace/meNavigationSpaceSelection.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/localization/igdeTranslationManager.h>

#include <dragengine/common/exceptions.h>



// Class meUDeleteNavSpace
////////////////////////////

// Constructor, destructor
////////////////////////////

meUDeleteNavSpace::meUDeleteNavSpace(meWorld *world){
	if(!world){
		DETHROW(deeInvalidParam);
	}
	
	world->GetSelectionNavigationSpace().GetSelected().Visit([&](meNavigationSpace *n){
		pNavSpaces.Add(meUndoDataNavSpace::Ref::New(n));
	});
	
	decString text;
	
	igdeTranslationManager &tm = world->GetEnvironment()->GetTranslationManager();
	
	SetShortInfo("@World.UDeleteObject.DeleteObjects");
	if(pNavSpaces.GetCount() > 1){
		text.FormatSafe(tm.Translate("World.UDeleteNavSpace.CountObjects").ToUTF8(), pNavSpaces.GetCount());
		
	}else{
		text = tm.Translate("World.UDeleteNavSpace.OneObject").ToUTF8();
	}
	SetLongInfo(text);
}

meUDeleteNavSpace::~meUDeleteNavSpace(){
}



// Management
///////////////

void meUDeleteNavSpace::Undo(){
	meNavigationSpaceSelection &selection = pWorld->GetSelectionNavigationSpace();
	
	selection.Reset();
	
	pNavSpaces.Visit([&](const meUndoDataNavSpace &n){
		pWorld->AddNavSpace(n.GetNavSpace());
		selection.Add(n.GetNavSpace());
		
		pWorld->NotifyNavSpaceCountChanged();
	});
	
	pWorld->NotifyNavSpaceSelectionChanged();
}

void meUDeleteNavSpace::Redo(){
	meNavigationSpaceSelection &selection = pWorld->GetSelectionNavigationSpace();
	
	pNavSpaces.Visit([&](const meUndoDataNavSpace &n){
		selection.Remove(n.GetNavSpace());
		
		pWorld->RemoveNavSpace(n.GetNavSpace());
		
		pWorld->NotifyNavSpaceCountChanged();
	});
	
	pWorld->NotifyNavSpaceSelectionChanged();
}
