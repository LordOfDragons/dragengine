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

#include <string.h>

#include "deMicrosoftGDK.h"
#include "deGDKServiceSystem.h"

#include <dragengine/common/exceptions.h>


// export definition
#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *MSGDKCreateModule(deLoadableModule *loadableModule);
#ifdef  __cplusplus
}
#endif


// entry function
// used to create a dragonscript scripting module.
// has to be named CreateModule returning deBaseModule.
// returns NULL on error.
/////////////////////////////////////////////////////////

deBaseModule *MSGDKCreateModule(deLoadableModule *loadableModule){
	try{
		return new deMicrosoftGDK(*loadableModule);
	}catch(...){
		return nullptr;
	}
}


// Class deMicrosoftGDK
/////////////////////////


// Constructor, destructor
////////////////////////////

deMicrosoftGDK::deMicrosoftGDK(deLoadableModule& loadableModule) :
deBaseServiceModule(loadableModule){
}

deMicrosoftGDK::~deMicrosoftGDK(){
}


// Management
///////////////

decStringSet deMicrosoftGDK::GetSupportedServices(){
	decStringSet names;
	names.Add(deGDKServiceSystem::serviceName);
	return names;
}

deBaseServiceService* deMicrosoftGDK::CreateService(deService *service, const char* name){
	DEASSERT_NOTNULL(service)

	if(strcmp(name, deGDKServiceSystem::serviceName) == 0){
		return new deGDKServiceSystem(*this, service);
	}

	return nullptr;
}
