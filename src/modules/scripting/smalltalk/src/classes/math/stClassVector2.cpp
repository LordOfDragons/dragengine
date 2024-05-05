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

#include "stClassVector2.h"
#include "../stClassScripting.h"
#include "../../ScriptingSmalltalk.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class Struct
/////////////////

struct csVector2Class : public csClassObject{
	OOP scripting;
};

struct csVector2 : public csObject{
	stClassVector2 *clsVec2;
	decVector2 vector;
};



// Class stClassVector2
/////////////////////////

// Constructor, destructor
////////////////////////////

stClassVector2::stClassVector2( ScriptingSmalltalk &st ) :
pST( st ){
	gst_define_cfunc( "DEVector2.new", ( void* )ccNew );
	gst_define_cfunc( "DEVector2.newXY", ( void* )ccNewXY );
	
	gst_define_cfunc( "DEVector2.x", ( void* )ccX );
	gst_define_cfunc( "DEVector2.y", ( void* )ccY );
	
	gst_define_cfunc( "DEVector2.hash", ( void* )ccHash );
	gst_define_cfunc( "DEVector2.asString", ( void* )ccAsString );
}

stClassVector2::~stClassVector2(){
}



// Management
///////////////

void stClassVector2::SetUpLinks(){
	pOOPClass = gst_class_name_to_oop( "DEVector2" );
	if( ! pOOPClass ){
		DETHROW( deeInvalidParam );
	}
	
	csVector2Class &csclass = *( ( csVector2Class* )OOP_TO_OBJ( pOOPClass ) );
	csclass.scripting = pST.GetClassScripting()->GetSingleton();
}

const decVector2 &stClassVector2::OOPToVector2( OOP object ) const{
	if( object == pST.GetNil() ){
		DETHROW( deeNullPointer );
	}
	
	if( gst_get_object_class( object ) != pOOPClass ){
		DETHROW( deeInvalidAction );
	}
	return ( ( csVector2* )OOP_TO_OBJ( object ) )->vector;
}

OOP stClassVector2::Vector2ToOOP( const decVector2 &vector ){
	OOP oopVector = NULL;
	
	oopVector = pST.CreateNewObjectWithInit( pOOPClass, sizeof( csVector2 ) - sizeof( csObject ) );
	if( ! oopVector || oopVector == pST.GetNil() ){
		DETHROW( deeOutOfMemory );
	}
	
	csVector2 &csobject = *( ( csVector2* )OOP_TO_OBJ( oopVector ) );
	csobject.clsVec2 = this;
	csobject.vector = vector;
	
	return oopVector;
}



// cCall Methods
//////////////////

OOP stClassVector2::ccNew( OOP self ){
	const csVector2Class &csclass = *( ( csVector2Class* )OOP_TO_OBJ( self ) );
	const ScriptingSmalltalk &st = stClassScripting::GetSTFromOOP( csclass.scripting );
	OOP result = NULL;
	
	try{
		result = st.GetClassVector2()->Vector2ToOOP( decVector2() );
		
	}catch( const deException &e ){
		result = st.GetNil();
		e.PrintError();
	}
	
	return result;
}

OOP stClassVector2::ccNewXY( OOP self, double x, double y ){
	const csVector2Class &csclass = *( ( csVector2Class* )OOP_TO_OBJ( self ) );
	const ScriptingSmalltalk &st = stClassScripting::GetSTFromOOP( csclass.scripting );
	OOP result = NULL;
	
	try{
		result = st.GetClassVector2()->Vector2ToOOP( decVector2( ( float )x, ( float )y ) );
		
	}catch( const deException &e ){
		result = st.GetNil();
		e.PrintError();
	}
	
	return result;
}



double stClassVector2::ccX( OOP self ){
	const csVector2 &cscanvas = *( ( csVector2* )OOP_TO_OBJ( self ) );
	return ( double )cscanvas.vector.x;
}

double stClassVector2::ccY( OOP self ){
	const csVector2 &cscanvas = *( ( csVector2* )OOP_TO_OBJ( self ) );
	return ( double )cscanvas.vector.y;
}



int stClassVector2::ccHash( OOP self ){
	const csVector2 &cscanvas = *( ( csVector2* )OOP_TO_OBJ( self ) );
	
	return ( int )( cscanvas.vector.x * 1000000.0f )
		+ ( int )( cscanvas.vector.y * 1000.0f );
}

OOP stClassVector2::ccAsString( OOP self ){
	const csVector2 &cscanvas = *( ( csVector2* )OOP_TO_OBJ( self ) );
	
	char buffer[ 50 ];
	sprintf( ( char* )buffer, "(%f,%f)", cscanvas.vector.x, cscanvas.vector.y );
	
	return gst_string_to_oop( buffer );
}
