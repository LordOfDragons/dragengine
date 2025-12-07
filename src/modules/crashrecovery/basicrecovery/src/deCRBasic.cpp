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

#include "deCRBasic.h"
#include "decrbWindowMain.h"
#include "corefault/decrbCoreFault.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOS.h>
#include <dragengine/systems/deGraphicSystem.h>
#include <dragengine/systems/deInputSystem.h>
#include <dragengine/systems/modules/deLoadableModule.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/errortracing/deErrorTrace.h>
#include <dragengine/errortracing/deErrorTracePoint.h>
#include <dragengine/errortracing/deErrorTraceValue.h>



#ifndef WITH_INTERNAL_MODULE
#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *BRCreateModule(deLoadableModule *loadableModule);
#ifdef  __cplusplus
}
#endif
#endif


// Entry Point
////////////////

deBaseModule *BRCreateModule(deLoadableModule *loadableModule){
	deBaseModule *module = NULL;
	
	try{
		module = new deCRBasic(*loadableModule);
		
	}catch(const deException &){
		return NULL;
	}
	
	return module;
}



// class deCRBasic
//////////////////////////

// constructor, destructor
deCRBasic::deCRBasic(deLoadableModule &loadableModule) :
deBaseCrashRecoveryModule(loadableModule){
	pQuitEngine = true;
	pCoreFault = NULL;
}
deCRBasic::~deCRBasic(){
}



// Management
///////////////

bool deCRBasic::Init(){
	try{
		pCoreFault = new decrbCoreFault(this);
		
	}catch(const deException &e){
		LogException(e);
		return false;
	}
	
	return true;
}

void deCRBasic::CleanUp(){
	if(pCoreFault){
		delete pCoreFault;
		pCoreFault = NULL;
	}
}



// crash management
bool deCRBasic::RecoverFromError(){
	char argBuffer[6] = "dummy";
	int argc = 1;
	char *args[1] = {argBuffer};
	deEngine *engine = GetGameEngine();
	FXApp *app = NULL;
	
	// print error to console in case stopping graphic system crashes due to very ugly circumstances
	//LogTrace();
	
	// shut down graphic system
	engine->GetGraphicSystem()->Stop();
	//engine->GetOS()->OnFrameUpdate();
	//engine->GetInputSystem()->ProcessEvents();
	// show window
	try{
		app = new FXApp("Drag[en]gine Crash Recovery", "RPTD");
		if(!app) DETHROW(deeOutOfMemory);
		app->init(argc, args);
		new decrbWindowMain(app, this);
		app->create();
		app->run();
		delete app;
	}catch(const deException &e){
		if(app) delete app;
		e.PrintError();
		pQuitEngine = true;
	}
	// if we are not quitting restart system
	if(pQuitEngine){
		LogInfo("shuting down engine.");
		return false;
	}else{
		try{
			engine->GetGraphicSystem()->Start();
		}catch(const deException &){
			engine->GetGraphicSystem()->Stop();
			LogError("restarting engine systems failed, quitting.");
			return false;
		}
		return true;
	}
}

// internal functions for module classes only
void deCRBasic::SetQuitEngine(bool quitEngine){
	pQuitEngine = quitEngine;
}

void deCRBasic::LogTrace(){
	const deErrorTrace &trace = *GetGameEngine()->GetErrorTrace();
	const int pointCount = trace.GetPointCount();
	int i;
	
	LogError(decrbWindowMain::GetTextForError(trace.GetError()));
	
	for(i=0; i<pointCount; i++){
		const deErrorTracePoint &tracePoint = *trace.GetPoint(i);
		
		if(tracePoint.GetSourceModule()){
			LogErrorFormat("Trace %i: %s, %s at %i", i + 1,
				tracePoint.GetSourceModule()->GetName().GetString(),
				tracePoint.GetSourceFunction().GetString(),
				tracePoint.GetSourceLine());
			
		}else{
			LogErrorFormat("Trace %i: Game Engine, %s at %i", i + 1,
				tracePoint.GetSourceFunction().GetString(),
				tracePoint.GetSourceLine());
		}
		
		const int valueCount = tracePoint.GetValueCount();
		int j;
		for(j=0; j<valueCount; j++){
			LogTraceSubValues(*tracePoint.GetValue(j), "  ");
		}
	}
}

void deCRBasic::LogTraceSubValues(const deErrorTraceValue &traceValue, const char *indent){
	LogErrorFormat("- %s = %s", traceValue.GetName().GetString(), traceValue.GetValue().GetString());
	
	const int valueCount = traceValue.GetSubValueCount();
	if(valueCount == 0){
		return;
	}
	
	const decString childIndent(decString("  ") + indent);
	int i;
	
	for(i=0; i<valueCount; i++){
		LogTraceSubValues(*traceValue.GetSubValue(i), indent);
	}
}

#ifdef WITH_INTERNAL_MODULE
#include <dragengine/systems/modules/deInternalModule.h>

#ifndef MODULE_VERSION
#include "module_version.h"
#endif

class decrbModuleInternal : public deInternalModule{
public:
	decrbModuleInternal(deModuleSystem *system) : deInternalModule(system){
		SetName("BasicRecovery");
		SetDescription("Provides basic crash recovery support using the FOX Toolkit. \
Allows the user to restart failed systems and to change modules.");
		SetAuthor("DragonDreams GmbH (info@dragondreams.ch)");
		SetVersion(MODULE_VERSION);
		SetType(deModuleSystem::emtCrashRecovery);
		SetDirectoryName("basicrecovery");
		SetPriority(1);
		SetDefaultLoggingName();
	}
	
	void CreateModule() override{
		SetModule(BRCreateModule(this));
		if(!GetModule()){
			SetErrorCode(eecCreateModuleFailed);
		}
	}
};

deInternalModule *decrbRegisterInternalModule(deModuleSystem *system){
	return new decrbModuleInternal(system);
}
#endif
