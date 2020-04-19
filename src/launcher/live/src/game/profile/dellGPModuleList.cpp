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

#include "dellGPModule.h"
#include "dellGPModuleList.h"
#include "dellGPMParameter.h"
#include "../../dellLauncher.h"
#include "../../engine/dellEngine.h"
#include "../../engine/modules/dellEngineModule.h"
#include "../../engine/modules/dellEngineModuleList.h"
#include "../../engine/modules/parameter/dellEMParameter.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>
#include <dragengine/systems/modules/deLoadableModule.h>
#include <dragengine/logger/deLogger.h>



// Definitions
////////////////

#define LOGSOURCE "Launcher"



// Class dellGPModuleList
///////////////////////////

// Constructors and Destructors
/////////////////////////////////

dellGPModuleList::dellGPModuleList(){
}

dellGPModuleList::~dellGPModuleList(){
	RemoveAll();
}



// Management
///////////////

int dellGPModuleList::GetCount() const{
	return pModules.GetCount();
}

dellGPModule *dellGPModuleList::GetAt( int index ) const{
	return ( dellGPModule* )pModules.GetAt( index );
}

dellGPModule *dellGPModuleList::GetNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pModules.GetCount();
	dellGPModule *module;
	int i;
	
	for( i=0; i<count; i++ ){
		module = ( dellGPModule* )pModules.GetAt( i );
		if( module->GetName().Equals( name ) ){
			return module;
		}
	}
	
	return NULL;
}

bool dellGPModuleList::Has( dellGPModule *module ) const{
	return pModules.Has( module );
}

bool dellGPModuleList::HasNamed( const char *name ) const{
	return GetNamed( name ) != NULL;
}

int dellGPModuleList::IndexOf( dellGPModule *module ) const{
	return pModules.IndexOf( module );
}

int dellGPModuleList::IndexOfNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pModules.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( dellGPModule* )pModules.GetAt( i ) )->GetName().Equals( name ) ){
			return i;
		}
	}
	
	return -1;
}

void dellGPModuleList::Add( dellGPModule *module ){
	if( ! module || HasNamed( module->GetName().GetString() ) ){
		DETHROW( deeInvalidParam );
	}
	
	pModules.Add( module );
}

void dellGPModuleList::Remove( dellGPModule *module ){
	const int index = IndexOf( module );
	
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	pModules.RemoveFrom( index );
}

void dellGPModuleList::RemoveAll(){
	pModules.RemoveAll();
}



void dellGPModuleList::Apply( dellLauncher &launcher ) const{
	const dellEngineModuleList &engineModuleList = launcher.GetEngine().GetModuleList();
	const int moduleCount = pModules.GetCount();
	deLogger &logger = *launcher.GetLogger();
	dellEngineModule *engineModule = NULL;
	int m, p;
	
	for( m=0; m<moduleCount; m++ ){
		const dellGPModule &profileModule = *( ( dellGPModule* )pModules.GetAt( m ) );
		
		engineModule = engineModuleList.GetNamed( profileModule.GetName().GetString() );
		
		if( engineModule && engineModule->GetLoadableModule() && engineModule->GetLoadableModule()->GetModule() ){
			const dellGPMParameterList &profileParameterList = profileModule.GetParameterList();
			const dellEMParameterList &engineParameterList = engineModule->GetParameterList();
			const int parameterCount = profileParameterList.GetCount();
			dellEMParameter *engineParameter;
			
			for( p=0; p<parameterCount; p++ ){
				const dellGPMParameter &profileParameter = *profileParameterList.GetAt( p );
				
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
