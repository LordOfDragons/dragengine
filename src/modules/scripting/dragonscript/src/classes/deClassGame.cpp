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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deClassGame.h"
#include "input/deClassInputEvent.h"
#include "../deScriptingDragonScript.h"
#include "../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deCmdLineArgs.h>

#include <libdscript/exceptions.h>
#include <libdscript/libdscript.h>



// Native functions
/////////////////////



// public func new()
deClassGame::nfNew::nfNew( const sInitData &init ) :
dsFunction( init.clsGame, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
}
void deClassGame::nfNew::RunFunction( dsRunTime*, dsValue* ){
}



// public func void inputEvent( InputEvent event )
deClassGame::nfInputEvent::nfInputEvent( const sInitData &init ) :
dsFunction( init.clsGame, "inputEvent", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInputEvent ); // event
}
void deClassGame::nfInputEvent::RunFunction( dsRunTime*, dsValue* ){
}



// public func void initGame()
deClassGame::nfInitGame::nfInitGame( const sInitData &init ) :
dsFunction( init.clsGame, "initGame", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassGame::nfInitGame::RunFunction( dsRunTime*, dsValue* ){
}

// ppublic func void cleanUp()
deClassGame::nfCleanUp::nfCleanUp( const sInitData &init ) :
dsFunction( init.clsGame, "cleanUp", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassGame::nfCleanUp::RunFunction( dsRunTime*, dsValue* ){
}

// public func void onFrameUpdate()
deClassGame::nfOnFrameUpdate::nfOnFrameUpdate( const sInitData &init ) :
dsFunction( init.clsGame, "onFrameUpdate", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassGame::nfOnFrameUpdate::RunFunction( dsRunTime*, dsValue* ){
}

// public func void onResizeRenderWindow()
deClassGame::nfOnResizeRenderWindow::nfOnResizeRenderWindow( const sInitData &init ) :
dsFunction( init.clsGame, "onResizeRenderWindow", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassGame::nfOnResizeRenderWindow::RunFunction( dsRunTime*, dsValue* ){
}

// public func void onAppActivate()
deClassGame::nfOnAppActivate::nfOnAppActivate( const sInitData &init ) :
dsFunction( init.clsGame, "onAppActivate", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassGame::nfOnAppActivate::RunFunction( dsRunTime*, dsValue* ){
}



// Arguments
//////////////

// public func int getArgumentCount()
deClassGame::nfGetArgumentCount::nfGetArgumentCount( const sInitData &init ) :
dsFunction( init.clsGame, "getArgumentCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}
void deClassGame::nfGetArgumentCount::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassGame* )GetOwnerClass() )->GetDS();
	
	rt->PushInt( ds.GetGameEngine()->GetArguments()->GetCount() );
}

// public func String getArgumentAt( int index )
deClassGame::nfGetArgumentAt::nfGetArgumentAt( const sInitData &init ) :
dsFunction( init.clsGame, "getArgumentAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsString ){
	p_AddParameter( init.clsInteger ); // index
}
void deClassGame::nfGetArgumentAt::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassGame* )GetOwnerClass() )->GetDS();
	
	const int index = rt->GetValue( 0 )->GetInt();
	
	rt->PushString( ds.GetGameEngine()->GetArguments()->GetArgument( index ) );
}



// class deClassGame
/////////////////////

// Constructor
////////////////

deClassGame::deClassGame( deScriptingDragonScript &ds ) :
dsClass( "Game", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE ),
pDS( ds )
{
	GetParserInfo()->SetParent( DENS_DRAGENGINE );
	GetParserInfo()->SetBase( "Object" );
	p_SetNativeDataSize( 0 );
}

deClassGame::~deClassGame(){
}



// Management
///////////////

void deClassGame::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	init.clsGame = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInteger = engine->GetClassInt();
	init.clsString = engine->GetClassString();
	init.clsInputEvent = pDS.GetClassInputEvent();
	
	AddFunction( new nfNew( init ) );
	AddFunction( new nfInputEvent( init ) );
	AddFunction( new nfInitGame( init ) );
	AddFunction( new nfCleanUp( init ) );
	AddFunction( new nfOnFrameUpdate( init ) );
	AddFunction( new nfOnResizeRenderWindow( init ) );
	AddFunction( new nfOnAppActivate( init ) );
	
	AddFunction( new nfGetArgumentCount( init ) );
	AddFunction( new nfGetArgumentAt( init ) );
	
	CalcMemberOffsets();
}
