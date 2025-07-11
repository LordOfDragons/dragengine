/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _DEWLLAUNCHER_H_
#define _DEWLLAUNCHER_H_

#include "dewlLoggerJS.h"

#include <string>
#include <vector>
#include <memory>
#include <pthread.h>

#include <delauncher/delLauncher.h>
#include <delauncher/game/delGame.h>
#include <delauncher/game/delGameRunParams.h>
#include <delauncher/game/profile/delGameProfile.h>

#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#include <dragengine/logger/deLogger.h>

#include <emscripten/bind.h>

namespace es = emscripten;


/**
 * Web Launcher Class.
 */
class dewlLauncher : public delLauncher{
public:
	enum class Commands{
		RunGame
	};
	
	enum class eResultCodes{
		Success,
		Exception,
		GameProblems,
		ProfileProblems,
		PatchProblems
	};
	
private:
	struct sCommand{
		Commands command;
	};
	
	struct sEvent{
		std::string event;
		eResultCodes resultCode;
		std::string error;
	};
	
	deLogger::Ref pEngineLogger, pEngineLoggerDebug;
	
	dewlLoggerJS::Ref pLoggerJS;
	
	decString pDelgaPath, pProfileName;
	decUnicodeArgumentList pGameArgs;
	decString pCanvasId;
	
	delGame::Ref pGame;
	delGameProfile::Ref pProfile;
	delGPModuleList *pModuleParameters;
	decUuid pPatchIdentifier;
	bool pHasPatchIdentifier, pPatchesValid;
	delGameRunParams pRunParams;
	decString pPatchProblems;
	
	pthread_t pThreadLauncher;
	pthread_mutex_t pMutexLauncher;
	pthread_cond_t pConditionLauncher;
	
	std::vector<sCommand> pCommands;
	
	static std::vector<sEvent> pEvents;
	static pthread_mutex_t pMutexEvents;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create launcher. */
	dewlLauncher();
	
	/** Clean up launcher. */
	~dewlLauncher() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Engine logger. */
	inline const deLogger::Ref &GetEngineLogger() const{ return pEngineLogger; }
	
	/** Engine logger debug. */
	inline const deLogger::Ref &GetEngineLoggerDebug() const{ return pEngineLoggerDebug; }
	
	/** Delga path. */
	std::string GetDelgaPath() const{ return pDelgaPath.GetString(); }
	void SetDelgaPath(const std::string &path){ pDelgaPath = path.c_str(); }
	
	/** Profile name. */
	std::string GetProfileName() const{ return pProfileName.GetString(); }
	void SetProfileName(const std::string &name){ pProfileName = name.c_str(); }
	
	/** Canvas id. */
	std::string GetCanvasId() const{ return pCanvasId.GetString(); }
	void SetCanvasId(const std::string &canvasId);
	
	/** Run game. */
	void RunGame();
	
	
	
	/** Remove all module parameters. */
	void RemoveAllModuleParameters();
	
	/** Add module parameter. */
	void AddModuleParameter(const std::string &module,
		const std::string &parameter, const std::string &value);
	
	/** Game problems. */
	std::vector<std::string> GetGameProblems();
	
	/** Located profile name or empty string. */
	std::string GetLocatedProfileName();
	
	/** Profile problems. */
	std::vector<std::string> GetProfileProblems();
	
	/** Patches problems. */
	std::vector<std::string> GetPatchProblems();
	
	/** Clean up. */
	void CleanUp();
	
	/** Main thread update. */
	static void MainThreadUpdate();
	/*@}*/
	
	
private:
	void pInitLogger();
	void pGameProblems(std::vector<std::string> &problems);
	void pProfileProblems(std::vector<std::string> &problems, const delGameProfile &profile);
	void pModuleProblem(std::vector<std::string> &problems, const char *moduleName,
		deModuleSystem::eModuleTypes moduleType);
	void pUpdateRunArguments();
	void pApplyCustomModuleParameters();
	void pUpdateEngineInstanceParams();
	
	void pFSMkdirAbsent(const char *path);
	void pFSSymlink(const char *pathTarget, const char *pathLink);
	
	static void *pLauncherThreadMainGlue(void*);
	void pLauncherThreadMain();
	void pCommandRunGame();
	
	void pPrepare();
	void pLocateGame();
	void pLocateProfile();
	void pLocatePatches();
	void pPrepareRunParameters();
	void pStartGame();
	
	static void pDispatchEvent(const std::string &event,
		eResultCodes resultCode = eResultCodes::Success, const char *error = "");
	static void pDispatchEvent(const std::string &event, const deException &exception);
};

#ifdef DEWL_LAUNCHER_REGISTER_BINDINGS
EMSCRIPTEN_BINDINGS(dragengine_web_launcher) {
	es::class_<dewlLauncher>("dewlLauncher")
		.constructor<>()
		.property("delgaPath", &dewlLauncher::GetDelgaPath, &dewlLauncher::SetDelgaPath)
		.property("profileName", &dewlLauncher::GetProfileName, &dewlLauncher::SetProfileName)
		.property("canvasId", &dewlLauncher::GetCanvasId, &dewlLauncher::SetCanvasId)
		.function("runGame", &dewlLauncher::RunGame)
		.function("getGameProblems", &dewlLauncher::GetGameProblems)
		.function("getLocatedProfileName", &dewlLauncher::GetLocatedProfileName)
		.function("getProfileProblems", &dewlLauncher::GetProfileProblems)
		.function("getPatchProblems", &dewlLauncher::GetPatchProblems)
		;
	
	es::register_vector<std::string>("vector<string>");
	es::register_optional<std::string>();
}
#endif

#endif
