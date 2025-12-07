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

#include "gdeMAOCNavSpaceCut.h"
#include "../../../gdeWindowMain.h"
#include "../../../../clipboard/gdeClipboardDataOCNavSpace.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../../gamedef/objectClass/navspace/gdeOCNavigationSpace.h"
#include "../../../../undosys/objectClass/navspace/gdeUOCRemoveNavSpace.h"

#include <deigde/clipboard/igdeClipboardData.h>
#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeMAOCNavSpaceCut
/////////////////////////////

// Constructor
////////////////

gdeMAOCNavSpaceCut::gdeMAOCNavSpaceCut(gdeWindowMain &windowMain) :
gdeBaseMAOCSubObject(windowMain, "Cut Object Class Navigation Space",
	windowMain.GetEnvironment().GetStockIcon(igdeEnvironment::esiCut),
	"Cut object class navigation space")
{
}



// Management
///////////////

igdeUndo *gdeMAOCNavSpaceCut::OnActionSubObject(
gdeGameDefinition &gameDefinition, gdeObjectClass &objectClass){
	if(gameDefinition.GetSelectedObjectType() != gdeGameDefinition::eotOCNavigationSpace){
		return NULL;
	}
	
	gdeOCNavigationSpace * const navSpace = gameDefinition.GetActiveOCNavigationSpace();
	if(! navSpace){
		return NULL;
	}
	
	const gdeOCNavigationSpace::Ref clipOCNavigationSpace(gdeOCNavigationSpace::Ref::NewWith(*navSpace));
	
	pWindowMain.GetClipboard().Set(gdeClipboardDataOCNavSpace::Ref::NewWith(clipOCNavigationSpace));
	
	return new gdeUOCRemoveNavSpace(&objectClass, navSpace);
}

void gdeMAOCNavSpaceCut::Update(){
	const gdeGameDefinition * const gameDefinition = pWindowMain.GetActiveGameDefinition();
	SetEnabled(gameDefinition
		&& gameDefinition->GetSelectedObjectType() == gdeGameDefinition::eotOCNavigationSpace
		&& gameDefinition->GetActiveOCNavigationSpace() != NULL);
}
