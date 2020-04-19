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

#include "dealGPDisableModuleVersion.h"
#include "dealGPDisableModuleVersionList.h"

#include "../../common/exceptions.h"



// Class dealGPDisableModuleVersionList
//////////////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dealGPDisableModuleVersionList::dealGPDisableModuleVersionList(){
}

dealGPDisableModuleVersionList::dealGPDisableModuleVersionList( const dealGPDisableModuleVersionList &copy ){
	*this = copy;
}

dealGPDisableModuleVersionList::~dealGPDisableModuleVersionList(){
	RemoveAll();
}



// Management
///////////////

int dealGPDisableModuleVersionList::GetCount() const{
	return pList.GetCount();
}

dealGPDisableModuleVersion *dealGPDisableModuleVersionList::GetAt( int index ) const{
	return ( dealGPDisableModuleVersion* )pList.GetAt( index );
}

dealGPDisableModuleVersion *dealGPDisableModuleVersionList::GetWith( const char *name, const char *version ) const{
	if( ! name || ! version ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pList.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		dealGPDisableModuleVersion * const module = ( dealGPDisableModuleVersion* )pList.GetAt( i );
		if( module->GetName() == name && module->GetVersion() == version ){
			return module;
		}
	}
	
	return NULL;
}

bool dealGPDisableModuleVersionList::Has( dealGPDisableModuleVersion *entry ) const{
	return pList.Has( entry );
}

bool dealGPDisableModuleVersionList::HasWith( const char *name, const char *version ) const{
	return GetWith( name, version ) != NULL;
}

int dealGPDisableModuleVersionList::IndexOf( dealGPDisableModuleVersion *entry ) const{
	return pList.IndexOf( entry );
}

int dealGPDisableModuleVersionList::IndexOfWith( const char *name, const char *version ) const{
	if( ! name || ! version ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pList.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const dealGPDisableModuleVersion &module = *( ( dealGPDisableModuleVersion* )pList.GetAt( i ) );
		if( module.GetName() == name && module.GetVersion() == version ){
			return i;
		}
	}
	
	return -1;
}

void dealGPDisableModuleVersionList::Add( dealGPDisableModuleVersion *entry ){
	if( ! entry || HasWith( entry->GetName(), entry->GetVersion() ) ){
		DETHROW( deeInvalidParam );
	}
	pList.Add( entry );
}

void dealGPDisableModuleVersionList::Remove( dealGPDisableModuleVersion *entry ){
	const int index = IndexOf( entry );
	
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	pList.RemoveFrom( index );
}

void dealGPDisableModuleVersionList::RemoveAll(){
	pList.RemoveAll();
}



// Operators
//////////////

dealGPDisableModuleVersionList &dealGPDisableModuleVersionList::operator=( const dealGPDisableModuleVersionList &other ){
	const decObjectList &otherList = other.pList;
	dealGPDisableModuleVersion *entry = NULL;
	const int count = otherList.GetCount();
	int i;
	
	pList.RemoveAll();
	
	try{
		for( i=0; i<count; i++ ){
			entry = new dealGPDisableModuleVersion( *( ( dealGPDisableModuleVersion* )otherList.GetAt( i ) ) );
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
