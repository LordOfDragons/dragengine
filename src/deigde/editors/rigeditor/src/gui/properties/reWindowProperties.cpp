/* 
 * Drag[en]gine IGDE Rig Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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

#include <deigde/gui/igdeContainerReference.h>
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
