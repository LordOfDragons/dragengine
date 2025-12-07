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

#include "gdeMAOCLightCut.h"
#include "../../../gdeWindowMain.h"
#include "../../../../clipboard/gdeClipboardDataOCLight.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../../gamedef/objectClass/light/gdeOCLight.h"
#include "../../../../undosys/objectClass/light/gdeUOCRemoveLight.h"

#include <deigde/clipboard/igdeClipboardData.h>
#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeMAOCLightCut
//////////////////////////

// Constructor
////////////////

gdeMAOCLightCut::gdeMAOCLightCut(gdeWindowMain &windowMain) :
gdeBaseMAOCSubObject(windowMain, "Cut Object Class Light",
	windowMain.GetEnvironment().GetStockIcon(igdeEnvironment::esiCut),
	"Cut object class light")
{
}



// Management
///////////////

igdeUndo *gdeMAOCLightCut::OnActionSubObject(
gdeGameDefinition &gameDefinition, gdeObjectClass &objectClass){
	if(gameDefinition.GetSelectedObjectType() != gdeGameDefinition::eotOCLight){
		return NULL;
	}
	
	gdeOCLight * const light = gameDefinition.GetActiveOCLight();
	if(! light){
		return NULL;
	}
	
	const gdeOCLight::Ref clipOCLight(gdeOCLight::Ref::NewWith(*light));
	
	pWindowMain.GetClipboard().Set(gdeClipboardDataOCLight::Ref::NewWith(clipOCLight));
	
	return new gdeUOCRemoveLight(&objectClass, light);
}

void gdeMAOCLightCut::Update(){
	const gdeGameDefinition * const gameDefinition = pWindowMain.GetActiveGameDefinition();
	SetEnabled(gameDefinition
		&& gameDefinition->GetSelectedObjectType() == gdeGameDefinition::eotOCLight
		&& gameDefinition->GetActiveOCLight() != NULL);
}
