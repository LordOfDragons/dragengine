/* 
 * Drag[en]gine Synthesizer Module
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

#include "desynParameter.h"
#include "desynParameterList.h"

#include <dragengine/common/exceptions.h>



// class desynParameterList
/////////////////////////////

// Constructor, destructor
////////////////////////////

desynParameterList::desynParameterList(){
}

desynParameterList::~desynParameterList(){
	RemoveAllParameters();
}



// Management
///////////////

int desynParameterList::GetParameterCount() const{
	return pParameters.GetCount();
}

int desynParameterList::IndexOfParameterNamed( const char *name ) const{
	const int count = pParameters.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( desynParameter* )pParameters.GetAt( i ) )->GetName() == name ){
			return i;
		}
	}
	
	return -1;
}

desynParameter &desynParameterList::GetParameterAt( int index ) const{
	return *( ( desynParameter* )pParameters.GetAt( index ) );
}

desynParameter &desynParameterList::GetParameterNamed( const char *name ) const{
	const int count = pParameters.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		desynParameter &parameter = *( ( desynParameter* )pParameters.GetAt( i ) );
		if( parameter.GetName() == name ){
			return parameter;
		}
	}
	
	DETHROW( deeInvalidParam );
}

void desynParameterList::AddParameter( desynParameter *parameter ){
	if( ! parameter || IndexOfParameterNamed( parameter->GetName() ) != -1 ){
		DETHROW( deeInvalidParam );
	}
	pParameters.Add( parameter );
}

void desynParameterList::RemoveAllParameters(){
	const int count = pParameters.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		delete ( desynParameter* )pParameters.GetAt( i );
	}
	pParameters.RemoveAll();
}
