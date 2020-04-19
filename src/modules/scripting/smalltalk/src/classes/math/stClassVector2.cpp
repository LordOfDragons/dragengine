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
