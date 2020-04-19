/* 
 * Drag[en]gine DragonScript Script Module
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



#include <time.h>
#include <stdio.h>
#include <string.h>

#include "deClassSystem.h"
#include "../../deScriptingDragonScript.h"

#include <libdscript/exceptions.h>



// Native functions
/////////////////////


// public static func void print(string text)
deClassSystem::nfPrint::nfPrint(const sInitData &init) : dsFunction(init.clsSys, "print",
DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid){
	p_AddParameter(init.clsStr); // text
}
void deClassSystem::nfPrint::RunFunction(dsRunTime *RT, dsValue *This){
	deScriptingDragonScript &ds = *( ( ( deClassSystem* )GetOwnerClass() )->GetDS() );
	
	ds.LogInfo( RT->GetValue( 0 )->GetString() );
}




// Class deClassSystem
////////////////////////

// Constructors, destructor
/////////////////////////////

deClassSystem::deClassSystem( deScriptingDragonScript *ds ) :
dsClass( "System", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE ){
	pDS = ds;
	
	GetParserInfo()->SetBase("Object");
	p_SetNativeDataSize( 0 );
	CalcMemberOffsets();
}

deClassSystem::~deClassSystem(){
}



// Management
///////////////

void deClassSystem::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	init.clsSys = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsFlt = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsInt = engine->GetClassInt();
	
	AddFunction( new nfPrint( init ) );
}
