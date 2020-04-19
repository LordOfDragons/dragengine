/* 
 * Drag[en]gine SmallTalk Script Module
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

#include "stClassGraphicSystem.h"
#include "../graphics/stClassCanvas.h"
#include "../stClassScripting.h"
#include "../../ScriptingSmalltalk.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deCmdLineArgs.h>
#include <dragengine/resources/rendering/deRenderWindow.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/systems/deGraphicSystem.h>
#include <dragengine/systems/modules/graphic/deBaseGraphicModule.h>
#include <dragengine/common/exceptions.h>



// Class Struct
/////////////////

struct csGraSysClass : public csClassObject{
	OOP scripting;
};



// Class stClassGraphicSystem
////////////////////////

// Constructor, destructor
////////////////////////////

stClassGraphicSystem::stClassGraphicSystem( ScriptingSmalltalk &st ) :
pST( st ){
	gst_define_cfunc( "DEGraphicSystem.getWindowWidth", ( void* )ccGetWindowWidth );
	gst_define_cfunc( "DEGraphicSystem.getWindowHeight", ( void* )ccGetWindowHeight );
	gst_define_cfunc( "DEGraphicSystem.setWindowSize", ( void* )ccSetWindowSize );
	gst_define_cfunc( "DEGraphicSystem.setWindowTitle", ( void* )ccSetWindowTitle );
	gst_define_cfunc( "DEGraphicSystem.getPrimaryCanvas", ( void* )ccGetPrimaryCanvas );
}

stClassGraphicSystem::~stClassGraphicSystem(){
}



// Management
///////////////

void stClassGraphicSystem::SetUpLinks(){
	OOP oopClass = NULL;
	
	oopClass = gst_class_name_to_oop( "DEGraphicSystem" );
	if( ! oopClass ){
		DETHROW( deeInvalidParam ); // GraphicSystem.st missing
	}
	
	csGraSysClass &csclass = *( ( csGraSysClass* )OOP_TO_OBJ( oopClass ) );
	csclass.scripting = pST.GetClassScripting()->GetSingleton();
}



// cCall Methods
//////////////////

int stClassGraphicSystem::ccGetWindowWidth( OOP self ){
	const csGraSysClass &csclass = *( ( csGraSysClass* )OOP_TO_OBJ( self ) );
	const ScriptingSmalltalk &st = stClassScripting::GetSTFromOOP( csclass.scripting );
	const deGraphicSystem &grasys = *st.GetGameEngine()->GetGraphicSystem();
	
	return grasys.GetRenderWindow()->GetWidth();
}

int stClassGraphicSystem::ccGetWindowHeight( OOP self ){
	const csGraSysClass &csclass = *( ( csGraSysClass* )OOP_TO_OBJ( self ) );
	const ScriptingSmalltalk &st = stClassScripting::GetSTFromOOP( csclass.scripting );
	const deGraphicSystem &grasys = *st.GetGameEngine()->GetGraphicSystem();
	
	return grasys.GetRenderWindow()->GetHeight();
}

void stClassGraphicSystem::ccSetWindowSize( OOP self, int width, int height ){
	const csGraSysClass &csclass = *( ( csGraSysClass* )OOP_TO_OBJ( self ) );
	const ScriptingSmalltalk &st = stClassScripting::GetSTFromOOP( csclass.scripting );
	const deGraphicSystem &grasys = *st.GetGameEngine()->GetGraphicSystem();
	
	grasys.GetRenderWindow()->SetSize( width, height );
}

void stClassGraphicSystem::ccSetWindowTitle( OOP self, const char *title ){
	const csGraSysClass &csclass = *( ( csGraSysClass* )OOP_TO_OBJ( self ) );
	const ScriptingSmalltalk &st = stClassScripting::GetSTFromOOP( csclass.scripting );
	const deGraphicSystem &grasys = *st.GetGameEngine()->GetGraphicSystem();
	
	grasys.GetRenderWindow()->SetTitle( title );
}

OOP stClassGraphicSystem::ccGetPrimaryCanvas( OOP self ){
	const csGraSysClass &csclass = *( ( csGraSysClass* )OOP_TO_OBJ( self ) );
	const ScriptingSmalltalk &st = stClassScripting::GetSTFromOOP( csclass.scripting );
	const deGraphicSystem &grasys = *st.GetGameEngine()->GetGraphicSystem();
	
	try{
		return st.GetClassCanvas()->CanvasToOOP( grasys.GetRenderWindow()->GetCanvasView() );
		
	}catch( const deException &e ){
		e.PrintError();
		return st.GetNil();
	}
}
