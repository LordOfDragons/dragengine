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

#include "gdeMAOCWorldPaste.h"
#include "../../../gdeWindowMain.h"
#include "../../../../clipboard/gdeClipboardDataOCWorld.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../../gamedef/objectClass/world/gdeOCWorld.h"
#include "../../../../undosys/objectClass/world/gdeUOCAddWorld.h"

#include <deigde/clipboard/igdeClipboardDataReference.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>


// Class gdeMAOCWorldPaste
////////////////////////////

// Constructor
////////////////

gdeMAOCWorldPaste::gdeMAOCWorldPaste(gdeWindowMain &windowMain) :
gdeBaseMAOCSubObject(windowMain, "Paste Object Class World",
	windowMain.GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste),
	"Paste object class world"){
}


// Management
///////////////

igdeUndo *gdeMAOCWorldPaste::OnActionSubObject(gdeGameDefinition&, gdeObjectClass &objectClass){
	igdeClipboardDataReference clip(pWindowMain.GetClipboard().GetWithTypeName(
		gdeClipboardDataOCWorld::TYPE_NAME));
	if(!clip){
		return nullptr;
	}
	
	const gdeClipboardDataOCWorld &clipOCWorld =
		(const gdeClipboardDataOCWorld &)(igdeClipboardData&)clip;
	
	igdeUndo * const undo = new gdeUOCAddWorld(&objectClass,
		gdeOCWorld::Ref::New(new gdeOCWorld(*clipOCWorld.GetWorld())));
	undo->SetShortInfo("Paste object class world");
	return undo;
}

void gdeMAOCWorldPaste::Update(){
	SetEnabled(GetActiveObjectClass() && pWindowMain.GetClipboard().HasWithTypeName(
		gdeClipboardDataOCWorld::TYPE_NAME));
}
