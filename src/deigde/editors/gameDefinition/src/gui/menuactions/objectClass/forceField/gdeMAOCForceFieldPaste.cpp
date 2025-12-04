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

#include "gdeMAOCForceFieldPaste.h"
#include "../../../gdeWindowMain.h"
#include "../../../../clipboard/gdeClipboardDataOCForceField.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../../gamedef/objectClass/forceField/gdeOCForceField.h"
#include "../../../../undosys/objectClass/forceField/gdeUOCAddForceField.h"

#include <deigde/clipboard/igdeClipboardData.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeMAOCForceFieldPaste
/////////////////////////////////

// Constructor
////////////////

gdeMAOCForceFieldPaste::gdeMAOCForceFieldPaste( gdeWindowMain &windowMain ) :
gdeBaseMAOCSubObject( windowMain, "Paste Object Class Force Field",
	windowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiPaste ),
	"Paste object class force field" )
{
}



// Management
///////////////

igdeUndo *gdeMAOCForceFieldPaste::OnActionSubObject( gdeGameDefinition&, gdeObjectClass &objectClass ){
	igdeClipboardData::Ref clip( pWindowMain.GetClipboard()
		.GetWithTypeName( gdeClipboardDataOCForceField::TYPE_NAME ) );
	if( ! clip ){
		return NULL;
	}
	
	const gdeClipboardDataOCForceField &clipOCForceField =
		( const gdeClipboardDataOCForceField & )( igdeClipboardData& )clip;
	
	deObjectReference forceField;
	forceField.TakeOver( new gdeOCForceField( *clipOCForceField.GetForceField() ) );
	
	igdeUndo * const undo = new gdeUOCAddForceField( &objectClass,
		( gdeOCForceField* )( deObject* )forceField );
	undo->SetShortInfo( "Paste object class particle emitter" );
	return undo;
}

void gdeMAOCForceFieldPaste::Update(){
	SetEnabled( GetActiveObjectClass() != NULL
		&& pWindowMain.GetClipboard().HasWithTypeName( gdeClipboardDataOCForceField::TYPE_NAME ) );
}
