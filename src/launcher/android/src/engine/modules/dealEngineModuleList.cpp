/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
