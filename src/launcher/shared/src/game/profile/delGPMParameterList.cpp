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

#include "delGPMParameter.h"
#include "delGPMParameterList.h"

#include <dragengine/common/exceptions.h>



// Class delGPMParameterList
///////////////////////////////

// Constructors and Destructors
/////////////////////////////////

delGPMParameterList::delGPMParameterList(){
}

delGPMParameterList::~delGPMParameterList(){
	RemoveAllParameters();
}



// Management
///////////////

int delGPMParameterList::GetParameterCount() const{
	return pParameters.GetCount();
}

delGPMParameter *delGPMParameterList::GetParameterAt( int index ) const{
	return ( delGPMParameter* )pParameters.GetAt( index );
}

delGPMParameter *delGPMParameterList::GetParameterNamed( const char *name ) const{
	if( ! name ){
		DETHROW_INFO( deeNullPointer, "name" );
	}
	
	const int count = pParameters.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		delGPMParameter * const parameter = ( delGPMParameter* )pParameters.GetAt( i );
		if( parameter->GetName() == name ){
			return parameter;
		}
	}
	
	return nullptr;
}

bool delGPMParameterList::HasParameter( delGPMParameter *parameter ) const{
	return pParameters.Has( parameter );
}

bool delGPMParameterList::HasParameterNamed( const char *name ) const{
	return GetParameterNamed( name ) != NULL;
}

int delGPMParameterList::IndexOfParameter( delGPMParameter *parameter ) const{
	return pParameters.IndexOf( parameter );
}

int delGPMParameterList::IndexOfParameterNamed( const char *name ) const{
	if( ! name ){
		DETHROW_INFO( deeNullPointer, "name" );
	}
	
	const int count = pParameters.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( delGPMParameter* )pParameters.GetAt( i ) )->GetName() == name ){
			return i;
		}
	}
	
	return -1;
}

void delGPMParameterList::AddParameter( delGPMParameter *parameter ){
	if( ! parameter ){
		DETHROW_INFO( deeNullPointer, "parameter" );
	}
	if( HasParameterNamed( parameter->GetName() ) ){
		DETHROW_INFO( deeInvalidParam, "parameter is present" );
	}
	
	pParameters.Add( parameter );
}

void delGPMParameterList::RemoveParameter( delGPMParameter *parameter ){
	const int index = IndexOfParameter( parameter );
	if( index == -1 ){
		DETHROW_INFO( deeInvalidParam, "parameter is absent" );
	}
	
	pParameters.RemoveFrom( index );
}

void delGPMParameterList::RemoveAllParameters(){
	pParameters.RemoveAll();
}
