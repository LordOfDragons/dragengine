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

#include "gdeMAObjectClassExportXmlEClass.h"
#include "../../gdeWindowMain.h"
#include "../../../clipboard/gdeClipboardDataObjectClass.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../loadsave/gdeLoadSaveSystem.h"

#include <deigde/clipboard/igdeClipboardDataReference.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/filedialog/igdeFilePattern.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>



// Class gdeMAObjectClassExportXmlEClass
//////////////////////////////////////////

decString gdeMAObjectClassExportXmlEClass::pLastDirExportXmlEClass;

// Constructor
////////////////

gdeMAObjectClassExportXmlEClass::gdeMAObjectClassExportXmlEClass( gdeWindowMain &windowMain ) :
gdeBaseAction( windowMain, "Object Class Export XML EClass",
	windowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiSaveAs ),
	"Export object class as XML EClass (*.deeclass)" )
{
}



// Management
///////////////

igdeUndo *gdeMAObjectClassExportXmlEClass::OnAction( gdeGameDefinition &gameDefinition ){
	gdeObjectClass * const category = gameDefinition.GetActiveObjectClass();
	if( ! category || gameDefinition.GetSelectedObjectType() != gdeGameDefinition::eotObjectClass ){
		return NULL;
	}
	
	const gdeObjectClass * const objectClass = gameDefinition.GetActiveObjectClass();
	if( ! objectClass ){
		return NULL;
	}
	
	if( pLastDirExportXmlEClass.IsEmpty() ){
		pLastDirExportXmlEClass = gameDefinition.GetBasePath();
	}
	
	decPath path( decPath::CreatePathNative( pLastDirExportXmlEClass ) );
	path.AddComponent( objectClass->GetName() );
	
	decString filename( path.GetPathNative() );
	
	if( ! igdeCommonDialogs::GetFileSave( &pWindowMain, "Export XML Element Class",
	pWindowMain.GetLoadSaveSystem().GetXmlEClassFilePatterns(), filename ) ){
		return NULL;
	}
	
	path.SetFromNative( filename );
	path.RemoveLastComponent();
	pLastDirExportXmlEClass = path.GetPathNative();
	
	pWindowMain.GetLoadSaveSystem().SaveXmlEClass( gameDefinition, *objectClass, filename );
	return NULL;
}

void gdeMAObjectClassExportXmlEClass::Update(){
	gdeGameDefinition * const gameDefinition = pWindowMain.GetActiveGameDefinition();
	if( ! gameDefinition ){
		SetEnabled( false );
		return;
	}
	
	SetEnabled( gameDefinition->GetSelectedObjectType() == gdeGameDefinition::eotObjectClass 
		&& gameDefinition->GetActiveObjectClass() != NULL );
}
