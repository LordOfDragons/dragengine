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
