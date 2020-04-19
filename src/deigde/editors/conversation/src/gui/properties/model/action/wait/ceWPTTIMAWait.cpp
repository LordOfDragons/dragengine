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

#include "ceWPTTIMAWait.h"
#include "ceWPTTIMAWaitCondition.h"
#include "ceWPTTIMAWaitActions.h"
#include "../../ceWPTTreeItem.h"
#include "../../../../ceWindowMain.h"
#include "../../../../../conversation/ceConversation.h"
#include "../../../../../conversation/action/ceCAWait.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTTIMAWait::ceWPTTIMAWait( ceWindowMain &windowMain,
ceConversation &conversation, ceCAWait *action ) :
ceWPTTIMAction( windowMain, etActionWait, conversation, action ),
pCondition( NULL ),
pActions( NULL )
{
	SetIcon( windowMain.GetIconActionWait() );
	SetText( "Wait" );
	
	try{
		pCondition = new ceWPTTIMAWaitCondition( windowMain, conversation, action );
		AddChild( pCondition );
		
		pActions = new ceWPTTIMAWaitActions( windowMain, conversation, action->GetActions() );
		AddChild( pActions );
		
	}catch( const deException & ){
		if( pActions ){
			pActions->FreeReference();
		}
		if( pCondition ){
			pCondition->FreeReference();
		}
		throw;
	}
	
	pCondition->FreeReference(); // held by super-class child list
	pActions->FreeReference(); // held by super-class child list
}

ceWPTTIMAWait::~ceWPTTIMAWait(){
}



// Management
///////////////

void ceWPTTIMAWait::Update(){
	const ceCAWait &wait = *GetActionWait();
	SetExpanded( wait.GetTIMExpanded() );
	
	pCondition->Update();
	pActions->Update();
}

void ceWPTTIMAWait::UpdateActionLists(){
	pActions->UpdateActions();
}

void ceWPTTIMAWait::OnExpandedChanged(){
	GetActionWait()->SetTIMExpanded( GetExpanded() );
}
