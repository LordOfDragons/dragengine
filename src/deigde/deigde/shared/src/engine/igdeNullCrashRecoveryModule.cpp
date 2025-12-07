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

#include "igdeNullCrashRecoveryModule.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/deBaseModule.h>



// igdeNullCrashRecoveryModule::cModule
/////////////////////////////////////////

igdeNullCrashRecoveryModule::cModule::cModule(deModuleSystem *system) :
deInternalModule(system)
{
	SetName("NullCrashRecovery");
	SetType(deModuleSystem::emtCrashRecovery);
	SetDescription("Null CrashRecovery Module");
	SetAuthor("Plüss Roland");
	SetVersion("1.0");
	SetDirectoryName("NullCrashRecovery");
	SetDefaultLoggingName();
}

igdeNullCrashRecoveryModule::cModule::~cModule(){
}

void igdeNullCrashRecoveryModule::cModule::CreateModule(){
	SetModule(new igdeNullCrashRecoveryModule(*this));
	if(!GetModule()){
		SetErrorCode(eecCreateModuleFailed);
	}
}



// class igdeNullCrashRecoveryModule
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNullCrashRecoveryModule::igdeNullCrashRecoveryModule(deLoadableModule &loadableModule) :
deBaseCrashRecoveryModule(loadableModule){
}

igdeNullCrashRecoveryModule::~igdeNullCrashRecoveryModule(){
}



// Runtime
////////////

bool igdeNullCrashRecoveryModule::Init(){
	return true;
}

void igdeNullCrashRecoveryModule::CleanUp(){
}

bool igdeNullCrashRecoveryModule::RecoverFromError(){
	return false;
}
