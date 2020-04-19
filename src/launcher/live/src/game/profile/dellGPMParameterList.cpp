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

#include "dellGPMParameter.h"
#include "dellGPMParameterList.h"

#include <dragengine/common/exceptions.h>



// Class dellGPMParameterList
///////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dellGPMParameterList::dellGPMParameterList(){
}

dellGPMParameterList::~dellGPMParameterList(){
	RemoveAll();
}



// Management
///////////////

int dellGPMParameterList::GetCount() const{
	return pParameters.GetCount();
}

dellGPMParameter *dellGPMParameterList::GetAt( int index ) const{
	return ( dellGPMParameter* )pParameters.GetAt( index );
}

dellGPMParameter *dellGPMParameterList::GetNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pParameters.GetCount();
	dellGPMParameter *parameter;
	int i;
	
	for( i=0; i<count; i++ ){
		parameter = ( dellGPMParameter* )pParameters.GetAt( i );
		if( parameter->GetName().Equals( name ) ){
			return parameter;
		}
	}
	
	return NULL;
}

bool dellGPMParameterList::Has( dellGPMParameter *parameter ) const{
	return pParameters.Has( parameter );
}

bool dellGPMParameterList::HasNamed( const char *name ) const{
	return GetNamed( name ) != NULL;
}

int dellGPMParameterList::IndexOf( dellGPMParameter *parameter ) const{
	return pParameters.IndexOf( parameter );
}

int dellGPMParameterList::IndexOfNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pParameters.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( dellGPMParameter* )pParameters.GetAt( i ) )->GetName().Equals( name ) ){
			return i;
		}
	}
	
	return -1;
}

void dellGPMParameterList::Add( dellGPMParameter *parameter ){
	if( ! parameter || HasNamed( parameter->GetName().GetString() ) ){
		DETHROW( deeInvalidParam );
	}
	
	pParameters.Add( parameter );
}

void dellGPMParameterList::Remove( dellGPMParameter *parameter ){
	const int index = IndexOf( parameter );
	
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	pParameters.RemoveFrom( index );
}

void dellGPMParameterList::RemoveAll(){
	pParameters.RemoveAll();
}
