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
