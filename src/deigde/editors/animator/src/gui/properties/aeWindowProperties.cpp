/* 
 * Drag[en]gine IGDE Animator Editor
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

#include "aeWindowProperties.h"
#include "aeWPAnimator.h"
#include "aeWPController.h"
#include "aeWPLink.h"
#include "aeWPRule.h"
#include "aeWPView.h"
#include "aeWPPlayground.h"
#include "aeWPUndoHistory.h"
#include "../aeWindowMain.h"

#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/layout/igdeContainerBox.h>
#include <deigde/gui/theme/themeNames.h>

#include <dragengine/common/exceptions.h>


// Class aeWindowProperties
/////////////////////////////

// Constructor, destructor
////////////////////////////

aeWindowProperties::aeWindowProperties( aeWindowMain &windowMain ) :
igdeTabBook( windowMain.GetEnvironment() ),
pWindowMain( windowMain ),

pPropAnimator( NULL ),
pPropRule( NULL ),
pPropLink( NULL ),
pPropController( NULL ),
pPropView( NULL ),
pPropPlayground( NULL )
{
	SetWidgetGuiThemeName( igdeGuiThemeNames::properties );
	
	pPropAnimator = new aeWPAnimator( *this );
	AddChild( pPropAnimator, "Animator" );
	
	pPropController = new aeWPController( *this );
	AddChild( pPropController, "Controller" );
	
	pPropLink = new aeWPLink( *this );
	AddChild( pPropLink, "Link" );
	
	pPropRule = new aeWPRule( *this );
	AddChild( pPropRule, "Rule" );
	
	pPropPlayground = new aeWPPlayground( *this );
	AddChild( pPropPlayground, "Playground" );
	
	pPropView = new aeWPView( *this );
	AddChild( pPropView, "View" );
	
	pPanelUndoHistory.TakeOver( new aeWPUndoHistory( GetEnvironment() ) );
	AddChild( pPanelUndoHistory, "History" );
	
	SetActivePanel( 1 ); // controller
}

aeWindowProperties::~aeWindowProperties(){
}



// Management
///////////////

void aeWindowProperties::SetAnimator( aeAnimator *animator ){
	pPropAnimator->SetAnimator( animator );
	pPropController->SetAnimator( animator );
	pPropLink->SetAnimator( animator );
	pPropRule->SetAnimator( animator );
	pPropView->SetAnimator( animator );
	pPropPlayground->SetAnimator( animator );
	( ( aeWPUndoHistory& )( igdeWPUndoHistory& )pPanelUndoHistory ).SetAnimator( animator );
}

void aeWindowProperties::OnAnimatorPathChanged(){
	pPropRule->OnAnimatorPathChanged();
}
