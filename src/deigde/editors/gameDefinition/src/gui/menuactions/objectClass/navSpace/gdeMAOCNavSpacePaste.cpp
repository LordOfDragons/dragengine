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

#include "gdeMAOCNavSpacePaste.h"
#include "../../../gdeWindowMain.h"
#include "../../../../clipboard/gdeClipboardDataOCNavSpace.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../../gamedef/objectClass/navspace/gdeOCNavigationSpace.h"
#include "../../../../undosys/objectClass/navspace/gdeUOCAddNavSpace.h"

#include <deigde/clipboard/igdeClipboardData.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeMAOCNavSpacePaste
///////////////////////////////

// Constructor
////////////////

gdeMAOCNavSpacePaste::gdeMAOCNavSpacePaste(gdeWindowMain &windowMain) :
gdeBaseMAOCSubObject(windowMain, "Paste Object Class Navigation Space",
	windowMain.GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste),
	"Paste object class navigation space")
{
}



// Management
///////////////

igdeUndo *gdeMAOCNavSpacePaste::OnActionSubObject(gdeGameDefinition&, gdeObjectClass &objectClass){
	igdeClipboardData::Ref clip(pWindowMain.GetClipboard()
		.GetWithTypeName(gdeClipboardDataOCNavSpace::TYPE_NAME));
	if(! clip){
		return NULL;
	}
	
	const gdeClipboardDataOCNavSpace &clipOCNavigationSpace =
		(const gdeClipboardDataOCNavSpace &)(igdeClipboardData&)clip;
	
	const gdeOCNavigationSpace::Ref navSpace(gdeOCNavigationSpace::Ref::NewWith(*clipOCNavigationSpace.GetNavSpace()));
	
	igdeUndo * const undo = new gdeUOCAddNavSpace(&objectClass,
		navSpace);
	undo->SetShortInfo("Paste object class navigation space");
	return undo;
}

void gdeMAOCNavSpacePaste::Update(){
	SetEnabled(GetActiveObjectClass() != NULL
		&& pWindowMain.GetClipboard().HasWithTypeName(gdeClipboardDataOCNavSpace::TYPE_NAME));
}
