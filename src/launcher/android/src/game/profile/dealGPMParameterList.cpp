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

#include "dealGPMParameter.h"
#include "dealGPMParameterList.h"

#include "../../common/exceptions.h"



// Class dealGPMParameterList
///////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dealGPMParameterList::dealGPMParameterList(){
}

dealGPMParameterList::~dealGPMParameterList(){
	RemoveAllParameters();
}



// Management
///////////////

int dealGPMParameterList::GetParameterCount() const{
	return pParameters.GetCount();
}

dealGPMParameter *dealGPMParameterList::GetParameterAt( int index ) const{
	return ( dealGPMParameter* )pParameters.GetAt( index );
}

dealGPMParameter *dealGPMParameterList::GetParameterNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	int i, count = pParameters.GetCount();
	dealGPMParameter *parameter;
	
	for( i=0; i<count; i++ ){
		parameter = ( dealGPMParameter* )pParameters.GetAt( i );
		if( parameter->GetName().Equals( name ) ){
			return parameter;
		}
	}
	
	return NULL;
}

bool dealGPMParameterList::HasParameter( dealGPMParameter *parameter ) const{
	return pParameters.Has( parameter );
}

bool dealGPMParameterList::HasParameterNamed( const char *name ) const{
	return GetParameterNamed( name ) != NULL;
}

int dealGPMParameterList::IndexOfParameter( dealGPMParameter *parameter ) const{
	return pParameters.IndexOf( parameter );
}

int dealGPMParameterList::IndexOfParameterNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	int i, count = pParameters.GetCount();
	
	for( i=0; i<count; i++ ){
		if( ( ( dealGPMParameter* )pParameters.GetAt( i ) )->GetName().Equals( name ) ){
			return i;
		}
	}
	
	return -1;
}

void dealGPMParameterList::AddParameter( dealGPMParameter *parameter ){
	if( ! parameter || HasParameterNamed( parameter->GetName().GetString() ) ){
		DETHROW( deeInvalidParam );
	}
	pParameters.Add( parameter );
}

void dealGPMParameterList::RemoveParameter( dealGPMParameter *parameter ){
	const int index = IndexOfParameter( parameter );
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	pParameters.RemoveFrom( index );
}

void dealGPMParameterList::RemoveAllParameters(){
	pParameters.RemoveAll();
}
