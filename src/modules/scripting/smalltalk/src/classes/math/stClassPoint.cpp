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

#include "stClassPoint.h"
#include "../stClassScripting.h"
#include "../../ScriptingSmalltalk.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class Struct
/////////////////

struct csPointClass : public csClassObject{
	OOP scripting;
};

struct csPoint : public csObject{
	stClassPoint *clsPoint;
	decPoint point;
};



// Class stClassPoint
/////////////////////////

// Constructor, destructor
////////////////////////////

stClassPoint::stClassPoint( ScriptingSmalltalk &st ) :
pST( st ){
	gst_define_cfunc( "DEPoint.new", ( void* )ccNew );
	gst_define_cfunc( "DEPoint.newXY", ( void* )ccNewXY );
	
	gst_define_cfunc( "DEPoint.x", ( void* )ccX );
	gst_define_cfunc( "DEPoint.y", ( void* )ccY );
	
	gst_define_cfunc( "DEPoint.hash", ( void* )ccHash );
	gst_define_cfunc( "DEPoint.asString", ( void* )ccAsString );
}

stClassPoint::~stClassPoint(){
}



// Management
///////////////

void stClassPoint::SetUpLinks(){
	pOOPClass = gst_class_name_to_oop( "DEPoint" );
	if( ! pOOPClass ){
		DETHROW( deeInvalidParam );
	}
	
	csPointClass &csclass = *( ( csPointClass* )OOP_TO_OBJ( pOOPClass ) );
	csclass.scripting = pST.GetClassScripting()->GetSingleton();
}

const decPoint &stClassPoint::OOPToPoint( OOP object ) const{
	if( object == pST.GetNil() ){
		DETHROW( deeNullPointer );
	}
	
	if( gst_get_object_class( object ) != pOOPClass ){
		DETHROW( deeInvalidAction );
	}
	return ( ( csPoint* )OOP_TO_OBJ( object ) )->point;
}

OOP stClassPoint::PointToOOP( const decPoint &vector ){
	OOP oopVector = NULL;
	
	oopVector = pST.CreateNewObjectWithInit( pOOPClass, sizeof( csPoint ) - sizeof( csObject ) );
	if( ! oopVector || oopVector == pST.GetNil() ){
		DETHROW( deeOutOfMemory );
	}
	
	csPoint &csobject = *( ( csPoint* )OOP_TO_OBJ( oopVector ) );
	csobject.clsPoint = this;
	csobject.point = vector;
	
	return oopVector;
}



// cCall Methods
//////////////////

OOP stClassPoint::ccNew( OOP self ){
	const csPointClass &csclass = *( ( csPointClass* )OOP_TO_OBJ( self ) );
	const ScriptingSmalltalk &st = stClassScripting::GetSTFromOOP( csclass.scripting );
	OOP result = NULL;
	
	try{
		result = st.GetClassPoint()->PointToOOP( decPoint() );
		
	}catch( const deException &e ){
		result = st.GetNil();
		e.PrintError();
	}
	
	return result;
}

OOP stClassPoint::ccNewXY( OOP self, int x, int y ){
	const csPointClass &csclass = *( ( csPointClass* )OOP_TO_OBJ( self ) );
	const ScriptingSmalltalk &st = stClassScripting::GetSTFromOOP( csclass.scripting );
	OOP result = NULL;
	
	try{
		result = st.GetClassPoint()->PointToOOP( decPoint( x, y ) );
		
	}catch( const deException &e ){
		result = st.GetNil();
		e.PrintError();
	}
	
	return result;
}



int stClassPoint::ccX( OOP self ){
	const csPoint &cscanvas = *( ( csPoint* )OOP_TO_OBJ( self ) );
	return cscanvas.point.x;
}

int stClassPoint::ccY( OOP self ){
	const csPoint &cscanvas = *( ( csPoint* )OOP_TO_OBJ( self ) );
	return cscanvas.point.y;
}



int stClassPoint::ccHash( OOP self ){
	const csPoint &cscanvas = *( ( csPoint* )OOP_TO_OBJ( self ) );
	
	return cscanvas.point.x * 10000 + cscanvas.point.y;
}

OOP stClassPoint::ccAsString( OOP self ){
	const csPoint &cscanvas = *( ( csPoint* )OOP_TO_OBJ( self ) );
	
	char buffer[ 50 ];
	sprintf( ( char* )buffer, "(%i,%i)", cscanvas.point.x, cscanvas.point.y );
	
	return gst_string_to_oop( buffer );
}
