/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include "../dragengine_configuration.h"
#if defined(OS_UNIX_X11) && defined(OS_UNIX_WAYLAND)

#include "deWaylandHelper.h"


// Class dewlDynamicLibrary
/////////////////////////////

// Constructor, destructor
////////////////////////////

deWlDynLib::deWlDynLib(const char *source, const char *name, int majorVersion) :
pSource(source),
pName(name)
{
	pHandle = dlopen(decString::Formatted("lib{0}.so.{1}", pName, majorVersion), RTLD_LAZY | RTLD_LOCAL);
	if(!pHandle){
		pHandle = dlopen(decString::Formatted("lib{0}.so", pName), RTLD_LAZY | RTLD_LOCAL);
	}
}

deWlDynLib::~deWlDynLib(){
	if(pHandle){
		dlclose(pHandle);
	}
}


// Management
///////////////

void deWlDynLib::Required(){
	if(!pHandle){
		DETHROW_INFO(deeInvalidParam, decString::Formatted("Failed loading library lib{0}", pName));
	}
}

bool deWlDynLib::RequiredLog(){
	if(!pHandle){
		printf("[%s] lib%s not found\n", pSource.GetString(), pName.GetString());
	}
	return pHandle != nullptr;
}

void *deWlDynLib::Resolve(const char *symbol){
	if(!pHandle){
		return nullptr;
	}
	
	auto ptr = dlsym(pHandle, symbol);
	if(!ptr){
		printf("[%s] Failed resolving symbol '%s' in library lib%s\n",
			pSource.GetString(), symbol, pName.GetString());
	}
	return ptr;
}

void *deWlDynLib::ResolveRequired(const char *symbol){
	auto ptr = Resolve(symbol);
	if(!ptr){
		DETHROW_INFO(deeInvalidParam, decString::Formatted(
			"Failed resolving symbol '{0}' in library lib{1}", symbol, pName));
	}
	return ptr;
}

#endif
