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

#include "gdeMAOCSnapPointCopy.h"
#include "../../../gdeWindowMain.h"
#include "../../../../clipboard/gdeClipboardDataOCSnapPoint.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../../gamedef/objectClass/snappoint/gdeOCSnapPoint.h"

#include <deigde/clipboard/igdeClipboardData.h>
#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeMAOCSnapPointCopy
///////////////////////////////

// Constructor
////////////////

gdeMAOCSnapPointCopy::gdeMAOCSnapPointCopy( gdeWindowMain &windowMain ) :
gdeBaseMAOCSubObject( windowMain, "Copy Object Class Snap Point",
	windowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiCopy ),
	"Copy object class snap point" )
{
}



// Management
///////////////

igdeUndo *gdeMAOCSnapPointCopy::OnActionSubObject( gdeGameDefinition &gameDefinition, gdeObjectClass & ){
	if( gameDefinition.GetSelectedObjectType() != gdeGameDefinition::eotOCSnapPoint ){
		return NULL;
	}
	
	gdeOCSnapPoint * const snapPoint = gameDefinition.GetActiveOCSnapPoint();
	if( ! snapPoint ){
		return NULL;
	}
	
	deObjectReference clipOCSnapPoint;
	clipOCSnapPoint.TakeOver( new gdeOCSnapPoint( *snapPoint ) );
	
	igdeClipboardData::Ref clipData;
	clipData.TakeOver( new gdeClipboardDataOCSnapPoint( ( gdeOCSnapPoint* )( deObject* )clipOCSnapPoint ) );
	
	pWindowMain.GetClipboard().Set( clipData );
	return NULL;
}

void gdeMAOCSnapPointCopy::Update(){
	const gdeGameDefinition * const gameDefinition = pWindowMain.GetActiveGameDefinition();
	SetEnabled( gameDefinition
		&& gameDefinition->GetSelectedObjectType() == gdeGameDefinition::eotOCSnapPoint
		&& gameDefinition->GetActiveOCSnapPoint() != NULL );
}
