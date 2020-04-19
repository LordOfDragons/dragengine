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

#include "ceActorParameter.h"
#include "ceActorParameterList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class ceActorParameter
///////////////////////////

// Constructor, destructor
////////////////////////////

ceActorParameterList::ceActorParameterList(){
}

ceActorParameterList::~ceActorParameterList(){
}



// Management
///////////////

int ceActorParameterList::GetCount() const{
	return pParameters.GetCount();
}

ceActorParameter *ceActorParameterList::GetAt( int index ) const{
	return ( ceActorParameter* )pParameters.GetAt( index );
}

ceActorParameter *ceActorParameterList::GetNamed( const char *name ) const{
	const int count = pParameters.GetCount();
	ceActorParameter *parameter;
	int i;
	
	for( i=0; i<count; i++ ){
		parameter = ( ceActorParameter* )pParameters.GetAt( i );
		
		if( parameter->GetName().Equals( name ) ){
			return parameter;
		}
	}
	
	return NULL;
}

int ceActorParameterList::IndexOf( ceActorParameter *parameter ) const{
	return pParameters.IndexOf( parameter );
}

int ceActorParameterList::IndexOfNamed( const char *name ) const{
	const int count = pParameters.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( ceActorParameter* )pParameters.GetAt( i ) )->GetName().Equals( name ) ){
			return i;
		}
	}
	
	return -1;
}

bool ceActorParameterList::Has( ceActorParameter *parameter ) const{
	return pParameters.Has( parameter );
}

bool ceActorParameterList::HasNamed( const char *name ) const{
	const int count = pParameters.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( ceActorParameter* )pParameters.GetAt( i ) )->GetName().Equals( name ) ){
			return true;
		}
	}
	
	return false;
}

void ceActorParameterList::Set( const char *name, int value ){
	ceActorParameter *parameter = GetNamed( name );
	
	if( parameter ){
		parameter->SetValue( value );
		
	}else{
		parameter = new ceActorParameter( name, value );
		
		pParameters.Add( parameter );
		parameter->FreeReference();
	}
}

void ceActorParameterList::Remove( ceActorParameter *parameter ){
	pParameters.Remove( parameter );
}

void ceActorParameterList::RemoveAll(){
	pParameters.RemoveAll();
}



ceActorParameterList &ceActorParameterList::operator=( const ceActorParameterList &list ){
	pParameters = list.pParameters;
	return *this;
}
