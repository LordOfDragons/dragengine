/* 
 * Drag[en]gine OpenAL Audio Module
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

#include "deoalParameter.h"
#include "deoalParameterList.h"

#include <dragengine/common/exceptions.h>



// class deoalParameterList
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoalParameterList::deoalParameterList(){
}

deoalParameterList::~deoalParameterList(){
	RemoveAllParameters();
}



// Management
///////////////

int deoalParameterList::GetParameterCount() const{
	return pParameters.GetCount();
}

int deoalParameterList::IndexOfParameterNamed( const char *name ) const{
	const int count = pParameters.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( deoalParameter* )pParameters.GetAt( i ) )->GetName() == name ){
			return i;
		}
	}
	
	return -1;
}

deoalParameter &deoalParameterList::GetParameterAt( int index ) const{
	return *( ( deoalParameter* )pParameters.GetAt( index ) );
}

deoalParameter &deoalParameterList::GetParameterNamed( const char *name ) const{
	const int count = pParameters.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoalParameter &parameter = *( ( deoalParameter* )pParameters.GetAt( i ) );
		if( parameter.GetName() == name ){
			return parameter;
		}
	}
	
	DETHROW( deeInvalidParam );
}

void deoalParameterList::AddParameter( deoalParameter *parameter ){
	if( ! parameter || IndexOfParameterNamed( parameter->GetName() ) != -1 ){
		DETHROW( deeInvalidParam );
	}
	pParameters.Add( parameter );
}

void deoalParameterList::RemoveAllParameters(){
	const int count = pParameters.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		delete ( deoalParameter* )pParameters.GetAt( i );
	}
	pParameters.RemoveAll();
}
