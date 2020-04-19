/* 
 * Drag[en]gine Game Engine
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

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deInternalModule.h"
#include "deLoadableModuleVisitor.h"
#include "deBaseModule.h"
#include "../deModuleSystem.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"



// class deInternalModule
///////////////////////////

// Constructor, destructor
////////////////////////////

deInternalModule::deInternalModule( deModuleSystem *system ) : deLoadableModule( system ){
}

deInternalModule::~deInternalModule(){
}



// Module management
//////////////////////

void deInternalModule::LoadModule(){
	if( IsLoaded() ) DETHROW( deeInvalidAction );
	
	// creates the module using the subclassed function.
	CreateModule();
	if( GetErrorCode() != eecSuccess ) return;
}

void deInternalModule::UnloadModule(){
	if( ! IsLoaded() || IsLocked() ) DETHROW( deeInvalidAction );
	SetModule( NULL );
	SetErrorCode( eecSuccess );
}



// Type Testing and Safe Casting
//////////////////////////////////

bool deInternalModule::IsInternalModule() const{
	return true;
}

deInternalModule *deInternalModule::CastToInternalModule(){
	return this;
}



// visiting
/////////////

void deInternalModule::Visit( deLoadableModuleVisitor &visitor ){
	visitor.VisitInternalModule( *this );
}
