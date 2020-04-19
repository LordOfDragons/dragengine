/* 
 * Drag[en]gine IGDE Skin Editor
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
#include <stdlib.h>
#include <string.h>

#include "seWindowProperties.h"
#include "seWPTexture.h"
#include "seWPNode.h"
#include "seWPView.h"
#include "seWPUndoHistory.h"
#include "dynamicskin/seWPDynamicSkin.h"
#include "../seWindowMain.h"
#include "../../skin/seSkin.h"

#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/layout/igdeContainerBox.h>
#include <deigde/gui/theme/themeNames.h>

#include <dragengine/common/exceptions.h>



// Class seWindowProperties
/////////////////////////////

// Constructor, destructor
////////////////////////////

seWindowProperties::seWindowProperties( seWindowMain &windowMain ) :
igdeTabBook( windowMain.GetEnvironment() ),
pWindowMain( windowMain )
{
	SetWidgetGuiThemeName( igdeGuiThemeNames::properties );
	
	pPanelTexture.TakeOver( new seWPTexture( *this ) );
	AddChild( pPanelTexture, "Texture" );
	
	pPanelNode.TakeOver( new seWPNode( *this ) );
	AddChild( pPanelNode, "Node" );
	
	pPanelDynamicSkin.TakeOver( new seWPDynamicSkin( *this ) );
	AddChild( pPanelDynamicSkin, "Dynamic Skin" );
	
	pPanelView.TakeOver( new seWPView( *this ) );
	AddChild( pPanelView, "View" );
	
	pPanelUndoHistory.TakeOver( new seWPUndoHistory( GetEnvironment() ) );
	AddChild( pPanelUndoHistory, "Undo" );
	
	SetActivePanel( 0 ); // texture
}

seWindowProperties::~seWindowProperties(){
}



// Management
///////////////

void seWindowProperties::SetSkin( seSkin *skin ){
	( ( seWPTexture& )( igdeWidget& )pPanelTexture ).SetSkin( skin );
	( ( seWPNode& )( igdeWidget& )pPanelNode ).SetSkin( skin );
	( ( seWPDynamicSkin& )( igdeWidget& )pPanelDynamicSkin ).SetSkin( skin );
	( ( seWPView& )( igdeWidget& )pPanelView ).SetSkin( skin );
	( ( seWPUndoHistory& )( igdeWidget& )pPanelUndoHistory ).SetSkin( skin );
}
