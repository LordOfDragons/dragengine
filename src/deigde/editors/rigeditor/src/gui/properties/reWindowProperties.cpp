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
#include <string.h>
#include <stdlib.h>

#include "reWindowProperties.h"
#include "reWPBone.h"
#include "reWPConstraint.h"
#include "reWPPush.h"
#include "reWPRig.h"
#include "reWPShape.h"
#include "reWPUndoHistory.h"
#include "reWPView.h"
#include "../reWindowMain.h"

#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/layout/igdeContainerBox.h>
#include <deigde/gui/theme/themeNames.h>

#include <dragengine/common/exceptions.h>



// Class reWindowProperties
/////////////////////////////

// Constructor, destructor
////////////////////////////

reWindowProperties::reWindowProperties( reWindowMain &windowMain ) :
igdeTabBook( windowMain.GetEnvironment() ),
pWindowMain( windowMain )
{
	SetWidgetGuiThemeName( igdeGuiThemeNames::properties );
	
	pPanelRig.TakeOver( new reWPRig( *this ) );
	AddChild( pPanelRig, "Rig" );
	
	pPanelBone.TakeOver( new reWPBone( *this ) );
	AddChild( pPanelBone, "Bone" );
	
	pPanelShape.TakeOver( new reWPShape( *this ) );
	AddChild( pPanelShape, "Shape" );
	
	pPanelConstraint.TakeOver( new reWPConstraint( *this ) );
	AddChild( pPanelConstraint, "Constraint" );
	
	pPanelPush.TakeOver( new reWPPush( *this ) );
	AddChild( pPanelPush, "Push" );
	
	pPanelView.TakeOver( new reWPView( *this ) );
	AddChild( pPanelView, "View" );
	
	pPanelUndoHistory.TakeOver( new reWPUndoHistory( GetEnvironment() ) );
	AddChild( pPanelUndoHistory, "Undo" );
	
	SetActivePanel( 5 ); // view
}

reWindowProperties::~reWindowProperties(){
}



// Management
///////////////

void reWindowProperties::SetRig( reRig *rig ){
	( ( reWPRig& )( igdeWidget& )pPanelRig ).SetRig( rig );
	( ( reWPBone& )( igdeWidget& )pPanelBone ).SetRig( rig );
	( ( reWPShape& )( igdeWidget& )pPanelShape ).SetRig( rig );
	( ( reWPConstraint& )( igdeWidget& )pPanelConstraint ).SetRig( rig );
	( ( reWPPush& )( igdeWidget& )pPanelPush ).SetRig( rig );
	( ( reWPView& )( igdeWidget& )pPanelView ).SetRig( rig );
	( ( reWPUndoHistory& )( igdeWPUndoHistory& )pPanelUndoHistory ).SetRig( rig );
}
