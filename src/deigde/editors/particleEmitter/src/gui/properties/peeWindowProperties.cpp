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

#include "peeWindowProperties.h"
#include "peeWPType.h"
#include "peeWPController.h"
#include "peeWPView.h"
#include "peeWPUndoHistory.h"
#include "../peeWindowMain.h"
#include "../../emitter/peeEmitter.h"

#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/layout/igdeContainerBox.h>
#include <deigde/gui/theme/themeNames.h>

#include <dragengine/common/exceptions.h>



// Class peeWindowProperties
/////////////////////////////

// Constructor, destructor
////////////////////////////

peeWindowProperties::peeWindowProperties( peeWindowMain &windowMain ) :
igdeTabBook( windowMain.GetEnvironment() ),
pWindowMain( windowMain )
{
	SetWidgetGuiThemeName( igdeGuiThemeNames::properties );
	
	pPanelType.TakeOver( new peeWPType( *this ) );
	AddChild( pPanelType, "Type" );
	
	pPanelController.TakeOver( new peeWPController( *this ) );
	AddChild( pPanelController, "Controller" );
	
	pPanelView.TakeOver( new peeWPView( *this ) );
	AddChild( pPanelView, "View" );
	
	pPanelUndoHistory.TakeOver( new peeWPUndoHistory( GetEnvironment() ) );
	AddChild( pPanelUndoHistory, "Undo" );
	
	SetActivePanel( 0 ); // type
}

peeWindowProperties::~peeWindowProperties(){
}



// Management
///////////////

void peeWindowProperties::SetEmitter( peeEmitter *emitter ){
	( ( peeWPType& )( igdeWidget& )pPanelType ).SetEmitter( emitter );
	( ( peeWPController& )( igdeWidget& )pPanelController ).SetEmitter( emitter );
	( ( peeWPView& )( igdeWidget& )pPanelView ).SetEmitter( emitter );
	( ( peeWPUndoHistory& )( igdeWPUndoHistory& )pPanelUndoHistory ).SetEmitter( emitter );
}

void peeWindowProperties::OnEmitterPathChanged(){
	( ( peeWPType& )( igdeWidget& )pPanelType ).OnEmitterPathChanged();
}
