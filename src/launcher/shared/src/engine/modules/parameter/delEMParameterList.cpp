/* 
 * Drag[en]gine Launcher Shared
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include "delEMParameter.h"
#include "delEMParameterList.h"

#include <dragengine/common/exceptions.h>


// Class delEMParameterList
/////////////////////////////

// Constructors and Destructors
/////////////////////////////////

delEMParameterList::delEMParameterList(){
}

delEMParameterList::~delEMParameterList(){
	RemoveAll();
}



// Management
///////////////

int delEMParameterList::GetCount() const{
	return pParameters.GetCount();
}

delEMParameter *delEMParameterList::GetAt( int index ) const{
	return ( delEMParameter* )pParameters.GetAt( index );
}

delEMParameter *delEMParameterList::GetNamed( const char *name ) const{
	if( ! name ){
		DETHROW_INFO( deeNullPointer, "name" );
	}
	
	const int count = pParameters.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		delEMParameter * const parameter = ( delEMParameter* )pParameters.GetAt( i );
		if( parameter->GetInfo().GetName().Equals( name ) ){
			return parameter;
		}
	}
	
	return nullptr;
}

bool delEMParameterList::Has( delEMParameter *parameter ) const{
	return pParameters.Has( parameter );
}

bool delEMParameterList::HasNamed( const char *name ) const{
	return GetNamed ( name );
}

int delEMParameterList::IndexOf( delEMParameter *parameter ) const{
	return pParameters.IndexOf( parameter );
}

int delEMParameterList::IndexOfNamed( const char *name ) const{
	if( ! name ){
		DETHROW_INFO( deeNullPointer, "name" );
	}
	
	const int count = pParameters.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( delEMParameter* )pParameters.GetAt( i ) )->GetInfo().GetName().Equals( name ) ){
			return i;
		}
	}
	
	return -1;
}

void delEMParameterList::Add( delEMParameter *parameter ){
	if( ! parameter ){
		DETHROW_INFO( deeNullPointer, "parameter" );
	}
	if( HasNamed ( parameter->GetInfo().GetName() ) ){
		DETHROW_INFO( deeInvalidParam, "named parameter is present" );
	}
	
	pParameters.Add( parameter );
}

void delEMParameterList::Remove( delEMParameter *parameter ){
	const int index = IndexOf ( parameter );
	if( index == -1 ){
		DETHROW_INFO( deeInvalidParam, "parameter is absent" );
	}
	
	pParameters.RemoveFrom( index );
}

void delEMParameterList::RemoveAll(){
	pParameters.RemoveAll();
}
