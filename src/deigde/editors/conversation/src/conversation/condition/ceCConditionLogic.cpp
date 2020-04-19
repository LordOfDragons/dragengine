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

#include "ceCConditionLogic.h"

#include <dragengine/common/exceptions.h>



// Class ceCConditionLogic
////////////////////////////

// Constructor, destructor
////////////////////////////

ceCConditionLogic::ceCConditionLogic() :
ceConversationCondition( ectLogic ),
pOperator( eopAll ),
pTIMExpanded( true ){
}

ceCConditionLogic::ceCConditionLogic( const ceCConditionLogic &condition ) :
ceConversationCondition( condition ),
pOperator( condition.GetOperator() ),
pTIMExpanded( condition.pTIMExpanded )
{
	const ceConversationConditionList &conditions = condition.GetConditions();
	ceConversationCondition *newCondition = NULL;
	int i, count;
	
	try{
		count = conditions.GetCount();
		for( i=0; i<count; i++ ){
			newCondition = conditions.GetAt( i )->CreateCopy();
			pConditions.Add( newCondition );
			newCondition->FreeReference();
			newCondition = NULL;
		}
		
	}catch( const deException & ){
		if( newCondition ){
			newCondition->FreeReference();
		}
		pConditions.RemoveAll();
		throw;
	}
}

ceCConditionLogic::~ceCConditionLogic(){
}



// Management
///////////////

void ceCConditionLogic::SetOperator( eOperators aOperator ){
	if( aOperator < eopNone || aOperator > eopAll ){
		DETHROW( deeInvalidParam );
	}
	
	pOperator = aOperator;
}



ceConversationCondition *ceCConditionLogic::CreateCopy() const{
	return new ceCConditionLogic( *this );
}



// UI
///////

void ceCConditionLogic::SetTIMExpanded( bool expanded ){
	pTIMExpanded = expanded;
}
