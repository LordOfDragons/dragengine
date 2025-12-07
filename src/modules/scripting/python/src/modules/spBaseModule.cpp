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

#include "spBaseModule.h"
#include "../ScriptingPython.h"

#include <dragengine/common/exceptions.h>



// Initializers
/////////////////

static const PyModuleDef vModuleDefinitionInitializer = {
	PyModuleDef_HEAD_INIT,
	NULL,
	NULL,
	0,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};



// Native Data
////////////////

struct sModuleData{
	spBaseModule *ownerClass;
};



// Class spBaseModule
///////////////////////

// Constructor, destructor
////////////////////////////

spBaseModule::spBaseModule(ScriptingPython &sp, const char *name) :
pSP(sp)
{
	pParent = NULL;
	pName = name;
	
	pFunctions = NULL;
	pFunctionCount = 0;
	
	pPyModuleDefinition = NULL;
	pPyMethodDefs = NULL;
	pPyModule = NULL;
}

spBaseModule::~spBaseModule(){
	if(pPyModuleDefinition){
		delete pPyModuleDefinition;
		pPyModuleDefinition = NULL;
	}
	if(pPyMethodDefs){
		delete [] pPyMethodDefs;
		pPyMethodDefs = NULL;
	}
	
	if(pFunctions){
		while(pFunctionCount > 0){
			pFunctionCount--;
			if(pFunctions[pFunctionCount]){
				delete pFunctions[pFunctionCount];
			}
		}
		delete [] pFunctions;
		pFunctions = NULL;
		pFunctionCount = 0;
	}
}



// Management
///////////////

void spBaseModule::SetDocumentation(const char *documentation){
	pDocumentation = documentation;
}

void spBaseModule::SetParent(spBaseModule *parent){
	pParent = parent;
}

void spBaseModule::AddMethod(const char *name, PyCFunction pyFunction, int argumentType, const char *documentation){
	if(! name || ! pyFunction || ! documentation){
		DETHROW(deeInvalidParam);
	}
	
	// TODO check for duplicate name
	
	// enlarge array
	sFunction ** const newArray = new sFunction*[pFunctionCount + 1];
	
	if(pFunctions){
		memcpy(newArray, pFunctions, sizeof(sFunction*) * pFunctionCount);
		delete [] pFunctions;
	}
	
	pFunctions = newArray;
	pFunctions[pFunctionCount] = new sFunction;
	pFunctions[pFunctionCount]->name = name;
	pFunctions[pFunctionCount]->pyFunction = pyFunction;
	pFunctions[pFunctionCount]->argumentType = argumentType;
	pFunctions[pFunctionCount]->documentation = documentation;
	pFunctionCount++;
}



spBaseModule *spBaseModule::GetOwnerClass(PyObject *object){
	if(! object){
		DETHROW(deeNullPointer);
	}
	
	return ((sModuleData*)PyModule_GetState(object))->ownerClass;
}



void spBaseModule::PythonCreate(){
	pUpdateFullName();
	
	//pSP.LogInfoFormat( "Module %s PythonCreate", pFullModuleName.GetString() );
	
	pCreateMethodsDefinition();
	pCreateModuleDefinition();
	pCreateModule();
}

void spBaseModule::PythonCleanUp(){
	//pSP.LogInfoFormat( "Module %s PythonCleanUp", pFullModuleName.GetString() );
	
	if(pPyModule){
		Py_DECREF(pPyModule);
		pPyModule = NULL;
	}
}



// Private Functions
//////////////////////

void spBaseModule::pUpdateFullName(){
	spBaseModule *module = pParent;
	
	pFullModuleName = pName;
	while(module){
		pFullModuleName = module->GetName() + "." + pFullModuleName;
		module = module->GetParent();
	}
}

void spBaseModule::pCreateModuleDefinition(){
	decString fullModuleName;
	
	if(pPyModuleDefinition){
		delete pPyModuleDefinition;
		pPyModuleDefinition = NULL;
	}
	
	pPyModuleDefinition = new PyModuleDef;
	memcpy(pPyModuleDefinition, &vModuleDefinitionInitializer, sizeof(PyMethodDef));
	
	pPyModuleDefinition->m_name = pFullModuleName.GetString();
	pPyModuleDefinition->m_doc = pDocumentation.GetString();
	pPyModuleDefinition->m_size = sizeof(sModuleData);
	pPyModuleDefinition->m_methods = pPyMethodDefs;
	pPyModuleDefinition->m_reload = NULL;
	pPyModuleDefinition->m_traverse = NULL;
	pPyModuleDefinition->m_clear = NULL;
	pPyModuleDefinition->m_free = NULL;
}

void spBaseModule::pCreateMethodsDefinition(){
	if(pPyMethodDefs){
		delete [] pPyMethodDefs;
		pPyMethodDefs = NULL;
	}
	
	int i;
	
	pPyMethodDefs = new PyMethodDef[pFunctionCount + 1];
	memset(pPyMethodDefs, '\0', sizeof(PyMethodDef) * (pFunctionCount + 1));
	
	for(i=0; i<pFunctionCount; i++){
		pPyMethodDefs[i].ml_name = pFunctions[i]->name.GetString();
		pPyMethodDefs[i].ml_meth = pFunctions[i]->pyFunction;
		pPyMethodDefs[i].ml_flags = pFunctions[i]->argumentType;
		pPyMethodDefs[i].ml_doc = pFunctions[i]->documentation.GetString();
	}
}

void spBaseModule::pCreateModule(){
	if(pPyModule){
		Py_DECREF(pPyModule);
		pPyModule = NULL;
	}
	
	//pSP.LogInfoFormat( "Module %s create module", pFullModuleName.GetString() );
	
	pPyModule = PyModule_Create(pPyModuleDefinition);
	if(! pPyModule){
		DETHROW(deeInvalidAction);
	}
	
	((sModuleData*)PyModule_GetState(pPyModule))->ownerClass = this;
	
// 	PyObject *pyoModulePath = PyUnicode_FromString( "" );
// 	PyDict_SetItemString( PyModule_GetDict( pPyModule ), "__path__", pyoModulePath );
// 	Py_DECREF( pyoModulePath );
	
	if(pParent){
		PyDict_SetItemString(PyModule_GetDict(pParent->GetPyModule()), pName.GetString(), pPyModule);
		
	}else{
		PyDict_SetItemString(PyImport_GetModuleDict(), pName.GetString(), pPyModule);
	}
}
