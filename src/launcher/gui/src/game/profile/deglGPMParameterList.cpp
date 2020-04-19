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

#include "deglGPMParameter.h"
#include "deglGPMParameterList.h"

#include <dragengine/common/exceptions.h>



// Class deglGPMParameterList
///////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deglGPMParameterList::deglGPMParameterList(){
}

deglGPMParameterList::~deglGPMParameterList(){
	RemoveAllParameters();
}



// Management
///////////////

int deglGPMParameterList::GetParameterCount() const{
	return pParameters.GetCount();
}

deglGPMParameter *deglGPMParameterList::GetParameterAt( int index ) const{
	return ( deglGPMParameter* )pParameters.GetAt( index );
}

deglGPMParameter *deglGPMParameterList::GetParameterNamed( const char *name ) const{
	if( ! name ) DETHROW( deeInvalidParam );
	
	int i, count = pParameters.GetCount();
	deglGPMParameter *parameter;
	
	for( i=0; i<count; i++ ){
		parameter = ( deglGPMParameter* )pParameters.GetAt( i );
		if( parameter->GetName().Equals( name ) ){
			return parameter;
		}
	}
	
	return NULL;
}

bool deglGPMParameterList::HasParameter( deglGPMParameter *parameter ) const{
	return pParameters.Has( parameter );
}

bool deglGPMParameterList::HasParameterNamed( const char *name ) const{
	return GetParameterNamed( name ) != NULL;
}

int deglGPMParameterList::IndexOfParameter( deglGPMParameter *parameter ) const{
	return pParameters.IndexOf( parameter );
}

int deglGPMParameterList::IndexOfParameterNamed( const char *name ) const{
	if( ! name ) DETHROW( deeInvalidParam );
	
	int i, count = pParameters.GetCount();
	
	for( i=0; i<count; i++ ){
		if( ( ( deglGPMParameter* )pParameters.GetAt( i ) )->GetName().Equals( name ) ){
			return i;
		}
	}
	
	return -1;
}

void deglGPMParameterList::AddParameter( deglGPMParameter *parameter ){
	if( ! parameter || HasParameterNamed( parameter->GetName().GetString() ) ) DETHROW( deeInvalidParam );
	
	pParameters.Add( parameter );
}

void deglGPMParameterList::RemoveParameter( deglGPMParameter *parameter ){
	int index = IndexOfParameter( parameter );
	
	if( index == -1 ) DETHROW( deeInvalidParam );
	
	pParameters.RemoveFrom( index );
}

void deglGPMParameterList::RemoveAllParameters(){
	pParameters.RemoveAll();
}
