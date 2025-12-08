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

#include "android_native_app_glue.h"

#include <stdio.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>

#include <android/asset_manager.h>

#include "dealLauncher.h"
#include "dealAssetUnpacker.h"
#include "dealRemoveDirTree.h"
#include "dealGameData.h"
#include "common/exceptions.h"
#include "common/file/decDiskFileReader.h"
#include "common/file/decDiskFileWriter.h"
#include "common/file/decMemoryFile.h"
#include "common/file/decMemoryFileReader.h"
#include "common/file/decMemoryFileWriter.h"
#include "logger/deLoggerConsole.h"
#include "logger/deLoggerChain.h"
#include "logger/deLoggerFile.h"
#include "engine/dealEngine.h"
#include "filesystem/deVFSDiskDirectory.h"
#include "game/dealGame.h"
#include "game/fileformat/dealFileFormat.h"
#include "game/profile/dealGameProfile.h"
#include "gui/dealDisplay.h"
#include "gui/dealDialog.h"
#include "gui/dialogs/dealDialogMain.h"
#include "gui/widgets/dealWidget.h"
#include "gui/widgets/dealWidgetLayoutFlow.h"
#include "gui/widgets/dealWidgetTaskStatus.h"
#include "../srcengine/dealIEngineInstance.h"

#define LOGSOURCE "DEAndroidLauncher"



/**
 * Null logger to deal with possibility of logger being
 * used during destructor unwinding.
 */
extern deLoggerConsole *pNullLogger;



// Class dealLauncher
///////////////////////

// Constructors, destructors
//////////////////////////////

dealLauncher::dealLauncher(android_app &androidApp) :
pAndroidApp(androidApp),
pJniEnv(NULL),

pState(esRestoreState),
pAssetUnpacker(NULL),

pDisplay(*this),
pConfiguration(*this),
pGameManager(*this),
pEngine(*this),
pModuleParameters(NULL),
pRunWidth(0),
pRunHeight(0),
pRunFullScreen(true),
pFocused(false),
pFrozen(false)
{
	try{
		// create file system
		pFileSystem.TakeOver(new deVirtualFileSystem);
		
		// sanity checks
		if(!pAndroidApp.activity){
			DETHROW(deeInvalidAction);
		}
		if(!pAndroidApp.config){
			DETHROW(deeInvalidAction);
		}
		if(!pAndroidApp.looper){
			DETHROW(deeInvalidAction);
		}
		
		// configurate
		pConfiguration.LocatePath();
		pConfiguration.InitVirtualFileSystem();
		pInitLogger(); // has to come after file system is setup since it uses it
		pConfiguration.LogImportantValues();
		pConfiguration.LoadConfiguration();
		
		// attach JNI environment
		androidApp.activity->vm->AttachCurrentThread(&pJniEnv, 0);
		
	}catch(const deException &e){
		GetLogger().LogException(LOGSOURCE, e);
		pCleanUp();
		throw;
	}
}

dealLauncher::~dealLauncher(){
	pCleanUp();
}



// Management
///////////////

const char *dealLauncher::GetInternalPath() const{
	return pAndroidApp.activity->internalDataPath;
}

const char *dealLauncher::GetExternalPath() const{
	// if the external path is NULL use the internal one instead. this should never happen but if
	// it does delivering anything but NULL is desired
	if(pAndroidApp.activity->externalDataPath){
		return pAndroidApp.activity->externalDataPath;
		
	}else{
		return pAndroidApp.activity->internalDataPath;
	}
}

bool dealLauncher::HasAppWindow() const{
	return pAndroidApp.window != NULL;
}

deLogger &dealLauncher::GetLogger() const{
	if(pLogger){
		return::Ref pLogger;
		
	}else{
		// during destructor unwinding it is possible an object accesses the logger causing a
		// segfault. if the logger is NULL return temporary console logger stored in general
		// memory which is never freed until the process dies. this prevents problems
		return *pNullLogger;
	}
}



void dealLauncher::LoadAsset(const char *filename, decMemoryFile &memoryFile){
	AAsset *asset = NULL;
	
	GetLogger().LogInfoFormat(LOGSOURCE, "Load asset %s", filename);
	
	try{
		asset = AAssetManager_open(pAndroidApp.activity->assetManager, filename, AASSET_MODE_STREAMING);
		if(!asset){
			DETHROW(deeOpenFile);
		}
		
		const int length = AAsset_getLength(asset);
		memoryFile.Resize(length);
		
		AAsset_read(asset, memoryFile.GetPointer(), length);
		AAsset_close(asset);
		
	}catch(const deException &){
		if(asset){
			AAsset_close(asset);
		}
		throw;
	}
}



void dealLauncher::RestoreState(){
	decMemoryFileReader::Ref reader = NULL;
	decMemoryFile::Ref data = NULL;
	
	try{
		data.TakeOver(new decMemoryFile("launcher-state.dat"));
		
		// copy data from android to the memory file
		data->Resize(pAndroidApp.savedStateSize);
		memcpy(data->GetPointer(), pAndroidApp.savedState, data->GetLength());
		
		// read state from memory file
		reader.TakeOver(new decMemoryFileReader(data));
		pReadState(*reader);
		
		GetLogger().LogInfoFormat(LOGSOURCE, "Restored launcher state (%i bytes)", data->GetLength());
		reader = NULL;
		data = NULL;
		
		pState = esProcessActivityIntent;
		
	}catch(const deException &e){
		GetLogger().LogException(LOGSOURCE, e);
		ANativeActivity_finish(pAndroidApp.activity);
		pState = esWaitDestroy;
	}
}

void dealLauncher::ProcessActivityIntent(){
	/*
	 * NOTE how to deal with intent parameters send to the engine for example to
	 *      tell it what game to launch:
	 * 
	 * JNIEnv *env;
	 * //androidApp.activity->vm->AttachCurrentThread(&env, 0);
	 * //is this required? from what I gather native activities should be already attached
	 * 
	 * jobject me = androidApp.activity->clazz; // NativeActivity.this
	 * 
	 * jclass acl = env->GetObjectClass( me ); // NativeActivity.class
	 * jmethodID giid = env->GetMethodID( acl, "getIntent", "()Landroid/content/Intent;" );
	 * jobject intent = env->CallObjectMethod( me, giid ); // intent used to launch the engine
	 * 
	 * jclass icl = env->GetObjectClass( intent ); // class pointer of Intent
	 * jmethodID gseid = env->GetMethodID( icl, "getStringExtra", "(Ljava/lang/String;)Ljava/lang/String;" );
	 * 
	 * jstring jsParam1 = env->CallObjectMethod( intent, gseid, env->NewStringUTF( "MY_PARAM_1" ) );
	 * const char *Param1 = env->GetStringUTFChars( jsParam1, 0 );
	 * const decString param1( Param1 ); // since param is only temporary
	 * env->ReleaseStringUTFChars( jsParam1, Param1 );
	 * 
	 * 
	 * 
	 * jmethodID methodGetAction = env->GetMethodID( icl, "getAction", "()Ljava/lang/String;" );
	 * jstring jsActionName = env->CallObjectMethod( intent, methodGetAction );
	 * const char *jscActionName = env->GetStringUTFChars( jsActionName, 0 );
	 * const decString actionName( jscActionName );
	 * env->ReleaseStringUTFChars( jsActionName, jscActionName );
	 * 
	 * For example to run a game with the engine:
	 * actionName = "android.intent.action.RUN"
	 * data = Uri
	 * 
	 * 
	 * 
	 * basically this call chain converted to JNI:
	 * 
	 * final String actionName = intent.getAction();
	 * if (actionName.equals("android.intent.action.RUN")) {
	 *     final Uri uriGameData = intent.getData();
	 *     // debug: uriGameData.toString()
	 *     final AssetFileDescriptor assetFileDescriptor = getContentResolver().openAssetFileDescriptor(uriGameData, "r");
	 *     final int fdGameData = assetFileDescriptor.getFd();
	 *     // can throw FileNotFoundException
	 *     or
	 *     final ParcelFileDescriptor parcelFileDescriptor = getContentResolver().openFileDescriptor(uriGameData, "r");
	 *     final int fdGameData = parcelGameData.getFd();
	 *     // can throw FileNotFoundException
	 *     // which one to fetch could be encoded in the url, for example:
	 *     //   "content://{game-package-name}.degamearchive/asset/filename.dga"
	 *     //      -> to access using openAssetFileDescriptor
	 *     //   "content://{game-package-name}.degamearchive/file/filename.dga"
	 *     //      -> to access using openFileDescriptor
	 *     // NOTE: getContentResolver() has to be called on a context thus 'clazz' should work
	 *     // NOTE: fdGameData is an actual unix file descriptor and can be used in regular file function calls
	 * }
	 */
	
	try{
		// Intent intent = getIntent();
		jobject objActivity = pAndroidApp.activity->clazz;
		jniGlobalJClass clsActivity(pJniEnv, pJniEnv->GetObjectClass(objActivity));
		jmethodID metGetIntent = pJniEnv->GetMethodID(clsActivity, "getIntent", "()Landroid/content/Intent;");
		jniGlobalJObject objIntent(pJniEnv, pJniEnv->CallObjectMethod(objActivity, metGetIntent));
		
		// String action = intent.getAction();
		jniGlobalJClass clsIntent(pJniEnv, pJniEnv->GetObjectClass(objIntent));
		jmethodID metGetAction = pJniEnv->GetMethodID(clsIntent, "getAction", "()Ljava/lang/String;");
		const decString action(jniGlobalJString(pJniEnv,
			(jstring)pJniEnv->CallObjectMethod(objIntent, metGetAction)));
		
		// log intent information
		GetLogger().LogInfoFormat(LOGSOURCE, "Activity Intent: Action %s", action.GetString());
		
		if(action == "ch.dragondreams.intent.action.LaunchDelga"
		|| action == "android.intent.action.VIEW"){
			// store object to use later on depending on the content
			jmethodID metGetData = pJniEnv->GetMethodID(clsIntent, "getData", "()Landroid/net/Uri;");
			pObjUriIntentData.Set(pJniEnv, pJniEnv->CallObjectMethod(objIntent, metGetData));
		}
		
		pState = esInitialCheck;
		
	}catch(const deException &e){
		GetLogger().LogException(LOGSOURCE, e);
		ANativeActivity_finish(pAndroidApp.activity);
		pState = esWaitDestroy;
	}
}

bool dealLauncher::CheckInitialInstall(){
	char sha1sumInstalled[256];
	char sha1sumInstall[256];
	decString filename;
	
	// read SHA1 checksum of installation archive
	AAsset *asset = NULL;
	
	memset(sha1sumInstall, 0, 256);
	
	filename.Format("install_%s.sha1sum", ANDROID_JNIDIR);
	
	try{
		asset = AAssetManager_open(pAndroidApp.activity->assetManager, filename.GetString(), AASSET_MODE_STREAMING);
		if(!asset){
			DETHROW(deeOpenFile);
		}
		AAsset_read(asset, sha1sumInstall, 256);
		AAsset_close(asset);
		
	}catch(const deException &){
		if(asset){
			AAsset_close(asset);
		}
		throw;
	}
	
	// read SHA1 checksum of installation if present
	const decString pathSha1sumInstalled(decString(pAndroidApp.activity->internalDataPath) + "/installed.sha1sum");
	
	memset(sha1sumInstalled, 0, 256);
	
	if(access(pathSha1sumInstalled, F_OK) == 0){
		decDiskFileReader *reader = NULL;
		
		try{
			reader.TakeOver(new decDiskFileReader(pathSha1sumInstalled));
			reader->Read(sha1sumInstalled, reader->GetLength());
		}catch(const deException &){
			throw;
		}
	}
	
	// if the SHA1 checksums do not match begin installation
	if(memcmp(sha1sumInstalled, sha1sumInstall, 256) == 0){
		pDialogMain->GetTaskCheckEngine().SetStatus("Ready");
		pDialogMain->GetTaskCheckEngine().SetProgress(100);
		pDialogMain->GetTaskCheckEngine().SetFinished(true);
		return true;
		
	}else{
		pDialogMain->GetTaskCheckEngine().SetStatus("Installing");
		return false;
	}
}

void dealLauncher::StartInitialInstall(){
	if(pAssetUnpacker){
		return;
	}
	
	decString filename;
	filename.Format("install_%s.zip", ANDROID_JNIDIR);
	
	decStringList filter;
// 	decString filterLine;
// 	filterLine.Format( "%s/lib/*", ANDROID_JNIDIR );
// 	filter.Add( filterLine );
	
	GetLogger().LogInfo(LOGSOURCE, "Start initial installation. Remove old files...");
	dealRemoveDirTree removeDirTree(*pLogger);
	decPath pathDelTree;
	pathDelTree.SetFromNative(pAndroidApp.activity->internalDataPath);
	pathDelTree.AddComponent("armeabi-v7a");
	removeDirTree.Remove(pathDelTree.GetPathNative());
	
	GetLogger().LogInfo(LOGSOURCE, "Unpacking...");
	pAssetUnpacker = new dealAssetUnpacker(pLogger);
	pAssetUnpacker->StartUnpacking(pAndroidApp.activity->assetManager,
		filename.GetString(), pAndroidApp.activity->internalDataPath, filter);
}

bool dealLauncher::InitialInstallProgress(){
	bool unpacking = false;
	bool finished = false;
	float progress;
	
	// check progress
	pAssetUnpacker->UnpackProgress(unpacking, progress);
	
	pDialogMain->GetTaskCheckEngine().SetProgress((int)(progress * 100.0f));
	
	finished = !unpacking;
	
	// if unpacking finished stop the asset unpacker
	if(finished){
		StopAssetUnpacking();
		pDialogMain->GetTaskCheckEngine().SetStatus("Ready");
		pDialogMain->GetTaskCheckEngine().SetProgress(100);
		pDialogMain->GetTaskCheckEngine().SetFinished(true);
	}
	
	// if finished copy sha1 checksum to avoid installing again
	if(!finished){
		return unpacking;
	}
	
	const decString pathSha1sumInstalled(decString(pAndroidApp.activity->internalDataPath) + "/installed.sha1sum");
	decDiskFileWriter::Ref writer = NULL;
	char sha1sumInstall[256];
	AAsset *asset = NULL;
	decString filename;
	
	memset(sha1sumInstall, 0, 256);
	
	filename.Format("install_%s.sha1sum", ANDROID_JNIDIR);
	
	try{
		writer.TakeOver(new decDiskFileWriter(pathSha1sumInstalled, false));
		
		asset = AAssetManager_open(pAndroidApp.activity->assetManager, filename.GetString(), AASSET_MODE_STREAMING);
		if(!asset){
			DETHROW(deeOpenFile);
		}
		
		AAsset_read(asset, sha1sumInstall, 256);
		writer->Write(sha1sumInstall, strlen(sha1sumInstall));
		AAsset_close(asset);
		
	}catch(const deException &e){
		if(asset){
			AAsset_close(asset);
		}
		throw;
	}
	
	return unpacking;
}



void dealLauncher::StopAssetUnpacking(){
	if(!pAssetUnpacker){
		return;
	}
	
	GetLogger().LogInfo(LOGSOURCE, "Closing asset unpacker");
	pAssetUnpacker->StopUnpacking();
	delete pAssetUnpacker;
	pAssetUnpacker = NULL;
}



void dealLauncher::StartEngine(){
	if(!pAndroidApp.inputQueue){
		DETHROW(deeInvalidAction);
	}
	
	pGameManager.LoadGameList();
	pLocateGame();
	
	if(!pGame){
		GetLogger().LogError(LOGSOURCE, "Game missing.");
		pState = esMainMenu;
		return;
	}
	
	pEngine.Start(pGame->GetIdentifier());
	pEngine.PutEngineIntoVFS();
	pEngine.LoadModuleList();
	pEngine.LoadConfig();
	
	pGameManager.CreateDefaultProfile();
	pGameManager.LoadGameConfigs();
	pGameManager.Verify();
	
	// verify game if loaded from file which is the default on android
	if(!pGameManager.GetGameList().Has(pGame)){
		pGame->VerifyRequirements();
	}
}

void dealLauncher::StartGame(){
	// check if game is present. this should never fail
	if(!pGame){
		GetLogger().LogError(LOGSOURCE, "Game missing.");
		pState = esMainMenu;
		return;
	}
	
	// check if the game can run
	if(!pGame->GetCanRun()){
		GetLogger().LogInfoFormat(LOGSOURCE, "Game '%s' can not be run using profile '%s'",
			pGame->GetTitle().ToUTF8().GetString(), pProfile ? pProfile->GetName().GetString() : "<missing>");
		pPrintGameProblems();
		DETHROW(deeInvalidAction);
	}
	
	pLocateProfile();
	
	pWindowTitle = pGame->GetTitle().ToUTF8();
	
	GetLogger().LogInfoFormat(LOGSOURCE, "Starting game '%s' using profile '%s'",
		pGame->GetTitle().ToUTF8().GetString(), pProfile->GetName().GetString());
	
	pProfile->Activate(*this);
	
	// Temporary Hack!
	// fix path until this is properly handled
	pGame->SetGameDirectory(pAndroidApp.activity->internalDataPath);
	pGame->SetDataDirectory(pGame->GetDataDirectory());
	
	// activate script module and set the path.
	// NOTE currently we use the latest module version. profiles can not define the script
	//      module version since they can be applied to different games and thus on different
	//      script modules. to get a specific version the same trick has to be used as for
	//      multiple type modules by disabling/enabling specific module versions. this will
	//      be applied during profile activation above so we get the appropriate version here
	if(!pEngine.GetEngine()->ActivateModule(pGame->GetScriptModule(), "")){
		DETHROW(deeInvalidAction);
	}
	
	// set data directory. the engine expects an absolute native path. it is composed of the
	// data directory placed underneath the game directory
	decPath pathDataDir;
	pathDataDir.SetFromNative(pGame->GetGameDirectory());
	pathDataDir.AddUnixPath(pGame->GetDataDirectory());
	if(!pEngine.GetEngine()->SetDataDirectory(pathDataDir.GetPathNative())){
		DETHROW(deeInvalidAction);
	}
	if(!pEngine.GetEngine()->SetCmdLineArgs(pRunArguments + " " + pGame->GetRunArguments())){
		DETHROW(deeInvalidAction);
	}
	
	// open delga file to mount directories inside into the virtual file system
	if(!pEngine.GetEngine()->OpenDelga(pGameData->GetFileDescriptor(),
	pGameData->GetFileOffset(), pGameData->GetFileLength())){
		DETHROW(deeInvalidAction);
	}
	
	// set up virtual file system
	decPath filePath;
	filePath.SetFromUnix("/");
	filePath.AddUnixPath(pGame->GetDataDirectory());
	if(!pEngine.GetEngine()->VFSAddDelga("/", filePath.GetPathUnix())){
		DETHROW(deeInvalidAction);
	}
	
	if(!pEngine.GetEngine()->VFSAddScriptSharedDataDir()){
		DETHROW(deeInvalidAction);
	}
	
	DEASSERT_TRUE(pEngine.GetEngine()->ModulesAddVFSContainers(deModuleSystem::VFSStagePatches))
	DEASSERT_TRUE(pEngine.GetEngine()->ModulesAddVFSContainers(deModuleSystem::VFSStageMods))
	DEASSERT_TRUE(pEngine.GetEngine()->ModulesAddVFSContainers(deModuleSystem::VFSStageOverlay))
	
	filePath.SetFromNative(pConfiguration.GetPathConfigUser());
	filePath.AddComponent("games");
	filePath.AddComponent(pGame->GetIdentifier());
	filePath.AddComponent("overlay");
	if(!pEngine.GetEngine()->VFSAddDiskDir("/", filePath.GetPathNative(), false)){
		DETHROW(deeInvalidAction);
	}
	
	filePath.SetFromNative(pConfiguration.GetPathConfigUser());
	filePath.AddComponent("games");
	filePath.AddComponent(pGame->GetIdentifier());
	filePath.AddComponent("config");
	if(!pEngine.GetEngine()->VFSAddDiskDir(pGame->GetPathConfig(), filePath.GetPathNative(), false)){
		DETHROW(deeInvalidAction);
	}
	
	filePath.SetFromNative(pConfiguration.GetPathConfigUser());
	filePath.AddComponent("games");
	filePath.AddComponent(pGame->GetIdentifier());
	filePath.AddComponent("capture");
	if(!pEngine.GetEngine()->VFSAddDiskDir(pGame->GetPathCapture(), filePath.GetPathNative(), false)){
		DETHROW(deeInvalidAction);
	}
	
	// create render window. size has to match the screen size
	pRunWidth = pDisplay.GetWidth();
	pRunHeight = pDisplay.GetHeight();
	pRunFullScreen = true;
	
	if(pGame->GetWindowSize() != decPoint()){
		pRunWidth = pGame->GetWindowSize().x;
		pRunHeight = pGame->GetWindowSize().y;
		pRunFullScreen = false;
	}
	
	if(!pEngine.GetEngine()->CreateRenderWindow(pRunWidth, pRunHeight, pRunFullScreen, pWindowTitle)){
		DETHROW(deeInvalidAction);
	}
	
	// start game
	if(!pEngine.GetEngine()->StartGame(pGame->GetScriptDirectory(), pGame->GetGameObject())){
		DETHROW(deeInvalidAction);
	}
}

bool dealLauncher::UpdateFrame(){
	bool keepRunning = true;
	if(!pEngine.GetEngine()->FrameUpdate(keepRunning)){
		DETHROW(deeInvalidAction);
	}
	return keepRunning;
}

void dealLauncher::LaunchGame(){
	if(pState != esMainMenu){
		DETHROW(deeInvalidParam);
	}
	
	// check intent data
	if(!pObjUriIntentData){
		GetLogger().LogInfo(LOGSOURCE, "Intent Data URI is null");
		pState = esMainMenu;
		return;
	}
	
	pGameData.TakeOver(new dealGameData(*this, pObjUriIntentData));
	
	pState = esGameStart;
}

void dealLauncher::StopGame(){
	if(!pGame){
		return;
	}
	GetLogger().LogInfoFormat(LOGSOURCE, "Stop game '%s'", pGame->GetTitle().ToUTF8().GetString());
	pEngine.GetEngine()->StopGame(); 
}

void dealLauncher::StopEngine(){
	GetLogger().LogInfo(LOGSOURCE, "Stop engine");
	pEngine.Stop();
	GetLogger().LogInfo(LOGSOURCE, "Engine stopped");
}



void dealLauncher::InitWindow(){
	GetLogger().LogInfo(LOGSOURCE, "dealLauncher.InitWindow()");
	if(pState == esGameRunning){
		pEngine.InitAppWindow();
		return;
	}
	
	pDisplay.Init();
	
	if(!pDialogMain){
		pDialogMain.TakeOver(new dealDialogMain(pDisplay));
		pDisplay.SetDialog(pDialogMain);
	}
}

void dealLauncher::TerminateWindow(){
	GetLogger().LogInfo(LOGSOURCE, "dealLauncher.TerminateWindow()");
	if(pState == esGameRunning){
		pEngine.TerminateAppWindow();
		return;
	}
	
	pDisplay.Close();
}

bool dealLauncher::FrameLoop(){
	switch(pState){
	case esRestoreState:
		if(pAndroidApp.destroyRequested != 0){
			pState = esWaitDestroy;
			return true;
		}
		if(!HasAppWindow()){
			return true;
		}
		
		RestoreState();
		return true;
		
	case esProcessActivityIntent:
		if(pAndroidApp.destroyRequested != 0){
			pState = esWaitDestroy;
			return true;
		}
		if(!HasAppWindow()){
			return true;
		}
		
		ProcessActivityIntent();
		return true;
		
	case esInitialCheck:
		if(pAndroidApp.destroyRequested != 0){
			pState = esWaitDestroy;
			return true;
		}
		if(!HasAppWindow()){
			return true;
		}
		
		if(CheckInitialInstall()){
			pState = esMainMenu;
			pDialogMain->SetRunEnabled(true);
			
		}else{
			try{
				StartInitialInstall();
				pState = esInitialInstall;
				
			}catch(const deException &e){
				GetLogger().LogException(LOGSOURCE, e);
				pState = esQuit;
				return true;
			}
		}
		
		if(pDisplay.GetDialog()){
			pDisplay.GetDialog()->OnFrameUpdate();
		}
		pDisplay.Paint();
		return true;
		
	case esInitialInstall:
		if(pAndroidApp.destroyRequested != 0){
			pState = esWaitDestroy;
			return true;
		}
		if(!HasAppWindow()){
			return true;
		}
		
		if(!pAssetUnpacker){
			pState = esQuit;
			return true;
		}
		if(!InitialInstallProgress()){
			pState = esMainMenu;
			pDialogMain->SetRunEnabled(true);
		}
		
		if(pDisplay.GetDialog()){
			pDisplay.GetDialog()->OnFrameUpdate();
		}
		pDisplay.Paint();
		return true;
		
	case esMainMenu:
		if(pAndroidApp.destroyRequested != 0){
			pState = esWaitDestroy;
			return true;
		}
		if(!HasAppWindow()){
			return true;
		}
		
		try{
			pDisplay.Paint();
			
		}catch(const deException &e){
			GetLogger().LogException(LOGSOURCE, e);
			pState = esGameStop;
		}
		return true;
		
	case esGameStart:
		if(pAndroidApp.destroyRequested != 0){
			pState = esWaitDestroy;
			return true;
		}
		if(!HasAppWindow()){
			return true;
		}
		
		try{
			// close the display. this frees the window allowing the graphic module
			// to take over the application window.
			pDisplay.Close();
			
			// start engine and game
			StartEngine();
			StartGame();
			
			// force orientation
			//AConfiguration_setOrientation( pAndroidApp.config, ACONFIGURATION_ORIENTATION_LAND );
			// ACONFIGURATION_ORIENTATION_ANY
			// ACONFIGURATION_ORIENTATION_PORT
			// ACONFIGURATION_ORIENTATION_LAND
			// ACONFIGURATION_ORIENTATION_SQUARE
			
		}catch(const deException &e){
			GetLogger().LogException(LOGSOURCE, e);
			pState = esGameStop;
			//pDisplay.Paint(); // useless. display is closed and will be restarted during esGameStop
			return true;
		}
		
		pState = esGameRunning;
		return true;
		
	case esGameRunning:
		if(pAndroidApp.destroyRequested != 0){
			GetLogger().LogInfo(LOGSOURCE, "Destroy Requested");
			pState = esGameStop;
			return true;
		}
		if(!HasAppWindow()){
			return true;
		}
		
		try{
			if(!UpdateFrame()){
				pState = esGameStop;
			}
			
		}catch(const deException &e){
			GetLogger().LogException(LOGSOURCE, e);
			pState = esGameStop;
		}
		
		return true;
		
	case esGameStop:
		try{
			// stop game and engine
			StopGame();
			StopEngine();
			
			// restart the display to take it over from the graphic module
			// TODO this currently fails to init. maybe window is not ready anymore?
			//      make this better so we can take over and inform the user about the problem
			GetLogger().LogInfo(LOGSOURCE, "Restart Display");
			pDisplay.Init();
			
		}catch(const deException &e){
			GetLogger().LogException(LOGSOURCE, e);
		}
		
		//pState = esQuit;
		pState = esMainMenu; // temporary hack. go back to main menu
		pDisplay.Paint();
		
		return true;
		
	case esQuit:
		GetLogger().LogInfo(LOGSOURCE, "Quit application");
		ANativeActivity_finish(pAndroidApp.activity);
		pState = esWaitDestroy;
		return true;
		
	case esWaitDestroy:
		if(pAndroidApp.destroyRequested != 0){
			pState = esDestroyed;
			GetLogger().LogInfo(LOGSOURCE, "Received application quit from system");
			return false;
		}
		return true;
		
	case esDestroyed:
	default:
		return false;
	}
}

void dealLauncher::ProcessInputEvent(const AInputEvent &event){
	if(pState == esGameRunning){
		pEngine.ProcessInputEvent(event);
		return;
	}
	
	switch(AInputEvent_getType(&event)){
	case AINPUT_EVENT_TYPE_KEY:{
		const int key = AKeyEvent_getKeyCode(&event);
		
		switch(AKeyEvent_getAction(&event)){
		case AKEY_EVENT_ACTION_DOWN:
			GetLogger().LogInfoFormat(LOGSOURCE, "AKEY_EVENT_ACTION_DOWN key=%i", key);
			if(key == AKEYCODE_BACK){
				if(pDisplay.GetDialog()){
					pDisplay.SetDialog(pDisplay.GetDialog()->BackKeyDialog());
				}
				if(!pDisplay.GetDialog()){
					ExitApplication();
				}
				
			}else{
			}
			break;
			
		case AKEY_EVENT_ACTION_UP:
			GetLogger().LogInfoFormat(LOGSOURCE, "AKEY_EVENT_ACTION_UP key=%i", key);
			break;
			
		case AKEY_EVENT_ACTION_MULTIPLE:
			 // Multiple duplicate key events have occurred in a row, or a complex string is
			 // being delivered.  The repeat_count property of the key event contains the number
			 // of times the given key code should be executed.
			GetLogger().LogInfoFormat(LOGSOURCE, "AKEY_EVENT_ACTION_MULTIPLE key=%i count=%i", key, AKeyEvent_getRepeatCount(&event));
			break;
			
		default:
			break;
		}
		}break;
		
	case AINPUT_EVENT_TYPE_MOTION:
		switch(AMotionEvent_getAction(&event) & AMOTION_EVENT_ACTION_MASK){
		case AMOTION_EVENT_ACTION_DOWN:{
			const int buttons = AMotionEvent_getButtonState(&event);
			const decPoint position(AMotionEvent_getX(&event, 0), AMotionEvent_getY(&event, 0));
			
			if(pDisplay.GetDialog()){
				dealWidget * const content = pDisplay.GetDialog()->GetContent();
				dealWidget * const widget = content->WidgetAtPosition(position - content->GetPosition());
				
				if(widget){
					widget->OnMousePress(buttons, position - widget->GetScreenPosition());
				}
			}
			
			/*
			if((buttonstate & AMOTION_EVENT_BUTTON_PRIMARY) == AMOTION_EVENT_BUTTON_PRIMARY){
				return 0;
				
			}else if((buttonstate & AMOTION_EVENT_BUTTON_SECONDARY) == AMOTION_EVENT_BUTTON_SECONDARY){
				return 1;
				
			}else if((buttonstate & AMOTION_EVENT_BUTTON_TERTIARY) == AMOTION_EVENT_BUTTON_TERTIARY){
				return 2;
				
			}else if((buttonstate & AMOTION_EVENT_BUTTON_BACK) == AMOTION_EVENT_BUTTON_BACK){
				return 3;
				
			}else if((buttonstate & AMOTION_EVENT_BUTTON_FORWARD) == AMOTION_EVENT_BUTTON_FORWARD){
				return 4;
				
			}else{
				return 0xff; // invalid button
			}
			*/
			
			//GetLogger().LogInfoFormat( LOGSOURCE, "AMOTION_EVENT_ACTION_DOWN buttonState=%i", buttons );
			}break;
			
		case AMOTION_EVENT_ACTION_UP:{
			const int buttons = AMotionEvent_getButtonState(&event);
			const decPoint position(AMotionEvent_getX(&event, 0), AMotionEvent_getY(&event, 0));
			
			if(pDisplay.GetDialog()){
				dealWidget * const content = pDisplay.GetDialog()->GetContent();
				dealWidget * const widget = content->WidgetAtPosition(position - content->GetPosition());
				
				if(widget){
					widget->OnMouseRelease(buttons, position - widget->GetScreenPosition());
				}
			}
			
			//GetLogger().LogInfoFormat( LOGSOURCE, "AMOTION_EVENT_ACTION_UP buttonState=%i", buttons );
			}break;
			
		case AMOTION_EVENT_ACTION_MOVE:{ // movement while touching screen
			const int button = AMotionEvent_getButtonState(&event);
			const decPoint position(AMotionEvent_getX(&event, 0), AMotionEvent_getY(&event, 0));
			dealWidget * const widget = pDisplay.GetCaptureWidget();
			
			if(widget && pDisplay.GetDialog()){
				widget->OnMouseMove(button, position - widget->GetScreenPosition());
			}
			
			//GetLogger().LogInfoFormat( LOGSOURCE, "AMOTION_EVENT_ACTION_MOVE buttonState=%i", button );
			}break;
			
		case AMOTION_EVENT_ACTION_HOVER_MOVE: // movement while not touching screen
			break;
			// these are absolute positions. need to track difference to previous values
			// most probably has to use AMotionEvent_getHistoricalX, AMotionEvent_getHistoricalY
			// and AMotionEvent_getHistorySize, whereas history exists only for MOVE not HOVER_MOVE
			//engine->state.x = AMotionEvent_getX(event, 0);
			//engine->state.y = AMotionEvent_getY(event, 0);
			break;
			
		case AMOTION_EVENT_ACTION_CANCEL:
			// gesture stopped. documentation claims this is the same as AMOTION_EVENT_ACTION_UP
			// but no action should be done like in that case. no idea what this is supposed to mean
			GetLogger().LogInfoFormat(LOGSOURCE, "AMOTION_EVENT_ACTION_CANCEL buttonState=%i",
				AMotionEvent_getButtonState(&event));
			break;
			
		case AMOTION_EVENT_ACTION_OUTSIDE:
			// movement outside of the screen. can this be called between up and down?
			break;
			
		case AMOTION_EVENT_ACTION_POINTER_DOWN:
			// non primary pointer. is this the case for stylus?
			GetLogger().LogInfoFormat(LOGSOURCE, "AMOTION_EVENT_ACTION_POINTER_DOWN buttonState=%i",
				AMotionEvent_getButtonState(&event));
			break;
			
		case AMOTION_EVENT_ACTION_POINTER_UP:
			// non primary pointer. is this the case for stylus?
			GetLogger().LogInfoFormat(LOGSOURCE, "AMOTION_EVENT_ACTION_POINTER_UP buttonState=%i",
				AMotionEvent_getButtonState(&event));
			break;
			
		case AMOTION_EVENT_ACTION_SCROLL:
			// The motion event contains relative vertical and/or horizontal scroll offsets.
			// Use getAxisValue to retrieve the information from AMOTION_EVENT_AXIS_VSCROLL
			// and AMOTION_EVENT_AXIS_HSCROLL.
			// The pointer may or may not be down when this event is dispatched.
			// This action is always delivered to the winder under the pointer, which
			// may not be the window currently touched.
			GetLogger().LogInfoFormat(LOGSOURCE, "AMOTION_EVENT_ACTION_SCROLL axisX=%f axisY=%f",
				AMotionEvent_getAxisValue(&event, AMOTION_EVENT_AXIS_HSCROLL, 0),
				AMotionEvent_getAxisValue(&event, AMOTION_EVENT_AXIS_VSCROLL, 0));
			break;
			
		case AMOTION_EVENT_ACTION_HOVER_ENTER:
			// not touching but enternig window
			break;
			
		case AMOTION_EVENT_ACTION_HOVER_EXIT:
			// not touching but enternig window
			break;
			
		default:
			break;
		}
		break;
		
	default:
		break;
	}
}

void dealLauncher::FocusGained(){
	GetLogger().LogInfo(LOGSOURCE, "dealLauncher.FocusGained()");
	pFocused = true;
	
	if(pState == esGameRunning){
		pEngine.FocusGained();
	}
}

void dealLauncher::FocusLost(){
	GetLogger().LogInfo(LOGSOURCE, "dealLauncher.FocusLost()");
	pFocused = false;
	
	if(pState == esGameRunning){
		pEngine.FocusLost();
	}
}

void dealLauncher::AppStart(){
	GetLogger().LogInfo(LOGSOURCE, "dealLauncher.AppStart()");
}

void dealLauncher::AppStop(){
	GetLogger().LogInfo(LOGSOURCE, "dealLauncher.AppStop()");
}

void dealLauncher::AppPause(){
	GetLogger().LogInfo(LOGSOURCE, "dealLauncher.AppPause()");
	pFrozen = true;
	
	if(pState == esGameRunning){
		pEngine.Freeze();
	}
}

void dealLauncher::AppResume(){
	GetLogger().LogInfo(LOGSOURCE, "dealLauncher.AppResume()");
	pFrozen = false;
	
	if(pState == esGameRunning){
		pEngine.Thaw();
	}
}



void dealLauncher::SaveState(){
	GetLogger().LogInfo(LOGSOURCE, "dealLauncher.SaveState()");
	
	decMemoryFileWriter *writer = NULL;
	decMemoryFile::Ref data = NULL;
	
	// write state to memory file
	try{
		data.TakeOver(new decMemoryFile("launcher-state.dat"));
		writer.TakeOver(new decMemoryFileWriter(data, false));
		pWriteState(*writer);
		
		// create copy of data for android to tuck away
		pAndroidApp.savedState = malloc(data->GetLength());
		memcpy(pAndroidApp.savedState, data->GetPointer(), data->GetLength());
		pAndroidApp.savedStateSize = data->GetLength();
		
		GetLogger().LogInfoFormat(LOGSOURCE, "Saved launcher state (%i bytes)", data->GetLength());
		writer = NULL;
	}catch(const deException &e){
		GetLogger().LogException(LOGSOURCE, e);
		return;
	}
}

void dealLauncher::ConfigChanged(){
	GetLogger().LogInfo(LOGSOURCE, "dealLauncher.ConfigChanged()");
	if(pState != esGameRunning){
		pDisplay.ConfigChanged();
	}
}

void dealLauncher::InputChanged(){
	GetLogger().LogInfo(LOGSOURCE, "dealLauncher.InputChanged()");
}



void dealLauncher::ExitApplication(){
	switch(pState){
	case esGameStart:
	case esGameRunning:
		pState = esGameStop;
		break;
		
	default:
		pState = esQuit;
	}
}



// Private Functions
//////////////////////

void dealLauncher::pCleanUp(){
	GetLogger().LogInfo(LOGSOURCE, "Clean up launcher");
	
	try{
		StopEngine();
		
		StopAssetUnpacking();
		if(pModuleParameters){
			delete pModuleParameters;
		}
		
		if(pDialogMain){
			pDialogMain = NULL;
		}
		
		pDisplay.Close(); // this is required since close potentially accesses the logger destroyed below
		
	}catch(const deException &e){
		GetLogger().LogException(LOGSOURCE, e);
	}
	
	if(pGameData){
		pGameData = NULL;
	}
	pObjUriIntentData.Clear();
	
	if(pJniEnv){
		pAndroidApp.activity->vm->DetachCurrentThread();
		pJniEnv = NULL;
	}
	pLogger = NULL; // this is vital or GetLogger() does not fall back to the null logger causing segfaults
}

void dealLauncher::pInitLogger(){
	deLoggerConsole::Ref loggerConsole = NULL;
	decBaseFileWriter::Ref fileWriter = NULL;
	deLoggerChain::Ref loggerChain = NULL;
	deLoggerFile::Ref loggerFile = NULL;
	
	bool useConsole = true;
	bool useFile = true;
	decPath pathLogFile;
	
	try{
		// chain logger
		loggerChain.TakeOver(new deLoggerChain);
		
		// console logger
		if(useConsole){
			loggerConsole.TakeOver(new deLoggerConsole);
			loggerChain->AddLogger(loggerConsole);
			loggerConsole = NULL;
		}
		
		// file logger
		if(useFile){
			pathLogFile.SetFromUnix("/logs/delauncherandroid.log");
			fileWriter = pFileSystem->OpenFileForWriting(pathLogFile);
			
			loggerFile.TakeOver(new deLoggerFile(fileWriter));
			fileWriter = NULL;
			
			loggerChain->AddLogger(loggerFile);
			loggerFile = NULL;
		}
		
		// set the logger
		pLogger = loggerChain;
		
	}catch(const deException &){
		throw;
	}
}

void dealLauncher::pLocateGame(){
	if(!pGameData){
		GetLogger().LogError(LOGSOURCE, "Game Data missing.");
		DETHROW(deeInvalidParam);
	}
	
	if(pGameData->GetGames().GetCount() == 0){
		GetLogger().LogError(LOGSOURCE, "No games found in game data.");
		DETHROW(deeInvalidParam);
	}
	
	// for the time being use the first game. usually there is one but it is allowed to
	// have multiple games in one game data. for this case a selection screen is needed
	dealGame::Ref game = pGameData->GetGames().GetAt(0);
	
	/* by game identifier
	pGame = pGameManager.GetGameList().GetWithID(gameIdentifier);
	*/
	
	dealGame * const gameCheck = pGameManager.GetGameList().GetWithID(game->GetIdentifier());
	
	if(gameCheck){
		gameCheck->AddReference();
		pGame = gameCheck;
		
	}else{
		// game->VerifyRequirements(); // not possible here. has to be done after engine started
		pGame = game;
	}
	
	// final check
	if(!pGame){
		GetLogger().LogError(LOGSOURCE, "No game found to run");
		DETHROW(deeInvalidParam);
	}
}

void dealLauncher::pLocateProfile(){
	// locate the profile to run
	if(pProfileName.IsEmpty()){
		pProfile = pGame->GetProfileToUse();
		
	}else{
		pProfile = pGame->GetProfileList().GetNamed(pProfileName);
		
		if(!pProfile){
			pProfile = pGameManager.GetProfileList().GetNamed(pProfileName);
		}
		
		if(!pProfile){
			GetLogger().LogErrorFormat(LOGSOURCE, "No profile found with name '%s'", pProfileName.GetString());
			DETHROW(deeInvalidParam);
		}
	}
	if(!pProfile->GetValid()){
//		PrintProfileProblems();
		DETHROW(deeInvalidAction);
	}
	
	// udpate the run parameters
	if(pProfile->GetReplaceRunArguments()){
		pRunArguments = pProfile->GetRunArguments();
		
	}else{
		pRunArguments.Format("%s %s", pGame->GetRunArguments().GetString(), pProfile->GetRunArguments().GetString());
	}
	
	pRunWidth = pProfile->GetWidth();
	pRunHeight = pProfile->GetHeight();
	pRunFullScreen = pProfile->GetFullScreen();
}

void dealLauncher::pPrintGameProblems() const{
	const dealFileFormatList &fileFormatList = pGame->GetFileFormatList();
	const int fileFormatCount = fileFormatList.GetFormatCount();
	deLogger &logger = GetLogger();
	int i;
	
	logger.LogErrorFormat(LOGSOURCE, "Game '%s' has the following problems:", pGame->GetIdentifier().GetString());
	
	for(i=0; i<fileFormatCount; i++){
		const dealFileFormat &fileFormat = *fileFormatList.GetFormatAt(i);
		
		if(!fileFormat.GetSupported()){
			if(dealEngineModule::IsSingleType(fileFormat.GetType())){
				logger.LogErrorFormat(LOGSOURCE, "- File Format '%s' defines single type %s",
					fileFormat.GetPattern().GetString(), dealEngineModule::GetStringForType(fileFormat.GetType()));
				
			}else{
				logger.LogErrorFormat(LOGSOURCE, "- File Format '%s' is not supported by any loaded modules",
					fileFormat.GetPattern().GetString());
			}
		}
	}
	
	if(!pGame->GetScriptModuleFound()){
		pPrintModuleProblem(pGame->GetScriptModule(), dealEngineModule::emtScript);
	}
}


void dealLauncher::pPrintProfileProblems() const{
	deLogger &logger = GetLogger();
	
	logger.LogErrorFormat(LOGSOURCE, "Profile '%s' has the following problems:", pProfile->GetName().GetString());
	
	pPrintModuleProblem(pProfile->GetModuleGraphic(), dealEngineModule::emtGraphic);
	pPrintModuleProblem(pProfile->GetModuleInput(), dealEngineModule::emtInput);
	pPrintModuleProblem(pProfile->GetModulePhysics(), dealEngineModule::emtPhysics);
	pPrintModuleProblem(pProfile->GetModuleAnimator(), dealEngineModule::emtAnimator);
	pPrintModuleProblem(pProfile->GetModuleAI(), dealEngineModule::emtAI);
	pPrintModuleProblem(pProfile->GetModuleCrashRecovery(), dealEngineModule::emtCrashRecovery);
	pPrintModuleProblem(pProfile->GetModuleAudio(), dealEngineModule::emtAudio);
	pPrintModuleProblem(pProfile->GetModuleSynthesizer(), dealEngineModule::emtSynthesizer);
	pPrintModuleProblem(pProfile->GetModuleNetwork(), dealEngineModule::emtNetwork);
}

void dealLauncher::pPrintModuleProblem(const char *moduleName, dealEngineModule::eModuleTypes moduleType) const{
	dealEngineModule * const module = pEngine.GetModuleList().GetModuleNamed(moduleName);
	deLogger &logger = GetLogger();
	
	if(!module){
		logger.LogErrorFormat(LOGSOURCE, "- %s module '%s' does not exist",
			dealEngineModule::GetStringForType(moduleType), moduleName);
		
	}else if(module->GetType() != dealEngineModule::emtScript){
		logger.LogErrorFormat(LOGSOURCE, "- Module '%s' is not a %s module",
			moduleName, dealEngineModule::GetStringForType(moduleType));
		
	}else if(module->GetStatus() == dealEngineModule::emsReady){
		const char *reason = "Unknown problem";
		
		if(module->GetErrorCode() == dealEngineModule::eecCreateModuleFailed){
			reason = "Creating module failed";
			
		}else if(module->GetStatus() == dealEngineModule::eecLibFileNotFound){
			reason = "Module library file could not be found";
			
		}else if(module->GetStatus() == dealEngineModule::eecLibFileNotRegularFile){
			reason = "Module library file is not a regular library file";
			
		}else if(module->GetStatus() == dealEngineModule::eecLibFileSizeMismatch){
			reason = "Size of the module library file does not match the size in the module definition";
			
		}else if(module->GetStatus() == dealEngineModule::eecLibFileCheckSumMismatch){
			reason = "Checksum of the module library file does not match the checksum in the module definition";
			
		}else if(module->GetStatus() == dealEngineModule::eecLibFileOpenFailed){
			reason = "Module library file could not be opened";
			
		}else if(module->GetStatus() == dealEngineModule::eecLibFileEntryPointNotFound){
			reason = "Entry point function specified in the module definition could not be found in the library file";
			
		}else if(module->GetStatus() == dealEngineModule::eecLibFileCreateModuleFailed){
			reason = "Creating the module using the entry point function failed";
		}
		
		logger.LogErrorFormat(LOGSOURCE, "- %s module '%s' is not working (%s)",
			dealEngineModule::GetStringForType(moduleType), moduleName, reason);
	}
}

void dealLauncher::pReadState(decBaseFileReader &reader){
}

void dealLauncher::pWriteState(decBaseFileWriter &writer){
	writer.WriteInt(0); // just so the save state is not empty
}
