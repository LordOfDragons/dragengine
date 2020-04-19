/* 
 * Drag[en]gine GUI Launcher
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

#include "deglEMParameter.h"
#include "deglEMParameterList.h"

#include <dragengine/common/exceptions.h>



// Class deglEMParameterList
//////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deglEMParameterList::deglEMParameterList(){
}

deglEMParameterList::~deglEMParameterList(){
	RemoveAllParameters();
}



// Management
///////////////

int deglEMParameterList::GetParameterCount() const{
	return pParameters.GetCount();
}

deglEMParameter *deglEMParameterList::GetParameterAt( int index ) const{
	return ( deglEMParameter* )pParameters.GetAt( index );
}

deglEMParameter *deglEMParameterList::GetParameterNamed( const char *name ) const{
	if( ! name ) DETHROW( deeInvalidParam );
	
	int i, count = pParameters.GetCount();
	deglEMParameter *parameter;
	
	for( i=0; i<count; i++ ){
		parameter = ( deglEMParameter* )pParameters.GetAt( i );
		if( parameter->GetInfo().GetName().Equals( name ) ){
			return parameter;
		}
	}
	
	return NULL;
}

bool deglEMParameterList::HasParameter( deglEMParameter *parameter ) const{
	return pParameters.Has( parameter );
}

bool deglEMParameterList::HasParameterNamed( const char *name ) const{
	return GetParameterNamed( name ) != NULL;
}

int deglEMParameterList::IndexOfParameter( deglEMParameter *parameter ) const{
	return pParameters.IndexOf( parameter );
}

int deglEMParameterList::IndexOfParameterNamed( const char *name ) const{
	if( ! name ) DETHROW( deeInvalidParam );
	
	int i, count = pParameters.GetCount();
	
	for( i=0; i<count; i++ ){
		if( ( ( deglEMParameter* )pParameters.GetAt( i ) )->GetInfo().GetName().Equals( name ) ){
			return i;
		}
	}
	
	return -1;
}

void deglEMParameterList::AddParameter( deglEMParameter *parameter ){
	if( ! parameter || HasParameterNamed( parameter->GetInfo().GetName() ) ){
		DETHROW( deeInvalidParam );
	}
	
	pParameters.Add( parameter );
}

void deglEMParameterList::RemoveParameter( deglEMParameter *parameter ){
	int index = IndexOfParameter( parameter );
	
	if( index == -1 ) DETHROW( deeInvalidParam );
	
	pParameters.RemoveFrom( index );
}

void deglEMParameterList::RemoveAllParameters(){
	pParameters.RemoveAll();
}
