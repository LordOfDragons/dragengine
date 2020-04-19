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

#include "dellGPDisableModuleVersion.h"
#include "dellGPDisableModuleVersionList.h"

#include <dragengine/common/exceptions.h>



// Class dellGPDisableModuleVersionList
//////////////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dellGPDisableModuleVersionList::dellGPDisableModuleVersionList(){
}

dellGPDisableModuleVersionList::dellGPDisableModuleVersionList( const dellGPDisableModuleVersionList &copy ){
	*this = copy;
}

dellGPDisableModuleVersionList::~dellGPDisableModuleVersionList(){
	RemoveAll();
}



// Management
///////////////

int dellGPDisableModuleVersionList::GetCount() const{
	return pList.GetCount();
}

dellGPDisableModuleVersion *dellGPDisableModuleVersionList::GetAt( int index ) const{
	return ( dellGPDisableModuleVersion* )pList.GetAt( index );
}

dellGPDisableModuleVersion *dellGPDisableModuleVersionList::GetWith( const char *name, const char *version ) const{
	if( ! name || ! version ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pList.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		dellGPDisableModuleVersion * const module = ( dellGPDisableModuleVersion* )pList.GetAt( i );
		if( module->GetName() == name && module->GetVersion() == version ){
			return module;
		}
	}
	
	return NULL;
}

bool dellGPDisableModuleVersionList::Has( dellGPDisableModuleVersion *entry ) const{
	return pList.Has( entry );
}

bool dellGPDisableModuleVersionList::HasWith( const char *name, const char *version ) const{
	return GetWith( name, version ) != NULL;
}

int dellGPDisableModuleVersionList::IndexOf( dellGPDisableModuleVersion *entry ) const{
	return pList.IndexOf( entry );
}

int dellGPDisableModuleVersionList::IndexOfWith( const char *name, const char *version ) const{
	if( ! name || ! version ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pList.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const dellGPDisableModuleVersion &module = *( ( dellGPDisableModuleVersion* )pList.GetAt( i ) );
		if( module.GetName() == name && module.GetVersion() == version ){
			return i;
		}
	}
	
	return -1;
}

void dellGPDisableModuleVersionList::Add( dellGPDisableModuleVersion *entry ){
	if( ! entry || HasWith( entry->GetName(), entry->GetVersion() ) ){
		DETHROW( deeInvalidParam );
	}
	pList.Add( entry );
}

void dellGPDisableModuleVersionList::Remove( dellGPDisableModuleVersion *entry ){
	const int index = IndexOf( entry );
	
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	pList.RemoveFrom( index );
}

void dellGPDisableModuleVersionList::RemoveAll(){
	pList.RemoveAll();
}



// Operators
//////////////

dellGPDisableModuleVersionList &dellGPDisableModuleVersionList::operator=( const dellGPDisableModuleVersionList &other ){
	const decObjectList &otherList = other.pList;
	dellGPDisableModuleVersion *entry = NULL;
	const int count = otherList.GetCount();
	int i;
	
	pList.RemoveAll();
	
	try{
		for( i=0; i<count; i++ ){
			entry = new dellGPDisableModuleVersion( *( ( dellGPDisableModuleVersion* )otherList.GetAt( i ) ) );
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
