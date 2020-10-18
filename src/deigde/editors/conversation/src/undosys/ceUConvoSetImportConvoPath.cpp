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
#include <string.h>
#include <stdlib.h>

#include "ceUConvoSetImportConvoPath.h"
#include "../conversation/ceConversation.h"

#include <dragengine/common/exceptions.h>



// Class ceUConvoSetImportConvoPath
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

ceUConvoSetImportConvoPath::ceUConvoSetImportConvoPath( ceLoadSaveSystem &lssystem,
	ceConversation *conversation, const decStringList &newValue ) :
pLSSystem( lssystem ),
pConversation( conversation ),
pNewValue( newValue )
{
	if( ! conversation ){
		DETHROW( deeInvalidParam );
	}
	
	pOldValue = conversation->GetImportConversationPath();
	
	SetShortInfo( "Set Import Conversation Path List" );
}

ceUConvoSetImportConvoPath::~ceUConvoSetImportConvoPath(){
}



// Management
///////////////

void ceUConvoSetImportConvoPath::Undo(){
	pConversation->SetImportConversationPath( pOldValue );
	pConversation->UpdateImportedConversations( pLSSystem );
	pConversation->NotifyConversationChanged();
}

void ceUConvoSetImportConvoPath::Redo(){
	pConversation->SetImportConversationPath( pNewValue );
	pConversation->UpdateImportedConversations( pLSSystem );
	pConversation->NotifyConversationChanged();
}
