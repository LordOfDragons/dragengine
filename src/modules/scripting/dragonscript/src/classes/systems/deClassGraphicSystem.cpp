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
#include <libdscript/libdscript.h>

#include "deClassGraphicSystem.h"
#include "../deClassEngine.h"
#include "../deClassModuleParameter.h"
#include "../canvas/deClassCanvasView.h"
#include "../math/deClassPoint.h"
#include "../math/deClassDVector.h"
#include "../math/deClassQuaternion.h"
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

// static public func int getWindowX()
deClassGraphicSystem::nfGetWindowX::nfGetWindowX(const sInitData &init) :
dsFunction(init.clsGraSys, "getWindowX", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInt){
}
void deClassGraphicSystem::nfGetWindowX::RunFunction(dsRunTime *rt, dsValue *myself){
	const deClassGraphicSystem &clsGraSys = *((deClassGraphicSystem*)GetOwnerClass());
	const deGraphicSystem &graSys = *clsGraSys.GetDS().GetGameEngine()->GetGraphicSystem();
	rt->PushInt(clsGraSys.CoordWindows2Canvas(graSys.GetRenderWindow()->GetX()));
}

// static public func int getWindowY()
deClassGraphicSystem::nfGetWindowY::nfGetWindowY(const sInitData &init) :
dsFunction(init.clsGraSys, "getWindowY", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInt){
}
void deClassGraphicSystem::nfGetWindowY::RunFunction(dsRunTime *rt, dsValue *myself){
	const deClassGraphicSystem &clsGraSys = *((deClassGraphicSystem*)GetOwnerClass());
	const deGraphicSystem &graSys = *clsGraSys.GetDS().GetGameEngine()->GetGraphicSystem();
	rt->PushInt(clsGraSys.CoordWindows2Canvas(graSys.GetRenderWindow()->GetY()));
}

// static public func Point getWindowPosition()
deClassGraphicSystem::nfGetWindowPosition::nfGetWindowPosition(const sInitData &init) :
dsFunction(init.clsGraSys, "getWindowPosition", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsPoint){
}
void deClassGraphicSystem::nfGetWindowPosition::RunFunction(dsRunTime *rt, dsValue *myself){
	const deClassGraphicSystem &clsGraSys = *((deClassGraphicSystem*)GetOwnerClass());
	const deGraphicSystem &graSys = *clsGraSys.GetDS().GetGameEngine()->GetGraphicSystem();
	const deRenderWindow &renderWindow = *graSys.GetRenderWindow();
	const decPoint position(clsGraSys.CoordWindows2Canvas(renderWindow.GetX()),
		clsGraSys.CoordWindows2Canvas(renderWindow.GetY()));
	
	clsGraSys.GetDS().GetClassPoint()->PushPoint(rt, position);
}

// static public func int getWindowWidth()
deClassGraphicSystem::nfGetWindowWidth::nfGetWindowWidth(const sInitData &init) :
dsFunction(init.clsGraSys, "getWindowWidth", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInt){
}
void deClassGraphicSystem::nfGetWindowWidth::RunFunction(dsRunTime *rt, dsValue *myself){
	const deClassGraphicSystem &clsGraSys = *((deClassGraphicSystem*)GetOwnerClass());
	const deGraphicSystem &graSys = *clsGraSys.GetDS().GetGameEngine()->GetGraphicSystem();
	rt->PushInt(clsGraSys.CoordWindows2Canvas(graSys.GetRenderWindow()->GetWidth()));
}

// static public func int getWindowHeight()
deClassGraphicSystem::nfGetWindowHeight::nfGetWindowHeight(const sInitData &init) :
dsFunction(init.clsGraSys, "getWindowHeight", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInt){
}
void deClassGraphicSystem::nfGetWindowHeight::RunFunction(dsRunTime *rt, dsValue *myself){
	const deClassGraphicSystem &clsGraSys = *((deClassGraphicSystem*)GetOwnerClass());
	const deGraphicSystem &graSys = *clsGraSys.GetDS().GetGameEngine()->GetGraphicSystem();
	rt->PushInt(clsGraSys.CoordWindows2Canvas(graSys.GetRenderWindow()->GetHeight()));
}

// static public func Point getWindowSize()
deClassGraphicSystem::nfGetWindowSize::nfGetWindowSize(const sInitData &init) :
dsFunction(init.clsGraSys, "getWindowSize", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsPoint){
}
void deClassGraphicSystem::nfGetWindowSize::RunFunction(dsRunTime *rt, dsValue *myself){
	const deClassGraphicSystem &clsGraSys = *((deClassGraphicSystem*)GetOwnerClass());
	const deGraphicSystem &graSys = *clsGraSys.GetDS().GetGameEngine()->GetGraphicSystem();
	const deRenderWindow &renderWindow = *graSys.GetRenderWindow();
	const decPoint size(clsGraSys.CoordWindows2Canvas(renderWindow.GetWidth()),
		clsGraSys.CoordWindows2Canvas(renderWindow.GetHeight()));
	
	clsGraSys.GetDS().GetClassPoint()->PushPoint(rt, size);
}

// public static func void setWindowGeometry(int width, int height, bool fullScreen)
deClassGraphicSystem::nfSetWindowGeometry::nfSetWindowGeometry(const sInitData &init) :
dsFunction(init.clsGraSys, "setWindowGeometry", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid){
	p_AddParameter(init.clsInt); // width
	p_AddParameter(init.clsInt); // height
	p_AddParameter(init.clsBool); // fullScreen
}
void deClassGraphicSystem::nfSetWindowGeometry::RunFunction(dsRunTime *rt, dsValue *myself){
	const deClassGraphicSystem &clsGraSys = *((deClassGraphicSystem*)GetOwnerClass());
	deGraphicSystem &graSys = *clsGraSys.GetDS().GetGameEngine()->GetGraphicSystem();
	const int width = rt->GetValue(0)->GetInt();
	const int height = rt->GetValue(1)->GetInt();
	const bool fullScreen = rt->GetValue(2)->GetBool();
	
	graSys.GetRenderWindow()->SetSize(clsGraSys.CoordCanvas2Window(width),
		clsGraSys.CoordCanvas2Window(height));
	graSys.GetRenderWindow()->SetFullScreen(fullScreen);
}

// public static func void setWindowGeometry(int x, int y, int width, int height, bool fullScreen)
deClassGraphicSystem::nfSetWindowGeometry2::nfSetWindowGeometry2(const sInitData &init) :
dsFunction(init.clsGraSys, "setWindowGeometry", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid){
	p_AddParameter(init.clsInt); // x
	p_AddParameter(init.clsInt); // y
	p_AddParameter(init.clsInt); // width
	p_AddParameter(init.clsInt); // height
	p_AddParameter(init.clsBool); // fullScreen
}
void deClassGraphicSystem::nfSetWindowGeometry2::RunFunction(dsRunTime *rt, dsValue *myself){
	const deClassGraphicSystem &clsGraSys = *((deClassGraphicSystem*)GetOwnerClass());
	deGraphicSystem &graSys = *clsGraSys.GetDS().GetGameEngine()->GetGraphicSystem();
	const int x = rt->GetValue(0)->GetInt();
	const int y = rt->GetValue(1)->GetInt();
	const int width = rt->GetValue(2)->GetInt();
	const int height = rt->GetValue(3)->GetInt();
	const bool fullScreen = rt->GetValue(4)->GetBool();
	
	graSys.GetRenderWindow()->SetPosition(clsGraSys.CoordCanvas2Window(x),
		clsGraSys.CoordCanvas2Window(y));
	graSys.GetRenderWindow()->SetSize(clsGraSys.CoordCanvas2Window(width),
		clsGraSys.CoordCanvas2Window(height));
	graSys.GetRenderWindow()->SetFullScreen(fullScreen);
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

// static public func int getWindowScaleFactor()
deClassGraphicSystem::nfGetWindowScaleFactor::nfGetWindowScaleFactor(const sInitData &init) :
dsFunction(init.clsGraSys, "getWindowScaleFactor", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInt){
}
void deClassGraphicSystem::nfGetWindowScaleFactor::RunFunction(dsRunTime *rt, dsValue *myself){
	const deClassGraphicSystem &clsGraSys = *((deClassGraphicSystem*)GetOwnerClass());
	const deGraphicSystem &graSys = *clsGraSys.GetDS().GetGameEngine()->GetGraphicSystem();
	rt->PushInt(graSys.GetRenderWindow()->GetScaleFactor());
}

// static public func float getWindowScaleFactorFloat()
deClassGraphicSystem::nfGetWindowScaleFactorFloat::nfGetWindowScaleFactorFloat(const sInitData &init) :
dsFunction(init.clsGraSys, "getWindowScaleFactorFloat", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsFlt){
}
void deClassGraphicSystem::nfGetWindowScaleFactorFloat::RunFunction(dsRunTime *rt, dsValue *myself){
	const deClassGraphicSystem &clsGraSys = *((deClassGraphicSystem*)GetOwnerClass());
	const deGraphicSystem &graSys = *clsGraSys.GetDS().GetGameEngine()->GetGraphicSystem();
	rt->PushFloat(0.01f * (float)graSys.GetRenderWindow()->GetScaleFactor());
}

// public static func CanvasView getPrimaryCanvas()
deClassGraphicSystem::nfGetPrimaryCanvas::nfGetPrimaryCanvas(const sInitData &init) :
dsFunction(init.clsGraSys, "getPrimaryCanvas", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsCView){
}
void deClassGraphicSystem::nfGetPrimaryCanvas::RunFunction(dsRunTime *rt, dsValue *myself){
	const deClassGraphicSystem &clsGraSys = *((deClassGraphicSystem*)GetOwnerClass());
	const deGraphicSystem &graSys = *clsGraSys.GetDS().GetGameEngine()->GetGraphicSystem();
	
	clsGraSys.ResizeActiveRenderWindowCanvas();

	clsGraSys.GetDS().GetClassCanvasView()->PushCanvas(rt, graSys.GetRenderWindow()->GetCanvasView());
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

// public static func int getFPSRate()
deClassGraphicSystem::nfGetFPSRate::nfGetFPSRate( const sInitData &init ) :
dsFunction( init.clsGraSys, "getFPSRate", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInt ){
}
void deClassGraphicSystem::nfGetFPSRate::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassGraphicSystem* )GetOwnerClass() )->GetDS();
	rt->PushInt( ds.GetGameEngine()->GetGraphicSystem()->GetActiveModule()->GetFPSRate() );
}

// public static func void setVRDebugPanelPosition( DVector position, Orientation orientation )
deClassGraphicSystem::nfSetVRDebugPanelPosition::nfSetVRDebugPanelPosition( const sInitData &init ) :
dsFunction( init.clsGraSys, "setVRDebugPanelPosition", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid ){
	p_AddParameter( init.clsDVector ); // position
	p_AddParameter( init.clsQuaternion ); // orientation
}
void deClassGraphicSystem::nfSetVRDebugPanelPosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deScriptingDragonScript &ds = ( ( deClassGraphicSystem* )GetOwnerClass() )->GetDS();
	
	const decDVector &position = ds.GetClassDVector()->GetDVector( rt->GetValue( 0 )->GetRealObject() );
	const decQuaternion &orientation = ds.GetClassQuaternion()->GetQuaternion( rt->GetValue( 1 )->GetRealObject() );
	
	ds.GetGameEngine()->GetGraphicSystem()->GetActiveModule()->SetVRDebugPanelPosition( position, orientation );
}



// class deClassGraphicSystem
///////////////////////////////
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
	init.clsDVector = pDS.GetClassDVector();
	init.clsQuaternion = pDS.GetClassQuaternion();
	
	// add functions
	AddFunction(new nfGetWindowX(init));
	AddFunction(new nfGetWindowY(init));
	AddFunction(new nfGetWindowPosition(init));
	AddFunction( new nfGetWindowWidth( init ) );
	AddFunction( new nfGetWindowHeight( init ) );
	AddFunction( new nfGetWindowSize( init ) );
	AddFunction( new nfSetWindowGeometry( init ) );
	AddFunction(new nfSetWindowGeometry2(init));
	AddFunction( new nfSetWindowTitle( init ) );
	AddFunction(new nfGetWindowScaleFactor(init));
	AddFunction( new nfGetPrimaryCanvas( init ) );
	AddFunction( new nfGetParameterCount( init ) );
	AddFunction( new nfGetParameterInfo( init ) ) ;
	AddFunction( new nfGetParameterInfo2( init ) );
	AddFunction( new nfGetParameterValue( init ) );
	AddFunction( new nfSetParameterValue( init ) );
	AddFunction( new nfSendCommand( init ) );
	AddFunction( new nfGetFPSRate( init ) );
	AddFunction( new nfSetVRDebugPanelPosition( init ) );
}

int deClassGraphicSystem::CoordCanvas2Window(int coordinates) const{
	return pDS.GetClassEngine()->GetDpiAware() ? coordinates
		: CoordCanvas2WindowAlways(coordinates);
}

int deClassGraphicSystem::CoordCanvas2WindowAlways(int coordinates) const{
	const deRenderWindow * const renderWindow =
		pDS.GetGameEngine()->GetGraphicSystem()->GetRenderWindow();
	if(renderWindow){
		coordinates = coordinates * renderWindow->GetScaleFactor() / 100;
	}
	return coordinates;
}

int deClassGraphicSystem::CoordWindows2Canvas(int coordinates) const{
	return pDS.GetClassEngine()->GetDpiAware() ? coordinates
		: CoordWindows2CanvasAlways(coordinates);
}

int deClassGraphicSystem::CoordWindows2CanvasAlways(int coordinates) const{
	const deRenderWindow * const renderWindow =
		pDS.GetGameEngine()->GetGraphicSystem()->GetRenderWindow();
	if(renderWindow){
		coordinates = coordinates * 100 / renderWindow->GetScaleFactor();
	}
	return coordinates;
}

void deClassGraphicSystem::ResizeActiveRenderWindowCanvas() const{
	deRenderWindow * const renderWindow =
		pDS.GetGameEngine()->GetGraphicSystem()->GetRenderWindow();
	if(renderWindow){
		ResizeRenderWindowCanvas(*renderWindow);
	}
}

void deClassGraphicSystem::ResizeRenderWindowCanvas(deRenderWindow& renderWindow) const{
	if(pDS.GetClassEngine()->GetDpiAware()){
		return;
	}

	renderWindow.GetCanvasView()->SetSize(decPoint(
		renderWindow.GetWidth() * 100 / renderWindow.GetScaleFactor(),
		renderWindow.GetHeight() * 100 / renderWindow.GetScaleFactor()
	));
}
