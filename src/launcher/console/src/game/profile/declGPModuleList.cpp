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

#include "declGPModule.h"
#include "declGPModuleList.h"
#include "declGPMParameter.h"
#include "../../declLauncher.h"
#include "../../engine/declEngine.h"
#include "../../engine/modules/declEngineModule.h"
#include "../../engine/modules/declEngineModuleList.h"
#include "../../engine/modules/parameter/declEMParameter.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>
#include <dragengine/systems/modules/deLoadableModule.h>
#include <dragengine/logger/deLogger.h>



// Definitions
////////////////

#define LOGSOURCE "Launcher"



// Class declGPModuleList
///////////////////////////

// Constructors and Destructors
/////////////////////////////////

declGPModuleList::declGPModuleList(){
}

declGPModuleList::~declGPModuleList(){
	RemoveAll();
}



// Management
///////////////

int declGPModuleList::GetCount() const{
	return pModules.GetCount();
}

declGPModule *declGPModuleList::GetAt( int index ) const{
	return ( declGPModule* )pModules.GetAt( index );
}

declGPModule *declGPModuleList::GetNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pModules.GetCount();
	declGPModule *module;
	int i;
	
	for( i=0; i<count; i++ ){
		module = ( declGPModule* )pModules.GetAt( i );
		if( module->GetName().Equals( name ) ){
			return module;
		}
	}
	
	return NULL;
}

bool declGPModuleList::Has( declGPModule *module ) const{
	return pModules.Has( module );
}

bool declGPModuleList::HasNamed( const char *name ) const{
	return GetNamed( name ) != NULL;
}

int declGPModuleList::IndexOf( declGPModule *module ) const{
	return pModules.IndexOf( module );
}

int declGPModuleList::IndexOfNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pModules.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( declGPModule* )pModules.GetAt( i ) )->GetName().Equals( name ) ){
			return i;
		}
	}
	
	return -1;
}

void declGPModuleList::Add( declGPModule *module ){
	if( ! module || HasNamed( module->GetName().GetString() ) ){
		DETHROW( deeInvalidParam );
	}
	
	pModules.Add( module );
}

void declGPModuleList::Remove( declGPModule *module ){
	const int index = IndexOf( module );
	
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	pModules.RemoveFrom( index );
}

void declGPModuleList::RemoveAll(){
	pModules.RemoveAll();
}



void declGPModuleList::Apply( declLauncher &launcher ) const{
	const declEngineModuleList &engineModuleList = launcher.GetEngine()->GetModuleList();
	const int moduleCount = pModules.GetCount();
	deLogger &logger = *launcher.GetLogger();
	declEngineModule *engineModule = NULL;
	int m, p;
	
	for( m=0; m<moduleCount; m++ ){
		const declGPModule &profileModule = *( ( declGPModule* )pModules.GetAt( m ) );
		
		engineModule = engineModuleList.GetNamed( profileModule.GetName().GetString() );
		
		if( engineModule && engineModule->GetLoadableModule() && engineModule->GetLoadableModule()->GetModule() ){
			const declGPMParameterList &profileParameterList = profileModule.GetParameterList();
			const declEMParameterList &engineParameterList = engineModule->GetParameterList();
			const int parameterCount = profileParameterList.GetCount();
			declEMParameter *engineParameter;
			
			for( p=0; p<parameterCount; p++ ){
				const declGPMParameter &profileParameter = *profileParameterList.GetAt( p );
				
				engineParameter = engineParameterList.GetNamed( profileParameter.GetName().GetString() );
				
				if( engineParameter ){
					logger.LogInfoFormat( LOGSOURCE, "Set Module Parameter: %s %s = %s",
						profileModule.GetName().GetString(), profileParameter.GetName().GetString(),
						profileParameter.GetValue().GetString() );
					engineModule->GetLoadableModule()->GetModule()->SetParameterValue(
						profileParameter.GetName(), profileParameter.GetValue() );
				}
			}
		}
	}
}
