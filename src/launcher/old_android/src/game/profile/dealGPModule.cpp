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

#include "dealGPModule.h"
#include "dealGPMParameter.h"
#include "../../dealLauncher.h"
#include "../../engine/dealEngine.h"
#include "../../engine/modules/dealEngineModule.h"
#include "../../engine/modules/parameter/dealEMParameter.h"

#include "../../../srcengine/dealIEngineInstance.h"

#include "../../common/exceptions.h"



// Class dealGPModule
///////////////////////

// Constructors and Destructors
/////////////////////////////////

dealGPModule::dealGPModule(){
}

dealGPModule::dealGPModule(const dealGPModule &module){
	CopyFrom(module);
}

dealGPModule::~dealGPModule(){
}



// Management
///////////////

void dealGPModule::SetName(const char *name){
	pName = name;
}



void dealGPModule::CopyFrom(const dealGPModule &module){
	const dealGPMParameterList &parameterList = module.GetParameterList();
	int i, parameterCount = parameterList.GetParameterCount();
	dealGPMParameter::Ref parameter = NULL;
	
	pName = module.pName;
	
	pParameterList.RemoveAllParameters();
	
	try{
		for(i=0; i<parameterCount; i++){
			parameter.TakeOver(new dealGPMParameter(*parameterList.GetParameterAt(i)));
			pParameterList.AddParameter(parameter);
			parameter = NULL;
		}
		
	}catch(const deException &){
		throw;
	}
}

void dealGPModule::ApplyParameters(const char *version, dealLauncher &launcher,
dealIEngineInstance &engineInstance) const{
	dealEngineModule * const engineModule = launcher.GetEngine()
		.GetModuleList().GetModuleNamed(pName, version);
	if(!engineModule){
		return;
	}
	
	const dealEMParameterList &engineParameterList = engineModule->GetParameterList();
	const int count = pParameterList.GetParameterCount();
	int i;
	
	for(i=0; i<count; i++){
		const dealGPMParameter &parameter = *pParameterList.GetParameterAt(i);
		dealEMParameter * const engineParameter =
			engineParameterList.GetParameterNamed(parameter.GetName());
		
		if(engineParameter){
			if(!engineInstance.SetModuleParameterValue(pName, version,
					parameter.GetName(), parameter.GetValue())){
				DETHROW(deeInvalidAction);
			}
		}
	}
}
