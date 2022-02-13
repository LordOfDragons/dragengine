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

#include "delGPModule.h"
#include "delGPModuleList.h"
#include "delGPMParameter.h"

#include <dragengine/common/exceptions.h>



// Class delGPModuleList
///////////////////////////

// Constructors and Destructors
/////////////////////////////////

delGPModuleList::delGPModuleList(){
}

delGPModuleList::~delGPModuleList(){
	RemoveAll();
}



// Management
///////////////

int delGPModuleList::GetCount() const{
	return pModules.GetCount();
}

delGPModule *delGPModuleList::GetAt( int index ) const{
	return ( delGPModule* )pModules.GetAt( index );
}

delGPModule *delGPModuleList::GetNamed ( const char *name ) const{
	if( ! name ){
		DETHROW_INFO( deeNullPointer, "name" );
	}
	
	const int count = pModules.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		delGPModule * const module = ( delGPModule* )pModules.GetAt( i );
		if( module->GetName() == name ){
			return module;
		}
	}
	
	return nullptr;
}

bool delGPModuleList::Has( delGPModule *module ) const{
	return pModules.Has( module );
}

bool delGPModuleList::HasNamed( const char *name ) const{
	return GetNamed ( name );
}

int delGPModuleList::IndexOf( delGPModule *module ) const{
	return pModules.IndexOf( module );
}

int delGPModuleList::IndexOfNamed( const char *name ) const{
	if( ! name ){
		DETHROW_INFO( deeNullPointer, "name" );
	}
	
	const int count = pModules.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( delGPModule* )pModules.GetAt( i ) )->GetName().Equals( name ) ){
			return i;
		}
	}
	
	return -1;
}

void delGPModuleList::Add( delGPModule *module ){
	if( ! module ){
		DETHROW_INFO( deeNullPointer, "module" );
	}
	if( HasNamed ( module->GetName() ) ){
		DETHROW_INFO( deeInvalidParam, "named module is present" );
	}
	
	pModules.Add( module );
}

void delGPModuleList::Remove ( delGPModule *module ){
	const int index = IndexOf ( module );
	if( index == -1 ){
		DETHROW_INFO( deeInvalidParam, "module is absent" );
	}
	
	pModules.RemoveFrom( index );
}

void delGPModuleList::RemoveAll(){
	pModules.RemoveAll();
}

void delGPModuleList::Update( const delGPModuleList &list ){
	const int moduleCount = list.GetCount();
	int i, j;
	
	for( i=0; i<moduleCount; i++ ){
		const delGPModule &moduleChanges = *list.GetAt ( i );
		delGPModule * const module = GetNamed( moduleChanges.GetName() );
		
		if( module ){
			delGPMParameterList &parameters = module->GetParameters();
			const delGPMParameterList &paramsChanges = moduleChanges.GetParameters();
			const int paramCount = paramsChanges.GetCount();
			
			for( j=0; j<paramCount; j++ ){
				const delGPMParameter &paramChanges = *paramsChanges.GetAt ( j );
				delGPMParameter * const parameter = parameters.GetNamed ( paramChanges.GetName() );
				
				if( parameter ){
					parameter->SetValue( paramChanges.GetValue() );
					
				}else{
					parameters.Add ( delGPMParameter::Ref::New( new delGPMParameter( paramChanges ) ) );
				}
			}
			
		}else{
			Add( delGPModule::Ref::New( new delGPModule( moduleChanges ) ) );
		}
	}
}
