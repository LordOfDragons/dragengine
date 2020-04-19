/* 
 * Drag[en]gine Console Launcher
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

#include "declGPDisableModuleVersion.h"
#include "declGPDisableModuleVersionList.h"

#include <dragengine/common/exceptions.h>



// Class declGPDisableModuleVersionList
//////////////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

declGPDisableModuleVersionList::declGPDisableModuleVersionList(){
}

declGPDisableModuleVersionList::declGPDisableModuleVersionList( const declGPDisableModuleVersionList &copy ){
	*this = copy;
}

declGPDisableModuleVersionList::~declGPDisableModuleVersionList(){
	RemoveAll();
}



// Management
///////////////

int declGPDisableModuleVersionList::GetCount() const{
	return pList.GetCount();
}

declGPDisableModuleVersion *declGPDisableModuleVersionList::GetAt( int index ) const{
	return ( declGPDisableModuleVersion* )pList.GetAt( index );
}

declGPDisableModuleVersion *declGPDisableModuleVersionList::GetWith( const char *name, const char *version ) const{
	if( ! name || ! version ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pList.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		declGPDisableModuleVersion * const module = ( declGPDisableModuleVersion* )pList.GetAt( i );
		if( module->GetName() == name && module->GetVersion() == version ){
			return module;
		}
	}
	
	return NULL;
}

bool declGPDisableModuleVersionList::Has( declGPDisableModuleVersion *entry ) const{
	return pList.Has( entry );
}

bool declGPDisableModuleVersionList::HasWith( const char *name, const char *version ) const{
	return GetWith( name, version ) != NULL;
}

int declGPDisableModuleVersionList::IndexOf( declGPDisableModuleVersion *entry ) const{
	return pList.IndexOf( entry );
}

int declGPDisableModuleVersionList::IndexOfWith( const char *name, const char *version ) const{
	if( ! name || ! version ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pList.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const declGPDisableModuleVersion &module = *( ( declGPDisableModuleVersion* )pList.GetAt( i ) );
		if( module.GetName() == name && module.GetVersion() == version ){
			return i;
		}
	}
	
	return -1;
}

void declGPDisableModuleVersionList::Add( declGPDisableModuleVersion *entry ){
	if( ! entry || HasWith( entry->GetName(), entry->GetVersion() ) ){
		DETHROW( deeInvalidParam );
	}
	pList.Add( entry );
}

void declGPDisableModuleVersionList::Remove( declGPDisableModuleVersion *entry ){
	const int index = IndexOf( entry );
	
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	pList.RemoveFrom( index );
}

void declGPDisableModuleVersionList::RemoveAll(){
	pList.RemoveAll();
}



// Operators
//////////////

declGPDisableModuleVersionList &declGPDisableModuleVersionList::operator=( const declGPDisableModuleVersionList &other ){
	const decObjectList &otherList = other.pList;
	declGPDisableModuleVersion *entry = NULL;
	const int count = otherList.GetCount();
	int i;
	
	pList.RemoveAll();
	
	try{
		for( i=0; i<count; i++ ){
			entry = new declGPDisableModuleVersion( *( ( declGPDisableModuleVersion* )otherList.GetAt( i ) ) );
			pList.Add( entry );
			entry->FreeReference();
			entry = NULL;
		}
		
	}catch( const deException & ){
		if( entry ){
			entry->FreeReference();
		}
		throw;
	}
	
	return *this;
}
