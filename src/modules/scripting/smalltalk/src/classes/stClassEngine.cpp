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

#include "stClassEngine.h"
#include "stClassScripting.h"
#include "../ScriptingSmalltalk.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deCmdLineArgs.h>
#include <dragengine/common/exceptions.h>



// Class Struct
/////////////////

struct csEngineClass : public csClassObject{
	OOP scripting;
};



// Class stClassEngine
////////////////////////

// Constructor, destructor
////////////////////////////

stClassEngine::stClassEngine( ScriptingSmalltalk &st ) :
pST( st ){
	gst_define_cfunc( "DEEngine.quit", ( void* )ccQuit );
	gst_define_cfunc( "DEEngine.getElapsedTime", ( void* )ccElapsedTime );
	gst_define_cfunc( "DEEngine.resetTimers", ( void* )ccResetTimers );
	gst_define_cfunc( "DEEngine.getGame", ( void* )ccGame );
	gst_define_cfunc( "DEEngine.getFPSRate", ( void* )ccFPSRate );
	gst_define_cfunc( "DEEngine.getArguments", ( void* )ccArguments );
}

stClassEngine::~stClassEngine(){
}



// Management
///////////////

void stClassEngine::SetUpLinks(){
	OOP oopClass = gst_class_name_to_oop( "DEEngine" );
	if( ! oopClass ){
		DETHROW( deeInvalidParam );
	}
	
	csEngineClass &csclass = *( ( csEngineClass* )OOP_TO_OBJ( oopClass ) );
	csclass.scripting = pST.GetClassScripting()->GetSingleton();
}



// cCall Methods
//////////////////

void stClassEngine::ccQuit( OOP self ){
	csEngineClass &csclass = *( ( csEngineClass* )OOP_TO_OBJ( self ) );
	const ScriptingSmalltalk &st = stClassScripting::GetSTFromOOP( csclass.scripting );
	
	st.GetGameEngine()->Quit();
}

float stClassEngine::ccElapsedTime( OOP self ){
	csEngineClass &csclass = *( ( csEngineClass* )OOP_TO_OBJ( self ) );
	const ScriptingSmalltalk &st = stClassScripting::GetSTFromOOP( csclass.scripting );
	
	return st.GetGameEngine()->GetElapsedTime();
}

void stClassEngine::ccResetTimers( OOP self ){
	csEngineClass &csclass = *( ( csEngineClass* )OOP_TO_OBJ( self ) );
	const ScriptingSmalltalk &st = stClassScripting::GetSTFromOOP( csclass.scripting );
	
	st.GetGameEngine()->ResetTimers();
}

OOP stClassEngine::ccGame( OOP self ){
	csEngineClass &csclass = *( ( csEngineClass* )OOP_TO_OBJ( self ) );
	const ScriptingSmalltalk &st = stClassScripting::GetSTFromOOP( csclass.scripting );
	
	return st.GetObjectGame();
}

int stClassEngine::ccFPSRate( OOP self ){
	csEngineClass &csclass = *( ( csEngineClass* )OOP_TO_OBJ( self ) );
	const ScriptingSmalltalk &st = stClassScripting::GetSTFromOOP( csclass.scripting );
	
	return st.GetGameEngine()->GetFPSRate();
}

OOP stClassEngine::ccArguments( OOP self ){
	csEngineClass &csclass = *( ( csEngineClass* )OOP_TO_OBJ( self ) );
	ScriptingSmalltalk &st = stClassScripting::GetSTFromOOP( csclass.scripting );
	
	const deCmdLineArgs &args = *st.GetGameEngine()->GetArguments();
	OOP selectorAdd = st.GetSelectorAdd();
	int a, acount = args.GetCount();
	OOP oopArgs = NULL;
	
	try{
		oopArgs = st.CreateNewObject( st.GetClassOrderedCollection() );
		gst_register_oop( oopArgs );
		
		for( a=0; a<acount; a++ ){
			gst_perform_with( oopArgs, selectorAdd, gst_string_to_oop( args.GetArgument( a ) ) );
		}
		
		gst_unregister_oop( oopArgs );
		
	}catch( const deException &e ){
		if( oopArgs ){
			gst_unregister_oop( oopArgs );
		}
		e.PrintError();
		return st.GetNil();
	}
	
	return oopArgs;
}
