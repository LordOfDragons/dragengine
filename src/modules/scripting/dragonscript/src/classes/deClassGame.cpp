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



// func new()
deClassGame::nfNew::nfNew( const sInitData &init ) :
dsFunction( init.clsGame, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
}
void deClassGame::nfNew::RunFunction( dsRunTime*, dsValue* ){
}



// func void inputEvent(InputEvent event)
deClassGame::nfInputEvent::nfInputEvent( const sInitData &init ) :
dsFunction( init.clsGame, "inputEvent", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInputEvent ); // event
}
void deClassGame::nfInputEvent::RunFunction( dsRunTime*, dsValue* ){
}



// func void initGame()
deClassGame::nfInitGame::nfInitGame( const sInitData &init ) :
dsFunction( init.clsGame, "initGame", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassGame::nfInitGame::RunFunction( dsRunTime*, dsValue* ){
}

// pfunc void cleanUp()
deClassGame::nfCleanUp::nfCleanUp( const sInitData &init ) :
dsFunction( init.clsGame, "cleanUp", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassGame::nfCleanUp::RunFunction( dsRunTime*, dsValue* ){
}

// func void onFrameUpdate()
deClassGame::nfOnFrameUpdate::nfOnFrameUpdate( const sInitData &init ) :
dsFunction( init.clsGame, "onFrameUpdate", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassGame::nfOnFrameUpdate::RunFunction( dsRunTime*, dsValue* ){
}

// func void onResizeRenderWindow()
deClassGame::nfOnResizeRenderWindow::nfOnResizeRenderWindow( const sInitData &init ) :
dsFunction( init.clsGame, "onResizeRenderWindow", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassGame::nfOnResizeRenderWindow::RunFunction( dsRunTime*, dsValue* ){
}

// func void onAppActivate()
deClassGame::nfOnAppActivate::nfOnAppActivate( const sInitData &init ) :
dsFunction( init.clsGame, "onAppActivate", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassGame::nfOnAppActivate::RunFunction( dsRunTime*, dsValue* ){
}



// func int getArgumentCount()
deClassGame::nfGetArgumentCount::nfGetArgumentCount( const sInitData &init ) :
dsFunction( init.clsGame, "getArgumentCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}
void deClassGame::nfGetArgumentCount::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassGame* )GetOwnerClass() )->GetDS();
	
	rt->PushInt( ds.GetGameEngine()->GetArguments()->GetCount() );
}

// func String getArgumentAt(int index)
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

// func String getRestartInfo()
deClassGame::nfGetRestartInfo::nfGetRestartInfo( const sInitData &init ) :
dsFunction( init.clsGame, "getRestartInfo", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsString ){
}
void deClassGame::nfGetRestartInfo::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassGame* )GetOwnerClass() )->GetDS();
	rt->PushString( ds.GetRestartInfo() );
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
	AddFunction( new nfGetRestartInfo( init ) );
	
	CalcMemberOffsets();
}
