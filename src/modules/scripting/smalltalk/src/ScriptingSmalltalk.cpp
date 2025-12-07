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
#include <unistd.h>

#include "ScriptingSmalltalk.h"
#include "classes/stClassScripting.h"
#include "classes/stClassEngine.h"
#include "classes/graphics/stClassColor.h"
#include "classes/graphics/stClassCanvas.h"
#include "classes/math/stClassVector2.h"
#include "classes/math/stClassPoint.h"
#include "classes/systems/stClassGraphicSystem.h"
//#include "classes/world/stClassWorld.h"

#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingConnection.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingNetworkState.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingServer.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingTouchSensor.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingPropField.h>

#include <dragengine/deEngine.h>
#include "dragengine/input/deInputEvent.h"
#include <dragengine/filesystem/dePathList.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/filesystem/deCollectFileSearchVisitor.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/exceptions.h>



// Export Definition
//////////////////////

#ifndef WITH_INTERNAL_MODULE
#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *STCreateModule(deLoadableModule *loadableModule);
#ifdef  __cplusplus
}
#endif
#endif


// Entry Function
///////////////////

deBaseModule *STCreateModule(deLoadableModule *loadableModule){
	deBaseModule *module = NULL;
	
	try{
		module = new ScriptingSmalltalk(*loadableModule);
		
	}catch(const deException &){
		return NULL;
	}
	
	return module;
}



// Definitions
////////////////



// Class ScriptingSmalltalk
/////////////////////////////

// Constructor, destructor
////////////////////////////

ScriptingSmalltalk::ScriptingSmalltalk(deLoadableModule &loadableModule) :
deBaseScriptingModule(loadableModule),

pObjNil(NULL),
pClsCharacter(NULL),
pClsOrderedCollection(NULL),
pSelectorNew(NULL),
pSelectorValue(NULL),
pSelectorAdd(NULL),
pSelectorInitialize(NULL),
pSelectorAt(NULL),

pSelectorInitGame(NULL),
pSelectorCleanUp(NULL),
pSelectorOnFrameUpdate(NULL),
pSelectorOnResizeRenderWindow(NULL),
pSelectorEventKeyPress(NULL),
pSelectorEventKeyRelease(NULL),
pSelectorEventKeyType(NULL),
pSelectorEventMousePress(NULL),
pSelectorEventMouseRelease(NULL),
pSelectorEventMouseMove(NULL),

pClsGame(NULL),
pObjGame(NULL),

pClsDragengine(NULL),
pClsScenery(NULL),
pClsGui(NULL),

pClsCanvas(NULL),
pClsColor(NULL),
pClsEngine(NULL),
pClsGraphicSystem(NULL),
pClsScripting(NULL),
pClsVector2(NULL),
pClsPoint(NULL),
pClsWorld(NULL)
{
}

ScriptingSmalltalk::~ScriptingSmalltalk(){
	ShutDown();
}



// Management
///////////////

const char *ScriptingSmalltalk::GetSharedDataDir() const{
	return "data";
}

const char *ScriptingSmalltalk::GetVFSSharedDataDir() const{
	return "/shareddata";
}

bool ScriptingSmalltalk::Init(const char *scriptDirectory, const char *gameObject){
	// WARNING
	// GST has the bad habit of using SEGFAULT for flow control. While debugging
	// keep this in mind or you see problems where none exist
	
	// some version stuff
	gst_set_var(GST_VERBOSITY, 1);
	
	int hackARGC = 1;
	const char *hackARGV[1] = {"-V"};
	gst_smalltalk_args(hackARGC, (const char**)&hackARGV); // we have none
	
	// we use the game script directory as the executable path
	//gst_set_executable_path( dataDir ); // undefined in gstpub.h ?!
	
	// fire up the vm
	// gst_initialize ("kernel-dir", "image-file", GST_NO_TTY);
	if(gst_initialize(NULL, NULL, GST_NO_TTY) != 0){
		LogError("gst_initialize failed.\n");
		return false;
	}
	
	// protect this one here
	try{
		// obtain smalltalk class and symbols
		pObtainSTStuff();
		
		// add classes
		pCreateClasses();
		
		// load the game script
		if(!pLoadGameScripts(scriptDirectory, gameObject)){
			return false;
		}
		
		// create game object
		
	}catch(const deException &e){
		e.PrintError();
		return false;
	}
	
	// ready to go
	return true;
}

void ScriptingSmalltalk::ShutDown(){
	// unregister the game object
	gst_unregister_oop(pObjGame);
	pObjGame = NULL;
	
	// send smalltalk home
	gst_invoke_hook(GST_ABOUT_TO_QUIT);
	
	// free classes
	if(pClsVector2){
		delete pClsVector2;
		pClsVector2 = NULL;
	}
	if(pClsPoint){
		delete pClsPoint;
		pClsPoint = NULL;
	}
	if(pClsGraphicSystem){
		delete pClsGraphicSystem;
		pClsGraphicSystem = NULL;
	}
	if(pClsEngine){
		delete pClsEngine;
		pClsEngine = NULL;
	}
	if(pClsColor){
		delete pClsColor;
		pClsColor = NULL;
	}
	if(pClsCanvas){
		delete pClsCanvas;
		pClsCanvas = NULL;
	}
	
	if(pClsScripting){
		delete pClsScripting;
		pClsScripting = NULL;
	}
}

deBaseScriptingCollider *ScriptingSmalltalk::CreateCollider(deCollider *collider){
	return new deBaseScriptingCollider;
}

deBaseScriptingServer *ScriptingSmalltalk::CreateServer(deServer *server){
	return new deBaseScriptingServer;
}

deBaseScriptingConnection *ScriptingSmalltalk::CreateConnection(deConnection *connection){
	return new deBaseScriptingConnection;
}

deBaseScriptingNetworkState *ScriptingSmalltalk::CreateNetworkState(deNetworkState *state){
	return new deBaseScriptingNetworkState;
}

deBaseScriptingTouchSensor *ScriptingSmalltalk::CreateTouchSensor(deTouchSensor *touchSensor){
	return new deBaseScriptingTouchSensor;
}

deBaseScriptingPropField *ScriptingSmalltalk::CreatePropField(dePropField *propField){
	return new deBaseScriptingPropField;
}

deBaseScriptingParticleEmitterInstance *ScriptingSmalltalk::CreateParticleEmitterInstance(
deParticleEmitterInstance *instance){
	return NULL;
}

deBaseScriptingSoundLevelMeter *ScriptingSmalltalk::CreateSoundLevelMeter(deSoundLevelMeter *meter){
	return NULL;
}

deBaseScriptingSpeaker *ScriptingSmalltalk::CreateSpeaker(deSpeaker *speaker){
	return NULL;
}



bool ScriptingSmalltalk::InitGame(){
	return SendSTMessage(pSelectorInitGame);
}

bool ScriptingSmalltalk::ExitGame(){
	return SendSTMessage(pSelectorCleanUp);
}

bool ScriptingSmalltalk::OnFrameUpdate(){
	OOP argElapsed = NULL;
	OOP result = NULL;
	
	try{
		argElapsed = gst_float_to_oop((double)GetGameEngine()->GetElapsedTime());
		
		result = gst_perform_with(pObjGame, pSelectorOnFrameUpdate, argElapsed);
		if(result == pObjNil){
			DETHROW(deeInvalidParam);
		}
		
	}catch(const deException &e){
		e.PrintError();
		return false;
	}
	
	return true;
}

bool ScriptingSmalltalk::OnResizeRenderWindow(){
	return SendSTMessage(pSelectorOnResizeRenderWindow);
}

bool ScriptingSmalltalk::SendEvent(deInputEvent *event){
	OOP arguments[4] = {NULL, NULL, NULL, NULL};
	int type = event->GetType();
	int argumentCount = 0;
	OOP result = NULL;
	OOP selector;
	
	try{
		if(type == deInputEvent::eeKeyPress){
			selector = pSelectorEventKeyPress;
			arguments[0] = gst_int_to_oop(event->GetCode());
			arguments[1] = gst_int_to_oop(event->GetState());
			arguments[2] = gst_perform_with(pClsCharacter, pSelectorValue, gst_int_to_oop(event->GetKeyChar()));
			argumentCount = 3;
			
		}else if(type == deInputEvent::eeKeyRelease){
			selector = pSelectorEventKeyRelease;
			arguments[0] = gst_int_to_oop(event->GetCode());
			arguments[1] = gst_int_to_oop(event->GetState());
			arguments[2] = gst_perform_with(pClsCharacter, pSelectorValue, gst_int_to_oop(event->GetKeyChar()));
			argumentCount = 3;
			
		}else if(type == deInputEvent::eeMousePress){
			selector = pSelectorEventMousePress;
			arguments[0] = gst_int_to_oop(event->GetCode());
			arguments[1] = gst_int_to_oop(event->GetState());
			argumentCount = 2;
			
		}else if(type == deInputEvent::eeMouseRelease){
			selector = pSelectorEventMouseRelease;
			arguments[0] = gst_int_to_oop(event->GetCode());
			arguments[1] = gst_int_to_oop(event->GetState());
			argumentCount = 2;
			
		}else if(type == deInputEvent::eeMouseMove){
			selector = pSelectorEventMouseMove;
			arguments[0] = gst_int_to_oop(event->GetX());
			arguments[1] = gst_int_to_oop(event->GetY());
			arguments[2] = gst_int_to_oop(event->GetCode());
			arguments[3] = gst_int_to_oop(event->GetState());
			argumentCount = 4;
			
		}else{
			LogErrorFormat("What the... an unknown event code?!(%i)", type);
			return false;
		}
		
		result = gst_nvmsg_send(pObjGame, selector, &arguments[0], argumentCount);
		if(result == pObjNil){
			DETHROW(deeInvalidParam);
		}
		
	}catch(const deException &e){
		e.PrintError();
		return false;
	}
	
	return true;
}



bool ScriptingSmalltalk::SendSTMessage(OOP selector){
	OOP result = NULL;
	
	try{
		result = gst_perform(pObjGame, selector);
		if(result == pObjNil){
			DETHROW(deeInvalidParam);
		}
		
	}catch(const deException &e){
		e.PrintError();
		return false;
	}
	
	return true;
}

OOP ScriptingSmalltalk::CreateNewObject(OOP stclass){
	OOP result = NULL;
	
	result = gst_perform(stclass, pSelectorNew);
	if(result == pObjNil){
		DETHROW(deeInvalidParam);
	}
	
	return result;
}

OOP ScriptingSmalltalk::CreateNewObject(OOP stclass, int instVarBytes){
	if(instVarBytes < 0){
		DETHROW(deeInvalidParam);
	}
	OOP result = NULL;
	
	result = gst_object_alloc(stclass, instVarBytes);
	if(result == pObjNil){
		DETHROW(deeInvalidParam);
	}
	
	return result;
}

OOP ScriptingSmalltalk::CreateNewObjectWithInit(OOP stclass, int instVarBytes){
	if(instVarBytes < 0){
		DETHROW(deeInvalidParam);
	}
	OOP result = NULL;
	
	result = gst_object_alloc(stclass, instVarBytes);
	if(result == pObjNil){
		DETHROW(deeInvalidParam);
	}
	
	gst_perform(result, pSelectorInitialize);
	
	return result;
}



bool ScriptingSmalltalk::LoadScripts(deVirtualFileSystem &vfs, const char *directory){
	if(!directory){
		DETHROW(deeInvalidParam);
	}
	
	try{
		LogInfoFormat("Load Scripts from '%s'", directory);
		pAddScripts(vfs, directory);
		
	}catch(const deException &e){
		/*
		SetErrorTrace(e);
		deErrorTracePoint *tracePoint = AddErrorTracePoint("deScriptingDragonScript::LoadPackage", __LINE__);
		tracePoint->AddValue("name", name);
		tracePoint->AddValue("directory", directory);
		*/
		e.PrintError();
		return false;
	}
	
	return true;
}



// Private functions
//////////////////////

OOP ScriptingSmalltalk::pGetSystemClassByName(const char *name){
	OOP oop = gst_class_name_to_oop(name);
	
	if(!oop){
		LogErrorFormat("Failed to obtain system class '%s'. Please reinstall Smalltalk.", name);
		DETHROW(deeInvalidParam);
	}
	
	return oop;
}

OOP ScriptingSmalltalk::pGetSystemSymbolByName(const char *name){
	OOP oop = gst_symbol_to_oop(name);
	
	if(!oop){
		LogErrorFormat("Failed to obtain system symbol '%s'. Please reinstall Smalltalk.", name);
		DETHROW(deeInvalidParam);
	}
	
	return oop;
}



void ScriptingSmalltalk::pObtainSTStuff(){
	// get nil
	pObjNil = gst_eval_expr("nil");
	if(!pObjNil){
		LogError("Failed to obtain 'nil'. Please reinstall Smalltalk.");
		DETHROW(deeInvalidParam);
	}
	
	// get some useful classes
	pClsCharacter = pGetSystemClassByName("Character");
	pClsOrderedCollection = pGetSystemClassByName("OrderedCollection");
	
	// get various selectors
	pSelectorNew = pGetSystemSymbolByName("new");
	pSelectorValue = pGetSystemSymbolByName("value:");
	pSelectorAdd = pGetSystemSymbolByName("add:");
	pSelectorInitialize = pGetSystemSymbolByName("initialize");
	pSelectorAt = pGetSystemSymbolByName("at:");
	
	pSelectorInitGame = pGetSystemSymbolByName("initGame");
	pSelectorCleanUp = pGetSystemSymbolByName("cleanUp");
	pSelectorOnFrameUpdate = pGetSystemSymbolByName("onFrameUpdate:");
	pSelectorOnResizeRenderWindow = pGetSystemSymbolByName("onResizeRenderWindow");
	pSelectorEventKeyPress = pGetSystemSymbolByName("eventKeyPress:modifiers:keychar:");
	pSelectorEventKeyRelease = pGetSystemSymbolByName("eventKeyRelease:modifiers:keychar:");
	pSelectorEventKeyType = pGetSystemSymbolByName("eventKeyType:modifiers:keychar:");
	pSelectorEventMousePress = pGetSystemSymbolByName("eventMousePress:modifiers:");
	pSelectorEventMouseRelease = pGetSystemSymbolByName("eventMouseRelease:modifiers:");
	pSelectorEventMouseMove = pGetSystemSymbolByName("eventMouseMove:dy:button:modifiers:");
}

void ScriptingSmalltalk::pCreateClasses(){
	// create classes
	pClsScripting = new stClassScripting(this);
	
	pClsCanvas = new stClassCanvas(*this);
	pClsColor = new stClassColor(this);
	pClsEngine = new stClassEngine(*this);
	pClsGraphicSystem = new stClassGraphicSystem(*this);
	pClsPoint = new stClassPoint(*this);
	pClsVector2 = new stClassVector2(*this);
	//pClsWorld = new stClassWorld( this );
	
	// load scripts
	pAddScripts(GetVFS(), "/share/scripts");
	
	// create scripting singleton
	pClsScripting->CreateSingleton();
	
	// set up links
	pClsCanvas->SetUpLinks();
	pClsColor->SetUpLinks();
	pClsEngine->SetUpLinks();
	pClsGraphicSystem->SetUpLinks();
	pClsPoint->SetUpLinks();
	pClsVector2->SetUpLinks();
	//pClsWorld->SetUpLinks();
}

bool ScriptingSmalltalk::pLoadGameScripts(const char *directory, const char *gameClass){
	// load scripts from the game directory
	bool result;
	try{
		result = LoadScripts(*GetGameEngine()->GetVirtualFileSystem(), directory);
		if(!result){
			return false;
		}
		
	}catch(const deException &e){
		/*
		SetErrorTrace(e);
		deErrorTracePoint *tracePoint = AddErrorTracePoint("deScriptingDragonScript::pLoadGamePackage", __LINE__);
		tracePoint->AddValue("directory", directory);
		pAddExceptionTrace(tracePoint);
		*/
		e.PrintError();
		return false;
	}
	
	// examine vm to find class to start
	OOP clsGame = gst_class_name_to_oop("DEGame");
	if(!clsGame){
		LogError("Class #DEGame not found. Please reinstall the Smalltalk Scripting Module.");
		return false;
	}
	
	pClsGame = gst_class_name_to_oop(gameClass);
	if(!pClsGame){
		LogErrorFormat("Game class #%s not found.", gameClass);
		return false;
	}
	
	// create instance of game class
	pObjGame = gst_perform(pClsGame, pSelectorNew);
	if(pObjGame == pObjNil){
		LogError("Failed to create game object.");
		return false;
	}
	
	gst_register_oop(pObjGame);
	
	// finished
	return true;
}

void ScriptingSmalltalk::pAddScripts(deVirtualFileSystem &vfs, const char *path){
	decBaseFileReader *filereader = NULL;
	decString content;
	int p, filetype;
	int filelen;
	
	try{
		deCollectFileSearchVisitor collect("*.st", true);
		vfs.SearchFiles(decPath::CreatePathUnix(path), collect);
		
		const dePathList &pathList = collect.GetFiles();
		for(p=0; p<pathList.GetCount(); p++){
			const decPath &path = pathList.GetAt(p);
			const decString filename(path.GetPathUnix());
			
			filetype = vfs.GetFileType(path);
			if(filetype == deVFSContainer::eftRegularFile){
				//if( decString( filename ).MatchesPattern( "*.st" ) ){
				if(pIsFileType(filename, "st")){
					//LogInfoFormat( "parsing script file %s.", path.GetPathUnix() );
					
					filereader = vfs.OpenFileForReading(path);
					if(!filereader){
						DETHROW(deeOutOfMemory);
					}
					
					filelen = filereader->GetLength();
					filereader->SetPosition(0);
					
					content.Set(' ', filelen);
					filereader->Read((void*)content.GetString(), filelen);
					
					//LogInfoFormat( "filereader %s.", filereader->GetFilename() );
					//if( ! gst_process_file( filereader->GetFilename(), GST_DIR_ABS ) ) DETHROW( deeReadFile );
					gst_eval_code(content.GetString());
					
					filereader->FreeReference();
					filereader = NULL;
				}
				
			}else if(filetype == deVFSContainer::eftDirectory){
				pAddScripts(vfs, filename);
			}
		}
		
	}catch(const deException &e){
		if(filereader){
			filereader->FreeReference();
		}
		/*
		SetErrorTrace(e);
		deErrorTracePoint *tracePoint = AddErrorTracePoint("deScriptingDragonScript::pAddScripts", __LINE__);
		tracePoint->AddValue("package", Package->GetName());
		tracePoint->AddValue("path", path);
		*/
		e.PrintError();
		throw;
	}
}

bool ScriptingSmalltalk::pIsFileType(const char *filename, const char *extension){
	const char *ext = strrchr(filename, '.');
	if(!ext){
		return false;
	}
	return strcmp(ext + 1, extension) == 0;
}
