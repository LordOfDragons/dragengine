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

#include "dealGPModule.h"
#include "dealGPMParameter.h"
#include "../../dealLauncher.h"
#include "../../engine/dealEngine.h"
#include "../../engine/modules/dealEngineModule.h"
#include "../../engine/modules/parameter/dealEMParameter.h"

#include "../../../srcengine/dealIEngineInstance.h"

#include "../../common/exceptions.h"



// Class dealGPModule
///////////////////////

// Constructors and Destructors
/////////////////////////////////

dealGPModule::dealGPModule(){
}

dealGPModule::dealGPModule( const dealGPModule &module ){
	CopyFrom( module );
}

dealGPModule::~dealGPModule(){
}



// Management
///////////////

void dealGPModule::SetName( const char *name ){
	pName = name;
}



void dealGPModule::CopyFrom( const dealGPModule &module ){
	const dealGPMParameterList &parameterList = module.GetParameterList();
	int i, parameterCount = parameterList.GetParameterCount();
	dealGPMParameter *parameter = NULL;
	
	pName = module.pName;
	
	pParameterList.RemoveAllParameters();
	
	try{
		for( i=0; i<parameterCount; i++ ){
			parameter = new dealGPMParameter( *parameterList.GetParameterAt( i ) );
			pParameterList.AddParameter( parameter );
			parameter = NULL;
		}
		
	}catch( const deException & ){
		if( parameter ){
			parameter->FreeReference();
		}
		throw;
	}
}

void dealGPModule::ApplyParameters( const char *version, dealLauncher &launcher,
dealIEngineInstance &engineInstance ) const{
	dealEngineModule * const engineModule = launcher.GetEngine()
		.GetModuleList().GetModuleNamed( pName, version );
	if( ! engineModule ){
		return;
	}
	
	const dealEMParameterList &engineParameterList = engineModule->GetParameterList();
	const int count = pParameterList.GetParameterCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const dealGPMParameter &parameter = *pParameterList.GetParameterAt( i );
		dealEMParameter * const engineParameter =
			engineParameterList.GetParameterNamed( parameter.GetName() );
		
		if( engineParameter ){
			if( ! engineInstance.SetModuleParameterValue( pName, version,
					parameter.GetName(), parameter.GetValue() ) ){
				DETHROW( deeInvalidAction );
			}
		}
	}
}
