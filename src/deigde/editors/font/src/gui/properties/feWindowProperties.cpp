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

#include "feWindowProperties.h"
#include "feWPFont.h"
#include "feWPGlyph.h"
#include "feWPUndoHistory.h"
#include "../feWindowMain.h"
#include "../../font/feFont.h"

#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/layout/igdeContainerBox.h>
#include <deigde/gui/theme/themeNames.h>

#include <dragengine/common/exceptions.h>



// Class feWindowProperties
/////////////////////////////

// Constructor, destructor
////////////////////////////

feWindowProperties::feWindowProperties( feWindowMain &windowMain ) :
igdeTabBook( windowMain.GetEnvironment() ),
pWindowMain( windowMain )
{
	igdeEnvironment &env = GetEnvironment();
	SetWidgetGuiThemeName( igdeGuiThemeNames::properties );
	
	pPanelFont.TakeOver( new feWPFont( *this ) );
	AddChild( pPanelFont, "Font" );
	
	pPanelGlyph.TakeOver( new feWPGlyph( *this ) );
	AddChild( pPanelGlyph, "Glyph" );
	
	pPanelUndoHistory.TakeOver( new feWPUndoHistory( env ) );
	AddChild( pPanelUndoHistory, "Undo" );
	
	SetActivePanel( 0 );
}

feWindowProperties::~feWindowProperties(){
}



// Management
///////////////

void feWindowProperties::SetFont( feFont *font ){
	( ( feWPFont& )( igdeWidget& )pPanelFont ).SetFont( font );
	( ( feWPGlyph& )( igdeWidget& )pPanelGlyph ).SetFont( font );
	( ( feWPUndoHistory* )pPanelUndoHistory.operator->() )->SetFont( font );
}
