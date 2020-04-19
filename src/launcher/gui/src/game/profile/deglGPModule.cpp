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

#include "deglGPModule.h"
#include "deglGPMParameter.h"
#include "../../deglLauncher.h"
#include "../../engine/deglEngine.h"
#include "../../engine/deglEngineInstance.h"
#include "../../engine/modules/deglEngineModule.h"
#include "../../engine/modules/parameter/deglEMParameter.h"

#include <dragengine/common/exceptions.h>



// Class deglGPModule
///////////////////////

// Constructors and Destructors
/////////////////////////////////

deglGPModule::deglGPModule( const char *name ) :
pName( name ){
}

deglGPModule::deglGPModule( const deglGPModule &module ){
	CopyFrom( module );
}

deglGPModule::~deglGPModule(){
}



// Management
///////////////

void deglGPModule::SetName( const char *name ){
	if( ! name ) DETHROW( deeInvalidParam );
	pName = name;
}



void deglGPModule::CopyFrom( const deglGPModule &module ){
	const deglGPMParameterList &parameterList = module.GetParameterList();
	int i, parameterCount = parameterList.GetParameterCount();
	deglGPMParameter *parameter = NULL;
	
	pName = module.pName;
	
	pParameterList.RemoveAllParameters();
	
	try{
		for( i=0; i<parameterCount; i++ ){
			parameter = new deglGPMParameter( *parameterList.GetParameterAt( i ) );
			if( ! parameter ) DETHROW( deeOutOfMemory );
			
			pParameterList.AddParameter( parameter );
			parameter = NULL;
		}
		
	}catch( const deException & ){
		if( parameter ) parameter->FreeReference();
		throw;
	}
}

void deglGPModule::ApplyParameters( const char *version, deglLauncher &launcher,
deglEngineInstance &engineInstance ) const{
	deglEngineModule * const engineModule = launcher.GetEngine()->GetModuleList()
		.GetModuleNamed( pName, version );
	if( ! engineModule ){
		return;
	}
	
	const deglEMParameterList &engineParameterList = engineModule->GetParameterList();
	const int count = pParameterList.GetParameterCount();
	deglEMParameter *engineParameter;
	int i;
	
	for( i=0; i<count; i++ ){
		const deglGPMParameter &parameter = *pParameterList.GetParameterAt( i );
		
		engineParameter = engineParameterList.GetParameterNamed( parameter.GetName() );
		
		if( engineParameter ){
			engineInstance.SetModuleParameter( pName, version,
				parameter.GetName(), parameter.GetValue() );
		}
	}
}
