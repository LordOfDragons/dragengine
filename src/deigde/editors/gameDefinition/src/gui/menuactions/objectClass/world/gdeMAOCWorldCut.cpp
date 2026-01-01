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

#include "gdeMAOCWorldCut.h"
#include "../../../gdeWindowMain.h"
#include "../../../../clipboard/gdeClipboardDataOCWorld.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../../gamedef/objectClass/world/gdeOCWorld.h"
#include "../../../../undosys/objectClass/world/gdeUOCRemoveWorld.h"

#include <deigde/clipboard/igdeClipboardData.h>
#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>


// Class gdeMAOCWorldCut
//////////////////////////

// Constructor
////////////////

gdeMAOCWorldCut::gdeMAOCWorldCut(gdeWindowMain &windowMain) :
gdeBaseMAOCSubObject(windowMain, "Cut Object Class World",
	windowMain.GetEnvironment().GetStockIcon(igdeEnvironment::esiCut), "Cut object class world"){
}


// Management
///////////////

igdeUndo::Ref gdeMAOCWorldCut::OnActionSubObject(
gdeGameDefinition &gameDefinition, gdeObjectClass &objectClass){
	if(gameDefinition.GetSelectedObjectType() != gdeGameDefinition::eotOCWorld){
		return {};
	}
	
	gdeOCWorld * const world = gameDefinition.GetActiveOCWorld();
	if(!world){
		return {};
	}
	
	pWindowMain.GetClipboard().Set(gdeClipboardDataOCWorld::Ref::New(gdeOCWorld::Ref::New(*world)));
	
	return gdeUOCRemoveWorld::Ref::New(&objectClass, world);
}

void gdeMAOCWorldCut::Update(){
	const gdeGameDefinition * const gameDefinition = pWindowMain.GetActiveGameDefinition();
	SetEnabled(gameDefinition
		&& gameDefinition->GetSelectedObjectType() == gdeGameDefinition::eotOCWorld
		&& gameDefinition->GetActiveOCWorld());
}
