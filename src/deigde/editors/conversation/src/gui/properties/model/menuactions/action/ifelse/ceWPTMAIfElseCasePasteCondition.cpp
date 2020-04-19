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

#include "ceWPTMAIfElseCasePasteCondition.h"
#include "../../../../../../conversation/action/ceCAIfElseCase.h"
#include "../../../../../../undosys/action/ifElse/ceUCAIfElseCasePasteCondition.h"

#include <dragengine/common/exceptions.h>

#include <deigde/undo/igdeUndoSystem.h>



// Constructor, destructor
////////////////////////////

ceWPTMAIfElseCasePasteCondition::ceWPTMAIfElseCasePasteCondition( ceWindowMain &windowMain,
ceConversation &conversation, ceConversationTopic &topic,
ceCAIfElse &ifElse, ceCAIfElseCase &ifCase ) :
ceWPTMAPasteCondition( windowMain, conversation ),
pTopic( &topic ),
pIfElse( &ifElse ),
pIfCase( &ifCase ){
}



// Management
///////////////

igdeUndo *ceWPTMAIfElseCasePasteCondition::CreateUndo( const ceConversationConditionList &conditions ){
	if( conditions.GetCount() != 1 ){
		//DETHROW( deeInvalidParam );
	}
	
	return new ceUCAIfElseCasePasteCondition( pTopic, pIfElse, pIfCase, conditions.GetAt( 0 ) );
}
