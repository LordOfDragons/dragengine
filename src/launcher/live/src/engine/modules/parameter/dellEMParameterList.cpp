/* 
 * Drag[en]gine Live Launcher
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

#include "dellEMParameter.h"
#include "dellEMParameterList.h"

#include <dragengine/common/exceptions.h>



// Class dellEMParameterList
//////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dellEMParameterList::dellEMParameterList(){
}

dellEMParameterList::~dellEMParameterList(){
	RemoveAll();
}



// Management
///////////////

int dellEMParameterList::GetCount() const{
	return pParameters.GetCount();
}

dellEMParameter *dellEMParameterList::GetAt( int index ) const{
	return ( dellEMParameter* )pParameters.GetAt( index );
}

dellEMParameter *dellEMParameterList::GetNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pParameters.GetCount();
	dellEMParameter *parameter;
	int i;
	
	for( i=0; i<count; i++ ){
		parameter = ( dellEMParameter* )pParameters.GetAt( i );
		if( parameter->GetName().Equals( name ) ){
			return parameter;
		}
	}
	
	return NULL;
}

bool dellEMParameterList::Has( dellEMParameter *parameter ) const{
	return pParameters.Has( parameter );
}

bool dellEMParameterList::HasNamed( const char *name ) const{
	return GetNamed( name ) != NULL;
}

int dellEMParameterList::IndexOf( dellEMParameter *parameter ) const{
	return pParameters.IndexOf( parameter );
}

int dellEMParameterList::IndexOfNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pParameters.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( dellEMParameter* )pParameters.GetAt( i ) )->GetName().Equals( name ) ){
			return i;
		}
	}
	
	return -1;
}

void dellEMParameterList::Add( dellEMParameter *parameter ){
	if( ! parameter || HasNamed( parameter->GetName().GetString() ) ){
		DETHROW( deeInvalidParam );
	}
	
	pParameters.Add( parameter );
}

void dellEMParameterList::Remove( dellEMParameter *parameter ){
	int index = IndexOf( parameter );
	
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	pParameters.RemoveFrom( index );
}

void dellEMParameterList::RemoveAll(){
	pParameters.RemoveAll();
}
