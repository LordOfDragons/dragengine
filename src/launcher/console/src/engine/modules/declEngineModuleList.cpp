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

#include "declEngineModule.h"
#include "declEngineModuleList.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/deModuleSystem.h>



// Class declEngineModuleList
///////////////////////////////

// Constructors and Destructors
/////////////////////////////////

declEngineModuleList::declEngineModuleList(){
}

declEngineModuleList::~declEngineModuleList(){
	RemoveAll();
}



// Management
///////////////

int declEngineModuleList::GetCount() const{
	return pModules.GetCount();
}

declEngineModule *declEngineModuleList::GetAt( int index ) const{
	return ( declEngineModule* )pModules.GetAt( index );
}

declEngineModule *declEngineModuleList::GetNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pModules.GetCount();
	declEngineModule *latestModule = NULL;
	int i;
	
	for( i=0; i<count; i++ ){
		declEngineModule * const module = ( declEngineModule* )pModules.GetAt( i );
		if( module->GetName() != name ){
			continue;
		}
		if( latestModule && deModuleSystem::CompareVersion( module->GetVersion(), latestModule->GetVersion() ) <= 0 ){
			continue;
		}
		
		latestModule = module;
	}
	
	return latestModule;
}

declEngineModule *declEngineModuleList::GetNamed( const char *name, const char *version ) const{
	if( ! name || ! version ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pModules.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		declEngineModule * const module = ( declEngineModule* )pModules.GetAt( i );
		if( module->GetName() == name && module->GetVersion() == version ){
			return module;
		}
	}
	
	return NULL;
}

declEngineModule *declEngineModuleList::GetNamedAtLeast( const char* name, const char* version ) const{
	if( ! name || ! version ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pModules.GetCount();
	declEngineModule *latestModule = NULL;
	int i;
	
	for( i=0; i<count; i++ ){
		declEngineModule * const module = ( declEngineModule* )pModules.GetAt( i );
		if( module->GetName() != name || deModuleSystem::CompareVersion( module->GetVersion(), version ) < 0 ){
			continue;
		}
		if( latestModule && deModuleSystem::CompareVersion( module->GetVersion(), latestModule->GetVersion() ) <= 0 ){
			continue;
		}
		
		latestModule = module;
	}
	
	return latestModule;
}

void declEngineModuleList::GetAllNamed( decObjectList &list, const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pModules.GetCount();
	int i;
	
	list.RemoveAll();
	
	for( i=0; i<count; i++ ){
		declEngineModule * const module = ( declEngineModule* )pModules.GetAt( i );
		if( module->GetName() == name ){
			list.Add( module );
		}
	}
}

void declEngineModuleList::GetAllNames( decStringList &list ) const{
	const int count = pModules.GetCount();
	int i;
	
	list.RemoveAll();
	
	for( i=0; i<count; i++ ){
		declEngineModule * const module = ( declEngineModule* )pModules.GetAt( i );
		if( ! list.Has( module->GetName() ) ){
			list.Add( module->GetName() );
		}
	}
}

bool declEngineModuleList::Has( declEngineModule *module ) const{
	return pModules.Has( module );
}

bool declEngineModuleList::HasNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pModules.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const declEngineModule &module = *( ( declEngineModule* )pModules.GetAt( i ) );
		if( module.GetName() == name ){
			return true;
		}
	}
	
	return false;
}

bool declEngineModuleList::HasModuleNamed( const char *name, const char *version ) const{
	if( ! name || ! version ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pModules.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const declEngineModule &module = *( ( declEngineModule* )pModules.GetAt( i ) );
		if( module.GetName() == name && module.GetVersion() == version ){
			return true;
		}
	}
	
	return false;
}

int declEngineModuleList::IndexOf( declEngineModule *module ) const{
	return pModules.IndexOf( module );
}

int declEngineModuleList::IndexOfNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pModules.GetCount();
	const declEngineModule *latestModule = NULL;
	int latestIndex = -1;
	int i;
	
	for( i=0; i<count; i++ ){
		const declEngineModule * const module = ( declEngineModule* )pModules.GetAt( i );
		if( module->GetName() == name ){
			if( ! latestModule || deModuleSystem::CompareVersion( module->GetVersion(), latestModule->GetVersion() ) > 0 ){
				latestModule = module;
				latestIndex = i;
			}
		}
	}
	
	return latestIndex;
}

int declEngineModuleList::IndexOfModuleNamed( const char *name, const char *version ) const{
	if( ! name || ! version ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pModules.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const declEngineModule &module = *( ( declEngineModule* )pModules.GetAt( i ) );
		if( module.GetName() == name && module.GetVersion() == version ){
			return i;
		}
	}
	
	return -1;
}

void declEngineModuleList::Add( declEngineModule *module ){
	if( ! module || HasModuleNamed( module->GetName(), module->GetVersion() ) ){
		DETHROW( deeInvalidParam );
	}
	pModules.Add( module );
}

void declEngineModuleList::Remove( declEngineModule *module ){
	int index = IndexOf( module );
	
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	pModules.RemoveFrom( index );
}

void declEngineModuleList::RemoveAll(){
	pModules.RemoveAll();
}
