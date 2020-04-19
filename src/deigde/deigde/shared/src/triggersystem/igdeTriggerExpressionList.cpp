/* 
 * Drag[en]gine IGDE
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

#include "igdeTriggerExpression.h"
#include "igdeTriggerExpressionList.h"

#include <dragengine/common/exceptions.h>



// Class igdeTriggerExpressionList
////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeTriggerExpressionList::igdeTriggerExpressionList(){
}

igdeTriggerExpressionList::~igdeTriggerExpressionList(){
}



// Management
///////////////

int igdeTriggerExpressionList::GetCount() const{
	return pExpressions.GetCount();
}

igdeTriggerExpression *igdeTriggerExpressionList::GetAt( int position ) const{
	return ( igdeTriggerExpression* )pExpressions.GetAt( position );
}

int igdeTriggerExpressionList::IndexOf( igdeTriggerExpression *expression ) const{
	return pExpressions.IndexOf( expression );
}

bool igdeTriggerExpressionList::Has( igdeTriggerExpression *expression ) const{
	return pExpressions.Has( expression );
}

void igdeTriggerExpressionList::Add( igdeTriggerExpression *expression ){
	if( ! expression ){
		DETHROW( deeInvalidParam );
	}
	
	pExpressions.Add( expression );
}

void igdeTriggerExpressionList::AddIfAbsent( igdeTriggerExpression *expression ){
	if( ! expression ){
		DETHROW( deeInvalidParam );
	}
	
	pExpressions.AddIfAbsent( expression );
}

void igdeTriggerExpressionList::Remove( igdeTriggerExpression *expression ){
	if( ! expression ){
		DETHROW( deeInvalidParam );
	}
	
	pExpressions.Remove( expression );
}

void igdeTriggerExpressionList::RemoveIfPresent( igdeTriggerExpression *expression ){
	if( ! expression ){
		DETHROW( deeInvalidParam );
	}
	
	pExpressions.RemoveIfPresent( expression );
}

void igdeTriggerExpressionList::RemoveAll(){
	pExpressions.RemoveAll();
}
