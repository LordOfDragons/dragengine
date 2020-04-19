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

#include "deglEngineModule.h"
#include "deglEngineModuleList.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/deModuleSystem.h>



// Class deglEngineModuleList
///////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deglEngineModuleList::deglEngineModuleList(){
}

deglEngineModuleList::~deglEngineModuleList(){
	RemoveAllModules();
}



// Management
///////////////

int deglEngineModuleList::GetModuleCount() const{
	return pModules.GetCount();
}

deglEngineModule *deglEngineModuleList::GetModuleAt( int index ) const{
	return ( deglEngineModule* )pModules.GetAt( index );
}

deglEngineModule *deglEngineModuleList::GetModuleNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pModules.GetCount();
	deglEngineModule *latestModule = NULL;
	int i;
	
	for( i=0; i<count; i++ ){
		deglEngineModule * const module = ( deglEngineModule* )pModules.GetAt( i );
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

deglEngineModule *deglEngineModuleList::GetModuleNamed( const char *name, const char *version ) const{
	if( ! name || ! version ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pModules.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deglEngineModule * const module = ( deglEngineModule* )pModules.GetAt( i );
		if( module->GetName() == name && module->GetVersion() == version ){
			return module;
		}
	}
	
	return NULL;
}

deglEngineModule *deglEngineModuleList::GetModuleNamedAtLeast( const char *name, const char *version ) const{
	if( ! name || ! version ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pModules.GetCount();
	deglEngineModule *latestModule = NULL;
	int i;
	
	for( i=0; i<count; i++ ){
		deglEngineModule * const module = ( deglEngineModule* )pModules.GetAt( i );
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

void deglEngineModuleList::GetModulesNamed( decObjectList &list, const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pModules.GetCount();
	int i;
	
	list.RemoveAll();
	
	for( i=0; i<count; i++ ){
		deglEngineModule * const module = ( deglEngineModule* )pModules.GetAt( i );
		if( module->GetName() == name ){
			list.Add( module );
		}
	}
}

void deglEngineModuleList::GetModulesNames( decStringList &list ) const{
	const int count = pModules.GetCount();
	int i;
	
	list.RemoveAll();
	
	for( i=0; i<count; i++ ){
		deglEngineModule * const module = ( deglEngineModule* )pModules.GetAt( i );
		if( ! list.Has( module->GetName() ) ){
			list.Add( module->GetName() );
		}
	}
}

bool deglEngineModuleList::HasModule( deglEngineModule *module ) const{
	return pModules.Has( module );
}

bool deglEngineModuleList::HasModuleNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pModules.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const deglEngineModule &module = *( ( deglEngineModule* )pModules.GetAt( i ) );
		if( module.GetName() == name ){
			return true;
		}
	}
	
	return false;
}

bool deglEngineModuleList::HasModuleNamed( const char *name, const char *version ) const{
	if( ! name || ! version ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pModules.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const deglEngineModule &module = *( ( deglEngineModule* )pModules.GetAt( i ) );
		if( module.GetName() == name && module.GetVersion() == version ){
			return true;
		}
	}
	
	return false;
}

int deglEngineModuleList::IndexOfModule( deglEngineModule *module ) const{
	return pModules.IndexOf( module );
}

int deglEngineModuleList::IndexOfModuleNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pModules.GetCount();
	const deglEngineModule *latestModule = NULL;
	int latestIndex = -1;
	int i;
	
	for( i=0; i<count; i++ ){
		const deglEngineModule * const module = ( deglEngineModule* )pModules.GetAt( i );
		if( module->GetName() == name ){
			if( ! latestModule || deModuleSystem::CompareVersion( module->GetVersion(), latestModule->GetVersion() ) > 0 ){
				latestModule = module;
				latestIndex = i;
			}
		}
	}
	
	return latestIndex;
}

int deglEngineModuleList::IndexOfModuleNamed( const char *name, const char *version ) const{
	if( ! name || ! version ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pModules.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const deglEngineModule &module = *( ( deglEngineModule* )pModules.GetAt( i ) );
		if( module.GetName() == name && module.GetVersion() == version ){
			return i;
		}
	}
	
	return -1;
}

void deglEngineModuleList::AddModule( deglEngineModule *module ){
	if( ! module || HasModuleNamed( module->GetName(), module->GetVersion() ) ){
		DETHROW( deeInvalidParam );
	}
	pModules.Add( module );
}

void deglEngineModuleList::RemoveModule( deglEngineModule *module ){
	const int index = IndexOfModule( module );
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	pModules.RemoveFrom( index );
}

void deglEngineModuleList::RemoveAllModules(){
	pModules.RemoveAll();
}
