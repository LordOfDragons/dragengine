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

#include "spTypeWorld.h"
#include "../spBaseModule.h"
#include "../../ScriptingPython.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/world/deWorldManager.h>
#include <dragengine/common/exceptions.h>



// Python Binding
///////////////////

struct sObjectData{
	deWorld *world;
};



// Class spTypeWorld
////////////////////////

// Constructor, destructor
////////////////////////////

spTypeWorld::spTypeWorld(ScriptingPython &sp) : spBaseType(sp, "World"){
	SetDocumentation("World resource");
	SetNativeDataSize(sizeof(sObjectData));
	SetParent(sp.GetNamespaceScenery());
	
	AddMethod("update", (PyCFunction)cfUpdate, METH_VARARGS, "Updates the world.");
}

spTypeWorld::~spTypeWorld(){
	PythonCleanUp();
}



// Management
///////////////

deWorld *spTypeWorld::WorldFromObject(PyObject *object){
	if(object){
		return ((sObjectData*)GetObjectData(object))->world;
		
	}else{
		return NULL;
	}
}

PyObject *spTypeWorld::ObjectFromWorld(deWorld *world){
	if(world){
		PyObject *pyoWorld = CreatePyObject();
		
		sObjectData &od = *((sObjectData*)GetObjectData(pyoWorld));
		od.world = world;
		world->AddReference();
		
		return pyoWorld;
		
	}else{
		Py_INCREF(Py_None);
		return Py_None;
	}
}



void spTypeWorld::Constructor(PyObject *myself, PyObject *args, PyObject *kwds){
	sObjectData &od = *((sObjectData*)GetObjectData(myself));
	
	od.world = NULL;
	
	od.world = GetSP().GetGameEngine()->GetWorldManager()->CreateWorld();
}

void spTypeWorld::Destructor(PyObject *myself) {
	sObjectData &od = *((sObjectData*)GetObjectData(myself));
	
	if(od.world){
		od.world->FreeReference();
		od.world = NULL;
	}
}



// Functions
//////////////

PyObject *spTypeWorld::cfUpdate(PyObject *myself, PyObject *args){
	deWorld &world = *(((sObjectData*)GetObjectData(myself))->world);
	float elapsed;
	
	if(! PyArg_ParseTuple(args, "f", &elapsed)){
		return NULL;
	}
	
	world.Update(elapsed);
	
	Py_INCREF(Py_None);
	return Py_None;
}
