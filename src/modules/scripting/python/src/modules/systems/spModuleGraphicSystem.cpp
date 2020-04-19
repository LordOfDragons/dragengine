/* 
 * Drag[en]gine Python Script Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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

spModuleGraphicSystem::spModuleGraphicSystem( ScriptingPython &sp ) : spBaseModule( sp, "GraphicSystem" ){
	SetDocumentation( "Drag[en]gine Graphic System" );
	SetParent( sp.GetNamespaceDragengine() );
	
	AddMethod( "getWindowWidth", ( PyCFunction )cfGetWindowWidth, METH_NOARGS, "returns width in pixel of the render window." );
	AddMethod( "getWindowHeight", ( PyCFunction )cfGetWindowHeight, METH_NOARGS, "returns height in pixel of the render window." );
	AddMethod( "setWindowGeometry", ( PyCFunction )cfSetWindowGeometry, METH_VARARGS, "sets the window geometry." );
	AddMethod( "setWindowTitle", ( PyCFunction )cfSetWindowTitle, METH_VARARGS, "sets the window title." );
	AddMethod( "getPrimaryCanvas", ( PyCFunction )cfGetPrimaryCanvas, METH_NOARGS, "Primary canvas view." );
}

spModuleGraphicSystem::~spModuleGraphicSystem(){
}



// Management
///////////////




// Functions
//////////////

// int getWindowWidth()
PyObject *spModuleGraphicSystem::cfGetWindowWidth( PyObject *myself ){
	spModuleGraphicSystem &clsModule = *( ( spModuleGraphicSystem* )GetOwnerClass( myself ) );
	deGraphicSystem &grasys = *clsModule.GetSP().GetGameEngine()->GetGraphicSystem();
	
	return Py_BuildValue( "i", grasys.GetRenderWindow()->GetWidth() );
}

// int getWindowHeight()
PyObject *spModuleGraphicSystem::cfGetWindowHeight( PyObject *myself ){
	spModuleGraphicSystem &clsModule = *( ( spModuleGraphicSystem* )GetOwnerClass( myself ) );
	deGraphicSystem &grasys = *clsModule.GetSP().GetGameEngine()->GetGraphicSystem();
	
	return Py_BuildValue( "i", grasys.GetRenderWindow()->GetHeight() );
}

// void setWindowGeometry( int width, int height, bool fullScreen )
PyObject *spModuleGraphicSystem::cfSetWindowGeometry( PyObject *myself, PyObject *args ){
	spModuleGraphicSystem &clsModule = *( ( spModuleGraphicSystem* )GetOwnerClass( myself ) );
	deGraphicSystem &grasys = *clsModule.GetSP().GetGameEngine()->GetGraphicSystem();
	int width, height;
	bool fullScreen;
	
	if( ! PyArg_ParseTuple( args, "iib", &width, &height, &fullScreen ) ){
		return NULL;
	}
	
	grasys.GetRenderWindow()->SetSize( width, height );
	grasys.GetRenderWindow()->SetFullScreen( fullScreen );
	
	Py_INCREF( Py_None );
	return Py_None;
}

// void setWindowTitle( String title )
PyObject *spModuleGraphicSystem::cfSetWindowTitle( PyObject *myself, PyObject *args ){
	spModuleGraphicSystem &clsModule = *( ( spModuleGraphicSystem* )GetOwnerClass( myself ) );
	deGraphicSystem &grasys = *clsModule.GetSP().GetGameEngine()->GetGraphicSystem();
	char *title;
	
	if( ! PyArg_ParseTuple( args, "s", &title ) ){
		return NULL;
	}
	
	grasys.GetRenderWindow()->SetTitle( title );
	
	Py_INCREF( Py_None );
	return Py_None;
}

// Canvas getPrimaryCanvas()
PyObject *spModuleGraphicSystem::cfGetPrimaryCanvas( PyObject *myself ){
	spModuleGraphicSystem &clsModule = *( ( spModuleGraphicSystem* )GetOwnerClass( myself ) );
	deGraphicSystem &grasys = *clsModule.GetSP().GetGameEngine()->GetGraphicSystem();
	ScriptingPython &sp = clsModule.GetSP();
	
	return sp.GetTypeCanvas()->ObjectFromCanvas( grasys.GetRenderWindow()->GetCanvasView() );
}
