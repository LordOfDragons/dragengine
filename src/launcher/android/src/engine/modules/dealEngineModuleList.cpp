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

#include "dealEngineModule.h"
#include "dealEngineModuleList.h"
#include "../../common/exceptions.h"



// Class dealEngineModuleList
///////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dealEngineModuleList::dealEngineModuleList(){
}

dealEngineModuleList::~dealEngineModuleList(){
	RemoveAllModules();
}



// Management
///////////////

int dealEngineModuleList::GetModuleCount() const{
	return pModules.GetCount();
}

dealEngineModule *dealEngineModuleList::GetModuleAt( int index ) const{
	return ( dealEngineModule* )pModules.GetAt( index );
}

dealEngineModule *dealEngineModuleList::GetModuleNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pModules.GetCount();
	dealEngineModule *latestModule = NULL;
	int i;
	
	for( i=0; i<count; i++ ){
		dealEngineModule * const module = ( dealEngineModule* )pModules.GetAt( i );
		if( module->GetName() == name ){
			if( ! latestModule || dealEngineModule::CompareVersion( module->GetVersion(), latestModule->GetVersion() ) > 0 ){
				latestModule = module;
			}
		}
	}
	
	return latestModule;
}

dealEngineModule *dealEngineModuleList::GetModuleNamed( const char *name, const char *version ) const{
	if( ! name || ! version ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pModules.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		dealEngineModule * const module = ( dealEngineModule* )pModules.GetAt( i );
		if( module->GetName() == name && module->GetVersion() == version ){
			return module;
		}
	}
	
	return NULL;
}

void dealEngineModuleList::GetModulesNamed( decObjectList &list, const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pModules.GetCount();
	int i;
	
	list.RemoveAll();
	
	for( i=0; i<count; i++ ){
		dealEngineModule * const module = ( dealEngineModule* )pModules.GetAt( i );
		if( module->GetName() == name ){
			list.Add( module );
		}
	}
}

void dealEngineModuleList::GetModulesNames( decStringList &list ) const{
	const int count = pModules.GetCount();
	int i;
	
	list.RemoveAll();
	
	for( i=0; i<count; i++ ){
		dealEngineModule * const module = ( dealEngineModule* )pModules.GetAt( i );
		if( ! list.Has( module->GetName() ) ){
			list.Add( module->GetName() );
		}
	}
}

bool dealEngineModuleList::HasModule( dealEngineModule *module ) const{
	return pModules.Has( module );
}

bool dealEngineModuleList::HasModuleNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pModules.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const dealEngineModule &module = *( ( dealEngineModule* )pModules.GetAt( i ) );
		if( module.GetName() == name ){
			return true;
		}
	}
	
	return false;
}

bool dealEngineModuleList::HasModuleNamed( const char *name, const char *version ) const{
	if( ! name || ! version ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pModules.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const dealEngineModule &module = *( ( dealEngineModule* )pModules.GetAt( i ) );
		if( module.GetName() == name && module.GetVersion() == version ){
			return true;
		}
	}
	
	return false;
}

int dealEngineModuleList::IndexOfModule( dealEngineModule *module ) const{
	return pModules.IndexOf( module );
}

int dealEngineModuleList::IndexOfModuleNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pModules.GetCount();
	const dealEngineModule *latestModule = NULL;
	int latestIndex = -1;
	int i;
	
	for( i=0; i<count; i++ ){
		const dealEngineModule * const module = ( dealEngineModule* )pModules.GetAt( i );
		if( module->GetName() == name ){
			if( ! latestModule || dealEngineModule::CompareVersion( module->GetVersion(), latestModule->GetVersion() ) > 0 ){
				latestModule = module;
				latestIndex = i;
			}
		}
	}
	
	return latestIndex;
}

int dealEngineModuleList::IndexOfModuleNamed( const char *name, const char *version ) const{
	if( ! name || ! version ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pModules.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const dealEngineModule &module = *( ( dealEngineModule* )pModules.GetAt( i ) );
		if( module.GetName() == name && module.GetVersion() == version ){
			return i;
		}
	}
	
	return -1;
}

void dealEngineModuleList::AddModule( dealEngineModule *module ){
	if( ! module || HasModuleNamed( module->GetName(), module->GetVersion() ) ){
		DETHROW( deeInvalidParam );
	}
	pModules.Add( module );
}

void dealEngineModuleList::RemoveModule( dealEngineModule *module ){
	const int index = IndexOfModule( module );
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	pModules.RemoveFrom( index );
}

void dealEngineModuleList::RemoveAllModules(){
	pModules.RemoveAll();
}
