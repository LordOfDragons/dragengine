/* 
 * Drag[en]gine Bullet Physics Module
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

#include "debpParameter.h"
#include "debpParameterList.h"

#include <dragengine/common/exceptions.h>



// class debpParameterList
/////////////////////////////

// Constructor, destructor
////////////////////////////

debpParameterList::debpParameterList(){
}

debpParameterList::~debpParameterList(){
	RemoveAllParameters();
}



// Management
///////////////

int debpParameterList::GetParameterCount() const{
	return pParameters.GetCount();
}

int debpParameterList::IndexOfParameterNamed( const char *name ) const{
	const int count = pParameters.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( debpParameter* )pParameters.GetAt( i ) )->GetName() == name ){
			return i;
		}
	}
	
	return -1;
}

debpParameter &debpParameterList::GetParameterAt( int index ) const{
	return *( ( debpParameter* )pParameters.GetAt( index ) );
}

debpParameter &debpParameterList::GetParameterNamed( const char *name ) const{
	const int count = pParameters.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		debpParameter &parameter = *( ( debpParameter* )pParameters.GetAt( i ) );
		if( parameter.GetName() == name ){
			return parameter;
		}
	}
	
	DETHROW( deeInvalidParam );
}

void debpParameterList::AddParameter( debpParameter *parameter ){
	if( ! parameter || IndexOfParameterNamed( parameter->GetName() ) != -1 ){
		DETHROW( deeInvalidParam );
	}
	pParameters.Add( parameter );
}

void debpParameterList::RemoveAllParameters(){
	const int count = pParameters.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		delete ( debpParameter* )pParameters.GetAt( i );
	}
	pParameters.RemoveAll();
}
