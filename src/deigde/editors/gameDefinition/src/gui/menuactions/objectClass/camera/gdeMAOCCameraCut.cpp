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

#include "gdeMAOCCameraCut.h"
#include "../../../gdeWindowMain.h"
#include "../../../../clipboard/gdeClipboardDataOCCamera.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../../gamedef/objectClass/camera/gdeOCCamera.h"
#include "../../../../undosys/objectClass/camera/gdeUOCRemoveCamera.h"

#include <deigde/clipboard/igdeClipboardData.h>
#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeMAOCCameraCut
///////////////////////////

// Constructor
////////////////

gdeMAOCCameraCut::gdeMAOCCameraCut( gdeWindowMain &windowMain ) :
gdeBaseMAOCSubObject( windowMain, "Cut Object Class Camera",
	windowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiCut ),
	"Cut object class camera" )
{
}



// Management
///////////////

igdeUndo *gdeMAOCCameraCut::OnActionSubObject(
gdeGameDefinition &gameDefinition, gdeObjectClass &objectClass ){
	if( gameDefinition.GetSelectedObjectType() != gdeGameDefinition::eotOCCamera ){
		return NULL;
	}
	
	gdeOCCamera * const camera = gameDefinition.GetActiveOCCamera();
	if( ! camera ){
		return NULL;
	}
	
	const gdeOCCamera::Ref clipOCCamera(gdeOCCamera::Ref::NewWith(*camera));
	
	gdeClipboardDataOCCamera::Ref clipData(gdeClipboardDataOCCamera::Ref::New(
		new gdeClipboardDataOCCamera( clipOCCamera ) ));
	
	pWindowMain.GetClipboard().Set( clipData );
	
	return new gdeUOCRemoveCamera( &objectClass, camera );
}

void gdeMAOCCameraCut::Update(){
	const gdeGameDefinition * const gameDefinition = pWindowMain.GetActiveGameDefinition();
	SetEnabled( gameDefinition
		&& gameDefinition->GetSelectedObjectType() == gdeGameDefinition::eotOCCamera
		&& gameDefinition->GetActiveOCCamera() != NULL );
}
