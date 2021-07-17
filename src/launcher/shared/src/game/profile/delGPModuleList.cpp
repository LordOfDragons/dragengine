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

#include "delGPModule.h"
#include "delGPModuleList.h"

#include <dragengine/common/exceptions.h>



// Class delGPModuleList
///////////////////////////

// Constructors and Destructors
/////////////////////////////////

delGPModuleList::delGPModuleList(){
}

delGPModuleList::~delGPModuleList(){
	RemoveAllModules();
}



// Management
///////////////

int delGPModuleList::GetModuleCount() const{
	return pModules.GetCount();
}

delGPModule *delGPModuleList::GetModuleAt( int index ) const{
	return ( delGPModule* )pModules.GetAt( index );
}

delGPModule *delGPModuleList::GetModuleNamed( const char *name ) const{
	if( ! name ) DETHROW( deeInvalidParam );
	
	int i, count = pModules.GetCount();
	delGPModule *module;
	
	for( i=0; i<count; i++ ){
		module = ( delGPModule* )pModules.GetAt( i );
		if( module->GetName().Equals( name ) ){
			return module;
		}
	}
	
	return NULL;
}

bool delGPModuleList::HasModule( delGPModule *module ) const{
	return pModules.Has( module );
}

bool delGPModuleList::HasModuleNamed( const char *name ) const{
	return GetModuleNamed( name ) != NULL;
}

int delGPModuleList::IndexOfModule( delGPModule *module ) const{
	return pModules.IndexOf( module );
}

int delGPModuleList::IndexOfModuleNamed( const char *name ) const{
	if( ! name ) DETHROW( deeInvalidParam );
	
	int i, count = pModules.GetCount();
	
	for( i=0; i<count; i++ ){
		if( ( ( delGPModule* )pModules.GetAt( i ) )->GetName().Equals( name ) ){
			return i;
		}
	}
	
	return -1;
}

void delGPModuleList::AddModule( delGPModule *module ){
	if( ! module || HasModuleNamed( module->GetName().GetString() ) ) DETHROW( deeInvalidParam );
	
	pModules.Add( module );
}

void delGPModuleList::RemoveModule( delGPModule *module ){
	int index = IndexOfModule( module );
	
	if( index == -1 ) DETHROW( deeInvalidParam );
	
	pModules.RemoveFrom( index );
}

void delGPModuleList::RemoveAllModules(){
	pModules.RemoveAll();
}
