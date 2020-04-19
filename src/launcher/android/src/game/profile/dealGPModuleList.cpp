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

#include "dealGPModule.h"
#include "dealGPModuleList.h"

#include "../../common/exceptions.h"



// Class dealGPModuleList
///////////////////////////

// Constructors and Destructors
/////////////////////////////////

dealGPModuleList::dealGPModuleList(){
}

dealGPModuleList::~dealGPModuleList(){
	RemoveAllModules();
}



// Management
///////////////

int dealGPModuleList::GetModuleCount() const{
	return pModules.GetCount();
}

dealGPModule *dealGPModuleList::GetModuleAt( int index ) const{
	return ( dealGPModule* )pModules.GetAt( index );
}

dealGPModule *dealGPModuleList::GetModuleNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	int i, count = pModules.GetCount();
	dealGPModule *module;
	
	for( i=0; i<count; i++ ){
		module = ( dealGPModule* )pModules.GetAt( i );
		if( module->GetName().Equals( name ) ){
			return module;
		}
	}
	
	return NULL;
}

bool dealGPModuleList::HasModule( dealGPModule *module ) const{
	return pModules.Has( module );
}

bool dealGPModuleList::HasModuleNamed( const char *name ) const{
	return GetModuleNamed( name ) != NULL;
}

int dealGPModuleList::IndexOfModule( dealGPModule *module ) const{
	return pModules.IndexOf( module );
}

int dealGPModuleList::IndexOfModuleNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	int i, count = pModules.GetCount();
	
	for( i=0; i<count; i++ ){
		if( ( ( dealGPModule* )pModules.GetAt( i ) )->GetName().Equals( name ) ){
			return i;
		}
	}
	
	return -1;
}

void dealGPModuleList::AddModule( dealGPModule *module ){
	if( ! module || HasModuleNamed( module->GetName().GetString() ) ){
		DETHROW( deeInvalidParam );
	}
	pModules.Add( module );
}

void dealGPModuleList::RemoveModule( dealGPModule *module ){
	const int index = IndexOfModule( module );
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	pModules.RemoveFrom( index );
}

void dealGPModuleList::RemoveAllModules(){
	pModules.RemoveAll();
}
