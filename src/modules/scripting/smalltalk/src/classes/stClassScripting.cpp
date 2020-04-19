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

#include "stClassScripting.h"
#include "../ScriptingSmalltalk.h"

#include <dragengine/common/exceptions.h>



// Class Struct
/////////////////

struct csScripting : public csObject{
	ScriptingSmalltalk *st;
};



// Class stClassScripting
///////////////////////////

// Constructor, destructor
////////////////////////////

stClassScripting::stClassScripting( ScriptingSmalltalk *st ) :
pST( st ),
pSingleton( NULL ){
}

stClassScripting::~stClassScripting(){
	FreeSingleton();
}



// Management
///////////////

ScriptingSmalltalk &stClassScripting::GetSTFromOOP( OOP object ){
	return *( ( csScripting* )OOP_TO_OBJ( object ) )->st;
}

void stClassScripting::CreateSingleton(){
	OOP oopClass = NULL;
	
	oopClass = gst_class_name_to_oop( "DEScripting" );
	if( ! oopClass ){
		DETHROW( deeInvalidParam );
	}
	
	pSingleton = pST->CreateNewObject( oopClass, sizeof( csScripting ) - sizeof( csObject ) );
	if( ! pSingleton || pSingleton == pST->GetNil() ){
		DETHROW( deeOutOfMemory );
	}
	
	gst_register_oop( pSingleton );
	
	csScripting &csobj = *( ( csScripting* )OOP_TO_OBJ( pSingleton ) );
	csobj.st = pST;
}

void stClassScripting::FreeSingleton(){
	if( pSingleton ){
		gst_unregister_oop( pSingleton );
		pSingleton = NULL;
	}
}
