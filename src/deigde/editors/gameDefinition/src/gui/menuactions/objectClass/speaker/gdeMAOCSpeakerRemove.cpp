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

#include "gdeMAOCSpeakerRemove.h"
#include "../../../gdeWindowMain.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../../gamedef/objectClass/speaker/gdeOCSpeaker.h"
#include "../../../../undosys/objectClass/speaker/gdeUOCRemoveSpeaker.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeMAOCSpeakerRemove
///////////////////////////////

// Constructor
////////////////

gdeMAOCSpeakerRemove::gdeMAOCSpeakerRemove(gdeWindowMain &windowMain) :
gdeBaseMAOCSubObject(windowMain, "Remove Object Class Speaker",
	windowMain.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
	"Remove object class speaker")
{
}



// Management
///////////////

igdeUndo *gdeMAOCSpeakerRemove::OnActionSubObject(
gdeGameDefinition &gameDefinition, gdeObjectClass &objectClass){
	if(gameDefinition.GetSelectedObjectType() != gdeGameDefinition::eotOCSpeaker){
		return NULL;
	}
	
	gdeOCSpeaker * const speaker = gameDefinition.GetActiveOCSpeaker();
	if(! speaker){
		return NULL;
	}
	
	return new gdeUOCRemoveSpeaker(&objectClass, speaker);
}

void gdeMAOCSpeakerRemove::Update(){
	const gdeGameDefinition * const gameDefinition = pWindowMain.GetActiveGameDefinition();
	SetEnabled(gameDefinition
		&& gameDefinition->GetSelectedObjectType() == gdeGameDefinition::eotOCSpeaker
		&& gameDefinition->GetActiveOCSpeaker() != NULL);
}
