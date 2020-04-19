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

#include "ceUndoCAction.h"
#include "ceUndoCActionList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class ceUndoCAction
////////////////////////

// Constructor, destructor
////////////////////////////

ceUndoCActionList::ceUndoCActionList(){
}

ceUndoCActionList::~ceUndoCActionList(){
}



// Management
///////////////

int ceUndoCActionList::GetCount() const{
	return pActions.GetCount();
}

ceUndoCAction *ceUndoCActionList::GetAt( int index ) const{
	return ( ceUndoCAction* )pActions.GetAt( index );
}

int ceUndoCActionList::IndexOf( ceUndoCAction *action ) const{
	return pActions.IndexOf( action );
}

bool ceUndoCActionList::Has( ceUndoCAction *action ) const{
	return pActions.Has( action );
}

void ceUndoCActionList::Add( ceUndoCAction *action ){
	if( ! action ) DETHROW( deeInvalidParam );
	
	pActions.Add( action );
}

void ceUndoCActionList::Remove( ceUndoCAction *action ){
	pActions.Remove( action );
}

void ceUndoCActionList::RemoveAll(){
	pActions.RemoveAll();
}



ceUndoCActionList &ceUndoCActionList::operator=( const ceUndoCActionList &list ){
	pActions = list.pActions;
	return *this;
}
