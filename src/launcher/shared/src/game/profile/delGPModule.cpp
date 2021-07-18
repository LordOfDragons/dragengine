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
#include "../../delLauncherSupport.h"
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

void delGPModule::ApplyParameters( const char *version, delLauncherSupport &support,
delEngineInstance &engineInstance ) const{
	delEngineModule * const engineModule = support.GetEngine()->GetModuleList()
		.GetModuleNamed( pName, version );
	if( ! engineModule ){
		return;
	}
	
	const delEMParameterList &engineParameterList = engineModule->GetParameters();
	const int count = pParameterList.GetCount();
	delEMParameter *engineParameter;
	int i;
	
	for( i=0; i<count; i++ ){
		const delGPMParameter &parameter = *pParameterList.GetAt ( i );
		
		engineParameter = engineParameterList.GetNamed ( parameter.GetName() );
		
		if( engineParameter ){
			engineInstance.SetModuleParameter( pName, version,
				parameter.GetName(), parameter.GetValue() );
		}
	}
}



// Operators
//////////////

delGPModule &delGPModule::operator=( const delGPModule &module ){
	const delGPMParameterList &parameterList = module.GetParameterList();
	const int parameterCount = parameterList.GetCount();
	int i;
	
	pName = module.pName;
	
	pParameterList.RemoveAll();
	
	for( i=0; i<parameterCount; i++ ){
		pParameterList.Add ( delGPMParameter::Ref::With(
			new delGPMParameter( *parameterList.GetAt ( i ) ) ) );
	}
}
