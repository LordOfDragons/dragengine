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

#include "spModuleEngine.h"
#include "../ScriptingPython.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class spModuleEngine
/////////////////////////

// Constructor, destructor
////////////////////////////

spModuleEngine::spModuleEngine( ScriptingPython &sp ) : spBaseModule( sp, "Engine" ){
	SetDocumentation( "Drag[en]gine Game Engine" );
	SetParent( sp.GetNamespaceDragengine() );
	
	AddMethod( "quit", ( PyCFunction )cfQuit, METH_NOARGS, "Quits the game." );
}

spModuleEngine::~spModuleEngine(){
}



// Management
///////////////



// Functions
//////////////

PyObject *spModuleEngine::cfQuit( PyObject *myself ){
	GetOwnerClass( myself )->GetSP().GetGameEngine()->Quit();
	
	Py_INCREF( Py_None );
	return Py_None;
}
