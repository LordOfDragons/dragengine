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

#include "spModuleGraphicSystem.h"
#include "../graphics/spTypeCanvas.h"
#include "../../ScriptingPython.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/rendering/deRenderWindow.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/systems/deGraphicSystem.h>
#include <dragengine/common/exceptions.h>



// Class spModuleGraphicSystem
////////////////////////////////

// Constructor, destructor
////////////////////////////

spModuleGraphicSystem::spModuleGraphicSystem(ScriptingPython &sp) : spBaseModule(sp, "GraphicSystem"){
	SetDocumentation("Drag[en]gine Graphic System");
	SetParent(sp.GetNamespaceDragengine());
	
	AddMethod("getWindowWidth", (PyCFunction)cfGetWindowWidth, METH_NOARGS, "returns width in pixel of the render window.");
	AddMethod("getWindowHeight", (PyCFunction)cfGetWindowHeight, METH_NOARGS, "returns height in pixel of the render window.");
	AddMethod("setWindowGeometry", (PyCFunction)cfSetWindowGeometry, METH_VARARGS, "sets the window geometry.");
	AddMethod("setWindowTitle", (PyCFunction)cfSetWindowTitle, METH_VARARGS, "sets the window title.");
	AddMethod("getPrimaryCanvas", (PyCFunction)cfGetPrimaryCanvas, METH_NOARGS, "Primary canvas view.");
}

spModuleGraphicSystem::~spModuleGraphicSystem(){
}



// Management
///////////////




// Functions
//////////////

// int getWindowWidth()
PyObject *spModuleGraphicSystem::cfGetWindowWidth(PyObject *myself){
	spModuleGraphicSystem &clsModule = *((spModuleGraphicSystem*)GetOwnerClass(myself));
	deGraphicSystem &grasys = *clsModule.GetSP().GetGameEngine()->GetGraphicSystem();
	
	return Py_BuildValue("i", grasys.GetRenderWindow()->GetWidth());
}

// int getWindowHeight()
PyObject *spModuleGraphicSystem::cfGetWindowHeight(PyObject *myself){
	spModuleGraphicSystem &clsModule = *((spModuleGraphicSystem*)GetOwnerClass(myself));
	deGraphicSystem &grasys = *clsModule.GetSP().GetGameEngine()->GetGraphicSystem();
	
	return Py_BuildValue("i", grasys.GetRenderWindow()->GetHeight());
}

// void setWindowGeometry( int width, int height, bool fullScreen )
PyObject *spModuleGraphicSystem::cfSetWindowGeometry(PyObject *myself, PyObject *args){
	spModuleGraphicSystem &clsModule = *((spModuleGraphicSystem*)GetOwnerClass(myself));
	deGraphicSystem &grasys = *clsModule.GetSP().GetGameEngine()->GetGraphicSystem();
	int width, height;
	bool fullScreen;
	
	if(!PyArg_ParseTuple(args, "iib", &width, &height, &fullScreen)){
		return NULL;
	}
	
	grasys.GetRenderWindow()->SetSize(width, height);
	grasys.GetRenderWindow()->SetFullScreen(fullScreen);
	
	Py_INCREF(Py_None);
	return Py_None;
}

// void setWindowTitle( String title )
PyObject *spModuleGraphicSystem::cfSetWindowTitle(PyObject *myself, PyObject *args){
	spModuleGraphicSystem &clsModule = *((spModuleGraphicSystem*)GetOwnerClass(myself));
	deGraphicSystem &grasys = *clsModule.GetSP().GetGameEngine()->GetGraphicSystem();
	char *title;
	
	if(!PyArg_ParseTuple(args, "s", &title)){
		return NULL;
	}
	
	grasys.GetRenderWindow()->SetTitle(title);
	
	Py_INCREF(Py_None);
	return Py_None;
}

// Canvas getPrimaryCanvas()
PyObject *spModuleGraphicSystem::cfGetPrimaryCanvas(PyObject *myself){
	spModuleGraphicSystem &clsModule = *((spModuleGraphicSystem*)GetOwnerClass(myself));
	deGraphicSystem &grasys = *clsModule.GetSP().GetGameEngine()->GetGraphicSystem();
	ScriptingPython &sp = clsModule.GetSP();
	
	return sp.GetTypeCanvas()->ObjectFromCanvas(grasys.GetRenderWindow()->GetCanvasView());
}
