/* 
 * Drag[en]gine IGDE Conversation Editor
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

#include "ceWindowProperties.h"
#include "ceWPConversation.h"
#include "ceWPTopic.h"
#include "ceWPView.h"
#include "ceWPUndoHistory.h"
#include "../ceWindowMain.h"
#include "../../conversation/ceConversation.h"

#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/igdeWidgetReference.h>
#include <deigde/gui/layout/igdeContainerBox.h>
#include <deigde/gui/theme/themeNames.h>

#include <dragengine/common/exceptions.h>



// Class ceWindowProperties
/////////////////////////////

// Constructor, destructor
////////////////////////////

ceWindowProperties::ceWindowProperties( ceWindowMain &windowMain ) :
igdeTabBook( windowMain.GetEnvironment() ),
pWindowMain( windowMain )
{
	igdeWidgetReference panel;
	
	SetWidgetGuiThemeName( igdeGuiThemeNames::properties );
	
	panel.TakeOver( pPanelConversation = new ceWPConversation( *this ) );
	AddChild( panel, "Conversation" );
	
	panel.TakeOver( pPanelTopic = new ceWPTopic( *this ) );
	AddChild( panel, "Topic" );
	
	panel.TakeOver( pPanelView = new ceWPView( *this ) );
	AddChild( panel, "View" );
	
	panel.TakeOver( pPanelUndoHistory = new ceWPUndoHistory( GetEnvironment() ) );
	AddChild( panel, "Undo" );
	
	SetActivePanel( 0 ); // conversation
}

ceWindowProperties::~ceWindowProperties(){
}



// Management
///////////////

ceWPTopic &ceWindowProperties::GetPanelTopic() const{
	return *pPanelTopic;
}

void ceWindowProperties::SetConversation( ceConversation *conversation ){
	pPanelConversation->SetConversation( conversation );
	pPanelTopic->SetConversation( conversation );
	pPanelView->SetConversation( conversation );
	pPanelUndoHistory->SetConversation( conversation );
}

void ceWindowProperties::OnConversationPathChanged(){
	pPanelConversation->OnConversationPathChanged();
	pPanelTopic->OnConversationPathChanged();
}
