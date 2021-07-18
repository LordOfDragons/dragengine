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

#include "delGPModule.h"
#include "delGPMParameter.h"
#include "../../delLauncher.h"
#include "../../engine/delEngine.h"
#include "../../engine/delEngineInstance.h"
#include "../../engine/modules/delEngineModule.h"
#include "../../engine/modules/parameter/delEMParameter.h"

#include <dragengine/common/exceptions.h>



// Class delGPModule
///////////////////////

// Constructors and Destructors
/////////////////////////////////

delGPModule::delGPModule( const char *name ) :
pName( name ){
}

delGPModule::delGPModule( const delGPModule &module ){
	*this = module;
}

delGPModule::~delGPModule(){
}



// Management
///////////////

void delGPModule::SetName( const char *name ){
	pName = name;
}

void delGPModule::ApplyParameters( const char *version, delLauncher &launcher,
delEngineInstance &engineInstance ) const{
	delEngineModule * const module = launcher.GetEngine().GetModules().GetNamed( pName, version );
	if( ! module ){
		return;
	}
	
	const delEMParameterList &parameters = module->GetParameters();
	const int count = pParameters.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const delGPMParameter &parameter = *pParameters.GetAt ( i );
		if( parameters.HasNamed( parameter.GetName() ) ){
			engineInstance.SetModuleParameter( pName, version, parameter.GetName(), parameter.GetValue() );
		}
	}
}



// Operators
//////////////

delGPModule &delGPModule::operator=( const delGPModule &module ){
	const delGPMParameterList &parameters = module.GetParameters();
	const int count = parameters.GetCount();
	int i;
	
	pName = module.pName;
	
	pParameters.RemoveAll();
	
	for( i=0; i<count; i++ ){
		pParameters.Add ( delGPMParameter::Ref::With( new delGPMParameter( *parameters.GetAt ( i ) ) ) );
	}
	
	return *this;
}
