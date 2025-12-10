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

#include "declLauncher.h"
#include "action/declRunGame.h"
#include "action/declActionGames.h"
#include "action/declActionDelga.h"
#include "action/declActionHelp.h"
#include "action/declActionPatches.h"
#include "action/declListProfiles.h"
#include "config/declConfiguration.h"
#include "logger/declLoggerFiltered.h"

#include <delauncher/engine/delEngineInstanceDirect.h>

#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/logger/deLoggerConsoleColor.h>
#include <dragengine/logger/deLoggerChain.h>
#include <dragengine/logger/deLoggerFile.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>



// Definitions
////////////////

#define LOGSOURCE "Launcher"



// Class declLauncher
///////////////////////

// Constructor, destructor
////////////////////////////

declLauncher::declLauncher() :
pConfiguration(NULL){
	// also log to "/logs/delauncher-console"
	AddFileLogger("delauncher-console");
	
	// set launcher to use direct engine instance
	SetEngineInstanceFactory(delEngineInstanceDirect::Factory::Ref::NewWith());
}

declLauncher::~declLauncher(){
	CleanUp();
}



// Management
///////////////

void declLauncher::AddArgument(const decUnicodeString &argument){
	pArgList.AddArgument(argument);
}

void declLauncher::PrintSyntax(){
	printf("Drag[en]gine Console Launcher.\n");
	printf("Written by Plüss Roland (roland@rptd.ch).\n");
	printf("Released under the GPL (http://www.gnu.org/licenses/gpl.html), 2020.\n");
	printf("\n");
	printf("Syntax:\n");
	printf("delauncher-console [<action> [<action-options>]]\n");
	printf("   <action> can be one or more of the following:\n");
	printf("      help       Print syntax of an action.\n");
	printf("      run        Run games.\n");
	printf("      delga      Manage DELGA files.\n");
	printf("      games      Manage games.\n");
	printf("      profiles   Manage profiles.\n");
	printf("      patches    Manage patches.\n");
	printf("\n");
}



void declLauncher::Init(){
	pConfiguration = new declConfiguration(*this);
	
	pInitLogger();
	
	pConfiguration->LoadConfiguration();
}

int declLauncher::Run(){
	if(pArgList.GetArgumentCount() < 1){
		PrintSyntax();
		return -1;
	}
	
	const decString actionName(pArgList.GetArgumentAt(0)->ToUTF8());
	
	if(actionName == "help"){
		declActionHelp(*this).Run();
		
	}else if(actionName == "version"){
		printf("%s", DE_VERSION);
		return 0;
		
	}else if(actionName == "run"){
		Init();
		declRunGame(*this).Run();
		
	}else if(actionName == "games"){
		Init();
		return declActionGames(*this).Run();
		
	}else if(actionName == "delga"){
		Init();
		return declActionDelga(*this).Run();
		
	}else if(actionName == "profiles"){
		Init();
		declListProfiles(*this).Run();
		
	}else if(actionName == "patches"){
		Init();
		declActionPatches(*this).Run();
		
	}else{
		GetLogger()->LogErrorFormat(LOGSOURCE, "Unknown action '%s'", actionName.GetString());
		PrintSyntax();
	}
	
	return 0;
}

void declLauncher::CleanUp(){
	if(pConfiguration){
		//pConfiguration->SaveConfiguration();
	}
	
	if(pConfiguration){
		delete pConfiguration;
		pConfiguration = NULL;
	}
}

decString declLauncher::ReadInput() const{
	decString input;
	while(true){
		const int character = fgetc(stdin);
		if(character == '\n' || character == EOF){
			break;
		}
		input.AppendCharacter(character);
	}
	return input;
}

bool declLauncher::ReadInputConfirm() const{
	const decString input(ReadInput());
	return input == "y" || input == "Y";
}

int declLauncher::ReadInputSelection() const{
	const decString input(ReadInput());
	if(input.IsEmpty()){
		return -1;
		
	}else{
		return input.ToInt();
	}
}



// Private Functions
//////////////////////

void declLauncher::pInitLogger(){
	// clear logger chain set up by the shared launcher. we want a custom one
	GetLogger()->RemoveAllLoggers();
	
	const declLoggerFiltered::Ref loggerLauncher(declLoggerFiltered::Ref::NewWith());
	GetLogger()->AddLogger(loggerLauncher);
	
	const deLoggerChain::Ref loggerLauncherError(deLoggerChain::Ref::NewWith());
	loggerLauncher->SetLoggerError(loggerLauncherError);
	
	const deLoggerChain::Ref loggerLauncherWarn(deLoggerChain::Ref::NewWith());
	loggerLauncher->SetLoggerWarning(loggerLauncherWarn);
	
	const deLoggerChain::Ref loggerLauncherInfo(deLoggerChain::Ref::NewWith());
	loggerLauncher->SetLoggerInfo(loggerLauncherInfo);
	
	declLoggerFiltered::Ref loggerEngine(declLoggerFiltered::Ref::NewWith());
	loggerEngine->SetLoggerError(loggerLauncherError);
	loggerEngine->SetLoggerWarning(loggerLauncherWarn);
	pEngineLogger = loggerEngine;
	
	const deLoggerChain::Ref loggerEngineDebug(deLoggerChain::Ref::NewWith());
	pEngineLoggerDebug = loggerEngineDebug;
	
	// console
	const deLoggerConsoleColor::Ref loggerConsole(deLoggerConsoleColor::Ref::NewWith());
	loggerLauncherError->AddLogger(loggerConsole);
	loggerEngineDebug->AddLogger(loggerConsole);
	
	// file
	const deLoggerFile::Ref loggerFile(deLoggerFile::Ref::NewWith(
		GetVFS()->OpenFileForWriting(decPath::CreatePathUnix("/logs/delauncher-console.log"))));
	
	loggerLauncherError->AddLogger(loggerFile);
	loggerLauncherWarn->AddLogger(loggerFile);
	loggerLauncherInfo->AddLogger(loggerFile);
	
	loggerEngine->SetLoggerInfo(loggerFile);
	loggerEngineDebug->AddLogger(loggerFile);
}
