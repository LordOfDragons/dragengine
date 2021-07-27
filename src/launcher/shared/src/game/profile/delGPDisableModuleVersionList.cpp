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

#include "delGPDisableModuleVersion.h"
#include "delGPDisableModuleVersionList.h"

#include <dragengine/common/exceptions.h>



// Class delGPDisableModuleVersionList
////////////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

delGPDisableModuleVersionList::delGPDisableModuleVersionList(){
}

delGPDisableModuleVersionList::delGPDisableModuleVersionList( const delGPDisableModuleVersionList &copy ){
	*this = copy;
}

delGPDisableModuleVersionList::~delGPDisableModuleVersionList(){
	RemoveAll();
}



// Management
///////////////

int delGPDisableModuleVersionList::GetCount() const{
	return pList.GetCount();
}

delGPDisableModuleVersion *delGPDisableModuleVersionList::GetAt( int index ) const{
	return ( delGPDisableModuleVersion* )pList.GetAt( index );
}

delGPDisableModuleVersion *delGPDisableModuleVersionList::GetWith(
const char *name, const char *version ) const{
	if( ! name ){
		DETHROW_INFO( deeNullPointer, "name" );
	}
	if( ! version ){
		DETHROW_INFO( deeNullPointer, "version" );
	}
	
	const int count = pList.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		delGPDisableModuleVersion * const module = ( delGPDisableModuleVersion* )pList.GetAt( i );
		if( module->GetName() == name && module->GetVersion() == version ){
			return module;
		}
	}
	
	return nullptr;
}

bool delGPDisableModuleVersionList::Has( delGPDisableModuleVersion *entry ) const{
	return pList.Has( entry );
}

bool delGPDisableModuleVersionList::HasWith( const char *name, const char *version ) const{
	return GetWith( name, version );
}

int delGPDisableModuleVersionList::IndexOf( delGPDisableModuleVersion *entry ) const{
	return pList.IndexOf( entry );
}

int delGPDisableModuleVersionList::IndexOfWith( const char *name, const char *version ) const{
	if( ! name ){
		DETHROW_INFO( deeNullPointer, "name" );
	}
	if( ! version ){
		DETHROW_INFO( deeNullPointer, "version" );
	}
	
	const int count = pList.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const delGPDisableModuleVersion &module = *( ( delGPDisableModuleVersion* )pList.GetAt( i ) );
		if( module.GetName() == name && module.GetVersion() == version ){
			return i;
		}
	}
	
	return -1;
}

void delGPDisableModuleVersionList::Add( delGPDisableModuleVersion *entry ){
	if( ! entry ){
		DETHROW_INFO( deeNullPointer, "entry" );
	}
	if( HasWith( entry->GetName(), entry->GetVersion() ) ){
		DETHROW_INFO( deeInvalidParam, "matching entry is present" );
	}
	
	pList.Add( entry );
}

void delGPDisableModuleVersionList::Remove( delGPDisableModuleVersion *entry ){
	const int index = IndexOf( entry );
	if( index == -1 ){
		DETHROW_INFO( deeInvalidParam, "entry is absent" );
	}
	
	pList.RemoveFrom( index );
}

void delGPDisableModuleVersionList::RemoveAll(){
	pList.RemoveAll();
}



// Operators
//////////////

delGPDisableModuleVersionList &delGPDisableModuleVersionList::operator=(
const delGPDisableModuleVersionList &other ){
	const decObjectList &otherList = other.pList;
	const int count = otherList.GetCount();
	int i;
	
	pList.RemoveAll();
	
	for( i=0; i<count; i++ ){
		pList.Add( delGPDisableModuleVersion::Ref::New( new delGPDisableModuleVersion(
			*( ( delGPDisableModuleVersion* )otherList.GetAt( i ) ) ) ) );
	}
	
	return *this;
}
