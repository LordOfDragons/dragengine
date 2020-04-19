/* 
 * Drag[en]gine OpenGL Graphic Module
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

#include "deoglParameter.h"
#include "deoglParameterList.h"

#include <dragengine/common/exceptions.h>



// class deoglParameterList
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglParameterList::deoglParameterList(){
}

deoglParameterList::~deoglParameterList(){
	RemoveAllParameters();
}



// Management
///////////////

int deoglParameterList::GetParameterCount() const{
	return pParameters.GetCount();
}

int deoglParameterList::IndexOfParameterNamed( const char *name ) const{
	const int count = pParameters.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( deoglParameter* )pParameters.GetAt( i ) )->GetName() == name ){
			return i;
		}
	}
	
	return -1;
}

deoglParameter &deoglParameterList::GetParameterAt( int index ) const{
	return *( ( deoglParameter* )pParameters.GetAt( index ) );
}

deoglParameter &deoglParameterList::GetParameterNamed( const char *name ) const{
	const int count = pParameters.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglParameter &parameter = *( ( deoglParameter* )pParameters.GetAt( i ) );
		if( parameter.GetName() == name ){
			return parameter;
		}
	}
	
	DETHROW( deeInvalidParam );
}

void deoglParameterList::AddParameter( deoglParameter *parameter ){
	if( ! parameter || IndexOfParameterNamed( parameter->GetName() ) != -1 ){
		DETHROW( deeInvalidParam );
	}
	pParameters.Add( parameter );
}

void deoglParameterList::RemoveAllParameters(){
	const int count = pParameters.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		delete ( deoglParameter* )pParameters.GetAt( i );
	}
	pParameters.RemoveAll();
}
