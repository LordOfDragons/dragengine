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

#include "saeWindowProperties.h"
#include "saeWPSAnim.h"
#include "saeWPView.h"
#include "saeWPUndoHistory.h"
#include "../saeWindowMain.h"
#include "../../sanimation/saeSAnimation.h"

#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/layout/igdeContainerBox.h>
#include <deigde/gui/theme/themeNames.h>

#include <dragengine/common/exceptions.h>



// Class saeWindowProperties
//////////////////////////////

// Constructor, destructor
////////////////////////////

saeWindowProperties::saeWindowProperties( saeWindowMain &windowMain ) :
igdeTabBook( windowMain.GetEnvironment() ),
pWindowMain( windowMain )
{
	SetWidgetGuiThemeName( igdeGuiThemeNames::properties );
	
	pPanelSAnimation.TakeOver( new saeWPSAnim( *this ) );
	AddChild( pPanelSAnimation, "SAnimation" );
	
	pPanelView.TakeOver( new saeWPView( *this ) );
	AddChild( pPanelView, "View" );
	
	pPanelUndoHistory.TakeOver( new saeWPUndoHistory( GetEnvironment() ) );
	AddChild( pPanelUndoHistory, "Undo" );
	
	SetActivePanel( 0 ); // speech animation panel
}

saeWindowProperties::~saeWindowProperties(){
}



// Management
///////////////

void saeWindowProperties::SetSAnimation( saeSAnimation *sanimation ){
	( ( saeWPSAnim& )( igdeWidget& )pPanelSAnimation ).SetSAnimation( sanimation );
	( ( saeWPView& )( igdeWidget& )pPanelView ).SetSAnimation( sanimation );
	( ( saeWPUndoHistory& )( igdeWPUndoHistory& )pPanelUndoHistory ).SetSAnimation( sanimation );
}

void saeWindowProperties::OnSAnimationPathChanged(){
	( ( saeWPSAnim& )( igdeWidget& )pPanelSAnimation ).OnSAnimationPathChanged();
}
