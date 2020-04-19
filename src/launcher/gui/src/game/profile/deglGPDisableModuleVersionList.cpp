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

#include "deglGPDisableModuleVersion.h"
#include "deglGPDisableModuleVersionList.h"

#include <dragengine/common/exceptions.h>



// Class deglGPDisableModuleVersionList
//////////////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deglGPDisableModuleVersionList::deglGPDisableModuleVersionList(){
}

deglGPDisableModuleVersionList::deglGPDisableModuleVersionList( const deglGPDisableModuleVersionList &copy ){
	*this = copy;
}

deglGPDisableModuleVersionList::~deglGPDisableModuleVersionList(){
	RemoveAll();
}



// Management
///////////////

int deglGPDisableModuleVersionList::GetCount() const{
	return pList.GetCount();
}

deglGPDisableModuleVersion *deglGPDisableModuleVersionList::GetAt( int index ) const{
	return ( deglGPDisableModuleVersion* )pList.GetAt( index );
}

deglGPDisableModuleVersion *deglGPDisableModuleVersionList::GetWith( const char *name, const char *version ) const{
	if( ! name || ! version ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pList.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deglGPDisableModuleVersion * const module = ( deglGPDisableModuleVersion* )pList.GetAt( i );
		if( module->GetName() == name && module->GetVersion() == version ){
			return module;
		}
	}
	
	return NULL;
}

bool deglGPDisableModuleVersionList::Has( deglGPDisableModuleVersion *entry ) const{
	return pList.Has( entry );
}

bool deglGPDisableModuleVersionList::HasWith( const char *name, const char *version ) const{
	return GetWith( name, version ) != NULL;
}

int deglGPDisableModuleVersionList::IndexOf( deglGPDisableModuleVersion *entry ) const{
	return pList.IndexOf( entry );
}

int deglGPDisableModuleVersionList::IndexOfWith( const char *name, const char *version ) const{
	if( ! name || ! version ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pList.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const deglGPDisableModuleVersion &module = *( ( deglGPDisableModuleVersion* )pList.GetAt( i ) );
		if( module.GetName() == name && module.GetVersion() == version ){
			return i;
		}
	}
	
	return -1;
}

void deglGPDisableModuleVersionList::Add( deglGPDisableModuleVersion *entry ){
	if( ! entry || HasWith( entry->GetName(), entry->GetVersion() ) ){
		DETHROW( deeInvalidParam );
	}
	pList.Add( entry );
}

void deglGPDisableModuleVersionList::Remove( deglGPDisableModuleVersion *entry ){
	const int index = IndexOf( entry );
	
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	pList.RemoveFrom( index );
}

void deglGPDisableModuleVersionList::RemoveAll(){
	pList.RemoveAll();
}



// Operators
//////////////

deglGPDisableModuleVersionList &deglGPDisableModuleVersionList::operator=( const deglGPDisableModuleVersionList &other ){
	const decObjectList &otherList = other.pList;
	deglGPDisableModuleVersion *entry = NULL;
	const int count = otherList.GetCount();
	int i;
	
	pList.RemoveAll();
	
	try{
		for( i=0; i<count; i++ ){
			entry = new deglGPDisableModuleVersion( *( ( deglGPDisableModuleVersion* )otherList.GetAt( i ) ) );
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
