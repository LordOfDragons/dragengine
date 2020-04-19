/* 
 * Drag[en]gine Android Launcher
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

#include "dealEMParameter.h"
#include "dealEMParameterList.h"

#include "../../../common/exceptions.h"



// Class dealEMParameterList
//////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dealEMParameterList::dealEMParameterList(){
}

dealEMParameterList::~dealEMParameterList(){
	RemoveAllParameters();
}



// Management
///////////////

int dealEMParameterList::GetParameterCount() const{
	return pParameters.GetCount();
}

dealEMParameter *dealEMParameterList::GetParameterAt( int index ) const{
	return ( dealEMParameter* )pParameters.GetAt( index );
}

dealEMParameter *dealEMParameterList::GetParameterNamed( const char *name ) const{
	if( ! name ) DETHROW( deeInvalidParam );
	
	int i, count = pParameters.GetCount();
	dealEMParameter *parameter;
	
	for( i=0; i<count; i++ ){
		parameter = ( dealEMParameter* )pParameters.GetAt( i );
		if( parameter->GetName().Equals( name ) ){
			return parameter;
		}
	}
	
	return NULL;
}

bool dealEMParameterList::HasParameter( dealEMParameter *parameter ) const{
	return pParameters.Has( parameter );
}

bool dealEMParameterList::HasParameterNamed( const char *name ) const{
	return GetParameterNamed( name ) != NULL;
}

int dealEMParameterList::IndexOfParameter( dealEMParameter *parameter ) const{
	return pParameters.IndexOf( parameter );
}

int dealEMParameterList::IndexOfParameterNamed( const char *name ) const{
	if( ! name ) DETHROW( deeInvalidParam );
	
	int i, count = pParameters.GetCount();
	
	for( i=0; i<count; i++ ){
		if( ( ( dealEMParameter* )pParameters.GetAt( i ) )->GetName().Equals( name ) ){
			return i;
		}
	}
	
	return -1;
}

void dealEMParameterList::AddParameter( dealEMParameter *parameter ){
	if( ! parameter || HasParameterNamed( parameter->GetName().GetString() ) ) DETHROW( deeInvalidParam );
	
	pParameters.Add( parameter );
}

void dealEMParameterList::RemoveParameter( dealEMParameter *parameter ){
	int index = IndexOfParameter( parameter );
	
	if( index == -1 ) DETHROW( deeInvalidParam );
	
	pParameters.RemoveFrom( index );
}

void dealEMParameterList::RemoveAllParameters(){
	pParameters.RemoveAll();
}
