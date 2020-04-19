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

#include "ceWPUndoHistory.h"
#include "ceWPUndoHistoryListener.h"
#include "../../conversation/ceConversation.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class ceWPUndoHistory
//////////////////////////

// Constructor, destructor
////////////////////////////

ceWPUndoHistory::ceWPUndoHistory( igdeEnvironment &environment ) :
igdeWPUndoHistory( environment ),
pListener( NULL ),
pConversation( NULL )
{
	pListener = new ceWPUndoHistoryListener( *this );
}

ceWPUndoHistory::~ceWPUndoHistory(){
	SetConversation( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void ceWPUndoHistory::SetConversation( ceConversation *conversation ){
	if( conversation == pConversation ){
		return;
	}
	
	SetUndoSystem( NULL );
	
	if( pConversation ){
		pConversation->RemoveListener( pListener );
		pConversation->FreeReference();
	}
	
	pConversation = conversation;
	
	if( conversation ){
		conversation->AddListener( pListener );
		conversation->AddReference();
		
		SetUndoSystem( conversation->GetUndoSystem() );
	}
}
