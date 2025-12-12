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

delGPModule::delGPModule(const char *name) :
pName(name){
}

delGPModule::delGPModule(const delGPModule &module){
	*this = module;
}

delGPModule::~delGPModule(){
}



// Management
///////////////

void delGPModule::SetName(const char *name){
	pName = name;
}

void delGPModule::ApplyParameters(const char *version, delLauncher &launcher,
delEngineInstance &engineInstance) const{
	delEngineModule * const module = launcher.GetEngine().GetModules().GetNamed(pName, version);
	if(!module){
		return;
	}
	
	const delEMParameterList &parameters = module->GetParameters();
	const int count = pParameters.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		const delGPMParameter &parameter = *pParameters.GetAt (i);
		if(parameters.HasNamed(parameter.GetName())){
			engineInstance.SetModuleParameter(pName, version, parameter.GetName(), parameter.GetValue());
		}
	}
}



// Operators
//////////////

delGPModule &delGPModule::operator=(const delGPModule &module){
	const delGPMParameterList &parameters = module.GetParameters();
	const int count = parameters.GetCount();
	int i;
	
	pName = module.pName;
	
	pParameters.RemoveAll();
	
	for(i=0; i<count; i++){
		pParameters.Add (delGPMParameter::Ref::New(*parameters.GetAt (i)));
	}
	
	return *this;
}
