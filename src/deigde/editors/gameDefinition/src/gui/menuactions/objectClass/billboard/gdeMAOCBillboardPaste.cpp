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

#include "gdeMAOCBillboardPaste.h"
#include "../../../gdeWindowMain.h"
#include "../../../../clipboard/gdeClipboardDataOCBillboard.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../../gamedef/objectClass/billboard/gdeOCBillboard.h"
#include "../../../../undosys/objectClass/billboard/gdeUOCAddBillboard.h"

#include <deigde/clipboard/igdeClipboardData.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeMAOCBillboardPaste
////////////////////////////////

// Constructor
////////////////

gdeMAOCBillboardPaste::gdeMAOCBillboardPaste(gdeWindowMain &windowMain) :
gdeBaseMAOCSubObject(windowMain, "Paste Object Class Billboard",
	windowMain.GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste),
	"Paste object class billboard")
{
}



// Management
///////////////

igdeUndo *gdeMAOCBillboardPaste::OnActionSubObject(gdeGameDefinition&, gdeObjectClass &objectClass){
	igdeClipboardData::Ref clip(pWindowMain.GetClipboard()
		.GetWithTypeName(gdeClipboardDataOCBillboard::TYPE_NAME));
	if(!clip){
		return NULL;
	}
	
	const gdeClipboardDataOCBillboard &clipOCBillboard =
		(const gdeClipboardDataOCBillboard &)(igdeClipboardData&)clip;
	
	const gdeOCBillboard::Ref billboard(gdeOCBillboard::Ref::NewWith(*clipOCBillboard.GetBillboard()));
	
	igdeUndo * const undo = new gdeUOCAddBillboard(&objectClass,
		billboard);
	undo->SetShortInfo("Paste object class billboard");
	return undo;
}

void gdeMAOCBillboardPaste::Update(){
	SetEnabled(GetActiveObjectClass() != NULL
		&& pWindowMain.GetClipboard().HasWithTypeName(gdeClipboardDataOCBillboard::TYPE_NAME));
}
