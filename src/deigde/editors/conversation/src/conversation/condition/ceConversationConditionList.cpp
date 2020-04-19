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

#include "ceConversationCondition.h"
#include "ceConversationConditionList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class ceConversationCondition
///////////////////////////////

// Constructor, destructor
////////////////////////////

ceConversationConditionList::ceConversationConditionList(){
}

ceConversationConditionList::~ceConversationConditionList(){
}



// Management
///////////////

int ceConversationConditionList::GetCount() const{
	return pConditions.GetCount();
}

ceConversationCondition *ceConversationConditionList::GetAt( int index ) const{
	return ( ceConversationCondition* )pConditions.GetAt( index );
}

int ceConversationConditionList::IndexOf( ceConversationCondition *condition ) const{
	return pConditions.IndexOf( condition );
}

bool ceConversationConditionList::Has( ceConversationCondition *condition ) const{
	return pConditions.Has( condition );
}

void ceConversationConditionList::Add( ceConversationCondition *condition ){
	if( ! condition ) DETHROW( deeInvalidParam );
	
	pConditions.Add( condition );
}

void ceConversationConditionList::InsertAt( ceConversationCondition *condition, int index ){
	if( ! condition ) DETHROW( deeInvalidParam );
	
	pConditions.Insert( condition, index );
}

void ceConversationConditionList::MoveTo( ceConversationCondition *condition, int index ){
	pConditions.Move( condition, index );
}

void ceConversationConditionList::Remove( ceConversationCondition *condition ){
	pConditions.Remove( condition );
}

void ceConversationConditionList::RemoveAll(){
	pConditions.RemoveAll();
}



ceConversationConditionList &ceConversationConditionList::operator=( const ceConversationConditionList &list ){
	pConditions = list.pConditions;
	return *this;
}
