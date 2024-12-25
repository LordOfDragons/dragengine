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

#include <dragengine/dragengine_configuration.h>

#include <stdlib.h>
#include <string.h>

#include "delLauncher.h"

#ifdef OS_ANDROID
#include "engine/delEngineInstanceDirect.h"
#else
#include "engine/delEngineInstanceThreaded.h"
#endif

#include "game/delGame.h"
#include "game/icon/delGameIcon.h"
#include "game/profile/delGameProfile.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/logger/deLoggerChain.h>
#include <dragengine/logger/deLoggerConsole.h>
#include <dragengine/logger/deLoggerConsoleColor.h>
#include <dragengine/logger/deLoggerFile.h>

#ifdef OS_W32
#include <shlobj.h>
#include <dragengine/app/deOSWindows.h>
#endif


// Class delLauncher
//////////////////////

#ifdef OS_ANDROID
delLauncher::delLauncher(const sConfig &config) :
pConfig(config),
pLoggerHistory(delLoggerHistory::Ref::New(new delLoggerHistory)),
pLogSource(config.loggerSource),
pEngine(*this, config.engineLogFileTitle),
pGameManager(*this),
pPatchManager(*this)
{
	DEASSERT_FALSE(config.engineLogFileTitle.IsEmpty())
	DEASSERT_FALSE(config.loggerSource.IsEmpty())
	DEASSERT_FALSE(config.pathConfig.IsEmpty())
	DEASSERT_FALSE(config.pathGames.IsEmpty())
	DEASSERT_FALSE(config.pathLauncher.IsEmpty())
	
	DEASSERT_NOTNULL(config.osConfig.javavm)
	DEASSERT_NOTNULL(config.osConfig.nativeWindow)
	DEASSERT_FALSE(config.osConfig.pathCache.IsEmpty())
	DEASSERT_FALSE(config.osConfig.pathConfig.IsEmpty())
	DEASSERT_FALSE(config.osConfig.pathEngine.IsEmpty())
	
	try{
		pLogger.TakeOver(new deLoggerChain);
		pLogger->AddLogger(deLoggerConsole::Ref::New(new deLoggerConsole));
		pLogger->AddLogger(pLoggerHistory);
		
		const delEngineInstanceDirect::Factory::Ref factory(
			delEngineInstanceDirect::Factory::Ref::New(new delEngineInstanceDirect::Factory));
		factory->SetConfig(config.osConfig);
		pEngineInstanceFactory = factory;
		
		pLocatePath();
		pInitVFS();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

#else
delLauncher::delLauncher(const char *loggerSource, const char *engineLogFileTitle) :
pLoggerHistory(delLoggerHistory::Ref::New(new delLoggerHistory)),
pLogSource(loggerSource),
pEngine(*this, engineLogFileTitle),
pGameManager(*this),
pPatchManager(*this),
pEngineInstanceFactory(delEngineInstance::Factory::Ref::New(new delEngineInstanceThreaded::Factory))
{
	try{
		pLogger.TakeOver(new deLoggerChain);
		pLogger->AddLogger(deLoggerConsoleColor::Ref::New(new deLoggerConsoleColor));
		pLogger->AddLogger(pLoggerHistory);
		
		pLocatePath();
		pInitVFS();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}
#endif

delLauncher::~delLauncher(){
	pCleanUp();
}



// Management
///////////////

void delLauncher::AddFileLogger(const char *filetitle){
	DEASSERT_NOTNULL(filetitle)
	DEASSERT_TRUE(strlen(filetitle) > 0)
	
	decString filename;
	filename.Format("/logs/%s.log", filetitle);
	
	pLogger->AddLogger(deLoggerFile::Ref::New(new deLoggerFile(decBaseFileWriter::Ref::New(
		pVFS->OpenFileForWriting(decPath::CreatePathUnix(filename))))));
}

void delLauncher::SetEngineInstanceFactory(delEngineInstance::Factory *factory){
	DEASSERT_NOTNULL(factory)
	pEngineInstanceFactory = factory;
}

void delLauncher::Prepare(){
	pLogger->LogInfoFormat(pLogSource, "System config path: %s", pPathConfigSystem.GetString());
	pLogger->LogInfoFormat(pLogSource, "User config path: %s", pPathConfigUser.GetString());
	pLogger->LogInfoFormat(pLogSource, "Shares path: %s", pPathShares.GetString());
	pLogger->LogInfoFormat(pLogSource, "Games path: %s", pPathGames.GetString());
	pLogger->LogInfoFormat(pLogSource, "Logs path: %s", pPathLogs.GetString());
	
	{
	const delEngineInstance::Ref instance(delEngineInstance::Ref::New(
		pEngineInstanceFactory->CreateEngineInstance(*this, pEngine.GetLogFile())));
	instance->StartEngine();
	instance->LoadModules();
	
	pEngine.PutEngineIntoVFS(instance);
	
	pLogger->LogInfoFormat(pLogSource, "Engine config path = '%s'", pEngine.GetPathConfig().GetString());
	pLogger->LogInfoFormat(pLogSource, "Engine share path = '%s'", pEngine.GetPathShare().GetString());
	pLogger->LogInfoFormat(pLogSource, "Engine lib path = '%s'", pEngine.GetPathLib().GetString());
	
	pEngine.UpdateResolutions(instance);
	pEngine.UpdateScaleFactor(instance);
	pEngine.ReloadModules();
	pEngine.CheckModules(instance);
	pEngine.LoadConfig();
	
	pGameManager.CreateDefaultProfile();
	
	pGameManager.LoadGames(instance);
	pPatchManager.LoadPatches(instance);
	}
	
	pGameManager.LoadGameConfigs();
	pGameManager.Verify();
}



delGame * delLauncher::CreateGame(){
	return new delGame(*this);
}

delGameProfile *delLauncher::CreateGameProfile(const delGameProfile *copyFrom){
	return copyFrom ? new delGameProfile(*copyFrom) : new delGameProfile;
}

delGameIcon *delLauncher::CreateGameIcon(int size, const char *path){
	return new delGameIcon(size, path);
}



// Private Functions
//////////////////////

void delLauncher::pCleanUp(){
	pPatchManager.Clear();
	pGameManager.Clear();
	pEngine.Clear();
	
	pLogger = nullptr;
	pVFS = nullptr;
}

void delLauncher::pLocatePath(){
#ifdef OS_ANDROID
	pPathConfigSystem.Format("%s/etc/delauncher", pConfig.pathLauncher.GetString());
	pPathConfigUser = pConfig.pathConfig;
	pPathShares.Format("%s/share/delauncher", pConfig.pathLauncher.GetString());
	pPathGames = pConfig.pathGames;
	pPathLogs.Format("%s/logs", pConfig.pathConfig.GetString());
	
#else
	decPath path;

#ifdef OS_W32
	TCHAR value[ 256 ];
#else
	const char *value;
#endif
	
	// the system wide configuration directory is hard coded at compile time.
	// can be changed at runtime using an environment parameter.
	pPathConfigSystem = LAUNCHER_CONFIG_PATH;
#ifdef OS_W32
	const decString pathEngineBase(deOSWindows().GetPathEngineBase());
	
	//pPathConfigSystem = deOSWindows::GetRegistryValue("SOFTWARE\\Drag[en]gine",
	//	"PathLauncherConfig", pPathConfigSystem);
	pPathConfigSystem = pathEngineBase + "\\Launchers\\Config";
#endif
	
#ifdef OS_W32
	if(GetEnvironmentVariable(L"DELAUNCHER_SYS_CONFIG", &value[ 0 ], sizeof(value))){
		pPathConfigSystem = deOSWindows::WideToUtf8(value);
	}
#else
	value = getenv("DELAUNCHER_SYS_CONFIG");
	if(value){
		pPathConfigSystem = value;
	}
#endif

#ifdef OS_W32
	pPathConfigSystem = deOSWindows::ParseNativePath(pPathConfigSystem);
#endif
	
#ifdef OS_W32
	pPathConfigUser = "@RoamingAppData\\DELaunchers\\Config";
	
#else
	// the user configuration directory is located under the user home directory.
	// can be changed at runtime using an environment parameter.
	value = getenv("HOME");
	if(value){
		path.SetFromNative(value);
		
	}else{
		value = getenv("USER");
		
		if(value){
			path.SetFromNative("/home");
			path.AddComponent(value);
			
		}else{
			value = getenv("LOGUSER");
			
			if(value){
				path.SetFromNative("/home");
				path.AddComponent(value);
			}
		}
	}
	
	if(path.GetComponentCount() > 0){
		path.AddComponent(".config");
		path.AddComponent("delauncher");
		pPathConfigUser = path.GetPathNative();
	}
#endif
	
#ifdef OS_W32
	if(GetEnvironmentVariable(L"DELAUNCHER_USER_CONFIG", &value[ 0 ], sizeof(value))){
		pPathConfigUser = deOSWindows::WideToUtf8(value);
	}
#else
	value = getenv("DELAUNCHER_USER_CONFIG");
	if(value){
		pPathConfigUser = value;
	}
#endif

#ifdef OS_W32
	pPathConfigUser = deOSWindows::ParseNativePath(pPathConfigUser);
#endif
	
	// the shares directory is hard coded at compile time. can be changed at runtime
	// using an environment parameter.
	pPathShares = LAUNCHER_SHARE_PATH;
#ifdef OS_W32
	pPathShares = pathEngineBase + "\\Launchers\\Share";
	
	//pPathShares = deOSWindows::GetRegistryValue("SOFTWARE\\Drag[en]gine",
	//	"PathLauncherShares", pPathShares);
#endif
	
#ifdef OS_W32
	if(GetEnvironmentVariable(L"DELAUNCHER_SHARES", &value[ 0 ], sizeof(value))){
		pPathShares = deOSWindows::WideToUtf8(value);
	}
#else
	value = getenv("DELAUNCHER_SHARES");
	if(value){
		pPathShares = value;
	}
#endif

#ifdef OS_W32
	pPathShares = deOSWindows::ParseNativePath(pPathShares);
#endif
	
	// the games directory is hard coded at compile time. can be changed at runtime
	// using an environment parameter.
	pPathGames = LAUNCHER_GAMES_PATH;
#ifdef OS_W32
	pPathGames = deOSWindows::GetRegistryValue("SOFTWARE\\Drag[en]gine",
		"PathLauncherGames", pPathGames);
#endif
	
#ifdef OS_W32
	if(GetEnvironmentVariable(L"DELAUNCHER_GAMES", &value[ 0 ], sizeof(value))){
		pPathGames = deOSWindows::WideToUtf8(value);
	}
#else
	value = getenv("DELAUNCHER_GAMES");
	if(value){
		pPathGames = value;
	}
#endif

#ifdef OS_W32
	pPathGames = deOSWindows::ParseNativePath(pPathGames);
#endif
	
	// the logs directory is located right under the user configuration directory.
	// can be changed at runtime using an environment parameter
	path.SetFromNative(pPathConfigUser);
	path.AddComponent("logs");
	pPathLogs = path.GetPathNative();
#ifdef OS_W32
	pPathLogs = "@LocalAppData\\DELaunchers\\Logs";
#endif
	
#ifdef OS_W32
	if(GetEnvironmentVariable(L"DELAUNCHER_LOGS", &value[ 0 ], sizeof(value))){
		pPathLogs = deOSWindows::WideToUtf8(value);
	}
#else
	value = getenv("DELAUNCHER_LOGS");
	if(value){
		pPathLogs = value;
	}
#endif

#ifdef OS_W32
	pPathLogs = deOSWindows::ParseNativePath(pPathLogs);
#endif
#endif // OS_ANDROID
}

void delLauncher::pInitVFS(){
	pVFS.TakeOver(new deVirtualFileSystem);
	
	// add the found path to the virtual file system. this makes it easier to find the
	// files later on without having to deal with file system specific quirks
	
	// add the configuration containers. the containers are added in separate locations
	// as we want to read the config files one by one and mapping both containers to
	// the same path would shadow the system config files.
	if(! pPathConfigSystem.IsEmpty()){
		pVFS->AddContainer(deVFSDiskDirectory::Ref::New(new deVFSDiskDirectory(
			decPath::CreatePathUnix("/config/system"),
			decPath::CreatePathNative(pPathConfigSystem), true)));
	}
	
	if(! pPathConfigUser.IsEmpty()){
		pVFS->AddContainer(deVFSDiskDirectory::Ref::New(new deVFSDiskDirectory(
			decPath::CreatePathUnix("/config/user"),
			decPath::CreatePathNative(pPathConfigUser), false)));
	}
	
	// add the data directory. currently there exists only one which is the system shares
	// directory. a user one could be layered on top of it though if required later on.
	// the shares container is set to read-write as the launcher has to potentiall install
	// new games or uninstall them
	if(! pPathShares.IsEmpty()){
		pVFS->AddContainer(deVFSDiskDirectory::Ref::New(new deVFSDiskDirectory(
			decPath::CreatePathUnix("/data"),
			decPath::CreatePathNative(pPathShares), false)));
	}
	
	// add the logs directory. this is read-write
	if(! pPathLogs.IsEmpty()){
		pVFS->AddContainer(deVFSDiskDirectory::Ref::New(new deVFSDiskDirectory(
			decPath::CreatePathUnix("/logs"),
			decPath::CreatePathNative(pPathLogs), false)));
	}
}
