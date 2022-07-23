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

#include "delEngineModule.h"
#include "delEngineModuleList.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/deModuleSystem.h>



// Class delEngineModuleList
///////////////////////////////

// Constructors and Destructors
/////////////////////////////////

delEngineModuleList::delEngineModuleList(){
}

delEngineModuleList::~delEngineModuleList(){
	RemoveAll();
}



// Management
///////////////

int delEngineModuleList::GetCount() const{
	return pModules.GetCount();
}

delEngineModule *delEngineModuleList::GetAt( int index ) const{
	return ( delEngineModule* )pModules.GetAt( index );
}

delEngineModule *delEngineModuleList::GetNamed( const char *name ) const{
	if( ! name ){
		DETHROW_INFO( deeNullPointer, "name" );
	}
	
	const int count = pModules.GetCount();
	delEngineModule *latestModule = NULL;
	int i;
	
	for( i=0; i<count; i++ ){
		delEngineModule * const module = ( delEngineModule* )pModules.GetAt( i );
		if( module->GetName() != name ){
			continue;
		}
		if( latestModule && deModuleSystem::CompareVersion(
				module->GetVersion(), latestModule->GetVersion() ) <= 0 ){
			continue;
		}
		
		latestModule = module;
	}
	
	return latestModule;
}

delEngineModule *delEngineModuleList::GetNamed( const char *name, const char *version ) const{
	if( ! name ){
		DETHROW_INFO( deeNullPointer, "name" );
	}
	if( ! version ){
		DETHROW_INFO( deeNullPointer, "version" );
	}
	
	const int count = pModules.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		delEngineModule * const module = ( delEngineModule* )pModules.GetAt( i );
		if( module->GetName() == name && module->GetVersion() == version ){
			return module;
		}
	}
	
	return nullptr;
}

delEngineModule *delEngineModuleList::GetNamedAtLeast( const char *name, const char *version ) const{
	if( ! name ){
		DETHROW_INFO( deeNullPointer, "name" );
	}
	if( ! version ){
		DETHROW_INFO( deeNullPointer, "version" );
	}
	
	const int count = pModules.GetCount();
	delEngineModule *latestModule = NULL;
	int i;
	
	for( i=0; i<count; i++ ){
		delEngineModule * const module = ( delEngineModule* )pModules.GetAt( i );
		if( module->GetName() != name || deModuleSystem::CompareVersion(
				module->GetVersion(), version ) < 0 ){
			continue;
		}
		if( latestModule && deModuleSystem::CompareVersion(
				module->GetVersion(), latestModule->GetVersion() ) <= 0 ){
			continue;
		}
		
		latestModule = module;
	}
	
	return latestModule;
}

void delEngineModuleList::GetNamed( decObjectList &list, const char *name ) const{
	if( ! name ){
		DETHROW_INFO( deeNullPointer, "name" );
	}
	
	const int count = pModules.GetCount();
	int i;
	
	list.RemoveAll();
	
	for( i=0; i<count; i++ ){
		delEngineModule * const module = ( delEngineModule* )pModules.GetAt( i );
		if( module->GetName() == name ){
			list.Add( module );
		}
	}
}

void delEngineModuleList::GetNames( decStringList &list ) const{
	const int count = pModules.GetCount();
	int i;
	
	list.RemoveAll();
	
	for( i=0; i<count; i++ ){
		delEngineModule * const module = ( delEngineModule* )pModules.GetAt( i );
		if( ! list.Has( module->GetName() ) ){
			list.Add( module->GetName() );
		}
	}
}

bool delEngineModuleList::Has( delEngineModule *module ) const{
	return pModules.Has( module );
}

bool delEngineModuleList::HasNamed( const char *name ) const{
	if( ! name ){
		DETHROW_INFO( deeNullPointer, "name" );
	}
	
	const int count = pModules.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const delEngineModule &module = *( ( delEngineModule* )pModules.GetAt( i ) );
		if( module.GetName() == name ){
			return true;
		}
	}
	
	return false;
}

bool delEngineModuleList::HasNamed( const char *name, const char *version ) const{
	if( ! name ){
		DETHROW_INFO( deeNullPointer, "name" );
	}
	if( ! version ){
		DETHROW_INFO( deeNullPointer, "version" );
	}
	
	const int count = pModules.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const delEngineModule &module = *( ( delEngineModule* )pModules.GetAt( i ) );
		if( module.GetName() == name && module.GetVersion() == version ){
			return true;
		}
	}
	
	return false;
}

int delEngineModuleList::IndexOf( delEngineModule *module ) const{
	return pModules.IndexOf( module );
}

int delEngineModuleList::IndexOfNamed( const char *name ) const{
	if( ! name ){
		DETHROW_INFO( deeNullPointer, "name" );
	}
	
	const int count = pModules.GetCount();
	const delEngineModule *latestModule = NULL;
	int latestIndex = -1;
	int i;
	
	for( i=0; i<count; i++ ){
		const delEngineModule * const module = ( delEngineModule* )pModules.GetAt( i );
		if( module->GetName() == name ){
			if( ! latestModule || deModuleSystem::CompareVersion(
					module->GetVersion(), latestModule->GetVersion() ) > 0 ){
				latestModule = module;
				latestIndex = i;
			}
		}
	}
	
	return latestIndex;
}

int delEngineModuleList::IndexOfNamed( const char *name, const char *version ) const{
	if( ! name ){
		DETHROW_INFO( deeNullPointer, "name" );
	}
	if( ! version ){
		DETHROW_INFO( deeNullPointer, "version" );
	}
	
	const int count = pModules.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const delEngineModule &module = *( ( delEngineModule* )pModules.GetAt( i ) );
		if( module.GetName() == name && module.GetVersion() == version ){
			return i;
		}
	}
	
	return -1;
}

void delEngineModuleList::Add( delEngineModule *module ){
	if( ! module ){
		DETHROW_INFO( deeNullPointer, "module" );
	}
	if( HasNamed( module->GetName(), module->GetVersion() ) ){
		DETHROW_INFO( deeInvalidParam, "named module with version is present" );
	}
	
	pModules.Add( module );
}

void delEngineModuleList::Remove( delEngineModule *module ){
	const int index = IndexOf ( module );
	if( index == -1 ){
		DETHROW_INFO( deeInvalidParam, "moduel is absent" );
	}
	pModules.RemoveFrom( index );
}

void delEngineModuleList::RemoveAll(){
	pModules.RemoveAll();
}
