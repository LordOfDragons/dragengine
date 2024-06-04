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

#include "lpeWindowProperties.h"
#include "lpeWPUndoHistory.h"
#include "lpeWPLangPack.h"
#include "../lpeWindowMain.h"
#include "../../langpack/lpeLangPack.h"

#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/layout/igdeContainerBox.h>
#include <deigde/gui/theme/themeNames.h>

#include <dragengine/common/exceptions.h>



// Class lpeWindowProperties
//////////////////////////////

// Constructor, destructor
////////////////////////////

lpeWindowProperties::lpeWindowProperties( lpeWindowMain &windowMain ) :
igdeTabBook( windowMain.GetEnvironment() ),
pWindowMain( windowMain ),
pPanelLangPack( NULL )
{
	igdeEnvironment &env = GetEnvironment();
	SetWidgetGuiThemeName( igdeGuiThemeNames::properties );
	
	pPanelLangPack = new lpeWPLangPack( *this );
	AddChild( pPanelLangPack, "Language Pack" );
	
	pPanelUndoHistory.TakeOver( new lpeWPUndoHistory( env ) );
	AddChild( pPanelUndoHistory, "Undo" );
	
	SetActivePanel( 0 );
}

lpeWindowProperties::~lpeWindowProperties(){
}



// Management
///////////////

void lpeWindowProperties::SetLangPack( lpeLangPack *langpack ){
	pPanelLangPack->SetLangPack( langpack );
	( ( lpeWPUndoHistory* )pPanelUndoHistory.operator->() )->SetLangPack( langpack );
}
