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

#include "cePlaybackVariable.h"
#include "cePlaybackVariableList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class cePlaybackVariable
/////////////////////////////

// Constructor, destructor
////////////////////////////

cePlaybackVariableList::cePlaybackVariableList(){
}

cePlaybackVariableList::~cePlaybackVariableList(){
}



// Management
///////////////

int cePlaybackVariableList::GetCount() const{
	return pVariables.GetCount();
}

cePlaybackVariable *cePlaybackVariableList::GetAt( int index ) const{
	return ( cePlaybackVariable* )pVariables.GetAt( index );
}

cePlaybackVariable *cePlaybackVariableList::GetNamed( const char *name ) const{
	const int count = pVariables.GetCount();
	cePlaybackVariable *variable;
	int i;
	
	for( i=0; i<count; i++ ){
		variable = ( cePlaybackVariable* )pVariables.GetAt( i );
		
		if( variable->GetName().Equals( name ) ){
			return variable;
		}
	}
	
	return NULL;
}

int cePlaybackVariableList::IndexOf( cePlaybackVariable *variable ) const{
	return pVariables.IndexOf( variable );
}

int cePlaybackVariableList::IndexOfNamed( const char *name ) const{
	const int count = pVariables.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( cePlaybackVariable* )pVariables.GetAt( i ) )->GetName().Equals( name ) ){
			return i;
		}
	}
	
	return -1;
}

bool cePlaybackVariableList::Has( cePlaybackVariable *variable ) const{
	return pVariables.Has( variable );
}

bool cePlaybackVariableList::HasNamed( const char *name ) const{
	const int count = pVariables.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( cePlaybackVariable* )pVariables.GetAt( i ) )->GetName().Equals( name ) ){
			return true;
		}
	}
	
	return false;
}

void cePlaybackVariableList::Set( const char *name, int value ){
	cePlaybackVariable *variable = GetNamed( name );
	
	if( variable ){
		variable->SetValue( value );
		
	}else{
		variable = new cePlaybackVariable( name, value );
		
		pVariables.Add( variable );
		variable->FreeReference();
	}
}

void cePlaybackVariableList::Remove( cePlaybackVariable *variable ){
	pVariables.Remove( variable );
}

void cePlaybackVariableList::RemoveAll(){
	pVariables.RemoveAll();
}



cePlaybackVariableList &cePlaybackVariableList::operator=( const cePlaybackVariableList &list ){
	pVariables = list.pVariables;
	return *this;
}
