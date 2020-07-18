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
#include <libdscript/libdscript.h>

#include "deClassGraphicSystem.h"
#include "../deClassModuleParameter.h"
#include "../canvas/deClassCanvasView.h"
#include "../math/deClassPoint.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/rendering/deRenderWindow.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/font/deFontManager.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/systems/deGraphicSystem.h>
#include <dragengine/systems/modules/graphic/deBaseGraphicModule.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#include <libdscript/exceptions.h>



// native functions
/////////////////////

// static public func int getWindowWidth()
deClassGraphicSystem::nfGetWindowWidth::nfGetWindowWidth( const sInitData &init ) : dsFunction( init.clsGraSys,
"getWindowWidth", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInt ){
}
void deClassGraphicSystem::nfGetWindowWidth::RunFunction( dsRunTime *RT, dsValue *This ){
	deClassGraphicSystem *clsGraSys = (deClassGraphicSystem*)GetOwnerClass();
	deGraphicSystem *graSys = clsGraSys->GetDS().GetGameEngine()->GetGraphicSystem();
	RT->PushInt( graSys->GetRenderWindow()->GetWidth() );
}

// static public func int getWindowHeight()
deClassGraphicSystem::nfGetWindowHeight::nfGetWindowHeight( const sInitData &init ) : dsFunction( init.clsGraSys,
"getWindowHeight", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInt){
}
void deClassGraphicSystem::nfGetWindowHeight::RunFunction( dsRunTime *RT, dsValue *This ){
	deClassGraphicSystem *clsGraSys = (deClassGraphicSystem*)GetOwnerClass();
	deGraphicSystem *graSys = clsGraSys->GetDS().GetGameEngine()->GetGraphicSystem();
	RT->PushInt( graSys->GetRenderWindow()->GetHeight() );
}

// static public func Point getWindowSize()
deClassGraphicSystem::nfGetWindowSize::nfGetWindowSize( const sInitData &init ) : dsFunction( init.clsGraSys,
"getWindowSize", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsPoint ){
}
void deClassGraphicSystem::nfGetWindowSize::RunFunction( dsRunTime *RT, dsValue *This ){
	const deClassGraphicSystem &clsGraSys = *( ( deClassGraphicSystem* )GetOwnerClass() );
	const deGraphicSystem &graSys = *clsGraSys.GetDS().GetGameEngine()->GetGraphicSystem();
	const deRenderWindow &renderWindow = *graSys.GetRenderWindow();
	const decPoint size( renderWindow.GetWidth(), renderWindow.GetHeight() );
	
	clsGraSys.GetDS().GetClassPoint()->PushPoint( RT, size );
}

// public static func void setWindowGeometry( int width, int height, bool fullScreen )
deClassGraphicSystem::nfSetWindowGeometry::nfSetWindowGeometry( const sInitData &init ) : dsFunction( init.clsGraSys,
"setWindowGeometry", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid ){
	p_AddParameter( init.clsInt ); // width
	p_AddParameter( init.clsInt ); // height
	p_AddParameter( init.clsBool ); // fullScreen
}
void deClassGraphicSystem::nfSetWindowGeometry::RunFunction( dsRunTime *RT, dsValue *This ){
	deClassGraphicSystem *clsGraSys = ( deClassGraphicSystem* )GetOwnerClass();
	deGraphicSystem *graSys = clsGraSys->GetDS().GetGameEngine()->GetGraphicSystem();
	const int width = RT->GetValue( 0 )->GetInt();
	const int height = RT->GetValue( 1 )->GetInt();
	const bool fullScreen = RT->GetValue( 2 )->GetBool();
	
	graSys->GetRenderWindow()->SetSize( width, height );
	graSys->GetRenderWindow()->SetFullScreen( fullScreen );
}

// public static func void setWindowTitle( String title )
deClassGraphicSystem::nfSetWindowTitle::nfSetWindowTitle( const sInitData &init ) : dsFunction( init.clsGraSys,
"setWindowTitle", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid ){
	p_AddParameter( init.clsStr ); // title
}
void deClassGraphicSystem::nfSetWindowTitle::RunFunction( dsRunTime *RT, dsValue *This ){
	deClassGraphicSystem *clsGraSys = ( deClassGraphicSystem* )GetOwnerClass();
	deGraphicSystem *graSys = clsGraSys->GetDS().GetGameEngine()->GetGraphicSystem();
	graSys->GetRenderWindow()->SetTitle( RT->GetValue( 0 )->GetString() );
}

// public static func CanvasView getPrimaryCanvas()
deClassGraphicSystem::nfGetPrimaryCanvas::nfGetPrimaryCanvas( const sInitData &init ) : dsFunction( init.clsGraSys,
"getPrimaryCanvas", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsCView ){
}
void deClassGraphicSystem::nfGetPrimaryCanvas::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deClassGraphicSystem &clsGraSys = *( ( deClassGraphicSystem* )GetOwnerClass() );
	const deGraphicSystem &graSys = *clsGraSys.GetDS().GetGameEngine()->GetGraphicSystem();
	
	clsGraSys.GetDS().GetClassCanvasView()->PushCanvas( rt, graSys.GetRenderWindow()->GetCanvasView() );
}



// parameters

// public static func int getParameterCount()
deClassGraphicSystem::nfGetParameterCount::nfGetParameterCount(const sInitData &init) : dsFunction(init.clsGraSys,
"getParameterCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInt){
}
void deClassGraphicSystem::nfGetParameterCount::RunFunction(dsRunTime *RT, dsValue *This){
	deClassGraphicSystem *clsGraSys = (deClassGraphicSystem*)GetOwnerClass();
	deGraphicSystem *graSys = clsGraSys->GetDS().GetGameEngine()->GetGraphicSystem();
	deBaseGraphicModule *module = graSys->GetActiveModule();
	RT->PushInt( module->GetParameterCount() );
}

// public static func ModuleParameter getParameterInfo(int index)
deClassGraphicSystem::nfGetParameterInfo::nfGetParameterInfo(const sInitData &init) : dsFunction(init.clsGraSys,
"getParameterInfo", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsModPar){
	p_AddParameter(init.clsInt); // index
}
void deClassGraphicSystem::nfGetParameterInfo::RunFunction(dsRunTime *RT, dsValue *This){
	deClassGraphicSystem *clsGraSys = (deClassGraphicSystem*)GetOwnerClass();
	deGraphicSystem *graSys = clsGraSys->GetDS().GetGameEngine()->GetGraphicSystem();
	deBaseGraphicModule *module = graSys->GetActiveModule();
	int index = RT->GetValue(0)->GetInt();
	// check
	if(index < 0 || index >= module->GetParameterCount()) DSTHROW(dueInvalidParam);
	// retrieve information
	clsGraSys->GetDS().GetClassModuleParameter()->PushParameter(RT, module, index);
}

// public static func ModuleParameter getParameterInfo( String name )
deClassGraphicSystem::nfGetParameterInfo2::nfGetParameterInfo2( const sInitData &init ) :
dsFunction( init.clsGraSys, "getParameterInfo", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsModPar ){
	p_AddParameter( init.clsStr ); // name
}
void deClassGraphicSystem::nfGetParameterInfo2::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deScriptingDragonScript &ds = ( ( deClassGraphicSystem* )GetOwnerClass() )->GetDS();
	const deGraphicSystem &graSys = *ds.GetGameEngine()->GetGraphicSystem();
	deBaseGraphicModule * const module = graSys.GetActiveModule();
	
	ds.GetClassModuleParameter()->PushParameter( rt, module,
		module->IndexOfParameterNamed( rt->GetValue( 0 )->GetString() ) );
}

// public static func String getParameterValue(String name)
deClassGraphicSystem::nfGetParameterValue::nfGetParameterValue(const sInitData &init) : dsFunction(init.clsGraSys,
"getParameterValue", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsStr){
	p_AddParameter(init.clsStr); // name
}
void deClassGraphicSystem::nfGetParameterValue::RunFunction(dsRunTime *RT, dsValue *This){
	deClassGraphicSystem *clsGraSys = (deClassGraphicSystem*)GetOwnerClass();
	deGraphicSystem *graSys = clsGraSys->GetDS().GetGameEngine()->GetGraphicSystem();
	deBaseGraphicModule *module = graSys->GetActiveModule();
	RT->PushString( module->GetParameterValue(RT->GetValue( 0 )->GetString()) );
}

// public static func void setParameterValue(String name, String value)
deClassGraphicSystem::nfSetParameterValue::nfSetParameterValue(const sInitData &init) : dsFunction(init.clsGraSys,
"setParameterValue", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid){
	p_AddParameter(init.clsStr); // name
	p_AddParameter(init.clsStr); // value
}
void deClassGraphicSystem::nfSetParameterValue::RunFunction(dsRunTime *RT, dsValue *This){
	deClassGraphicSystem *clsGraSys = (deClassGraphicSystem*)GetOwnerClass();
	deGraphicSystem *graSys = clsGraSys->GetDS().GetGameEngine()->GetGraphicSystem();
	deBaseGraphicModule *module = graSys->GetActiveModule();
	
	module->SetParameterValue( RT->GetValue( 0 )->GetString(), RT->GetValue( 1 )->GetString() );
}



// public static func String sendCommand( String command )
deClassGraphicSystem::nfSendCommand::nfSendCommand( const sInitData &init ) : dsFunction( init.clsGraSys,
"sendCommand", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsStr ){
	p_AddParameter( init.clsStr ); // command
}
void deClassGraphicSystem::nfSendCommand::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deScriptingDragonScript &ds = ( ( deClassGraphicSystem* )GetOwnerClass() )->GetDS();
	const deGraphicSystem &graSys = *ds.GetGameEngine()->GetGraphicSystem();
	deBaseGraphicModule &module = *graSys.GetActiveModule();
	decUnicodeArgumentList argList;
	decUnicodeString command;
	decUnicodeString answer;
	
	command.SetFromUTF8( rt->GetValue( 0 )->GetString() );
	argList.ParseCommand( command );
	module.SendCommand( argList, answer );
	rt->PushString( answer.ToUTF8().GetString() );
}



// class deClassGraphicSystem
////////////////////////////////
// constructor
deClassGraphicSystem::deClassGraphicSystem( deScriptingDragonScript &ds ) :
dsClass("GraphicSystem", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE),
pDS( ds )
{
	GetParserInfo()->SetParent(DENS_DRAGENGINE);
	GetParserInfo()->SetBase("Object");
	// do the rest
	p_SetNativeDataSize(0);
}
deClassGraphicSystem::~deClassGraphicSystem(){
}
// management
void deClassGraphicSystem::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	init.clsGraSys = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsBool = engine->GetClassBool();
	init.clsStr = engine->GetClassString();
	init.clsModPar = pDS.GetClassModuleParameter();
	init.clsCView = pDS.GetClassCanvasView();
	init.clsPoint = pDS.GetClassPoint();
	
	// add functions
	AddFunction( new nfGetWindowWidth( init ) );
	AddFunction( new nfGetWindowHeight( init ) );
	AddFunction( new nfGetWindowSize( init ) );
	AddFunction( new nfSetWindowGeometry( init ) );
	AddFunction( new nfSetWindowTitle( init ) );
	AddFunction( new nfGetPrimaryCanvas( init ) );
	AddFunction(new nfGetParameterCount(init));
	AddFunction(new nfGetParameterInfo(init));
	AddFunction(new nfGetParameterInfo2(init));
	AddFunction(new nfGetParameterValue(init));
	AddFunction(new nfSetParameterValue(init));
	AddFunction( new nfSendCommand( init ) );
}
