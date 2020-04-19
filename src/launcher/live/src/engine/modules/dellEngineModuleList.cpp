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

#include "dellEngineModule.h"
#include "dellEngineModuleList.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/deModuleSystem.h>



// Class dellEngineModuleList
///////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dellEngineModuleList::dellEngineModuleList(){
}

dellEngineModuleList::~dellEngineModuleList(){
	RemoveAll();
}



// Management
///////////////

int dellEngineModuleList::GetCount() const{
	return pModules.GetCount();
}

dellEngineModule *dellEngineModuleList::GetAt( int index ) const{
	return ( dellEngineModule* )pModules.GetAt( index );
}

dellEngineModule *dellEngineModuleList::GetNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pModules.GetCount();
	dellEngineModule *latestModule = NULL;
	int i;
	
	for( i=0; i<count; i++ ){
		dellEngineModule * const module = ( dellEngineModule* )pModules.GetAt( i );
		if( module->GetName() == name ){
			if( ! latestModule || deModuleSystem::CompareVersion( module->GetVersion(), latestModule->GetVersion() ) > 0 ){
				latestModule = module;
			}
		}
	}
	
	return latestModule;
}

dellEngineModule *dellEngineModuleList::GetNamed( const char *name, const char *version ) const{
	if( ! name || ! version ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pModules.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		dellEngineModule * const module = ( dellEngineModule* )pModules.GetAt( i );
		if( module->GetName() == name && module->GetVersion() == version ){
			return module;
		}
	}
	
	return NULL;
}

void dellEngineModuleList::GetAllNamed( decObjectList &list, const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pModules.GetCount();
	int i;
	
	list.RemoveAll();
	
	for( i=0; i<count; i++ ){
		dellEngineModule * const module = ( dellEngineModule* )pModules.GetAt( i );
		if( module->GetName() == name ){
			list.Add( module );
		}
	}
}

void dellEngineModuleList::GetAllNames( decStringList &list ) const{
	const int count = pModules.GetCount();
	int i;
	
	list.RemoveAll();
	
	for( i=0; i<count; i++ ){
		dellEngineModule * const module = ( dellEngineModule* )pModules.GetAt( i );
		if( ! list.Has( module->GetName() ) ){
			list.Add( module->GetName() );
		}
	}
}

bool dellEngineModuleList::Has( dellEngineModule *module ) const{
	return pModules.Has( module );
}

bool dellEngineModuleList::HasNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pModules.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const dellEngineModule &module = *( ( dellEngineModule* )pModules.GetAt( i ) );
		if( module.GetName() == name ){
			return true;
		}
	}
	
	return false;
}

bool dellEngineModuleList::HasModuleNamed( const char *name, const char *version ) const{
	if( ! name || ! version ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pModules.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const dellEngineModule &module = *( ( dellEngineModule* )pModules.GetAt( i ) );
		if( module.GetName() == name && module.GetVersion() == version ){
			return true;
		}
	}
	
	return false;
}

int dellEngineModuleList::IndexOf( dellEngineModule *module ) const{
	return pModules.IndexOf( module );
}

int dellEngineModuleList::IndexOfNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pModules.GetCount();
	const dellEngineModule *latestModule = NULL;
	int latestIndex = -1;
	int i;
	
	for( i=0; i<count; i++ ){
		const dellEngineModule * const module = ( dellEngineModule* )pModules.GetAt( i );
		if( module->GetName() == name ){
			if( ! latestModule || deModuleSystem::CompareVersion( module->GetVersion(), latestModule->GetVersion() ) > 0 ){
				latestModule = module;
				latestIndex = i;
			}
		}
	}
	
	return latestIndex;
}

int dellEngineModuleList::IndexOfModuleNamed( const char *name, const char *version ) const{
	if( ! name || ! version ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pModules.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const dellEngineModule &module = *( ( dellEngineModule* )pModules.GetAt( i ) );
		if( module.GetName() == name && module.GetVersion() == version ){
			return i;
		}
	}
	
	return -1;
}

void dellEngineModuleList::Add( dellEngineModule *module ){
	if( ! module || HasModuleNamed( module->GetName(), module->GetVersion() ) ){
		DETHROW( deeInvalidParam );
	}
	pModules.Add( module );
}

void dellEngineModuleList::Remove( dellEngineModule *module ){
	int index = IndexOf( module );
	
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	pModules.RemoveFrom( index );
}

void dellEngineModuleList::RemoveAll(){
	pModules.RemoveAll();
}
