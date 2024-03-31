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

#include <stdlib.h>

#include "deOS.h"
#include "../deEngine.h"
#include "../common/exceptions.h"
#include "../systems/deInputSystem.h"
#include "../systems/deScriptingSystem.h"



// Class deOS
///////////////

// Constructor, destructor
////////////////////////////

deOS::deOS() :
pEngine( NULL ),
pAppActive( false ){
}

deOS::~deOS(){
}



// Management
///////////////

void deOS::SetEngine( deEngine *engine ){
	pEngine = engine;
}

void deOS::SetAppActive( bool active ){
	if( active == pAppActive ){
		return;
	}
	
	pAppActive = active;
	
	if( pEngine ){
		pEngine->GetInputSystem()->AppActivationChanged();
		pEngine->GetScriptingSystem()->OnAppActivate();
	}
}



// Casting
////////////

deOSUnix *deOS::CastToOSUnix(){
	DETHROW( deeInvalidParam );
}

deOSConsole *deOS::CastToOSConsole(){
	DETHROW( deeInvalidParam );
}

deOSWindows *deOS::CastToOSWindows(){
	DETHROW( deeInvalidParam );
}

deOSBeOS *deOS::CastToOSBeOS(){
	DETHROW( deeInvalidParam );
}

deOSAndroid *deOS::CastToOSAndroid(){
	DETHROW( deeInvalidParam );
}

deOSMacOS *deOS::CastToOSMacOS(){
	DETHROW( deeInvalidParam );
}
