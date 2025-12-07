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

#include "spBaseType.h"
#include "spBaseModule.h"
#include "../ScriptingPython.h"

#include <dragengine/common/exceptions.h>



// Initializers
/////////////////

/*
static const PyTypeObject vTypeInitializer = {
	PyObject_HEAD_INIT(NULL)
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0
};
*/


// Native Data
////////////////

struct sObjectType : public PyTypeObject{
	spBaseType *ownerClass;
};

struct sObjectData{
	PyObject_HEAD
};



// Class spBaseType
/////////////////////

// Constructor, destructor
////////////////////////////

spBaseType::spBaseType(ScriptingPython &sp, const char *name) :
pSP(sp)
{
	pParent = NULL;
	pName = name;
	pNativeDataSize = 0;
	pPrivateNew = false;
	
	pFunctions = NULL;
	pFunctionCount = 0;
	
	pPyMethodDefs = NULL;
	pPyType = NULL;
}

spBaseType::~spBaseType(){
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

void spBaseType::SetDocumentation(const char *documentation){
	pDocumentation = documentation;
}

void spBaseType::SetParent(spBaseModule *parent){
	pParent = parent;
}

void spBaseType::SetNativeDataSize(int size){
	if(size < 0){
		DETHROW(deeInvalidParam);
	}
	
	pNativeDataSize = size;
}

void spBaseType::SetPrivateNew(bool privateNew){
	pPrivateNew = privateNew;
}

void spBaseType::AddMethod(const char *name, PyCFunction pyFunction, int argumentType, const char *documentation){
	if(!name || !pyFunction || !documentation){
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



spBaseType *spBaseType::GetOwnerClass(PyObject *object){
	if(!object){
		DETHROW(deeNullPointer);
	}
	
	return ((sObjectType*)object->ob_type)->ownerClass;
}

void *spBaseType::GetObjectData(PyObject *object){
	if(!object){
		DETHROW(deeNullPointer);
	}
	
	return (char*)object + sizeof(sObjectData);
}

PyObject *spBaseType::CreatePyObject(){
	return pPyType->tp_alloc(pPyType, 0);
}



void spBaseType::PythonCreate(){
	pUpdateFullName();
	
	//pSP.LogInfoFormat( "Type %s PythonCreate", pFullTypeName.GetString() );
	
	pCreateMethodsDefinition();
	pCreateType();
}

void spBaseType::PythonCleanUp(){
	//pSP.LogInfoFormat( "Type %s PythonCleanUp", pFullTypeName.GetString() );
	
	if(pPyType){
		Py_DECREF(pPyType);
		pPyType = NULL;
	}
}



void spBaseType::Constructor(PyObject *myself, PyObject *args, PyObject *kwds){
}

void spBaseType::Destructor(PyObject *myself){
}



// Private Functions
//////////////////////

PyObject *spBaseType::cfNew(PyTypeObject *type, PyObject *args, PyObject *kwds){
	if(!type){
		PyErr_SetString(PyExc_RuntimeError, "Tried to create a NULL type");
		return NULL;
	}
	
	const spBaseType &ownerClass = *(((sObjectType*)type)->ownerClass);
	
	if(ownerClass.GetPrivateNew()){
		PyErr_SetString(PyExc_RuntimeError, "Object creation is private");
		return NULL;
	}
	
	//ownerClass.GetSP()->LogInfoFormat( "Type %s cfNew", ownerClass.GetFullTypeName().GetString() );
	
	PyObject * const myself = type->tp_alloc(type, 0);
	if(!myself){
		if(PyErr_Occurred()){
			PyErr_Print();
		}
		PyErr_SetString(PyExc_RuntimeError, "Failed to create object");
	}
	
	return myself;
}

int spBaseType::cfInit(PyObject *myself, PyObject *args, PyObject *kwds){
	if(!myself){
		PyErr_SetString(PyExc_RuntimeError, "Tried to init a NULL object");
		return 1;
	}
	
	try{
		spBaseType &ownerClass = *GetOwnerClass(myself);
		//ownerClass.GetSP()->LogInfoFormat( "Type %s cfInit myself=%p", ownerClass.GetFullTypeName().GetString(), myself );
		
		ownerClass.Constructor(myself, args, kwds);
		
	}catch(const deException &e){
		PyErr_SetString(PyExc_RuntimeError, e.GetDescription());
		return 1;
	}
	
	return 0;
}

void spBaseType::cfDealoc(PyObject *myself){
	if(myself){
		try{
			spBaseType &ownerClass = *GetOwnerClass(myself);
			//ownerClass.GetSP()->LogInfoFormat( "Type %s cfDealoc myself=%p", ownerClass.GetFullTypeName().GetString(), myself );
			
			ownerClass.Destructor(myself);
			
		}catch(const deException &e){
			PyErr_SetString(PyExc_RuntimeError, e.GetDescription());
		}
		
		myself->ob_type->tp_free(myself);
	}
}



void spBaseType::pUpdateFullName(){
	spBaseModule *module = pParent;
	
	pFullTypeName = pName;
	while(module){
		pFullTypeName = module->GetName() + "." + pFullTypeName;
		module = module->GetParent();
	}
}

void spBaseType::pCreateMethodsDefinition(){
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

void spBaseType::pCreateType(){
	if(pPyType){
		Py_DECREF(pPyType);
		pPyType = NULL;
	}
	
	//pSP.LogInfoFormat( "Type %s create type", pFullTypeName.GetString() );
	
	pPyType = new sObjectType;
	memset(pPyType, '\0', sizeof(sObjectType));
	//memcpy( pPyType, &vTypeInitializer, sizeof( PyTypeObject ) );
	
	pPyType->tp_name = pFullTypeName.GetString();
	pPyType->tp_basicsize = sizeof(sObjectData) + pNativeDataSize;
	pPyType->tp_flags = Py_TPFLAGS_DEFAULT;
	pPyType->tp_doc = pDocumentation.GetString();
	pPyType->tp_methods = pPyMethodDefs;
	pPyType->tp_new = (newfunc)spBaseType::cfNew;
	pPyType->tp_init = (initproc)spBaseType::cfInit;
	pPyType->tp_dealloc = (destructor)spBaseType::cfDealoc;
	((sObjectType*)pPyType)->ownerClass = this;
	
	if(PyType_Ready(pPyType) < 0){
		DETHROW(deeInvalidAction);
	}
	
	if(pParent){
		PyDict_SetItemString(PyModule_GetDict(pParent->GetPyModule()), pName.GetString(), (PyObject*)pPyType);
		
	}else{
		PyDict_SetItemString(PyImport_GetModuleDict(), pName.GetString(), (PyObject*)pPyType);
	}
}
