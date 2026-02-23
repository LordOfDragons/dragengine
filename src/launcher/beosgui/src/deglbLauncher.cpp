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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <Alert.h>
#include <Autolock.h>

#include "deglbLauncher.h"
#include "config/deglbConfiguration.h"
#include "config/deglbConfigWindow.h"
#include "game/deglbGameIcon.h"
#include "gui/deglbWindowMain.h"

#include <delauncher/engine/delEngineInstanceThreaded.h>
#include <delauncher/game/delGameRunParams.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/utils/decUuid.h>


// Class deglbLauncher
///////////////////////

// Constructor, destructor
////////////////////////////

deglbLauncher::deglbLauncher(deglbWindowMain *windowMain, int argc, char **argv) :
delLauncher("LauncherGui", "delauncher-gui-engine"),
pSignalHandler(*this),
pConfiguration(*this),
pWindowMain(windowMain),
pCmdLineGame(nullptr),
pCmdLineQuitNow(false)
{
	if(!windowMain){
		DETHROW(deeInvalidParam);
	}
	
	AddFileLogger("delauncher-gui");
	
	int i;
	for(i=1; i<argc; i++){
		pArguments.AddArgument(decUnicodeString::NewFromUTF8(argv[i]));
	}
	pParseArguments();
	
	if(pCmdLineQuitNow){
		return;
	}
	
	pConfiguration.LoadConfiguration();
	
	Prepare();
}

deglbLauncher::~deglbLauncher(){
	pConfiguration.SaveConfiguration();
}



// Management
///////////////

bool deglbLauncher::HasCommandLineRunGame() const{
	return !pRunGame.IsEmpty();
}

bool deglbLauncher::RunCommandLineGame(){
	if(!HasCommandLineRunGame() || pRunGame.IsEmpty()){
		return true;
	}
	
	pCmdLineGame = nullptr;
	
	delGame::Ref game;
	
	if(pRunGame.EndsWith(".delga") || pRunGame.EndsWith(".degame")){
		if(!decPath::IsNativePathAbsolute(pRunGame)){
			decPath path(decPath::CreateWorkingDirectory());
			path.AddNativePath(pRunGame);
			pRunGame = path.GetPathNative();
		}
		
		GetLogger()->LogInfoFormat(GetLogSource(), "Run Game: '%s'", pRunGame.GetString());
		
		delGame::List list;
		
		try{
			const delEngineInstance::Ref instance(GetEngineInstanceFactory().
				CreateEngineInstance(*this, GetEngine().GetLogFile()));
			
			instance->StartEngine();
			instance->LoadModules();
			
			GetGameManager().LoadGameFromDisk(instance, pRunGame, list);
			
		}catch(const deException &e){
			pWindowMain->DisplayException(e);
			return false;
		}
		
		if(list.GetCount() == 0){
			decString message;
			message.Format("No game definition found: %s", pRunGame.GetString());
			GetLogger()->LogInfo(GetLogSource(), message);
			BAlert alert("Run Game", message.GetString(), "OK");
			alert.Go();
			return false;
		}
		
		pCmdLineGame = list.GetAt(0);
		
		if(!GetGameManager().GetGames().Has(pCmdLineGame)){
			pCmdLineGame->LoadConfig();
		}
		
		pCmdLineGame->VerifyRequirements();
		game = pCmdLineGame;
		
	}else{
		try{
			game = GetGameManager().GetGames().FindWithId(decUuid(pRunGame.GetString(), false));
		}catch(const deException &){
		}
		
		if(!game){
			const delGame::List matching(GetGameManager().GetGames().CollectWithAliasId(pRunGame));
			
			if(matching.GetCount() == 1){
				game = matching.GetAt(0);
				
			}else if(matching.GetCount() > 1){
				GetLogger()->LogInfoFormat(GetLogSource(),
					"More than one game matching '%s'.", pRunGame.GetString());
				BAlert alert("Run Game", "More than one game matching the specified identifier.", "OK");
				alert.Go();
				return false;
			}
		}
	}
	
	if(!game){
		decString message;
		message.Format("Game not found: %s", pRunGame.GetString());
		BAlert alert("Run Game", message.GetString(), "OK");
		alert.Go();
		return false;
	}
	
	if(game->GetCanRun()){
		delGameProfile *profile = game->GetProfileToUse();
		
		if(!pRunProfileName.IsEmpty()){
			profile = GetGameManager().GetProfiles().FindNamed(pRunProfileName);
			if(!profile){
				decString message;
				message.Format("No profile found named '%s'", pRunProfileName.GetString());
				BAlert alert("Run Game", message.GetString(), "OK");
				alert.Go();
				return false;
			}
		}
		
		if(profile->GetValid()){
			delGameRunParams runParams;
			runParams.SetGameProfile(profile);
			
			decString error;
			if(!runParams.FindPatches(*game, game->GetUseLatestPatch(),
			game->GetUseCustomPatch(), error)){
				BAlert alert("Can not run game", error.GetString(), "OK");
				alert.Go();
				return false;
			}
			
			decString arguments(profile->GetRunArguments());
			if(!profile->GetReplaceRunArguments()){
				arguments = game->GetRunArguments() + " " + arguments;
			}
			
			const int argCount = pRunGameArgList.GetArgumentCount();
			int i;
			for(i=0; i<argCount; i++){
				arguments.Append(" ");
				decString argument(pRunGameArgList.GetArgumentAt(i)->ToUTF8());
				if(argument.Find(' ') != -1){
					argument = decString("\"") + argument + decString("\"");
				}
				arguments.Append(argument);
			}
			
			runParams.SetRunArguments(arguments);
			runParams.SetFullScreen(profile->GetFullScreen());
			runParams.SetWidth(profile->GetWidth());
			runParams.SetHeight(profile->GetHeight());
			
			const decPoint windowSize(game->GetDisplayScaledWindowSize());
			if(windowSize != decPoint()){
				runParams.SetWidth(windowSize.x);
				runParams.SetHeight(windowSize.y);
				runParams.SetFullScreen(false);
			}
			
			game->StartGame(runParams);
		}
		
	}else if(!game->GetAllFormatsSupported()){
		BAlert alert("Can not run game",
			"One or more File Formats required by the game are not working.\n\n"
			"Try updating Drag[en]gine to the latest version", "OK");
		alert.Go();
		return false;
		
	}else{
		BAlert alert("Can not run game",
			"Game related properties are incorrect.\n\n"
			"Try updating Drag[en]gine to the latest version", "OK");
		alert.Go();
		return false;
	}
	
	return true;
}

void deglbLauncher::RunCommandLineGameStopCheck(){
	if(!HasCommandLineRunGame()){
		return;
	}
	
	if(!pCmdLineGame){
		BAutolock autolock(pWindowMain);
		if(autolock.IsLocked()){
			pWindowMain->PostMessage(B_QUIT_REQUESTED);
		}
		return;
	}
	
	if(!GetGameManager().GetGames().Has(pCmdLineGame)){
		pCmdLineGame->PulseChecking();
	}
	
	if(pCmdLineGame->IsRunning()){
		return;
	}
	
	BAutolock autolock(pWindowMain);
	if(autolock.IsLocked()){
		pWindowMain->PostMessage(B_QUIT_REQUESTED);
	}
}



void deglbLauncher::PulseChecking(){
	GetGameManager().PulseChecking();
}



delGameIcon::Ref deglbLauncher::CreateGameIcon(int size, const char *path){
	return deglbGameIcon::Ref::New(size, path);
}



// Private Functions
//////////////////////

void deglbLauncher::pParseArguments(){
	const int argumentCount = pArguments.GetArgumentCount();
	int argumentIndex = 0;
	
	if(argumentCount > 0){
		const decString argument(pArguments.GetArgumentAt(argumentIndex)->ToUTF8());
		if(argument == "--version"){
			printf("%s", DE_VERSION);
			pCmdLineQuitNow = true;
			return;
		}
	}
	
	GetLogger()->LogInfo(GetLogSource(), "Command line arguments:");
	for(argumentIndex=0; argumentIndex<argumentCount; argumentIndex++){
		GetLogger()->LogInfoFormat(GetLogSource(), "- '%s'",
			pArguments.GetArgumentAt(argumentIndex)->ToUTF8().GetString());
	}
	
	argumentIndex = 0;
	
	while(argumentIndex < argumentCount){
		const decString argument(pArguments.GetArgumentAt(argumentIndex)->ToUTF8());
		
		if(argument == "--profile"){
			if(argumentCount - argumentIndex > 0){
				pRunProfileName = pArguments.GetArgumentAt(++argumentIndex)->ToUTF8();
			}else{
				GetLogger()->LogError(GetLogSource(), "Missing profile name after --profile");
				DETHROW_INFO(deeInvalidParam, "Missing profile name after --profile");
			}
		}else if(argument.GetLength() > 0){
			if(argument[0] == '-'){
				const int optionLen = argument.GetLength();
				int o;
				
				for(o=1; o<optionLen; o++){
					const int option = argument[o];
					
					if(option == 'p'){
						if(o == optionLen - 1){
							if(argumentCount - argumentIndex > 0){
								pRunProfileName = pArguments.GetArgumentAt(++argumentIndex)->ToUTF8();
							}else{
								GetLogger()->LogError(GetLogSource(), "Missing profile name after -p");
								DETHROW_INFO(deeInvalidParam, "Missing profile name after -p");
							}
						}else{
							decString message;
							message.Format("Invalid option '%s'", argument.GetString());
							GetLogger()->LogError(GetLogSource(), message);
							DETHROW_INFO(deeInvalidParam, message);
						}
					}else{
						decString message;
						message.Format("Unknown option -%c", (char)option);
						GetLogger()->LogError(GetLogSource(), message);
						DETHROW_INFO(deeInvalidParam, message);
					}
				}
			}else{
				break;
			}
		}else{
			break;
		}
		
		argumentIndex++;
	}
	
	if(argumentIndex < argumentCount){
		pRunGame = pArguments.GetArgumentAt(argumentIndex++)->ToUTF8();
	}
	
	while(argumentIndex < argumentCount){
		pRunGameArgList.AddArgument(*pArguments.GetArgumentAt(argumentIndex));
		argumentIndex++;
	}
}

decString deglbLauncher::pUrlDecode(const char *url){
	const char *walker = url;
	decString decoded;
	
	while(*walker){
		if(walker[0] == '%'
		&&  walker[1] && isxdigit(walker[1])
		&&  walker[2] && isxdigit(walker[2])){
			const char hex[3] = {walker[1], walker[2], 0};
			decoded.AppendCharacter((char)strtol(hex, nullptr, 16));
			walker += 3;
		}else{
			decoded.AppendCharacter(*(walker++));
		}
	}
	
	return decoded;
}
