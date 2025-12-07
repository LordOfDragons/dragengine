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

#include "gdeMAObjectClassPaste.h"
#include "../../gdeWindowMain.h"
#include "../../../clipboard/gdeClipboardDataObjectClass.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../undosys/objectClass/gdeUAddObjectClass.h"

#include <deigde/clipboard/igdeClipboardData.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeMAObjectClassPaste
////////////////////////////////

// Constructor
////////////////

gdeMAObjectClassPaste::gdeMAObjectClassPaste(gdeWindowMain &windowMain) :
gdeBaseAction(windowMain, "Paste Object Class",
	windowMain.GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste),
	"Paste object class")
{
}



// Management
///////////////

igdeUndo *gdeMAObjectClassPaste::OnAction(gdeGameDefinition &gameDefinition){
	igdeClipboardData::Ref clip(pWindowMain.GetClipboard()
		.GetWithTypeName(gdeClipboardDataObjectClass::TYPE_NAME));
	if(! clip){
		return NULL;
	}
	
	const gdeClipboardDataObjectClass &clipObjectClass =
		(const gdeClipboardDataObjectClass &)(igdeClipboardData&)clip;
	
	const gdeObjectClassList &list = gameDefinition.GetObjectClasses();
	decString name(clipObjectClass.GetObjectClass()->GetName());
	
	if(list.HasNamed(name)){
		while(true){
			if(! igdeCommonDialogs::GetString(&pWindowMain, "Paste Object Class", "Name:", name)){
				return NULL;
			}
			
			if(list.HasNamed(name)){
				igdeCommonDialogs::Error(&pWindowMain, "Paste Object Class", "Object Class exists already.");
				
			}else{
				break;
			}
		}
	}
	
	const gdeObjectClass::Ref objectClass(gdeObjectClass::Ref::NewWith(*clipObjectClass.GetObjectClass()));
	((gdeObjectClass&)(deObject&)objectClass).SetName(name);
	
	igdeUndo * const undo = new gdeUAddObjectClass(
		&gameDefinition, objectClass);
	undo->SetShortInfo("Paste object class");
	return undo;
}

void gdeMAObjectClassPaste::Update(){
	gdeGameDefinition * const gameDefinition = pWindowMain.GetActiveGameDefinition();
	if(! gameDefinition){
		SetEnabled(false);
		return;
	}
	
	SetEnabled(pWindowMain.GetClipboard().HasWithTypeName(gdeClipboardDataObjectClass::TYPE_NAME));
}
