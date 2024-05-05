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

#include "ScriptingPython.h"
#include "modules/spBaseModule.h"
#include "modules/spModuleEngine.h"
#include "modules/spTypeModuleLoader.h"
#include "modules/graphics/spTypeCanvas.h"
#include "modules/systems/spModuleGraphicSystem.h"
#include "modules/world/spTypeWorld.h"
#include "modules/spScriptFileList.h"

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
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/exceptions.h>



// Export Definition
//////////////////////

#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *SPCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif



// Entry Function
///////////////////

deBaseModule *SPCreateModule( deLoadableModule *loadableModule ){
	deBaseModule *module = NULL;
	
	try{
		module = new ScriptingPython( *loadableModule );
		
	}catch( const deException & ){
		return NULL;
	}
	
	return module;
}



// Definitions
////////////////

/** \brief The class name of the game class the user is required to provide in the base script directory. */
#define SP_GAME_CLASS_SCRIPT		"Game"



// Class ScriptingPython
//////////////////////////

// Constructor, destructor
////////////////////////////

ScriptingPython::ScriptingPython( deLoadableModule &loadableModule ) :
deBaseScriptingModule( loadableModule ){
	if( pSingleton ){
		DETHROW( deeInvalidParam );
	}
	
	pModuleGame = NULL;
	pObjectGame = NULL;
	
	pNamespaceDragengine = NULL;
	pNamespaceScenery = NULL;
	pNamespaceGui = NULL;
	
	pTypeModuleLoader = NULL;
	pModuleEngine = NULL;
	pTypeCanvas = NULL;
	pModuleGraphicSystem = NULL;
	pTypeWorld = NULL;
	
	pScriptFileList = NULL;
	
	pSingleton = this;
}

ScriptingPython::~ScriptingPython(){
	ShutDown();
	
	pSingleton = NULL;
}



// Management
///////////////

const char *ScriptingPython::GetSharedDataDir() const{
	return "data";
}

const char *ScriptingPython::GetVFSSharedDataDir() const{
	return "/shareddata";
}

bool ScriptingPython::Init( const char *scriptDirectory, const char *gameObject ){
	PyObject *funcConstructor = NULL;
	
	pScriptPath = scriptDirectory;
	
	pScriptFileList = new spScriptFileList( this );
	
	Py_Initialize();
	if( PyErr_Occurred() ){
		PyErr_Print();
		return false;
	}
	
	try{
		pCreateModules();
		pLoadGameScript( scriptDirectory, gameObject );
		
		// create game object
		funcConstructor = PyObject_GetAttrString( pModuleGame, SP_GAME_CLASS_SCRIPT );
		if( ! funcConstructor || ! PyCallable_Check( funcConstructor ) ){
			LogErrorFormat( "Failed to run function '%s'.\n", SP_GAME_CLASS_SCRIPT );
			DETHROW( deeInvalidAction );
		}
		
		pObjectGame = PyObject_CallObject( funcConstructor, NULL );
		if( ! pObjectGame ){
			LogErrorFormat( "Failed to construct instance of class '%s'.\n", SP_GAME_CLASS_SCRIPT );
			DETHROW( deeInvalidAction );
		}
		Py_DECREF( funcConstructor );
		
	}catch( const deException &e ){
		LogException( e );
		if( PyErr_Occurred() ){
			PyErr_Print();
		}
		if( funcConstructor ){
			Py_DECREF( funcConstructor );
		}
		Py_Finalize();
		return false;
	}
	
	return true;
}

void ScriptingPython::ShutDown(){
	if( Py_IsInitialized() ){
		PythonCleanUp();
		
		// delete script file modules
		pScriptFileList->RemoveAll();
		
		// delete modules
		if( pModuleEngine ){
			delete pModuleEngine;
			pModuleEngine = NULL;
		}
		if( pTypeCanvas ){
			delete pTypeCanvas;
			pTypeCanvas = NULL;
		}
		if( pModuleGraphicSystem ){
			delete pModuleGraphicSystem;
			pModuleGraphicSystem = NULL;
		}
		if( pTypeWorld ){
			delete pTypeWorld;
			pTypeWorld = NULL;
		}
		if( pTypeModuleLoader ){
			delete pTypeModuleLoader;
			pTypeModuleLoader = NULL;
		}
		
		// delete namespaces
		if( pNamespaceGui ){
			delete pNamespaceGui;
			pNamespaceGui = NULL;
		}
		if( pNamespaceScenery ){
			delete pNamespaceScenery;
			pNamespaceScenery = NULL;
		}
		if( pNamespaceDragengine ){
			delete pNamespaceDragengine;
			pNamespaceDragengine = NULL;
		}
	}
	
	if( pScriptFileList ){
		delete pScriptFileList;
		pScriptFileList = NULL;
	}
}

deBaseScriptingCollider *ScriptingPython::CreateCollider( deCollider *collider ){
	return new deBaseScriptingCollider;
}

deBaseScriptingServer *ScriptingPython::CreateServer( deServer *server ){
	return new deBaseScriptingServer;
}

deBaseScriptingConnection *ScriptingPython::CreateConnection( deConnection *connection ){
	return new deBaseScriptingConnection;
}

deBaseScriptingNetworkState *ScriptingPython::CreateNetworkState( deNetworkState *state ){
	return new deBaseScriptingNetworkState;
}

deBaseScriptingTouchSensor *ScriptingPython::CreateTouchSensor( deTouchSensor *touchSensor ){
	return new deBaseScriptingTouchSensor;
}

deBaseScriptingPropField *ScriptingPython::CreatePropField( dePropField *propField ){
	return new deBaseScriptingPropField;
}

deBaseScriptingParticleEmitterInstance *ScriptingPython::CreateParticleEmitterInstance(
deParticleEmitterInstance *instance ){
	return NULL;
}

deBaseScriptingSoundLevelMeter *ScriptingPython::CreateSoundLevelMeter( deSoundLevelMeter *meter ){
	return NULL;
}

deBaseScriptingSpeaker *ScriptingPython::CreateSpeaker( deSpeaker *speaker ){
	return NULL;
}



ScriptingPython *ScriptingPython::pSingleton = NULL;

ScriptingPython *ScriptingPython::GetSP(){
	return pSingleton;
}



bool ScriptingPython::InitGame(){
	return CallFunction( "initGame" );
}

bool ScriptingPython::ExitGame(){
	return CallFunction( "cleanUp" );
}

bool ScriptingPython::OnFrameUpdate(){
	const char *functionName = "onFrameUpdate";
	PyObject *function = NULL;
	PyObject *retval = NULL;
	PyObject *args = NULL;
	
	args = Py_BuildValue( "(f)", GetGameEngine()->GetElapsedTime() );
	if( ! args ){
		LogErrorFormat( "Failed to run function '%s'.\n", functionName );
		if( PyErr_Occurred() ){
			PyErr_Print();
		}
		return false;
	}
	
	function = PyObject_GetAttrString( pObjectGame, functionName );
	if( ! function || ! PyCallable_Check( function ) ){
		LogErrorFormat( "Failed to run function '%s'.\n", functionName );
		if( PyErr_Occurred() ){
			PyErr_Print();
		}
		if( function ){
			Py_DECREF( function );
		}
		Py_DECREF( args );
		return false;
	}
	
	retval = PyObject_CallObject( function, args );
	if( ! retval ){
		LogErrorFormat( "Failed to run function '%s'.\n", functionName );
		if( PyErr_Occurred() ){
			PyErr_Print();
		}
		Py_DECREF( function );
		Py_DECREF( args );
		return false;
	}
	Py_DECREF( retval );
	Py_DECREF( function );
	Py_DECREF( args );
	
	return true;
}

bool ScriptingPython::OnResizeRenderWindow(){
	return CallFunction( "onResizeRenderWindow" );
}

bool ScriptingPython::SendEvent( deInputEvent *event ){
	const char *functionName = "";
	int type = event->GetType();
	PyObject *function = NULL;
	PyObject *retval = NULL;
	PyObject *args = NULL;
	
	// eventKeyPress( keycode, modifiers, keychar )
	if( type == deInputEvent::eeKeyPress ){
		functionName = "eventKeyPress";
		args = Py_BuildValue( "(iii)", event->GetCode(), event->GetState(), event->GetKeyChar() );
		
	// eventKeyRelease( keycode, modifiers, keychar )
	}else if( type == deInputEvent::eeKeyRelease ){
		functionName = "eventKeyRelease";
		args = Py_BuildValue( "(iii)", event->GetCode(), event->GetState(), event->GetKeyChar() );
		
	// eventMousePress( button, modifiers )
	}else if( type == deInputEvent::eeMousePress ){
		functionName = "eventMousePress";
		args = Py_BuildValue( "(ii)", event->GetCode(), event->GetState() );
		
	// eventMouseRelease( button, modifiers )
	}else if( type == deInputEvent::eeMouseRelease ){
		functionName = "eventMouseRelease";
		args = Py_BuildValue( "(ii)", event->GetCode(), event->GetState() );
		
	// eventMouseMove( dx, dy, button, modifiers )
	}else if( type == deInputEvent::eeMouseMove ){
		functionName = "eventMouseMove";
		args = Py_BuildValue( "(iiii)", event->GetX(), event->GetY(), event->GetCode(), event->GetState() );
		
	// this should never happen
	}else{
		LogErrorFormat( "What the... an unknown event code?! (%i)\n", type );
		return false;
	}
	
	if( ! args ){
		LogErrorFormat( "Failed to run function '%s'.\n", functionName );
		if( PyErr_Occurred() ){
			PyErr_Print();
		}
		return false;
	}
	
	// call the function if possible
	function = PyObject_GetAttrString( pObjectGame, functionName );
	if( ! function || ! PyCallable_Check( function ) ){
		LogErrorFormat( "Failed to run function '%s'.\n", functionName );
		if( PyErr_Occurred() ){
			PyErr_Print();
		}
		if( function ){
			Py_DECREF( function );
		}
		Py_DECREF( args );
		return false;
	}
	
	retval = PyObject_CallObject( function, args );
	if( ! retval ){
		LogErrorFormat( "Failed to run function '%s'.\n", functionName );
		if( PyErr_Occurred() ){
			PyErr_Print();
		}
		Py_DECREF( function );
		Py_DECREF( args );
		return false;
	}
	Py_DECREF( retval );
	Py_DECREF( function );
	Py_DECREF( args );
	
	return true;
}



void ScriptingPython::PythonCleanUp(){
	// clean up game object
	if( pObjectGame ){
		Py_DECREF( pObjectGame );
		pObjectGame = NULL;
	}
	if( pModuleGame ){
		Py_DECREF( pModuleGame );
		pModuleGame = NULL;
	}
	
	// clean up script file modules
	pScriptFileList->PythonCleanUpAll();
	
	// cleanup modules
	if( pTypeWorld ){
		pTypeWorld->PythonCleanUp();
	}
	if( pModuleGraphicSystem ){
		pModuleGraphicSystem->PythonCleanUp();
	}
	if( pTypeCanvas ){
		pTypeCanvas->PythonCleanUp();
	}
	if( pModuleEngine ){
		pModuleEngine->PythonCleanUp();
	}
	if( pTypeModuleLoader ){
		pTypeModuleLoader->PythonCleanUp();
	}
	
	// clean up namespaces
	if( pNamespaceGui ){
		pNamespaceGui->PythonCleanUp();
	}
	if( pNamespaceScenery ){
		pNamespaceScenery->PythonCleanUp();
	}
	if( pNamespaceDragengine ){
		pNamespaceDragengine->PythonCleanUp();
	}
	
	// tear down python
	Py_Finalize();
}



bool ScriptingPython::CallFunction( const char *functionName ){
	PyObject * const function = PyObject_GetAttrString( pObjectGame, functionName );
	if( ! function || ! PyCallable_Check( function ) ){
		LogErrorFormat( "Failed to run function '%s'.\n", functionName );
		if( PyErr_Occurred() ){
			PyErr_Print();
			PyErr_Clear();
		}
		if( function ){
			Py_DECREF( function );
		}
		return false;
	}
	
	PyObject * const retval = PyObject_CallObject( function, NULL );
	if( ! retval ){
		LogErrorFormat( "Failed to run function '%s'.\n", functionName );
		if( PyErr_Occurred() ){
			PyErr_Print();
			PyErr_Clear();
		}
		Py_DECREF( function );
		return false;
	}
	Py_DECREF( retval );
	Py_DECREF( function );
	
	return true;
}



void ScriptingPython::SetErrorTracePython(){
}

void ScriptingPython::LogExceptionPython(){
}



// Private functions
//////////////////////

void ScriptingPython::pCreateModules(){
	// create namespaces
	pNamespaceDragengine = new spBaseModule( *this, "Dragengine" );
	pNamespaceDragengine->SetDocumentation( "Namespace Dragengine" );
	pNamespaceDragengine->PythonCreate();
	
	PyRun_SimpleString( "print( globals() )\n" );
	PyRun_SimpleString( "import sys\nprint( sys.modules.keys() )\n" );
	PyRun_SimpleString( "import Dragengine\nprint( 'Dragengine:', [(x,Dragengine.__dict__[x]) for x in dir(Dragengine)] )\n" );
	
	pNamespaceScenery = new spBaseModule( *this, "Scenery" );
	pNamespaceScenery->SetDocumentation( "Namespace Dragengine.Scenery" );
	pNamespaceScenery->SetParent( pNamespaceDragengine );
	pNamespaceScenery->PythonCreate();
	
	pNamespaceGui = new spBaseModule( *this, "Gui" );
	pNamespaceGui->SetDocumentation( "Namespace Dragengine.Gui" );
	pNamespaceGui->SetParent( pNamespaceDragengine );
	pNamespaceGui->PythonCreate();
	
	PyRun_SimpleString( "print( globals() )\n" );
	
	// create modules
	pTypeModuleLoader = new spTypeModuleLoader( *this );
	pTypeModuleLoader->PythonCreate();
	
	pModuleEngine = new spModuleEngine( *this );
	pModuleEngine->PythonCreate();
	
	pTypeWorld = new spTypeWorld( *this );
	pTypeWorld->PythonCreate();
	
	pTypeCanvas = new spTypeCanvas( *this );
	pTypeCanvas->PythonCreate();
	
	pModuleGraphicSystem = new spModuleGraphicSystem( *this );
	pModuleGraphicSystem->PythonCreate();
}

void ScriptingPython::pLoadGameScript( const char *directory, const char *gameObject ){
	PyRun_SimpleString( "print('test')\n"
		"import Game\n"
		"print(globals())\n"
		"print(Game)\n" );
	
	pModuleGame = PyImport_ImportModule( gameObject );
	if( ! pModuleGame ){
		DETHROW( deeInvalidAction );
	}
}
