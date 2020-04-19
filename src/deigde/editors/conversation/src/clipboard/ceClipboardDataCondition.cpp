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

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "ceClipboardDataCondition.h"
#include "../conversation/condition/ceConversationCondition.h"

#include <dragengine/common/exceptions.h>



// Class ceClipboardDataCondition
////////////////////////////////

const char * const ceClipboardDataCondition::TYPE_NAME = "condition";

// Constructor, destructor
////////////////////////////

ceClipboardDataCondition::ceClipboardDataCondition( const ceConversationConditionList &conditions ) :
igdeClipboardData( TYPE_NAME )
{
	ceConversationCondition *condition = NULL;
	const int count = conditions.GetCount();
	int i;
	
	try{
		for( i=0; i<count; i++ ){
			condition = conditions.GetAt( i )->CreateCopy();
			pConditions.Add( condition );
			condition->FreeReference();
			condition = NULL;
		}
		
	}catch( const deException & ){
		if( condition ){
			condition->FreeReference();
		}
		throw;
	}
}

ceClipboardDataCondition::~ceClipboardDataCondition(){
	pConditions.RemoveAll();
}
